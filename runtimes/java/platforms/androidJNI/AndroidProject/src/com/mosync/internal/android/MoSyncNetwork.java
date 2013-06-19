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

/*
 How the connection state bit flags work

 The purpuse of these flags are to ensure that
 there is only one IO-operation of the same type
 going on at the same time.

 IO is asynchronous, and therefore we need to guard
 operations to prevent the programmer from starting
 a second operation while one is in progress.

 The bit flags are held in ConnectionObject.mState

 There are methods in ConnectionObject that are used
 to enter an IO state. These methods panic if the state
 is already ongoing. Do not access mState directly.

 We call the enterState* methods as late as possible
 in each syscall, to to prevent bit flags from being
 set before we know that the initiation of the syscall
 has not failed.
*/

package com.mosync.internal.android;

import static com.mosync.internal.android.MoSyncHelpers.DebugIsOn;
import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;
import static com.mosync.internal.android.MoSyncHelpers.MYASSERT;
import static com.mosync.internal.android.MoSyncHelpers.SYSLOG;

import static com.mosync.internal.generated.MAAPI_consts.*;

import java.io.IOException;
import java.io.InputStream;
import java.io.InterruptedIOException;
import java.io.OutputStream;
import java.lang.reflect.Method;
import java.net.HttpURLConnection;
import java.net.InetAddress;
import java.net.MalformedURLException;
import java.net.Socket;
import java.net.DatagramSocket;
import java.net.DatagramPacket;
import java.net.InetSocketAddress;
import java.net.URL;
import java.net.URLConnection;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.UUID;

import javax.net.SocketFactory;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.net.SSLCertificateSocketFactory;
import android.os.Looper;
import android.util.Log;

public class MoSyncNetwork
{
	static final int CONNOP_CONNECT_SUCCESS = 1;

	static final int SSL_HANDSHAKE_TIMEOUT_MILLIS = 5000;

	/**
	 * The MoSync thread object.
	 */
	public MoSyncThread mMoSyncThread;

	/**
	 * Hashtable is used to keep all the open connections.
	 */
	private Hashtable<Integer, ConnectionObject> mConnectionTable =
		new Hashtable<Integer, ConnectionObject>();

	/**
	 * Connection handle counter.
	 */
	private int mConnectionHandleCounter = 1;

	/**
	 * The number of currently open connections, should not exceed CONN_MAX.
	 */
	private int mNumberOfOpenConnections = 0;

	/**
	 * Thread pool for connections.
	 */
	public static ThreadPool sConnectionThreadPool = new ThreadPool();

	/**
	 * Constructor.
	 * @param thread The MoSync thread.
	 */
	public MoSyncNetwork(MoSyncThread thread)
	{
		mMoSyncThread = thread;

		// This fixes a bug in HttpURLConnection on Android versions < 2.3.
		// For more info, see:
		// http://jira.mosync.com/browse/MOSYNC-2050
		// http://stackoverflow.com/questions/1440957/httpurlconnection-getresponsecode-returns-1-on-second-invocation
		// http://code.google.com/p/android/issues/detail?id=7786
		System.setProperty("http.keepAlive", "false");
	}

	/**
	 * @return The Activity object.
	 */
	public Activity getActivity()
	{
		return mMoSyncThread.getActivity();
	}

	/**
	 * @return The MoSync Bluetooth object.
	 */
	public MoSyncBluetooth getMoSyncBluetooth()
	{
		return mMoSyncThread.mMoSyncBluetooth;
	}

	/**
	 * Called from MoSyncThread when Bluetooth is turned off.
	 * Here we close Bluetooth connections.
	 */
	public void bluetoothTurnedOff()
	{
		synchronized (mConnectionTable)
		{
			// Close Bluetooth connections.
			for (ConnectionObject connObj : mConnectionTable.values())
			{
				try
				{
					connObj.bluetoothTurnedOff();
				}
				catch (IOException ex)
				{
					ex.printStackTrace();
				}
			}
		}
	}

	/**
	 * @brief closes all the available connections
	 * Iterates through the map of available connections
	 * and closes them
	 */
	public synchronized void killAllConnections()
	{
		if(mConnectionTable.isEmpty())
			return;

		Enumeration<Integer> keys = mConnectionTable.keys();

		while( keys.hasMoreElements() )
		{
			ConnectionObject connectionObject =
				mConnectionTable.get( keys.nextElement() );

			try
			{
				connectionObject.closeConnection(true);
			}
			catch(Exception e)
			{
				Log.i("MOSYNC: Exception in killAllConnections", e.toString());
				e.printStackTrace();
			}
		}
	}

	/**
	 * Post a event to the MoSync event queue.
	 */
	public void postEvent(int[] event)
	{
		mMoSyncThread.postEvent(event);
	}

	/**
	 * Initiates a connection and if successful, adds the new connection to the
	 * table of open connections.
	 *
	 * @param url	The URL to which the connection should be established.
	 *				URLs that starts with "socket://" need to specify port number.
	 *				URLs that starts with "btspp://" connect to a bluetooth port.
	 *
	 * @return CONNERR_URL if the URL wasn't correct.
	 *         CONNERR_GENERIC on other errors.
	 *         > 0 on success. This number is the handle to the opened connection.
	 */
	int maConnect(final String url)
	{
		//Log.i("MoSyncSyscall", "maConnect url: " + url);

		if (isMaxNumberOfConnectionsReached())
		{
			return CONNERR_MAX;
		}

		// If this is a Bluetooth url, Bluetooth must be available and enabled.
		if (url.startsWith("btspp://"))
		{
			if (mMoSyncThread.isBluetoothApiAvailable())
			{
				getMoSyncBluetooth().panicIfBluetoothPermissionsAreNotSet();

				// If we cannot get the Bluetooth adapter, we return an error
				// (currently, this also occurs if Bluetooth is not turned on).
				if (null == getMoSyncBluetooth().getBluetoothAdapter())
				{
					return CONNERR_UNAVAILABLE;
				}
			}
			else
			{
				return CONNERR_UNAVAILABLE;
			}
		}

		int connHandle = getNextConnectionHandle();

		try
		{
			ConnectionObject connObj;

			if (url.startsWith("socket://"))
			{
				connObj = new SocketConnectionObject(this)
					.create(url, connHandle);
			}
			else if (url.startsWith("datagram://"))
			{
				connObj = new DatagramConnectionObject(this)
					.create(url, connHandle);
			}
			else if (url.startsWith("ssl://"))
			{
				connObj = new SecureSocketConnectionObject(this)
					.create(url, connHandle);
			}
			else if (url.startsWith("btspp://"))
			{
				if (mMoSyncThread.isBluetoothApiAvailable())
				{
					connObj = createBluetoothConnectionObject(url, connHandle);
				}
				else
				{
					throw new Exception("Bluetooth not supported!");
				}
			}
			else
			{
				connObj = new UrlConnectionObject(this)
					.create(url, connHandle);
			}

			// The connect method is the main entry point for the
			// connect operation. Here the connect state is set and
			// the connect thread is started here (except for server
			// connections, which do not use any connect thread).
			int result = connObj.connect();
			if (result != RES_OK)
			{
				return CONNERR_GENERIC;
			}
		}
		catch (MalformedURLException ex)
		{
			//Log.i("MoSyncSyscall", "maConnect MalformedURLException: " + ex);
			ex.printStackTrace();
			return CONNERR_URL;
		}
		catch (IOException ex)
		{
			//Log.i("MoSyncSyscall", "maConnect IOException: " + ex);
			ex.printStackTrace();
			return CONNERR_GENERIC;
		}
		catch (Exception ex)
		{
			//Log.i("MoSyncSyscall", "maConnect Exception: " + ex);
			ex.printStackTrace();
			return CONNERR_GENERIC;
		}

		return connHandle;
	}

	/**
	 * Get the address of the connection.
	 */
	int maConnGetAddr(int connHandle, int addrPointer)
	{
		//Log.i("MoSyncSyscall", "maConnGetAddr");

		// Is this a request for a local address?
		if (HANDLE_LOCAL == connHandle)
		{
			return localConnectionAddress(addrPointer);
		}

		// Get the connection object.
		ConnectionObject connObj = getConnectionObject(connHandle);

		// Fill in the connection struct.
		return connObj.getAddr(addrPointer);
	}

	/**
	 * Close a connection.
	 */
	void maConnClose(int connHandle)
	{
		try
		{
			//Log.i("MoSyncSyscall", "maConnClose: " + connHandle);

			ConnectionObject connObj = getConnectionObject(connHandle);

			// Remove before close so that we are certain the connection
			// is not in the table any more. Was after close previously.
			// Keep an eye of this, if it will cause problems.
			removeConnectionObject(connObj);

			// Pass true to indicate a controlled cancel.
			connObj.closeConnection(true);
		}
		catch (IOException ex)
		{
			//Log.i("MoSyncSyscall", "maConnClose IOException: " + ex);
			ex.printStackTrace();
		}
	}

