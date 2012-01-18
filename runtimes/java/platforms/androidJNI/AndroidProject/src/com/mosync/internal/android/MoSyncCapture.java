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
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.Hashtable;

import com.mosync.internal.android.MoSyncThread.ImageCache;
import com.mosync.nativeui.core.NativeUI;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;

import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Build;
import android.os.Environment;
import android.provider.MediaStore;
import android.provider.MediaStore.MediaColumns;
import android.util.Log;

import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_ACTION_RECORD_VIDEO;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_ACTION_TAKE_PICTURE;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_CAPTURE;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_EVENT_TYPE_IMAGE;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_EVENT_TYPE_VIDEO;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_EVENT_TYPE_CANCEL;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_MAX_DURATION;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_VIDEO_QUALITY;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_VIDEO_QUALITY_LOW;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_VIDEO_QUALITY_HIGH;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_OK;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_INVALID_HANDLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_INVALID_PROPERTY;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_INVALID_PROPERTY_VALUE;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_PROPERTY_VALUE_UNDEFINED;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_MAX_DURATION_NOT_SUPPORTED;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_INVALID_ACTION;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_INVALID_STRING_BUFFER_SIZE;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_FILE_INVALID_NAME;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_FILE_ALREADY_EXISTS;

/**
 * Class that implements Capture API.
 * @author emma tresanszki
 */
public class MoSyncCapture
{

	/**
	 * Store the type of the capture.
	 */
	enum CaptureType {
		IMAGE, VIDEO
	}

	/**
	 * Request codes for Camera intent.
	 */
	public static final int CAPTURE_MODE_RECORD_VIDEO_REQUEST = 0;
	public static final int CAPTURE_MODE_STOP_RECORDING_REQUEST = 1;
	public static final int CAPTURE_MODE_TAKE_PICTURE_REQUEST = 2;

	// The value for HIGH and LOW video quality.
	public static final int CAPTURE_VIDEO_QUALITY_HIGH = 1;
	public static final int CAPTURE_VIDEO_QUALITY_LOW = 0;

	// The default value for properties that are not set.
	public static final int CAPTURE_DEFAULT_PROPERTY_VALUE = -1;

	// String used as a key for determining whether or not a photo was taken.
	public static final String PHOTO_TAKEN = "photo_taken";

	// The MediaStore.EXTRA_DURATION_LIMIT String which is available since API level 8.
	public static final String EXTRA_DURATION_LIMIT = "android.intent.extra.durationLimit";

	// The temporary file name for the IMAGE capture.
	public static final String CAPTURE_TEMP_DEFAULT_NAME = "MoSyncTempFile";

