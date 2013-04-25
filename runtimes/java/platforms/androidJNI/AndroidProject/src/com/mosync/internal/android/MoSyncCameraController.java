package com.mosync.internal.android;

import static com.mosync.internal.android.MoSyncHelpers.EXTENT;
import static com.mosync.internal.android.MoSyncHelpers.SYSLOG;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_CAMERA_SNAPSHOT;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_CAMERA_PREVIEW;

import static com.mosync.internal.generated.MAAPI_consts.MA_CAMERA_RES_OK;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAMERA_RES_FAILED;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAMERA_RES_INVALID_PROPERTY_VALUE;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAMERA_RES_VALUE_NOTSUPPORTED;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAMERA_RES_EVENTS_ALREADY_ENABLED;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAMERA_RES_INVALID_PREVIEW_SIZE;

import static com.mosync.internal.generated.MAAPI_consts.MA_CAMERA_FOCUS_AUTO;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAMERA_FOCUS_MACRO;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAMERA_IMAGE_JPEG;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAMERA_IMAGE_RAW;

import static com.mosync.internal.generated.MAAPI_consts.MA_IMAGE_REPRESENTATION_UNKNOWN;
import static com.mosync.internal.generated.MAAPI_consts.MA_IMAGE_REPRESENTATION_RAW;
import static com.mosync.internal.generated.MAAPI_consts.MA_IMAGE_REPRESENTATION_JPEG;

import static com.mosync.internal.generated.MAAPI_consts.MA_CAMERA_FOCUS_MODE;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAMERA_FLASH_MODE;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAMERA_IMAGE_FORMAT;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAMERA_ZOOM_SUPPORTED;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAMERA_FLASH_SUPPORTED;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAMERA_PREVIEW_FRAME;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAMERA_PREVIEW_AUTO_FOCUS;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAMERA_SNAPSHOT_MAX_SIZE;

import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;

import java.lang.System;

import android.app.Activity;
import android.hardware.Camera;
import android.hardware.Camera.PictureCallback;
import android.hardware.Camera.ShutterCallback;
import android.hardware.Camera.Size;

import java.lang.Thread;