	void maConnRead(int connHandle, int dst, int size)
	{
		// TODO: Remove Log.i() printout when debugging is done.
		//Log.i("MoSyncSyscall",
		//	"maConnRead connHandle:" + connHandle +
		//	" dst:" + dst + " size:" + size);
		SYSLOG("maConnRead connHandle:" + connHandle +
			" dst:" + dst + " size:" + size);

		MYASSERT(size > 0);

		ConnectionObject connObj = getConnectionObject(connHandle);
		connObj.read(dst, size);
	}

	void maConnWrite(int connHandle, int src, int size)
	{
		// TODO: Remove Log.i() printout when debugging is done.
		//Log.i("MoSyncSyscall", "maConnWrite connHandle:" + connHandle +
		//	" src:" + src + " size:" + size);
		SYSLOG("maConnWrite connHandle:" + connHandle +
			" src:" + src + " size:" + size);

		MYASSERT(size > 0);

		ConnectionObject connObj = getConnectionObject(connHandle);
		connObj.write(src, size);
	}

	void maConnReadFrom(int connHandle, int dst, int size, int src)
	{
		SYSLOG("maConnRead connHandle:" + connHandle +
			" dst:" + dst + " size:" + size + " src:" + src);

		MYASSERT(size > 0);

		ConnectionObject connObj = getConnectionObject(connHandle);
		connObj.readFrom(dst, size, src);
	}

	void maConnWriteTo(int connHandle, int src, int size, int dst)
	{
		SYSLOG("maConnWrite connHandle:" + connHandle +
			" src:" + src + " size:" + size + " dst:" + dst);

		MYASSERT(size > 0);

		// set dp address/port
		int port;
		InetAddress addr;
		int family = readIntFromMemory(dst);
		try {
			if(family == CONN_FAMILY_INET4)
			{
				byte[] a = new byte[4];
				readBytesFromMemory(dst + 4, a);
				// byte array is in host order; gotta reverse it.
				byte b = a[0];
				a[0] = a[3];
				a[3] = b;
				b = a[1];
				a[1] = a[2];
				a[2] = b;
				addr = InetAddress.getByAddress(a);

				port = readIntFromMemory(dst + 8);
			}
			else if(family == CONN_FAMILY_INET6)
			{
				port = readIntFromMemory(dst + 4);
				byte[] a = new byte[16];
				readBytesFromMemory(dst + 8, a);
				addr = InetAddress.getByAddress(a);
			}
			else
			{
				Log.i("writeTo family", ""+family);
				throw new BigPhatError("writeTo family");
			}
		} catch(UnknownHostException e) {
			e.printStackTrace();
			throw new BigPhatError("UnknownHostException");
		}

		ConnectionObject connObj = getConnectionObject(connHandle);
		connObj.writeTo(src, size, addr, port);
	}

	void maConnReadToData(int connHandle, int data, int offset, int size)
	{
		// TODO: Remove Log.i() printout when debugging is done.
		//Log.i("@MoSyncSyscall", "maConnReadToData connHandle:" + connHandle +
		//	" dataHandle:" + data + " offset:" + offset + " size:" + size);
		SYSLOG("maConnReadToData connHandle:" + connHandle +
			" dataHandle:" + data + " size:" + size);

		MYASSERT(offset >= 0);
		MYASSERT(size > 0);
		MYASSERT(offset + size > 0);

		ConnectionObject connObj = getConnectionObject(connHandle);
		connObj.readToData(data, offset, size);
	}

	void maConnWriteFromData(
		int connHandle,
		int data,
		int offset,
		int size)
	{
		// TODO: Remove Log.i() printout when debugging is done.
		//Log.i("@MoSyncSyscall", "maConnWriteFromData connHandle:" + connHandle +
		//	" dataHandle:" + data + " offset:" + offset + " size:" + size);
		SYSLOG("maConnWriteFromData connHandle:" + connHandle +
			" dataHandle:" + data + " size:" + size);

		MYASSERT(offset >= 0);
		MYASSERT(size > 0);
		MYASSERT(offset + size > 0);

		ConnectionObject connObj = getConnectionObject(connHandle);
		connObj.writeFromData(data, offset, size);
	}

	int maHttpCreate(String url, int method)
	{
		//Log.i("MoSyncSyscall", "maHttpCreate");

		// Have we reached the max number of connections?
		if (isMaxNumberOfConnectionsReached())
		{
			//Log.i("MoSyncSyscall", "maHttpCreate CONNERR_MAX");
			return CONNERR_MAX;
		}

		int connHandle = getNextConnectionHandle();

		try
		{
			ConnectionObject connObj =
				new UrlConnectionObject(this)
					.createHttp(url, method, connHandle);
			addConnectionObject(connObj);
		}
		catch (MalformedURLException ex)
		{
			ex.printStackTrace();
			return CONNERR_URL;
		}
		catch (IOException ex)
		{
			ex.printStackTrace();
			return CONNERR_GENERIC;
		}
		catch (Exception ex)
		{
			ex.printStackTrace();
			return CONNERR_GENERIC;
		}

		//Log.i("MoSyncSyscall", "maHttpCreate returning handle: " + connHandle);

		return connHandle;
	}

	void maHttpSetRequestHeader(int connHandle, String key, String value)
	{
		//Log.i("MoSyncSyscall", "maHttpSetRequestHeader");

		ConnectionObject connObj = getConnectionObject(connHandle);
		connObj.setRequestProperty(key, value);
	}

	int maHttpGetResponseHeader(
		int connHandle,
		String key,
		int address,
		int bufSize)
	{
		//Log.i("MoSyncSyscall", "maHttpGetResponseHeader");

		ConnectionObject connObj = getConnectionObject(connHandle);
		return connObj.getResponseHeader(key, address, bufSize);
	}

	void maHttpFinish(int connHandle)
	{
		//Log.i("MoSyncSyscall", "maHttpFinish connHandle: " + connHandle);

		ConnectionObject connObj = getConnectionObject(connHandle);
		connObj.startFinishConnectThread();
	}

	int maAccept(int connHandle)
	{
		//Log.i("MoSyncSyscall", "maAccept connHandle = " + connHandle);

		// Find the connection object.
		ConnectionObject connObj = getConnectionObject(connHandle);

		// Verify that it is a server connection.
		MYASSERT(connObj.isServerConnection());

		// Start accept thread.
		return connObj.startAcceptThread();
	}

	// Helper functions, prefixed with "network". Some of them are
	// generally useful and could be renamed and move to some other file.

	int getNextConnectionHandle()
	{
		return mConnectionHandleCounter++;
	}

	boolean isMaxNumberOfConnectionsReached()
	{
		return mNumberOfOpenConnections >= CONN_MAX;
	}

	ConnectionObject getConnectionObject(int connHandle)
	{
		synchronized (mConnectionTable)
		{
			ConnectionObject connObj = mConnectionTable.get(connHandle);
			MYASSERT(connObj != null);
			return connObj;
		}
	}

	void addConnectionObject(ConnectionObject connObj)
	{
		synchronized (mConnectionTable)
		{
			++mNumberOfOpenConnections;
			//Log.i("@@@ networkAddConnectionObject",
			//	"Number of open connections: " + mNumberOfOpenConnections);
			mConnectionTable.put(connObj.mHandle, connObj);
		}
	}

	void removeConnectionObject(ConnectionObject connObj)
	{
		synchronized (mConnectionTable)
		{
			--mNumberOfOpenConnections;
			mConnectionTable.remove(connObj.mHandle);
		}
	}

	/**
	 * Copy a byte array to the given address in the system memory.
	 */
	void copyBytesToMemory(int address, byte[] bytes)
	{
		ByteBuffer buffer = mMoSyncThread.getMemorySlice(address, bytes.length);
		buffer.put(bytes);
	}

	/**
	 * Copy length bytes of a byte array starting at offset
	 * to the given address in the system memory.
	 */
	void copyBytesToMemory(int address, byte[] bytes, int offset, int length)
	{
		ByteBuffer buffer = mMoSyncThread.getMemorySlice(address, length);
		buffer.put(bytes, offset, length);
	}

	/**
	 * Read data from system memory.
	 */
	void readBytesFromMemory(int address, byte[] bytes)
	{
		ByteBuffer buffer = mMoSyncThread.getMemorySlice(address, bytes.length);
		buffer.get(bytes);
	}

	/**
	 * Copy an int value to the given address in the system memory.
	 */
	void copyIntToMemory(int address, int value)
	{
		ByteBuffer buffer = mMoSyncThread.getMemorySlice(address, 4);
		buffer.putInt(value);
	}

	/**
	 * Read the int value at the given address in the system memory.
	 */
	int readIntFromMemory(int address)
	{
		int value = mMoSyncThread.getMemorySlice(address, 4).getInt();

		return value;
	}

