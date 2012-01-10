/* Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

package com.mosync.internal.android;

import java.io.File;
import java.util.Hashtable;
import java.util.TimerTask;

import com.mosync.internal.android.MoSyncCapture.CaptureObject.CaptureType;
import com.mosync.internal.android.MoSyncThread.ImageCache;
import com.mosync.internal.android.notifications.LocalNotificationObject;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.util.HandleTable;
import com.mosync.nativeui.util.properties.BooleanConverter;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

import android.app.Activity;
import android.content.ContentValues;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Environment;
import android.provider.MediaStore;
import android.provider.MediaStore.MediaColumns;
import android.util.Log;

import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_ACTION_RECORD_VIDEO;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_ACTION_STOP_RECORDING;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_ACTION_TAKE_PICTURE;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_CAPTURE;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_EVENT_TYPE_IMAGE;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_EVENT_TYPE_VIDEO;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_EVENT_TYPE_CANCEL;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_DISPLAY_NAME;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_GALLERY;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_MAX_DURATION;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_VIDEO_QUALITY;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_INVALID_ACTION;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_OK;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_CAMERA_NOT_AVAILABLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_VIDEO_NOT_SUPPORTED;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_PICTURE_NOT_SUPPORTED;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_INVALID_HANDLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_INVALID_PROPERTY;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_INVALID_PROPERTY_VALUE;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_INVALID_STRING_BUFFER_SIZE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_INVALID_PROPERTY_VALUE;

/**
 * Class that implements Capture API.
 * @author emma tresanszki
 */
public class MoSyncCapture
{

	/**
	 * Request codes for Camera intent.
	 */
	public static final int CAPTURE_MODE_RECORD_VIDEO_REQUEST = 0;
	public static final int CAPTURE_MODE_STOP_RECORDING_REQUEST = 1;
	public static final int CAPTURE_MODE_TAKE_PICTURE_REQUEST = 2;

	// Used as a key for determining whether or not a photo was taken.
	public static final String PHOTO_TAKEN = "photo_taken";

	/**
	 * Class for holding capture object information.
	 * After a photo is captured a new CaptureObject is created and it
	 * contains the imageUri, the path.
	 * After a video is recorded a new CaptureObject is created and it
	 * contains the videoUri the absolute path to it etc.
	 * @author emma.
	 *
	 */
	public class CaptureObject
	{

// Store the type of the capture.
//		public enum CaptureType {
//			IMAGE, VIDEO
//		}
//
//		public CaptureObject(CaptureType type)
//		{
//			mCaptureType = type;
//		}

		//public CaptureType mCaptureType;

		public String mPath = null;
	}

	/**
	 * Constructor Capture API.
	 * @param thread The underlying MoSync thread.
	 */
	public MoSyncCapture(MoSyncThread thread, Hashtable<Integer, ImageCache> imageTable)
	{
		mMoSyncThread = thread;
		mImageTable = imageTable;
	}

	/**
	 * @return The Activity object.
	 */
	private Activity getActivity() {
		return mMoSyncThread.getActivity();
	}

	/************************ Syscalls ************************/

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
		try{
			if ( property.equals(MA_CAPTURE_DISPLAY_NAME) )
			{

			}
			else if ( property.equals(MA_CAPTURE_GALLERY) )
			{
				// If set to true store the captured image also in Gallery.
				// If set to false do not create a file for the image.
				mStoreToGallery = BooleanConverter.convert(value);
			}
			else
			{
				Log.e("@@MoSync","maCaptureSetProperty: Invalid property name " + property);
				return MA_CAPTURE_RES_INVALID_PROPERTY;
			}
		} catch(InvalidPropertyValueException ipve)
		{
			Log.e("@@MoSync","maCaptureSetProperty: Error while converting property value " + value + ":" + ipve.getMessage() );
			return MA_CAPTURE_RES_INVALID_PROPERTY_VALUE;
		}
		return MA_CAPTURE_RES_OK;
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
		String result = null;
		if ( property.equals(MA_CAPTURE_GALLERY) )
		{
			result = String.valueOf(mStoreToGallery);
		}
		else
		{
			Log.e("@@MoSync","maCaptureGetProperty: Invalid property " + property);
			return MA_CAPTURE_RES_INVALID_PROPERTY;
		}

		if( result.length( ) + 1 > valueSize )
		{
			Log.e( "@@MoSync", "maCaptureGetProperty: Buffer size " + valueSize +
					" too short to hold buffer of size: " + result.length( ) + 1 );
			return MA_CAPTURE_RES_INVALID_STRING_BUFFER_SIZE;
		}

		byte[] byteArray = result.getBytes();

		// Write string to MoSync memory.
		mMoSyncThread.mMemDataSection.position( valueBuffer );
		mMoSyncThread.mMemDataSection.put( byteArray );
		mMoSyncThread.mMemDataSection.put( (byte)0 );

