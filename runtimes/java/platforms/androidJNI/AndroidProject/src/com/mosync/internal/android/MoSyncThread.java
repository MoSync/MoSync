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

package com.mosync.internal.android;

import static com.mosync.internal.android.MoSyncHelpers.EXTENT;
import static com.mosync.internal.android.MoSyncHelpers.SYSLOG;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_BLUETOOTH_TURNED_OFF;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_BLUETOOTH_TURNED_ON;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_SCREEN_STATE_OFF;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_SCREEN_STATE_ON;
import static com.mosync.internal.generated.MAAPI_consts.IOCTL_UNAVAILABLE;
import static com.mosync.internal.generated.MAAPI_consts.MAS_CREATE_IF_NECESSARY;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAMERA_RES_OK;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAMERA_RES_SNAPSHOT_IN_PROGRESS;
import static com.mosync.internal.generated.MAAPI_consts.MA_IMAGE_PICKER_EVENT_RETURN_TYPE_IMAGE_HANDLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NFC_NOT_AVAILABLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_RESOURCE_CLOSE;
import static com.mosync.internal.generated.MAAPI_consts.MA_RESOURCE_OPEN;
import static com.mosync.internal.generated.MAAPI_consts.MA_TOAST_DURATION_LONG;
import static com.mosync.internal.generated.MAAPI_consts.MA_TOAST_DURATION_SHORT;
import static com.mosync.internal.generated.MAAPI_consts.MA_WAKE_LOCK_ON;
import static com.mosync.internal.generated.MAAPI_consts.NOTIFICATION_TYPE_APPLICATION_LAUNCHER;
import static com.mosync.internal.generated.MAAPI_consts.RES_BAD_INPUT;
import static com.mosync.internal.generated.MAAPI_consts.RES_OK;
import static com.mosync.internal.generated.MAAPI_consts.RES_OUT_OF_MEMORY;
import static com.mosync.internal.generated.MAAPI_consts.STERR_GENERIC;
import static com.mosync.internal.generated.MAAPI_consts.STERR_NONEXISTENT;
import static com.mosync.internal.generated.MAAPI_consts.TRANS_MIRROR;
import static com.mosync.internal.generated.MAAPI_consts.TRANS_MIRROR_ROT180;
import static com.mosync.internal.generated.MAAPI_consts.TRANS_MIRROR_ROT270;
import static com.mosync.internal.generated.MAAPI_consts.TRANS_MIRROR_ROT90;
import static com.mosync.internal.generated.MAAPI_consts.TRANS_NONE;
import static com.mosync.internal.generated.MAAPI_consts.TRANS_ROT180;
import static com.mosync.internal.generated.MAAPI_consts.TRANS_ROT270;
import static com.mosync.internal.generated.MAAPI_consts.TRANS_ROT90;

import java.io.File;
import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.LineNumberReader;
import java.lang.reflect.Method;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.nio.channels.Channels;
import java.nio.channels.FileChannel;
import java.nio.channels.ReadableByteChannel;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Hashtable;
import java.util.Locale;
import java.util.TimeZone;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnKeyListener;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.PorterDuff.Mode;
import android.graphics.Rect;
import android.graphics.Region;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.opengl.GLUtils;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.SystemClock;
import android.os.Vibrator;
import android.provider.Settings.Secure;
import android.telephony.TelephonyManager;
import android.telephony.gsm.GsmCellLocation;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Window;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;
import android.widget.FrameLayout;
import android.widget.Toast;

import com.mosync.api.MoSyncContext;
import com.mosync.internal.android.MoSyncFont.MoSyncFontHandle;
import com.mosync.internal.android.extensions.MoSyncExtensionLoader;
import com.mosync.internal.android.nfc.MoSyncNFC;
import com.mosync.internal.android.nfc.MoSyncNFCService;
import com.mosync.internal.generated.IX_OPENGL_ES_MA;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.java.android.MoSync;
import com.mosync.java.android.MoSyncPanicDialog;
import com.mosync.java.android.MoSyncService;
import com.mosync.java.android.TextBox;
import com.mosync.nativeui.ui.widgets.MoSyncCameraPreview;
import com.mosync.nativeui.ui.widgets.ScreenWidget;
import com.mosync.nativeui.util.AsyncWait;
import com.mosync.nativeui.util.MediaManager;

/**
 * Thread that runs the MoSync virtual machine and handles all syscalls.
 * @author Anders Malm
 */
public class MoSyncThread extends Thread implements MoSyncContext
{
	private static boolean isNative = false;

	/**
	 * Global reference to the single instance of this class.
	 */
	private static MoSyncThread sMoSyncThread;

	// Declaration of native methods.
	public native boolean nativeInitRuntime();
	public native boolean nativeLoad(
		FileDescriptor program,
		long programOffset,
		FileDescriptor resource,
		long resourceOffset,
		boolean isNative);
	//public native boolean nativeLoadResource(ByteBuffer resource);
	public native boolean nativeLoadResource(
		FileDescriptor resource,
		long resoruceOffset,
		int handle,
		int placeholder);
	public native ByteBuffer nativeLoadCombined(ByteBuffer combined);
	public native void nativeRun();
	public native void nativeRun2(String appLibPath, String mainFn);
	public native ByteBuffer nativeGetMemorySlice(int addr, int len);
	public native void nativePostEvent(int[] eventBuffer);
	public native int nativeGetEventQueueSize();
	public native int nativeCreateBinaryResource(
		int resourceIndex,
		int length);
	public native int nativeCreatePlaceholder();
	public native void nativeExit();

	// Modules that handle syscalls for various subsystems.
	// We delegate syscalls from this class to the modules.
	MoSyncNetwork mMoSyncNetwork;
	MoSyncBluetooth mMoSyncBluetooth;
	MoSyncSound mMoSyncSound;
	MoSyncAudio mMoSyncAudio;
	MoSyncLocation mMoSyncLocation;
	MoSyncHomeScreen mMoSyncHomeScreen;
	MoSyncNativeUI mMoSyncNativeUI;
	MoSyncFile mMoSyncFile;
	// Module for device fonts.
	static MoSyncFont mMoSyncFont;
	MoSyncCameraController mMoSyncCameraController;
	MoSyncSMS mMoSyncSMS;
	MoSyncSensor mMoSyncSensor;
	MoSyncPIM mMoSyncPIM;
	MoSyncNFC mMoSyncNFC;
	MoSyncAds mMoSyncAds;
	MoSyncNotifications mMoSyncNotifications;
	MoSyncCapture mMoSyncCapture;
	MoSyncPurchase mMoSyncPurchase;
	MoSyncDB mMoSyncDB;
	MoSyncExtensionLoader mMoSyncExtensionLoader;
	MoSyncOrientationHelper mOrientation;

	/**
	 * Synchronization monitor for postEvent
	 */
	private final Object mPostEventMonitor = new Object();

	static final String PROGRAM_FILE = "program.mp3";
	static final String RESOURCE_FILE = "resources.mp3";

	static boolean loadedNativeLibs = false;

	/**
	 * Chunk size used when loading resources.
	 */
	private static final int CHUNK_READ_SIZE = 1024;

	/**
	 * This is the MoSync Activity.
	 */
	private final MoSync mContext;

	/**
	 * The standard MoSync view.
	 */
	private MoSyncView mMoSyncView;

	/**
	 * Flag that tells if the display should be updated.
	 */
	volatile private boolean mUpdateDisplay = true;

	/**
	 * true if the MoSync program is considered to be dead,
	 * used for maPanic.
	 */
	volatile private boolean mHasDied;

	/**
	 * a handle used for full screen camera preview
	 */
	private int cameraScreen;

	FileDescriptor mResourceFd = null;

	/**
	 * This is the size of the header of the asset file
	 * (an Android file header, this is not a MoSync header).
	 * The actual resource data is found at this offset.
	 * On the Java side an input stream to the asset file
	 * will begin at this offset.
	 * UBinData resources have offsets that start at the
	 * very beginning of the resource file.
	 * Therefore mResourceOffset needs to be used on the
	 * Java side when reading data from ubin resource file.
	 */
	private long mResourceOffset = 0;

	ByteBuffer mRecompilerCodeBlock;
	ByteBuffer mRecompilerEntryBlock;

	/**
	 * Table that holds binary resources.
	 */
	Hashtable<Integer, ByteBuffer> mBinaryResources =
		new Hashtable<Integer, ByteBuffer>();

	/**
	 * Table that holds ubin resources.
	 */
	Hashtable<Integer, UBinData> mUBinaryResources =
		new Hashtable<Integer, UBinData>();

	/**
	 * Table that holds image resources.
	 */
	Hashtable<Integer, ImageCache> mImageResources =
		new Hashtable<Integer, ImageCache>();

	/**
	 * Table that holds data stores.
	 */
	Hashtable<Integer, String> mStores =
		new Hashtable<Integer, String>();

	// Various variables, should be moved to subsystems
	// along with the syscalls.
	private ByteBuffer mMemDataSection;
	ByteBuffer mResourceFile;

	Canvas mCanvas;
	Bitmap mBitmap;

	int mWidth;
	int mHeight;

	int mClipLeft, mClipTop, mClipWidth, mClipHeight;

	volatile boolean mUsingFrameBuffer;
	int mFrameBufferAddress;
	int mFrameBufferSize;
	Bitmap mFrameBufferBitmap;

	ImageCache mDrawTargetScreen = null;
	ImageCache mDrawTarget = null;
	int mDrawTargetHandle = 0;

	Paint mPaint = new Paint();

	Paint mBlitPaint = new Paint();

	int mTextConsoleHeight;

	/**
	 * Flag used to signal if the thread is sleeping.
	 */
	private volatile boolean mIsSleeping;

	/**
	 * Ascent of text in the default console font.
	 */
	//int mTextConsoleAscent;

	/**
	 * Rectangle that is used to get the extent of a text string.
	 */
	Rect mTextSizeRect = new Rect();

	// Rectangle objects used for drawing in maDrawImageRegion().
	private final Rect mMaDrawImageRegionTempSourceRect = new Rect();
	private final Rect mMaDrawImageRegionTempDestRect = new Rect();

	/**
	 * An Instance of Connectivity Manager used for detecting connection type
	 */
	private ConnectivityManager mConnectivityManager;

	int mMaxStoreId = 0;

	// TODO: Make this private and access via a method.
	volatile public boolean mIsUpdatingScreen = false;

	final static String storesPath = "MAStore";

	/**
	 * BroadcastReceiver that handles screen on/off events.
	 */
	BroadcastReceiver mScreenActivatedReceiver = null;

	String mNativeLibraryDir;

	private HashMap<String, String> mAppLibPaths = new HashMap<String, String>();

	private ArrayList<String> mDeferredModules = new ArrayList<String>();

	private HashSet<String> mStaticModules = new HashSet<String>();

	private String mUserLib = null;

	/**
	 * MoSyncThread constructor
	 */
	public MoSyncThread(Context context, Handler handler) throws Exception
	{
		mContext = (MoSync) context;
		loadNativeLibraries(context);

		// TODO: Clean this up! The static reference should be in one place.
		// Now the instance of MoSyncThread is passed to many classes and
		// also accessed via the static variables. We use use one consistent way.
		EventQueue.sMoSyncThread = this;
		sMoSyncThread = this;

		MoSyncError.getSingletonObject().setThread(this);

		mHasDied = false;

		mIsSleeping = false;

		mMoSyncNetwork = new MoSyncNetwork(this);
		mMoSyncSound = new MoSyncSound(this);
		mMoSyncAudio = new MoSyncAudio(this);
		mMoSyncLocation = new MoSyncLocation(this);
		mMoSyncHomeScreen = new MoSyncHomeScreen(this);
		mMoSyncNativeUI = new MoSyncNativeUI(this, mImageResources);
		mMoSyncFile = new MoSyncFile(this);

		try
		{
			mMoSyncFont = new MoSyncFont(this);
		}
		catch (Throwable e)
		{
			mMoSyncFont = null;
		}

		cameraScreen = 0;
		//Do not access camera if it is not available
		try
		{
			Boolean isCameraAccessGranted =
					(PackageManager.PERMISSION_GRANTED ==
					getActivity().checkCallingOrSelfPermission("android.permission.CAMERA"));
			if ( isCameraAccessGranted )
			{
				mMoSyncCameraController = new MoSyncCameraController(this);
			}
			else
			{
				mMoSyncCameraController = null;
			}
		}
		catch (Throwable e)
		{
			mMoSyncCameraController = null;
		}

		// Bluetooth is not available on all platforms and
		// therefore we do a conditional loading of the
		// MoSyncBluetooth class.
		try
		{
			mMoSyncBluetooth = new MoSyncBluetooth(this);
		}
		catch (java.lang.VerifyError error)
		{
			mMoSyncBluetooth = null;
		}

		// SMS is not available on all platforms and
		// therefore we do a conditional loading of the
		// MoSyncSMS class.
		try
		{
			mMoSyncSMS = new MoSyncSMS(this);
		}
		catch (java.lang.VerifyError error)
		{
			mMoSyncSMS = null;
		}

		//nativeInitRuntime();
		mMoSyncPurchase = new MoSyncPurchase(this);

		mMoSyncSensor = new MoSyncSensor(this);

		mMoSyncPIM = new MoSyncPIM(this);

		try
		{
			mMoSyncNFC = MoSyncNFCService.getDefault();
			if (mMoSyncNFC != null)
			{
				mMoSyncNFC.setMoSyncThread(this);
			}
		}
		catch (Throwable t)
		{
			mMoSyncNFC = null;
		}

		mMoSyncAds = new MoSyncAds(this);

		mMoSyncNotifications = new MoSyncNotifications(this);

		mMoSyncCapture = new MoSyncCapture(this, mImageResources);

		mMoSyncDB = new MoSyncDB();

		mConnectivityManager = (ConnectivityManager) mContext.getSystemService(Context.CONNECTIVITY_SERVICE);

		mMoSyncExtensionLoader = MoSyncExtensionLoader.getDefault();

		mOrientation = new MoSyncOrientationHelper(mContext);

		nativeInitRuntime();
	}

	private synchronized void loadNativeLibraries(Context context) {
		try {
			InputStream depsList = context.getAssets().open("startup.mf");
			LineNumberReader reader = new LineNumberReader(new InputStreamReader(depsList, Charset.forName("UTF-8")));
			for (String line = reader.readLine(); line != null; line = reader.readLine()) {
				if (line.length() > 0) {
					String[] moduleAndInitFn = line.split(":", 3);
					String libType = moduleAndInitFn[0];
					boolean shared = "SHARED".equalsIgnoreCase(libType);
					String module = moduleAndInitFn[1];
					mUserLib = module;
					String initFn = moduleAndInitFn.length > 2 ? moduleAndInitFn[2] : null;
					if (initFn != null) {
						// Deferred init until nativeRun2 is executed.
						// Static ones will be called using the last, main lib.
						mDeferredModules.add(module);
						if (!shared) {
							mStaticModules.add(module);
						}
						mAppLibPaths.put(module, initFn);
					}
					if (shared) {
						System.loadLibrary(module);
					}
				}
			}
			mNativeLibraryDir = context.getApplicationInfo().nativeLibraryDir;
			isNative = true;
		} catch (Exception e) {
			// The native runtime
			System.loadLibrary("mosync");
		}
	}

	public static MoSyncThread getInstance()
	{
		return sMoSyncThread;
	}

	public void onResume()
	{
		// Turn on display update again.
		mUpdateDisplay = true;

		// Turn on sensors.
		mMoSyncSensor.onResume();
		mMoSyncPurchase.restoreService();
	}

	public void onPause()
	{
		// Do not update the display when paused.
		mUpdateDisplay = false;

		// Pause sensors.
		mMoSyncSensor.onPause();
	}

	/**
	 * Called from the MoSync activity in onDestroy.
	 * Do cleanup here (but it is not guaranteed that
	 * this will be called.)
	 */
	public void onDestroy()
	{
		if (null != mMoSyncBluetooth)
		{
			// Delegate onDestroy to the Bluetooth object.
			mMoSyncBluetooth.onDestroy();
			mMoSyncBluetooth = null;
		}

		if(null != mMoSyncNetwork)
		{
			mMoSyncNetwork.killAllConnections();
			mMoSyncNetwork = null;
		}
		if (null != mMoSyncPurchase)
		{
			/**
			 * Unbind from the MarketBillingService.
			 */
			mMoSyncPurchase.unbindService();
			mMoSyncPurchase = null;
		}
	}

	/**
	 * Return the activity that this thread is related to.
	 * @return the Activity object.
	 */
	public Activity getActivity()
	{
		return mContext;
	}

	/**
	 * Check if the Bluetooth API is available.
	 * @return true if the API is available.
	 */
	public boolean isBluetoothApiAvailable()
	{
		return mMoSyncBluetooth != null;
	}

	/**
	 * Called from broadcast listener in MoSyncBluetooth.
	 */
	public void bluetoothTurnedOn()
	{
		// Send event.
		int[] event = new int[1];
		event[0] = EVENT_TYPE_BLUETOOTH_TURNED_ON;
		postEvent(event);
	}

	/**
	 * Called from broadcast listener in MoSyncBluetooth.
	 */
	public void bluetoothTurnedOff()
	{
		// Close Bluetooth connections.
		mMoSyncNetwork.bluetoothTurnedOff();

		// Send event.
		int[] event = new int[1];
		event[0] = EVENT_TYPE_BLUETOOTH_TURNED_OFF;
		postEvent(event);
	}

	/**
	 * Check if the Font API is available.
	 * @return true if the API is available.
	 */
	public boolean isFontApiAvailable()
	{
		return mMoSyncFont != null;
	}

	/**
	 * setMoSyncView
	 */
	public void setMoSyncView(MoSyncView moSyncView)
	{
		mMoSyncView = moSyncView;

		// Each time the MoSync view is changed its reference
		// must be updated in native UI.
		mMoSyncNativeUI.setMoSyncScreen( moSyncView );
	}

	/**
	 * @return true of the application is considered to to have died.
	 */
	public boolean isDead()
	{
		return mHasDied;
	}

	/**
	 * Update the size of the drawing surface.
	 */
	public synchronized void updateSurfaceSize(int width, int height)
	{
		SYSLOG("updateSurfaceSize");

		// If the surface has same width and height we
		// do not create a new canvas.
		if((mWidth == width) && (mHeight == height)) return;

		mWidth = width;
		mHeight = height;

		Bitmap bitmap = createBitmap(mWidth, mHeight);

		Canvas canvas = new Canvas(bitmap);
		mDrawTargetScreen = new ImageCache(canvas, bitmap);

		mCanvas = mDrawTargetScreen.mCanvas;
		mBitmap = mDrawTargetScreen.mBitmap;

		// Set/reset clip rect.
		mClipLeft = 0;
		mClipTop = 0;
		mClipWidth = mWidth;
		mClipHeight = mHeight;

		// Set original clip rect.
		// Save the clip state since we have a new canvas created.
		// This initial save is needed for the clip rect stack to
		// work correctly when calling restore clip rect.
		mCanvas.save();
		mCanvas.clipRect(mClipLeft, mClipTop, mClipWidth, mClipHeight, Region.Op.REPLACE);

		updateScreen();

		SYSLOG("mBitmap width:" + mBitmap.getWidth()
			+ " height:" + mBitmap.getHeight());
	}

	/**
	 * @brief Returns a slice of the memory ByteBuffer
	 *
	 * All calls which are accessing memory shall use this function!
	 * The use of .position in any other call is strictly forbidden!
	 *
	 * @param addr	The address to the beginning of memory block
	 * @param len	The size of the block, -1 means that the whole
	 * 				block should be used.
	 * @return
	 */
	public synchronized ByteBuffer getMemorySlice(int addr, int len)
	{
		if (len < 0) {
			maPanic(1, "Negative memory size");
		}
		if (isNative) {
			return nativeGetMemorySlice(addr, len);
		} else {
			mMemDataSection.position(addr);
			ByteBuffer slice = mMemDataSection.slice().order(null);
			if(-1 != len)
				slice.limit(len);
			return slice;
		}
	}

	/**
	 * Allocate memory for the program data section.
	 */
	 public boolean generateDataSection(ByteBuffer byteBuffer)
	{
		try
		{
			mMemDataSection = byteBuffer;
			mMemDataSection.order(null);
		}
		catch (Exception e)
		{
			logError("MoSyncThread - Out of Memory!", e);
			mMemDataSection = null;
			return false;
		}
		catch (Error e)
		{
			logError("MoSyncThread - Out of Memory!", e);
			mMemDataSection = null;
			return false;
		}
		return true;
	}