	/**
	 * Copy a String value to the given address in the system memory.
	 */
	void copyStringToMemory(int address, String str)
	{
		byte[] strAsBytes = str.getBytes();
		ByteBuffer buffer = mMoSyncThread.getMemorySlice(address, strAsBytes.length + 1);
		buffer.put(str.getBytes());
		buffer.put((byte)0); // Terminating null char.
	}

	/**
	 * Specialised method used to avoid code duplication.
	 * Writes data to the structure MAConnAddr/MAConnAddrBt.
	 * @param addrPointer The address un MoSync memory where to write data.
	 * @param port Port number.
	 * @param addr The address of the Bluetooth device.
	 * @return 1 on success, -1 on failure.
	 */
	int copyBluetoothAddressToMemory(int addrPointer, int port, byte[] addr)
	{
		// Check that the address is valid.
		if (null == addr || 6 != addr.length)
		{
			return CONNERR_INTERNAL;
		}

		// Write family.
		copyIntToMemory(addrPointer, CONN_FAMILY_BT);

		// Write port.
		copyIntToMemory(addrPointer + 4, port);

		// Write address.
		copyBytesToMemory(addrPointer + 8, addr);

		return 1;
	}

	/**
	 * Specialised method used to avoid code duplication.
	 */
	int copyInetAddressToMemory(int addrPointer, int port, byte[] addr)
	{
		// Check that the address is valid.
		if(addr == null) {
			Log.i("copyInetAddressToMemory", "null");
			return CONNERR_INTERNAL;
		}
		if(addr.length == 4) {
			// Write family.
			copyIntToMemory(addrPointer, CONN_FAMILY_INET4);

			// Write address.
			copyIntToMemory(addrPointer + 4, inet4AddressAsInt(addr));

			// Write port.
			copyIntToMemory(addrPointer + 8, port);
		} else if(addr.length == 16) {
			// Write family.
			copyIntToMemory(addrPointer, CONN_FAMILY_INET6);

			// Write port.
			copyIntToMemory(addrPointer + 4, port);

			// Write address.
			copyBytesToMemory(addrPointer + 8, addr);
		} else {
			Log.i("copyInetAddressToMemory", "len: "+addr.length);
			for(int i=0; i<addr.length; i++) {
				Log.i(""+i, ""+((int)addr[i] & 0xff));
			}
			return CONNERR_INTERNAL;
		}
		return 1;
	}

	/**
	 * Return an inet4 address in a byte array as an int.
	 */
	int inet4AddressAsInt(byte[] addr)
	{
		return
			(((int)addr[0] & 0xff) << 24) |
			(((int)addr[1] & 0xff) << 16) |
			(((int)addr[2] & 0xff) << 8) |
			((int)addr[3] & 0xff);
	}

	/**
	 * Fill in the local address in the addr structure.
	 */
	int localConnectionAddress(int addrPointer)
	{
		try
		{
			// Family can be inet4 or Bluetooth.
			int family = readIntFromMemory(addrPointer);
			if (CONN_FAMILY_INET4 == family)
			{
				InetAddress inetAddr = InetAddress.getLocalHost();

				if (null == inetAddr)
				{
					return CONNERR_GENERIC;
				}

				return copyInetAddressToMemory(
					addrPointer,
					0, // Port
					inetAddr.getAddress());
			}
			else if (CONN_FAMILY_BT == family)
			{
				if (mMoSyncThread.isBluetoothApiAvailable())
				{
					// Write address data. Use zero as port number
					// since we do not know the port.
					return copyBluetoothAddressToMemory(
						addrPointer,
						0, // Port
						mMoSyncThread.mMoSyncBluetooth.getDefaultAdapterAddressAsBytes());
				}
				else
				{
					return CONNERR_GENERIC;
				}
			}
			else
			{
				// Unknown family.
				return CONNERR_GENERIC;
			}
		}
		catch (Exception ex)
		{
			ex.printStackTrace();
			return CONNERR_GENERIC;
		}
	}

	/**
	 * Super class for connections.
	 */
	static abstract class ConnectionObject
	{
		// Values that are changed/read when communicating with
		// threads are marked volatile to prevent thread caching
		// of values.
		protected int mHandle;
		volatile protected boolean mCancelled;
		volatile protected int mState;
		protected MoSyncNetwork mMoSyncNetwork;
		volatile private OutputStream mOutputStream;
		volatile private InputStream mInputStream;

		public ConnectionObject(MoSyncNetwork network)
		{
			mState = 0;
			mCancelled = false;
			mMoSyncNetwork = network;
		}

		/**
		 * @return The MoSync Bluetooth object.
		 */
		protected MoSyncBluetooth getMoSyncBluetooth()
		{
			return mMoSyncNetwork.getMoSyncBluetooth();
		}

		void assertInputStream() {
			MYASSERT(getInputStream() != null);
		}

		void assertOutputStream() {
			MYASSERT(getOutputStream() != null);
		}

		// returns the number of bytes actually read.
		int read(byte[] data) throws IOException {
			return getInputStream().read(data);
		}

		void write(byte[] data) throws IOException {
			getOutputStream().write(data);
		}

		// returns the number of bytes actually read.
		int readFrom(byte[] data, int src) throws IOException {
			throw new BigPhatError("readFrom: Wrong connection type");
		}

		void writeTo(byte[] data, InetAddress addr, int port) throws IOException {
			throw new BigPhatError("writeTo: Wrong connection type");
		}

		/**
		 * Post CONNOP result event. Here the state flag bits for
		 * the event is cleared.
		 */
		public synchronized void postResultEvent(int opType, int result)
		{
			// Check that state is ongoing.
			MYASSERT((mState & opType) != 0);

			// Clear state bit.
			mState &= ~opType;

			int[] event = new int[4];
			event[0] = EVENT_TYPE_CONN;
			event[1] = mHandle;
			event[2] = opType;
			event[3] = result;

			mMoSyncNetwork.postEvent(event);
		}

		/**
		 * Closes the connection.
		 * @param softCancel true if the cancel is "soft", that is requested
		 * by the application. This is considered as a normal condition.
		 * false is closed as result of an error.
		 * @throws IOException
		 */
		public void closeConnection(boolean softCancel) throws IOException
		{
			mCancelled = softCancel;

			if (getInputStream() != null)
			{
				getInputStream().close();
			}
			if (getOutputStream() != null)
			{
				getOutputStream().close();
			}
		}

		/**
		 * Sent when Bluetooth is turned off.
		 * @throws IOException
		 */
		public void bluetoothTurnedOff() throws IOException
		{
			// This method is implemented in relevant subclasses.
		}

		/**
		 * Fill in the connection struct. This is done by subclasses.
		 * @return Status code, -1 on error.
		 */
		public int getAddr(int addrPointer)
		{
			return -1;
		}

		public void read(final int dst, final int size)
		{
			final int opType = CONNOP_READ;

			enterStateRead();
			// Must have an input stream.
			assertInputStream();

			sConnectionThreadPool.execute(new Runnable()
			{
				public final void run()
				{
					try
					{
						// Read data into a byte array.
						byte[] bytes = new byte[size];
						int result = read(bytes);

						if (result > 0)
						{
							// Store data in memory at address dst.
							mMoSyncNetwork.copyBytesToMemory(
								dst, bytes, 0, result);

							postResultEvent(opType, result);
						}
						else if (result == -1)
						{
							postResultEvent(opType, CONNERR_CLOSED);
						}
						else
						{
							postResultEvent(opType, CONNERR_INTERNAL);
						}
					}
					catch (Exception ex)
					{
						ex.printStackTrace();
						postResultEvent(opType, CONNERR_GENERIC);
					}
				}
			});
		}

		public void write(final int src, final int size)
		{
			final int opType = CONNOP_WRITE;

			enterStateWrite();
			// Must have an output stream.
			assertOutputStream();

			sConnectionThreadPool.execute(new Runnable()
			{
				public final void run()
				{
					try
					{
						// Byte array with data to write.
						byte[] data = new byte[size];

						// Get data to write.
						mMoSyncNetwork.readBytesFromMemory(src, data);

						// Write byte array to stream.
						write(data);

						// Post event.
						postResultEvent(opType, 1); // Success
					}
					catch (Exception ex)
					{
						ex.printStackTrace();
						postResultEvent(opType, CONNERR_GENERIC);
					}
				}
			});
		}

		public void readFrom(final int dst, final int size, final int src)
		{
			final int opType = CONNOP_READ;

			enterStateRead();
			// Must have an input stream.
			assertInputStream();

			sConnectionThreadPool.execute(new Runnable()
			{
				public final void run()
				{
					try
					{
						// Read data into a byte array.
						byte[] bytes = new byte[size];
						int result = readFrom(bytes, src);

						if (result > 0)
						{
							// Store data in memory at address dst.
							mMoSyncNetwork.copyBytesToMemory(
								dst, bytes, 0, result);

							postResultEvent(opType, result);
						}
						else if (result == -1)
						{
							postResultEvent(opType, CONNERR_CLOSED);
						}
						else
						{
							postResultEvent(opType, CONNERR_INTERNAL);
						}
					}
					catch (Exception ex)
					{
						ex.printStackTrace();
						postResultEvent(opType, CONNERR_GENERIC);
					}
				}
			});
		}

