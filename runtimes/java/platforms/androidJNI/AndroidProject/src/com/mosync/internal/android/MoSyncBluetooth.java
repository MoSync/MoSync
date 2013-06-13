/* Copyright (C) 2010 MoSync AB

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
TODO: Delete this comment when development and testing is done.

http://stackoverflow.com/questions/3397071/android-bluetooth-service-discovery-failed-exception
http://stackoverflow.com/questions/3072776/android-bluetooth-cant-connect-out
http://stackoverflow.com/questions/2661932/problem-with-bluetooth-on-android-2-1-samsung-spica-i5700-where-pairing-works-b
http://stackoverflow.com/questions/2660968/how-to-prevent-android-bluetooth-rfcomm-connection-from-dying-immediately-after
http://stackoverflow.com/questions/2207975/bluetooth-service-discovery-failed
http://stackoverflow.com/questions/2437664/android-bluetooth-fails-to-pair
http://stackoverflow.com/questions/3331392/is-it-reflection-when-i-use-getclass-getmethod
http://code.google.com/p/android/issues/detail?id=5427
http://www.kiwidoc.com/java/l/p/android/android/5/p/android.bluetooth/c/BluetoothDevice
http://www.kiwidoc.com/java/l/p/android/android/8/p/android.bluetooth/c/BluetoothDevice
http://android.git.kernel.org/?p=platform/frameworks/base.git;a=blob;f=core/java/android/bluetooth/BluetoothDevice.java;h=e77e76f79b2c4a70bb2542552ef1e8cd20d6d7a0;hb=HEAD
http://en.wikipedia.org/wiki/Bluetooth_profile
http://stackoverflow.com/questions/157476/what-guis-exist-for-git-on-windows

Pairing problem on Motorola Droid:
http://code.google.com/p/android/issues/detail?id=10002

http://www.slideshare.net/erinyueh/android-bluetooth-introduction
Passkey agent??

You can make the server discoverable by calling:
Intent discoverableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
discoverableIntent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);
startActivity(discoverableIntent);

------

I got it working, the problem was that I needed to pair with the
device first (for some reason it kept an incorrect PIN and wouldn't
pair). Here is a snippet
BluetoothDevice device;
BluetoothSocket tmpsock = null;
int port = 1;
...
try
{
Method m = device.getClass().getMethod("createRfcommSocket", new
Class[] { int.class });
tmpsock = (BluetoothSocket)m.invoke(device, port);
Log.d(TAG, "CONNECTING...");
tmpsock.connect();
...
}
catch (Exception e)
{
Log.e(TAG, "Failed to connect", e);
...
}

*/

package com.mosync.internal.android;

import static com.mosync.internal.generated.MAAPI_consts.CONNERR_CANCELED;
import static com.mosync.internal.generated.MAAPI_consts.CONNERR_GENERIC;
import static com.mosync.internal.generated.MAAPI_consts.CONNERR_UNAVAILABLE;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_BT;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.atomic.AtomicInteger;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;

public class MoSyncBluetooth
{
	/**
	 * The MoSync thread object.
	 */
	MoSyncThread mMoSyncThread;

	/**
	 * Reference to the Bluetooth adapter. Used for caching the reference.
	 * Do not use this reference as we try to reduce global reference.
	 * Use method btGetBluetoothAdapter() to get the adapter.
	 */
	BluetoothAdapter mBluetoothAdapter = null;

	/**
	 * BroadcastReceiver that listens for Bluetooth on/off events.
	 */
	BroadcastReceiver mBluetoothTurnedOnOffListener;

	/**
	 * Discovery state values.
	 */
	int BLUETOOTH_DISCOVERY_NOT_STARTED = 0;
	int BLUETOOTH_DISCOVERY_IN_PROGRESS = 1;
	int BLUETOOTH_DISCOVERY_ABORTED = 2;
	int BLUETOOTH_DISCOVERY_FINISHED = 3;

	/**
	 * State of device discovery.
	 */
	AtomicInteger mBluetoothDeviceDiscoveryState =
		new AtomicInteger(BLUETOOTH_DISCOVERY_NOT_STARTED);

	/**
	 * List that holds discovered Bluetooth devices.
	 */
	ConcurrentLinkedQueue<BluetoothDevice> mBluetoothDevices = null;

	/**
	 * Set that holds the address of discovered Bluetooth devices.
	 * This set is used to avoid adding duplicates to the list of found devices
	 * (mBluetoothDevices). Duplicate BluetoothDevice.ACTION_FOUND messages are
	 * frequently seen on Android (e.g. WildFire), so we need to track this.
	 * Using ConcurrentHashMap because there is no concurrent set.
	 */
	ConcurrentHashMap<String, Boolean> mBluetoothDeviceAddressSet = null;

	/**
	 * Thread that runs the device discovery process.
	 */
	BluetoothDeviceDiscoveryThread mBluetoothDeviceDiscoveryThread = null;

	/**
	 * Constructor.
	 * @param thread The MoSync thread.
	 */
	public MoSyncBluetooth(MoSyncThread thread)
	{
		mMoSyncThread = thread;
		registerBluetoothReceiver();
	}

	/**
	 * Called from the MoSync thread in onDestroy.
	 * Do cleanup here (but it is not guaranteed that
	 * this will be called.)
	 */
    public void onDestroy()
	{
    	unregisterBluetoothReceiver();
    }

	/**
	 * Register a broadcast receiver that is notified when
	 * Bluetooth is turned on and off, and sends events to
	 * MoSync.
	 */
	private void registerBluetoothReceiver()
	{
		// Only register receiver if Bluetooth permissions are set.
		if (!isBluetoothPermissionsSet())
		{
			return;
		}

		// Receiver for Bluetooth state changes.
		mBluetoothTurnedOnOffListener = new BroadcastReceiver()
		{
			@Override
			public void onReceive(Context context, Intent intent)
			{
				int newState = intent.getIntExtra(
					BluetoothAdapter.EXTRA_STATE,
					BluetoothAdapter.STATE_OFF);
				if (newState == BluetoothAdapter.STATE_ON)
				{
					mMoSyncThread.bluetoothTurnedOn();
				}
				else if (newState == BluetoothAdapter.STATE_OFF)
				{
					mMoSyncThread.bluetoothTurnedOff();
				}
			}
		};

		// Register receiver.
		getActivity().registerReceiver(
			mBluetoothTurnedOnOffListener,
			new IntentFilter(BluetoothAdapter.ACTION_STATE_CHANGED)
		);
	}