	/**
	 * Issue a panic. Launches a panic dialog.
	 * @param errorCode
	 * @param message
	 */
	public void threadPanic(int errorCode, String message)
	{
		// Print debug data to the logcat console.
		Log.e("@@@ MoSync",
			"threadPanic errorCode: " + errorCode + " message: " + message);
		new Exception("STACKTRACE: threadPanic").printStackTrace();

		mHasDied = true;

		// TODO: Run on UI thread?

		// Launch panic dialog.
		MoSyncPanicDialog.sPanicMessage = message;
		Intent myIntent = new Intent(
			mMoSyncView.getContext(), MoSyncPanicDialog.class);
		mMoSyncView.getContext().startActivity(myIntent);

		while(true)
		{
			try
			{
				sleep(500);
			}
			catch(Exception e)
			{
				Log.i("MoSync Thread","oops.. got an exception, conutine until application ends");
			}
		}
	}

	/**
	 * Loads the MoSync program.
	 *
	 * This function loads the program file and the resource file.
	 * It loads all the resources and generates the data
	 * section used by the program.
	 *
	 * @return false if failed, true if succeeded.
	 */
	public boolean loadProgram()
	{
		SYSLOG("loadProgram");
		try
		{
			AssetManager assetManager = mContext.getAssets();
			FileDescriptor rFd = null;
			mResourceOffset = 0;

			// Try to load the resource file, if we get an exception
			// it just means that this application has no resource file
			// and that is not an error.
			try
			{
				AssetFileDescriptor rAfd = assetManager.openFd(RESOURCE_FILE);
				rFd = rAfd.getFileDescriptor();
				mResourceOffset = rAfd.getStartOffset();
			}
			catch (FileNotFoundException fnfe)
			{
				Log.i("@@MoSync", "No resources");
			}

			// We have a program file so now we sends it to the native side
			// so it will be loaded into memory. The data section will also be
			// created and if there are any resources they will be loaded.
			FileDescriptor pFd = null;
			long pFdOffset = 0;
			try {
				AssetFileDescriptor pAfd = assetManager.openFd(PROGRAM_FILE);
				pFd = pAfd.getFileDescriptor();
				pFdOffset = pAfd.getStartOffset();
			} catch (FileNotFoundException fnfe) {
				Log.i("@@MoSync", "No program file");
			}

			if (isNative || (null != pFd))
			{
				if (false == nativeLoad(pFd, pFdOffset, rFd, mResourceOffset,
						isNative))
				{
					logError("loadProgram - "
							+ "ERROR Load program was unsuccesfull");

					if (null == mMemDataSection) {
						threadPanic(0, "This device does not have enough"
								+ "memory to run this application.");
					} else {
						threadPanic(0, "Unable to load program or resources");
					}

					return false;
				}

				SYSLOG("loadProgram - Load program was succesfull");
				return true;
			}
		}
		catch (Exception e)
		{
			logError("loadProgram - Exception while loading program:"
				+ e.toString(), e);
		}
		return false;
	}

	/**
	 * Get file descriptor for the resource file.
	 */
	public FileDescriptor getResourceFileDesriptor()
	{
		try
		{
			AssetManager assetManager = mContext.getAssets();
			AssetFileDescriptor pAfd = assetManager.openFd(RESOURCE_FILE);
			FileDescriptor pFd = pAfd.getFileDescriptor();
			mResourceOffset = pAfd.getStartOffset();
			return pFd;
		}
		catch (Exception e)
		{
			logError("getResourceFileDesriptor exception:" + e.toString(), e);
		}
		return null;
	}

	/**
	 * Get the offset to the start of the resources.
	 */
	public int getResourceStartOffset()
	{
		return (int) mResourceOffset;
	}

	/**
	 * Read data from an input stream to a ByteBuffer.
	 * @param is The imput stream.
	 * @return ByteBuffer with the data.
	 * @throws Exception
	 */
	public synchronized ByteBuffer readInputStream(InputStream is) throws Exception
	{
		ReadableByteChannel byteChannel = Channels.newChannel(is);
		ByteBuffer byteBuffer = ByteBuffer.allocateDirect(CHUNK_READ_SIZE);

		while (byteChannel.read(byteBuffer) != -1)
		{
			if (byteBuffer.remaining() < CHUNK_READ_SIZE)
			{
				ByteBuffer temp =
					ByteBuffer.allocateDirect(byteBuffer.capacity() * 2);
				byteBuffer.flip();
				temp.put(byteBuffer);
				byteBuffer = temp;
			}
		}

		return byteBuffer;
	}

	public boolean isNative() {
		return isNative;
	}

	/**
	 * Create a data object by (indirectly) calling maCreatePlaceholder
	 * (if no placeholder is supplied), and maCreateData, then copy the
	 * data given in the data parameter to the new buffer.
	 * @param placeholder The placeholder that should refer to the data,
	 * if this parameter is zero, a new placeholder is created.
	 * @param data The data to fill the new data object with.
	 * @return The handle to the data if successful, <0 on error.
	 * If a placeholder is supplied, that value is returned on success.
	 *
	 * TODO: This method needs improved error checking.
	 */
	public int createDataObject(int placeholder, byte[] data)
	{
		// The handle to the data.
		int dataHandle = placeholder;

		// Create handle if no placeholder is given.
		if (0 == placeholder)
		{
			// This calls maCreatePlaceholder.
			// TODO: Check return value for error.
			dataHandle = nativeCreatePlaceholder();
		}
		// Allocate data. This calls maCreateData and will create a
		// new data object.
		int result = nativeCreateBinaryResource(dataHandle, data.length);
		if (result < 0)
		{
			return result;
		}

		// Get byte buffer for the handle and copy data
		// to the data object.
		// TODO: Handle error if this fails.
		ByteBuffer buf = getBinaryResource(dataHandle);
		if (null != buf)
		{
			// Copy data.
			buf.put(data);
		}

		return dataHandle;
	}

	/**
	 * Called from JNI, delegate to the audio module.
	 * @param soundHandle
	 */
	void storeIfBinaryAudioResource(int soundHandle)
	{
		mMoSyncSound.storeIfBinaryAudioResource(soundHandle, 0);
	}

	/*
	 * @brief Function that decodes an image into a Bitmap
	 *
	 * @param	data	The decoded image data
	 * @param	options	The bitmapFactory options
	 *
	 * @return	The created Bitmap, null if it failed
	 *
	 * TODO: There is no need for this method to be synchronized.
	 * Find out why it was synchronized.
	 */
	//synchronized
	Bitmap decodeImageFromData(
		final byte[] data,
		final BitmapFactory.Options options)
	{
		{
			final AsyncWait<Bitmap> waiter = new AsyncWait<Bitmap>();
			getActivity().runOnUiThread(new Runnable()
			{
				public void run()
				{
					// Here we can get:
					// java.lang.OutOfMemoryError: bitmap size exceeds VM budget
					try
					{
						Bitmap bitmap = BitmapFactory.decodeByteArray(
							data, 0, data.length, options);
						waiter.setResult(bitmap);
					}
					catch (OutOfMemoryError e1)
					{
						//Log.i("@@@", "decodeImageFromData - " + "Out of memory error : " + e1);
						waiter.setResult(null);
					}
					catch (Throwable e2)
					{
						//Log.i("@@@", "decodeImageFromData - " + "Error : " + e2);
						waiter.setResult(null);
					}
				}
			});
			return waiter.getResult();
		}
	}

	/*
	 * @brief Recycles the bitmap
	 *
	 * @param	bitmap	The bitmap which is being recycled
	 */
	void recycleImageData(final Bitmap bitmap)
	{
		getActivity().runOnUiThread(new Runnable()
		{
			public void run()
			{
				bitmap.recycle();
			}
		});
		return;
	}

	/*
	 * @brief Function that creates an empty Bitmap
	 *
	 * @param	width	The width of the created Bitmap
	 * @param	height	The width of the created Bitmap
	 *
	 * @return	The created Bitmap, null if it failed
	 *
	 * TODO: There is no need for this method to be synchronized.
	 * Find out why it was synchronized.
	 *
	 * TODO: Look into eliminating code duplication between
	 * createBitmap and createBitmapFromData.
	 */
	//synchronized
	Bitmap createBitmap(final int width, final int height)
	{
		{
			final AsyncWait<Bitmap> waiter = new AsyncWait<Bitmap>();
			getActivity().runOnUiThread(new Runnable()
			{
				public void run()
				{
					// Here we can get:
					// java.lang.OutOfMemoryError: bitmap size exceeds VM budget
					try
					{
						Bitmap bitmap = Bitmap.createBitmap(
							width, height, Bitmap.Config.ARGB_8888);

						waiter.setResult(bitmap);
					}
					catch (OutOfMemoryError e1)
					{
						//Log.i("@@@", "createBitmapFromData - " + "Out of memory error : " + e1);
						waiter.setResult(null);
					}
					catch (Throwable e2)
					{
						//Log.i("@@@", "createBitmapFromData - " + "Error : " + e2);
						waiter.setResult(null);
					}
				}
			});
			return waiter.getResult();
		}
	}

	/*
	 * @brief Function that creates a Bitmap from pixel data
	 *
	 * @param	width	The width of the created Bitmap
	 * @param	height	The width of the created Bitmap
	 * @param	pixels	The pixel data
	 *
	 * @return	The created Bitmap, null if it failed
	 *
	 * TODO: There is no need for this method to be synchronized.
	 * Find out why it was synchronized.
	 */
	//synchronized
	Bitmap createBitmapFromData(
		final int width,
		final int height,
		final int[] pixels)
	{
		{
			final AsyncWait<Bitmap> waiter = new AsyncWait<Bitmap>();
			getActivity().runOnUiThread(new Runnable()
			{
				public void run()
				{
					// Here we can get:
					// java.lang.OutOfMemoryError: bitmap size exceeds VM budget
					try
					{
						Bitmap bitmap = Bitmap.createBitmap(
							pixels, width, height, Bitmap.Config.ARGB_8888);

						waiter.setResult(bitmap);
					}
					catch (OutOfMemoryError e1)
					{
						//Log.i("@@@", "createBitmapFromData - " + "Out of memory error : " + e1);
						waiter.setResult(null);
					}
					catch (Throwable e2)
					{
						//Log.i("@@@", "createBitmapFromData - " + "Error : " + e2);
						waiter.setResult(null);
					}
				}
			});
			return waiter.getResult();
		}
	}

	/**
	 * Redraws the screen.
	 */
	public void updateScreen()
	{
		maUpdateScreen();
	}

	/**
	 * Main entry point for the thread. This is where
	 * the main loop is called.
	 */
	@Override
	public void run()
	{
		//Log.i("MoSync Thread", "run");

		// Load the program.
		if (false == loadProgram() && !isNative)
		{
			logError("load program failed!!");
			return;
		}

		// Run program. This enters a while loop in C-code.
		if (isNative) {
			for (String deferred : mDeferredModules) {
				String lib = mStaticModules.contains(deferred) ? mUserLib : deferred;
				runInitFunction(lib, mAppLibPaths.get(deferred));
			}
			mContext.finish();
			nativeExit();
		} else {
			nativeRun();
		}
	}

	private void runInitFunction(String module, String initFn) {
		String lib = mNativeLibraryDir + "/lib" + module + ".so";
		nativeRun2(lib, initFn);
	}

	public static void logError(String message , Throwable t)
	{
		logError(message);
		t.printStackTrace();
	}

	public static void logError(String message)
	{
		Log.e("Dalvik Syscall ERROR", message);
	}

	public ImageCache getImageResource(int handle)
	{
		return mImageResources.get(handle);
	}

	public ByteBuffer getBinaryResource(int handle)
	{
		return mBinaryResources.get(handle);
	}

	public byte[] getUnloadedBinaryResourceAsByteArray(int handle)
	{
		UBinData uBinData = mUBinaryResources.get(handle);
		if (null == uBinData)
		{
			// Handle not found.
			return null;
		}

		try
		{
			// Read data from the resource asset file.
			// All resources are bundled into this file.
			AssetManager assetManager = mContext.getAssets();
			InputStream resourceFileInputStream =
				assetManager.open(RESOURCE_FILE);

			// Position the stream pointer to the offset where
			// the data starts.
			resourceFileInputStream.skip(
				uBinData.getOffset() - getResourceStartOffset());

			// Allocate byte array to hold data.
			byte[] resourceData = new byte[uBinData.getSize()];

			// Read from asset file into the data array.
			resourceFileInputStream.read(resourceData);

			// Close input stream.
			resourceFileInputStream.close();

			return resourceData;
		}
		catch (Exception ex)
		{
			ex.printStackTrace();
			return null;
		}
	}

	void initSyscalls()
	{
		SYSLOG("initSyscalls");

		//Log.i("@@@@@", "initSyscalls");

		mUsingFrameBuffer = false;

		mPaint.setStyle(Paint.Style.FILL);
		mPaint.setAntiAlias(false);
		mPaint.setColor(0xffffffff);

		mBlitPaint.setAntiAlias(false);
		mBlitPaint.setColor(0xffffffff);
		mBlitPaint.setAlpha(255);

		// Generates a default text height used for console text writing.
		// This is used so that all text which is printed to the console
		// gets the same text height.
		// Old code:
		//int extent = maGetTextSize(
		//	"abcdefghijklmnopqrstuvwxyz" +
		//	"ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
		//mTextConsoleHeight = EXTENT_Y(extent);

		// New code uses font metrics. Ascent is a negative number,
		// thus multiplying by -1 to get a positive number.
		Paint.FontMetricsInt fontMetrics =
			new Paint.FontMetricsInt();
		mPaint.getFontMetricsInt(fontMetrics);
		mTextConsoleHeight = -1 * fontMetrics.ascent;
	}

	/**
	 * maSetColor
	 */
	int maSetColor(int rgb)
	{
		SYSLOG("maSetColor");

		int t = mPaint.getColor();
		mPaint.setColor(rgb);
		mPaint.setAlpha(255);
		return t;
	}

	/**
	 * maSetClipRect
	 */
	void maSetClipRect(int left, int top, int width, int height)
	{
		SYSLOG("maSetClipRect");

		mClipLeft = left;
		mClipTop = top;
		mClipWidth = width;
		mClipHeight = height;

		// Restore clip state and save before setting the clip rect.
		// Note that we do an initial save of the clip state in initSyscalls.
		mCanvas.restore();
		mCanvas.save();
		mCanvas.clipRect(left, top, left+width, top+height, Region.Op.REPLACE);
	}

	/**
	 * maPlot
	 */
	void maPlot(int posX, int posY)
	{
		SYSLOG("maPlot");

		mCanvas.drawPoint(posX, posY, mPaint);
	}

	/**
	 * maLine
	 */
	void maLine(int startX, int startY, int endX, int endY)
	{
		SYSLOG("maLine");

		mCanvas.drawLine(startX, startY, endX, endY, mPaint);
	}

	/**
	 * maFillRect
	 */
	void maFillRect(int left, int top, int width, int height)
	{
		SYSLOG("maFillRect");

		mCanvas.drawRect(left, top, left+width, top+height, mPaint);
	}

	/**
	 * This implementation might seem a bit strange but it works so
	 * much better than using Canvas.drawVertices.
	 */
	void maFillTriangleStrip(int address, int count)
	{
		SYSLOG("maFillTriangleStrip");

		if (count < 3)
		{
			maPanic(1,"maFillTriangleStrip takes more than 3 vertices");
		}

		//mMemDataSection.position(address);
		//IntBuffer ib = mMemDataSection.asIntBuffer();

		int[] vertices = new int[count*2];

		IntBuffer ib = getMemorySlice(address, 4 * vertices.length).order(null).asIntBuffer();

		ib.get(vertices);

		Path path = new Path();

		int i = 0;

		int xa = vertices[i++];
		int ya = vertices[i++];

		int xb = vertices[i++];
		int yb = vertices[i++];
		for(;;) {
			int xc = vertices[i++];
			int yc = vertices[i++];
			path.moveTo(xa,ya);
			path.lineTo(xb,yb);
			path.lineTo(xc,yc);
			path.close();

			if(i >= count*2)
				break;
			xa = vertices[i++];
			ya = vertices[i++];
			path.moveTo(xb,yb);
			path.lineTo(xc,yc);
			path.lineTo(xa,ya);
			path.close();

			if(i >= count*2)
				break;
			xb = vertices[i++];
			yb = vertices[i++];
			path.moveTo(xc,yc);
			path.lineTo(xa,ya);
			path.lineTo(xb,yb);
			path.close();

			if(i >= count*2)
				break;
		}
		mCanvas.drawPath(path, mPaint);
	}


	/**
	 * maFillTriangleFan
	 */
	void maFillTriangleFan(int address, int count)
	{
		SYSLOG("maFillTriangleFan");

		if (count < 3)
		{
			maPanic(1, "maFillTriangleFan takes more than 3 vertices");
		}

		//mMemDataSection.position(address);
		//IntBuffer ib = mMemDataSection.asIntBuffer();

		int[] vertices = new int[count*2];

		IntBuffer ib = getMemorySlice(address, 4 * vertices.length).order(null).asIntBuffer();

		ib.get(vertices);

		Path path = new Path();

		int i = 0;

		int xa = vertices[i++];
		int ya = vertices[i++];

		int xb = vertices[i++];
		int yb = vertices[i++];
		for(;;) {
			int xc = vertices[i++];
			int yc = vertices[i++];
			path.moveTo(xa,ya);
			path.lineTo(xb,yb);
			path.lineTo(xc,yc);
			path.close();

			if(i >= count*2)
				break;
			xb = vertices[i++];
			yb = vertices[i++];
			path.moveTo(xa,ya);
			path.lineTo(xc,yc);
			path.lineTo(xb,yb);
			path.close();

			if(i >= count*2)
				break;
		}
		mCanvas.drawPath(path, mPaint);
	}

	/**
	 * Gets the height in pixels of the string.
	 *
	 * Calculates the size pixels of this string, as it's gonna
	 * be drawn to the screen.
	 *
	 * @param str The string which the height should be calculated from.
	 *
	 * @return the height in pixels.
	 */
	int maGetTextSize(final String str)
	{
		SYSLOG("maGetTextSize");

		mPaint.getTextBounds(str, 0, str.length(), mTextSizeRect);

		// Old code:
		// return EXTENT(mTextSizeRect.width(), mTextSizeRect.height());

		return EXTENT(mTextSizeRect.width(), mTextConsoleHeight);
	}

	/**
	 * Gets the height in pixels of the string.
	 *
	 * Calculates the size in pixels of this string, as it's gonna
	 * be drawn to the screen.
	 *
	 * @param str The string which the height should be calculated from.
	 *
	 * @return the height in pixels.
	 */
	int maGetTextSizeW(final String str)
	{
		SYSLOG("maGetTextSizeW");

		mPaint.getTextBounds(str, 0, str.length(), mTextSizeRect);

		return EXTENT(mTextSizeRect.width(), mTextConsoleHeight);
	}

	/**
	* Returns a handle to one of the default fonts of the device, in the style and size you specify.
	* \param 'type' The type of the font, can be FONT_TYPE_[SANS_SERIF,SERIF,MONOSPACE].
	* \param 'style' The style of the font, can be FONT_STYLE_[NORMAL,BOLD,ITALIC].
	* \param 'size' The size of the font.
	* \return The handle to the font, RES_FONT_NO_TYPE_STYLE_COMBINATION, or RES_FONT_INVALID_SIZE.
	*/
	int maFontLoadDefault(int type, int style, int size)
	{
		if(null == mMoSyncFont)
		{
			return -1;
		}
		SYSLOG("maFontCreateDefault");

		return mMoSyncFont.maFontLoadDefault(type, style, size);
	}

	/**
	* Sets the font to be used with maDrawText and maDrawTextW, and returns the handle
	* to the previous font
	* \param 'font' an MAHandle for a font object
	* \return The handle to the previous font, or RES_FONT_INVALID_HANDLE
	*/
	int maFontSetCurrent(int fontHandle)
	{
		if(null == mMoSyncFont)
		{
			return -1;
		}

		SYSLOG("maFontSetCurrent");

		// change the current font and update the mPaint
		int previousFontHandle = mMoSyncFont.maFontSetCurrent(fontHandle);

		// update the font "metrics"
		Paint.FontMetricsInt fontMetrics = new Paint.FontMetricsInt();
		mPaint.getFontMetricsInt(fontMetrics);
		mTextConsoleHeight = -1 * fontMetrics.ascent;

		return previousFontHandle;
	}

	/**
	* Returns the number of fonts that are available in the system
	* \return Number of fonts
	* \see maFontGetName
	*/
	int maFontGetCount()
	{
		if(null == mMoSyncFont)
		{
			return -1;
		}
		SYSLOG("maFontGetCount");

		return mMoSyncFont.maFontGetCount();
	}

	/**
	* Copies the font postscript name of the given index to the buffer.
	* You must have first called maFontGetCount() at least once before calling this function.
	* \param 'index' A 0-based index to the font
	* \param 'buffer' An empty char buffer that will receive the font name
	* \param 'bufferLen' The size of the buffer
	* \return The number of bytes copied (with terminating NULL) or RES_FONT_INDEX_OUT_OF_BOUNDS
	* 		or RES_FONT_INSUFFICIENT_BUFFER or RES_FONT_LIST_NOT_INITIALIZED
	* \see maFontGetCount, maFontLoadWithName
	*/
	int maFontGetName(int index,
			final int memBuffer,
			final int memBufferSize)
	{
		if(null == mMoSyncFont)
		{
			return -1;
		}
		SYSLOG("maFontGetName");

		return mMoSyncFont.maFontGetName(index, memBuffer, memBufferSize);
	}

