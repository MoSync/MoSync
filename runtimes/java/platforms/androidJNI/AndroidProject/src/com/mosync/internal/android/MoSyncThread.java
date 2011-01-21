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
import static com.mosync.internal.android.MoSyncHelpers.EXTENT_Y;
import static com.mosync.internal.android.MoSyncHelpers.SYSLOG;
import static com.mosync.internal.generated.MAAPI_consts.IOCTL_UNAVAILABLE;
import static com.mosync.internal.generated.MAAPI_consts.MAS_CREATE_IF_NECESSARY;
import static com.mosync.internal.generated.MAAPI_consts.NOTIFICATION_TYPE_APPLICATION_LAUNCHER;
import static com.mosync.internal.generated.MAAPI_consts.RES_BAD_INPUT;
import static com.mosync.internal.generated.MAAPI_consts.RES_OK;
import static com.mosync.internal.generated.MAAPI_consts.RES_OUT_OF_MEMORY;
import static com.mosync.internal.generated.MAAPI_consts.SCREEN_ORIENTATION_DYNAMIC;
import static com.mosync.internal.generated.MAAPI_consts.SCREEN_ORIENTATION_LANDSCAPE;
import static com.mosync.internal.generated.MAAPI_consts.SCREEN_ORIENTATION_PORTRAIT;
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
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.nio.channels.Channels;
import java.nio.channels.FileChannel;
import java.nio.channels.ReadableByteChannel;
import java.util.Date;
import java.util.Hashtable;
import java.util.Locale;
import java.util.TimeZone;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.PorterDuff.Mode;
import android.graphics.Rect;
import android.graphics.Region;
import android.net.Uri;
import android.opengl.GLUtils;
import android.os.Bundle;
import android.os.Handler;
import android.os.SystemClock;
import android.os.Vibrator;
import android.telephony.TelephonyManager;
import android.util.Log;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;

import com.mosync.java.android.MessageBox;
import com.mosync.java.android.MoSync;
import com.mosync.java.android.MoSyncPanicDialog;
import com.mosync.java.android.MoSyncService;
import com.mosync.java.android.TextBox;

/**
 * Thread that runs the MoSync virtual machine and handles all syscalls.
 * @author Anders Malm
 */
public class MoSyncThread extends Thread
{
	// When this class is loaded we load the native library.
	static 
	{ 
		System.loadLibrary("mosync"); 
	}
	
	// Declaration of native methods.
	public native boolean nativeInitRuntime();
	public native boolean nativeLoad(
		FileDescriptor program, 
		long programOffset, 
		FileDescriptor resource, 
		long resourceOffset);
	public native boolean nativeLoadResource(ByteBuffer resource);
	public native ByteBuffer nativeLoadCombined(ByteBuffer combined);
	public native void nativeRun();	
	public native void nativePostEvent(int[] eventBuffer);

	// Modules that handle syscalls for various subsystems.
	// We delegate syscalls from this class to the modules.
	MoSyncNetwork mMoSyncNetwork;
	MoSyncBluetooth mMoSyncBluetooth;
	MoSyncSound mMoSyncSound;
	MoSyncLocation mMoSyncLocation;
	MoSyncHomeScreen mMoSyncHomeScreen;
	MoSyncNativeUI mMoSyncNativeUI;

	static final String PROGRAM_FILE = "program.mp3";
	static final String RESOURCE_FILE = "resources.mp3";
	
	/**
	 * Chunk size used when loading resources.
	 */
	private static final int CHUNK_READ_SIZE = 1024; 

	private MoSync mContext;
	private MoSyncView mMoSyncView;
	private boolean mHasDied;
	
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
	public ByteBuffer mMemDataSection;
	ByteBuffer mResourceFile;

	Canvas mCanvas;
	Bitmap mBitmap;

	int mWidth;
	int mHeight;

	int mClipLeft, mClipTop, mClipWidth, mClipHeight;