	/**
	 * Unregister the Bluetooth on/off broadcast receiver.
	 */
	private void unregisterBluetoothReceiver()
	{
		if (null != mBluetoothTurnedOnOffListener)
		{
			getActivity().unregisterReceiver(mBluetoothTurnedOnOffListener);
			mBluetoothTurnedOnOffListener = null;
		}
	}

	/**
	 * @return The Activity object.
	 */
	private Activity getActivity()
	{
		return mMoSyncThread.getActivity();
	}

	/**
	 * Start discovery of Bluetooth devices.
	 * @param names Not used on Android.
	 * @return 0 on success, < 0 on failure.
	 */
	int maBtStartDeviceDiscovery(int names)
	{
		panicIfBluetoothPermissionsAreNotSet();

		// Device discovery must not be running.
		if (null != mBluetoothDeviceDiscoveryThread)
		{
			return CONNERR_GENERIC;
		}

		// Note: Service discovery is not implemented on Android.
		/*
		// Service disovery must not be running.
		if (null != mBluetoothServiceDiscoveryThread)
		{
			return CONNERR_GENERIC;
		}
		*/

		// Bluetooth must be available and enabled.
		if (null == getBluetoothAdapter())
		{
			return CONNERR_UNAVAILABLE;
		}

		// List where devices are saved for retrieval.
		mBluetoothDevices = new ConcurrentLinkedQueue<BluetoothDevice>();

		// Hash map used as a set to keep track of found devices.
		mBluetoothDeviceAddressSet = new ConcurrentHashMap<String, Boolean>();

		// Create and start device discovery thread.
		mBluetoothDeviceDiscoveryThread = new BluetoothDeviceDiscoveryThread();
		mBluetoothDeviceDiscoveryThread.start();

		return 0; // Zero means success.
	}

	/**
	 * Get and remove the next device in the list of discovered devices.
	 * @param nameBufPointer Pointer to buffer where to store device name.
	 * @param nameBufSize Size of name buffer.
	 * @param actualNameLengthPointer Pointer to int to store actual name length.
	 * @param addressPointer Pointer to byte array to store Bluetooth address.
	 * @return 1 if successful, zero if the queue is empty.
	 */
	int maBtGetNewDevice(
		int nameBufPointer,
		int nameBufSize,
		int actualNameLengthPointer,
		int addressPointer)
	{
		panicIfBluetoothPermissionsAreNotSet();

		// List of disovered devices. We use a local variable to refer to
		// the list to be sure it does not go away if set to null by another
		// thread.
		ConcurrentLinkedQueue<BluetoothDevice> deviceList = mBluetoothDevices;

		if (null == deviceList)
		{
			return CONNERR_GENERIC; // Error.
		}

		// Get and remove head element of the queue.
		BluetoothDevice device = deviceList.poll();
		if (null == device)
		{
			return 0; // No more elements.
		}

		// Get device name.
		String name = device.getName();
		if (null == name)
		{
			// Name that is unsed if there is no name.
			name = "Unknown Device";
		}

		// Get length of name.
		int nameLength = name.length();

		// Can the buffer hold the name? (Take null termination char into account.)
		if (nameLength >= nameBufSize - 1)
		{
			// Truncate name if too long.
			name = name.substring(nameBufSize - 1);
		}

		// Copy name to memory buffer.
		mMoSyncThread.mMoSyncNetwork.copyStringToMemory(nameBufPointer, name);

		// Copy actual length to memory buffer.
		mMoSyncThread.mMoSyncNetwork.copyIntToMemory(actualNameLengthPointer, nameLength);

		// Copy device hardware address to memory buffer.
		byte[] addr = getDeviceAddressAsBytes(device);
		if (null != addr)
		{
			mMoSyncThread.mMoSyncNetwork.copyBytesToMemory(addressPointer, addr);
		}
		else
		{
			return CONNERR_GENERIC; // Could not get device address.
		}

		return 1; // Success retrieving device info.
	}

	/**
	 * Cancel any ongoing device or service discovery process.
	 * @return 0 if there was no active operation, 1 if there was.
	 */
	int maBtCancelDiscovery()
	{
		panicIfBluetoothPermissionsAreNotSet();

		// Is device discovery in progress?
		if (null != mBluetoothDeviceDiscoveryThread &&
			BLUETOOTH_DISCOVERY_IN_PROGRESS ==
				mBluetoothDeviceDiscoveryState.get())
		{
			// Set to aborted.
			mBluetoothDeviceDiscoveryState.set(BLUETOOTH_DISCOVERY_ABORTED);

			// Cancel Android Bluetooth discovery. This will send an
			// event with action "BluetoothAdapter.ACTION_DISCOVERY_FINISHED".
			// There we trap the aborted flag, do cleanup, and send canceled
			// MoSync event rather than a discovery finished event.
			btCancelAndroidLevelDeviceDiscovery();

			return 1;
		}

		// Note: Service discovery is not implemented on Android.
		/*
		// Is service discovery in progress?
		if (null != mBluetoothServiceDiscoveryThread &&
			BLUETOOTH_DISCOVERY_IN_PROGRESS ==
				mBluetoothServiceDiscoveryState.get())
		{
			// Set to aborted.
			mBluetoothServiceDiscoveryState.set(BLUETOOTH_DISCOVERY_ABORTED);

			mBluetoothServiceDiscoveryThread.serviceDiscoveryCanceled();
			mBluetoothServiceDiscoveryThread.stopServiceDiscovery();

			return 1;
		}
		*/

		return 0; // No active operation.
	}

	/**
	 * Check if Bluetooth permissions are set, and if not call maPanic().
	 */
	boolean isBluetoothPermissionsSet()
	{
		return
			(PackageManager.PERMISSION_GRANTED ==
				getActivity().checkCallingOrSelfPermission(
						android.Manifest.permission.BLUETOOTH))
			&&
			(PackageManager.PERMISSION_GRANTED ==
				getActivity().checkCallingOrSelfPermission(
						android.Manifest.permission.BLUETOOTH_ADMIN));
	}