	/**
	* Returns a handle to a font with the specific postscript name and size
	* \param 'postScriptName' The postscript name of the font
	* \param 'size' The size of the font
	* \return Handle to the font, or RES_FONT_NAME_NONEXISTENT
	*/
	int maFontLoadWithName(final String postScriptName, int size)
	{
		if(null == mMoSyncFont)
		{
			return -1;
		}
		SYSLOG("maFontLoadWithName");

		return mMoSyncFont.maFontLoadWithName(postScriptName, size);
	}

	/**
	* Deletes a loaded font
	* \param 'font' A font handle
	* \return RES_FONT_OK, RES_FONT_INVALID_HANDLE, or RES_FONT_DELETE_DENIED.
	*/
	int maFontDelete(int fontHandle)
	{
		if(null == mMoSyncFont)
		{
			return -1;
		}
		SYSLOG("maFontDelete");

		return mMoSyncFont.maFontDelete(fontHandle);
	}

	/**
	 * Get a font in the list based on key.
	 * @param fontHandle A font handle.
	 * @return The font handle object.
	 */
	public static MoSyncFontHandle getMoSyncFont(int fontHandle)
	{
		return mMoSyncFont.getMoSyncFont(fontHandle);
	}

	/**
	 * Draws a string of text to the screen.
	 *
	 * @param left The left most pixel on which the text should be rendered.
	 * @param top The top most pixel on which the text should be rendered.
	 */
	void maDrawText(int left, int top, final String str)
	{
		SYSLOG("maDrawText");

		mCanvas.drawText( str, left, top + mTextConsoleHeight, mPaint);
	}

	/**
	 * Draws a string of text to the screen.
	 *
	 * @param left The left most pixel on which the text should be rendered.
	 * @param top The top most pixel on which the text should be rendered.
	 */
	void maDrawTextW(int left, int top, final String str)
	{
	 	SYSLOG("maDrawTextW");

		mCanvas.drawText(str, left, top + mTextConsoleHeight, mPaint);
	}

	/**
	 * maUpdateScreen
	 * Sets the boolean mIsUpdatingScreen when updating the screen
	 * so that we won't get touch events while drawing. This to
	 * not hog the system with events.
	 */
	synchronized void maUpdateScreen()
	{
		//SYSLOG("maUpdateScreen");

		Canvas lockedCanvas = null;

		if (mOpenGLView != -1) {
			maWidgetSetProperty(mOpenGLView, "invalidate", "");
			return;
		}

		// We won't update the display if the app is not active,
		// this is controlled by this flag.
		if (!mUpdateDisplay) { return; }

		// Mark that we are now updating the screen (we skip
		// touch events occurring during drawing in class MoSync).
		mIsUpdatingScreen = true;

		try
		{
			// Start editing the pixels in the surface.
			lockedCanvas = mMoSyncView.mSurfaceHolder.lockCanvas();
			synchronized (mMoSyncView.mSurfaceHolder)
			{
				if (mUsingFrameBuffer)
				{
					if (isNative) {
						// Ok, obviously we need to fix the -1 below. What
						// should be the proper size?
						maPanic(1, "Frame buffer not supported when building as library.");
					}
					ByteBuffer framebufferSlice = getMemorySlice(mFrameBufferAddress, -1);
					mFrameBufferBitmap.copyPixelsFromBuffer(framebufferSlice);

					// Clear the screen.. in this case draw the canvas black
					lockedCanvas.drawRGB(0,0,0);

					// Blit the framebuffer.
					lockedCanvas.drawBitmap(
						mFrameBufferBitmap, 0, 0, mBlitPaint);
				}
				else
				{
					if (null != lockedCanvas)
					{
						lockedCanvas.drawBitmap(mBitmap, 0, 0, mBlitPaint);
					}
				}
			}
		}
		catch (Exception e)
		{
			logError("updateScreen Exception : " + e.toString(), e);
			mIsUpdatingScreen = false;
			return;
		}

		if (lockedCanvas != null)
		{
			mMoSyncView.mSurfaceHolder.unlockCanvasAndPost(lockedCanvas);
		}

		mIsUpdatingScreen = false;
	}

	// TODO: WTH is this!!!
	// Why was not method body also commented out?
	// Doing that now. Previously only maResetBacklight
	// was commented out.
	/**
	 * maResetBacklight
	 */
//	{
//		SYSLOG("maResetBacklight");
//	}

	/**
	 * maGetScrSize
	 */
	public int maGetScrSize()
	{
		SYSLOG("maGetScrSize");
		return EXTENT(mWidth, mHeight);
	}

	/**
	 * maDrawImage
	 */
	void maDrawImage(int image, int left, int top)
	{
		SYSLOG("maDrawImage");

		ImageCache imageResource = mImageResources.get(image);
		if (imageResource == null)
		{
			return;
		}

		mCanvas.drawBitmap(imageResource.mBitmap, left, top, null);
	}

	/**
	 * _maDrawRGB
	 */
	synchronized void _maDrawRGB(
		int dstX,
		int dstY,
		int mem,
		int srcRectLeft,
		int srcRectTop,
		int srcRectWidth,
		int srcRectHeight,
		int scanLength)
	{
		SYSLOG("maDrawRGB");

		int pixels[] = new int[srcRectWidth];

		//mMemDataSection.position(mem);
		//IntBuffer ib = mMemDataSection.asIntBuffer();

		IntBuffer ib = getMemorySlice(mem, 4 * srcRectWidth * srcRectHeight).order(null).asIntBuffer();

		for (int y = 0; y < srcRectHeight; y++)
		{
			ib.position(y * scanLength);
			ib.get(pixels);
			mCanvas.drawBitmap(
				pixels,
				0,
				srcRectWidth,
				dstX,
				dstY + y,
				srcRectWidth,
				1,
				true,
				mPaint);
		}
	}

	/**
	 * _maDrawImageRegion
	 *
	 * Draw rectangular region of an image.
	 *
	 * @param image Handle to the image.
	 * @param srcRectLeft Left coord of source rectangle.
	 * @param srcRectTop Top coord of source rectangle.
	 * @param srcRectWidth Width of source rectangle.
	 * @param srcRectHeight Height of source rectangle.
	 * @param dstLeft Left coord of destination point.
	 * @param dstTop Top coord of destination point.
	 * @param transformMode A TRANS_* constant.
	 */
	synchronized void _maDrawImageRegion(
		final int image,
		final int srcRectLeft,
		final int srcRectTop,
		final int srcRectWidth,
		final int srcRectHeight,
		final int dstLeft,
		final int dstTop,
		final int transformMode)
	{
		SYSLOG("_maDrawImageRegion");

		// Get image resource.
		ImageCache imageResource = mImageResources.get(image);
		if (imageResource == null) { return; }

		// Get image dimension.
		final int imgHeight = imageResource.mBitmap.getHeight();
		final int imgWidth = imageResource.mBitmap.getWidth();

		// Make sure source rect is inside the image.
		// This is according to the syscall spec.
		// TODO: Should we call maPanic()?
		if (srcRectLeft < 0) { return; }
		if (srcRectTop < 0) { return; }
		if (srcRectLeft + srcRectWidth > imgWidth) { return; }
		if (srcRectTop + srcRectHeight > imgHeight) { return; }

		// Set up image source rectangle.
		mMaDrawImageRegionTempSourceRect.left = srcRectLeft;
		mMaDrawImageRegionTempSourceRect.right = (srcRectLeft + srcRectWidth);
		mMaDrawImageRegionTempSourceRect.top = srcRectTop;
		mMaDrawImageRegionTempSourceRect.bottom = (srcRectTop + srcRectHeight);

		// Set up destination rectangle.
		// We set origin to 0,0 and translate below.
		mMaDrawImageRegionTempDestRect.left = 0;
		mMaDrawImageRegionTempDestRect.right = srcRectWidth;
		mMaDrawImageRegionTempDestRect.top = 0;
		mMaDrawImageRegionTempDestRect.bottom = srcRectHeight;

		// Save the canvas matrix state.
		mCanvas.save(Canvas.MATRIX_SAVE_FLAG);

		// Set up canvas matrix for each transformation.
		// The transforms that mirror and rotate the image
		// might take some time to understand, because mirroring
		// flips the coordinate system, positive becomes negative,
		// rotation goes the other way, and so on.
		switch (transformMode)
		{
			case TRANS_NONE:
				mCanvas.translate(dstLeft, dstTop);
				break;

			case TRANS_ROT90:
				mCanvas.translate(dstLeft + srcRectHeight, dstTop);
				mCanvas.rotate(90);
				break;

			case TRANS_ROT180:
				mCanvas.translate(dstLeft + srcRectWidth, dstTop + srcRectHeight);
				mCanvas.rotate(180);
				break;

			case TRANS_ROT270:
				mCanvas.translate(dstLeft, dstTop + srcRectWidth);
				mCanvas.rotate(270);
				break;

			case TRANS_MIRROR:
				mCanvas.translate(dstLeft + srcRectWidth, dstTop);
				mCanvas.scale(-1, 1);
				break;

			case TRANS_MIRROR_ROT90:
				mCanvas.translate(dstLeft + srcRectHeight, dstTop + srcRectWidth);
				mCanvas.scale(-1, 1);
				mCanvas.rotate(270);
				break;

			case TRANS_MIRROR_ROT180:
				mCanvas.translate(dstLeft, dstTop + srcRectHeight);
				mCanvas.scale(-1, 1);
				mCanvas.rotate(180);
				break;

			case TRANS_MIRROR_ROT270:
				mCanvas.translate(dstLeft, dstTop);
				mCanvas.scale(-1, 1);
				mCanvas.rotate(90);
				break;

			default:
				maPanic(0, "maDrawImageRegion - unsupported transform");
				break;
		}

		mCanvas.drawBitmap(
			imageResource.mBitmap,
			mMaDrawImageRegionTempSourceRect,
			mMaDrawImageRegionTempDestRect,
			null);

		// Restore canvas matrix.
		mCanvas.restore();
	}

	/**
	 * maGetImageSize
	 */
	int maGetImageSize(int image)
	{
		SYSLOG("maGetImageSize");
		ImageCache imageResource = mImageResources.get(image);
		if (imageResource == null)
		{
			logError("maGetImageSize : no such resource");
			maPanic(0, "PANIC, Checking image size for non image resource");
		}

		return EXTENT(
			imageResource.mBitmap.getWidth(),
			imageResource.mBitmap.getHeight());
	}

	/**
	 * This syscall takes an image resource and reads the pixels from the given
	 * rectangle and copies the pixels to the memory location given by the dst
	 * parameter.
	 * @param image The image resource index.
	 * @param dst The memory location to which the result should be written.
	 * @param srcLeft The left-most pixel of the rectangle.
	 * @param srcTop The top-most pixel of the rectangle.
	 * @param srcWidth The width of the rectangle.
	 * @param srcHeight The height of the rectangle.
	 * @param scanLength The scan length, the number of bytes per row
	 * in the source image.
	 */
	void _maGetImageData(
		int image,
		int dst,
		int srcLeft,
		int srcTop,
		int srcWidth,
		int srcHeight,
		int scanLength)
	{
		SYSLOG("maGetImageData");

		ImageCache imageResource = mImageResources.get(image);
		if(null == imageResource)
		{
			maPanic(1,"maGetImageData failed, no image resource!");
		}

		// TODO: Remove variable bitmapSize, it is not used.
		//int bitmapSize = scanLength * srcHeight;

		//Log.i("@@@@@@", "_maGetImageData >>> handle: " + image + " hasAlpha: " + imageResource.mBitmap.hasAlpha());

		if ((srcTop + srcHeight) > imageResource.mBitmap.getHeight())
		{
			maPanic(
				-1,
				"maGetImageData, (srcTop + srcHeight) > " +
				"imageResource.mBitmap.getHeight() : " +
				(srcTop + srcHeight) +
				" > " +
				imageResource.mBitmap.getHeight() );
		}

		if ((srcLeft + srcWidth) > imageResource.mBitmap.getWidth())
		{
			maPanic(
				-1,
				"maGetImageData, (srcLeft + srcWidth) > "+
				"imageResource.mBitmap.getWidth() : " +
				(srcLeft + srcWidth) +
				" > " +
				imageResource.mBitmap.getWidth() );
		}

		// TODO: removed the "fast" version because of a bug, visual output
		// looks bad, run TestApp to test, fix this in 3.1. We use the "slow"
		// version for now. Try approach to see if .hasAlpha() can be used.

		// if (android.os.Build.VERSION.SDK_INT <= android.os.Build.VERSION_CODES.DONUT)

		// In 1.6 and below we use the version that includes the bug fix.
		_maGetImageDataAlphaBugFix(
			image,
			imageResource,
			dst,
			srcLeft,
			srcTop,
			srcWidth,
			srcHeight,
			scanLength);

		/*
		 * For what it is worth, this might be interesting to investigate,
		 * found this comment on a forum:
		 *
		 * API level 12 added a method setHasAlpha() which you
		 * can use instead of having to copy the image to get the alpha channel.
		 */
	}

	/**
	 * TODO: Not used, results look bad on tansparent pixels,
	 * test to enable in 3.1. Call from maGetImageData.
	 *
	 * Plain way of getting the image pixel data.
	 *
	 * @param image
	 * @param imageResource
	 * @param dst
	 * @param srcLeft
	 * @param srcTop
	 * @param srcWidth
	 * @param srcHeight
	 * @param scanLength
	 */
	private void _maGetImageDataFast(
		int image,
		ImageCache imageResource,
		int dst,
		int srcLeft,
		int srcTop,
		int srcWidth,
		int srcHeight,
		int scanLength)
	{
		try
		{
			int pixels[] = new int[srcWidth * srcHeight];

			IntBuffer intBuffer = getMemorySlice(dst, 4 * pixels.length).order(null).asIntBuffer();

			imageResource.mBitmap.getPixels(
				pixels,
				0,
				scanLength,
				srcLeft,
				srcTop,
				srcWidth,
				srcHeight);

			intBuffer.put(pixels);
		}
		catch (Exception e)
		{
			e.printStackTrace();
			maPanic(
				-1,
				"Exception in _maGetImageDataFast - " +
				"check stack trace in logcat: " +
				e.toString());
		}
	}

	/**
	 * Get image pixel data using a bug fix for the alpha channel.
	 *
	 * @param image
	 * @param imageResource
	 * @param dst
	 * @param srcLeft
	 * @param srcTop
	 * @param srcWidth
	 * @param srcHeight
	 * @param scanLength
	 */
	private void _maGetImageDataAlphaBugFix(
		int image,
		ImageCache imageResource,
		int dst,
		int srcLeft,
		int srcTop,
		int srcWidth,
		int srcHeight,
		int scanLength)
	{
		int pixels[] = new int[srcWidth];
		int colors[] = new int[srcWidth];
		int alpha[] = new int[srcWidth];

		/*
			The section generates a copy of the source image
			and renders that into a surface. The copy is then
			given full opaque since the color values wasn't
			returned by the Android.Bitmap.getPixels when the
			alpha value of a pixel was zero.
			The alpha values are then read from the source
			image and the result are added with the alpha values
			and the result is stored in the given memory location.
		*/

		Bitmap temporaryBitmap = createBitmap(srcWidth, srcHeight);

		Canvas temporaryCanvas = new Canvas(temporaryBitmap);

		temporaryCanvas.drawBitmap(imageResource.mBitmap, -srcLeft, -srcTop, new Paint());

		temporaryCanvas.drawColor(0xff000000, Mode.DST_ATOP);

		//mMemDataSection.position(dst);
		//IntBuffer intBuffer = mMemDataSection.asIntBuffer();

		IntBuffer intBuffer = getMemorySlice(dst, 4 * srcWidth * srcHeight).order(null).asIntBuffer();

		try
		{
			for (int y = 0; y < srcHeight; y++)
			{
				intBuffer.position(y*scanLength);

				imageResource.mBitmap.getPixels(
					alpha,
					0,
					srcWidth,
					srcLeft,
					srcTop+y,
					srcWidth,
					1);

				temporaryBitmap.getPixels(
					colors,
					0,
					srcWidth,
					0,
					y,
					srcWidth,
					1);

				for( int i = 0; i < srcWidth; i++)
				{
					pixels[i] = Color.argb(Color.alpha(alpha[i]),
							Color.red(colors[i]), Color.green(colors[i]),
							Color.blue(colors[i]));
					//pixels[i] = (alpha[i]&0xff000000) + (colors[i]&0x00ffffff);
				}

				intBuffer.put(pixels);
			}
		}
		catch(Exception e)
		{
			//Log.i("_maGetImageData", "("+image+", "+srcLeft+","+srcTop+", "+srcWidth+"x"+srcHeight+"): "+
			//	imageResource.mBitmap.getWidth()+"x"+imageResource.mBitmap.getHeight()+"\n");
			e.printStackTrace();
			maPanic(
				-1,
				"Exception in _maGetImageDataAlphaBugFix - " +
				"check stack trace in logcat: " +
				e.toString());
		}
	}

	/**
	 * Set the target image for drawing.
	 * @param image The target image, 0 means the screen.
	 */
	synchronized int maSetDrawTarget(int image)
	{
		SYSLOG("maSetDrawTarget");
		if (0 == image)
		{
			mCanvas = mDrawTargetScreen.mCanvas;
			mBitmap = mDrawTargetScreen.mBitmap;
		}
		else
		{
			ImageCache imageResource = mImageResources.get(image);
			if (null == imageResource)
			{
				return -1;
			}

			mCanvas = imageResource.mCanvas;
			mBitmap = imageResource.mBitmap;
		}
		return 1;
	}

	/**
	 * Create an image from a resource and store it in an image object.
	 * @param placeholder The handle for the image.
	 * @param dataHandle The handle of the resource where the image data is stored.
	 * @param offset Byte offset into the resource data.
	 * @param size Byte size of the image data.
	 * @return Status code.
	 */
	int maCreateImageFromData(int placeholder, int dataHandle, int offset, int size)
	{
		SYSLOG("maCreateImageFromData");

		// long freeSize = 0L;
		// long totalSize = 0L;
		// long usedSize = -1L;
		// try {
		// Runtime info = Runtime.getRuntime();
		// freeSize = info.freeMemory() / 1024L;
		// totalSize = info.totalMemory() / 1024L;
		// usedSize = totalSize - freeSize;
		// } catch (Exception e) {
		// e.printStackTrace();
		// }
		//
		// Log.i("memory 2", "################# total = " + totalSize
		// + "; free = " + freeSize + "; used = " + usedSize);

		// Byte array to hold resource data. This is the data we will
		// use to create the image.
		byte[] resourceData = null;
		try
		{
			resourceData = new byte[size];
		}
		catch(OutOfMemoryError ex)
		{
			ex.printStackTrace( );
			return RES_OUT_OF_MEMORY;
		}

		// If the handle is a binary resource it is in mBinaryResources.
		ByteBuffer binData = mBinaryResources.get(dataHandle);
		if (null != binData)
		{
			try
			{
				// Load binary data into resource memory.
				binData.position(offset);
				binData.get(resourceData);
			}
			catch(Exception ex)
			{
				logError("maCreateImageFromData - "
					+ "Couldn't read data, exception : " + ex, ex);
				return RES_BAD_INPUT;
			}
		}
		else // ubin
		{
			// If the handle is an unloaded binary resource (.ubin)
			// it is in mUBinaryResources.
			UBinData uBinData = mUBinaryResources.get(dataHandle);
			if (null != uBinData)
			{
				// Sanity check that offset + size is not larger than
				// the resource itself.
				if (uBinData.getSize() < offset + size)
				{
					logError("maCreateImageFromData " +
						"size error, uBinData.getSize(): "
							+ uBinData.getSize() +
						" size: " + size +
						" offset: " + offset);
					return RES_BAD_INPUT;
				}

				try
				{
					// Read data from the resource asset file.
					// All resources are bundled into this file.
					AssetManager assetManager = mContext.getAssets();
					InputStream resourceFileInputStream =
						assetManager.open(RESOURCE_FILE);

					// Here we position the stream pointer at the offset where
					// the image data starts.
					resourceFileInputStream.skip(
						uBinData.getOffset()
						- getResourceStartOffset()
						+ offset);

					// Read from asset file into the data array.
					resourceFileInputStream.read(resourceData);

					// Close input stream.
					resourceFileInputStream.close();
				}
				catch(Exception e)
				{
					logError("maCreateImageFromData - "
						+ "Couldn't read read stream, exception : "
						+ e.toString(), e);
					return RES_BAD_INPUT;
				}
			}
		}

		try
		{
			// Create a Bitmap object from the resource data.
			BitmapFactory.Options options = new BitmapFactory.Options();
			options.inPreferredConfig = Bitmap.Config.ARGB_8888;

			// /**
			// * The code below converts the Bitmap to the ARGB format.
			// * If you do not use this method or a similar one, Android will
			// ignore
			// * the specified format and use the format of the screen, usually
			// * RGB 565.
			// */
			//
			Bitmap decodedImage = decodeImageFromData(resourceData, options);

			if (decodedImage == null) {
				logError("maCreateImageFromData - "
						+ "could not decode image data (decodedImage == null)");
				return RES_BAD_INPUT;
			}
			//
			// int width = decodedImage.getWidth();
			// int height = decodedImage.getHeight();
			// int[] pixels = new int[width * height];
			// decodedImage.getPixels(pixels, 0, width, 0, 0, width, height);
			//
			// recycleImageData(decodedImage);
			//
			// Bitmap argbImage = createBitmapFromData(width, height, pixels);
			//
			// mImageResources.put(
			// placeholder, new ImageCache(null, argbImage));
			mImageResources
					.put(placeholder, new ImageCache(null, decodedImage));
		}
		catch (UnsupportedOperationException e)
		{
			logError("maCreateImageFromData - "
				+ "Unsupported operation, exception : " + e, e);
			return RES_BAD_INPUT;
		}
		catch (java.lang.IllegalArgumentException e)
		{
			logError("maCreateImageFromData - "
				+ "Illegal argument, exception : " + e, e);
			return RES_BAD_INPUT;
		}
		catch (java.lang.OutOfMemoryError e)
		{
			logError("maCreateImageFromData - "
				+ "Out of memory, exception : " + e, e);
			return RES_OUT_OF_MEMORY;
		}
		return RES_OK;
	}