	/**
	 * Constructor Capture API.
	 * @param thread The underlying MoSync thread.
	 */
	public MoSyncCapture(MoSyncThread thread, Hashtable<Integer, ImageCache> imageTable)
	{
		mMoSyncThread = thread;
		mImageTable = imageTable;
		mContentResolver = mMoSyncThread.getActivity().getContentResolver();
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
		Log.e("@@MoSync","maCaptureSetProperty");

		try{
			if ( property.equals(MA_CAPTURE_MAX_DURATION) )
			{
				// EXTRA_DURATION_LIMIT is supported on Android 2.2 and higher.
				// Version.SDK_INT is not available on sdk 3.
				int target = IntConverter.convert( Build.VERSION.SDK );
				if ( target >= 8 )
				{
					mVideoDurationLimit = IntConverter.convert(value);
				}
				else
				{
					Log.e("@@MoSync","maCaptureSetProperty property " + property +
							" is not supported on current platform.");
					return MA_CAPTURE_RES_MAX_DURATION_NOT_SUPPORTED;
				}
			}
			else if ( property.equals( MA_CAPTURE_VIDEO_QUALITY) )
			{
				// Quality can be 0 or 1.
				int quality = IntConverter.convert(value);
				if ( quality == MA_CAPTURE_VIDEO_QUALITY_LOW )
				{
					mVideoQuality = CAPTURE_VIDEO_QUALITY_LOW;
				}
				else if ( quality == MA_CAPTURE_VIDEO_QUALITY_HIGH )
				{
					mVideoQuality = CAPTURE_VIDEO_QUALITY_HIGH;
				}
				else
				{
					Log.e("@@MoSync","maCaptureSetProperty: Invalid video quality value " + value);
					throw new InvalidPropertyValueException(property, value);
				}
			}
			else
			{
				Log.e("@@MoSync","maCaptureSetProperty: Invalid property name " + property);
				return MA_CAPTURE_RES_INVALID_PROPERTY;
			}
		} catch(Exception ex)
		{
			Log.e("@@MoSync","maCaptureSetProperty: Error while converting property value " + value + ":" + ex.getMessage() );
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
		Log.e("@@MoSync","maCaptureGetProperty");

		String result = null;
		if ( property.equals(MA_CAPTURE_MAX_DURATION) )
		{
			// EXTRA_DURATION_LIMIT is supported on Android 2.2 and higher.
			// Version.SDK_INT is not available on sdk 3.
			int target = IntConverter.convert( Build.VERSION.SDK );
			if ( target >= 8 )
			{
				// Return max duration only if it was explicitly set.
				if ( mVideoDurationLimit != CAPTURE_DEFAULT_PROPERTY_VALUE )
				{
					result = Integer.toString(mVideoDurationLimit);
				}
				else
				{
					Log.e("@@MoSync", "maCaptureGetProperty property " + property +
							" was not set. ");
					return MA_CAPTURE_RES_PROPERTY_VALUE_UNDEFINED;
				}
			}
			else
			{
				Log.e("@@MoSync","maCaptureGetProperty property " + property +
						" is not supported on current platform.");
				return MA_CAPTURE_RES_MAX_DURATION_NOT_SUPPORTED;
			}
		}
		else if ( property.equals( MA_CAPTURE_VIDEO_QUALITY) )
		{
			if ( mVideoQuality != CAPTURE_DEFAULT_PROPERTY_VALUE )
			{
				result = Integer.toString(mVideoQuality);
			}
			else
			{
				Log.e("@@MoSync", "maCaptureGetProperty property " + property +
						" was not set. ");
				return MA_CAPTURE_RES_PROPERTY_VALUE_UNDEFINED;
			}
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

		return MA_CAPTURE_RES_OK;
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
				return MA_CAPTURE_RES_INVALID_ACTION;
			}
		}
		return MA_CAPTURE_RES_OK;
	}