		public void writeTo(final int src, final int size, final InetAddress addr, final int port)
		{
			final int opType = CONNOP_WRITE;

			enterStateWrite();
			// Must have an output stream.
			assertOutputStream();

			sConnectionThreadPool.execute(new Runnable()
			{
				public final void run()
				{
					try
					{
						// Byte array with data to write.
						byte[] data = new byte[size];

						// Get data to write.
						mMoSyncNetwork.readBytesFromMemory(src, data);

						// Write byte array to stream.
						writeTo(data, addr, port);

						// Post event.
						postResultEvent(opType, 1); // Success
					}
					catch (Exception ex)
					{
						ex.printStackTrace();
						postResultEvent(opType, CONNERR_GENERIC);
					}
				}
			});
		}

		public void readToData(
			final int dataHandle,
			final int offset,
			final int size)
		{
			final int opType = CONNOP_READ;

			enterStateRead();

			// Must have an input stream.
			MYASSERT(getInputStream() != null);

			// Get the resource to read data to.
			final ByteBuffer byteBuffer =
				mMoSyncNetwork.mMoSyncThread.getBinaryResource(dataHandle);
			if (null == byteBuffer)
			{
				throw new BigPhatError(
					"ConnectionObject.readToData: invalid handle");
			}

			sConnectionThreadPool.execute(new Runnable()
			{
				public void run()
				{

					try
					{
						// Read data into a byte array.
						byte[] bytes = new byte[size];
						int result = read(bytes);

						// Result is number of bytes read if successful.
						if (result > 0)
						{
							byteBuffer.position(offset);
							byteBuffer.put(bytes, 0, result);
							postResultEvent(opType, result);
						}
						else if (result == -1)
						{
							postResultEvent(opType, CONNERR_CLOSED);
						}
						else
						{
							postResultEvent(opType, CONNERR_INTERNAL);
						}
					}
					catch (Exception ex)
					{
						ex.printStackTrace();
						postResultEvent(opType, CONNERR_GENERIC);
					}
				}
			});
		}

		public void writeFromData(
			final int dataHandle,
			final int offset,
			final int size)
		{
			final int opType = CONNOP_WRITE;

			enterStateWrite();

			// Must have an output stream.
			MYASSERT(getOutputStream() != null);

			final ByteBuffer byteBuffer =
				mMoSyncNetwork.mMoSyncThread.getBinaryResource(dataHandle);
			if (null == byteBuffer)
			{
				throw new BigPhatError(
					"ConnectionObject.writeFromData: invalid handle");
			}

			sConnectionThreadPool.execute(new Runnable()
			{
				public final void run()
				{
					try
					{
						// Byte array with data to write.
						byte[] data = new byte[size];

						// Get and write data.
						byteBuffer.position(offset);
						byteBuffer.get(data);
						write(data);

						// Post event.
						postResultEvent(opType, 1);
					}
					catch (Exception ex)
					{
						ex.printStackTrace();
						postResultEvent(opType, CONNERR_GENERIC);
					}
				}
			});
		}

		public int connect()
		{
			// Do this before starting the thread.
			enterStateConnect();

			startConnectThread();

			mMoSyncNetwork.addConnectionObject(this);

			return RES_OK; // Success;
		}

		public void setInputStream(InputStream stream)
		{
			mInputStream = stream;
		}

		public void setOutputStream(OutputStream stream)
		{
			mOutputStream = stream;
		}

		public InputStream getInputStream()
		{
			return mInputStream;
		}

		public OutputStream getOutputStream()
		{
			return mOutputStream;
		}

		public void startConnectThread()
		{
			// TODO: Panic. Should not be called.
		}

		public int startAcceptThread()
		{
			return -1;
		}

		public void startFinishConnectThread()
		{
			// TODO: Panic. Should not be called.
		}

		public boolean isServerConnection()
		{
			return false;
		}

		public boolean isAcceptThreadRunning()
		{
			return false;
		}

		public int getResponseHeader(String key, int address, int bufSize)
		{
			return CONNERR_GENERIC;
		}

		public void setRequestProperty(String key, String value)
		{
			// TODO: Panic. Should not be called.
		}

		public void enterStateRead()
		{
			checkAndSetState(CONNOP_READ);
		}

		public void enterStateWrite()
		{
			checkAndSetState(CONNOP_WRITE);
		}

		public void enterStateConnect()
		{
			checkAndSetState(CONNOP_CONNECT);
		}

		public void enterStateFinish()
		{
			checkAndSetState(CONNOP_FINISH);
		}

		public void enterStateAccept()
		{
			checkAndSetState(CONNOP_ACCEPT);
		}

		private synchronized void checkAndSetState(int opType)
		{
			// There must not be an ongoing operation.
			MYASSERT(0 == (mState & opType));

			mState |= opType;
		}

	} // End of class ConnectionObject

	static class UrlConnectionObject extends ConnectionObject
	{
		private URLConnection mUrlConnection;

		/**
		 * Constructor.
		 */
		public UrlConnectionObject(MoSyncNetwork network)
		{
			super(network);
		}

		/**
		 * Initialize the connection object.
		 */
		public ConnectionObject create(String url, int handle)
			throws MalformedURLException, IOException
		{
			mHandle = handle;

			mUrlConnection = new URL(url).openConnection();

			mUrlConnection.setAllowUserInteraction(true);
			mUrlConnection.setUseCaches(false);

			return this;
		}

		/**
		 * Initialize the connection object for HTTP.
		 */
		public ConnectionObject createHttp(String url, int method, int handle)
			throws MalformedURLException, IOException
		{
			mHandle = handle;

			mUrlConnection = new URL(url).openConnection();

			mUrlConnection.setAllowUserInteraction(true);
			mUrlConnection.setUseCaches(false);

			HttpURLConnection httpConnection =
				(HttpURLConnection) mUrlConnection;

			httpConnection.setInstanceFollowRedirects(false);

			switch (method)
			{
				case HTTP_GET:
					httpConnection.setRequestMethod("GET");
					break;
				case HTTP_POST:
					mUrlConnection.setDoOutput(true);
					httpConnection.setRequestMethod("POST");
					break;
				case HTTP_PUT:
					mUrlConnection.setDoOutput(true);
					httpConnection.setRequestMethod("PUT");
					break;
				case HTTP_DELETE:
					httpConnection.setRequestMethod("DELETE");
					break;
				case HTTP_HEAD:
					httpConnection.setRequestMethod("HEAD");
					break;
				default:
					throw new BigPhatError("UrlConnectionObject.createHttp: "
						+ "unknown request method");
			}

			return this;
		}

		@Override
		public void setRequestProperty(String key, String value)
		{
			// TODO: Perhaps it is a good idea after all to have mHttpState?
			//MYASSERT(mHttpState == HTTP_STATE_SETUP);

			try
			{
				mUrlConnection.setRequestProperty(key, value);
			}
			catch (IllegalStateException ex)
			{
				ex.printStackTrace();
			}
			catch (NullPointerException ex)
			{
				ex.printStackTrace();
			}
		}

		@Override
		public int getResponseHeader(String key, int address, int bufSize)
		{
			try
			{
				String value = mUrlConnection.getHeaderField(key);

				if (value == null)
				{
					return CONNERR_NOHEADER;
				}

				{
					// Android transparently requests and unpacks gzip data,
					// but then content-length is the length of the packed data.
					// Therefore, if encoding is gzip,
					// tell the user that content-length header doesn't exist.
					String encoding = mUrlConnection.getContentEncoding();
					if(encoding != null) if(key.equalsIgnoreCase("content-length") &&
						encoding.equalsIgnoreCase("gzip"))
					{
						return CONNERR_NOHEADER;
					}
				}

				// Write the bufSize first characters in the String value
				// to ds_mem starting at address.
				byte[] valueAsBytes = value.getBytes( );
				if(bufSize < valueAsBytes.length + 1)
				{
					return valueAsBytes.length;
				}

				SYSLOG(
					"getResponseHeader value: " + value +
					" vstr: " + value +
					" vstr.length: " + valueAsBytes.length +
					" position: " + address);

				mMoSyncNetwork.copyStringToMemory((int) address, value);

				return valueAsBytes.length;
			}
			catch (IllegalStateException ex)
			{
				ex.printStackTrace();
			}

			return CONNERR_GENERIC;
		}

		@Override
		public void read(int dst, int size)
		{
			// There must not be any ongoing read operation.
			// TODO: This check is redundant as it is done
			// in the superclass method.
			MYASSERT((mState & CONNOP_READ) == 0);

			try
			{
				// Get the input stream.
				if (null == getInputStream())
				{
					setInputStream(mUrlConnection.getInputStream());
				}
			}
			catch (Exception ex)
			{
				ex.printStackTrace();
				// Need to enter read state before postResultEvent.
				enterStateRead();
				postResultEvent(CONNOP_READ, CONNERR_GENERIC);
				return;
			}

			super.read(dst, size);
		}