	/**
	 * Takes the pre-processed raw image data and copies the
	 * contents to a bitmap.
	 */
	int _maCreateImageRaw(int placeholder, int width, int height, ByteBuffer buffer)
	{
		SYSLOG("maCreateImageRaw");

		Bitmap bitmap = createBitmap(width, height);

		if (null == bitmap)
		{
			maPanic(1, "Unable to create ");
		}

		buffer.position(0);
		bitmap.copyPixelsFromBuffer(buffer);

		mImageResources.put(placeholder, new ImageCache(null, bitmap));

		return RES_OK;
	}

	/**
	 * maCreateDrawableImage
	 */
	int maCreateDrawableImage(int placeholder, int width, int height)
	{
		SYSLOG("maCreateDrawableImage");
		try
		{
			Bitmap bitmap = createBitmap(width, height);

			Canvas canvas = new Canvas(bitmap);

			mImageResources.put(
				placeholder, new ImageCache(canvas, bitmap));
		}
		catch (UnsupportedOperationException e)
		{
			logError("maCreateDrawableImage - e: " + e.toString(), e);
			return -1;
		}
		catch (Exception e)
		{
			logError("maCreateDrawableImage - e: " + e.toString(), e);
			return -1;
		}
		catch (java.lang.OutOfMemoryError e)
		{
			logError("maCreateDrawableImage - "
				+ "Out of Memory e: " + e.toString(), e);
			return -1;
		}
		return 1;
	}

	/**
	 * maOpenStore
	 */
	synchronized int maOpenStore(String name, int flags)
	{
		SYSLOG("maOpenStore");

		try
		{
			File f = mContext.getFileStreamPath(storesPath);

			if (f.isFile())
			{
				return STERR_GENERIC;
			}
			else if (!f.isDirectory())
			{
				boolean success = f.mkdir();
				if (!success) { return STERR_GENERIC; }
			}

			String filePath = storesPath + File.pathSeparator + name;
			f = mContext.getFileStreamPath(filePath);

			if (f.isDirectory()) { return STERR_GENERIC; }

			if (!f.isFile())
			{
				if ((flags & MAS_CREATE_IF_NECESSARY) == 0)
				{
					return STERR_NONEXISTENT;
				}
				f.createNewFile();
			}

			mMaxStoreId++;
			mStores.put(mMaxStoreId, filePath);
			return mMaxStoreId;
		}
		catch(Exception e)
		{
			logError("open store exception : " + e.toString(), e);
			return STERR_GENERIC;
		}
	}

	/**
	 * maWriteStore
	 */
	synchronized int maWriteStore(int store, int data)
	{
		SYSLOG("maWriteStore");
		try
		{
			File f = mContext.getFileStreamPath(mStores.get(store));
			if (f.isFile())
			{
				ByteBuffer bb = mBinaryResources.get(data);
				if (null != bb)
				{
					FileOutputStream fos = new FileOutputStream(f, false);
					FileChannel fc = fos.getChannel();

					// Make sure that the buffer is reading from the start.
					bb.position(0);

					// Write data.
					fc.write(bb);
					fc.force(false);
					fc.close();
					fos.close();

					return 1;
				}

				UBinData ubd = mUBinaryResources.get(data);
				if (null != ubd)
				{
					byte[] ra = new byte[ubd.getSize()];

					AssetManager assetManager = mContext.getAssets();
					InputStream is = assetManager.open(RESOURCE_FILE);

					is.skip(ubd.getOffset() - getResourceStartOffset());
					is.read(ra);

					FileOutputStream fos = new FileOutputStream(f, false);
					fos.write(ra);
					fos.close();

					return 1;
				}

				logError("Write Store - didn't find buffer, data:" + data);
				return STERR_GENERIC;
			}
		}
		catch(Exception e)
		{
			logError("write store exception : " + e.toString(), e);
		}
		return STERR_GENERIC;
	}

	/**
	 * _maReadStore
	 * @return RES_OUT_OF_MEMORY on error.
	 */
	synchronized int _maReadStore(int store, int resourceIndex)
	{
		SYSLOG("_maReadStore");

		try
		{
			File file = mContext.getFileStreamPath(mStores.get(store));
			byte[] buffer;
			int length = 0;
			if (file.isFile())
			{
				length = (int) file.length();

				// Will throw OutOfMemoryError on fail.
				buffer = new byte[length];

				FileInputStream inputStream = new FileInputStream(file);
				int offset = 0;
				while (offset >= 0 && offset < length)
				{
					offset = inputStream.read(buffer, offset, (length-offset));
				}
				inputStream.close();

				// Added error checking here of result.
				// After browsing the code it looks like nativeCreateBinaryResource
				// returns RES_OUT_OF_MEMORY on fail.
				// TODO: Confirm this.
				int result = nativeCreateBinaryResource(resourceIndex, length);
				if (RES_OUT_OF_MEMORY == result)
				{
					return RES_OUT_OF_MEMORY;
				}

				ByteBuffer byteBuffer = mBinaryResources.get(resourceIndex);
				byteBuffer.put(buffer);

				return RES_OK;
			}

			return RES_OUT_OF_MEMORY;
		}
		catch (Throwable e)
		{
			logError("read store exception : " + e.toString(), e);
			return RES_OUT_OF_MEMORY;
		}
	}

	/**
	 * maCloseStore
	 */
	void maCloseStore(int store, int remove)
	{
		SYSLOG("maCloseStore");

		String filePath = mStores.get(store);
		if (remove != 0)
		{
			File f = mContext.getFileStreamPath(filePath);
			f.delete();
		}

		if (!(mStores.remove(store) == filePath))
		{
			maPanic(1, "maCloseStore failed!");
		}
	}

	/**
	 * maLoadResource
	 */
	int maLoadResource(int handle, int placeholder, int flag)
	{
		SYSLOG("maLoadResource");
		// Try to load the resource file, if we get an exception
		// it just means that this application has no resource file
		// and that is not an error.
		if (((mResourceFd != null) && (!mResourceFd.valid())) ||
				(((flag & MA_RESOURCE_OPEN) != 0) && (mResourceFd == null)))
		{
			mResourceFd = getResourceFileDesriptor();
		}

		// We have a program file so now we sends it to the native side
		// so it will be loaded into memory. The data section will also be
		// created and if there are any resources they will be loaded.
		if (null != mResourceFd) {
			if (false == nativeLoadResource(mResourceFd, mResourceOffset,
					handle,
					placeholder)) {
				logError("maLoadResource - "
						+ "ERROR Load resource was unsuccesfull");
				return 1;
			}
		}

		if ((flag & MA_RESOURCE_CLOSE) != 0) {
			mResourceFd = null;
			//mResourceOffset = 0;
		}

		return 0;
	}

	/**
	 * maLoadProgram
	 */
	void maLoadProgram(int data, int reload)
	{
		SYSLOG("maLoadProgram");
	}

	/**
	 * maGetKeys
	 */
	int maGetKeys()
	{
		SYSLOG("maGetKeys");
		return -1;
	}

	/**
	 * maGetEvent
	 */
	int maGetEvent(int event)
	{
		SYSLOG("maGetEvent");
		return -1;
	}

	/**
	 * Post a event to the MoSync event queue.
	 */
	public void postEvent(int[] event)
	{
		synchronized (mPostEventMonitor)
		{
			// Add event to queue.
			nativePostEvent(event);

			// Wake up the MoSync thread if it is sleeping.
			if (mIsSleeping)
			{
				mPostEventMonitor.notifyAll();
			}
		}
	}

	/**
	 * maWait
	 *
	 * Now using wait/notifyAll for maWait/postEvent. This synchronises blocks
	 * much more safely than sleep/interrupt. For further details, see e.g.
	 * http://stackoverflow.com/questions/2779484/why-wait-should-always-be-in-synchronized-block
	 */
	void maWait(int timeout)
	{
		SYSLOG("maWait");

		// If there are NO events in the queue, we wait.
		synchronized (mPostEventMonitor)
		{
			int size = nativeGetEventQueueSize();
			if (size > 0)
			{
				// There are events in the queue, just return.
				//Log.i("@@@ MoSync", "maWait: direct return, size: " + size);
				return;
			}

			mIsSleeping = true;

			long timeStamp;

			if (timeout > 0)
			{
				timeStamp = System.currentTimeMillis() + timeout;
			}
			else
			{
				timeStamp = Long.MAX_VALUE;
			}

			try
			{
				while ((nativeGetEventQueueSize() < 1)
					&& (System.currentTimeMillis() < timeStamp))
				{
					// Note that wait gives up lock on this synchronised block.
					mPostEventMonitor.wait(timeStamp - System.currentTimeMillis());
				}
			}
			catch (InterruptedException ie)
			{
				SYSLOG("maWait interrupted (this is normal behaviour)");
				ie.printStackTrace();
			}
			catch (Exception e)
			{
				logError("maWait exception : " + e.toString(), e);
				e.printStackTrace();
			}

			mIsSleeping = false;
		}

		SYSLOG("maWait returned");
	}

	/**
	 * maTime
	 */
	int maTime()
	{
		SYSLOG("maTime");
		Date date = new Date();
		return (int)(date.getTime() / 1000);
	}

	/**
	 * maLocalTime
	 */
	int maLocalTime()
	{
		SYSLOG("maLocalTime");
		Date date = new Date();
		long timeNow = date.getTime();
		TimeZone tz = TimeZone.getDefault();
		int timeOffset = tz.getOffset(timeNow);
		return (int)((timeNow + timeOffset) / 1000);
	}

	/**
	 * maGetMilliSecondCount
	 */
	int maGetMilliSecondCount()
	{
		return (int)SystemClock.elapsedRealtime();
	}

	/**
	 * maFreeObjectMemory
	 */
	int maFreeObjectMemory()
	{
		SYSLOG("maFreeObjectMemory");
		return -1;
	}

	/**
	 * maTotalObjectMemory
	 */
	int maTotalObjectMemory()
	{
		SYSLOG("maTotalObjectMemory");
		return -1;
	}

	/**
	 * maVibrate
	 */
	int maVibrate(int millisecondsToVibrate)
	{
		SYSLOG("maVibrate");

		Vibrator vibrator = (Vibrator) mContext.getSystemService(
			Context.VIBRATOR_SERVICE);
		if (null == vibrator)
		{
			return 0; // No vibrator.
		}

		if (0 == millisecondsToVibrate)
		{
			vibrator.cancel();
		}
		else
		{
			vibrator.vibrate(millisecondsToVibrate);
		}

		return 1;
	}

	/**
	 * maPanic
	 */
	public void maPanic(int result, String message)
	{
		SYSLOG("maPanic");

		threadPanic(result, message);
	}


	int maExtensionFunctionInvoke(int function, int ptrs, int memstart) {
		return maInvokeExtension(function, ptrs, memstart);
	}

	int maExtensionModuleLoad(String name, int hash) {
		return mMoSyncExtensionLoader.maExtensionModuleLoad(name, hash);
	}

	int maExtensionFunctionLoad(int module, int ix) {
		return mMoSyncExtensionLoader.maExtensionFunctionLoad(module, ix);
	}



	/**
	 * maInvokeExtension
	 */
	int maInvokeExtension(int function, int ptr, int memstart)
	{
		SYSLOG("maInvokeExtension");
		return mMoSyncExtensionLoader.maInvokeExtension(function, ptr, memstart);
	}

	/**
	 * maIOCtl
	 */
	int maIOCtl(int function, int a, int b, int c)
	{
		SYSLOG("maIOCtl");
		return -1;
	}

	/**
	 * _enableFramebuffer
	 */
	void _enableFramebuffer(int address)
	{
		mUsingFrameBuffer = true;
		// address is pointing at a byte but that array is an int array.
		mFrameBufferAddress = address;
		mFrameBufferSize = mWidth*mHeight;

		mFrameBufferBitmap = createBitmap(mWidth, mHeight);

	}

	/**
	 * _disableFramebuffer
	 */
	void _disableFramebuffer()
	{
		mUsingFrameBuffer = false;
	}

	/**
	 * Implementation of the maWriteLog syscall which only
	 * sends the log message to the Android Logcat.
	 * @param str The string to send to logcat.
	 * @param size The number of characters in the string.
	 */
	int maWriteLog(String str, int size)
	{
		Log.i("maWriteLog", str);
		return 1;
	}

	/**
	 * Send an SMS (text message).
	 * @param phoneNo The destination phone number.
	 * @param message The text message.
	 * @return 0 on success, <0 on error.
	 */
	int maSendTextSMS(String phoneNo, String message)
	{
		if (null != mMoSyncSMS)
		{
			return mMoSyncSMS.maSendTextSMS(phoneNo, message);
		}
		else
		{
			// Not available.
			return -1;
		}
	}

	/**
	 * Get a system property as a string.
	 * @param key The name of the property.
	 * @param buf Index to the position or the buffer
	 * in memory that will receive the property value.
	 * @param key The name o the property.
	 * @return Length of the property value, including
	 * the null termination character. <0 on error.
	 * If return value size is > size, the buffer is too
	 * small and no data is copied.
	 */
	int maGetSystemProperty(String key, int buf, int size)
	{
		String property = null;

		if (key.equals("mosync.imei"))
		{
			TelephonyManager manager = (TelephonyManager)
				mContext.getSystemService(Context.TELEPHONY_SERVICE);
			property = manager.getDeviceId();
		}
		else if (key.equals("mosync.imsi"))
		{
			TelephonyManager manager = (TelephonyManager)
				mContext.getSystemService(Context.TELEPHONY_SERVICE);
			property = manager.getSubscriberId();
		}
		else if (key.equals("mosync.iso-639-1"))
		{
			Locale locale = Locale.getDefault();
			property = locale.getLanguage();
		}
		else if (key.equals("mosync.iso-639-2"))
		{
			Locale locale = Locale.getDefault();
			property = locale.getISO3Language();
		}
		else if(key.equals("mosync.device"))
		{
			property = Build.FINGERPRINT;
		}
		else if(key.equals("mosync.device.name"))
		{
			property = Build.DEVICE;
		}
		else if(key.equals("mosync.device.model"))
		{
			property = Build.MODEL;
		}
		else if(key.equals("mosync.device.UUID"))
		{
			property = Secure.getString( mContext.getContentResolver(),
					Secure.ANDROID_ID);
		}
		else if(key.equals("mosync.device.OS"))
		{
			property = "Android";
		}
		else if(key.equals("mosync.device.OS.version"))
		{
			property = Build.VERSION.RELEASE;
		}
		else if (key.equals("mosync.path.local"))
		{
			String path = getActivity().getFilesDir().getAbsolutePath() + "/";
			//Log.i("@@@ MoSync", "Property mosync.path.local: " + path);
			property = path;
		}
		else if (key.equals("mosync.path.local.urlPrefix"))
		{
			property = "file://";
		}
		else if (key.equals("mosync.path.local.url"))
		{
			String url = "file://" +
				getActivity().getFilesDir().getAbsolutePath() + "/";
			//Log.i("@@@ MoSync", "Property mosync.path.local.url: " + url);
			property = url;
		}
		else if (key.equals("mosync.network.type"))
		{
			property = getSystemPropertyNetworkType();
		}

		// Check that we have a valid property string.
		if (null == property) { return -2; }
		if (property.length() <= 0) { return -2; }

		// If the buffer is not big enough to hold the
		// property data, then return the length of
		// the property. This tells the caller that
		// the buffer was too small.
		if (property.length() + 1 > size)
		{
			return property.length() + 1;
		}

		// Write this property to memory.
		byte[] ba = property.getBytes();

		//mMemDataSection.position(buf);
		//mMemDataSection.put(ba);

		// Add null termination character.
		//mMemDataSection.put((byte)0);

		ByteBuffer slicedBuffer = getMemorySlice(buf, ba.length + 1);
		slicedBuffer.put(ba);
		slicedBuffer.put((byte)0);

		return property.length() + 1;
	}

	/**
	 * Get the network connection type.
	 *
	 * @return a String indicating the type of the connection.
	 * For Mobile networks it returns the exact type of mobile network,
	 * e.g. GSM, GPRS, or HSDPA...
	 * The result might contain the full name and version of the
	 * mobile network type.
	 * If there is no connectivity or network permissions are not set,
	 * "none" is returned.
	 */
	private String getSystemPropertyNetworkType()
	{
		if (PackageManager.PERMISSION_GRANTED !=
			getActivity().checkCallingOrSelfPermission(
				android.Manifest.permission.ACCESS_NETWORK_STATE))
		{
			return "none";
		}

		//get the connection that we are using right now
		NetworkInfo info = mConnectivityManager.getActiveNetworkInfo();
		if (info != null)
		{
			String type = info.getTypeName();
			if (type == null)
			{
				return "unknown";
			}
			else if (type.toLowerCase().equals("mobile"))
			{
				//return a generic default
				return "mobile";
			}
			else
			{
				return "wifi";
			}
		}

		return "none";
	}

	/**
	 * Perform a platform request.
	 * @param url The url that specifies the request.
	 * @return
	 */
	int maPlatformRequest(String url)
	{
		if (url.startsWith("http://") || url.startsWith("https://"))
		{
			//Log.i("maPlatformRequest","Starting browser:" + url);
			Intent intent = new Intent(Intent.ACTION_VIEW);
			intent.setData(Uri.parse(url));
			((Activity)mContext).startActivity(intent);

			return 0;
		}

		else if(url.startsWith("tel://"))
		{
			Log.i("maPlatformRequest","Starting a call - " + url);

			// check to see if the proper permission is granted
			if(!(mContext.getPackageManager().checkPermission("android.permission.CALL_PHONE",
								mContext.getPackageName()) == PackageManager.PERMISSION_GRANTED))
			{
				Log.i("@MoSync", "Permission to make phone call not set!");
				return -2;
			}

			Intent intent = new Intent(Intent.ACTION_CALL, Uri.parse(url));
			((Activity)mContext).startActivity(intent);

			return 0;
		}
		else if(url.startsWith("fb://") || url.startsWith("mailto:") || url.startsWith("tweetie://"))
		{
			Intent sharingIntent = new Intent(android.content.Intent.ACTION_SEND);
			sharingIntent.setType("text/plain");
			String sharedText = splitShareIntentText(url);
			sharingIntent.putExtra(android.content.Intent.EXTRA_TEXT, sharedText);

			((Activity)mContext).startActivity(Intent.createChooser(sharingIntent, "Share text via"));

			return 0;
		}

		return -1;
	}

	private String splitShareIntentText(String text)
	{
		String parsed[] = text.split(":");
		int size = parsed[1].length();
		if(parsed[1].startsWith("//"))
		{
			return parsed[1].substring(2, size);
		}
		return parsed[1];
	}

	/**
	 * maShowVirtualKeyboard
	 */
	int maShowVirtualKeyboard()
	{
		InputMethodManager mgr = (InputMethodManager)
			mContext.getSystemService(Context.INPUT_METHOD_SERVICE);
		mgr.showSoftInput(mMoSyncView, InputMethodManager.SHOW_FORCED);
		return 0;
	}