	/**
	* Save a image data object to a file.
	* Make it available through MediaStore also.
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
		Log.e("@@MoSync", "maCaptureWriteImage");

		MoSyncCaptureObject capture = m_captureObjects.get(handle);
		if ( null == capture ||  capture.getType() != CaptureType.IMAGE )
		{
			Log.e("@@MoSync", "maCaptureWriteImage: Invalid image handle: "+ handle);
			return MA_CAPTURE_RES_INVALID_HANDLE;
		}

		OutputStream fOut = null;
		File file = new File(fullPathBuffer);

        // Check if filePathBuffer already exists.
        if ( file.exists() )
        {
			Log.e("@@MoSync","maCaptureWriteImage File already exists.");
			return MA_CAPTURE_RES_FILE_ALREADY_EXISTS;
        }

        if ( !file.isAbsolute() )
        {
			Log.e("@@MoSync","maCaptureWriteImage Invalid path.");
			return MA_CAPTURE_RES_FILE_INVALID_NAME;
        }

		try {
			fOut = new FileOutputStream(file);
		} catch (FileNotFoundException e) {
			e.printStackTrace();
			return MA_CAPTURE_RES_FILE_INVALID_NAME;
		}

		Bitmap bitmap = capture.mBitmap;
		if ( bitmap != null )
		{
			try{
				// Write a compressed version of the bitmap to the specified output stream.
				bitmap.compress(Bitmap.CompressFormat.JPEG, 100, fOut);
				// Create a thumbnail for the image.
				Uri.parse(MediaStore.Images.Media.insertImage(mContentResolver,
						file.getAbsolutePath(), null, null));
				fOut.flush();
				fOut.close();
			} catch(FileNotFoundException e1)
			{
				e1.printStackTrace();
				return MA_CAPTURE_RES_FILE_INVALID_NAME;
			}catch(IOException e2)
			{
				e2.printStackTrace();
				Log.e("@@MoSync","maCaptureWriteImage Error while creating file.");
				return MA_CAPTURE_RES_FILE_INVALID_NAME;
			}
		}
		else
		{
			Log.e("@@MoSync","maCaptureWriteImage Error while creating file.");
			return MA_CAPTURE_RES_FILE_INVALID_NAME;
		}

		return MA_CAPTURE_RES_OK;
	}

	/**
	 * Construct a file from a specified Uri.
	 * @param imageUri The uri.
	 * @param activity Underlying activity.
	 * @return The file.
	 */
	private static File convertImageUriToFile (Uri imageUri, Activity activity)
	{
		Cursor cursor = null;
		try {
			String [] proj={ MediaColumns.DATA };
		    cursor = activity.managedQuery( imageUri,
		            proj, // Which columns to return
		            null,       // WHERE clause; which rows to return (all rows)
		            null,       // WHERE clause selection arguments (none)
		            null); // Order-by clause (ascending by name)
			if ( cursor == null )
				return null;
		    int file_ColumnIndex = cursor.getColumnIndexOrThrow(MediaColumns.DATA);
		    if (cursor.moveToFirst()) {
		        return new File(cursor.getString(file_ColumnIndex));
		    }
		    return null;
		} finally {
		    if (cursor != null) {
		        cursor.close();
		    }
		}
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
		MoSyncCaptureObject capture = m_captureObjects.get(handle);
		if ( null == capture ||  capture.getType() != CaptureType.VIDEO )
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
	* Get full path to a captured image.
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
		MoSyncCaptureObject capture = m_captureObjects.get(handle);
		if ( null == capture ||  capture.getType() != CaptureType.IMAGE )
		{
			Log.e("@@MoSync", "maCaptureGetImagePath: Invalid image handle: "+ handle);
			return MA_CAPTURE_RES_INVALID_HANDLE;
		}

		String result = capture.mPath;

		if( result.length( ) + 1 > bufferSize )
		{
			Log.e( "MoSync", "maCaptureGetImagePath: Buffer size " + bufferSize +
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
		MoSyncCaptureObject capture = m_captureObjects.get(handle);
		if ( null == capture )
		{
			Log.e("@@MoSync","maCaptureDestroyData: Invalid handle");
			return MA_CAPTURE_RES_INVALID_HANDLE;
		}

		m_captureObjects.remove(capture);

		return MA_CAPTURE_RES_OK;
	}

	/************************ Implementation Support ************************/

	/**
	 * Handle the result after a picture was taken( the control went back to MoSync).
	 * @param intent
	 */
	public static void handlePicture(Intent data)
	{
		File imageFile = null;
		Bitmap photo = null;

		/**
		 * Get the path of the latest saved image in MediaStore.
		 * Some versions of Android save to the MediaStore as well.
		 * We don't know what name Android will give either, so we
		 * search for this manually.
		 */
		imageFile = getLatestFile();
		// Second option:
//		imageFile = convertImageUriToFile(mImageUri, mMoSyncThread.getActivity());

		if ( imageFile == null )
		{
			Log.e("@@MoSync", "Capture error. The captured file does not exist anymore");
			return;
		}

	    String filePath = imageFile.getAbsolutePath();
		Log.e("@@MoSync", "Capture: The file path of the capture is "+ filePath);
	    mImageUri = Uri.fromFile(imageFile);

		// Get the bitmap.
	    BitmapFactory.Options mDecodingOptions = new BitmapFactory.Options() ;
	    // Return a smaller image to save memory and avoid memory issues.
	    mDecodingOptions.inSampleSize = 4;
	    photo = BitmapFactory.decodeFile(filePath, mDecodingOptions);

		if ( photo == null )
		{
			// Try to get a small thumbnail if an error occurred.
			photo = (Bitmap) data.getExtras().get("data");
		}

        // Create new handle.
        int dataHandle = mMoSyncThread.nativeCreatePlaceholder();
        // Store the image in the NativeUI image table.
		mImageTable.put(dataHandle, new ImageCache(null, photo));
		NativeUI.setImageTable(mImageTable);

		// Create and store a new CaptureObject.
		MoSyncCaptureObject capture = new MoSyncCaptureObject(CaptureType.IMAGE);
		capture.setData(mImageUri);
		capture.mBitmap = photo;
		capture.mPath = filePath;
		m_captureObjects.put(dataHandle, capture);

		// Free objects.
		photo = null;
		imageFile = null;
		capture = null;

	    // Post MoSync event.
	    postEventNotificationReceived(MA_CAPTURE_EVENT_TYPE_IMAGE, dataHandle);
	}

	/*
	 * @brief Recycles the bitmap
	 *
	 * @param	bitmap	The bitmap which is being recycled
	 */
	public static void recycleImageData(final Bitmap bitmap)
	{
		mMoSyncThread.getActivity().runOnUiThread(new Runnable()
		{
			public void run()
			{
				bitmap.recycle();
			}
		});
		return;
	}

	/**
	 * Get the latest File added to MediaStore.
	 * @return The file.
	 */
	private static File getLatestFile()
	{
		File file = null;
		// Get the latest uri added to MediaStore.
	    final String[] imageColumns = { MediaStore.Images.Media.DATA };
	    final String imageOrderBy = MediaStore.Images.Media._ID + " DESC";
	    final String imageWhere = null;
	    final String[] imageArguments = null;
	    Cursor imageCursor = mMoSyncThread.getActivity().managedQuery(
				MediaStore.Images.Media.EXTERNAL_CONTENT_URI,
				imageColumns,
				imageWhere,
				imageArguments,
				imageOrderBy);
	    if(imageCursor.moveToFirst()){
	        String path = imageCursor.getString(imageCursor.getColumnIndex(MediaStore.Images.Media.DATA));
	        file = new File(path);
	        imageCursor.close();
	    }else{
	        return null;
	    }

	    return file;
	}

	/*
	 * @brief Function that creates a Bitmap from uri.
	 * With this method we cannot obtain a bitmap in sample size.
	 * @param	uri The image uri.
	 * @return	The created Bitmap, null if it failed.
	 */
    private static Bitmap grabImage(Uri imageUri)
    {
        Bitmap bitmap = null;
        try
        {
            bitmap = MediaStore.Images.Media.getBitmap(
					mContentResolver, imageUri);
        } catch (FileNotFoundException e) {
			Log.e("@@MoSync","Capture error: failed to load capture.");
          e.printStackTrace();
        } catch (IOException e) {
			Log.e("@@MoSync","Capture error: failed to load capture.");
			e.printStackTrace();
        }
        return bitmap;
    }

	/**
	 * Handle the result after a video was recorded and the control
	 * went back to MoSync activity.
	 * @param data
	 */
	public static void handleVideo(Intent data)
	{
		String videoPath = null;

		mVideoUri = data.getData();

		if ( mVideoUri == null )
		{
			Log.e("@@MoSync","Capture error video is null");
			return;
		}

		// Get the name of the recorded video.
		String[] projection = { MediaColumns.DATA };

		Cursor cursor = mMoSyncThread.getActivity().managedQuery(
				mVideoUri, projection, null, null, null);
		int column_index = -1;
		try{
			column_index = cursor
		        .getColumnIndexOrThrow(MediaColumns.DATA);
		}catch ( IllegalArgumentException iae )
		{
			Log.e("@@MoSync","Capture Some error occured, video was not stored");
			return;
		}
		if ( cursor.moveToFirst() )
		{
			videoPath = cursor.getString(column_index);
			Log.e("@@MoSync", "Capture: video stored at " + videoPath);
		}

        // Create handle.
        int dataHandle = mMoSyncThread.nativeCreatePlaceholder();

        // Create and store a new CaptureObject.
        MoSyncCaptureObject capture = new MoSyncCaptureObject(CaptureType.VIDEO);
        capture.setData(mVideoUri);
        capture.mPath = videoPath;
		m_captureObjects.put(dataHandle, capture);

		// Post MoSync event.
		postEventNotificationReceived(MA_CAPTURE_EVENT_TYPE_VIDEO, dataHandle);
	}

	/**
	 * Handle capture cancel.
	 * Post event to the MoSync queue.
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
	 * Start video recording.
	 */
	private void recordVideo()
	{
		Log.e("@@MoSync","maCaptureAction: Video recording started");
		// Hack on Motorola: the activity is called com.motorola.Camera.Camcorder.
		Intent videoIntent = new Intent(android.provider.MediaStore.ACTION_VIDEO_CAPTURE);

		// Set properties that have assigned values.
		if ( mVideoDurationLimit != CAPTURE_DEFAULT_PROPERTY_VALUE )
		{
			videoIntent.putExtra(EXTRA_DURATION_LIMIT, mVideoDurationLimit);

		}
		// High is already set by
		if ( mVideoQuality != CAPTURE_VIDEO_QUALITY_HIGH )
		{
			videoIntent.putExtra(MediaStore.EXTRA_VIDEO_QUALITY, mVideoQuality);
		}

		// Launch the camera application.
		try{
			getActivity().startActivityForResult(videoIntent, CAPTURE_MODE_RECORD_VIDEO_REQUEST);
		}catch(ActivityNotFoundException anfe)
		{
			Log.e("@@MoSync","maCaptureAction error: There is no video recording available.");
		}
	}

	/**
	 * Start Camera activity that captures an image and returns it.
	 * Some devices (especially HTC) ignore the given file name and
	 * automatically save the image in the Gallery.
	 */
	private void takePicture()
	{
		Intent cameraIntent = new Intent(android.provider.MediaStore.ACTION_IMAGE_CAPTURE);

		/**
		 * On 1.5 documentation: "If the EXTRA_OUTPUT is not present, then a small
		 * sized image is returned as a Bitmap object in the extra field.
		 * If the EXTRA_OUTPUT is present, then the full-sized image will be written to
		 * the Uri value of EXTRA_OUTPUT."
		 * - Some devices use it completely and skip the Gallery.
		 * - Some devices ignore it completely and only use the Gallery and returns
		 * uri in intent.
		 * - Some devices save a full sized image to the Gallery, and only save a
		 * thumbnail to this location (some HTC devices).
		 */
		/**
		 * Try to save image to specified file, although some devices
		 * might ignore this setting.
		 */
		mImageUri = getTempImageUri();
		cameraIntent.putExtra(MediaStore.EXTRA_OUTPUT, mImageUri);

		// Launch the camera application.
		getActivity().startActivityForResult(cameraIntent, CAPTURE_MODE_TAKE_PICTURE_REQUEST);
	}

	/**
	 * Create a temporary file to store the image. This way the Gallery should not be used.
	 * On some phones this has no effect, that's why we check the output file path, and we
	 * delete it if necessary.
	 * @return The image Uri.
	 */
    public static Uri getTempImageUri()
    {
        File photo = null;
        Uri imgUri = null;
        try
        {
            // Place where to store captured image.
            photo = createTemporaryFile(CAPTURE_TEMP_DEFAULT_NAME, ".jpg");
            // The absolute path of the temp file will be : /mnt/sdcard/.temp/MoSyncTempFile.jpg
            imgUri = Uri.fromFile(photo);
        }
        catch(Exception e)
        {
            Log.e("@@MoSync", "Capture: Can't create file to take picture!");
        }
        return imgUri;
    }

    /**
     * Create a temporary file and return it.
     * @param part The file name.
     * @param ext The file extension.
     * @return the temporary file.
     */
	private static File createTemporaryFile(String part, String ext) throws Exception
	{
	    File tempDir = Environment.getExternalStorageDirectory();
	    tempDir = new File(tempDir.getAbsolutePath()+"/.temp/");

	    if(!tempDir.exists())
	    {
	        tempDir.mkdir();
	    }
	    return File.createTempFile(part, ext, tempDir);
	}

	/************************ Class members ************************/

	/**
	 * The MoSync thread object.
	 */
	private static MoSyncThread mMoSyncThread;

	/**
	 * The Content Resolver instance.
	 */
	private static ContentResolver mContentResolver;

    /**
     * It has access to the image resource table.
     */
    private static Hashtable<Integer, ImageCache> mImageTable;

	/**
	 * A table that contains mapping between a handle and a capture object.
	 * The capture object represents a captured image or recorded video.
	 * The key is the handle of the image or video.
	 */
	private static Hashtable<Integer,MoSyncCaptureObject> m_captureObjects =
		new Hashtable<Integer,MoSyncCaptureObject>();

	/**
	 * The current image.
	 */
	private static Uri mImageUri;

	/**
	 * The current video.
	 */
	private static Uri mVideoUri;

	/**
	 * Store the state of the last capture action.
	 * Modify if when a picture is taken.
	 */
	private static boolean mPhotoTaken = false;

	/**
	 * Capture properties: storeToGallery, videoDurationLimit, videoQuality.
	 * These properties are set before captures are taken, and are applied
	 * for each new image/video.
	 */

	/**
	 * Indicates if the captured image should be stored in Gallery.
	 * By default a captured image is stored in Gallery album.
	 * If the value is set to false then a temporary file will be used
	 * instead, and no file will be generated after the image capture.
	 */
	//private static boolean mStoreToGallery = true;

	/**
	 * Indicates the duration limit for the video recording.
	 * Value is in seconds.
	 */
	private int mVideoDurationLimit = CAPTURE_DEFAULT_PROPERTY_VALUE;

	/**
	 * Indicates the video quality of the next recordings.
	 * Note: some devices the default camera either does not support low-res video capture,
	 * or it doesn't understand/process the MediaStore.EXTRA_SIZE_LIMIT correctly.
	 */
	private int mVideoQuality = CAPTURE_DEFAULT_PROPERTY_VALUE;
}