/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#include "config_platform.h"
#include "generated\IX_CONNSERVER_CONSTS.h"

//*****************************************************************************
//Variables
//*****************************************************************************
Hashtable mConnections = new Hashtable();
int mConnNextHandle = 1;
ThreadPool mThreadPool = new ThreadPool();

#define NETSYSCALL(type) final synchronized type

#define HTTPS_NULL 0
#define HTTPS_SETUP 1
#define HTTPS_WRITING 2
#define HTTPS_FINISHING 3
#define HTTPS_FINISHED 4

//*****************************************************************************
//Classes
//*****************************************************************************

#if 1//defined(MA_IX_CONNSERVER)
public class MAConn {
	int state = 0;
	InputStream in;
	OutputStream out;
	private final MAHandle handle;

	MAConn(MAHandle h) {
		handle = h;
	}

	final void handleResult(int opType, int result) {
		MYASSERT((state & opType) != 0);
		state &= ~opType;
		DEBUG_TEMP("hR\n");

		int[] event = new int[4];
		EI_TYPE = EVENT_TYPE_CONN;
		EI_CONN_HANDLE = handle;
		EI_CONN_OPTYPE = opType;
		EI_CONN_RESULT = result;
		mCanvas.postEvent(event);
	}
	synchronized void close() throws IOException {
		if(in != null)
			in.close();
		if(out != null)
			out.close();
	}
}

public class MAStreamConn extends MAConn {
	StreamConnection conn;
	int httpState = HTTPS_NULL;
	boolean cancel = false;

	MAStreamConn(MAHandle h) {
		super(h);
	}

	synchronized final void close() throws IOException {
		cancel = true;
		super.close();
		if(conn != null)
			conn.close();
	}
}

#if defined(MA_SUPPORT_RECORD)
public class MARecordConn extends MAConn {
	Player player;
	RecordControl rc;
	BufferlessPipedOutputStream rcOut;
	boolean recording = false;

	MARecordConn(MAHandle h) {
		super(h);
	}

	synchronized final void close() throws IOException {
		if(recording)
			rc.stopRecord();
		player.close();
		super.close();
	}
}
#endif	//MA_SUPPORT_RECORD

public class MAServerConn extends MAConn {
	StreamConnectionNotifier notifier;

	MAServerConn(MAHandle h) {
		super(h);
	}

	synchronized final void close() throws IOException {
		super.close();
		if(notifier != null)
			notifier.close();
	}
}

#else	//stream connections only

#define MAStreamConn MAConn
public class MAConn {
	int state = 0;
	StreamConnection conn;
	InputStream in;
	OutputStream out;
	private final MAHandle handle;
	int httpState = HTTPS_NULL;
	boolean cancel = false;

	MAConn(int h) {
		handle = h;
	}

	final void handleResult(int opType, int result) {
		MYASSERT((state & opType) != 0);
		state &= ~opType;

		int[] event = new int[4];
		EI_TYPE = EVENT_TYPE_CONN;
		EI_CONN_HANDLE = handle;
		EI_CONN_OPTYPE = opType;
		EI_CONN_RESULT = result;
		mCanvas.postEvent(event);
	}

	synchronized final void close() throws IOException {
		cancel = true;
		if(in != null)
			in.close();
		if(out != null)
			out.close();
		if(conn != null)
			conn.close();
	}
}
#endif


class Connect implements Runnable {
	MAStreamConn mac;
	final String url;

	Connect(MAStreamConn m, String u) {
		mac = m;
		url = u;
	}

