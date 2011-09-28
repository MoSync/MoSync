package com.mosync.internal.android;


import static com.mosync.internal.android.MoSyncHelpers.SYSLOG;

import java.lang.reflect.Method;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

import android.app.Activity;
import android.content.res.Configuration;
import android.hardware.Camera;
import android.hardware.Camera.PictureCallback;

import android.util.Log;

import com.mosync.internal.generated.MAAPI_consts;
import com.mosync.nativeui.ui.widgets.MoSyncCameraPreview;

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
	private final ReentrantLock lock;
	private final Condition condition;

	private Camera mCamera;



	/**
	 * Static Attributes used for reflection of some functions in the API
	 */
	private static Method mGetNumberofCameras;
	private static Camera tempCamera;

	/**
	* Stores number of availabe cameras on the device
	*/
	private int mNumCameras;

	/**
	* Indicates the current camera index
	*/
	private int currentCameraIndex;

	/**
	* Stores the paramters for each camera
	*/
	List<Camera.Parameters> mCameraParametersList;

	/**
	 * A flag that indicates the format of the image to be taken
	 */
	private boolean rawMode;
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
		mCameraParametersList = new ArrayList<Camera.Parameters>();
		mNumCameras = numberOfCameras();
		initilizeCameras();
		rawMode = false;

	}

	/**
	* Lazy initialization of the cameras so we can use the number of cameras
	*/
	private void initilizeCameras()
	{
		try
		{
			currentCameraIndex = 0;
			if(mNumCameras <= 1)
			{
				mCamera = Camera.open();
				mCameraParametersList.add(mCamera.getParameters());
			}
			else
			{
				if(mCamera != null)
					mCamera.release();
				for(int ii=0; ii < mNumCameras; ii++)
				{

					mCamera = Camera.open(ii);
					mCameraParametersList.add(mCamera.getParameters());
					mCamera.release();
				}
				mCamera = Camera.open(currentCameraIndex);
			}
		}
		catch(Exception e)
		{
			SYSLOG("Failed to assign Cameras");
		}

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
	public int numberOfCameras()
	{
		if(mNumCameras != 0)
		{
			//Do not do the costly operation of reflection again
			return mNumCameras;
		}
		 try
		 {
			 if(mCamera == null)
			 {
				 tempCamera = Camera.open();
			 }
			 else
			 {
				 mCamera.release();
				 tempCamera = Camera.open(currentCameraIndex);
			 }

			 //We have to use and static instance of the camera in the reflection here
			 mGetNumberofCameras = tempCamera.getClass().getMethod(
					 "getNumberOfCameras");
			 tempCamera.release();
			 if(mCamera != null)
			 {
				 mCamera = Camera.open(currentCameraIndex);
			 }
			 return Camera.getNumberOfCameras();

		 }
		 catch (NoSuchMethodException nsme)
		 {
			 tempCamera.release();
			 SYSLOG("ANDROID Version is less than 2.3!!");
			 //before 2.3 only one camera is supported
			 return 1;
		 } catch (RuntimeException e) {
			 SYSLOG("Failed to set camera Parameters");
			 return MAAPI_consts.MA_CAMERA_RES_FAILED;
		 }
	}

	/**
	 * A function to provide the information on supported picture
	 *  sizes to the user
	 * @return total number of supported sizes for the selected camera
	 */
	public int getNumberOfPictureSizes()
	{
		List <Camera.Size> sizeList;
		Camera.Parameters parameters = getCurrentParameters();
		sizeList = parameters.getSupportedPictureSizes();
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
		if(mPreview.mCamera == null)
		{
			mPreview.mCamera = mCamera;
			mPreview.initiateCamera();
		}
	}

	/**
	 * Sets the active camera
	 * @param CameraNumber index of the camera from the available cameras
	 * @return RES_OK
	 */
	public int setActiveCamera(int CameraNumber)
	{
		if(mNumCameras > 1)
		{
			if(currentCameraIndex != CameraNumber)
			{
				currentCameraIndex = CameraNumber;
				mCamera.release();
				mPreview.mCamera = null;
				mCamera = Camera.open(CameraNumber);
				mCamera.setParameters(getCurrentParameters());
			}

		}
		return MAAPI_consts.MA_CAMERA_RES_OK;
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
				mPreview.mCamera = mCamera;
				mPreview.initiateCamera();
			}
			mCamera.startPreview();
		}
		catch (Exception e)
		{
			SYSLOG(e.getMessage());
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
			mCamera.stopPreview();
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

	private Camera.Parameters getCurrentParameters()
	{
		return mCameraParametersList.get(currentCameraIndex);
	}

	public int setCameraProperty(String key, String value)
	{
		if(mCamera == null)
		{
			return MAAPI_consts.MA_CAMERA_RES_FAILED;
		}
		try
		{
			Camera.Parameters param = getCurrentParameters();
			if(key.equals(MAAPI_consts.MA_CAMERA_IMAGE_FORMAT))
			{
				if(value.equals(MAAPI_consts.MA_CAMERA_IMAGE_RAW))
				{
					rawMode = true;
				}
				else
				{
					//default mode is jpeg
					rawMode = false;
				}
			}
			else if(key.equals(MAAPI_consts.MA_CAMERA_FOCUS_MODE))
			{
				if(value.equals(MAAPI_consts.MA_CAMERA_FOCUS_AUTO))
				{
					mCamera.autoFocus(null);
				}
				else if(value.equals(MAAPI_consts.MA_CAMERA_FOCUS_MACRO))
				{
					if(false == param.getSupportedFocusModes().contains(value))
					{
						return MAAPI_consts.MA_CAMERA_RES_VALUE_NOTSUPPORTED;
					}
					mCamera.autoFocus(null);
				}
				else if(false == param.getSupportedFocusModes().contains(value))
				{
					mCamera.cancelAutoFocus();
					return MAAPI_consts.MA_CAMERA_RES_VALUE_NOTSUPPORTED;
				}
				else
					mCamera.cancelAutoFocus();

				param.setFocusMode(value);
			}
			else if(key.equals(MAAPI_consts.MA_CAMERA_FLASH_MODE))
			{
				if(true == param.getSupportedFlashModes().contains(value))
				{
					param.setFlashMode(value);
				}
				else
				{
					return MAAPI_consts.MA_CAMERA_RES_VALUE_NOTSUPPORTED;
				}
			}
			else
			{
				param.set(key, value);
			}
			mCamera.setParameters(param);
		}
		catch (Exception e)
		{
			return MAAPI_consts.MA_CAMERA_RES_FAILED;
		}
		return MAAPI_consts.MA_CAMERA_RES_OK;
	}

	public int getCameraPorperty(String key,
			int memBuffer,
			int memBufferSize)
	{
		if(mCamera == null)
		{
			return MAAPI_consts.MA_CAMERA_RES_FAILED;
		}
		Camera.Parameters param = getCurrentParameters();
		String result;
		if(key.equals(MAAPI_consts.MA_CAMERA_IMAGE_FORMAT))
		{
			if(rawMode == true)
				result = MAAPI_consts.MA_CAMERA_IMAGE_RAW;
			else
				result = MAAPI_consts.MA_CAMERA_IMAGE_JPEG;
		}
		else if(key.equals(MAAPI_consts.MA_CAMERA_FLASH_SUPPORTED))
		{
			if( param.getSupportedFlashModes() != null )
			{
				if(param.getSupportedFlashModes().size() == 1)
				{
					result = "false";
				}
				else
				{
					result = "true";
				}
			}
			else
			{
				result = "false";
			}
		}
		else if(key.equals(MAAPI_consts.MA_CAMERA_ZOOM_SUPPORTED))
		{
			if(param.isZoomSupported())
				result = "true";
			else
				result = "false";
		}
		else
			result = param.get(key);
		if(result == null)
			return MAAPI_consts.MA_CAMERA_RES_INVALID_PROPERTY_VALUE;

		if( result.length( ) + 1 > memBufferSize )
		{
			Log.e( "MoSync", "maCameraGetProperty: Buffer size " + memBufferSize +
					" too short to hold buffer of size: " + result.length( ) + 1 );
			return MAAPI_consts.MA_CAMERA_RES_FAILED;
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
		if(mCamera != null)
		{
			mCamera.release();
			if (mPreview != null)
			{
				mPreview.mCamera = null;
			}
			mCamera = null;
		}
	}
	/**
	 * Releases the camera in case of pause or exit
	 */
	public void acquireCamera()
	{
		if(mCamera == null)
		{
			mCamera = Camera.open();
			if(mPreview != null)
			{
				mPreview.mCamera = mCamera;
			}
		}
	}

	/**
	 * a wrapper to set a new picture size when taking a snapshot
	 */
	private void setNewSize(int formatIndex)
	{
		int width = userWidths.get(formatIndex).intValue();
		int height = userHeights.get(formatIndex).intValue();
    	try
    	{
			Camera.Parameters parameters = getCurrentParameters();
			List <Camera.Size> supportedSizes =  parameters.getSupportedPictureSizes();
			Camera.Size optimalPictureSize = mPreview.getOptimalSize(supportedSizes, width, height);
			parameters.setPictureSize(optimalPictureSize.width,optimalPictureSize.height);
			mCamera.setParameters(parameters);
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
		@Override
		public void onPictureTaken(byte[] data, Camera camera) {
			if(rawMode == true)
			{
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
		}
	};

	/**
	 * Handles data for jpeg picture
	 */
	PictureCallback jpegCallback = new PictureCallback() {
		@Override
		public void onPictureTaken(byte[] data, Camera camera) {
			if(rawMode == false)
			{
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
		}
	};
}