	/**
	 * Check if Bluetooth permissions are set, and if not call maPanic().
	 */
	void panicIfBluetoothPermissionsAreNotSet()
	{
		if (!isBluetoothPermissionsSet())
		{
			mMoSyncThread.maPanic(1,
				"Bluetooth permission is not set in the MoSync project");
		}
	}

	/**
	 * Get the address of a Bluetooth device.
	 */
	byte[] getDeviceAddressAsBytes(BluetoothDevice device)
	{
		if (null == device)
		{
			return null;
		}

		return deviceAddressToBytes(device.getAddress());
	}

	/**
	 * Get the address of the default Bluetooth adapter.
	 */
	byte[] getDefaultAdapterAddressAsBytes()
	{
		BluetoothAdapter adapter = getBluetoothAdapter();
		if (null == adapter)
		{
			// No adapter found.
			return null;
		}

		return deviceAddressToBytes(adapter.getAddress());
	}

	/**
	 * Format device address for use with the Android API.
	 * Format of in data is: 001122AABBCC
	 * Format of out data is:  00:11:22:AA:BB:CC
	 */
	String formatDeviceAddress(String address)
	{
		// Do sanity check on the address.
		if (address.length() != 12)
		{
			return null;
		}

		// Upper case is an Android API requirement.
		address = address.toUpperCase();
		return
			address.substring(0, 2) + ":" +
			address.substring(2, 4) + ":" +
			address.substring(4, 6) + ":" +
			address.substring(6, 8) + ":" +
			address.substring(8, 10) + ":" +
			address.substring(10, 12);
	}

	/**
	 * Convert formatted device address back to plain hex number.
	 * Format of in data is:  00:11:22:AA:BB:CC
	 * Format of out data is: 001122AABBCC
	 */
	String unformatDeviceAddress(String address)
	{
		// Do sanity check on the address.
		if (address.length() != 17)
		{
			return null;
		}

		return
			address.substring(0, 2) +
			address.substring(3, 5) +
			address.substring(6, 8) +
			address.substring(9, 11) +
			address.substring(12, 15) +
			address.substring(15, 17);
	}

	/**
	 * Convert Android formatted device address to byte array.
	 * Format of in data is:  00:11:22:AA:BB:CC
	 * Format of out data is byte array { 0x00, 0x11, 0x22, 0xAA, 0xBB, 0xCC }
	 */
	byte[] deviceAddressToBytes(String address)
	{
		if (null == address)
		{
			return null;
		}

		byte[] addr = new byte[6];

		// Format of address string is "00:11:22:AA:BB:CC", should be 17 chars.
		if (17 == address.length())
		{
			addr[0] = (byte) Integer.parseInt(address.substring(0, 2), 16);
			addr[1] = (byte) Integer.parseInt(address.substring(3, 5), 16);
			addr[2] = (byte) Integer.parseInt(address.substring(6, 8), 16);
			addr[3] = (byte) Integer.parseInt(address.substring(9, 11), 16);
			addr[4] = (byte) Integer.parseInt(address.substring(12, 14), 16);
			addr[5] = (byte) Integer.parseInt(address.substring(15, 17), 16);

			return addr;
		}
		else
		{
			return null;
		}
	}

	/**
	 * Format service UUID for use with the Android API.
	 * Format of in data is:  0000110100001000800000805F9B34FB
	 * Format of out data is: 00001101-0000-1000-8000-00805F9B34FB
	 */
	String formatServiceUUID(String uuid)
	{
		// Do sanity check on the uuid.
		if (uuid.length() != 32)
		{
			return null;
		}

		// Upper case is an Android API requirement.
		uuid = uuid.toUpperCase();
		return
			uuid.substring(0, 8) + "-" +
			uuid.substring(8, 12) + "-" +
			uuid.substring(12, 16) + "-" +
			uuid.substring(16, 20) + "-" +
			uuid.substring(20, 32);
	}

	/**
	 * Convert formatted service UUID back to plain hex number.
	 * Format of in data is:  00001101-0000-1000-8000-00805F9B34FB
	 * Format of out data is: 0000110100001000800000805F9B34FB
	 */
	String unformatServiceUUID(String uuid)
	{
		// Do sanity check on the uuid.
		if (uuid.length() != 36)
		{
			return null;
		}

		return
			uuid.substring(0, 8) +
			uuid.substring(9, 13) +
			uuid.substring(14, 18) +
			uuid.substring(19, 23) +
			uuid.substring(24, 36);
	}

	/**
	 * On success, return a Bluetooth adapter.
	 * On error, return null.
	 */
	BluetoothAdapter getBluetoothAdapter()
	{
		// Do we have an enabled Bluetooth adapter?
		if (null != mBluetoothAdapter && mBluetoothAdapter.isEnabled())
		{
/*
			//Log.i("@@@ MoSync",
			//	"Found cached and enabled Bluetooth adapter with address: "
			//	+ mBluetoothAdapter.getAddress());
*/
			return mBluetoothAdapter;
		}

		// Get and enable the Bluetooth adapter.

		BluetoothAdapterRetrieveThread worker =
			new BluetoothAdapterRetrieveThread();
		try
		{
			// Start thread that gets the Bluetooth adapter.
			worker.start();

			// This call blocks.
			mBluetoothAdapter = worker.getBluetoothAdapter();
		}
		catch (Exception e) //TODO: Throwable
		{
			worker.quit();
			mBluetoothAdapter = null;
			e.printStackTrace();
		}

		// Print log message with the Bluetooth address.
/*
		if (null != mBluetoothAdapter)
		{
			Log.i("@@@ MoSync",
				"Found enabled Bluetooth adapter with address: "
				+ mBluetoothAdapter.getAddress());
		}
		else
		{
			Log.i("@@@ MoSync", "Enabled Bluetooth adapter not found.");
		}
*/
		return mBluetoothAdapter;
	}