		@Override
		public void write(int src, int size)
		{
			// There must not be any ongoing write operation.
			// TODO: This check is redundant as it is done
			// in the superclass method.
			MYASSERT((mState & CONNOP_WRITE) == 0);

			try
			{
				// Get the output stream.
				if (null == getOutputStream())
				{
					setOutputStream(mUrlConnection.getOutputStream());
				}
			}
			catch (Exception ex)
			{
				ex.printStackTrace();
				// Need to enter write state before postResultEvent.
				enterStateWrite();
				postResultEvent(CONNOP_WRITE, CONNERR_GENERIC);
				return;
			}

			super.write(src, size);
		}

		@Override
		public void readToData(int data, int offset, int size)
		//throws IOException
		{
			// There must not be any ongoing read operation.
			// TODO: This check is redundant as it is done
			// in the superclass method.
			MYASSERT((mState & CONNOP_READ) == 0);
			try
			{
				// Get the input stream.
				if (null == getInputStream())
				{
					setInputStream(mUrlConnection.getInputStream());
				}

			}
			catch (Exception ex)
			{
				ex.printStackTrace();
				// Need to enter read state before postResultEvent.
				enterStateRead();
				postResultEvent(CONNOP_READ, CONNERR_GENERIC);
				return;
			}

			super.readToData(data, offset, size);
		}

		@Override
		public void writeFromData(int data, int offset, int size)
		{
			// There must not be any ongoing write operation.
			// TODO: This check is redundant as it is done
			// in the superclass method.
			MYASSERT((mState & CONNOP_WRITE) == 0);

			try
			{
				// Get the output stream.
				if (null == getOutputStream())
				{
					setOutputStream(mUrlConnection.getOutputStream());
				}
			}
			catch (Exception ex)
			{
				ex.printStackTrace();
				// Need to enter write state before postResultEvent.
				enterStateWrite();
				postResultEvent(CONNOP_WRITE, CONNERR_GENERIC);
				return;
			}

			super.writeFromData(data, offset, size);
		}

		/**
		 * Closes the connection.
		 * @param softCancel true if the cancel is "soft", that is requested
		 * by the application. This is considered as a normal condition.
		 * false is closed as result of an error.
		 * @throws IOException
		 */
		@Override
		public void closeConnection(boolean softCancel) throws IOException
		{
			super.closeConnection(softCancel);

			mUrlConnection = null;
		}

		/**
		 * Start executing connection.
		 */
		@Override
		public void startConnectThread()
		{
			sConnectionThreadPool.execute(new Runnable()
			{
				public void run()
				{
					doConnect();
				}
			});
		}

		void doConnect()
		{
			try
			{
				HttpURLConnection http = (HttpURLConnection) mUrlConnection;

				http.connect();

				int result = http.getResponseCode();

				if (result < 0)
				{
					DebugPrint("UrlConnectionObject.doConnect: "
						+ "http.getResponseCode() < 0 result was: " + result);
					postResultEvent(CONNOP_CONNECT, CONNERR_PROTOCOL);
					return;
				}

				if (DebugIsOn())
				{
					// Print headers
					int i = 0;
					while (true)
					{
						String key = mUrlConnection.getHeaderFieldKey(i);
						String value = mUrlConnection.getHeaderField(i);
						DebugPrint(key + ": " + value + "\n");
						if(key == null && value == null)
						{
							break;
						}
						i++;
					}
				}

				//mHttpState = HTTP_STATE_FINISHED;

				// TODO: Why synchronized at this particular point?
				synchronized(this)
				{
					if (mCancelled)
					{
						postResultEvent(CONNOP_CONNECT, CONNERR_CANCELED);
						return;
					}
				}

				postResultEvent(CONNOP_CONNECT, result); // Success
			}
			catch (InterruptedIOException ex)
			{
				ex.printStackTrace();
				postResultEvent(CONNOP_CONNECT, CONNERR_CANCELED);
			}
			catch (UnknownHostException ex)
			{
				ex.printStackTrace();
				postResultEvent(CONNOP_FINISH, CONNERR_DNS);
			}
			catch (IOException ex)
			{
				ex.printStackTrace();
				postResultEvent(CONNOP_FINISH, CONNERR_GENERIC);
			}
			catch (Exception ex)
			{
				ex.printStackTrace();
				postResultEvent(CONNOP_FINISH, CONNERR_GENERIC);
			}
		}

		@Override
		public void startFinishConnectThread()
		{
			enterStateFinish();

			sConnectionThreadPool.execute(new Runnable()
			{
				public void run()
				{
					doHttpConnectionFinish();
				}
			});
		}

		public void doHttpConnectionFinish()
		{
			HttpURLConnection httpConn = (HttpURLConnection) mUrlConnection;

			try
			{
				if (getOutputStream() != null)
				{
					getOutputStream().close();
				}

				int result = httpConn.getResponseCode();

				if (result < 0)
				{
					DebugPrint("UrlConnectionObject.doHttpConnectionFinish: "
						+ "http.getResponseCode() < 0 result was: " + result);
					result = CONNERR_PROTOCOL;
				}

				if (DebugIsOn())
				{
					// Print headers
					int i = 0;
					while (true)
					{
						String key = mUrlConnection.getHeaderFieldKey(i);
						String value = mUrlConnection.getHeaderField(i);
						DebugPrint(key + ": " + value + "\n");
						if (key == null && value == null)
						{
							break;
						}
						i++;
					}
				}

				//mHttpState = HTTP_STATE_FINISHED;

				postResultEvent(CONNOP_FINISH, result);
			}
			catch (InterruptedIOException ex)
			{
				ex.printStackTrace();
				postResultEvent(CONNOP_FINISH, CONNERR_CANCELED);
			}
			catch (UnknownHostException ex)
			{
				ex.printStackTrace();
				postResultEvent(CONNOP_FINISH, CONNERR_DNS);
			}
			catch (IOException ex)
			{
				ex.printStackTrace();
				postResultEvent(CONNOP_FINISH, CONNERR_GENERIC);
			}
			catch (Exception ex)
			{
				ex.printStackTrace();
				postResultEvent(CONNOP_FINISH, CONNERR_GENERIC);
			}
		}

	} // End of class UrlConnectionObject

	static class SocketConnectionObject extends ConnectionObject
	{
		protected String mSocketAddress;
		protected int mSocketPort;
		private Socket mSocket;

		/**
		 * Constructor.
		 */
		public SocketConnectionObject(MoSyncNetwork network)
		{
			super(network);
		}

		public int defaultPort() {
			return 80;
		}

		/**
		 * Initialize the connection object.
		 */
		public ConnectionObject create(String url, int handle)
			throws MalformedURLException, IOException, Exception
		{
			// Restructure the url since port can't be part of it.
			int realUrlStart = url.indexOf("://"); // Skip to "://"
			if (-1 == realUrlStart)
			{
				throw new MalformedURLException("Malformed URL: " + url);
			}

			String realUrl = url.substring(realUrlStart + 3);
			String[] splitUrl = realUrl.split("\\b:\\d{1,5}\\b");

			Log.i("ConnectionObject::create", "realUrl: "+realUrl);
			for(int i=0; i<splitUrl.length; i++) {
				Log.i("ConnectionObject::create", "splitUrl["+i+"]: "+splitUrl[i]);
			}

			if (splitUrl.length > 2)
			{
				throw new MalformedURLException("Malformed URL: " + url);
			}

			String newUrl = splitUrl[0];
			int port;
			if (splitUrl.length == 2)
			{
				int end =(splitUrl[0].length() + realUrl.length()) - (splitUrl[0].length() + splitUrl[1].length());

				port = Integer.valueOf(realUrl.substring(splitUrl[0].length() + 1, end));
				newUrl += splitUrl[1];
			}
			else if(splitUrl.length == 1 && splitUrl[0].length() > 0)
			{
				String p;
				if(realUrl.charAt(0) == ':') {
					p = realUrl.substring(1);
					newUrl = "";
				} else {
					p = realUrl.substring(splitUrl[0].length() + 1);
				}
				port = Integer.parseInt(p);
			}
			else
			{
				port = defaultPort();
			}

			mSocketAddress = newUrl;
			mSocketPort = port;
			mHandle = handle;

			return this;
		}

		@Override
		public int getAddr(int addrPointer)
		{
			try
			{
				if (null == mSocket)
				{
					return CONNERR_GENERIC;
				}

				// Get the IP adress of the socket.
				InetAddress inetAddr = mSocket.getInetAddress();

				if (null == inetAddr)
				{
					return CONNERR_GENERIC;
				}

				return mMoSyncNetwork.copyInetAddressToMemory(
					addrPointer,
					mSocket.getPort(),
					inetAddr.getAddress());
			}
			catch (Exception ex)
			{
				ex.printStackTrace();
				return CONNERR_GENERIC;
			}
		}