	public final void run() {
		boolean http = url.startsWith("http");
		int result = 1;
		try {
			StreamConnection conn = (StreamConnection)Connector.open(url,
				http ? Connector.READ : Connector.READ_WRITE, true);
			if(http) {
				HttpConnection httpConn = (HttpConnection)conn;
				result = httpConn.getResponseCode();
				if(result <= 0) {
					DUMP(result);
					result = CONNERR_PROTOCOL;
				}
#ifndef PHONE_RELEASE
				else {
					//dump headers
					int i=0;
					while(true) {
						String key = httpConn.getHeaderFieldKey(i);
						String value = httpConn.getHeaderField(i);
						DEBUG_ALWAYS(key + ": " + value + "\n");
						if(key == null && value == null)
							break;
						i++;
					}
				}
#endif
				mac.httpState = HTTPS_FINISHED;
			}
			synchronized(mac) {
				if(mac.cancel) {
					mac.handleResult(CONNOP_CONNECT, CONNERR_CANCELED);
					return;
				}
				if(conn == null) {
					mac.handleResult(CONNOP_CONNECT, CONNERR_GENERIC);
					return;
				}
				mac.conn = conn;
				mac.in = conn.openInputStream();
				mac.out = http ? null : conn.openOutputStream();
			}
		} catch(Exception e) {
			PRINT_STACK_TRACE;
#ifdef CONN_PANIC
			MAMidlet.self.programError("Conn error", e);
#else
			synchronized(mac) {
				if(mac.cancel) {
					mac.handleResult(CONNOP_CONNECT, CONNERR_CANCELED);
					return;
				}
				mac.handleResult(CONNOP_CONNECT, CONNERR_GENERIC);
			}
#endif
			return;
		}
		mac.handleResult(CONNOP_CONNECT, result);
	}
}


class Accept implements Runnable {
	MAServerConn mac;

	Accept(MAServerConn m) {
		mac = m;
	}

	private final void finish(int result) {
		mac.handleResult(CONNOP_ACCEPT, result);
	}

	public final void run() {
		StreamConnection newConn;
		InputStream in;
		OutputStream out;
		try {
			newConn = mac.notifier.acceptAndOpen();
			in = newConn.openInputStream();
			out = newConn.openOutputStream();
		} catch(InterruptedIOException e) {
			PRINT_STACK_TRACE;
			finish(CONNERR_CANCELED);
			return;
		} catch(IOException e) {
			PRINT_STACK_TRACE;
#ifdef CONN_PANIC
			MAMidlet.self.programError("Conn error", e);
#else
			finish(CONNERR_GENERIC);
#endif
			return;
		}
		synchronized(Syscall.this) {
			MAStreamConn mac = new MAStreamConn(mConnNextHandle);
			mac.conn = newConn;
			mac.in = in;
			mac.out = out;
			mConnections.put(new Integer(mConnNextHandle), mac);
			finish(mConnNextHandle++);
		}
	}
}

class ConnRead implements Runnable {
	MAConn mac;
	Address dst;
	int size;

	ConnRead(MAConn m, Address d, int s) {
		mac = m;
		dst = d;
		size = s;
	}

	public final void run() {
		//TODO: handle bug BluetoothRfcommInputStream?
		byte[] bytes = new byte[size];
		int res;
		try {
			//DEBUG_ALWAYS("ConnRead " + size + "\n");
			res = mac.in.read(bytes);	//if connect failed, mac still exists, but in is null.
		} catch(InterruptedIOException e) {
			PRINT_STACK_TRACE;
			mac.handleResult(CONNOP_READ, CONNERR_CANCELED);
			return;
		} catch(IOException e) {
			PRINT_STACK_TRACE;
#ifdef CONN_PANIC
			MAMidlet.self.programError("Conn error", e);
#else
			mac.handleResult(CONNOP_READ, CONNERR_GENERIC);
#endif
			return;
		}
		int result;
		if(res > 0) {
#ifdef OLD_MEMCPY
			try {
				writeMemStream(dst, new ByteArrayInputStream(bytes), res);
			} catch(IOException e) {
				PRINT_STACK_TRACE;
				BIG_PHAT_ERROR;
			}
#else
			writeMemBytes(dst, bytes, 0, res);
#endif
			result = res;
		} else if(res == -1) {
			result = CONNERR_CLOSED;
		} else {
			DEBUG_ALWAYS("Weird-ass ConnRead error "+res+"!\n");
			result = CONNERR_INTERNAL;
		}
		//DEBUG_ALWAYS("ConnReadRes " + result + "\n");
		mac.handleResult(CONNOP_READ, result);
	}
}


class ConnReadToData implements Runnable {
	MAConn mac;
	Binary bin;
	MAHandle handle;
	int offset, size;

	ConnReadToData(MAConn m, Binary b, MAHandle h, int o, int s) {
		mac = m;
		bin = b;
		handle = h;
		offset = o;
		size = s;
	}

	private final void finish(int result) {
		setResource(handle, bin);
		mac.handleResult(CONNOP_READ, result);
	}