	/**
	 * Class that gets and also enables the Bluetooth adapter,
	 * if this is not done.
	 */
	class BluetoothAdapterRetrieveThread extends Thread
	{
		Looper mLooper;

		// List used to communicate when the adapter is available.
		LinkedBlockingQueue<ObjectRef> mQueue =
			new LinkedBlockingQueue<ObjectRef>(1);

		/**
		 * Called by user of this thread to get the Bluetooth adapter.
		 * Blocking call.
		 * @return A BluetoothAdapter on success, null on failure.
		 */
		public BluetoothAdapter getBluetoothAdapter()
		{
			ObjectRef obj = null;
			while (null == obj)
			{
				try
				{
					obj = mQueue.take();
				}
				catch (InterruptedException consume)
				{
					Log.i("@@@ MoSync", "InterruptedException in getBluetoothAdapter (this is ok).");
					//consume.printStackTrace();
				}
				//catch (RuntimeException ex)
				//{
				//	Log.i("@@@ MoSync", "RuntimeException in getBluetoothAdapter.");
				//	ex.printStackTrace();
				//}
			}
			return (BluetoothAdapter) obj.get();
		}

		public void quit()
		{
			if (null != mLooper)
			{
				mLooper.quit();
				mLooper = null;
			}
		}

		public void run()
		{
			// Ensure that this is a Looper thread.
			Looper.prepare();
			mLooper = Looper.myLooper();

			// Get the adapter.
			final BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();

			if (null == adapter)
			{
				Log.i("@@@ MoSync",
					"btGetBluetoothAdapter: Adapter NOT enabled, " +
					"displaying enable dialog.");

				// No adapter is available.
				mQueue.offer(new ObjectRef(null));

				return;
			}

			// If Bluetooth is not enabled, open an activity for doing this.
			// (Do not call adapter.enable() explicitly.)
			if (!adapter.isEnabled())
			{
				// Create message handler that will get notified when
				// the Bluetooth enable dialog is closed. Message is
				// sent from the Activity via the Mediator class.
				Handler handler = new Handler(mLooper)
				{
					public void handleMessage(Message message)
					{
						// The user has closed the Bluetooth enable dialog.
						if (Mediator.REQUEST_ENABLE_BLUETOOTH == message.arg1)
						{
							if (adapter.isEnabled())
							{
								//Log.i("@@@ MoSync",
								//	"btGetBluetoothAdapter: Adapter is ON");

								// Adapter is available and enabled.
								mQueue.offer(new ObjectRef(adapter));
							}
							else
							{
								//Log.i("@@@ MoSync",
								//	"btGetBluetoothAdapter: Adapter is OFF");

								// Adapter is NOT available.
								mQueue.offer(new ObjectRef(null));
							}

							// Set the mediator's handler reference to null.
							Mediator.getInstance().mBluetoothEnableMessageHandler = null;

							// Quit this thread.
							quit();

						}
					}
				};

				// Set the mediator's handler.
				Mediator.getInstance().mBluetoothEnableMessageHandler = handler;

				//Log.i("@@@ MoSync",
				//	"btGetBluetoothAdapter: Adapter NOT enabled, " +
				//	"displaying enable dialog.");

				// Open the activity.
				Intent btPermissionIntent = new Intent(
					BluetoothAdapter.ACTION_REQUEST_ENABLE);
				getActivity().startActivityForResult(
					btPermissionIntent,
					Mediator.REQUEST_ENABLE_BLUETOOTH);

				// Start looping to receive messages to the handler.
				Looper.loop();
			}
			else // Adapter is enabled.
			{
				//Log.i("@@@ MoSync",
				//	"btGetBluetoothAdapter: Cached adapter is ON");

				// Cached adapter is available and enabled.
				mQueue.offer(new ObjectRef(adapter));
			}
		}
	}

	/**
	 * Helper class that holds a reference to an object.
	 * Allows to refer to null values.
	 */
	class ObjectRef
	{
		Object mObj;

		public ObjectRef(Object obj)
		{
			mObj = obj;
		}

		public Object get()
		{
			return mObj;
		}
	}

	/**
	 * On success, return the requested Bluetooth device.
	 * On error, return null.
	 */
	BluetoothDevice btGetBluetoothDevice(String address)
	{
		final String deviceAddress = formatDeviceAddress(address);

		// List used to communicate the device.
		final LinkedBlockingQueue<ObjectRef> queue =
			new LinkedBlockingQueue<ObjectRef>(1);

		// Thread to do the job.
		Thread worker;

		try
		{
			worker = new Thread()
			{
				public void run()
				{
					// Ensure that this is a Looper thread.
					Looper.prepare();

					// Get the adapter.
					BluetoothAdapter adapter = getBluetoothAdapter();
					if (null == adapter)
					{
						queue.offer(new ObjectRef(null));
					}

					// Get the device.
					BluetoothDevice device =
						adapter.getRemoteDevice(deviceAddress);

					// Make it available.
					queue.offer(new ObjectRef(device));
				}
			}; // worker

			// Start the thread.
			worker.start();

			// Wait for the device.
			// If this times out, an exception will be thrown and
			// null will be returned in the catch clause.
			// ObjectRef obj = queue.poll(maxSecondsToWait, TimeUnit.SECONDS);
			ObjectRef obj = queue.take();
			if (null != obj)
			{
				return (BluetoothDevice) obj.get();
			}
			else
			{
				return null;
			}
		}
		catch (Exception e)
		{
			Log.e("*** btGetBluetoothDevice", "Exception finding device " + e);
			e.printStackTrace();
			return null;
		}
	}

	/**
	 * Cancel any ongoing device discovery on the Android level.
	 */
	public void btCancelAndroidLevelDeviceDiscovery()
	{
		BluetoothAdapter adapter = getBluetoothAdapter();
		if (null != adapter && adapter.isDiscovering())
		{
			adapter.cancelDiscovery();
		}
	}

	/**
	 * Post a Bluetooth message to the MoSync event queue.
	 */
	public void btPostBluetoothMessage(int state)
	{
		int[] event = new int[2];
		event[0] = EVENT_TYPE_BT;
		event[1] = state;
		mMoSyncThread.postEvent(event);
	}