	/**
	 * Stores an image resource. Since Android differs a lot from
	 * the other runtimes this is necessary. There isn't a duplicate
	 * stored on the JNI side.
	 */
	boolean loadImage(int resourceIndex, int pos, int length, int binaryResource)
	{
		SYSLOG("LoadImage " +
			"resourceIndex: " + resourceIndex +
			" pos: " + pos +
			" length: " + length +
			" binaryResource: " + binaryResource );
		try
		{
			byte[] ra = new byte[length];

			if (binaryResource != 0)
			{
				SYSLOG("Reading image resource from binary resource");
				ByteBuffer bb = mBinaryResources.get(binaryResource);
				if (null == bb)
				{
					return false;
				}
				bb.position(pos);
				bb.get(ra);
			}
			else // is ubin
			{
				SYSLOG("Reading image resource from asset");
				int offset =  pos - (int)mResourceOffset;

				AssetManager assetManager = mContext.getAssets();
				InputStream is = assetManager.open(RESOURCE_FILE);

				is.skip(offset);
				is.read(ra);
			}

			SYSLOG("Decode a bitmap!");

			Bitmap bitmap = decodeImageFromData(ra, null);

			// TODO: Remove commencted out line.
			//Bitmap bitmap = BitmapFactory.decodeByteArray(ra, 0, length);

			if (bitmap != null)
			{
				SYSLOG("Bitmap was created!");
				mImageResources.put(
					resourceIndex, new ImageCache(null, bitmap));

				// Return success.
				return true;
			}

			// If we end up here an error occurred.
			logError("loadImage - Bitmap wasn't created from Resource: "
				+ resourceIndex);
			return false;
		}
		catch (Throwable e)
		{
			logError("loadImage exception : " + e.toString(), e);
			return false;
		}
	}

	/**
	 * TODO: Consider renaming this method to for example setBinaryResource.
	 * No loading is going on here. And rewrite the comment.
	 *
	 * Load and store a binary resource. Since Android differs a lot from
	 * the other runtimes this is necessary. There isn't a duplicate stored
	 * on the JNI side.
	 *
	 * @param resourceIndex The handle/placeholder id.
	 * @param buffer Data referenced by the handle with id resourceIndex.
	 * @return true on success, false on error.
	 */
	public boolean loadBinary(int resourceIndex, ByteBuffer buffer)
	{
		SYSLOG("loadBinary index:" + resourceIndex);

		try
		{
			buffer.order(null);
			mBinaryResources.put(resourceIndex, buffer);
			return true;
		}
		catch(Throwable e)
		{
			logError("loadBinary - Out of Memory!", e);
			return false;
		}
	}

	/**
	 * Store information about an unloaded binary resource.
	 *
	 * If the resource is an audio ubin, we also write it to a file,
	 * because on Android we play audio from files.
	 *
	 * @param resourceIndex The id of the ubin resource.
	 * @param offset The offset of the reosurce in the resource file.
	 * @param size The size of the resource.
	 */
	void loadUBinary(int resourceIndex, int offset, int size)
	{
		SYSLOG("loadUBinary index:" + resourceIndex);

		// Create a ubin data object that holds information about the object.
		UBinData ubinData = new UBinData(offset, size);
		mUBinaryResources.put(resourceIndex, ubinData);

		// If this is an audio ubin, it is written to a file.
		mMoSyncSound.storeIfAudioUBin(ubinData, resourceIndex);
	}

	public ByteBuffer destroyBinary(int resourceIndex)
	{
		ByteBuffer buffer =  mBinaryResources.get(resourceIndex);

		if(null != buffer)
		{
			mBinaryResources.remove(resourceIndex);
			return buffer;
		}

		return null;
	}

	/**
	 * Destroy a resource.
	 *
	 * @param resourceIndex the resource index which should be destroyed.
	 */
	void destroyResource(int resourceIndex)
	{
		SYSLOG("destroyResource :" + resourceIndex);

		if(null != mUBinaryResources.get(resourceIndex))
		{
			mUBinaryResources.remove(resourceIndex);
		}
		else
		{
			ImageCache img = mImageResources.get(resourceIndex);
			if(null != img)
			{
				// recycle only if possible
				if(img.mBitmap != null && !img.mBitmap.isRecycled())
				{
				    Log.e("MoSyncThread", "recycle the bitmap" + img.mBitmap);
				    img.mBitmap.recycle();
				}

				img.mBitmap = null;
				mImageResources.remove(resourceIndex);
			}
		}

		System.gc();
	}

	/**
	 * Open a TextBox.
	 * @param title
	 * @param inText
	 * @param outText
	 * @param maxSize
	 * @param constraints
	 * @return
	 */
	int maTextBox(
		String title,
		String inText,
		int outText,
		int maxSize,
		int constraints)
	{
		Intent intent = new Intent(mContext, TextBox.class);
		Bundle bundle = new Bundle();
		bundle.putString("TITLE", title);
		bundle.putString("TEXT", inText);
		bundle.putInt("OUTPUT", outText);
		bundle.putInt("MAXSIZE", maxSize);
		bundle.putInt("CONSTRAINTS", constraints);
		intent.putExtras(bundle);
		mContext.startActivity(intent);
		//Log.i("MoSync", "New activity started for InputBox");
		return 0;
	}

	/**
	 * @deprecated Use maAlert instead.
	 * Opens a message box.
	 * @param title
	 * @param text
	 * @return
	 */
	int maMessageBox(
		final String title,
		final String text)
	{
		// Get parameters from the parent activity
		mContext.runOnUiThread( new Runnable( ) {

			@Override
			public void run()
			{
				AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
				builder.setCancelable(false);
				builder.setMessage(text);
				builder.setPositiveButton("Ok", new DialogInterface.OnClickListener()
				{
					@Override
					public void onClick(DialogInterface dialog, int which)
					{
					}
				});
				AlertDialog alertDialog = builder.create();

				alertDialog.setTitle(title);
				alertDialog.show();
			}
		});

		return 0;
	}

	/**
	 * Opens a message box.
	 * The dialog can be dismissed using one of the 3 optional buttons.
	 * @param title
	 * @param message
	 * @param buttonPositive
	 * @param buttonNeutral
	 * @param buttonNegative
	 * @return
	 */
	public int maAlert(
		final String title,
		final String message,
		final String buttonPositive,
		final String buttonNeutral,
		final String buttonNegative)
	{
		// Get parameters from the parent activity
		mContext.runOnUiThread( new Runnable( ) {

			@Override
			public void run()
			{
				AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
				if ( title.length() > 0 )
				{
					builder.setTitle(title);
				}
				if ( message.length() > 0 )
				{
					builder.setMessage(message);
				}
				if ( buttonPositive.length() > 0 )
				{
					builder.setPositiveButton(buttonPositive, new DialogInterface.OnClickListener() {

						@Override
						public void onClick(DialogInterface dialog, int which) {
							EventQueue.getDefault().postAlertEvent(1);
						}
					});
				}
				if ( buttonNeutral.length() > 0 )
				{
					builder.setNeutralButton(buttonNeutral, new DialogInterface.OnClickListener() {

						@Override
						public void onClick(DialogInterface dialog, int which) {
							EventQueue.getDefault().postAlertEvent(2);
						}
					});
				}
				if ( buttonNegative.length() > 0 )
				{
					builder.setNegativeButton(buttonNegative, new DialogInterface.OnClickListener() {

						@Override
						public void onClick(DialogInterface dialog, int which) {
							EventQueue.getDefault().postAlertEvent(3);
						}
					});
				}

				final AlertDialog alertDialog = builder.create();
				alertDialog.setCancelable(true);
				alertDialog.setOnKeyListener(new OnKeyListener() {

					@Override
					public boolean onKey(DialogInterface dialog, int keyCode, KeyEvent event) {
						if(keyCode == KeyEvent.KEYCODE_BACK)
						{
							//todo see if the back btn is send again.
							EventQueue.getDefault().postAlertDismissed();
							alertDialog.dismiss();
							return true;
						}
						return false;
					}
				});

				alertDialog.show();
			}
		});

		return 0;
	}

