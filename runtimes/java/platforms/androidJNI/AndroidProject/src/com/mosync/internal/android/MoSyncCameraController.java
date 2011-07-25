package com.mosync.internal.android;


import static com.mosync.internal.android.MoSyncHelpers.DebugIsOn;
import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;
import static com.mosync.internal.android.MoSyncHelpers.MYASSERT;
import static com.mosync.internal.android.MoSyncHelpers.SYSLOG;
import static com.mosync.internal.generated.MAAPI_consts.*;

import com.mosync.nativeui.ui.widgets.MoSyncCameraPreview;
import com.mosync.nativeui.util.AsyncWait;

import android.R.bool;
import android.app.Activity;
import android.hardware.Camera;
import android.hardware.Camera.PictureCallback;
import android.hardware.Camera.ShutterCallback;
import android.os.Bundle;
import android.util.Log;
import android.widget.FrameLayout;

import java.io.IOException;
import java.io.InterruptedIOException;
import java.lang.reflect.Method;
import java.nio.ByteBuffer;
import java.util.Hashtable;
import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.Condition;

/**
 * 
 * A class that controlls the behavior of camera nad the camerapreview
 * @author Ali Sarrafi
 *
 */
public class MoSyncCameraController {
	
	/**
	 * Error Definitions
	 */
	static final int CAMERA_SUCCESS = 1;
	static final int CAMERA_START_FAILED = -2;
	static final int CAMERA_STOP_FAILED = -3;
	static final int CAMERA_SNAPSHOT_FAILED = -4;
	
	/**
	 * Thread Synchronization variables used to synchronize the 
	 * snapshot call and the call backs
	 * 
	 */
	private boolean dataReady;
	private ReentrantLock lock;
	private Condition condition;
	private int resourceIndex;
	
	

	/**
	 * The MoSync Camera Preview object that handles the surfaceview 
	 * created by either the NativeUIWidget or the full screen call. 
	 */
	private MoSyncCameraPreview mPreview;
	
	/**
	 * The MoSync thread object.
	 */
	public MoSyncThread mMoSyncThread;
	
	
	/**
	 * Constructor.
	 * @param thread The MoSync thread.
	 */
	public MoSyncCameraController(MoSyncThread thread)
	{
		mMoSyncThread = thread;
		lock = new ReentrantLock();
		mPreview = null;
		condition = lock.newCondition();
		dataReady = false;
	}

	/**
	 * @return The Activity object.
	 */
	public Activity getActivity()
	{
		return mMoSyncThread.getActivity();
	}

	
	/**
	 * Sets the MoSyncCameraPreview object ot be controlled
	 * @param preview an object of MoSyncCameraPreview that has already been initialized
	 */
	public void setPreview(MoSyncCameraPreview preview)
	{
		mPreview = preview;
	}
	
	/**
	 * 
	 * @return a refrence to the preview that is currently being used for the camera
	 */
	public MoSyncCameraPreview getPreview()
	{
		return mPreview;
	}
	
	/**
	 * Starts the preview
	 */
	public int cameraStart()
	{
		try
		{
			mPreview.mCamera.startPreview();
			
		} 
		catch (Exception e) 
		{
			return CAMERA_START_FAILED;
		}
		
		return CAMERA_SUCCESS;
	}
	
	/**
	 * Called to stop the preview on the camera
	 */
	public int cameraStop()
	{
		try
		{
			mPreview.mCamera.stopPreview();
			
		} 
		catch (Exception e) 
		{
			return CAMERA_STOP_FAILED;
		}
		
		return CAMERA_SUCCESS;
	}
	
	
	/**
	 * Takes a picture using the camera, waits for the callbacks
	 * and sends the data to MoSync Program
	 */
	public int cameraSnapshot(int formatIndex, int placeHolder)
	{
		resourceIndex = placeHolder;
		mPreview.mCamera.takePicture(shutterCallback, rawCallback, jpegCallback);
		lock.lock();
		  try {
			    while (dataReady == false) {
			      condition.await();
			    }
			     dataReady = false;
			     return 0;
			  } catch (InterruptedException e) {
			    return CAMERA_SNAPSHOT_FAILED;
			} finally {
			    lock.unlock();
			  }
	}
	
	
	/**
	 * Called when the shutter is openned
	 */
	ShutterCallback shutterCallback = new ShutterCallback() {
		public void onShutter() {
			//Not Used in MoSync
		}
	};
	

	/** 
	 * Handles data for raw picture 
	 */
	PictureCallback rawCallback = new PictureCallback() {
		public void onPictureTaken(byte[] data, Camera camera) {
			//TODO: add format support
		}
	};

	/** 
	 * Handles data for jpeg picture
	 */
	PictureCallback jpegCallback = new PictureCallback() {
		public void onPictureTaken(byte[] data, Camera camera) {
			  lock.lock();
			  try {
				int result = mMoSyncThread.nativeCreateBinaryResource(resourceIndex, data.length);
				ByteBuffer byteBuffer = mMoSyncThread.mBinaryResources.get(resourceIndex);
				byteBuffer.put(data);
				dataReady = true;
				condition.signalAll();
			  } finally {
			    lock.unlock();
			  }
		}
	};	
}