	/**
	 * Thread that performs discovery of Bluetooth devices.
	 */
	class BluetoothDeviceDiscoveryThread extends Thread
	{
		Looper mLooper;
		BroadcastReceiver mBluetoothReciever;

		public BluetoothDeviceDiscoveryThread()
		{
		}

		public void run()
		{
			try
			{
				// Make sure we have Bluetooth enabled.
				BluetoothAdapter adapter = getBluetoothAdapter();
				if (null == adapter)
				{
					btPostBluetoothMessage(CONNERR_GENERIC);
					return;
				}

				// Set discovery state to in progress.
				mBluetoothDeviceDiscoveryState.set(
					BLUETOOTH_DISCOVERY_IN_PROGRESS);

				Looper.prepare();
				mLooper = Looper.myLooper();

				startDeviceDiscovery();

				Looper.loop();
			}
			catch (Throwable e) // Should rarely happen.
			{
				Log.e("BluetoothDeviceDiscoveryThread", "Error: " + e);
				e.printStackTrace();

				// Stop device discovery.
				stopDeviceDiscovery();

				// Is this the right thing to post?
				btPostBluetoothMessage(CONNERR_GENERIC);
			}
		}

		void startDeviceDiscovery()
		{
			// Cancel any ongoing discovery on the Android level.
			btCancelAndroidLevelDeviceDiscovery();

			// Create device discovery listener.
			mBluetoothReciever = new BroadcastReceiver()
			{
				@Override
				public void onReceive(Context context, Intent intent)
				{
					final String action = intent.getAction();

					//Log.i("BluetoothDeviceDiscoveryThread",
					//	"*** Intent action: " + action);

					if (action.equals(BluetoothDevice.ACTION_FOUND))
					{
						//Log.i("BluetoothDeviceDiscoveryThread",
						//	"*** Found device!");

						// Add found device to list of devices.
						BluetoothDevice device = intent.getParcelableExtra(
							BluetoothDevice.EXTRA_DEVICE);
						deviceFound(device);
					}
					else if (action.equals(
						BluetoothAdapter.ACTION_DISCOVERY_FINISHED))
					{
						if (BLUETOOTH_DISCOVERY_ABORTED ==
							mBluetoothDeviceDiscoveryState.get())
						{
							//Log.i("BluetoothDeviceDiscoveryThread",
							//	"ACTION_DISCOVERY_FINISHED - SCAN ABORTED");

							// Scan was canceled, report Bluetooth event
							// with state CONNERR_CANCELED.
							deviceDiscoveryCanceled();

							// Stop discovery thread and cleanup.
							stopDeviceDiscovery();
						}
						else
						{
							//Log.i("BluetoothDeviceDiscoveryThread",
							//	"ACTION_DISCOVERY_FINISHED - SCAN FINISHED");

							// Scan is successfully finished, report Bluetooth
							// event with the result.
							deviceDiscoveryFinished();

							// Stop discovery thread and cleanup.
							stopDeviceDiscovery();
						}
					}
				}
			};

			// Register receiver.
			IntentFilter filter = new IntentFilter();
			filter.addAction(BluetoothDevice.ACTION_FOUND);
			filter.addAction(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
			getActivity().registerReceiver(mBluetoothReciever, filter);

			// Start discovery on the Android level.
			getBluetoothAdapter().startDiscovery();
		}

		void stopDeviceDiscovery()
		{
			// Quit the thread and set thread variable to null.
			mLooper.quit();
			getActivity().unregisterReceiver(mBluetoothReciever);
			mBluetoothDeviceDiscoveryState.set(BLUETOOTH_DISCOVERY_FINISHED);
			mBluetoothDeviceDiscoveryThread = null;
		}

		void deviceFound(BluetoothDevice device)
		{
			// Add device to list (using a local variable to avoid
			// null pointer exceptions in a concurrent world).
			ConcurrentLinkedQueue<BluetoothDevice> deviceList =
				mBluetoothDevices;
			if (null != deviceList)
			{
				ConcurrentHashMap<String, Boolean>
					deviceSet = mBluetoothDeviceAddressSet;

				// Add device only if it is not already present.
				if (!deviceSet.containsKey(device.getAddress()))
				{
					//Log.i("BluetoothDeviceDiscoveryThread",
					//	"Adding device: " + device.getName() +
					//	" " + device.getAddress());

					deviceSet.put(device.getAddress(), true);
					deviceList.add(device);
					// State zero means "still working".
					btPostBluetoothMessage(0);
				}
/*
				else
				{
					Log.i("BluetoothDeviceDiscoveryThread",
						"Duplicate found: " + device.getName() +
						" " + device.getAddress());
				}
*/
			}
		}

		void deviceDiscoveryFinished()
		{
			// Post event with number of devices found + 1.
			ConcurrentLinkedQueue<BluetoothDevice> deviceList =
				mBluetoothDevices;
			if (null != deviceList)
			{
				// State is number of devices found + 1.
				btPostBluetoothMessage(deviceList.size() + 1);
			}
		}

		void deviceDiscoveryCanceled()
		{
			// Empty device list and set to null.
			ConcurrentLinkedQueue<BluetoothDevice> deviceList =
				mBluetoothDevices;
			if (null != deviceList)
			{
				// Might be a good idea to make sure queue is empty if
				// there is some other reference to it.
				while (null != deviceList.poll()) { }
			}

			// Set device list to null (so important it deserves a comment).
			mBluetoothDevices = null;

			// Post cancel event.
			btPostBluetoothMessage(CONNERR_CANCELED);
		}
	}

} // End of class MoSyncBluetooth


	// *******************************************************************
	// ***** COMMENTED OUT CODE AHEAD
	// *******************************************************************

