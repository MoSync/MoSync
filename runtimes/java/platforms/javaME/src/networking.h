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

//*****************************************************************************
//Variables
//*****************************************************************************
Hashtable mConnections = new Hashtable();
int mConnNextHandle = 1;
ThreadPool mThreadPool = new ThreadPool();

#define NETSYSCALL(type) final synchronized type

#define CATCH(ExceptionClass, function) try { function; } catch(ExceptionClass e) {\
	PRINT_STACK_TRACE; }

#define HTTPS_NULL 0
#define HTTPS_SETUP 1
#define HTTPS_WRITING 2
#define HTTPS_FINISHING 3
#define HTTPS_FINISHED 4

//*****************************************************************************
//Classes
//*****************************************************************************

#if 1//defined(MA_IX_CONNSERVER)
public abstract class MAConn {
	int state = 0;
	private final MAHandle handle;
	Connection baseConn;
	boolean cancel = false;

	MAConn(MAHandle h) {
		handle = h;
	}

	final void handleResult(int opType, int result) {
		MYASSERT((state & opType) != 0);
		state &= ~opType;

		//DEBUG_TEMP("handleResult("+opType+", "+result+")\n");
		int[] event = new int[4];
		EI_TYPE = EVENT_TYPE_CONN;
		EI_CONN_HANDLE = handle;
		EI_CONN_OPTYPE = opType;
		EI_CONN_RESULT = result;
		mCanvas.postEvent(event);
	}
	synchronized void close() {
		cancel = true;
		if(baseConn != null)
			CATCH(IOException, baseConn.close());
	}
	abstract void prepareRead();
	abstract int recv(byte[] bytes, int offset, int size) throws IOException;
	abstract void write(byte[] bytes, int offset, int size) throws IOException;
}

public class MAStreamConn extends MAConn {
	InputStream in;
	OutputStream out;
	StreamConnection conn;
	int httpState = HTTPS_NULL;

	MAStreamConn(MAHandle h) {
		super(h);
	}

	synchronized final void close() {
		cancel = true;
		if(in != null)
			CATCH(IOException, in.close());
		if(out != null)
			CATCH(IOException, out.close());
		super.close();
	}

	final void prepareRead() {
		MYASSERT(in != null);
	}
	final int recv(byte[] bytes, int offset, int size) throws IOException {
		return Syscall.recv(this, bytes, offset, size);
	}
	final void write(byte[] bytes, int offset, int size) throws IOException {
		Syscall.chunkWrite(out, bytes, offset, size);
		out.flush();
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

	synchronized final void close() {
		if(recording)
			rc.stopRecord();
		CATCH(IOException, player.close());
		super.close();
	}
}
#endif	//MA_SUPPORT_RECORD

public class MAServerConn extends MAConn {
	StreamConnectionNotifier notifier;

	MAServerConn(MAHandle h) {
		super(h);
	}
	final void prepareRead() {
		BIG_PHAT_ERROR;
	}
	final int recv(byte[] bytes, int offset, int size) throws IOException {
		BIG_PHAT_ERROR;
	}
	final void write(byte[] bytes, int offset, int size) throws IOException {
		BIG_PHAT_ERROR;
	}
}

public class MADatagramConn extends MAConn {
	DatagramConnection dConn;

	MADatagramConn(MAHandle h) {
		super(h);
	}
	final void prepareRead() {
	}
	final int recv(byte[] bytes, int offset, int size) throws IOException {
		Datagram d = dConn.newDatagram(bytes, size);
		d.setData(bytes, offset, size);
		dConn.receive(d);
		return d.getLength();
	}

	final void write(byte[] bytes, int offset, int size) throws IOException {
		Datagram d = dConn.newDatagram(bytes, size);
		d.setData(bytes, offset, size);
		dConn.send(d);
	}
}

public class MAUdpConn extends MAConn {
	UDPDatagramConnection dConn;
	Address src;
	String dst;

	MAUdpConn(MAHandle h) {
		super(h);
	}
	final void prepareRead() {
	}
	final int recv(byte[] bytes, int offset, int size) throws IOException {
		INIT_MEMDS;
		Datagram d = dConn.newDatagram(bytes, size);
		d.setData(bytes, offset, size);
		dConn.receive(d);
		String a = d.getAddress();
		WINT(src, CONN_FAMILY_INET4);
		// convert textual ip address to int.
		int c = a.indexOf(':', 11);
		int host = parseIpv4(a, 11, c);
		int port = Integer.parseInt(a.substring(c+1));
		WINT(src+4, host);
		WINT(src+8, port);
		return d.getLength();
	}

	final void write(byte[] bytes, int offset, int size) throws IOException {
		Datagram d = dConn.newDatagram(bytes, size);
		d.setData(bytes, offset, size);
		d.setAddress(dst);
		dConn.send(d);
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

	synchronized final void close() {
		cancel = true;
		if(in != null)
			CATCH(IOException, in.close());
		if(out != null)
			CATCH(IOException, out.close());
		if(conn != null)
			CATCH(IOException, conn.close());
	}
}
#endif

static class DatagramConnect implements Runnable {
	MADatagramConn mac;
	final String url;