		void setSocketAndOpenStreams(Socket socket) throws IOException
		{
			mSocket = socket;

			setInputStream(socket.getInputStream());
			setOutputStream(socket.getOutputStream());
		}

		/**
		 * Start executing connection.
		 */
		@Override
		public void startConnectThread()
		{
			sConnectionThreadPool.execute(new Runnable()
			{
				public void run()
				{
					doConnect();
				}
			});
		}

		void doConnect()
		{
			try
			{
				Socket socket = createSocket(mSocketAddress, mSocketPort);

				if (null == socket)
				{
					// Error
					//Log.i("SocketConnectionObject", "socket is null");
					postResultEvent(CONNOP_CONNECT, CONNERR_GENERIC);
					return;
				}

				if (!socket.isConnected())
				{
					// Error.
					//Log.i("SocketConnectionObject", "socket is not connected");
					postResultEvent(CONNOP_CONNECT, CONNERR_GENERIC);
					return;
				}

				setSocketAndOpenStreams(socket);

				// TODO: Why synchronized?
				synchronized (this)
				{
					if (mCancelled)
					{
						postResultEvent(CONNOP_CONNECT, CONNERR_CANCELED);
						return;
					}
				}

				postResultEvent(CONNOP_CONNECT, 1); // Success
			}
			catch (InterruptedIOException ex)
			{
				ex.printStackTrace();
				postResultEvent(CONNOP_CONNECT, CONNERR_CANCELED);
				return;
			}
			catch (IOException ex)
			{
				ex.printStackTrace();
				postResultEvent(CONNOP_CONNECT, CONNERR_GENERIC);
				return;
			}
		}

		/**
		 * Factory method that creates a socket.
		 *
		 * @param socketAddress The host name.
		 * @param socketPort The port number.
		 * @return A new socket.
		 * @throws An exception if the socket cannot be created.
		 */
		Socket createSocket(String socketAddress, int socketPort)
			throws IOException, InterruptedIOException
		{
			//Log.i("SocketConnectionObject", "creating socket");
			return new Socket(socketAddress, socketPort);
		}


	} // End of class SocketConnectionObject

	static class DatagramConnectionObject extends SocketConnectionObject
	{
		private DatagramSocket mDSocket;

		public DatagramConnectionObject(MoSyncNetwork network)
		{
			super(network);
		}

		void setSocketAndOpenStreams(Socket socket) throws IOException
		{
			throw new IOException("DatagramConnectionObject.setSocketAndOpenStreams");
		}

		@Override
		public int defaultPort() {
			return 0;
		}

		@Override
		public int connect()
		{
			if(!mSocketAddress.isEmpty())
				return super.connect();

			try
			{
				Log.i("@@DatagramConnectionObject",
					"mSocketPort: " + mSocketPort);
				if(mSocketPort == 0)
					mDSocket = new DatagramSocket();
				else
					mDSocket = new DatagramSocket(mSocketPort);
				if (null == mDSocket)
				{
					return CONNERR_GENERIC;
				}

				// Important to do this last.
				mMoSyncNetwork.addConnectionObject(this);
			}
			catch (Exception ex)
			{
				Log.e("@@DatagramConnectionObject",
					"Exception in connect, handle: " + mHandle +
					" exception: " + ex);
				ex.printStackTrace();
				return CONNERR_GENERIC;
			}
			return RES_OK; // Success
		}

		@Override
		public int getAddr(int addrPointer)
		{
			try
			{
				if (null == mDSocket)
				{
					return CONNERR_GENERIC;
				}

				// Get the IP adress of the socket.
				InetAddress inetAddr;
				int port;

				if(mSocketAddress.isEmpty())
				{
					inetAddr = mDSocket.getLocalAddress();
					port = mDSocket.getLocalPort();
				}
				else
				{
					inetAddr = mDSocket.getInetAddress();
					port = mDSocket.getPort();
				}

				if (null == inetAddr)
				{
					return CONNERR_GENERIC;
				}

				Log.i("DatagramConnectionObject", "getAddr host: "+inetAddr.getHostAddress()+" port: "+port);

				return mMoSyncNetwork.copyInetAddressToMemory(
					addrPointer,
					port,
					inetAddr.getAddress());
			}
			catch (Exception ex)
			{
				ex.printStackTrace();
				return CONNERR_GENERIC;
			}
		}

		void doConnect()
		{
			try
			{
				mDSocket = new DatagramSocket();

				if (null == mDSocket)
				{
					// Error
					//Log.i("SocketConnectionObject", "socket is null");
					postResultEvent(CONNOP_CONNECT, CONNERR_GENERIC);
					return;
				}

				mDSocket.connect(new InetSocketAddress(mSocketAddress, mSocketPort));

				if (!mDSocket.isConnected())
				{
					// Error.
					//Log.i("SocketConnectionObject", "socket is not connected");
					postResultEvent(CONNOP_CONNECT, CONNERR_GENERIC);
					return;
				}

				// TODO: Why synchronized?
				synchronized (this)
				{
					if (mCancelled)
					{
						//Log.i("SocketConnectionObject",
						//	"connection was canceled");
						postResultEvent(CONNOP_CONNECT, CONNERR_CANCELED);
						return;
					}
				}

				postResultEvent(CONNOP_CONNECT, 1); // Success
			}
			catch (IOException ex)
			{
				ex.printStackTrace();
				postResultEvent(CONNOP_CONNECT, CONNERR_GENERIC);
				return;
			}
		}

		void assertInputStream() {
		}

		void assertOutputStream() {
		}

		// returns the number of bytes actually read.
		int read(byte[] dst) throws IOException {
			DatagramPacket dp = new DatagramPacket(dst, dst.length);
			mDSocket.receive(dp);
			return dp.getLength();
		}

		void write(byte[] src) throws IOException {
			DatagramPacket dp = new DatagramPacket(src, src.length);
			mDSocket.send(dp);
		}

		int readFrom(byte[] dst, int src) throws IOException {
			DatagramPacket dp = new DatagramPacket(dst, dst.length);
			mDSocket.receive(dp);
			Log.i("DatagramConnectionObject", "readFrom addr: "+dp.getAddress().getHostAddress());
			if(mMoSyncNetwork.copyInetAddressToMemory(src, dp.getPort(), dp.getAddress().getAddress()) < 0)
				return CONNERR_GENERIC;
			return dp.getLength();
		}

		void writeTo(byte[] src, InetAddress addr, int port) throws IOException {
			DatagramPacket dp = new DatagramPacket(src, src.length);
			dp.setAddress(addr);
			dp.setPort(port);
			mDSocket.send(dp);
		}
	} // End of class DatagramConnectionObject

	/**
	 * This class handles SSL connections.
	 */
	static class SecureSocketConnectionObject extends SocketConnectionObject
	{
		/**
		 * Constructor.
		 */
		public SecureSocketConnectionObject(MoSyncNetwork network)
		{
			super(network);
		}

		@Override
		Socket createSocket(String socketAddress, int socketPort)
			throws IOException, InterruptedIOException
		{
			//Log.i("SecureSocketConnectionObject", "creating secure socket");
			SocketFactory factory = SSLCertificateSocketFactory.getDefault(
				SSL_HANDSHAKE_TIMEOUT_MILLIS);
			return factory.createSocket(socketAddress, socketPort);
		}

	} // End of class SecureSocketConnectionObject

	/**
	 * Helper method for creating a Bluetooth connection object.
	 * Will create either a client or a server connection depending
	 * on the  url.
	 * @param url
	 * @param handle
	 * @return
	 * @throws MalformedURLException
	 * @throws IOException
	 * @throws Exception
	 */
	ConnectionObject createBluetoothConnectionObject(String url, int handle)
		throws MalformedURLException, IOException, Exception
	{
		if (url.startsWith("btspp://localhost:"))
		{
			// Create a server connection.
			return new BluetoothServerConnectionObject(this)
				.create(url, handle);
		}
		else
		{
			// Create a client connection.
			return new BluetoothConnectionObject(this)
				.create(url, handle);
		}
	}

	/**
	 * Bluetooth connection object. Used for client connections
	 * and for server side accepted connections.
	 */
	static class BluetoothConnectionObject extends ConnectionObject
	{
		String mDeviceAddress = null;
		int mChannel = 0;
		String mUUID = null;
		BluetoothSocket mBluetoothSocket = null;
		boolean mIsChannelClientConnectionRequest = false;
		boolean mIsUUIDClientConnectionRequest = false;

		/**
		 * Constructor.
		 */
		public BluetoothConnectionObject(MoSyncNetwork network)
		{
			super(network);
		}

