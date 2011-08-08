package com.mosync.internal.android;


import static com.mosync.internal.android.MoSyncHelpers.SYSLOG;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.internal.generated.MAAPI_consts; 

import com.mosync.java.android.MoSync;
import com.mosync.nativeui.ui.widgets.MoSyncCameraPreview;

import android.app.Activity;
import android.hardware.Camera;
import android.hardware.Camera.PictureCallback;
import android.util.Log;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.Condition;

/**
 * 
 * A class that controls the behavior of camera and the camera preview
 * @author Ali Sarrafi
 *
 */
public class MoSyncCameraController {
	
	/**
	 * Thread Synchronization variables used to synchronize the 
	 * snapshot call and the call backs
	 * 
	 */
	private boolean dataReady;
	private ReentrantLock lock;
	private Condition condition;
	
	/**
	 * A private attribute used to keep track of 
	 * the data place holder in each snapshot
	 */
	private int resourceIndex;

	/**
	 * Arrays to store user assigned sizes
	 */
	List <Integer> userWidths;
	List <Integer> userHeights;

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
		userWidths = new ArrayList<Integer>();
		userHeights = new ArrayList<Integer>();
		
	}

	/**
	 * @return The Activity object.
	 */
	public Activity getActivity()
	{
		return mMoSyncThread.getActivity();
	}
	
	/**
	 * An equivalent to maCameraNumber syscall that
	 * queries the number of available cameras
	 * @return number of cameras on the device
	 */
	public static int numberOfCameras()
	{
		//Currently Anroid API Level 8 only supports one camera
		return 1;
	}

	/**
	 * A function to provide the information on supported picture
	 *  sizes to the user
	 * @return total number of supported sizes for the selected camera
	 */
	public int getNumberOfPictureSizes()
	{
		List <Camera.Size> sizeList;
		if(mPreview.mCamera == null)
		{
			mPreview.mCamera = Camera.open();
			Camera.Parameters parameters = mPreview.mCamera.getParameters();
			sizeList = parameters.getSupportedPictureSizes();
			mPreview.mCamera.release();
			mPreview.mCamera = null;
		}
		else
		{
			Camera.Parameters parameters = mPreview.mCamera.getParameters();
			sizeList = parameters.getSupportedPictureSizes();
		}
		return sizeList.size();
	}
	
	/**
	 * Adds a custom size to the list of sizes  requested by the user
	 * @param index the index used for addressing the specific size
	 * @param width width of the desired picture
	 * @param height height of the desired picutre
	 * @return RES_OK
	 */
	public int addSize(int index, int width, int height)
	{
		//TODO: move the optimal calculation here for better performance
		userWidths.add(index, new Integer(width));
		userHeights.add(index, new Integer(height));
		return MAAPI_consts.MA_CAMERA_RES_OK;
	}
	
	/**
	 * Sets the MoSyncCameraPreview object to be controlled
	 * @param preview an object of MoSyncCameraPreview that has already been initialized
	 */
	public void setPreview(MoSyncCameraPreview preview)
	{
		mPreview = preview;
	}
	
	/**
	 * Sets the active camera 
	 * @param CameraNumber index of the camera from the available cameras
	 * @return RES_OK
	 */
	public int setActiveCamera(int CameraNumber)
	{
		//android API Level 8 only support one camera
		return 1;
	}

	/**
	 * Checks to see if a view is setup already
	 * @return false if there is no view true if there is a view
	 */
	public boolean hasView()
	{
		if(mPreview == null)
			return false;
		else
			return true;
	}
	
	/**
	 * 
	 * @return a reference to the preview that is currently being used for the camera
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
			if(mPreview.mCamera == null)
			{
				mPreview.initiateCamera();
			}
			mPreview.mCamera.startPreview();			
		} 
		catch (Exception e) 
		{
			return MAAPI_consts.MA_CAMERA_RES_FAILED;
		}
		
		return MAAPI_consts.MA_CAMERA_RES_OK;
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
			return MAAPI_consts.MA_CAMERA_RES_FAILED;
		}
		
		return MAAPI_consts.MA_CAMERA_RES_OK;
	}
	
	
	/**
	 * Takes a picture using the camera, waits for the callbacks
	 * and sends the data to MoSync Program
	 */
	public int cameraSnapshot(int formatIndex, int placeHolder)
	{
		if(formatIndex >= 0)
		{
			setNewSize(formatIndex);
		}
		
		resourceIndex = placeHolder;
		mPreview.mCamera.takePicture(null, rawCallback, jpegCallback);
		
		lock.lock();
		  try 
		  {
			  while (dataReady == false) 
			  {
				  condition.await();
			  }
			  dataReady = false;   
			  return MAAPI_consts.MA_CAMERA_RES_OK;
		  } 
		  catch (InterruptedException e) 
		  {
			  return MAAPI_consts.MA_CAMERA_RES_FAILED;
		  } 
		  finally
		  {
			  lock.unlock();
		  }
	}
	
	public int setCameraProperty(String key, String value)
	{
		if(mPreview.mCamera == null)
			return -2;
		Camera.Parameters param = mPreview.mCamera.getParameters();
		param.set(key, value);
		try
		{
			mPreview.mCamera.setParameters(param);
		}
		catch (RuntimeException e)
		{
			return -3;
		}
		return 1;
	}
	
	public int getCameraPorperty(String key,
			int memBuffer, 
			int memBufferSize)
	{
		if(mPreview.mCamera == null)
			return -2;
		Camera.Parameters param = mPreview.mCamera.getParameters();
		String result = param.get(key);
		if(result == null)
			return -3;
		
		if( result.length( ) + 1 > memBufferSize )
		{
			Log.e( "MoSync", "maCameraGetProperty: Buffer size " + memBufferSize + 
					" too short to hold buffer of size: " + result.length( ) + 1 );
			return -4;
		}
		
		byte[] ba = result.getBytes();

		// Write string to MoSync memory.
		mMoSyncThread.mMemDataSection.position( memBuffer );
		mMoSyncThread.mMemDataSection.put( ba );
		mMoSyncThread.mMemDataSection.put( (byte)0 );
		
		return result.length( );
	}
	
	/**
	 * Releases the camera in case of pause or exit
	 */
	public void releaseCamera()
	{
		if(mPreview.mCamera != null)
		{
			mPreview.mCamera.release();
			mPreview.mCamera = null;
		}
	}
	
	/**
	 * a wrapper to set a new picture size when taking a snapshot 
	 */
	private void setNewSize(int formatIndex)
	{
		Camera.Parameters parameters = mPreview.mCamera.getParameters();
		List <Camera.Size> supportedSizes =  parameters.getSupportedPictureSizes();
		int width = userWidths.get(formatIndex).intValue(); 
		int height = userHeights.get(formatIndex).intValue();
		Camera.Size optimalPictureSize = mPreview.getOptimalSize(supportedSizes, width, height);
		parameters.setPictureSize(optimalPictureSize.width,optimalPictureSize.height);
    	try
    	{
    		mPreview.mCamera.setParameters(parameters);
    	}
    	catch (Exception e)
    	{
    		SYSLOG("FAILED TO SET the PARAMETERS");
    	}
	}
	

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
				mMoSyncThread.nativeCreateBinaryResource(resourceIndex, data.length);
				ByteBuffer byteBuffer = mMoSyncThread.mBinaryResources.get(resourceIndex);
				byteBuffer.put(data);
				dataReady = true;
				condition.signalAll();
			  } 
			  catch (Exception e) {
				  SYSLOG("Failed to create the data pool");
			  }
			  finally {
			    lock.unlock();
			  }
		}
	};	
}