		return MA_CAPTURE_RES_OK; // result.getBytes();
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
		switch ( action )
		{
			case MA_CAPTURE_ACTION_RECORD_VIDEO:
			{
				recordVideo();
				break;
			}
			case MA_CAPTURE_ACTION_TAKE_PICTURE:
			{
				takePicture();
				break;
			}
			default:
			{
				Log.e("@@MoSync","Capture: maCaptureAction invalid action " + action);
				break;
			}
		}
		return MA_CAPTURE_RES_OK;
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
	int maCaptureWriteImage(int handle, int fullPathBuffer, int fullPathBufSize)
	{

		return MA_CAPTURE_RES_OK;
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
		CaptureObject capture = m_captureObjects.get(handle);
		if ( null == capture )
		{
			Log.e("@@MoSync", "maCaptureGetVideoPath: Invalid video handle: "+ handle);
			return MA_CAPTURE_RES_INVALID_HANDLE;
		}
		String result = capture.mPath;
		if( result.length( ) + 1 > bufferSize )
		{
			Log.e( "MoSync", "maCaptureGetVideoPath: Buffer size " + bufferSize +
					" too short to hold buffer of size: " + result.length( ) + 1 );
			return MA_CAPTURE_RES_INVALID_STRING_BUFFER_SIZE;
		}

		byte[] byteArray = result.getBytes();

		// Write string to MoSync memory.
		mMoSyncThread.mMemDataSection.position( buffer );
		mMoSyncThread.mMemDataSection.put( byteArray );
		mMoSyncThread.mMemDataSection.put( (byte)0 );

		return MA_CAPTURE_RES_OK;
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
		return MA_CAPTURE_RES_OK;
	}

	/************************ Implementation Support ************************/

	/**
	 * Handle the result after a picture was taken( the control went back to MoSync).
	 * @param intent
	 */
	public static void handlePicture(Intent data)
	{
		//int imageHandle = data.getExtras().getInt("data");
	    Bitmap photo = (Bitmap) data.getExtras().get("data");

        // Create handle.
        int dataHandle = mMoSyncThread.nativeCreatePlaceholder();
		mImageTable.put(dataHandle, new ImageCache(null, photo));

		// Create new CaptureObject.
//		CaptureObject capture = new CaptureObject(CaptureType.IMAGE);
//		m_captureObjects.put(dataHandle, capture);

	    // Post MoSync event.
	    postEventNotificationReceived(MA_CAPTURE_EVENT_TYPE_IMAGE, dataHandle); // + the default image name
	}

	/**
	 * Handle the result after a video was recorded and the control
	 * went back to MoSync activity.
	 * @param data
	 */
	public static void handleVideo(Intent data)
	{
		mVideoUri = data.getData();
		Log.e("emma",mVideoUri.getPath());
		// here it is /external/video/media/16, instead of DCIM/MOV_0077.mp4
		if ( mVideoUri == null )
		{
			Log.e("emma","uri is null");
			return;
		}

		// Get the name of the recorder video.
		String[] projection = { MediaColumns.DATA };

		Cursor cursor = mMoSyncThread.getActivity().managedQuery(
				mVideoUri, projection, null, null, null);
		int column_index = -1;
		try{
			column_index = cursor
		        .getColumnIndexOrThrow(MediaColumns.DATA);
		}catch ( IllegalArgumentException iae )
		{
			return;
		}
		if ( cursor.moveToFirst() )
		{
			Log.e("emma",cursor.getString(column_index));
			// Here save name to mPath of captureObject.
		}
		//   /sdcard/DCIM/100ANDRO/MOV_0079.mp4
        // Create handle.
        int dataHandle = mMoSyncThread.nativeCreatePlaceholder();

        // Create a new CaptureObject.

		// Post MoSync event.
		postEventNotificationReceived(MA_CAPTURE_EVENT_TYPE_VIDEO, dataHandle);
	}

	/**
	 * Handle the result after a video was recorded and the control
	 * went back to MoSync activity.
	 * @param data
	 */
	public static void handleStopRecording(Intent data)
	{

		// Post MoSync event.
		postEventNotificationReceived(MA_CAPTURE_EVENT_TYPE_VIDEO, -1);
	}

	/**
	 *
	 */
	public static void handleCaptureCanceled()
	{
		Log.e("@@MoSync", "Capture: capture was canceled by the user.");

		// Post event to the MoSync queue.
		postEventNotificationReceived(MA_CAPTURE_EVENT_TYPE_CANCEL, -1);
	}