	/**
	 * Display a toast message.
	 * A toast is a view containing a quick little message for the user.
	 * @param message The toast message.
	 * @param duration One of the constants:
	 *  - #MA_TOAST_DURATION_SHORT or
	 *  - #MA_TOAST_DURATION_LONG
	 * @return
	 */
	int maToast(final String message, final int duration)
	{
		if ( duration != MA_TOAST_DURATION_LONG &&
				duration != MA_TOAST_DURATION_SHORT )
		{
			return -1;
		}

		mContext.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				Toast.makeText(
						mContext,
						message,
						(duration == MA_TOAST_DURATION_SHORT ? Toast.LENGTH_SHORT : Toast.LENGTH_LONG))
						.show();
			}
		});

		return 0;
	}

	/**
	 * Internal wrapper for maImagePickerOpen that runs
	 * the call in the UI thread.
	 */
	int maImagePickerOpen()
	{
		return mMoSyncNativeUI.maImagePickerOpen(MA_IMAGE_PICKER_EVENT_RETURN_TYPE_IMAGE_HANDLE);
	}

	/**
	 * Displays an image picker to the user and sets the event return type.
	 * @param eventReturnType One of the next constants:
	 * - #MA_IMAGE_PICKER_EVENT_RETURN_TYPE_IMAGE_HANDLE
	 * - #MA_IMAGE_PICKER_EVENT_RETURN_TYPE_IMAGE_DATA
	 */
	int maImagePickerOpenWithEventReturnType(int eventReturnType)
	{
		return mMoSyncNativeUI.maImagePickerOpen(eventReturnType);
	}

	/**
	 * Internal wrapper for showing the options dialog.
	 * It takes the title, the text for the destructive button, the Cancel button text, and the options for the dialog.
	 * The destructive button is iOS specific, so here it is not treated separately.
	 * @return
	 */
	int maOptionsBox(String title, String destructiveButtonTitle, String cancelButtonTitle, int buffPointer, int buffSize)
	{
		return mMoSyncNativeUI.maOptionsBox(title, destructiveButtonTitle, cancelButtonTitle, buffPointer, buffSize);
	}

	int maAdsBannerCreate(final int bannerSize, final String publisherID)
	{
		return mMoSyncAds.maAdsBannerCreate(bannerSize, publisherID);
	}

	int maAdsAddBannerToLayout(int bannerHandle, int layoutHandle)
	{
		return mMoSyncAds.maAdsAddBannerToLayout(bannerHandle, layoutHandle, mMoSyncNativeUI.getWidget(layoutHandle));
	}

	int maAdsRemoveBannerFromLayout(int bannerHandle, int layoutHandle)
	{
		return mMoSyncAds.maAdsRemoveBannerFromLayout(bannerHandle, layoutHandle, mMoSyncNativeUI.getWidget(layoutHandle));
	}

	int maAdsBannerDestroy(int bannerHandle)
	{
		return mMoSyncAds.maAdsBannerDestroy(bannerHandle);
	}

	int maAdsBannerSetProperty(
		final int adHandle,
		final String key,
		final String value)
	{
		return mMoSyncAds.maAdsBannerSetProperty(adHandle, key, value);
	}

	int maAdsBannerGetProperty(
		final int adHandle,
		final String key,
		final int memBuffer,
		final int memBufferSize)
	{
		return mMoSyncAds.maAdsBannerGetProperty(adHandle, key, memBuffer, memBufferSize);
	}

	/**
	 * Display a notification.
	 * @deprecated use maNotificationCreate() instead.
	 * @param type
	 * @param id
	 * @param title
	 * @param text
	 * @return
	 */
	int maNotificationAdd(int type, int id, String title, String text)
	{
		//Log.i("MoSync", "maNotificationAdd");

		// This is the only notification type we support at the moment.
		if (NOTIFICATION_TYPE_APPLICATION_LAUNCHER != type)
		{
			// TODO: Implement case for regular notifications.

			return -1;
		}

		MoSyncService.startService(
			mContext.getApplicationContext(),
			id,
			title,
			text);

		return 0;
	}

	/**
	 * Depending of whether this is a NOTIFICATION_TYPE_APPLICATION_LAUNCHER
	 * or a regular notification we either stop the service or remove the
	 * notification.
	 * @deprecated use maNotificationDestroy() instead.
	 * @param notificationId
	 * @return
	 */
	int maNotificationRemove(int notificationId)
	{
		//Log.i("MoSync", "maNotificationRemove");

		// Stop the service if this is the service notification id.
		if (MoSyncService.sNotificationId == notificationId)
		{
			// Remove the service notification.
			MoSyncService.removeServiceNotification(
				notificationId, mContext);

			// Stop the service.
			MoSyncService.stopService();

			return 0;
		}

		// TODO: Implement case for regular notifications.

		return -1;
	}

	/**
	 * Create a local notification.
	 * @return a handle to a new local notification object, or
	 * MA_NOTIFICATION_RES_UNSUPPORTED if the notifications are not supported on current system..
	 */
	int maNotificationLocalCreate()
	{
		return mMoSyncNotifications.maNotificationLocalCreate(mContext);
	}

	/**
	 * Destroys a local notification object, and clears it from the notifications list.
	 * @param handle Handle to a local notification object.
	 * @return MA_NOTIFICATION_RES_OK, or MA_NOTIFICATION_RES_INVALID_HANDLE.
	 */
	int maNotificationLocalDestroy(int handle)
	{
		return mMoSyncNotifications.maNotificationLocalDestroy(handle);
	}

	/**
	 * Set a specific property on a notification.
	 * @param handle Handle to a local notification object.
	 * @param propertyName
	 * @param propertyValue
	 * @return
	 */
	int maNotificationLocalSetProperty(int handle, String propertyName, String propertyValue)
	{
		return mMoSyncNotifications.maNotificationLocalSetProperty(handle, propertyName, propertyValue);
	}

	/**
	 * Get a specific property of a notification.
	 * @param handle Handle to a local notification object.
	 * @param propertyName
	 * @param propertyValue
	 * @return
	 */
	int maNotificationLocalGetProperty(int handle, String propertyName, int memBuffer, int memBufferSize)
	{
		return mMoSyncNotifications.maNotificationLocalGetProperty(handle, propertyName, memBuffer, memBufferSize);
	}

	/**
	 * Schedules a local notification for delivery at its encapsulated date and time.
	 * @param handle Handle to a local notification object.
	 * @return MA_NOTIFICATION_RES_OK if no error occurred,
	 * MA_NOTIFICATION_RES_INVALID_HANDLE if the notificationHandle is invalid,
	 * MA_NOTIFICATION_RES_ALREADY_SCHEDULED if it was already scheduled.
	 */
	int maNotificationLocalSchedule(int handle)
	{
		return mMoSyncNotifications.maNotificationLocalSchedule(handle, mContext.getApplicationContext());
	}

	/**
	 * Cancels the delivery of the specified scheduled local notification.
	 * @param handle Handle to a local notification object.
	 * @return MA_NOTIFICATION_RES_OK if no error occurred,
	 * MA_NOTIFICATION_RES_INVALID_HANDLE if the notificationHandle is invalid.
	 * MA_NOTIFICATION_RES_CANNOT_UNSCHEDULE if it wasn't scheduled before.
	 */
	int maNotificationLocalUnschedule(int handle)
	{
		return mMoSyncNotifications.maNotificationLocalUnschedule(handle);
	}

	/**
	 * Registers the current application for receiving push notifications for GCM/C2DM server.
	 * @param pushNotificationTypes ignored on Android.
	 * @param accountID Is the ID of the account authorized to send messages to the application,
	 * typically the email address of an account set up by the application's developer.
	 * @return MA_NOTIFICATION_RES_OK if no error occurred.
     * MA_NOTIFICATION_RES_ALREADY_REGISTERED if the application is already registered for receiving push notifications.
     * MA_NOTIFICATION_RES_UNSUPPORTED
	 */
	int maNotificationPushRegister(int pushNotificationTypes, String accountID)
	{
		// Ignore the first param on Android.
		return mMoSyncNotifications.maNotificationPushRegister(accountID);
	}

	/**
	 * Unregister application for push notifications.
	 * @return One of the next constants:
	 * - MA_NOTIFICATION_RES_OK if no error occurred.
	 * - MA_NOTIFICATION_NOT_REGISTERED if the application was not registered for receiving
	 * push notification.
	 */
	int maNotificationPushUnregister()
	{
		return mMoSyncNotifications.maNotificationPushUnregister();
	}

	/**
	 * Gets the latest registration response.
	 * \param registrationMesssage The registrationID if the registration was successfull,
	 * or the error messsage otherwise.
	 * \return  One of the next constants:
	 * - MA_NOTIFICATION_RES_OK if the application registered successfully.
	 * - MA_NOTIFICATION_RES_REGISTRATION_SERVICE_NOT_AVAILABLE
	 * - MA_NOTIFICATION_RES_REGISTRATION_ACCOUNT_MISSING
	 * - MA_NOTIFICATION_RES_REGISTRATION_AUTHENTICATION_FAILED
	 * - MA_NOTIFICATION_RES_REGISTRATION_TOO_MANY_REGISTRATIONS
	 * - MA_NOTIFICATION_RES_REGISTRATION_INVALID_SENDER
	 * - MA_NOTIFICATION_RES_REGISTRATION_PHONE_REGISTRATION_ERROR
	 */
	int maNotificationPushGetRegistration(int buf, int bufSize)
	{
		return mMoSyncNotifications.maNotificationPushGetRegistration(buf, bufSize);
	}

	/**
	 * Get info about for a given push notification.
	 * @param handle The push notification handle.
	 * @param type By default is 1.
	 * @param allertMessage Address to buffer to receive the data.
	 * The result is NOT zero terminated.
	 * @param allertMessageSize Max size of the buffer.
	 * @return  One of the next constants:
	 *  - MA_NOTIFICATION_RES_OK
	 *  - MA_NOTIFICATION_RES_INVALID_HANDLE
	 *  - MA_NOTIFICATION_RES_INVALID_STRING_BUFFER_SIZE
	 */
	int maNotificationPushGetData(int handle, int allertMessage,
			int allertMessageSize)
	{
		return mMoSyncNotifications.maNotificationPushGetData(handle, allertMessage, allertMessageSize);
	}

	/**
	 * Destroy a push notification object.
	 * @param handle Handle to a push notification object.
	 * @return One of the next constants:
	 * - #MA_NOTIFICATION_RES_OK if no error occurred.
	 * - #MA_NOTIFICATION_RES_INVALID_HANDLE if the notificationHandle is invalid.
	 */
	int maNotificationPushDestroy(int handle)
	{
		return mMoSyncNotifications.maNotificationPushDestroy(handle);
	}

	/**
	 * Set the ticker text in the notification status bar for incoming push notifications.
	 * @param tickerText The text that flows by in the status bar when the notification first activates.
	 * @return MA_NOTIFICATION_RES_OK, MA_NOTIFICATION_RES_ERROR.
	 */
	int maNotificationPushSetTickerText(String tickerText)
	{
		return mMoSyncNotifications.maNotificationPushSetTickerText(tickerText);
	}

	/**
	 * Set the  message title in the notification area for incoming push notifications.
	 * @param title The title that goes in the expanded entry of the notification.
	 * @return MA_NOTIFICATION_RES_OK, MA_NOTIFICATION_RES_ERROR.
	 */
	int maNotificationPushSetMessageTitle(String title)
	{
		return mMoSyncNotifications.maNotificationPushSetMessageTitle(title);
	}

	/**
	 * Set the display flags applied to the incoming push notifications.
	 * @param flag One of the constants:
	 *  - MA_NOTIFICATION_DISPLAY_FLAG_DEFAULT
	 *  - MA_NOTIFICATION_DISPLAY_FLAG_ANYTIME
	 * @return MA_NOTIFICATION_RES_OK, MA_NOTIFICATION_RES_ERROR.
	 */
	int maNotificationPushSetDisplayFlag(int flag)
	{
		return mMoSyncNotifications.maNotificationPushSetDisplayFlag(flag);
	}

	/**
	 * Sets the properties to the Native Image Picker.
	 * @param property property A string representing which property to set.
	 * One of the #MA_CAPTURE_ MA_CAPTURE constants.
	 * @param value The value that will be assigned to the property.
	 * @return One of the next constants:
	 *  - #MA_CAPTURE_RES_OK if no error occurred.
	 *  - #MA_CAPTURE_RES_INVALID_PROPERTY if the property name is not valid.
	 *  - #MA_CAPTURE_RES_INVALID_PROPERTY_VALUE if the property value is not valid.
	 */
	int maCaptureSetProperty(String property, String value)
	{
		return mMoSyncCapture.maCaptureSetProperty(property, value);
	}

	/**
	 * Retrieves the properties from the Native Image Picker.
	 * @param property A string representing which property to get.
	 * @param value A buffer that will hold the value of the property, represented as a string.
	 * @valueSize the value buffer size.
	 * @return One of the next constants:
	 * - #MA_CAPTURE_RES_OK if no error occurred.
	 * - #MA_CAPTURE_RES_INVALID_PROPERTY if the property name is not valid.
	 * - #MA_CAPTURE_RES_INVALID_STRING_BUFFER_SIZE if the buffer size was to small.
	 */
	int maCaptureGetProperty(String property, int valueBuffer, int valueSize)
	{
		return mMoSyncCapture.maCaptureGetProperty(property, valueBuffer, valueSize);
	}

	/**
	* Perform an action on the image picker.
	* @param action One of the #MA_CAPTURE_ACTION_ MA_CAPTURE_ACTION constants.
	* @return One of the next constants:
	*  - #MA_CAPTURE_RES_OK if no error occurred.
	*  - #MA_CAPTURE_RES_INVALID_ACTION if the given action is invalid.
	*  - #MA_CAPTURE_RES_CAMERA_NOT_AVAILABLE if camera is not available at the moment.
	*  - #MA_CAPTURE_RES_VIDEO_NOT_SUPPORTED if video recording is not supported.
	*  - #MA_CAPTURE_RES_PICTURE_NOT_SUPPORTED if camera picture mode is not supported.
	*/
	int maCaptureAction(int action)
	{
		return mMoSyncCapture.maCaptureAction(action);
	}

	/**
	* Save a image data object to a file.
	* @param handle Handle to a image data object.
	* @param fullPath A buffer containing the a full path where the file will be created.
	* @param fullPathBufSize The size of the fullPath buffer.
	* @return One of the next constants:
	*  - #MA_CAPTURE_RES_OK if no error occurred.
	*  - #MA_CAPTURE_RES_INVALID_HANDLE if the given handle was invalid.
	*  - #MA_CAPTURE_RES_FILE_INVALID_NAME if the fullPath param is invalid.
	*  - #MA_CAPTURE_RES_FILE_ALREADY_EXISTS if the file already exists.
	*/
	int maCaptureWriteImage(int handle, String fullPathBuffer, int fullPathBufSize)
	{
		return mMoSyncCapture.maCaptureWriteImage(handle, fullPathBuffer, fullPathBufSize);
	}

	/**
	* Get full path to a taken picture.
	* @param handle Handle to an image data object.
	* @param buffer Will contain the full path to the image file.
	* @param bufferSize Maximum size of the buffer.
	* @return One of the next constants:
	*  - MA_CAPTURE_RES_OK if no error occurred.
	*  - MA_CAPTURE_RES_INVALID_HANDLE if the given handle was invalid.
	*  - MA_CAPTURE_RES_INVALID_STRING_BUFFER_SIZE if the buffer size was to small.
	*/
	int maCaptureGetImagePath(int handle, int buffer, int bufferSize)
	{
		return mMoSyncCapture.maCaptureGetImagePath(handle, buffer, bufferSize);
	}

	/**
	* Get full path to a recorded video.
	* @param handle Handle to a video data object.
	* @param buffer Will contain the full path to the video file.
	* @param bufferSize Maximum size of the buffer.
	* @return One of the next constants:
	*  - MA_CAPTURE_RES_OK if no error occurred.
	*  - MA_CAPTURE_RES_INVALID_HANDLE if the given handle was invalid.
	*  - MA_CAPTURE_RES_INVALID_STRING_BUFFER_SIZE if the buffer size was to small.
	*/
	int maCaptureGetVideoPath(int handle, int buffer, int bufferSize)
	{
		return mMoSyncCapture.maCaptureGetVideoPath(handle, buffer, bufferSize);
	}

	/**
	* Destroys a image/video data object.
	* @param handle Handle to a image/video data object.
	* @return One of the next constants:
	*  - #MA_CAPTURE_RES_OK if no error occurred.
	*  - #MA_CAPTURE_RES_INVALID_HANDLE if the given handle was invalid.
	*/
	int maCaptureDestroyData(int handle)
	{
		return mMoSyncCapture.maCaptureDestroyData(handle);
	}

	/**
	* \brief Check if in app purchase is supported/enabled on a device.
	* It does not trigger any asynchronous responses.
	* Platform: Android and iOS.
	*
	* \returns One of the next result codes:
	* - #MA_PURCHASE_RES_OK if purchase is supported/allowed on the device.
	* - #MA_PURCHASE_RES_DISABLED if purchase is not allowed/enabled.
	* - #MA_PURCHASE_RES_UNAVAILABLE if purchase is not supported on the device.
	* - #MA_PURCHASE_RES_OUT_OF_DATE if GooglePlay application is out of date.
	* - #MA_PURCHASE_RES_CONNECTION_ERROR if there was an error connecting with the GooglePlay application.
	*/
	int maPurchaseSupported()
	{
		return mMoSyncPurchase.maPurchaseSupported();
	}

	/**
	* \brief Create a product object asynchronously.
	* The product is validated only on iOS. On the other hand, on Android the validation
	* will be done during a maPurchaseRequest.
	*
	* A #EVENT_TYPE_PURCHASE will be sent after calling this syscall.
	* The event will contain a MAPurchaseEventData struct object. The type member object
	* contained by the struct will be #MA_PURCHASE_EVENT_PRODUCT_CREATE. The state member
	* object can have one of the following values:
	* - #MA_PURCHASE_STATE_PRODUCT_VALID
	*				- on iOS if the product was validated by the App Store,
	*				- on Android if the product was internally allocated.
    * - #MA_PURCHASE_STATE_PRODUCT_INVALID the product is not valid in the App Store.
    * - #MA_PURCHASE_STATE_DISABLED purchase is not supported/disabled on the device.
    * - #MA_PURCHASE_STATE_DUPLICATE_HANDLE the given productHandle already exists.
	* Use #maCreatePlaceholder() to generate a new one.
	*
	* \param productHandle A valid handle that will be used to indetify the new product.
	* It must be unique. It is highly recommended to create it using #maCreatePlaceholder().
	* \param productID String that identifies the product. This string must be used by the
	* App Store / Google Play to identify the product.
	*/
	void maPurchaseCreate(final int productHandle, final String productID)
	{
		mMoSyncPurchase.maPurchaseCreate(productHandle, productID);
	}

	/**
	* \brief Set your Google Play public key to the application. This enables the application
	* to verify the signature of the transaction information that is returned from Google Play.
	* Must be set before #maVerifyReceipt.
	* Platform: Android.
	*
	* \param developerKey Base64-encoded public key, that can be found on the Google
	* Play publisher account page, under Licensing & In-app Billing panel in Edit Profile.
	*/
	int maPurchaseSetPublicKey(final String developerPublicKey)
	{
		return mMoSyncPurchase.maPurchaseSetPublicKey(developerPublicKey);
	}

	/**
	* Request the user to purchase a product.
	* The system will handle the proccess of purchasing.
	* Note: if there are another requests in progress, the requests will be queued.
	*
	* A #EVENT_TYPE_PURCHASE will be sent after calling this syscall.
	* The event will contain a MAPurchaseEventData struct object. The type member object
	* contained by the struct is #MA_PURCHASE_EVENT_REQUEST. The state member object can
	* have one of the following values:
    * - #MA_PURCHASE_STATE_DISABLED purchase is not supported/disabled on the device.
	* - #MA_PURCHASE_STATE_FAILED if the operation has failed. Check the errorCode member object
	* for more information about the reason.
	* - #MA_PURCHASE_STATE_IN_PROGRESS indicates that the transaction has been received by
	* the App Store / Google Play.
	* - #MA_PURCHASE_STATE_COMPLETED indicates that the transaction has been successfully processed.
	*
	* \param productHandle Handle to the product to be purchased.
	* \param quantity How many products to be purchased. Must be a value greater than zero.
	* This param is ignored on Android, as any purchase request can handle only one item.
	*/
	void maPurchaseRequest(final int productHandle)
	{
		mMoSyncPurchase.maPurchaseRequest(productHandle);
	}

	/**
	* Verify if the receipt came from Apple App Store / Google Play.
	* Make sure that the product is purchased before calling this syscall.
	*
	* A #EVENT_TYPE_PURCHASE will be sent after calling this syscall.
	* The event will contain a MAPurchaseEventData struct object. The type member object
	* contained by the struct is #MA_PURCHASE_EVENT_VERIFY_RECEIPT.The state member object
	* can have one of the following values:
    * - #MA_PURCHASE_STATE_DISABLED purchase is not supported/disabled on the device.
	* - #MA_PURCHASE_STATE_RECEIPT_VALID indicates that the transaction has been validated
	* by the App Store / Google Play.
	* - #MA_PURCHASE_STATE_RECEIPT_INVALID indicates that the transaction is invalid.
	* - #MA_PURCHASE_STATE_RECEIPT_ERROR indicates that an error occurred while verifying
	* the receipt. Check the errorCode member object for more information about the reason.
    *
	* \param productHandle Handle to the product that has been purchased.
	* - #MA_PURCHASE_RES_PUBLIC_KEY_NOT_SET.
	*/
	void maPurchaseVerifyReceipt(final int productHandle)
	{
		mMoSyncPurchase.maPurchaseVerifyReceipt(productHandle);
	}

	/**
	* Get product id using a product handle.
	*
	* \param productHandle Handle to the given product.
	* \param buffer Will contain the product id.
	* \param bufferSize Maximum size of the buffer.
	* \return In case of error:
	* - #MA_PURCHASE_RES_INVALID_HANDLE if productHandle is invalid.
	* - #MA_PURCHASE_RES_BUFFER_TOO_SMALL if the given handle is too small.
	* In case of success returns the number of written bytes.
	*/
	int maPurchaseGetName(
			final int productHandle,
			final int memBuffer,
			final int memSize)
	{
		return mMoSyncPurchase.maPurchaseGetName(productHandle, memBuffer, memSize);
	}

	/**
	* Get a field value contained by the receipt.
	* Make sure that the given product has a valid receipt.
	* Call the maPurchaseVerifyReceipt() syscall and wait for a #MA_PURCHASE_EVENT_RECEIPT_VALID
	* purchase event type.
	*
	* \param productHandle Handle to the product that has been purchased.
	* \param property The name of the field.
	* \param buffer Will be filled with the filed value.
	* \param bufferSize The maximum size of the buffer, in bytes.
	* \return The number of written bytes into buffer(a value greater than zero), or one
	* of the following error codes(a value smaller that zero):
	* - #MA_PURCHASE_RES_OK if the request has been send to the store for verifying.
	* - #MA_PURCHASE_RES_INVALID_HANDLE if the productHandle is invalid.
	* - #MA_PURCHASE_RES_RECEIPT if the product has not been purchased.
	* - #MA_PURCHASE_RES_DISABLED if purchase is not allowed/enabled.
	*/
	int maPurchaseGetField(
			final int productHandle,
			final String property,
			final int memBuffer,
			final int bufferSize)
	{
		return mMoSyncPurchase.maPurchaseGetField(productHandle, property, memBuffer, bufferSize);
	}

	/**
	* Restore transactions that were previously finished so that you can process them again.
	* For example, your application would use this to allow a user to unlock previously purchased
	* content onto a new device.
	*
	* A #EVENT_TYPE_PURCHASE will be sent after calling this syscall.
	* The event will contain a #MAPurchaseEventData struct object. The type member object
	* contained by the struct will be #MA_PURCHASE_EVENT_RESTORE. The productHandle member object
	* will contain a handle to the new product. Make sure you destroy the product after done
	* working with it.
	*/
	void maPurchaseRestoreTransactions()
	{
		mMoSyncPurchase.maPurchaseRestoreTransactions();
	}

	/**
	* Destroy a product object.
	* \param productHandle Handle to the product to destroy.
	* If the given handle is invalid the method does nothing.
	*/
	int maPurchaseDestroy(int handle)
	{
		return mMoSyncPurchase.maPurchaseDestroy(handle);
	}

	/**
	 * Send the application to background.
	 * @return
	 */
	int maSendToBackground()
	{
		//Log.i("MoSync", "maSendToBackground");

		final Activity activity = mContext;

		activity.moveTaskToBack(true);

		// Alternatively use an intent that shows the home screen.
		// The drawback is that the activity that displayed the
		// MoSync app will not be displayed. We will always go to
		// the home screen, so this solution is less desirable.
		/*
		Intent intent = new Intent(Intent.ACTION_MAIN);
		intent.addCategory(Intent.CATEGORY_HOME);
		intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		activity.startActivity(intent);
		*/

		return 0;
	}

	@Deprecated
	int maScreenSetOrientation(int orientation)
	{
		SYSLOG("@@MoSync maScreenSetOrientation " + orientation);

		return mOrientation.maScreenSetOrientation(orientation);
	}

	/**
	 * Set supported screen orientations.
	 * @param orientation A bitmask consisting of flags describing the supported screen orientations.
	 * On Android there is only one accepted value.
	 * @return One of the values:
	 * - #MA_SCREEN_ORIENTATION_RES_OK
	 * - #MA_SCREEN_ORIENTATION_RES_NOT_SUPPORTED
	 * - #MA_SCREEN_ORIENTATION_RES_INVALID_VALUE
	 */
	int maScreenSetSupportedOrientations(int orientation)
	{
		SYSLOG("MoSync maScreenSetSupportedOrientations " + orientation);

		return mOrientation.setSupportedOrientation(orientation);
	}

	/**
	* Get supported screen orientations.
	* @return A bitmask consisting of flags describing the supported screen orientations.
	* The bitmask is created using \link #MA_SCREEN_ORIENTATION_PORTRAIT MA_SCREEN_ORIENTATION \endlink
	* values.
	*/
	int maScreenGetSupportedOrientations()
	{
		SYSLOG("MoSync maScreenGetSupportedOrientations");

		return mOrientation.getSupportedOrientations();
	}

	/**
	* Get current screen orientation.
	* Currently implemented on iOS, WindowsPhone 7.1 and Android.
	* @return One of the \link #MA_SCREEN_ORIENTATION_PORTRAIT MA_SCREEN_ORIENTATION \endlink constants.
	*/
	int maScreenGetCurrentOrientation()
	{
		SYSLOG("@MoSync maScreenGetCurrentOrientation");

		return MoSync.getScreenOrientation();
	}

	int maScreenSetFullscreen(final int fullscreen)
	{
		//Log.i("MoSync", "maScreenSetFullscreen");

		if (1 == fullscreen)
		{
			maScreenSetFullscreenHelper(true);
		}
		else if (0 == fullscreen)
		{
			maScreenSetFullscreenHelper(false);
		}
		else
		{
			return -1;
		}

		return 0;
	}

	void maScreenSetFullscreenHelper(final boolean fullscreenOn)
	{
		final Activity activity = mContext;

		activity.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				if (fullscreenOn)
				{
					activity.getWindow().addFlags(
						WindowManager.LayoutParams.FLAG_FULLSCREEN);
					activity.getWindow().clearFlags(
						WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);
				}
				else
				{
					activity.getWindow().addFlags(
						WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);
					activity.getWindow().clearFlags(
						WindowManager.LayoutParams.FLAG_FULLSCREEN);
				}
			}
		});
	}

	int maConnect(final String url)
	{
		return mMoSyncNetwork.maConnect(url);
	}

	int maConnGetAddr(int connHandle, int addrPointer)
	{
		return mMoSyncNetwork.maConnGetAddr(connHandle, addrPointer);
	}

	void maConnClose(int connHandle)
	{
		mMoSyncNetwork.maConnClose(connHandle);
	}

	void maConnRead(int connHandle, int dst, int size)
	{
		mMoSyncNetwork.maConnRead(connHandle, dst, size);
	}

	void maConnWrite(int connHandle, int src, int size)
	{
		mMoSyncNetwork.maConnWrite(connHandle, src, size);
	}

	void maConnReadFrom(int connHandle, int dst, int size, int src)
	{
		mMoSyncNetwork.maConnReadFrom(connHandle, dst, size, src);
	}

	void maConnWriteTo(int connHandle, int src, int size, int dst)
	{
		mMoSyncNetwork.maConnWriteTo(connHandle, src, size, dst);
	}

	void maConnReadToData(int connHandle, int data, int offset, int size)
	{
		mMoSyncNetwork.maConnReadToData(connHandle, data, offset, size);
	}

	void maConnWriteFromData(
		int connHandle, int data, int offset, int size)
	{
		mMoSyncNetwork.maConnWriteFromData(connHandle, data, offset, size);
	}

	int maHttpCreate(String url, int method)
	{
		return mMoSyncNetwork.maHttpCreate(url, method);
	}

	void maHttpSetRequestHeader(int connHandle, String key, String value)
	{
		mMoSyncNetwork.maHttpSetRequestHeader(connHandle, key, value);
	}

	int maHttpGetResponseHeader(
		int connHandle, String key, int address, int bufSize)
	{
		return mMoSyncNetwork.maHttpGetResponseHeader(
			connHandle, key, address, bufSize);
	}

	void maHttpFinish(int connHandle)
	{
		mMoSyncNetwork.maHttpFinish(connHandle);
	}

	int maAccept(int connHandle)
	{
		return mMoSyncNetwork.maAccept(connHandle);
	}


	int maBtStartDeviceDiscovery(int names)
	{
		if (isBluetoothApiAvailable())
		{
			return mMoSyncBluetooth.maBtStartDeviceDiscovery(names);
		}
		else
		{
			return IOCTL_UNAVAILABLE;
		}
	}

	int maBtGetNewDevice(
		int nameBufPointer,
		int nameBufSize,
		int actualNameLengthPointer,
		int addressPointer)
	{
		if (isBluetoothApiAvailable())
		{
			return mMoSyncBluetooth.maBtGetNewDevice(
				nameBufPointer,
				nameBufSize,
				actualNameLengthPointer,
				addressPointer);
		}
		else
		{
			return IOCTL_UNAVAILABLE;
		}
	}

	int maBtCancelDiscovery()
	{
		if (isBluetoothApiAvailable())
		{
			return mMoSyncBluetooth.maBtCancelDiscovery();
		}
		else
		{
			return IOCTL_UNAVAILABLE;
		}
	}

	public int maLocationStart()
	{
		return mMoSyncLocation.maLocationStart();
	}

	public int maLocationStop()
	{
		return mMoSyncLocation.maLocationStop();
	}

	int maSoundPlay(int soundResource, int offset, int length)
	{
		return mMoSyncSound.maSoundPlay(soundResource, offset, length);
	}

	void maSoundStop()
	{
		mMoSyncSound.maSoundStop();
	}

	void maSoundSetVolume(int vol)
	{
		mMoSyncSound.maSoundSetVolume(vol);
	}

	int maSoundGetVolume()
	{
		return mMoSyncSound.maSoundGetVolume();
	}

	int maSoundIsPlaying()
	{
		return mMoSyncSound.maSoundIsPlaying();
	}

	int maAudioDataCreateFromResource(String mime, int data,
			int offset, int length, int flags)
	{
		return mMoSyncAudio.maAudioDataCreateFromResource(mime, data, offset, length, flags);
	}

	int maAudioDataCreateFromURL(String mime, String url, int flags)
	{
		return mMoSyncAudio.maAudioDataCreateFromURL(mime, url, flags);
	}

	int maAudioDataDestroy(int audioData)
	{
		return mMoSyncAudio.maAudioDataDestroy(audioData);
	}

	int maAudioInstanceCreate(int audioData)
	{
		return mMoSyncAudio.maAudioInstanceCreate(audioData);
	}

	int maAudioInstanceCreateDynamic(int sampleRate, int numChannels, int bufferSize)
	{
		return mMoSyncAudio.maAudioInstanceCreateDynamic(sampleRate, numChannels, bufferSize);
	}

	int maAudioSubmitBuffer(int audioInstance, int mem, int memSize)
	{
		return mMoSyncAudio.maAudioSubmitBuffer(audioInstance, mem, memSize);
	}

	int maAudioGetPendingBufferCount(int audioInstance)
	{
		return mMoSyncAudio.maAudioGetPendingBufferCount(audioInstance);
	}

	int maAudioInstanceDestroy(int audioInstance)
	{
		return mMoSyncAudio.maAudioInstanceDestroy(audioInstance);
	}

	int maAudioGetLength(int audio)
	{
		return mMoSyncAudio.maAudioGetLength(audio);
	}

	int maAudioSetNumberOfLoops(int audio, int loops)
	{
		return mMoSyncAudio.maAudioSetNumberOfLoops(audio, loops);
	}

	int maAudioPrepare(int audio, int async)
	{
		return mMoSyncAudio.maAudioPrepare(audio, async);
	}

	int maAudioPlay(int audio)
	{
		return mMoSyncAudio.maAudioPlay(audio);
	}

	int maAudioSetPosition(int audio, int milliseconds)
	{
		return mMoSyncAudio.maAudioSetPosition(audio, milliseconds);
	}

	int maAudioGetPosition(int audio)
	{
		return mMoSyncAudio.maAudioGetPosition(audio);
	}

	int maAudioSetVolume(int audio, float volume)
	{
		return mMoSyncAudio.maAudioSetVolume(audio, volume);
	}

	int maAudioPause(int audio)
	{
		return mMoSyncAudio.maAudioPause(audio);
	}

	int maAudioStop(int audio)
	{
		return mMoSyncAudio.maAudioStop(audio);
	}


	public int maAudioBufferInit(int info)
	{
		// TODO: Implement syscall.
		return IOCTL_UNAVAILABLE;
	}

	public int maAudioBufferReady()
	{
		// TODO: Implement syscall.
		return IOCTL_UNAVAILABLE;
	}

	public int maAudioBufferClose()
	{
		// TODO: Implement syscall.
		return IOCTL_UNAVAILABLE;
	}

	public int maSoundPlayConn(int conn, String mimeString)
	{
		// TODO: Implement syscall.
		return IOCTL_UNAVAILABLE;
	}



	/**
	 * Add a shortcut icon to the home screen. If called multiple times
	 * multiple icons will be added. The shortcut launches the current
	 * application.
	 * @param name The text that will be used for the shortcut label.
	 * @return < 0 on error
	 */
	int maHomeScreenShortcutAdd(String name)
	{
		return mMoSyncHomeScreen.maHomeScreenShortcutAdd(name);
	}

	/**
	 * Remove a shortcut icon to the home screen.
	 * @param name The shortcut(s) with this label will be removed.
	 * @return < 0 on error
	 */
	int maHomeScreenShortcutRemove(String name)
	{
		return mMoSyncHomeScreen.maHomeScreenShortcutRemove(name);
	}

	/**
	 * Turn on/off sending of HomeScreen events. Off by default.
	 * @param eventsOn 1 = events on, 0 = events off
	 */
	int maHomeScreenEventsOnOff(int eventsOn)
	{
		return mMoSyncHomeScreen.maHomeScreenEventsOnOff(eventsOn);
	}

	/**
	 * Activate/deactivate wake lock.
	 * @param flag
	 */
	int maWakeLock(final int flag)
	{
		getActivity().runOnUiThread(new Runnable()
		{
			public void run()
			{
				try
				{
					if (MA_WAKE_LOCK_ON == flag)
					{
						Window w = mContext.getWindow();
						w.setFlags(
							WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,
							WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
					}
					else
					{
						Window w = mContext.getWindow();
						w.setFlags(
							0,
							WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
					}
				}
				catch(Exception ex)
				{
					Log.i("MoSync", "maWakeLock: Could not set wake lock.");
					ex.printStackTrace();
				}
			}
		});
		return 1;
	}

	/**
	 * Internal wrapper for maWidgetCreate that runs
	 * the call in the UI thread.
	 */
	public int maWidgetCreate(final String type)
	{
		return mMoSyncNativeUI.maWidgetCreate(type);
	}

	/**
	 * Internal wrapper for maWidgetDestroy that runs
	 * the call in the UI thread.
	 */
	public int maWidgetDestroy(final int widget)
	{
		return mMoSyncNativeUI.maWidgetDestroy(widget);
	}

	/**
	 * Internal wrapper for maWidgetAddChild that runs
	 * the call in the UI thread.
	 */
	public int maWidgetAddChild(
		final int parentHandle,
		final int childHandle)
	{
		return mMoSyncNativeUI.maWidgetAddChild(parentHandle, childHandle);
	}

	/**
	 * Internal wrapper for maWidgetInsertChild that runs
	 * the call in the UI thread.
	 */
	public int maWidgetInsertChild(
		final int parentHandle,
		final int childHandle,
		final int index)
	{
		return mMoSyncNativeUI.maWidgetInsertChild(parentHandle, childHandle, index);
	}

	/**
	 * Internal wrapper for maWidgetRemoveChild that runs
	 * the call in the UI thread.
	 */
	public int maWidgetRemoveChild(
		final int childHandle)
	{
		return mMoSyncNativeUI.maWidgetRemoveChild(childHandle);
	}

	/**
	 * Internal wrapper for maWidgetModalDialogShow.
	 * @param dialogHandle
	 */
	public int maWidgetModalDialogShow(
			final int dialogHandle)
	{
		return mMoSyncNativeUI.maWidgetDialogShow(dialogHandle);
	}

	/**
	 * Internal wrapper for maWidgetModalDialogHide.
	 * @param dialogHandle
	 */
	public int maWidgetModalDialogHide(
			final int dialogHandle)
	{
		return mMoSyncNativeUI.maWidgetDialogHide(dialogHandle);
	}

	/**
	 * Internal wrapper for maWidgetScreenShow that runs
	 * the call in the UI thread.
	 */
	public int maWidgetScreenShow(final int screenHandle)
	{
		return mMoSyncNativeUI.maWidgetScreenShow(screenHandle);
	}

	/**
	 * Internal wrapper for maWidgetScreenShowWithTransition that runs
	 * the call in the UI thread.
	 */
	public int maWidgetScreenShowWithTransition(final int screenHandle, final int screenTransitionType, final int screenTransitionDurations)
	{
		return mMoSyncNativeUI.maWidgetScreenShowWithTransition(screenHandle, screenTransitionType, screenTransitionDurations);
	}

	/**
	 * Internal wrapper for maWidgetSetProperty that runs
	 * the call in the UI thread.
	 */
	public int maWidgetSetProperty(
		final int widgetHandle,
		final String key,
		final String value)
	{
		return mMoSyncNativeUI.maWidgetSetProperty(widgetHandle, key, value);
	}

	/**
	 * Internal wrapper for maWidgetGetProperty that runs
	 * the call in the UI thread.
	 */
	public int maWidgetGetProperty(
			final int widgetHandle,
			final String key,
			final int memBuffer,
			final int memBufferSize)
	{
		return mMoSyncNativeUI.maWidgetGetProperty(
			widgetHandle, key, memBuffer, memBufferSize);
	}

	/**
	 * Add an item to the Options Menu associated to a screen.
	 * @param widgetHandle The screen handle.
	 * @param title The title associated for the new item. Can be left null.
	 * @param iconHandle MoSync handle to an uncompressed image resource,or:
	 * a predefined Android icon.
	 * @param iconPredefined Specifies if the icon is a project resource, or one of
	 * the predefined Android icons. By default it's value is 0.
	 * @return The index on which the menu item was added in the options menu,
	 * or an error code otherwise.
	 */
	public int maWidgetScreenAddOptionsMenuItem(
			final int widgetHandle,
			final String title,
			final String iconHandle,
			final int iconPredefined)
	{
		return mMoSyncNativeUI.maWidgetScreenAddOptionsMenuItem(
				widgetHandle, title, iconHandle, iconPredefined);
	}

	public void invalidateOptionsMenu(Activity activity) {
		Class<?> activityClass = null;
		Method activity_invalidateOptionMenu = null;
		try {
			activityClass = Class.forName("android.app.Activity");
		} catch (Throwable e) {
			System.err.println(e);
		}

		// Search for invalidateOptionsMenu into the Activity class.
		try {
			activity_invalidateOptionMenu = activityClass
					.getMethod("invalidateOptionsMenu");
			activity_invalidateOptionMenu.invoke(activity);
			/* success, this is a newer device */
		} catch (Exception e) {
			/* failure, must be older device */
			Log.i("MoSync", "invalidateOptionsMenu failed");
		}
	}

	/**
	 * Get the focused  screen.
	 * @return The screen widget handle.
	 */
	public ScreenWidget getCurrentScreen()
	{
		return mMoSyncNativeUI.getCurrentScreen();
	}

	public void setCurrentScreen(int handle)
	{
		mMoSyncNativeUI.setCurrentScreen(handle);
	}

	/**
	 * Get the current screen without conversions.
	 * @return The current screen without conversions.
	 */
	public ScreenWidget getUnconvertedCurrentScreen()
	{
		return mMoSyncNativeUI.getUnconvertedCurrentScreen();
	}

	/**
	 * Internal wrapper for maWidgetStackScreenPush that runs
	 * the call in the UI thread.
	 */
	public int maWidgetStackScreenPush(
		final int stackScreenHandle,
		final int newScreenHandle)
	{
		return mMoSyncNativeUI.maWidgetStackScreenPush(stackScreenHandle, newScreenHandle);
	}

	/**
	 * Internal wrapper for maWidgetStackScreenPop that runs
	 * the call in the UI thread.
	 */
	public int maWidgetStackScreenPop(
		final int stackScreenWidget)
	{
		return mMoSyncNativeUI.maWidgetStackScreenPop(stackScreenWidget);
	}

	/**
	 * Releases the locked hardware for use in other apps
	 */
	public void releaseHardware()
	{
		if (mMoSyncCameraController != null) {
			mMoSyncCameraController.releaseCamera();
		}
	}

	/**
	 * Reacquires the released hardware in case of resume
	 */
	public void acquireHardware()
	{
		if (mMoSyncCameraController != null)
		{
			mMoSyncCameraController.acquireCamera();
		}
	}

	/**
	 * starts the Camera Preview
	 *
	 * @return 1 if succeeds
	 */
	int maCameraStart()
	{
		if(mMoSyncCameraController == null)
		{
			return IOCTL_UNAVAILABLE;
		}

		if ( mMoSyncCameraController.isSnapshotInProgress() )
		{
			return MA_CAMERA_RES_SNAPSHOT_IN_PROGRESS;
		}

		//Start a fullscreen preview and then start the camera
		if(false == mMoSyncCameraController.hasView())
		{
			if(cameraScreen == 0)
			{
				cameraScreen =  maWidgetCreate(IX_WIDGET.MAW_SCREEN);
				int cameraPreview = maWidgetCreate(IX_WIDGET.MAW_CAMERA_PREVIEW);
				maWidgetAddChild(cameraScreen, cameraPreview);
				FrameLayout layout =  (FrameLayout) mMoSyncNativeUI.getCameraPreview(cameraPreview).getView();
				mMoSyncCameraController.setPreview(
					(MoSyncCameraPreview)layout.getChildAt(0)
					);
				maWidgetScreenShow(cameraScreen);
			}
		}
		return mMoSyncCameraController.cameraStart();
	}

	/**
	 * Stops the Camera Preview
	 *
	 * @return 1 if succeeds
	 */
	int maCameraStop()
	{
		if(mMoSyncCameraController == null)
		{
			return IOCTL_UNAVAILABLE;
		}

		int result = mMoSyncCameraController.cameraStop();
		//go back to the previous screen
		if(cameraScreen != 0)
		{
			maWidgetDestroy(cameraScreen);
			mMoSyncCameraController.removePreview();
			maWidgetScreenShow(IX_WIDGET.MAW_CONSTANT_MOSYNC_SCREEN_HANDLE);
			cameraScreen = 0;
		}
		return result;
	}

	/**
	 * Takes a snapshot and stores it in the place holder
	 *
	 * @param formatIndex index of the format set by the user
	 * @param placeHolder a place holder
	 * @return <0 if fails and >0 if succeeds
	 */
	int maCameraSnapshot(int formatIndex, int placeHolder)
	{
		if(mMoSyncCameraController == null)
		{
			return IOCTL_UNAVAILABLE;
		}

		if ( mMoSyncCameraController.isSnapshotInProgress() )
		{
			return MA_CAMERA_RES_SNAPSHOT_IN_PROGRESS;
		}

		return mMoSyncCameraController.cameraSnapshot(formatIndex, placeHolder);
	}

	/**
	 * Takes a snapshot and send the place holder created via
	 * #EVENT_TYPE_CAMERA_SNAPSHOT.
	 *
	 * @param sizeIndex index of the snapshot size
	 * @return IOCTL_UNAVAILABLE, MA_CAMERA_RES_FAILED if fails
	 * and MA_CAMERA_RES_OK if succeeds
	 */
	int maCameraSnapshotAsync(int dataPlaceholder, int sizeIndex)
	{
		if(mMoSyncCameraController == null)
		{
			return IOCTL_UNAVAILABLE;
		}
		if ( mMoSyncCameraController.isSnapshotInProgress() )
		{
			return MA_CAMERA_RES_SNAPSHOT_IN_PROGRESS;
		}
		mMoSyncCameraController.cameraSnapshotAsync(dataPlaceholder, sizeIndex);
		return MA_CAMERA_RES_OK;
	}


	/**
	 * binds a preview to the camera
	 *
	 * @param widgetHandle the handle to the Camera preview widget
	 * @return 0 if widget not found, 1 for success
	 */
	int maCameraSetPreview(int widgetHandle)
	{
		if(mMoSyncCameraController == null)
		{
			return IOCTL_UNAVAILABLE;
		}

		FrameLayout layout =
				(FrameLayout)mMoSyncNativeUI.getCameraPreview(widgetHandle).getView();
		MoSyncCameraPreview preview = (MoSyncCameraPreview)layout.getChildAt(0);
		if(preview == null)
		{
			return 0; //Widget Not Found
		}
		mMoSyncCameraController.setPreview(preview);

		return 1;
	}

	/**
	 * Selects the active Camera
	 *
	 * @param cameraNumber index of the camera in the list
	 * @return > 0 for success, < 0 on failure
	 */
	int maCameraSelect(int cameraNumber)
	{
		if(mMoSyncCameraController == null)
		{
			return IOCTL_UNAVAILABLE;
		}

		return mMoSyncCameraController.setActiveCamera(cameraNumber);
	}

	/**
	 * Queries the number of available cameras on the device
	 *
	 * @return number of available cameras
	 */
	int maCameraNumber()
	{
		if(mMoSyncCameraController == null)
		{
			return IOCTL_UNAVAILABLE;
		}

		return mMoSyncCameraController.numberOfCameras();
	}

	/**
	 * Queries the number of supported formats
	 *
	 * @return number of supported formats
	 */
	int maCameraFormatNumber()
	{
		if(mMoSyncCameraController == null)
		{
			return IOCTL_UNAVAILABLE;
		}

		return mMoSyncCameraController.getNumberOfPictureSizes();
	}

	/**
	 * Adds a custom format to the list
	 *
	 * @param index index of the specific format
	 * @param width image width
	 * @param height image height
	 * @return 1 for success
	 */

	int maCameraFormat(int index, final int format)
	{
		if(mMoSyncCameraController == null)
		{
			return IOCTL_UNAVAILABLE;
		}

		mMoSyncCameraController.getSize(index, format);
		return 1;
	}


	public int maCameraSetProperty(
		final String key,
		final String value)
	{
		if(mMoSyncCameraController == null)
		{
			return IOCTL_UNAVAILABLE;
		}

		return mMoSyncCameraController.setCameraProperty(key, value);
	}

	public int maCameraGetProperty(
			final String key,
			final int memBuffer,
			final int memBufferSize)
	{
		if(mMoSyncCameraController == null)
		{
			return IOCTL_UNAVAILABLE;
		}

		return mMoSyncCameraController.getCameraProperty(key,
										memBuffer,
										memBufferSize);
	}

	public int maCameraPreviewSize()
	{
		if(mMoSyncCameraController == null)
		{
			return IOCTL_UNAVAILABLE;
		}

		return mMoSyncCameraController.getPreviewSize();
	}

	public int maCameraPreviewEventEnable(
			int eventType,
			int previewBuffer,
			int rectLeft,
			int rectTop,
			int rectWidth,
			int rectHeight
		)
	{
		if(mMoSyncCameraController == null)
		{
			return IOCTL_UNAVAILABLE;
		}

		int returnVal = mMoSyncCameraController.enablePreviewEvents(
			eventType, previewBuffer, rectLeft, rectTop,
			rectWidth, rectHeight);

		return returnVal;
	}

	int maCameraPreviewEventDisable()
	{
		if(mMoSyncCameraController == null)
		{
			return IOCTL_UNAVAILABLE;
		}

		return mMoSyncCameraController.disablePreviewEvents();
	}

	int maCameraPreviewEventConsumed()
	{
		if(mMoSyncCameraController == null)
		{
			return IOCTL_UNAVAILABLE;
		}

		return mMoSyncCameraController.previewEventConsumed();
	}


	/**
	 * Called when the back button has been pressed.
	 */
	public void handleBack()
	{
		mMoSyncNativeUI.handleBack();
	}

	/**
	 * Loads an OpenGL texture to the current texture handle. If the
	 * underlying bitmap has an alpha channel, the texture will also
	 * get an alpha channel.
	 *
	 * @param bitmapHandle Handle to the bitmap to load.
	 *
	 * @return 0 on success, RES_BAD_INPUT if the handle does not exist,
	 *         and -3 if the texture could not be loaded.
	 */
	public int loadGlTexture(final int bitmapHandle)
	{
		// Load texture as standard texture.
		return loadGlTextureHelper(bitmapHandle, false);
	}

	/**
	 * Turn on screen on/off events.
	 * @param eventsOn 1 = events on, 0 = events off
	 * @return 1 on success, < 0 on failure.
	 */
	int maScreenStateEventsOnOff(int eventsOn)
	{
		if (1 == eventsOn)
		{
			// There should be no receiver active.
			if (null != mScreenActivatedReceiver)
			{
				return -2;
			}

			IntentFilter filter = new IntentFilter();
			filter.addAction(Intent.ACTION_SCREEN_ON);
			filter.addAction(Intent.ACTION_SCREEN_OFF);
			mScreenActivatedReceiver = new ScreenActivatedReceiver(this);
			getActivity().registerReceiver(mScreenActivatedReceiver, filter);

			return 1;
		}
		else if (0 == eventsOn)
		{
			// There should be a receiver active.
			if (null == mScreenActivatedReceiver)
			{
				return -2;
			}

			getActivity().unregisterReceiver(mScreenActivatedReceiver);

			mScreenActivatedReceiver = null;

			return 1;
		}

		// eventsOn was not a valid on/off flag.
		return -2;
	}

	/**
	 * Loads an OpenGL sub texture to the current texture handle. If the
	 * underlying bitmap has an alpha channel, the texture will also
	 * get an alpha channel.
	 *
	 * @param bitmapHandle Handle to the bitmap to load.
	 *
	 * @return 0 on success, RES_BAD_INPUT if the handle does not exist,
	 *         and -3 if the texture could not be loaded.
	 */
	public int loadGlSubTexture(final int bitmapHandle)
	{
		// Load texture as sub texture.
		return loadGlTextureHelper(bitmapHandle, true);
	}

	/**
	 * Helper method that load a bitmap as a texture or as a sub texture.
	 * @param bitmapHandle Texture bitmap handle.
	 * @param isSubTexture true if sub texture false if standard texture.
	 * @return A status code.
	 */
	private int loadGlTextureHelper(
		final int bitmapHandle,
		final boolean isSubTexture)
	{
		// Get bitmap object.
		ImageCache texture = mImageResources.get(bitmapHandle);
		if(texture == null)
		{
			// Could not find bit map object.
			return RES_BAD_INPUT;
		}

		// Ensure that the bitmap we use has dimensions that are power of 2.
		Bitmap bitmap = loadGlTextureHelperMakeBitmapPowerOf2(texture.mBitmap);

		try
		{
			/**
			 * It is important here to let Android choose the internal format,
			 * by setting it to -1. Otherwise due to a bug in Android 1.5 it will
			 * always throw an exception stating 'invalid Bitmap format' when loading
			 * an image that previously was RGB.
			 */
			if (isSubTexture)
			{
				GLUtils.texSubImage2D(GL10.GL_TEXTURE_2D,
				  0,
				  0,
				  0,
				  bitmap,
				  -1,
				  GL10.GL_UNSIGNED_BYTE); // texSubImage2D does not support -1 here
			}
			else
			{
				GLUtils.texImage2D(
					GL10.GL_TEXTURE_2D,
					0,
					-1,
					bitmap,
					-1,
					0);
			}
		}
		catch (Throwable t)
		{
			//Log.i("MoSyncThread", "Exception in loadGlTexture: " + t.toString());
			t.printStackTrace();
		}
		finally
		{
			if (bitmap != texture.mBitmap)
			{
				recycleImageData(bitmap);
			}
		}

		// Check error status.
		EGL10 egl = (EGL10) EGLContext.getEGL();
		GL10 gl = (GL10) egl.eglGetCurrentContext().getGL();
		int glError = gl.glGetError();
		if(glError == GL10.GL_NO_ERROR)
		{
			return 0; // Success, no errors.
		}
		else
		{
			//Log.i("MoSyncThread", "Could not load texture glGetError returned: 0x" + Integer.toHexString(glError));
			return -3; // Texture could not be loaded.
		}
	}

	/**
	 * Ensures that the bitmap has width and height that is power of 2.
	 * @param bitmap Source bitmap.
	 * @return Source bitmap if it already has power of 2 dimensions,
	 * a new bitmap if not, that has power of 2 dimensions.
	 */
	private Bitmap loadGlTextureHelperMakeBitmapPowerOf2(Bitmap bitmap)
	{
		int width = bitmap.getWidth();
		int height = bitmap.getHeight();

		if (loadGlTextureHelperIsPowerOf2(width) &&
			loadGlTextureHelperIsPowerOf2(height))
		{
			// Bitmap is already power of 2, return the
			// the original bitmap.
			return bitmap;
		}

		// Allocate new bitmap.
		int newWidth = loadGlTextureHelperGetNextPowerOf2(width);
		int newHeight = loadGlTextureHelperGetNextPowerOf2(height);

		Bitmap newBitmap = createBitmap(newWidth, newHeight);

		if (null == newBitmap)
		{
			return null;
		}

		// Blit original bitmap to new bitmap.
		Canvas canvas = new Canvas(newBitmap);
		canvas.drawBitmap(bitmap, 0.0f, 0.0f, null);

		return newBitmap;
	}

	/**
	 * Tests is a number is a power of 2.
	 * @param n Number to test.
	 * @return true if number is a power of 2.
	 */
	private boolean loadGlTextureHelperIsPowerOf2(int n)
	{
		return ((0 != n) && (0 == (n & (n - 1))));
	}

	/**
	 * Return a number that is the next higher power of two after n.
	 * @param n Number to test.
	 * @return The next power of 2 number that is higher than n.
	 */
	private int loadGlTextureHelperGetNextPowerOf2(int n)
	{
		int powerOf2 = 2;

		while (powerOf2 < n)
		{
			powerOf2 = powerOf2 << 1;
		}

		return powerOf2;
	}


	private int mOpenGLScreen = -1;
	private int mOpenGLView = -1;

	int maOpenGLInitFullscreen(int glApi) {
		if(mOpenGLScreen != -1) return 0;

        if(glApi == IX_OPENGL_ES_MA.MA_GL_API_GL1)
            mOpenGLView = maWidgetCreate("GLView");
        else if(glApi == IX_OPENGL_ES_MA.MA_GL_API_GL2)
            mOpenGLView = maWidgetCreate("GL2View");
        else
            return IX_OPENGL_ES_MA.MA_GL_INIT_RES_UNAVAILABLE_API;

        if(mOpenGLView < 0) {
            return IX_OPENGL_ES_MA.MA_GL_INIT_RES_UNAVAILABLE_API;
        }

        mOpenGLScreen = maWidgetCreate("Screen");
		maWidgetSetProperty(mOpenGLView, "width", "-1");
		maWidgetSetProperty(mOpenGLView, "height", "-1");
		maWidgetAddChild(mOpenGLScreen, mOpenGLView);
		maWidgetScreenShow(mOpenGLScreen);
		maWidgetSetProperty(mOpenGLView, "bind", "");
		return 1;
	}

	int maOpenGLCloseFullscreen() {
		if(mOpenGLScreen == -1) return 0;
		maWidgetRemoveChild(mOpenGLView);
		maWidgetDestroy(mOpenGLView);
		maWidgetDestroy(mOpenGLScreen);
		mOpenGLView = -1;
		mOpenGLScreen = -1;
		return 1;
	}


	int maFileOpen(String path, int mode)
	{
		return mMoSyncFile.maFileOpen(path, mode);
	}

	int maFileExists(int file)
	{
		return mMoSyncFile.maFileExists(file);
	}

	int maFileClose(int file)
	{
		return mMoSyncFile.maFileClose(file);
	}

	int maFileCreate(int file)
	{
		return mMoSyncFile.maFileCreate(file);
	}

	int maFileDelete(int file)
	{
		return mMoSyncFile.maFileDelete(file);
	}

	int maFileSize(int file)
	{
		return mMoSyncFile.maFileSize(file);
	}

	int maFileAvailableSpace(int file)
	{
		return mMoSyncFile.maFileAvailableSpace(file);
	}

	int maFileTotalSpace(int file)
	{
		return mMoSyncFile.maFileTotalSpace(file);
	}

	int maFileDate(int file)
	{
		return mMoSyncFile.maFileDate(file);
	}

	int maFileRename(int file, String newName)
	{
		return mMoSyncFile.maFileRename(file, newName);
	}

	int maFileTruncate(int file, int offset)
	{
		return mMoSyncFile.maFileTruncate(file, offset);
	}

	int maFileWrite(int file, int src, int len)
	{
		return mMoSyncFile.maFileWrite(file, src, len);
	}

	int maFileWriteFromData(int file, int data, int offset, int len)
	{
		return mMoSyncFile.maFileWriteFromData(file, data, offset, len);
	}

	int maFileRead(int file, int dst, int len)
	{
		return mMoSyncFile.maFileRead(file, dst, len);
	}

	int maFileReadToData(int file, int data, int offset, int len)
	{
		return mMoSyncFile.maFileReadToData(file, data, offset, len);
	}

	int maFileTell(int file)
	{
		return mMoSyncFile.maFileTell(file);
	}

	int maFileSeek(int file, int offset, int whence)
	{
		return mMoSyncFile.maFileSeek(file, offset, whence);
	}

	int maFileListStart(String path, String filter)
	{
		return mMoSyncFile.maFileListStart(path, filter);
	}

	int maFileListNext(int list, int nameBuf, int bufSize)
	{
		return mMoSyncFile.maFileListNext(list, nameBuf, bufSize);
	}

	int maFileListClose(int list)
	{
		return mMoSyncFile.maFileListClose(list);
	}

	int maSaveImageToDeviceGallery(int imageHandle, String imageName)
	{
		return MediaManager.exportImageToPhotoGallery(imageHandle, imageName);
	}

	int maSensorStart(int sensor, int interval)
	{
		return mMoSyncSensor.maSensorStart(sensor, interval);
	}

	int maSensorStop(int sensor)
	{
		return mMoSyncSensor.maSensorStop(sensor);
	}

	/**
	 * Panics
	 */
	int maSyscallPanicsEnable()
	{
		MoSyncError.getSingletonObject().setFlag(true);
		return 0;
	}

	int maSyscallPanicsDisable()
	{
		MoSyncError.getSingletonObject().setFlag(false);
		return 0;
	}

	/**
	 * PIM related methods
	 */
	int maPimListOpen(int listType)
	{
		return mMoSyncPIM.maPimListOpen(listType);
	}

	int maPimListNext(int list)
	{
		return mMoSyncPIM.maPimListNext(list);
	}

	int maPimListNextSummary(int list)
	{
		return mMoSyncPIM.maPimListNextSummary(list);
	}

	int maPimListClose(int list)
	{
		return mMoSyncPIM.maPimListClose(list);
	}

	int maPimItemCount(int item)
	{
		return mMoSyncPIM.maPimItemCount(item);
	}

	int maPimItemGetField(int item, int n)
	{
		return mMoSyncPIM.maPimItemGetField(item, n);
	}

	int maPimItemFieldCount(int item, int field)
	{
		return mMoSyncPIM.maPimItemFieldCount(item, field);
	}

	int maPimItemGetAttributes(int item, int field, int index)
	{
		return mMoSyncPIM.maPimItemGetAttributes(item, field, index);
	}

	int maPimItemSetLabel(int item, int field, int buffPointer, int buffSize, int index)
	{
		return mMoSyncPIM.maPimItemSetLabel(item, field, buffPointer, buffSize, index);
	}

	int maPimItemGetLabel(int item, int field, int buffPointer, int buffSize, int index)
	{
		return mMoSyncPIM.maPimItemGetLabel(item, field, buffPointer, buffSize, index);
	}

	int maPimFieldType(int list, int field)
	{
		return mMoSyncPIM.maPimFieldType(list, field);
	}

	int maPimItemGetValue(int item, int field, int buffPointer, int buffSize, int index)
	{
		return mMoSyncPIM.maPimItemGetValue(item, field, buffPointer, buffSize, index);
	}

	int maPimItemSetValue(int item, int field, int buffPointer, int buffSize, int index, int attributes)
	{
		return mMoSyncPIM.maPimItemSetValue(item, field, buffPointer, buffSize, index, attributes);
	}

	int maPimItemAddValue(int item, int field, int buffPointer, int buffSize, int attributes)
	{
		return mMoSyncPIM.maPimItemAddValue(item, field, buffPointer, buffSize, attributes);
	}

	int maPimItemRemoveValue(int item, int field, int index)
	{
		return mMoSyncPIM.maPimItemRemoveValue(item, field, index);
	}

	int maPimItemClose(int item)
	{
		return mMoSyncPIM.maPimItemClose(item);
	}

	int maPimItemCreate(int list)
	{
		return mMoSyncPIM.maPimItemCreate(list);
	}

	int maPimItemRemove(int list, int item)
	{
		return mMoSyncPIM.maPimItemRemove(list, item);
	}

	int maNFCStart() {
		return mMoSyncNFC == null ? MA_NFC_NOT_AVAILABLE : mMoSyncNFC.maNFCStart();
	}

	void maNFCStop() {
		if (mMoSyncNFC != null) {
			mMoSyncNFC.maNFCStop();
		}
	}

	int maNFCReadTag(int nfcContext) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCReadTag(nfcContext);
	}

	void maNFCDestroyTag(int tagHandle) {
		if (mMoSyncNFC != null) {
			mMoSyncNFC.maNFCDestroyTag(tagHandle);
		}
	}

	int maNFCIsType(int tagHandle, int type) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCIsType(tagHandle, type);
	}

	int maNFCGetTypedTag(int tagHandle, int type) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCGetTypedTag(tagHandle, type);
	}

	int maNFCBatchStart(int nfcContext) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCBatchStart(nfcContext);
	}

	void maNFCBatchCommit(int nfcContext) {
		if (mMoSyncNFC != null) {
			mMoSyncNFC.maNFCBatchCommit(nfcContext);
		}
	}

	void maNFCBatchRollback(int nfcContext) {
		if (mMoSyncNFC != null) {
			mMoSyncNFC.maNFCBatchRollback(nfcContext);
		}
	}

	public int maNFCTransceive(int tagHandle, int src, int len, int dst, int dstLen, int dstPtr) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCTransceive(tagHandle, src, len, dst, dstLen, dstPtr);
	}

	public int maNFCGetSize(int tagHandle) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCGetSize(tagHandle);
	}

	public int maNFCGetId(int tagHandle, int dst, int len) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCGetId(tagHandle, dst, len);
	}

	void maNFCConnectTag(int tagHandle) {
		if (mMoSyncNFC != null) {
			mMoSyncNFC.maNFCConnectTag(tagHandle);
		}
	}

	void maNFCCloseTag(int tagHandle) {
		if (mMoSyncNFC != null) {
			mMoSyncNFC.maNFCCloseTag(tagHandle);
		}
	}

	int maNFCReadNDEFMessage(int tagHandle) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCReadNDEFMessage(tagHandle);
	}

	int maNFCGetNDEFMessage(int tagHandle) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCGetNDEFMessage(tagHandle);
	}

	int maNFCWriteNDEFMessage(int tagHandle, int ndefHandle) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCWriteNDEFMessage(tagHandle, ndefHandle);
	}

	int maNFCCreateNDEFMessage(int recordCount) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCCreateNDEFMessage(recordCount);
	}

	int maNFCGetNDEFRecord(int ndefHandle, int ix) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCGetNDEFRecord(ndefHandle, ix);
	}

	int maNFCGetNDEFRecordCount(int ndefHandle) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCGetNDEFRecordCount(ndefHandle);
	}

	int maNFCGetNDEFId(int ndefRecordHandle, int dst, int len) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCGetNDEFId(ndefRecordHandle, dst, len);
	}

	int maNFCGetNDEFPayload(int ndefRecordHandle, int dst, int len) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCGetNDEFPayload(ndefRecordHandle, dst, len);
	}

	int maNFCGetNDEFTnf(int ndefRecordHandle) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCGetNDEFTnf(ndefRecordHandle);
	}

	int maNFCGetNDEFType(int ndefRecordHandle, int dst, int len) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCGetNDEFType(ndefRecordHandle, dst, len);
	}

	int maNFCSetNDEFId(int ndefRecordHandle, int src, int len) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCSetNDEFId(ndefRecordHandle, src, len);
	}

	int maNFCSetNDEFPayload(int ndefRecordHandle, int src, int len) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCSetNDEFPayload(ndefRecordHandle, src, len);
	}

	int maNFCSetNDEFTnf(int ndefRecordHandle, int tnf) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCSetNDEFTnf(ndefRecordHandle, tnf);
	}

	int maNFCSetNDEFType(int ndefRecordHandle, int src, int len) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCSetNDEFType(ndefRecordHandle, src, len);
	}

	public int maNFCAuthenticateMifareSector(int tagHandle, int keyType, int sectorIndex, int keySrc, int keyLen) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCAuthenticateMifareSector(tagHandle, keyType, sectorIndex, keySrc, keyLen);
	}

	public int maNFCGetMifareSectorCount(int tagHandle) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCGetMifareSectorCount(tagHandle);
	}

	public int maNFCGetMifareBlockCountInSector(int tagHandle, int sectorIndex) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCGetMifareBlockCountInSector(tagHandle, sectorIndex);
	}

	public int maNFCMifareSectorToBlock(int tagHandle, int sectorIndex) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCMifareSectorToBlock(tagHandle, sectorIndex);
	}

	public int maNFCReadMifareBlocks(int tagHandle, int block, int dst, int resultSize) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCReadMifareBlocks(tagHandle, block, dst, resultSize);
	}

	public int maNFCReadMifarePages(int tagHandle, int firstPage, int dst, int resultSize) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCReadMifarePages(tagHandle, firstPage, dst, resultSize);
	}

	int maNFCWriteMifareBlocks(int tagHandle, int firstBlock, int src, int len) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCWriteMifareBlocks(tagHandle, firstBlock, src, len);
	}

	int maNFCWriteMifarePages(int tagHandle, int firstPage, int src, int len) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCWriteMifarePages(tagHandle, firstPage, src, len);
	}

	int maNFCSetReadOnly(int tagHandle) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCSetReadOnly(tagHandle);
	}

	int maNFCIsReadOnly(int tagHandle) {
		return mMoSyncNFC == null ? IOCTL_UNAVAILABLE : mMoSyncNFC.maNFCIsReadOnly(tagHandle);
	}

	/**
	 * Fill in an MACellInfo.
	 * @param cellinfo Pointer to an MACellInfo struct in MoSync memory.
	 * @return 0 on success, <0 on error.
	 * TODO: Should there be constants for error codes? Now they are
	 * undocumented (no documentation in maapi.idl).
	 */
	int maGetCellInfo(int cellinfo)
	{
		// Check that the Coarse Location permission is set,
		// otherwise the CellID request will freeze the device.
		int permission = mContext.getPackageManager().checkPermission(
			"android.permission.ACCESS_COARSE_LOCATION",
			mContext.getPackageName());
		if (permission != PackageManager.PERMISSION_GRANTED)
		{
			return -3; // Error (Permission not set).
		}

		TelephonyManager manager = (TelephonyManager)
				mContext.getSystemService(Context.TELEPHONY_SERVICE);
		if (null == manager)
		{
			return -2;
		}

		// Only works with GSM type phone, not with CDMA.
		if (manager.getPhoneType() != TelephonyManager.PHONE_TYPE_GSM)
		{
			return -2;
		}

		// Get the Cell information
		GsmCellLocation gsmCell = (GsmCellLocation) manager.getCellLocation();
		if (null == gsmCell)
		{
			return -2;
		}

		// Get cell id and lac.
		int cellId = gsmCell.getCid();
		int lac = gsmCell.getLac();

		// Get MCC and NMC.
		// MCC and NMC is concatenated in networkOperator.
		// First 3 chars are MCC, remaining chars are MNC.
		String mcc = "";
		String mnc = "";
		String networkOperator = manager.getNetworkOperator();
		if (null == networkOperator)
		{
			return -2;
		}

		if (networkOperator.length() > 4)
		{
			// Extract values.
			mcc = networkOperator.substring(0, 3);
			mnc = networkOperator.substring(3);

			// Guard against overflow.
			if (mnc.length() > 7)
			{
				mnc = mnc.substring(0, 8);
			}
		}

		// Store everything in the correct memory location.
		// Size of MACellInfo is 20 bytes.
		ByteBuffer mem;
		// Store MCC (max 4 bytes including terminator char).
		mem = getMemorySlice(cellinfo, 4);
		mem.put(mcc.getBytes());
		mem.put((byte)0);
		// Store MNC (max 8 bytes including terminator char).
		mem = getMemorySlice(cellinfo + 4, 8);
		mem.put(mnc.getBytes());
		mem.put((byte)0);

		// Put lac and cellid, these are two 4 byte ints.
		mem = getMemorySlice(cellinfo + 4 + 8, 8);
		mem.putInt(lac);
		mem.putInt(cellId);

		return 0;
	}

	// ********** Database API **********

	int maDBOpen(String path)
	{
		return mMoSyncDB.maDBOpen(path);
	}

	int maDBClose(int databaseHandle)
	{
		return mMoSyncDB.maDBClose(databaseHandle);
	}

	int maDBExecSQL(int databaseHandle, String sql)
	{
		return mMoSyncDB.maDBExecSQL(databaseHandle, sql);
	}

	int maDBExecSQLParams(
		int databaseHandle,
		String sql,
		int paramsAddress,
		int paramCount)
	{
		return mMoSyncDB.maDBExecSQLParams(
			databaseHandle,
			sql,
			paramsAddress,
			paramCount,
			this);
	}

	int maDBCursorDestroy(int cursorHandle)
	{
		return mMoSyncDB.maDBCursorDestroy(cursorHandle);
	}

	int maDBCursorNext(int cursorHandle)
	{
		return mMoSyncDB.maDBCursorNext(cursorHandle);
	}

	int maDBCursorGetColumnData(
		int cursorHandle,
		int columnIndex,
		int placeholder)
	{
		return mMoSyncDB.maDBCursorGetColumnData(
			cursorHandle,
			columnIndex,
			placeholder,
			this);
	}

	int maDBCursorGetColumnText(
		int cursorHandle,
		int columnIndex,
		int bufferAddress,
		int bufferSize)
	{
		return mMoSyncDB.maDBCursorGetColumnText(
			cursorHandle,
			columnIndex,
			bufferAddress,
			bufferSize,
			this);
	}

	int maDBCursorGetColumnInt(
		int cursorHandle,
		int columnIndex,
		int intValueAddress)
	{
		return mMoSyncDB.maDBCursorGetColumnInt(
			cursorHandle,
			columnIndex,
			intValueAddress,
			this);
	}

	int maDBCursorGetColumnDouble(
		int cursorHandle,
		int columnIndex,
		int doubleValueAddress)
	{
		return mMoSyncDB.maDBCursorGetColumnDouble(
			cursorHandle,
			columnIndex,
			doubleValueAddress,
			this);
	}

	/**
	 * Ends the application by calling the native exit() function
	 */
	public void exitApplication()
	{
		nativeExit();
	}

	/**
	 * Class that holds image data.
	 */
	public static final class ImageCache
	{
		public Bitmap mBitmap;
		public Canvas mCanvas;

		public ImageCache(Canvas canvas, Bitmap bitmap)
		{
			mCanvas = canvas;
			mBitmap = bitmap;
		}
	}

	/**
	 * Class that stores offset and size of an unloaded binary
	 * resource (.ubin). Ubins are memory saving resources that are
	 * loaded on demand. On Android we need to store audio resources
	 * as files to be able to play them, therefore there is some
	 * special handling of audio resources in the Android runtime.
	 * See file MoSyncSound.java.
	 */
	public static class UBinData
	{
		public UBinData(int offset, int size)
		{
			mOffset = offset;
			mSize = size;
		}

		/**
		 * Note that you need to subtract this offset by getResourceStartOffset()
		 * to get the actual start of the resource.
		 *
		 * The resources start after the asset file header, and this value is
		 * returned by getResourceStartOffset() in MoSyncThread.
		 *
		 * We need to subtract the offset because a FileInputStream for the
		 * resource file will begin at getResourceStartOffset and uBinData.mOffset
		 * is the offset from the actual beginning of the resource file.
		 *
		 * Sorry for this complicated explanation!
		 *
		 * @return offset of this ubin from the beginning of the
		 * asset resource file.
		 */
		public int getOffset()
		{
			return mOffset;
		}

		public int getSize()
		{
			return mSize;
		}

		/**
		 * The byte offset from the actual beginning of the asset
		 * resource file to the beginning of this ubin.
		 */
		private final int mOffset;

		/**
		 * The byte size of this ubin.
		 */
		private final int mSize;
	}

	/**
	 * This BroadcastReceiver listens to events ACTION_SCREEN_ON
	 * and ACTION_SCREEN_OFF and posts the corresponding events
	 * on the MoSync event queue.
	 */
	public static class ScreenActivatedReceiver extends BroadcastReceiver
	{
		private final MoSyncThread mMoSyncThread;

		public ScreenActivatedReceiver(MoSyncThread thread)
		{
			mMoSyncThread = thread;
		}

		@Override
		public void onReceive(Context context, Intent intent)
		{
			if (intent.getAction().equals(Intent.ACTION_SCREEN_ON))
			{
				// Send ON event.
				//Log.i("@@@MoSync", "@@@ Sending EVENT_TYPE_SCREEN_STATE_ON");
				int[] event = new int[1];
				event[0] = EVENT_TYPE_SCREEN_STATE_ON;
				mMoSyncThread.postEvent(event);
			}
			else if (intent.getAction().equals(Intent.ACTION_SCREEN_OFF))
			{
				// Send OFF event.
				//Log.i("@@@MoSync", "@@@ Sending EVENT_TYPE_SCREEN_STATE_OFF");
				int[] event = new int[1];
				event[0] = EVENT_TYPE_SCREEN_STATE_OFF;
				mMoSyncThread.postEvent(event);
			}
		}
	}

}