		/**
		 * Initialize the connection object with a prefabricated BluetoothSocket.
		 */
		public BluetoothConnectionObject createWithBluetoothSocket(
			BluetoothSocket socket,
			int handle)
		throws IOException
		{
			//Log.i("@@BluetoothConnectionObject",
			//	"createWithBluetoothSocket handle " + handle);

			mHandle = handle;
			setBluetoothSocketAndOpenStreams(socket);
			mMoSyncNetwork.addConnectionObject(this);

			// Post connect event on this connection handle.
			// We must set the connect state for this to work.
			// Note that this case is different from the other
			// state checks, as this code is called from the accept
			// thread.
			enterStateConnect();
			postResultEvent(CONNOP_CONNECT, CONNOP_CONNECT_SUCCESS);

			return this;
		}

		/**
		 * Initialize the connection object.
		 */
		public ConnectionObject create(String url, int handle)
			throws MalformedURLException, IOException, Exception
		{
			//Log.i("@@BluetoothConnectionObject",
			//	"create handle " + handle + " url: " + url);

			// Set the handle for this connection.
			mHandle = handle;

			// Get the content part of the url.
			// Strip "btspp://"
			String realUrl = url.substring(8);

			// Get the parts separated by the colon.
			String[] splitUrl = realUrl.split(":");
			if (splitUrl.length != 2)
			{
				throw new MalformedURLException("Malformed URL: " + url);
			}

			String part1 = splitUrl[0];
			String part2 = splitUrl[1];

			// Format device address for use with the Android API.
			mDeviceAddress = part1;

			// Is it a channel number?
			// Note we now use a regexp that will allow higher channel numbers.
			// TODO: Proper regexp to use when implementation is done is:
			// "\\b[0123]{1,2}\\b"
			if (part2.matches("\\b[0123456789]{1,5}\\b"))
			{
				mIsChannelClientConnectionRequest = true;
				mChannel = Integer.parseInt(part2);
			}
			// Is it a service record UUID?
			else if (part2.matches("\\b[0123456789ABCDEFabcdef]{32}\\b"))
			{
				mIsUUIDClientConnectionRequest = true;
				mUUID = part2;
			}
			else
			{
				throw new MalformedURLException("Malformed URL: " + url);
			}

			//Log.i("@@BluetoothConnectionObject",
			//	"mDeviceAddress = " + mDeviceAddress);
			//Log.i("@@BluetoothConnectionObject", "mChannel = " + mChannel);
			//Log.i("@@BluetoothConnectionObject", "mUUID = " + mUUID);

			return this;
		}

		/**
		 * Write the address of the Bluetooth device to MoSync memory.
		 * @param addrPointer The memory address in MoSync memory
		 * to write the Bluetooth address.
		 * @return A status code.
		 */
		@Override
		public int getAddr(int addrPointer)
		{
			// Bluetooth address.
			byte[] addr = null;

			// If we have a socket get the address from the socket's device.
			if (null != mBluetoothSocket)
			{
				BluetoothDevice device = mBluetoothSocket.getRemoteDevice();
				if (null != device)
				{
					addr = getMoSyncBluetooth().getDeviceAddressAsBytes(device);
				}
			}

			// Otherwise get if from the Bluetooth adapter.
			if (null == addr)
			{
				addr = getMoSyncBluetooth().getDefaultAdapterAddressAsBytes();
			}

			// Write address data. Use zero as port number since
			// we do not know the port.
			return mMoSyncNetwork.copyBluetoothAddressToMemory(
				addrPointer,
				0, // Port
				addr);
		}

		/**
		 * Closes the connection.
		 * @param softCancel true if the cancel is "soft", that is requested
		 * by the application. This is considered as a normal condition.
		 * false is closed as result of an error.
		 * @throws IOException
		 */
		@Override
		public void closeConnection(boolean softCancel) throws IOException
		{
			//Log.i("@@BluetoothConnectionObject",
			//	"closing connection handle: " + mHandle);

			super.closeConnection(softCancel);

			try
			{
				// Superclass closes the streams and sets mCancelled
				// to the value of softCancel.
				super.closeConnection(softCancel);

				if (null != mBluetoothSocket)
				{
					//Log.i("@@BluetoothConnectionObject", "closing socket");
					mBluetoothSocket.close();
					mBluetoothSocket = null;
					//Log.i("@@BluetoothConnectionObject", "closing socket done");
				}
			}
			catch (IOException ioException)
			{
				throw ioException;
			}
			catch (RuntimeException runtimeException)
			{
				//Log.i("@@BluetoothConnectionObject",
				//	"Caught a RuntimeException to prevent further errors, " +
				//	"see stack trace:");
				runtimeException.printStackTrace();
			}
		}

		/**
		 * Sent when Bluetooth is turned off.
		 * @throws IOException
		 */
		public void bluetoothTurnedOff() throws IOException
		{
			closeConnection(false);
		}

		public void setBluetoothSocketAndOpenStreams(BluetoothSocket socket)
			throws IOException
		{
			mBluetoothSocket = socket;

			setInputStream(socket.getInputStream());
			setOutputStream(socket.getOutputStream());
		}

		public BluetoothSocket getBluetoothSocket()
		{
			return mBluetoothSocket;
		}

		/**
		 * Start executing connection.
		 */
		@Override
		public void startConnectThread()
		{
			//Log.i("@@BluetoothConnectionObject", "startConnectThread");

			sConnectionThreadPool.execute(new Runnable()
			{
				public void run()
				{
					try
					{
						// Make this thread a looper thread if there is
						// no associated looper.
						// TODO: Investigate if we really need to use a
						// looper thread here?
						if (null == Looper.myLooper())
						{
							Looper.prepare();
						}

						doConnect();
					}
					catch (Exception ex)
					{
						ex.printStackTrace();
					}
				}
			});
		}

		public void doConnect()
		{
			//Log.i("@@BluetoothConnectionObject", "doConnect");

			try
			{
				BluetoothAdapter adapter =
					getMoSyncBluetooth().getBluetoothAdapter();
				if (null == adapter)
				{
					//Log.i("@@BluetoothConnectionObject",
					//	"Bluetooth adapter not found");
					postResultEvent(CONNOP_CONNECT, CONNERR_UNAVAILABLE);
					return;
				}
				//Log.i("@@BluetoothConnectionObject", "Bluetooth adapter FOUND");

				BluetoothDevice device =
					getMoSyncBluetooth().btGetBluetoothDevice(mDeviceAddress);
				if (null == device)
				{
					//Log.i("@@BluetoothConnectionObject",
					//	"Device not found at address: " + mDeviceAddress);
					postResultEvent(CONNOP_CONNECT, CONNERR_GENERIC);
					return;
				}

				if (mIsChannelClientConnectionRequest)
				{
					//Log.i("@@BluetoothConnectionObject",
					//	"mIsChannelClientConnectionRequest");

					// Get undocumented method via reflection
					// to connect to a channel.
					Method createRfcommSocket = device.getClass().getMethod(
						"createRfcommSocket",
						new Class[] { int.class });
					if (null == createRfcommSocket)
					{
						//Log.i("@@BluetoothConnectionObject",
						//	"Could not find method createRfcommSocket");
						postResultEvent(CONNOP_CONNECT, CONNERR_GENERIC);
						return;
					}

					BluetoothSocket socket = (BluetoothSocket)
						createRfcommSocket.invoke(device, mChannel);
					if (null == socket)
					{
						//Log.i("@@BluetoothConnectionObject",
						//	"Could not create socket via createRfcommSocket");
						postResultEvent(CONNOP_CONNECT, CONNERR_GENERIC);
						return;
					}
					setBluetoothSocketAndOpenStreams(socket);
				}
				else if (mIsUUIDClientConnectionRequest)
				{
					//Log.i("@@BluetoothConnectionObject",
					//	"mIsUUIDClientConnectionRequest");
					String uuid =
						getMoSyncBluetooth().formatServiceUUID(mUUID);
					//Log.i("@@BluetoothConnectionObject",
					//	"Connecting to service uuid: " + uuid);
					BluetoothSocket socket =
						device.createRfcommSocketToServiceRecord(
							java.util.UUID.fromString(uuid));
					if (null == socket)
					{
						//Log.i("@@BluetoothConnectionObject",
						//	"handle: "
						//	+ mHandle
						//	+ " No socket created via"
						//	+ " createRfcommSocketToServiceRecord");
						postResultEvent(CONNOP_CONNECT, CONNERR_GENERIC);
						return;
					}
					setBluetoothSocketAndOpenStreams(socket);
				}

				//Log.i("@@BluetoothConnectionObject", "connect 1");
				getBluetoothSocket().connect();
				//Log.i("@@BluetoothConnectionObject", "connect 2");

				postResultEvent(CONNOP_CONNECT, CONNOP_CONNECT_SUCCESS);
			}
			catch (InterruptedIOException ex)
			{
				//Log.i("@@BluetoothConnectionObject",
				//	"InterruptedIOException " + ex);
				ex.printStackTrace();
				postResultEvent(CONNOP_CONNECT, CONNERR_CANCELED);
				return;
			}
			catch (IOException ex)
			{
				//Log.i("@@BluetoothConnectionObject", "IOException " + ex);
				ex.printStackTrace();
				postResultEvent(CONNOP_CONNECT, CONNERR_GENERIC);
				return;
			}
			catch (Exception ex)
			{
				//Log.i("@@BluetoothConnectionObject", "Exception " + ex);
				ex.printStackTrace();
				postResultEvent(CONNOP_CONNECT, CONNERR_GENERIC);
				return;
			}
		}
	} // End of class BluetoothConnectionObject