	// Note: Service discovery is not implemented on Android.
	/**
	 * Thread that performs discovery of Bluetooth services.
	 * TODO: Service discovery is NOT implemented.
	 */
	/*
	class BluetoothServiceDiscoveryThread extends Thread
	{
		String mAddress;
		String mUUID;
		Looper mLooper;

		public BluetoothServiceDiscoveryThread(String address, String uuid)
		{
			mAddress = address;
			mUUID = uuid;
		}

		public void run()
		{
			Looper mLooper;

			Looper.prepare();
			mLooper = Looper.myLooper();

			try
			{
				// Set discovery state to in progress.
				mBluetoothServiceDiscoveryState.set(BLUETOOTH_DISCOVERY_IN_PROGRESS);

				startServiceDiscovery();

				//Looper.loop();  // TODO: Clean up this code.
			}
			catch (Throwable e)
			{
				Log.i("BluetoothServiceDiscoveryThread", "Error: " + e);
				e.printStackTrace();

				// Is this the right thing to post?
				btPostBluetoothMessage(CONNERR_GENERIC);

				// TODO: What about canceling ongong discovery, and setting state
				// to finished of not initialized?
			}
			finally
			{
				mLooper.quit();
			}
		}

		// Here we emulate the service discovery by doing a connect
		// with the UUID we have.
		// Note: We gave up this idea!
		void startServiceDiscovery()
		{
			// Cancel any ongoing discovery on the Android level.
			btCancelAndroidLevelDeviceDiscovery();

			try
			{
				BluetoothAdapter adapter = btGetBluetoothAdapter();
				if (null == adapter)
				{
					btPostBluetoothMessage(CONNERR_UNAVAILABLE);
					return;
				}

				BluetoothDevice device = btGetBluetoothAdapter().getRemoteDevice(btFormatDeviceAddress(mAddress));
				if (null == device)
				{
					btPostBluetoothMessage(CONNERR_GENERIC);
					return;
				}

				BluetoothSocket socket = device.createRfcommSocketToServiceRecord(
					java.util.UUID.fromString(btFormatServiceUUID(mUUID)));
				if (null == socket)
				{
					btPostBluetoothMessage(CONNERR_GENERIC);
					return;
				}

				// This will timeout and throw an exception if no service
				// with the given UUID is found.
				socket.connect();

				// Close socket right away, we only wanted to see if it could be opended.
				socket.close();

				Log.i("@@BluetoothServiceDiscovery", "Service found");

				// Add data to be retrieved by maBtGetNextServiceSize and maBtGetNewService.
				serviceFound(new BluetoothService(
					31, // Fake port
					btFormatServiceUUID(mUUID), // Use uuid as name of service
					mUUID // The uuid hex string
				));

				// Scan is successfully finished, report Bluetooth event with result.
				serviceDiscoveryFinished();

				// Do cleanup.
				stopServiceDiscovery();
			}
			catch (Exception e)
			{
				Log.i("@@BluetoothServiceDiscovery", "Service NOT found: " + e);
				btPostBluetoothMessage(CONNERR_GENERIC);
				return;
			}
		}

		void stopServiceDiscovery()
		{
			//mLooper.quit();
			mBluetoothServiceDiscoveryThread = null;
			mBluetoothServiceDiscoveryState.set(BLUETOOTH_DISCOVERY_FINISHED);
		}

		void serviceFound(BluetoothService service)
		{
			// Add device to list.
			ConcurrentLinkedQueue<BluetoothService> serviceList = mBluetoothServices;
			if (null != serviceList)
			{
				serviceList.add(service);
				btPostBluetoothMessage(0); // State zero means "still working"
			}
		}

		void serviceDiscoveryFinished()
		{
			// Post event with number of devices found + 1.
			ConcurrentLinkedQueue<BluetoothService> serviceList = mBluetoothServices;
			if (null != serviceList)
			{
				btPostBluetoothMessage(serviceList.size() + 1); // State is number of services found + 1
			}
		}

		void serviceDiscoveryCanceled()
		{
			// Empty service list and set to null.
			ConcurrentLinkedQueue<BluetoothService> serviceList = mBluetoothServices;
			if (null != serviceList)
			{
				// Might be a good idea to make sure queue is empty if
				// there is some other reference to it.
				while (null != serviceList.poll()) { }
			}

			mBluetoothServices = null;

			// Post cancel event.
			btPostBluetoothMessage(CONNERR_CANCELED);
		}
	}

	class BluetoothService
	{
		public int mChannel;
		public String mName;
		public String mUUID; // Should be in plain hex format.

		public BluetoothService(int channel, String name, String uuid)
		{
			mChannel = channel;
			mName = name;
			mUUID = uuid;
		}

		public int getChannel()
		{
			return mChannel;
		}

		public String getName()
		{
			return mName;
		}

		// @return Length of buffer needed to hold the name,
		// including the terminating null character.
		public int getNameLength()
		{
			return mName.length() + 1;
		}

		public int getNumberOfUUIDs()
		{
			return 1; // Hardcoded to one UUID in this implementation.
		}

		public ByteBuffer getUUIDsAsByteBuffer()
		{
			ByteBuffer buf = ByteBuffer.allocate(32);
			buf.order(ByteOrder.LITTLE_ENDIAN);
			IntBuffer intBuf = buf.asIntBuffer();
			intBuf.put(Integer.parseInt(mUUID.substring(0, 8), 16));
			intBuf.put(Integer.parseInt(mUUID.substring(8, 16), 16));
			intBuf.put(Integer.parseInt(mUUID.substring(16, 24), 16));
			intBuf.put(Integer.parseInt(mUUID.substring(24, 32), 16));
			return buf;
		}
	}

	// State of service discovery.
	AtomicInteger mBluetoothServiceDiscoveryState =
		new AtomicInteger(BLUETOOTH_DISCOVERY_NOT_STARTED);

	// List that holds discovered Bluetooth services.
	ConcurrentLinkedQueue<BluetoothService> mBluetoothServices =
		new ConcurrentLinkedQueue<BluetoothService>();

	// Thread that runs the service discovery process.
	BluetoothServiceDiscoveryThread
		mBluetoothServiceDiscoveryThread = null;

	*/ // End of commented out service discovery code.