	DatagramConnect(MADatagramConn m, String u) {
		mac = m;
		url = u;
	}

	public final void run() {
		try {
			mac.baseConn = Connector.open(url);
			synchronized(mac) {
				if(mac.cancel) {
					mac.handleResult(CONNOP_CONNECT, CONNERR_CANCELED);
					return;
				}
				if(mac.baseConn == null) {
					mac.handleResult(CONNOP_CONNECT, CONNERR_GENERIC);
					return;
				}
				mac.dConn = (DatagramConnection)mac.baseConn;
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
		mac.handleResult(CONNOP_CONNECT, 1);
	}
}

static class Connect implements Runnable {
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
#if MA_PROF_BLACKBERRY_VERSION == 4
				String url = this.url+";interface=wifi";
#endif
			StreamConnection conn = (StreamConnection)
#ifdef BB_RIM_NETWORKING
				BlackBerryConnectionFactory.openConnection(url);
#else
				Connector.open(url, http ? Connector.READ : Connector.READ_WRITE, true);
#endif
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
				mac.baseConn = conn;
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
			mac.baseConn = newConn;
			mac.in = in;
			mac.out = out;
			mConnections.put(new Integer(mConnNextHandle), mac);
			finish(mConnNextHandle++);
		}
	}
}

// workaround for the case where the buffer isn't filled
public static final int recv(MAStreamConn mac, byte[] bytes, int offset, int size)
throws IOException
{
	InputStream in = mac.in;
	//DEBUG_TEMP("special recv\n");
#if (MA_PROF_BLACKBERRY_VERSION == 4)
	int res = in.read(bytes, offset, 1);
#else
	int res = in.read(bytes, offset, size);
#endif
	if(res == 1) {
		int len = size - 1;
		if(len > 0) {
			int ava = in.available();
			//DEBUG_TEMP("available: "+ava+"\n");
			if(ava <= 0)
				return res;
			if(len > ava)
				len = ava;
			//DEBUG_TEMP("len: "+len+"\n");
			res = in.read(bytes, offset + 1, len);
			//DEBUG_TEMP("res: "+res+"\n");
			if(res > 0)
				res++;
		}
	}
	return res;
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
			//DEBUG_TEMP("ConnRead " + size + "\n");
#if 0
			// if connect failed, mac still exists, but in is null.
			// this means that if you try to read from a broken connection,
			// you get a NullPointerException.
			res = mac.in.read(bytes);
#else
			res = mac.recv(bytes, 0, size);
#endif
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
#if 0
			res = mac.in.read(bin.arr, offset, size);
#else
			res = mac.recv(bin.arr, offset, size);
#endif	//0
#endif	//SEGMENTED_DATA
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
			mac.write(getMemBytes(src, size), 0, size);
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

// workaround a bug on some SonyEricsson phones,
// where a too big a write() would cause an IOException.
static void chunkWrite(OutputStream out, byte[] arr, int offset, int size) throws IOException {
	int pos = offset;
	int endPos = offset + size;
	while(pos != endPos) {
		// arbitrary: 32 kB.
		int chunkSize = MIN(endPos - pos, 32*1024);
		out.write(arr, pos, chunkSize);
		pos += chunkSize;
	}
}

static void chunkWrite(OutputStream out, byte[] arr) throws IOException {
	chunkWrite(out, arr, 0, arr.length);
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
				mac.write(bin, offset, size);
#else
				mac.write(bin.arr, offset, size);
#endif
			} else {
				byte[] buffer = new byte[size];
				DataInputStream dis = new DataInputStream(bi.getInputStream());
				dis.readFully(buffer);
				dis.close();
				mac.write(buffer, 0, size);
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


static class HttpFinish implements Runnable {
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

// -----------------------------------------
// Blackberry-specific additions for BB OS 5
// -----------------------------------------
#ifdef BB_RIM_NETWORKING

#define TRANSPORT net.rim.device.api.io.transport

static class BlackBerryConnectionFactory {
	static TRANSPORT.ConnectionFactory connFactory =
		new TRANSPORT.ConnectionFactory();
	static int[] preferredTransports = {
		TRANSPORT.TransportInfo.TRANSPORT_TCP_WIFI,
		TRANSPORT.TransportInfo.TRANSPORT_WAP2,
		TRANSPORT.TransportInfo.TRANSPORT_TCP_CELLULAR,
		TRANSPORT.TransportInfo.TRANSPORT_MDS
	};
	static {
		connFactory.setPreferredTransportTypes(preferredTransports);
		connFactory.setConnectionMode(TRANSPORT.ConnectionFactory.ACCESS_READ_WRITE);
	};

	static Connection openConnection(String url) throws IOException {
		TRANSPORT.ConnectionDescriptor connDesc = connFactory.getConnection(url);
		if(null == connDesc) {
			throw new IOException("No path to destination url");
		}
		return connDesc.getConnection();
	}
};
#endif	//BB_RIM_NETWORKING