	boolean mUsingFrameBuffer;
	int mFrameBufferAddress;
	int mFrameBufferSize;
	Bitmap mFrameBufferBitmap;

	ImageCache mDrawTargetScreen = null;
	ImageCache mDrawTarget = null;
	int mDrawTargetHandle = 0;

	Paint mPaint = new Paint();
	Paint mBlitPaint = new Paint();

	int mTextConsoleHeight;
	
	Rect mTextSizeRect = new Rect();

	// Rectangle objects used for drawing in maDrawImageRegion().
	private Rect mMaDrawImageRegionTempSourceRect = new Rect();
	private Rect mMaDrawImageRegionTempDestRect = new Rect();

	ByteBuffer mTempImageRawBuffer;
	
	int mMaxStoreId = 0;

	public boolean mIsUpdatingScreen = false;
	
	final static String storesPath = "MAStore";

	/**
	 * MoSyncThread constructor
	 */
	public MoSyncThread(Context context, Handler handler) throws Exception
	{	
		mContext = (MoSync) context;
		
		EventQueue.sMoSyncThread = this;
		
		mHasDied = false;
		
		mMoSyncNetwork = new MoSyncNetwork(this);
		mMoSyncSound = new MoSyncSound(this);
		mMoSyncLocation = new MoSyncLocation(this);
		mMoSyncHomeScreen = new MoSyncHomeScreen(this);
		mMoSyncNativeUI = new MoSyncNativeUI(this, mImageResources);
		
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
		
		nativeInitRuntime();	
	}
	