	/**
	 * Post a message to the MoSync event queue.
	 * Send the handle of new image/video object.
	 * @param captureEventType One of the #MA_CAPTURE_EVENT_TYPE constants.
	 * @param handle Handle to a video/image data object.
	 * This value is set only if the event type is MA_CAPTURE_EVENT_TYPE_IMAGE
	 * or MA_CAPTURE_EVENT_TYPE_VIDEO.
	 */
	private static void postEventNotificationReceived(
			int captureEventType, int handle)
	{
		int[] event = new int[3];
		event[0] = EVENT_TYPE_CAPTURE;
		event[1] = captureEventType;
		event[2] = handle;

		mMoSyncThread.postEvent(event);
	}

	/**
	 * Start Camera activity that captures an image and returns it.
	 */
	private void takePicture()
	{
		Intent cameraIntent = new Intent(android.provider.MediaStore.ACTION_IMAGE_CAPTURE);
		File file = new File(Environment.getExternalStorageDirectory(), "testMyFile.jpg");
		mImageUri = Uri.fromFile(file);
		// mImageUri = getTempImageUri();

		/**
		 * On 1.5 documentation: "If the EXTRA_OUTPUT is not present, then a small
		 * sized image is returned as a Bitmap object in the extra field.
		 * If the EXTRA_OUTPUT is present, then the full-sized image will be written to
		 * the Uri value of EXTRA_OUTPUT."
		 */
		cameraIntent.putExtra(MediaStore.EXTRA_OUTPUT, mImageUri);

		getActivity().startActivityForResult(cameraIntent, CAPTURE_MODE_TAKE_PICTURE_REQUEST);
	}

	/**
	 *
	 * @return
	 */
    private Uri getTempImageUri()
    {
        File photo;
        Uri imgUri = null;
        try
        {
            // place where to store camera taken picture
            photo = this.createTemporaryFile("testFile", ".jpg");
            // the abs path of the temp file will be : /mnt/sdcard/.temp/testAgainDATEINMILLIS.jpg
            imgUri = Uri.fromFile(photo);
            photo.delete();
        }
        catch(Exception e)
        {
            Log.e("@@MoSync", "Capture: Can't create file to take picture!");
        }
        return imgUri;
    }

    /**
     *
     * @param part
     * @param ext
     * @return
     * @throws Exception
     */
	private File createTemporaryFile(String part, String ext) throws Exception
	{
	    File tempDir= Environment.getExternalStorageDirectory();
	    tempDir=new File(tempDir.getAbsolutePath()+"/.temp/");
//	    tempDir = new File(tempDir.getAbsolutePath());
	    if(!tempDir.exists())
	    {
	        tempDir.mkdir();
	    }
	    return File.createTempFile(part, ext, tempDir);
	}

	/**
	 * Start
	 */
	private void recordVideo()
	{
		// On Motorola the activity is called com.motorola.Camera.Camcorder
		Intent videoIntent = new Intent(android.provider.MediaStore.ACTION_VIDEO_CAPTURE);
		videoIntent.putExtra(MediaStore.EXTRA_DURATION_LIMIT, "10");
//    	cameraIntent.putExtra(MediaStore.EXTRA_MEDIA_ALBUM, "MyAlbum");
//    	cameraIntent.putExtra(MediaStore.EXTRA_MEDIA_ARTIST, "Artist");
		videoIntent.putExtra(MediaStore.EXTRA_VIDEO_QUALITY, 0);

		getActivity().startActivityForResult(videoIntent, CAPTURE_MODE_RECORD_VIDEO_REQUEST);
	}

	/**
	 * Stop video recording activity and return to MoSync.
	 */
	private void stopRecording()
	{

	}
	/************************ Class members ************************/

	/**
	 * The MoSync thread object.
	 */
	private static MoSyncThread mMoSyncThread;

    /**
     * It has access to the image resource table.
     */
    private static Hashtable<Integer, ImageCache> mImageTable;

	/**
	 * A table that contains a mapping between a handle and a capture object.
	 */
//	private HandleTable<CaptureObject> m_handles =
//		new HandleTable<CaptureObject>();
	/**
	 * A table that contains mapping between a handle and a capture object.
	 * The capture object represents a captured image or recorded video.
	 * The key is the handle of the image or video.
	 */
	private static Hashtable<Integer,CaptureObject> m_captureObjects =
		new Hashtable<Integer,CaptureObject>();
	/**
	 * The current image.
	 */
	private Uri mImageUri;

	/**
	 * The current video.
	 */
	private static Uri mVideoUri;

	/**
	 * Store the state of the last capture action.
	 * Modify if when a picture is taken.
	 */
	public static boolean mPhotoTaken = false;

	/**
	 * Indicates if the captured image should be stored in Gallery.
	 * By default a captured image is store in Gallery album.
	 * If the value is set to false then a temporary file will be used
	 * instead, and no file will be generated after the image capture.
	 */
	public static boolean mStoreToGallery = true;
}