	/**
	 * Bluetooth server object.
	 */
	static class BluetoothServerConnectionObject extends ConnectionObject
	{
		String mServiceName = null;
		String mUUID = null;
		BluetoothServerSocket mBluetoothServerSocket = null;

		/**
		 * Constructor.
		 */
		public BluetoothServerConnectionObject(MoSyncNetwork network)
		{
			super(network);
		}

		/**
		 * This is a server connection.
		 */
		public boolean isServerConnection()
		{
			return true;
		}

		/**
		 * Initialize the connection object.
		 */
		public ConnectionObject create(String url, int handle)
			throws MalformedURLException, IOException, Exception
		{
			//Log.i("@@BluetoothServerConnectionObject",
			//	"create handle : " + handle + " url: " + url);

			// Set the handle for this connection.
			mHandle = handle;

			// Example of server connection url:
			// btspp://localhost:0000110100001000800000805F9B34FB;name=Test

			// Strip "btspp://localhost:"
			String realUrl = url.substring(18);

			// Split into uuid part and name part.
			String[] splitUrl = realUrl.split(";name=");

			// Set UUID.
			mUUID = splitUrl[0];

			// Set service name.
			if (splitUrl.length == 2)
			{
				mServiceName = splitUrl[1];
			}

			//Log.i("@BT", "realServerUrl = " + realUrl);
			//Log.i("@BT", "splitUrl.length = " + splitUrl.length);
			//Log.i("@BT", "mUUID = " + mUUID);
			//Log.i("@BT", "mServiceName = " + mServiceName);

			return this;
		}

		@Override
		public int getAddr(int addrPointer)
		{
			// Write address data. Use zero as port number.
			return mMoSyncNetwork.copyBluetoothAddressToMemory(
				addrPointer,
				0, // Port
				getMoSyncBluetooth().getDefaultAdapterAddressAsBytes());
		}

		/**
		 * Closes the connection.
		 * @param softCancel true if the cancel is "soft", that is requested
		 * by the application. This is considered as a normal condition.
		 * false is closed as result of an error.
		 * @throws IOException
		 */
		@Override
		public void closeConnection(boolean softCancel) throws IOException
		{
			//Log.i("@@BluetoothServerConnectionObject",
			//	"closing connection handle: " + mHandle);

			try
			{
				// Superclass closes the streams and sets mCancelled
				// to the value of softCancel.
				super.closeConnection(softCancel);

				if (null != mBluetoothServerSocket)
				{
					// This throws an IOException for any ongoing accept
					// and the accept thread will finish.
					mBluetoothServerSocket.close();
					mBluetoothServerSocket = null;
				}
			}
			catch (IOException ioException)
			{
				throw ioException;
			}
			catch (RuntimeException runtimeException)
			{
				// We have seen the RuntimeException:
				// "sending message to a Handler on a dead thread".
				// Here we just catch that error.
				Log.i("@@BluetoothServerConnectionObject",
					"Caught a RuntimeException to prevent further errors, " +
					"see stack trace:");
				runtimeException.printStackTrace();
			}
		}

		/**
		 * Sent when Bluetooth is turned off.
		 * @throws IOException
		 */
		public void bluetoothTurnedOff() throws IOException
		{
			// This will close the connection and cause an exception
			// in the accept operation.
			closeConnection(false);
		}

		/**
		 * We do NOT spawn a thread here. That is done via maAccept.
		 * And we do NOT start an async connect operation, thus we do
		 * not efter the connect state.
		 */
		@Override
		public int connect()
		{
			try
			{
				//Log.i("@@BluetoothServerConnectionObject",
				//	"connect handle: " + mHandle);

				// Do NOT post a connect event when creating a server
				// with maConnect.
				// Keeping this comment as a remainder of that.
				//postResultEvent(CONNOP_CONNECT, CONNOP_CONNECT_SUCCESS);

				BluetoothAdapter adapter =
					getMoSyncBluetooth().getBluetoothAdapter();
				if (null == adapter)
				{
					//Log.i("@@BluetoothServerConnectionObject",
					//	"Bluetooth adapter not found");
					// Should NOT post event.
					//postResultEvent(CONNOP_ACCEPT, CONNERR_UNAVAILABLE);
					return CONNERR_UNAVAILABLE;
				}

				// Create server socket. This should be done only once.
				// We will get an exception if the channel is already opened.
				mBluetoothServerSocket =
					adapter.listenUsingRfcommWithServiceRecord(
						mServiceName,
						UUID.fromString(
							getMoSyncBluetooth().formatServiceUUID(mUUID)));

				// Important to do this last, because we want to know that
				// listenUsingRfcommWithServiceRecord has succeeded before
				// adding the connection to the table of connection objects.
				mMoSyncNetwork.addConnectionObject(this);
			}
			catch (Exception ex)
			{
				Log.e("@@BluetoothServerConnectionObject",
					"Exception in connect, handle: " + mHandle +
					" exception: " + ex);
				ex.printStackTrace();
				return CONNERR_GENERIC;
			}
			return RES_OK; // Success
		}

		/**
		 * Spawn accept thread.
		 */
		@Override
		public int startAcceptThread()
		{
			//Log.i("@@BluetoothServerConnectionObject",
			//	"startAcceptThread handle: " + mHandle);

			// Must do this before spawning thread, cannot do
			// this in thread code, because the it is too late.
			enterStateAccept();

			sConnectionThreadPool.execute(new Runnable()
			{
				public void run()
				{
					try
					{
						// Make this thread a looper thread if there is no
						// associated looper.
						if (null == Looper.myLooper())
						{
							Looper.prepare();
						}

						doAccept();
					}
					catch (Exception ex)
					{
						postResultEvent(CONNOP_ACCEPT, CONNERR_CANCELED);
						//Log.i("@@BluetoothServerConnectionObject",
						//	"startAcceptThread handle: " + mHandle +
						//	" exception: " + ex);
						ex.printStackTrace();
					}
				}
			});

			return 1;
		}

		/**
		 * This is where accept is performed.
		 */
		public void doAccept()
		{
			BluetoothSocket socket = null;

			//Log.i("@@BluetoothServerConnectionObject",
			//	"doAccept handle: " + mHandle);

			// Accept. Will block. Terminate by calling close
			// on the server socket.
			try
			{
				socket = mBluetoothServerSocket.accept();
			}
			catch (IOException ex)
			{
				if (mCancelled)
				{
					// The server socket was closed.
					//Log.i("@@BluetoothServerConnectionObject",
					//	"Server socket was closed, mCancelled == true, "
					//	+ " handle: " + mHandle);
					postResultEvent(CONNOP_ACCEPT, CONNERR_CANCELED);
					return;
				}
				else
				{
					// This can be caused by Bluetooth being turned off.
					Log.i("@@BluetoothServerConnectionObject",
						"IOException during accept; handle: "
						+ mHandle + " exception " + ex);
					ex.printStackTrace();
					postResultEvent(CONNOP_ACCEPT, CONNERR_UNAVAILABLE);
					return;
				}
			}

			//Log.i("@@BluetoothServerConnectionObject",
			//	"doAccept; socket accepted handle: " + mHandle);

			// Have we reached max number of connection handles?
			if (mMoSyncNetwork.isMaxNumberOfConnectionsReached())
			{
				postResultEvent(CONNOP_ACCEPT, CONNERR_MAX);
				return;
			}

			// Get new connection handle.
			int newConnHandle = mMoSyncNetwork.getNextConnectionHandle();

			try
			{
				//Log.i("@@BluetoothServerConnectionObject",
				//	"doAccept - create BluetoothConnectionObject handle: "
				//	+ newConnHandle);

				// Create new connection object. The connection object is
				// added to the connections table in createWithBluetoothSocket.
				new BluetoothConnectionObject(mMoSyncNetwork)
					.createWithBluetoothSocket(socket, newConnHandle);
			}
			catch (IOException ex)
			{
				Log.i("@@BluetoothServerConnectionObject",
					"handle: " + mHandle + " IOException " + ex);
				ex.printStackTrace();
				postResultEvent(CONNOP_ACCEPT, CONNERR_GENERIC);
				return;
			}

			//Log.i("@@BluetoothServerConnectionObject",
			//	"doAccept - post event handle: " + mHandle);

			// Post result event on server handle.
			postResultEvent(CONNOP_ACCEPT, newConnHandle);

			//Log.i("@@BluetoothServerConnectionObject",
			//	"doAccept - done handle: " + mHandle);
		}

	} // End of class BluetoothServerConnectionObject
}