import android.os.Build;
import android.util.Log;

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

	private byte[] mCallbackBuffer = null;
	private byte[] mImageBuffer = null;

	/**
	* Stores number of availabe cameras on the device
	*/
	private int mNumCameras;

	/**
	* Indicates the current camera index
	*/
	private int mCurrentCameraIndex;

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
	 * Used to check if snapshot is in progress.
	 */
	private boolean mIsSnapshotInProgress = false;

	/**
	 * We need this to know when to persist with the camera parameters.
	 */
	private MoSyncCameraPreview mPreviousPreview;


	/**
	 * The MoSync thread object.
	 */
	public MoSyncThread mMoSyncThread;


	private boolean mMoSyncPreviewEventEnabled = false;
	private boolean mMoSyncPreviewFrameEventEnabled = false;
	private boolean mMoSyncPreviewAutoFocusEventEnabled = false;
	private IntBuffer mMoSyncPreviewEventBuffer = null;

	private boolean mMoSyncPreviewHasFocus = false;

	private int mPreviewEventBufferLeft;
	private int mPreviewEventBufferTop;
	private int mPreviewEventBufferRight;
	private int mPreviewEventBufferBottom;

	private boolean mSendEvent = true;

	/**
	 * Constructor.
	 * @param thread The MoSync thread.
	 */
	public MoSyncCameraController(MoSyncThread thread)
	{
		mMoSyncThread = thread;
		lock = new ReentrantLock();
		mPreview = null;
		mPreviousPreview = null;
		condition = lock.newCondition();
		dataReady = false;
		userWidths = new ArrayList<Integer>();
		userHeights = new ArrayList<Integer>();
		mCameraParametersList = new ArrayList<Camera.Parameters>();
		mNumCameras = numberOfCameras();
		initializeCameras();
		rawMode = false;
		mCurrentCameraIndex = 0;

	}

	/**
	 * Open the given camera. On API levels below 9 only one
	 * camera is available and Camera.open() is used.
	 * @param cameraIndex
	 * @return A Camera object.
	 */
	private Camera cameraOpen(int cameraIndex)
	{
		int apiLevel = Build.VERSION.SDK_INT;
		if (apiLevel >= Build.VERSION_CODES.GINGERBREAD)
		{
		    return Camera.open(cameraIndex);
		}
		else
		{
			return Camera.open();
		}
	}

	/**
	* Lazy initialization of the cameras so we can use the number of cameras
	*/
	private void initializeCameras()
	{
		try
		{
			mCurrentCameraIndex = 0;
			// TODO: If mNumCameras is less than 1 we have no camera?
			// Then does it work to open it?
			if(mNumCameras <= 1)
			{
				mCamera = cameraOpen(0);
				mCameraParametersList.add(mCamera.getParameters());
			}
			else
			{
				if(mCamera != null)
					mCamera.release();
				for(int ii=0; ii < mNumCameras; ii++)
				{

					mCamera = cameraOpen(ii);
					mCameraParametersList.add(mCamera.getParameters());
					mCamera.release();
				}
				mCamera = cameraOpen(mCurrentCameraIndex);
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
		if (mNumCameras != 0)
		{
			// Do not do the costly operation of reflection again
			return mNumCameras;
		}

		int numCameras = 1;
		try
		{
			numCameras = Camera.getNumberOfCameras();
		}
		catch (NoSuchMethodError nsme)
		{
			SYSLOG("ANDROID Version is less than 2.3!!");
		}
		return numCameras;
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
		userWidths.add(index, Integer.valueOf(width));
		userHeights.add(index, Integer.valueOf(height));
		return MA_CAMERA_RES_OK;
	}

	/**
	 * Sets the MoSyncCameraPreview object to be controlled
	 * @param preview an object of MoSyncCameraPreview that has already been initialized
	 */
	public void setPreview(MoSyncCameraPreview preview)
	{
		SYSLOG("setPreview");
		acquireCamera();

		// Retain the previous preview.
		if ( preview != mPreview )
		{
			mPreviousPreview = mPreview;
		}

		mPreview = preview;
		if(mPreview.mCamera == null)
		{
			mPreview.mCamera = mCamera;
			mPreview.initiateCamera();
			mPreview.mCameraIndex = mCurrentCameraIndex;

			setPreviewCallback();

			Camera.Parameters parameters = getCurrentParameters();
			Camera.Size size = parameters.getPreviewSize();

			mImageBuffer = new byte[size.width * size.height * 4];
		}
	}

	private void setPreviewCallback()
	{
		if(mCamera == null)
		{
			android.util.Log.e("MOSYNC INTERNAL","No Preview set");
			return;
		}
		Camera.Parameters parameters = getCurrentParameters();
		Camera.Size size = parameters.getPreviewSize();

		mCallbackBuffer = new byte[size.width * size.height * 4];

		mCamera.addCallbackBuffer(mCallbackBuffer);
		mCamera.setPreviewCallbackWithBuffer(previewCallback);
	}

	/**
	 * Sets the MoSyncCameraPreview object to be controlled
	 * @param preview an object of MoSyncCameraPreview that has already been initialized
	 */
	public void removePreview()
	{
		mPreview = null;
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
			if(mCurrentCameraIndex != CameraNumber)
			{
				mCurrentCameraIndex = CameraNumber;
				if (null != mCamera)
					mCamera.release();
				mPreview.mCamera = null;
				try {
					mCamera = cameraOpen(CameraNumber);
					mPreview.mCameraIndex = mCurrentCameraIndex;
					mCamera.setParameters(getCurrentParameters());
				} catch (Exception e) {
					SYSLOG("cannot open camera " + e);
				}
			}

		}
		return MA_CAMERA_RES_OK;
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
	 * Checks to see if a snapshot is in progress.
	 *
	 * @return true if snapshot is in progress, false otherwise.
	 */
	public boolean isSnapshotInProgress()
	{
		return mIsSnapshotInProgress;
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
		SYSLOG("cameraStart");

		acquireCamera();

		try
		{
			if(mPreview.mCamera == null)
			{
				mPreview.mCamera = mCamera;
				mPreview.initiateCamera();
				mPreview.mCameraIndex = mCurrentCameraIndex;
				setPreviewCallback();
			}
			/*
			 * We need to maintain the old parameters. This is
			 * mandatory for the case when a zoom level is set
			 * and a snapshot is taken. Because a close and a reopen
			 * is mandatory after a snapshot, the zoom level would be
			 * reset to 0 and not to the previous level.
			 *
			 * If we don't have a previous preview or if it's the same as
			 * the current preview we maintain the camera parameters.
			 */
			if ( (mPreviousPreview == null) || (mPreviousPreview == mPreview) )
			{
				mCamera.stopPreview();
				Camera.Parameters param = getCurrentParameters();
				mCamera.setParameters(param);
			}

			mCamera.startPreview();
		}
		catch (Exception e)
		{
			SYSLOG("cameraStart: " + e.getMessage());
			return MA_CAMERA_RES_FAILED;
		}

		return MA_CAMERA_RES_OK;
	}

	/**
	 * Called to stop the preview on the camera
	 */
	public int cameraStop()
	{
		try
		{
			mCamera.stopPreview();
			releaseCamera();
			if ( mIsSnapshotInProgress )
			{
				postCameraSnapshotEvent(resourceIndex, 0,
						MA_IMAGE_REPRESENTATION_UNKNOWN, MA_CAMERA_RES_FAILED);
			}
			mIsSnapshotInProgress = false;
		}
		catch (Exception e)
		{
			return MA_CAMERA_RES_FAILED;
		}

		return MA_CAMERA_RES_OK;
	}


	/**
	 * Takes a picture using the camera, waits for the callbacks
	 * and sends the data to MoSync Program
	 */
	public int cameraSnapshot(int formatIndex, int placeHolder)
	{
		// If the max size constant is provided then we select the biggest size
		// available
		if ( MA_CAMERA_SNAPSHOT_MAX_SIZE == formatIndex )
		{
			Camera.Parameters parameters = getCurrentParameters();
			List <Camera.Size> sizeList = parameters.getSupportedPictureSizes();
			formatIndex = sizeList.size() - 1;
		}

		if(formatIndex >= 0)
		{
			setNewSize(formatIndex);
		}
		resourceIndex = placeHolder;

		// mPreview.mCamera.cancelAutoFocus();
		mPreview.mCamera.takePicture(null, rawCallback, jpegCallback);

		lock.lock();
		try
		{
			while (dataReady == false)
			{
				condition.await();
			}

			dataReady = false;
			return MA_CAMERA_RES_OK;
		}
		catch (InterruptedException e)
		{
			return MA_CAMERA_RES_FAILED;
		}
		finally
		{
			lock.unlock();
		}
	}

	public void cameraSnapshotAsync(int dataPlaceholder, int sizeIndex)
	{
		// If the max size constant is provided then we select the biggest size
		// available
		if ( MA_CAMERA_SNAPSHOT_MAX_SIZE == sizeIndex )
		{
			Camera.Parameters parameters = getCurrentParameters();
			List <Camera.Size> sizeList = parameters.getSupportedPictureSizes();
			sizeIndex = sizeList.size() - 1;
		}
		if(sizeIndex >= 0)
		{
			setNewSize(sizeIndex);
		}

		resourceIndex = dataPlaceholder;
		mIsSnapshotInProgress = true;
		mPreview.mCamera.takePicture(null,
				rawCallbackWithEventSupport, jpegCallbackWithEventSupport);
	}

	private Camera.Parameters getCurrentParameters()
	{
		return mCameraParametersList.get(mCurrentCameraIndex);
	}

	public int setCameraProperty(String key, String value)
	{
		if(mCamera == null)
		{
			return MA_CAMERA_RES_FAILED;
		}
		try
		{
			Camera.Parameters param = getCurrentParameters();
			if(key.equals(MA_CAMERA_IMAGE_FORMAT))
			{
				if(value.equals(MA_CAMERA_IMAGE_RAW))
				{
					rawMode = true;
				}
				else
				{
					//default mode is jpeg
					rawMode = false;
				}
			}
			else if(key.equals(MA_CAMERA_FOCUS_MODE))
			{
				if(value.equals(MA_CAMERA_FOCUS_AUTO))
				{
					if(false == param.getSupportedFocusModes().contains(value))
					{

						return MA_CAMERA_RES_VALUE_NOTSUPPORTED;
					}

					mCamera.autoFocus(autoFocusCallback);
				}
				else if(value.equals(MA_CAMERA_FOCUS_MACRO))
				{

					if(false == param.getSupportedFocusModes().contains(value))
					{
						return MA_CAMERA_RES_VALUE_NOTSUPPORTED;
					}
					mCamera.autoFocus(autoFocusCallback);
				}
				else if(false == param.getSupportedFocusModes().contains(value))
				{
					mCamera.cancelAutoFocus();
					return MA_CAMERA_RES_VALUE_NOTSUPPORTED;
				}
				else
					mCamera.cancelAutoFocus();

				param.setFocusMode(value);
			}
			else if(key.equals(MA_CAMERA_FLASH_MODE))
			{
				if(true == param.getSupportedFlashModes().contains(value))
				{
					param.setFlashMode(value);
				}
				else
				{
					return MA_CAMERA_RES_VALUE_NOTSUPPORTED;
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
			return MA_CAMERA_RES_FAILED;
		}
		return MA_CAMERA_RES_OK;
	}

	public int getCameraProperty(String key,
			int memBuffer,
			int memBufferSize)
	{
		if(mCamera == null)
		{
			return MA_CAMERA_RES_FAILED;
		}
		Camera.Parameters param = getCurrentParameters();
		String result;
		if(key.equals(MA_CAMERA_IMAGE_FORMAT))
		{
			if(rawMode == true)
				result = MA_CAMERA_IMAGE_RAW;
			else
				result = MA_CAMERA_IMAGE_JPEG;
		}
		else if(key.equals(MA_CAMERA_FLASH_SUPPORTED))
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
		else if(key.equals(MA_CAMERA_ZOOM_SUPPORTED))
		{
			if(param.isZoomSupported())
				result = "true";
			else
				result = "false";
		}
		else
		{
			result = param.get(key);
		}

		if(result == null)
		{
			return MA_CAMERA_RES_INVALID_PROPERTY_VALUE;
		}

		if( result.length( ) + 1 > memBufferSize )
		{
			Log.e( "Android Runtime", "maCameraGetProperty: Buffer size " + memBufferSize +
					" too short to hold buffer of size: " + result.length( ) + 1 );
			return MA_CAMERA_RES_FAILED;
		}

		byte[] ba = result.getBytes();

		// Write string to MoSync memory.
		ByteBuffer buffer = mMoSyncThread.getMemorySlice(memBuffer, ba.length + 1);
		buffer.put(ba);
		buffer.put((byte) 0);

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
		SYSLOG("acquireCamera");
		if(mCamera == null)// && mPreview != null)
		{
			try
			{
				if (mNumCameras <= 1) {
					mCamera = cameraOpen(0);
				} else {
					mCamera = cameraOpen(mCurrentCameraIndex);
				}
				//mPreview.mCamera = mCamera;
				mCamera.setErrorCallback(cameraErrorCallback);
			}
			catch (RuntimeException re)
			{
				SYSLOG("Exception while aquiring camera: " + re.toString());
				mCamera = null;
				mPreview = null;
				return;
			}
			SYSLOG("Camera was aquired");
			return;
		}
		SYSLOG("Camera already aquired");
		return;
	}

	/**
	 * a wrapper to set a new picture size when taking a snapshot
	 */
	private void setNewSize(int formatIndex)
	{
    	try
    	{
			Camera.Parameters parameters = getCurrentParameters();
			List <Camera.Size> supportedSizes =  parameters.getSupportedPictureSizes();
			try {
				Camera.Size optimalPictureSize = supportedSizes
						.get(formatIndex);
				parameters.setPictureSize(optimalPictureSize.width,
						optimalPictureSize.height);
			} catch (IndexOutOfBoundsException e) {
				SYSLOG("Size not supported " + formatIndex);
			}
			if (mCamera != null)
				mCamera.setParameters(parameters);
    	}
    	catch (Exception e)
    	{
			SYSLOG("FAILED TO SET the PARAMETERS " + e);
    	}
	}

	public int getPreviewSize()
	{
		SYSLOG("getPreviewSize");
		acquireCamera();

		Camera.Parameters parameters = getCurrentParameters();
		Camera.Size size = parameters.getPreviewSize();

		int extent = EXTENT(size.width, size.height);

		return extent;
	}


	/**
	 * Handles snapshot data for RAW representation.
	 */
	PictureCallback rawCallback = new PictureCallback() {
		@Override
		public void onPictureTaken(byte[] data, Camera camera) {
			if(rawMode == true)
			{
				handleSnapshotData(data);
			}
		}
	};

	/**
	 * Handles snapshot data for JPEG representation.
	 */
	PictureCallback jpegCallback = new PictureCallback() {
		@Override
		public void onPictureTaken(byte[] data, Camera camera) {
			if(rawMode == false)
			{
				handleSnapshotData(data);
			}
		}
	};

	/**
	 * Handles snapshot data.
	 * @param data array of snapshot data.
	 */
	private void handleSnapshotData(byte[] data)
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

	/**
	 * Handles snapshot data for RAW representation and notifies via camera event that snapshot
	 * operation has been completed.
	 */
	PictureCallback rawCallbackWithEventSupport = new PictureCallback() {
		@Override
		public void onPictureTaken(byte[] data, Camera camera) {
			if(rawMode == true)
			{
				handleSnapshotDataWithEventSupport(data, MA_IMAGE_REPRESENTATION_RAW);
			}
		}
	};

	ShutterCallback shutterCallback = new ShutterCallback() {
		@Override
		public void onShutter() {
			// If ever needed.
		}
	};

	PictureCallback postViewCallback = new PictureCallback() {
		@Override
		public void onPictureTaken(byte[] data, Camera camera) {
			// If ever needed.
		}
	};


	/**
	 * Handles snapshot data for JPEG representation and notifies via camera event that snapshot
	 * operation has been completed.
	 */
	PictureCallback jpegCallbackWithEventSupport = new PictureCallback() {
		@Override
		public void onPictureTaken(byte[] data, Camera camera) {
			if(rawMode == false)
			{
				handleSnapshotDataWithEventSupport(data, MA_IMAGE_REPRESENTATION_JPEG);
			}
		}
	};

	Camera.ErrorCallback cameraErrorCallback = new Camera.ErrorCallback() {

		@Override
		public void onError(int error, Camera camera) {
			/*
			 *  CAMERA_ERROR_SERVER_DIED 100
			 *  CAMERA_ERROR_UNKNOWN 1
			 */
			if ( mIsSnapshotInProgress )
			{
				mIsSnapshotInProgress = false;
				postCameraSnapshotEvent(resourceIndex, 0,
						MA_IMAGE_REPRESENTATION_UNKNOWN, MA_CAMERA_RES_FAILED);
			}
			SYSLOG("Camera failed with error: " + error);
		}
	};

	/**
	 * Handles snapshot data and notifies via camera event that snapshot operation is completed.
	 *
	 * @param data array of snapshot data.
	 * @param imageReprezentation snapshot representation.
	 */
	private void handleSnapshotDataWithEventSupport(byte[] data, int imageReprezentation)
	{
		mIsSnapshotInProgress = false;

		int snapshotSize = 0;
		int snapshotImageDataRepresentation = imageReprezentation;
		int snapshotReturnCode = MA_CAMERA_RES_OK;

		lock.lock();
		try {
			mMoSyncThread.nativeCreateBinaryResource(resourceIndex, data.length);
			ByteBuffer byteBuffer = mMoSyncThread.mBinaryResources.get(resourceIndex);
			byteBuffer.put(data);

			Camera.Parameters parameters = getCurrentParameters();
			Size pictureSize = parameters.getPictureSize();
			snapshotSize = EXTENT(pictureSize.width, pictureSize.height);
		}
		catch (Exception e) {
			SYSLOG("Failed to create the data pool");

			snapshotImageDataRepresentation = MA_IMAGE_REPRESENTATION_UNKNOWN;
			snapshotReturnCode = MA_CAMERA_RES_FAILED;
		}
		finally {
			lock.unlock();
			postCameraSnapshotEvent(resourceIndex, snapshotSize,
					snapshotImageDataRepresentation, snapshotReturnCode);
		}
	}

	/**
	 * Post a message to the MoSync event queue.
	 * Send the data of the snapshot.
	 * @param snapshotImageDataHandle Holds the place holder for the data object representing the camera snapshot.
	 * @param snapshotSize Holds the snapshot size.
	 * @param snapshotImageDataRepresentation Holds the snapshot data representation.
	 * @param snapshotReturnCode Holds the return code of the camera snapshot operation.
	 */
	private void postCameraSnapshotEvent(
			int snapshotImageDataHandle,
			int snapshotSize,
			int snapshotImageDataRepresentation,
			int snapshotReturnCode)
	{
		int[] event = new int[5];
		event[0] = EVENT_TYPE_CAMERA_SNAPSHOT;
		event[1] = snapshotImageDataHandle;
		event[2] = snapshotSize;
		event[3] = snapshotImageDataRepresentation;
		event[4] = snapshotReturnCode;
		mMoSyncThread.postEvent(event);
	}

	/**
	 * Handles the preview callbacks, which is fired everytime the camera has a new preview
	 * Returns directly unless the user has enabled preview events.
	 *
	 */
	Camera.PreviewCallback previewCallback = new Camera.PreviewCallback()
	{
		@Override
		public void onPreviewFrame(byte[] data, Camera camera)
		{
			if(!mMoSyncPreviewFrameEventEnabled)
			{
				if(!mMoSyncPreviewAutoFocusEventEnabled)
				{
					// Return the buffer if in use
					camera.addCallbackBuffer(data);

					return;
				}

				if(!mMoSyncPreviewHasFocus)
				{
					// Return the buffer if in use
					camera.addCallbackBuffer(data);

					return;
				}

				// Restore the flag for the next auto focus event
				mMoSyncPreviewHasFocus = false;
			}

			if(!mSendEvent)
			{
				// Return the buffer if in use
				camera.addCallbackBuffer(data);

				return;
			}

			lock.lock();

			try
			{
				if(data != null)
				{
					mSendEvent = false;

					System.arraycopy(data, 0, mImageBuffer, 0, data.length);

					// To the time consuming task in a new thread
					new Thread(new Runnable()
					{
						public void run()
						{
							YUV420toRGB8888();

							int[] event = new int[1];
							event[0] = EVENT_TYPE_CAMERA_PREVIEW;
							mMoSyncThread.postEvent(event);

						}
					}).start();
				}

			}
			catch (Exception e)
			{
				Log.i("Camera API","Got exception:" + e.toString());
			}
			finally
			{
				lock.unlock();

				// Return the buffer if in use
				camera.addCallbackBuffer(data);
			}
		}
	};

	Camera.AutoFocusCallback autoFocusCallback = new Camera.AutoFocusCallback()
	{
		@Override
		public void onAutoFocus(boolean success, Camera camera)
		{
			if(!mMoSyncPreviewAutoFocusEventEnabled)
				return;

			if(success)
				mMoSyncPreviewHasFocus = true;
		}
	};

	/**
	* Converts from YUV420 to RGB888
	* Source code for conversion found at:
	* https://groups.google.com/forum/?hl=fr&fromgroups#!topic/android-developers/yF6CmrIJzuo
	*/
	public void YUV420toRGB8888()
	{
		Camera.Size previewSize = mPreview.mPreviewSize;

		int width = previewSize.width;
		int height = previewSize.height;

		int size = width * height;

		mMoSyncPreviewEventBuffer.position(0);

		int i, j;
		int Y, Cr = 0, Cb = 0;
		for(j = mPreviewEventBufferTop; j < mPreviewEventBufferBottom; j++)
		{
			int pixPtr = j * width;
			final int jDiv2 = j >> 1;
			for(i = mPreviewEventBufferLeft; i < mPreviewEventBufferRight; i++)
			{
				Y = mImageBuffer[pixPtr++];
				if(Y < 0)
					Y += 255;
				if((i & 0x1) != 1)
				{
					final int cOff = size + jDiv2 * width + (i >> 1) * 2;
					Cb = mImageBuffer[cOff];
					if(Cb < 0)
						Cb += 127;
					else
						Cb -= 128;
					Cr = mImageBuffer[cOff + 1];
					if(Cr < 0)
						Cr += 127;
					else
						Cr -= 128;
				}
				int R = Y + Cr + (Cr >> 2) + (Cr >> 3) + (Cr >> 5);
				if(R < 0)
					R = 0;
				else if(R > 255)
					R = 255;
				int G = Y - (Cb >> 2) + (Cb >> 4) + (Cb >> 5) - (Cr >> 1) + (Cr >>
				3) + (Cr >> 4) + (Cr >> 5);
				if(G < 0)
					G = 0;
				else if(G > 255)
					G = 255;
				int B = Y + Cb + (Cb >> 1) + (Cb >> 2) + (Cb >> 6);
				if(B < 0)
					B = 0;
				else if(B > 255)
					B = 255;
				mMoSyncPreviewEventBuffer.put(0xff000000 + (B << 16) + (G << 8) + R);
			}
		}
	}

	public void getSize(int index, int format)
	{
		Camera.Parameters parameters = getCurrentParameters();
		List <Camera.Size> supportedSizes =  parameters.getSupportedPictureSizes();
		int []size;
		size = new int[2];
		size[0] = supportedSizes.get(index).width;
		size[1] = supportedSizes.get(index).height;

		byte[] array = int2byte(size);
		ByteBuffer buffer = mMoSyncThread.getMemorySlice(format, array.length + 1);
		buffer.put(array);
	}

	public int enablePreviewEvents(
			int type,
			int memBuffer,
			int left,
			int top,
			int width,
			int height)
	{
		if(mPreview == null && mCamera == null)
			return MA_CAMERA_RES_FAILED;

		if(mMoSyncPreviewEventEnabled)
			return MA_CAMERA_RES_EVENTS_ALREADY_ENABLED;

		if(type == MA_CAMERA_PREVIEW_FRAME)
		{
			if(mMoSyncPreviewAutoFocusEventEnabled)
				return MA_CAMERA_RES_EVENTS_ALREADY_ENABLED;

			// enable preview frame events
			mMoSyncPreviewFrameEventEnabled = true;

		}
		else if(type == MA_CAMERA_PREVIEW_AUTO_FOCUS)
		{
			if(mMoSyncPreviewFrameEventEnabled)
				return MA_CAMERA_RES_EVENTS_ALREADY_ENABLED;

			// enable preview auto foucs events
			mMoSyncPreviewAutoFocusEventEnabled = true;

		}
		else return MA_CAMERA_RES_FAILED;

		// Get the size of the preview and make sure the rect
		// ins't too big!
		//Camera.Parameters camParam = mCamera.getParameters();
		Camera.Size previewSize = mPreview.mPreviewSize;

		if(previewSize == null)
		{
			SYSLOG("Preview size is null");
			return MA_CAMERA_RES_FAILED;
		}

		int w = previewSize.width;
		int h = previewSize.height;

		if((left + width) > w)
			return MA_CAMERA_RES_INVALID_PREVIEW_SIZE;
		if((top + height) > h)
			return MA_CAMERA_RES_INVALID_PREVIEW_SIZE;

		int sliceSize = width * height * 4;

		mMoSyncPreviewEventBuffer =
			mMoSyncThread.getMemorySlice(memBuffer, sliceSize).asIntBuffer();

		if(mMoSyncPreviewEventBuffer == null)
			return MA_CAMERA_RES_FAILED;

		mPreviewEventBufferLeft = left;
		mPreviewEventBufferTop = top;
		mPreviewEventBufferRight = left + width;
		mPreviewEventBufferBottom = top + height;

		mMoSyncPreviewEventEnabled = true;

		return MA_CAMERA_RES_OK;
	}

	public int disablePreviewEvents()
	{
		mMoSyncPreviewEventEnabled = false;
		mMoSyncPreviewFrameEventEnabled = false;
		mMoSyncPreviewAutoFocusEventEnabled= false;

		mSendEvent = true;

		return MA_CAMERA_RES_OK;
	}

	public int previewEventConsumed()
	{
		if(!mMoSyncPreviewEventEnabled)
			return MA_CAMERA_RES_FAILED;

		mSendEvent = true;
		mMoSyncPreviewHasFocus = false;

		return MA_CAMERA_RES_OK;
	}

	private byte[] int2byte(int[]src)
	{
		int srcLength = src.length;
		    byte[]dst = new byte[srcLength << 2];

		    for (int i=0; i<srcLength; i++) {
				int x = src[i];
				int j = i << 2;
				dst[j++] = (byte) ((x >>> 0) & 0xff);
				dst[j++] = (byte) ((x >>> 8) & 0xff);
				dst[j++] = (byte) ((x >>> 16) & 0xff);
				dst[j++] = (byte) ((x >>> 24) & 0xff);
		    }
		    return dst;
	}
}