	public final void run() {
		int res;
		try {
#ifdef SEGMENTED_DATA
			res = bin.read(mac.in, offset, size);
#else
			res = mac.in.read(bin.arr, offset, size);
#endif
		} catch(InterruptedIOException e) {
			PRINT_STACK_TRACE;
			finish(CONNERR_CANCELED);
			return;
		} catch(IOException e) {
			PRINT_STACK_TRACE;
#ifdef CONN_PANIC
			MAMidlet.self.programError("Conn error", e);
#else
			finish(CONNERR_GENERIC);
#endif
			return;
		}
		int result;
		if(res > 0) {
			result = res;
		} else if(res == -1) {
			result = CONNERR_CLOSED;
		} else {
			DEBUG_ALWAYS("Weird-ass ConnRead error "+res+"!\n");
			result = CONNERR_INTERNAL;
		}
		finish(result);
	}
}


class ConnWrite implements Runnable {
	MAConn mac;
	Address src;
	int size;

	ConnWrite(MAConn m, Address a, int s) {
		mac = m;
		src = a;
		size = s;
	}

	public final void run() {
		try {
#ifdef OLD_MEMCPY
			readMemBytes(mac.out, src, size);
#else
			readMemStream(mac.out, src, size);
#endif
		} catch(InterruptedIOException e) {
			PRINT_STACK_TRACE;
			mac.handleResult(CONNOP_WRITE, CONNERR_CANCELED);
			return;
		} catch(IOException e) {
			PRINT_STACK_TRACE;
#ifdef CONN_PANIC
			MAMidlet.self.programError("Conn error", e);
#else
			mac.handleResult(CONNOP_WRITE, CONNERR_GENERIC);
#endif
			return;
		}
		mac.handleResult(CONNOP_WRITE, 1);
	}
}


class ConnWriteFromData implements Runnable {
	MAConn mac;
	BinaryInterface bi;
	MAHandle handle;
	int offset, size;

	ConnWriteFromData(MAConn m, BinaryInterface b, MAHandle h, int o, int s) {
		mac = m;
		bi = b;
		handle = h;
		offset = o;
		size = s;
	}

	private final void finish(int result) {
		setResource(handle, bi);
		mac.handleResult(CONNOP_WRITE, result);
	}

	public final void run() {
		try {
			if(bi instanceof Binary) {
				Binary bin = (Binary)bi;
#ifdef SEGMENTED_DATA
				bin.write(mac.out, offset, size);
#else
				mac.out.write(bin.arr, offset, size);
#endif
			} else {
				byte[] buffer = new byte[size];
				DataInputStream dis = new DataInputStream(bi.getInputStream());
				dis.readFully(buffer);
				dis.close();
				mac.out.write(buffer);
			}
		} catch(InterruptedIOException e) {
			PRINT_STACK_TRACE;
			finish(CONNERR_CANCELED);
			return;
		} catch(IOException e) {
			PRINT_STACK_TRACE;
#ifdef CONN_PANIC
			MAMidlet.self.programError("Conn error", e);
#else
			finish(CONNERR_GENERIC);
#endif
			return;
		}
		finish(1);
	}
}


class HttpFinish implements Runnable {
	MAStreamConn mac;

	HttpFinish(MAStreamConn m) {
		mac = m;
	}

	public final void run() {
		HttpConnection http = (HttpConnection)mac.conn;
		try {
			if(mac.out != null) {
				mac.out.close();
				mac.out = null;
			}
			int result = http.getResponseCode();
			if(result <= 0) {
				DUMP(result);
				result = CONNERR_PROTOCOL;
			} else {
#ifndef PHONE_RELEASE
				//dump headers
				int i=0;
				while(true) {
					String key = http.getHeaderFieldKey(i);
					String value = http.getHeaderField(i);
					DEBUG_ALWAYS(key + ": " + value + "\n");
					if(key == null && value == null)
						break;
					i++;
				}
#endif
				mac.httpState = HTTPS_FINISHED;
				mac.in = http.openInputStream();
			}
			mac.handleResult(CONNOP_FINISH, result);
		} catch(InterruptedIOException e) {
			PRINT_STACK_TRACE;
			mac.handleResult(CONNOP_FINISH, CONNERR_CANCELED);
		} catch(IOException e) {
			PRINT_STACK_TRACE;
#ifdef CONN_PANIC
			MAMidlet.self.programError("Conn error", e);
#else
			mac.handleResult(CONNOP_FINISH, CONNERR_GENERIC);
#endif
		}
	}
}