	// This class is not used. This is how you ask the user to enable
	// the Bluetooth adapter, if we want to do that.
	/**
	 * Class that gets and also enables the Bluetooth adapter,
	 * if this is not done.
	 */
	/*
	class BluetoothAdapterEnablerThread extends Thread
	{
		int REQUEST_ENABLE_BT = 2;

		Looper mLooper;

		// List used to communicate when the adapter is avaiable.
		LinkedBlockingQueue<ObjectRef> mQueue =
			new LinkedBlockingQueue<ObjectRef>(1);

		public BluetoothAdapter getBluetoothAdapter(int maxSecondsToWait)
		{
			try
			{
				// Wait for the user to enabling the adapter.
				// If this times out, an exception will be thrown and
				// null will be returned in the catch clause.
				Log.i("****** btGetBluetoothAdapter", "Waiting for adapter");
				// ObjectRef obj = mQueue.poll(maxSecondsToWait, TimeUnit.SECONDS);

				// TODO: Make while loop and catch InterruptedException !!!!

				ObjectRef obj = mQueue.take();
				if (null != obj)
				{
					return (BluetoothAdapter) obj.get();
				}
				else
				{
					return null;
				}
			}
			catch (Exception e)
			{
				Log.e("*** btGetBluetoothAdapter",
					"Exception in BluetoothAdapterEnablerThread: " + e);
				e.printStackTrace();
				return null;
			}
		}

		public void quit()
		{
			if (null != mLooper)
			{
				mLooper.quit();
				mLooper = null;
			}
		}

		public void run()
		{
			// Ensure that this is a Looper thread.
			Looper.prepare();
			mLooper = Looper.myLooper();

			// Get the adapter.
			final BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();

			if (null == adapter)
			{
				// No adapter is available.
				mQueue.offer(new ObjectRef(null));
				return;
			}

			// If not Bluetooth is enabled, open an activity for doing this.
			// (Do not call adapter.enable() explicitly.)
			if (!adapter.isEnabled())
			{
				Log.i("*** btGetBluetoothAdapter",
					"BluetoothAdapter NOT enabled - Enabling");

				// Register receiver for Bluetooth state changes.
				mContext.registerReceiver(new BroadcastReceiver()
				{
					// TODO: Problem is that we do not get any
					// BluetoothAdapter.ACTION_STATE_CHANGED if the user
					// selects No in the enable dialog (since the state
					// is not changed I guess). Is there some other
					// event that can be used to detect that the
					// Bluetooth dialog is closed?
					// This is now solved, get the result in onActivityResult
					// instead.
					@Override
					public void onReceive(Context context, Intent intent)
					{
						int newState = intent.getIntExtra(
							BluetoothAdapter.EXTRA_STATE,
							BluetoothAdapter.STATE_OFF)
						if (newState == BluetoothAdapter.STATE_ON)
						{
							// We have finished using this listener.
							context.unregisterReceiver(this);

							// Adapter is available.
							Log.i("*** btGetBluetoothAdapter",
								"BT is now turned ON - offer adapter 1");
							mQueue.offer(new ObjectRef(adapter));
							Log.i("*** btGetBluetoothAdapter",
								"BT is now turned ON - offer adapter 2");

							// Bluetooth is now enabled, quit this thread.
							quit();

						}
						else // TODO: This code seems not to be called.
						{
							// Bluetooth is off, quit this thread.
							quit();

							// We have finished using this listener.
							context.unregisterReceiver(this);

							// Adapter is NOT available.
							Log.i("*** btGetBluetoothAdapter",
								"BT is turned OFF - offer null 1");
							mQueue.offer(new ObjectRef(null));
							Log.i("*** btGetBluetoothAdapter",
								"BT is turned OFF - offer null 2");
						}
					}
				},
				new IntentFilter(BluetoothAdapter.ACTION_STATE_CHANGED));

				// Open the activity.
				Intent btPermissionIntent = new Intent(
					BluetoothAdapter.ACTION_REQUEST_ENABLE);
				((Activity) mContext).startActivityForResult(
					btPermissionIntent, REQUEST_ENABLE_BT);

				// Start looping to receive messages.
				mLooper.loop();
			}
			else // Adapter is enabled.
			{
				// Quit this thread.
				quit();

				Log.i("****** btGetBluetoothAdapter", "offer adapter 1");
				// Adapter is available.
				mQueue.offer(new ObjectRef(adapter));
				Log.i("****** btGetBluetoothAdapter", "offer adapter 2");
			}
		}
	}
	*/


	// Just want to keep this "snippet" for a litte longer.
	/*
	TODO: This code allows the user to enable Bluetooth, if not
	already enabled. But this is not used right now. We need to settle
	on the design regarding this, because on other platforms you have
	to enable BT manually but on Android it is good practice for the
	app to present the enable dialog.

	int initiateDeviceDiscovery()
	{
		Log.i("****** BluetoothDeviceDiscoveryThread.initiateDeviceDiscovery", "begin");

		BluetoothAdapter adapter = btGetBluetoothAdapter();
		if (null == adapter)
		{
			return CONNERR_UNAVAILABLE;
		}

		// If not enabled, open an activity for the user to enable Bluetooth,
		// then resume device discovery.
		// TODO: Test this code. --> Seems to work!
		if (!adapter.isEnabled())
		{
			Log.i("****** BluetoothDeviceDiscoveryThread.initiateDeviceDiscovery", "BluetoothAdapter NOT enabled");

			// Open the activity.
			Intent btPermissionIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
			((Activity) mContext).startActivityForResult(btPermissionIntent, REQUEST_ENABLE_BT);

			// Register receiver for Bluetooth state changes.
			mContext.registerReceiver(new BroadcastReceiver()
			{
				@Override
				public void onReceive(Context context, Intent intent)
				{
					int newState = intent.getIntExtra(
						BluetoothAdapter.EXTRA_STATE,
						BluetoothAdapter.STATE_OFF);
					if (newState == BluetoothAdapter.STATE_ON)
					{
						// Bluetooth is now enabled.

						// First tell we have finished using this listener.
						context.unregisterReceiver(this);

						// Then start discovery.
						startDeviceDiscovery();
					}
				}
			},
			new IntentFilter(BluetoothAdapter.ACTION_STATE_CHANGED));

			// Do not call explictly: adapter.enable();
		}
		else
		{
			// Start discovery.
			startDeviceDiscovery();
		}

		Log.i("****** BluetoothDeviceDiscoveryThread.initiateDeviceDiscovery", "end");

		return 0;
	}
	*/