	/**
	 * Return the activity that this thread is related to.
	 * @return the Activity object.
	 */
	public Activity getActivity()
	{
		return (Activity) mContext;
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
	 * setMoSyncView
	 */
	public void setMoSyncView(MoSyncView moSyncView)
	{
		mMoSyncView = moSyncView;
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
	public void updateSurfaceSize(int width, int height)
	{
		SYSLOG("updateSurfaceSize");		
		
		// If the surface has same width and height we 
		// do not create a new canvas.
		if((mWidth == width) && (mHeight == height)) return;
		
		mWidth = width;
		mHeight = height;
		
		Bitmap bitmap = Bitmap.createBitmap(
			mWidth, mHeight, Bitmap.Config.ARGB_8888);
		Canvas canvas = new Canvas(bitmap);
		mDrawTargetScreen = new ImageCache(canvas, bitmap);
		
		mCanvas = mDrawTargetScreen.mCanvas;
		mBitmap = mDrawTargetScreen.mBitmap;
		
		updateScreen();
		
		SYSLOG("mBitmap width:" + mBitmap.getWidth() 
			+ " height:" + mBitmap.getHeight());
	}

	/**
	 * Allocate memory for the program data section.
	 */
	public ByteBuffer generateDataSection(int size)
	{
		try
		{
			mMemDataSection = ByteBuffer.allocateDirect(size);
			mMemDataSection.order(null);
		}
		catch (Exception e)
		{
			logError("MoSyncThread - Out of Memory!", e);
			mMemDataSection = null;
			return null;
		}
		catch (Error e)
		{
			logError("MoSyncThread - Out of Memory!", e);
			mMemDataSection = null;
			return null;
		}
		return mMemDataSection;
	}
	
	/**
	 * Issue a panic. Launches a panic dialog.
	 * @param errorCode
	 * @param message
	 */
	public void threadPanic(int errorCode, String message)
	{
		SYSLOG("PANIC - errorCode: " + errorCode + " message: " + message);
		
		mHasDied = true;
		
		try
		{
			// Launch panic dialog.
			MoSyncPanicDialog.sPanicMessage = message;
			Intent myIntent = new Intent(
				mMoSyncView.getContext(), MoSyncPanicDialog.class);
			mMoSyncView.getContext().startActivity(myIntent);
			
			// Sleep so that the MoSync thread is kept alive while 
			// the dialog is open.
			while (true)
			{
				try
				{
					sleep(Long.MAX_VALUE);
				}
				catch (Exception e)
				{
					logError("threadPanic exception 1:" + e, e);
				}
			}
		}
		catch (Exception e)
		{
			logError("threadPanic exception 2:" + e, e);
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
			AssetFileDescriptor pAfd = assetManager.openFd(PROGRAM_FILE);
			FileDescriptor pFd = pAfd.getFileDescriptor();
			long pFdOffset = pAfd.getStartOffset();

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
				logError("loadProgram - Has no resources! exception: " 
					+ fnfe.toString(), fnfe);
			}

			// We have a program file so now we sends it to the native side
			// so it will be loaded into memory. The data section will also be
			// created and if there are any resources they will be loaded.
			if (null != pFd)
			{
				if (false == nativeLoad(pFd, pFdOffset, rFd, mResourceOffset))
				{
					logError("loadProgram - "
						+ "ERROR Load program was unsuccesfull");

					if (null == mMemDataSection)
					{
						threadPanic(
							0, 
							"This device does not have enough" +
							"memory to run this application."
						);
					}
					else
					{
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
	public ByteBuffer readInputStream(InputStream is) throws Exception
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
	
	/**
	 * Called from JNI, delegate to the audio module.
	 * @param soundHandle
	 */
	void storeIfBinaryAudioResource(int soundHandle)
	{
		mMoSyncSound.storeIfBinaryAudioResource(soundHandle);
	}

	/**
	 * Redraws the screen.
	 */
	public void updateScreen()
	{
		maUpdateScreen();
	}
	
	/**
	 * Post a event to the MoSync event queue.
	 */
	public void postEvent(int[] event)
	{
		// Add event to queue.
		nativePostEvent(event);
		
		// Wake up this thread to make it process events.
		interrupt(); 
	}

	/**
	 * Main entry point for the thread. This is where
	 * the main loop is called.
	 */
	@Override 
	public void run()
	{
		Log.i("MoSync Thread", "run");

		if (false == loadProgram())
		{
			logError("load program failed!!");
			return;
		}
	
		nativeRun();
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
		UBinData uBinData = (UBinData) mUBinaryResources.get(handle);
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
	
	boolean initSyscalls()
	{
		SYSLOG("initSyscalls");
		mUsingFrameBuffer = false;

		mClipLeft = 0;
		mClipTop = 0;
		mClipWidth = mWidth;
		mClipHeight = mHeight;

		mPaint.setStyle(Paint.Style.FILL);
		mPaint.setAntiAlias(false);
		mPaint.setColor(0xffffffff);
		 
		mBlitPaint.setAntiAlias(false);
		mBlitPaint.setColor(0xffffffff);
		mBlitPaint.setAlpha(255);
		 
		// Generates a default text height used for console text writing.
		// This is used so that all text which is printed to the console
		// gets the same text height.
		int extent = maGetTextSize(
			"abcdefghijklmnopqrstuvwxyz" +
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
		mTextConsoleHeight = EXTENT_Y(extent);
		 
		 return true;
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

		mMemDataSection.mark();
		mMemDataSection.position(address);

		IntBuffer ib = mMemDataSection.asIntBuffer();
		
		int[] vertices = new int[count*2];
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
		
		mMemDataSection.reset();
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

		mMemDataSection.mark();
		mMemDataSection.position(address);

		IntBuffer ib = mMemDataSection.asIntBuffer();
		
		int[] vertices = new int[count*2];
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
			path.moveTo(xa,ya);
			path.lineTo(xc,yc);
			path.lineTo(xb,yb);
			path.close();
			
			if(i >= count*2)
				break;			
		}
		mCanvas.drawPath(path, mPaint);
		
		mMemDataSection.reset();
	}

	/**
	 * Gets the height in pixels of the string.
	 *
	 * Calculates the height in pixels of this string, as it's gonna 
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
		
		return EXTENT(mTextSizeRect.width(), mTextSizeRect.height());
	}

	/**
	 * Gets the height in pixels of the string.
	 *
	 * Calculates the height in pixels of this string, as it's gonna 
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
		
		return EXTENT(mTextSizeRect.width(), mTextSizeRect.height());
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
		
		mCanvas.drawText(str, left, top+mTextConsoleHeight, mPaint);
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
		
		mCanvas.drawText(str, left, top+mTextConsoleHeight, mPaint);
	}

	/**
	 * maUpdateScreen
	 * Sets the boolean mIsUpdatingScreen when updating the screen
	 * so that we won't get touch events while drawing. This to
	 * not hog the system with events.
	 */
	void maUpdateScreen()
	{
		//SYSLOG("maUpdateScreen");
		Canvas lockedCanvas = null;
		
		if (mMoSyncView == null) return;
		
		mIsUpdatingScreen = true;
		
		try 
		{
			lockedCanvas = mMoSyncView.mSurfaceHolder.lockCanvas();
			synchronized (mMoSyncView.mSurfaceHolder)
			{
				if (mUsingFrameBuffer)
				{				
					mMemDataSection.mark();
					mMemDataSection.position(mFrameBufferAddress);
					mFrameBufferBitmap.copyPixelsFromBuffer(mMemDataSection);
					mMemDataSection.reset();
					
					// Clear the screen.. in this case draw the canvas black
					lockedCanvas.drawRGB(0,0,0);
					
					// Blit the framebuffer
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

	/**
	 * maResetBacklight
	 */
	void maResetBacklight()
	{
		SYSLOG("maResetBacklight");
	}

	/**
	 * maGetScrSize
	 */
	int maGetScrSize()
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
		
		ImageCache imageResource = (ImageCache)
			mImageResources.get(new Integer(image));
		if (imageResource == null)
		{
			return;
		}

		mCanvas.drawBitmap(imageResource.mBitmap, left, top, null);
	}

	/**
	 * _maDrawRGB
	 */
	void _maDrawRGB(
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
		
		mMemDataSection.mark();
		mMemDataSection.position(mem);

		IntBuffer ib = mMemDataSection.asIntBuffer();
		
		for(int y = 0; y < srcRectHeight; y++)
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
		
		mMemDataSection.reset();
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
	void _maDrawImageRegion(
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
		ImageCache imageResource = (ImageCache) mImageResources.get(image);
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
		ImageCache imageResource = (ImageCache) 
			mImageResources.get(new Integer(image));
		if (imageResource == null)
		{
			logError("maGetImageSize : no such resource");
			return -1;
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

		ImageCache imageResource = (ImageCache)mImageResources.get(image);
		if(null == imageResource)
		{
			maPanic(1,"maGetImageData failed, no image resource!");
		}
		
		// TODO: Remove variable bitmapSize, it is not used.
		//int bitmapSize = scanLength * srcHeight;
		
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
		Bitmap temporaryBitmap = Bitmap.createBitmap(
			srcWidth,
			srcHeight,
			Bitmap.Config.ARGB_8888);

		Canvas temporaryCanvas = new Canvas(temporaryBitmap);
		
		temporaryCanvas.drawBitmap(
			imageResource.mBitmap,
			new Matrix(),
			new Paint());
			
		temporaryCanvas.drawColor(0xff000000, Mode.DST_ATOP);
		
		mMemDataSection.mark();
		mMemDataSection.position(dst);

		IntBuffer intBuffer = mMemDataSection.asIntBuffer();
		
		for(int y = 0; y < srcHeight; y++)
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
				srcLeft,
				srcTop+y,
				srcWidth,
				1);
			
			for( int i = 0; i < srcWidth; i++)
			{
				pixels[i] = (alpha[i]&0xff000000) + (colors[i]&0x00ffffff);
			}
			
			intBuffer.put(pixels);	
		}
		
		mMemDataSection.reset();
	}

	/**
	 * Set the target image for drawing.
	 * @param image The target image, 0 means the screen.
	 */
	int maSetDrawTarget(int image)
	{
		SYSLOG("maSetDrawTarget");
		if (0 == image)
		{
			mCanvas = mDrawTargetScreen.mCanvas;
			mBitmap = mDrawTargetScreen.mBitmap;
		}
		else
		{
			ImageCache imageResource = (ImageCache)
				mImageResources.get(new Integer(image));
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
		ByteBuffer binData = (ByteBuffer) mBinaryResources.get(dataHandle);
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
			UBinData uBinData = (UBinData) mUBinaryResources.get(dataHandle);
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
			
			Bitmap decodedImage = BitmapFactory.decodeByteArray(
				resourceData, 0, resourceData.length, options);
			if (decodedImage == null)
			{
				logError("maCreateImageFromData - " 
					+ "could not decode image data (decodedImage == null)");
				return RES_BAD_INPUT;
			}
			mImageResources.put(
				placeholder, new ImageCache(null, decodedImage));
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
	 * This function generates a ByteBuffer which then is sent to 
	 * the JNI library for processing.
	 */
	ByteBuffer _maCreateImageRawGetData(int size)
	{
		mTempImageRawBuffer = ByteBuffer.allocateDirect(size);
		mTempImageRawBuffer.order(null);
		
		return mTempImageRawBuffer;
	}

	/**
	 * Takes the preprocessed raw image data and copies the 
	 * contents to a bitmap.
	 */
	int _maCreateImageRaw(int placeholder, int width, int height)
	{
		SYSLOG("maCreateImageRaw");
		
		Bitmap bitmap = Bitmap.createBitmap(
			width, height, Bitmap.Config.ARGB_8888);
		if(null == bitmap)
		{
			maPanic(1, "Unable to create ");
		}
		
		mTempImageRawBuffer.position(0);
		bitmap.copyPixelsFromBuffer(mTempImageRawBuffer);
		
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
			Bitmap bitmap = Bitmap.createBitmap(
				width, height, Bitmap.Config.ARGB_8888);
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
	int maOpenStore(String name, int flags)
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
			mStores.put(new Integer(mMaxStoreId), filePath);
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
	int maWriteStore(int store, int data)
	{
		SYSLOG("maWriteStore");
		try 
		{
			File f = mContext.getFileStreamPath(
				(String) mStores.get(new Integer(store)));
			if (f.isFile())
			{
				ByteBuffer bb = (ByteBuffer)mBinaryResources.get(data);
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
				
				UBinData ubd = (UBinData) mUBinaryResources.get(data);
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
	 */
	ByteBuffer _maReadStore(int store, int resourceIndex)
	{
		SYSLOG("_maReadStore");
		
		try 
		{
			File f = mContext.getFileStreamPath(
				(String) mStores.get(new Integer(store)));
			byte[] buffer;
			int length = 0;
			if (f.isFile())
			{
				length = (int)f.length();
				buffer = new byte[length];
				
				FileInputStream fis = new FileInputStream(f);
				int offset = 0;
				while (offset >= 0 && offset < length)
				{
					offset = fis.read(buffer, offset, (length-offset));
				}
				fis.close();	
			}
			else
			{
				buffer = new byte[0];
			}
			
			ByteBuffer bb = loadBinary(resourceIndex, length);
			bb.put(buffer);
			return bb;
			
		} 
		catch(Exception e) 
		{
			logError("read store exception : " + e.toString(), e);
			return null;
		}
	}

	/**
	 * maCloseStore
	 */
	void maCloseStore(int store, int remove)
	{
		SYSLOG("maCloseStore");
		
		String filePath = (String) mStores.get(new Integer(store));
		if (remove != 0) 
		{
			File f = mContext.getFileStreamPath(filePath);
			f.delete();
		} 
		
		if (!(mStores.remove(new Integer(store)) == filePath))
		{
			maPanic(1, "maCloseStore failed!");
		}
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
	 * maWait
	 */
	void maWait(int timeout)
	{
		SYSLOG("maWait");	

		try
		{
	 		if (timeout<=0)
			{
				Thread.sleep(Long.MAX_VALUE);
			}
			else
			{
				Thread.sleep(timeout);
			}
		} 
		catch (InterruptedException ie) 
		{
			SYSLOG("Sleeping thread interrupted!");
		} 
		catch (Exception e) 
		{
			logError("Thread sleep failed : " + e.toString(), e);
		}
		
		SYSLOG("maWait returned");
	}

	/**
	 * maTime
	 */
	int maTime()
	{
		SYSLOG("maTime");
		Date d = new Date();
		return (int)(d.getTime() / 1000);
	}

	/**
	 * maLocalTime
	 */
	int maLocalTime()
	{
		SYSLOG("maLocalTime");
		Date d = new Date();
		TimeZone tz = TimeZone.getDefault();
		return (int)((d.getTime() + tz.getRawOffset()) / 1000);
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
	void maPanic(int result, String message)
	{
		SYSLOG("maPanic");
		
		threadPanic(result, message);
	}

	/**
	 * maInvokeExtension
	 */
	int maInvokeExtension(int function, int a, int b, int c)
	{
		SYSLOG("maInvokeExtension");
		return -1;
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
		mFrameBufferBitmap = Bitmap.createBitmap(
			mWidth, mHeight, Bitmap.Config.ARGB_8888);
	}

	/**
	 * _disableFramebuffer
	 */
	void _disableFramebuffer()
	{
		mUsingFrameBuffer = false;
	}

	/**
	 * Implemation of the maWriteLog syscall which only 
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
	 * maGetSystemProperty
	 */
	int maGetSystemProperty(String key, int buf, int size)
	{
		String property = "";
		
		if(key.equals("mosync.imei"))
		{
			TelephonyManager manager = (TelephonyManager)
				mContext.getSystemService(Context.TELEPHONY_SERVICE);
			property = manager.getDeviceId();
		}
		else if(key.equals("mosync.imsi"))
		{
			TelephonyManager manager = (TelephonyManager)
				mContext.getSystemService(Context.TELEPHONY_SERVICE);
			property = manager.getSubscriberId();
		}
		else if(key.equals("mosync.iso-639-1"))
		{
			Locale locale = Locale.getDefault();
			property = locale.getLanguage();
		}
		else if(key.equals("mosync.iso-639-2"))
		{
			Locale locale = Locale.getDefault();
			property = locale.getISO3Language();
		}
		
		if (null == property) { return -2; }
		
		if (0 == property.compareTo("")) { return -2; }
		
		if (property.length() >= size)
		{
			return property.length() + 1;
		}
			
		// Write this property to memory.
		
		byte[] ba = property.getBytes();
		
		mMemDataSection.mark();
		mMemDataSection.position(buf);
		mMemDataSection.put(ba);
		mMemDataSection.put((byte)0);
		mMemDataSection.reset();
		
		return property.length() + 1;
	}

	/**
	 * Perform a platform request.
	 * @param url The url that specifies the request.
	 * @return
	 */
	int maPlatformRequest(String url)
	{
		if (url.startsWith("http://"))
		{
			Log.i("maPlatformRequest","Starting browser:" + url);
			Intent intent = new Intent(Intent.ACTION_VIEW);
			intent.setData(Uri.parse(url));
			((Activity)mContext).startActivity(intent);
			
			return 0;
		}
		return -1;
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
				ByteBuffer bb = (ByteBuffer)mBinaryResources.get(binaryResource);
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
			Bitmap bitmap = BitmapFactory.decodeByteArray(ra, 0, length);
			if(bitmap != null)
			{
				SYSLOG("Bitmap was created!");
				mImageResources.put(
					resourceIndex, new ImageCache(null, bitmap));
				return true;
			}
			logError("loadImage - Bitmap wasn't created from Resource: " 
				+ resourceIndex);
			return false;
		}
		catch (Exception e)
		{
			logError("loadImage - Bitmap exceptions : " + e.toString(), e);
			return false;
		}
	}

	/**
	 * Load and store a binary resource. Since Android differs a lot from
	 * the other runtimes this is necessary. There isn't a duplicate stored
	 * on the JNI side.
	 */
	ByteBuffer loadBinary(int resourceIndex, int size)
	{
		SYSLOG("loadBinary index:" + resourceIndex);
		
		try
		{
			ByteBuffer data = ByteBuffer.allocateDirect(size);
			data.order(null);
			mBinaryResources.put(resourceIndex, data);
			return data;
		}
		catch(Throwable e)
		{
			logError("loadBinary - Out of Memory!", e);
			return null;
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

	/**
	 * Destroy a resource.
	 * 
	 * @param resourceIndex the resource index which should be destroyed.
	 */
	void destroyResource(int resourceIndex)
	{
		SYSLOG("destroyResource :" + resourceIndex);
		
		if(null != mBinaryResources.get(resourceIndex))
		{	
			mBinaryResources.remove(resourceIndex);
		}
		else if(null != mUBinaryResources.get(resourceIndex))
		{
			mUBinaryResources.remove(resourceIndex);
		}
		else if(null != mImageResources.get(resourceIndex))
		{
			mImageResources.remove(resourceIndex);
		}
		
		Log.i("---MoSyncThread---", "binary: " + mBinaryResources.size( ) + " ubinary: " + mUBinaryResources.size( ) + " image: " + mImageResources.size( ) );
		
		Log.i("MoSyncThread", "Resource deleted, force GC");
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
		bundle.putInt("CONSTRAINTS", constraints);
		intent.putExtras(bundle);
		mContext.startActivity(intent);
		Log.i("MoSync", "New activity started for InputBox");
		return 0;
	}

	/**
	 * Opens a message box.
	 * @param title
	 * @param text
	 * @return
	 */
	int maMessageBox(
		String title, 
		String text)
	{
		Intent intent = new Intent(mContext, MessageBox.class);
		Bundle bundle = new Bundle();
		bundle.putString("TITLE", title);
		bundle.putString("TEXT", text);
		intent.putExtras(bundle);
		mContext.startActivity(intent);
		Log.i("MoSync", "New activity started for MessageBox");
		return 0;
	}
	
	/**
	 * Display a notification.
	 * @param type
	 * @param id
	 * @param title
	 * @param text
	 * @return
	 */
	int maNotificationAdd(int type, int id, String title, String text)
	{
		Log.i("MoSync", "maNotificationAdd");
		
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
	 * @param notificationId
	 * @return
	 */
	int maNotificationRemove(int notificationId)
	{
		Log.i("MoSync", "maNotificationRemove");
		
		// Stop the service if this is the service notification id.
		if (MoSyncService.sNotificationId == notificationId)
		{
			// Remove the service notification.
			MoSyncService.removeServceNotification(
				notificationId, (Activity) mContext);
			
			// Stop the service.
			MoSyncService.stopService();
			
			return 0;
		}
		
		// TODO: Implement case for regular notifications.
		
		return -1;
	}

	/**
	 * Send the application to background.
	 * @return
	 */
	int maSendToBackground()
	{
		Log.i("MoSync", "maSendToBackground");
		
		final Activity activity = (Activity) mContext;
		
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

	int maScreenSetOrientation(int orientation)
	{
		Log.i("MoSync", "maScreenSetOrientation orientation: " 
			+ orientation);

		if (SCREEN_ORIENTATION_LANDSCAPE == orientation)
		{
			maScreenSetOrientationHelper(
				ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		}
		else if (SCREEN_ORIENTATION_PORTRAIT == orientation)
		{
			maScreenSetOrientationHelper(
				ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
		}
		else if (SCREEN_ORIENTATION_DYNAMIC == orientation)
		{
			maScreenSetOrientationHelper(
				ActivityInfo.SCREEN_ORIENTATION_SENSOR);
		}
		else
		{
			return -1;
		}
		
		return 0;
	}

	void maScreenSetOrientationHelper(final int androidScreenOrientation)
	{
		final Activity activity = (Activity) mContext;
		
	    activity.runOnUiThread(new Runnable()
		{
			public void run()
			{
				activity.setRequestedOrientation(androidScreenOrientation);
			}
		});
	}

	int maScreenSetFullscreen(final int fullscreen)
	{
		Log.i("MoSync", "maScreenSetFullscreen");
		
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
		final Activity activity = (Activity) mContext;
		
	    activity.runOnUiThread(new Runnable()
		{
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
		int connHandle, String key, long address, int bufSize)
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
	 * Set the background image of the phone's home screen.
	 * @param data Handle to an image object (PNG of JPEG)
	 */
	int maWallpaperSet(int handle)
	{
		return mMoSyncHomeScreen.maWallpaperSet(handle);
	}

	/**
	 * Add a shortcut icon to the home screen. If called multiple times
	 * multiple icons will be added. The shortcut launches the current
	 * application.
	 * @param name The text that will be used for the shortcut label.
	 * @return <0 on error
	 */
	int maHomeScreenShortcutAdd(String name)
	{
		return mMoSyncHomeScreen.maHomeScreenShortcutAdd(name);
	}

	/**
	 * Remove a shortcut icon to the home screen. 
	 * @param name The shortcut(s) with this label will be removed.
	 * @return <0 on error
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
	 * Internal wrapper for maWidgetRemoveChild that runs
	 * the call in the UI thread.
	 */
	public int maWidgetRemoveChild(
		final int parentHandle, 
		final int childHandle)
	{
		return mMoSyncNativeUI.maWidgetRemoveChild(parentHandle, childHandle);
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
	
	public int maWidgetGetProperty(
			final int widgetHandle,
			final String key,
			final int memBuffer, 
			final int memBufferSize)
	{
		return mMoSyncNativeUI.maWidgetGetProperty(widgetHandle, key, memBuffer, memBufferSize);
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
		
		// The texture bitmap encoding.
		int textureFormat = GL10.GL_RGBA;

		try
		{
			// Load the texture into OpenGL.
			if (isSubTexture)
			{
				GLUtils.texSubImage2D(GL10.GL_TEXTURE_2D,
				  0,
				  0,
				  0,
				  bitmap,
				  textureFormat,
				  GL10.GL_UNSIGNED_BYTE);
			}
			else
			{
				GLUtils.texImage2D(
					GL10.GL_TEXTURE_2D, 
					0, 
					textureFormat, 
					bitmap, 
					0);
			}
		}
		catch (Throwable t)
		{
			Log.i("MoSyncThread", "Exception in loadGlTexture: " + t.toString());
			t.printStackTrace();
		}
		finally
		{
			if (bitmap != texture.mBitmap)
			{
				bitmap.recycle();
			}
		}
		
		// Check error status.
		EGL10 egl = (EGL10) EGLContext.getEGL( );
		if(egl.eglGetError( ) == EGL10.EGL_SUCCESS)
		{
			return 0; // Success, no errors.
		}
		else
		{
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
		
		Bitmap newBitmap = Bitmap.createBitmap(
			newWidth, 
			newHeight, 
			Bitmap.Config.ARGB_8888);
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
	
	/**
	 * Class that holds image data.
	 */
	public static final class ImageCache
	{
		public Bitmap mBitmap;
		public Canvas mCanvas;

		ImageCache(Canvas canvas, Bitmap bitmap)
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
		private int mOffset;
		
		/**
		 * The byte size of this ubin.
		 */
		private int mSize;
	}
}