	// Keep this code for a while.
	/**
	 * Old version of the Bluetooth adapter retrieve thread.
	 *
	 * Class that gets the Bluetooth adapter.
	 * This must be done from a Looper thread!
	 */
	/*
	class BluetoothAdapterRetrieveThread extends Thread
	{
		Looper mLooper;

		// List used to communicate when the adapter is available.
		final LinkedBlockingQueue<ObjectRef> mQueue =
			new LinkedBlockingQueue<ObjectRef>(1);

		public BluetoothAdapter getBluetoothAdapter()
		{
			// Timeout is 2 seconds, after that null is returned.
			long timeOut = 2000 + System.currentTimeMillis();
			ObjectRef obj = null;
			while (null == obj)
			{
				if (System.currentTimeMillis() >= timeOut)
				{
					return null;
				}

				try
				{
					obj = mQueue.take();
				}
				catch (InterruptedException consume)
				{
				}
			}
			return (BluetoothAdapter) obj.get();
		}

		public void quit()
		{
			if (null != mLooper)
			{
				mLooper.quit();
				mLooper = null;
			}
		}

		public void run()
		{
			try
			{
				// Ensure that this is a Looper thread.
				Looper.prepare();
				mLooper = Looper.myLooper();

				// Get the adapter.
				BluetoothAdapter adapter =
					BluetoothAdapter.getDefaultAdapter();
				if (null != adapter && adapter.isEnabled())
				{
					// Adapter is available.
					mQueue.put(new ObjectRef(adapter));
				}
				else
				{
					// No enabled adapter is available.
					mQueue.put(new ObjectRef(null));
				}
			}
			catch (Exception e)
			{
				Log.e("*** btGetBluetoothAdapter",
					"Exception in BluetoothAdapterRetrieveThread: " + e);
				e.printStackTrace();
			}
			finally
			{
				quit();
			}
		}
	}
	*/

	// Note: Service discovery currently does not work on Android.
	// This code shows how it might be implemented in a "fake" way.
	/**
	 * Start service discovery.
	 * @param address The address of the device to perform service discovery on.
	 * @param uuid The uuid of the service to discover.
	 * @return 0 on success, < 0 on failure.
	 */
	/*
	int maBtStartServiceDiscovery(String address, String uuid) throws Exception
	{
		if (btBluetoothApiIsNotAvailable())
		{
			return IOCTL_UNAVAILABLE;
		}

		btPanicIfBluetoothPermissionsAreNotSet();

		// Device disovery must not be running.
		if (null != mBluetoothDeviceDiscoveryThread)
		{
			return CONNERR_GENERIC;
		}

		// Service disovery must not be running.
		if (null != mBluetoothServiceDiscoveryThread)
		{
			return CONNERR_GENERIC;
		}

		// Bluetooth must be available and enabled.
		if (null == btGetBluetoothAdapter())
		{
			return CONNERR_UNAVAILABLE;
		}

		Log.i("@@@maBtStartServiceDiscovery",
			"Address: " + address + " UUID: " + uuid);

		// List where services are saved for retrieval.
		mBluetoothServices = new ConcurrentLinkedQueue<BluetoothService>();

		// Create and start device discovery thread.
		mBluetoothServiceDiscoveryThread =
			new BluetoothServiceDiscoveryThread(address, uuid);
		mBluetoothServiceDiscoveryThread.start();

		return 0; // Zero means success.
	}
	*/

	/**
	 * Get size info for the next service.
	 * @param nameBufSizePointer Pointer to int to get size of service name.
	 * @param nUuidsPointer ??
	 * @return > 0 on success, or zero if the queue is empty.
	 */
	/*
	int maBtGetNextServiceSize(int nameBufSizePointer, int nUuidsPointer)
		throws Exception
	{
		if (btBluetoothApiIsNotAvailable())
		{
			return IOCTL_UNAVAILABLE;
		}

		btPanicIfBluetoothPermissionsAreNotSet();

		ConcurrentLinkedQueue<BluetoothService> serviceList = mBluetoothServices;

		if (null == serviceList)
		{
			return CONNERR_GENERIC; // Error.
		}

		// Get but DO NOT remove head element of the queue.
		BluetoothService service = serviceList.peek();
		if (null == service)
		{
			return 0; // No elements.
		}

		// Copy length of service name to memory, includes null char.
		networkCopyIntToMemory(nameBufSizePointer, service.getNameLength());

		// Copy number of UUIDs to memory.
		networkCopyIntToMemory(nUuidsPointer, service.getNumberOfUUIDs());

		return 1; // Success retrieving service info.
	}
	*/

	/**
	 * Get info about the next service in the list of discovered services.
	 * @param portPointer ??
	 * @param nameBufPointer ??
	 * @param nameBufSize ??
	 * @param uuidsPointer ??
	 * @return > 0 on success, or zero if the queue is empty.
	 */
	/*
	int maBtGetNewService(
		int portPointer,
		int nameBufPointer,
		int nameBufSize,
		int uuidsPointer) throws Exception
	{
		if (btBluetoothApiIsNotAvailable())
		{
			return IOCTL_UNAVAILABLE;
		}

		btPanicIfBluetoothPermissionsAreNotSet();

		ConcurrentLinkedQueue<BluetoothService> serviceList = mBluetoothServices;

		if (null == serviceList)
		{
			return CONNERR_GENERIC; // Error.
		}

		// Get and remove head element of the queue.
		BluetoothService service = serviceList.poll();
		if (null == service)
		{
			return 0; // No elements.
		}

		// Copy port of service to memory.
		networkCopyIntToMemory(portPointer, service.getChannel());

		// Copy name of service to memory.

		// Truncate name if buffer is not large enough.
		// (Take null termination char into account.)
		String name = service.getName();
		int nameLength = name.length();
		if (nameLength >= nameBufSize - 1)
		{
			name = name.substring(nameBufSize - 1);
		}

		networkCopyStringToMemory(nameBufPointer, name);

		// Copy UUIDs to memory.
		//networkCopyBytesToMemory(uuidsPointer, service.getUUIDsAsByteBuffer());

		return 1; // Success retrieving device info.
	}
	*/

