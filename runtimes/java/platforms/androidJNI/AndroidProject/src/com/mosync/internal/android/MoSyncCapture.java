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

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_CAPTURE;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_ACTION_RECORD_VIDEO;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_ACTION_TAKE_PICTURE;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_EVENT_TYPE_CANCEL;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_EVENT_TYPE_IMAGE;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_EVENT_TYPE_VIDEO;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_MAX_DURATION;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_FILE_ALREADY_EXISTS;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_FILE_INVALID_NAME;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_INVALID_ACTION;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_INVALID_HANDLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_INVALID_PROPERTY;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_INVALID_PROPERTY_VALUE;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_INVALID_STRING_BUFFER_SIZE;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_MAX_DURATION_NOT_SUPPORTED;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_OK;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_RES_PROPERTY_VALUE_UNDEFINED;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_VIDEO_QUALITY;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_VIDEO_QUALITY_HIGH;
import static com.mosync.internal.generated.MAAPI_consts.MA_CAPTURE_VIDEO_QUALITY_LOW;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.OutputStream;
import java.nio.channels.FileChannel;
import java.util.Date;
import java.util.Hashtable;

import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.provider.MediaStore.MediaColumns;
import android.util.Log;

import com.mosync.internal.android.MoSyncThread.ImageCache;
import com.mosync.java.android.MoSync;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;

/**
 * Class that implements Capture API.
 * @author emma tresanszki
 *
 * TODO: Consider making this class non-static (move class members
 * to be instance members).
 */
public class MoSyncCapture
{
	/**
	 * Type of capture.
	 */
	enum CaptureType
	{
		IMAGE,
		VIDEO
	}

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
	public static final String CAPTURE_TEMP_DEFAULT_NAME = "MoSyncImageCaptureFile";

	private static int sHandleCounter = 0;

	/**
	 * The MoSync thread object.
	 */
	private static MoSyncThread sMoSyncThread;

	// TODO: Not used delete.
	/**
	 * The Content Resolver instance.
	 */
	//private static ContentResolver sContentResolver;

	// TODO: Not used delete.
	/**
	 * It has access to the image resource table.
	 */
	//private static Hashtable<Integer, ImageCache> sImageTable;

	/**
	 * A table that contains mapping between a handle and a capture object.
	 * The capture object represents a captured image or recorded video.
	 * The key is the handle of the image or video.
	 */
	private static Hashtable<Integer,MoSyncCaptureObject> sCaptureObjects =
		new Hashtable<Integer,MoSyncCaptureObject>();

	/**
	 * The current image.
	 */
	private static Uri sImageUri;

	/**
	 * The current video.
	 */
	private static Uri sVideoUri;

	/**
	 * Time stamp used to determine when an image was taken.
	 */
	private static long sImageCaptureStarted;

	/**
	 * Store the state of the last capture action.
	 * Modify if when a picture is taken.
	 */
	// TODO: Not used, can be deleted?
	//private static boolean sPhotoTaken = false;

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
	// TODO: Remove if not used.
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

	/**
	 * Constructor Capture API.
	 * @param thread The underlying MoSync thread.
	 */
	public MoSyncCapture(MoSyncThread thread, Hashtable<Integer, ImageCache> imageTable)
	{
		sMoSyncThread = thread;

		// TODO: Not used delete.
//		sImageTable = imageTable;
//		sContentResolver = sMoSyncThread.getActivity().getContentResolver();
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
		try
		{
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
		}
		catch(Exception ex)
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
		sMoSyncThread.mMemDataSection.position( valueBuffer );
		sMoSyncThread.mMemDataSection.put( byteArray );
		sMoSyncThread.mMemDataSection.put( (byte)0 );

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
		try
		{
			MoSyncCaptureObject capture = sCaptureObjects.get(handle);
			if (null == capture || capture.getType() != CaptureType.IMAGE)
			{
				Log.e("@@MoSync", "maCaptureWriteImage: Invalid image handle: "+ handle);
				return MA_CAPTURE_RES_INVALID_HANDLE;
			}

			File sourceFile = new File(capture.getPath());
			File destFile = new File(fullPathBuffer);

			if (!sourceFile.exists())
			{
				Log.e("@@MoSync","maCaptureWriteImage File already exists.");
				return MA_CAPTURE_RES_INVALID_HANDLE;
			}

			// Check if filePathBuffer already exists.
			// TODO: Should this really be an error?
			if (destFile.exists())
			{
				Log.e("@@MoSync","maCaptureWriteImage File already exists.");
				return MA_CAPTURE_RES_FILE_ALREADY_EXISTS;
			}

			if (!destFile.isAbsolute())
			{
				Log.e("@@MoSync","maCaptureWriteImage Invalid path.");
				return MA_CAPTURE_RES_FILE_INVALID_NAME;
			}

			// Write image by copying the image file to the destination.
			boolean success = copyFile(sourceFile, destFile);
			if (!success)
			{
				return MA_CAPTURE_RES_FILE_INVALID_NAME;
			}

		}
		catch (Exception ex)
		{
			ex.printStackTrace();
			return MA_CAPTURE_RES_FILE_INVALID_NAME;
		}

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
		MoSyncCaptureObject capture = sCaptureObjects.get(handle);
		if ( null == capture ||  capture.getType() != CaptureType.VIDEO )
		{
			Log.e("@@MoSync", "maCaptureGetVideoPath: Invalid video handle: "+ handle);
			return MA_CAPTURE_RES_INVALID_HANDLE;
		}

		String result = capture.getPath();

		if( result.length( ) + 1 > bufferSize )
		{
			Log.e( "MoSync", "maCaptureGetVideoPath: Buffer size " + bufferSize +
				" too short to hold buffer of size: " + result.length( ) + 1 );
			return MA_CAPTURE_RES_INVALID_STRING_BUFFER_SIZE;
		}

		byte[] byteArray = result.getBytes();

		// Write string to MoSync memory.
		sMoSyncThread.mMemDataSection.position( buffer );
		sMoSyncThread.mMemDataSection.put( byteArray );
		sMoSyncThread.mMemDataSection.put( (byte)0 );

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
		MoSyncCaptureObject capture = sCaptureObjects.get(handle);
		if ( null == capture ||  capture.getType() != CaptureType.IMAGE )
		{
			Log.e("@@MoSync", "maCaptureGetImagePath: Invalid image handle: "+ handle);
			return MA_CAPTURE_RES_INVALID_HANDLE;
		}

		String result = capture.getPath();

		if( result.length( ) + 1 > bufferSize )
		{
			Log.e( "MoSync", "maCaptureGetImagePath: Buffer size " + bufferSize +
				" too short to hold buffer of size: " + result.length( ) + 1 );
			return MA_CAPTURE_RES_INVALID_STRING_BUFFER_SIZE;
		}

		byte[] byteArray = result.getBytes();

		// Write string to MoSync memory.
		sMoSyncThread.mMemDataSection.position( buffer );
		sMoSyncThread.mMemDataSection.put( byteArray );
		sMoSyncThread.mMemDataSection.put( (byte)0 );

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
		MoSyncCaptureObject capture = sCaptureObjects.get(handle);
		if ( null == capture )
		{
			Log.e("@@MoSync","maCaptureDestroyData: Invalid handle");
			return MA_CAPTURE_RES_INVALID_HANDLE;
		}

		// TODO: Delete code not used.
//		// Explicitly mark bitmap as deleted if not null.
//		if (null != capture.mBitmap)
//		{
//			recycleBitmap(capture.mBitmap);
//		}

		// Delete temporary file if this is an image.
		if (CaptureType.IMAGE == capture.getType())
		{
			new File(capture.getPath()).delete();
		}

		// Remove capture entry from table.
		sCaptureObjects.remove(capture);

		return MA_CAPTURE_RES_OK;
	}

	/************************ Implementation Support ************************/

	/**
	 * @return The Activity object.
	 */
	private Activity getActivity() {
		return sMoSyncThread.getActivity();
	}

	/**
	 * Handle the result after a picture was taken (the control went back to MoSync).
	 * @param intent
	 */
	public static void handlePicture(Intent intent)
	{
		// There are four alternative ways used to obtain the captured image:
		//
		// 1) Access the image using the uri passed to the camera intent.
		// 2) Access the image using the uri passed with the intent.
		// 3) Get the last image stored in the media store (image gallery).
		// 4) Use the "small bitmap" passed with the intent.
		//
		// The reason for trying all these ways is that different devices
		// work differently and have different quirks.

		MoSyncCaptureObject capture = null;

		// Try to save the image uri that was passed to the Camera Activity.
		capture = saveImageUsingURI(sImageUri, true);

		// Try to use the image referred to by the intent.
		if (null == capture && null != intent)
		{
			try
			{
				Uri imageUri = intent.getData();
				Bitmap bitmap = MediaStore.Images.Media.getBitmap(
					sMoSyncThread.getActivity().getContentResolver(),
					imageUri);
				if (null != bitmap)
				{
					capture = saveBitmapAndCreateCaptureObject(bitmap);
				}
			}
			catch (Exception ex)
			{
				ex.printStackTrace();
			}
		}

		// Try getting the last saved image in the gallery (MediaStore).
		if (null == capture)
		{
			Uri imageUri = getLatestFileFromMediaStore();
			capture = saveImageUsingURI(imageUri, false);
		}

		// Try using the small bitmap that comes with the intent.
		if (null == capture && null != intent)
		{
			// Get the small bitmap.
			Bundle bundle = intent.getExtras();
			if (null != bundle)
			{
				Bitmap bitmap = (Bitmap) bundle.get("data");
				if (null != bitmap)
				{
					capture = saveBitmapAndCreateCaptureObject(bitmap);
				}
			}
		}

		if (null != capture)
		{
			// Create new handle for the capture object.
			//int captureHandle = sMoSyncThread.nativeCreatePlaceholder();

			// TODO: We use a local handle counter for now, to avoid
			// using up the placeholder handle space unnecessarily.
			// It is tricky to release a handle created with
			// nativeCreatePlaceholder as the code stands now.
			// Look into this further.
			int captureHandle = (++ sHandleCounter);
			sCaptureObjects.put(captureHandle, capture);

			// TODO: If we wish to add the bitmap to the image table,
			// here is the old code for doing that. I this case we
			// need to also look at what happens when an image is
			// destroyed, and remove the image when maCaptureDestroyData
			// is called.

//			// Store the image in the NativeUI image table.
//			sImageTable.put(dataHandle, new ImageCache(null, photo));
//			NativeUI.setImageTable(sImageTable);

			// Post MoSync event.
			postEventNotificationReceived(
				MA_CAPTURE_EVENT_TYPE_IMAGE,
				captureHandle);
		}
		else
		{
			// Post error result (using MA_CAPTURE_EVENT_TYPE_CANCEL for this).
			// TODO: Should there be an MA_CAPTURE_EVENT_TYPE_ERROR event?
			postEventNotificationReceived(MA_CAPTURE_EVENT_TYPE_CANCEL, -1);
		}
	}

	private static MoSyncCaptureObject saveImageUsingURI(
		Uri imageUri,
		boolean isTemporary)
	{
		if (null == imageUri) { return null; }

		try
		{
			File imageFile = new File(imageUri.getPath());
			if (null == imageFile || imageFile.length() < 1)
			{
				return null;
			}

			// Get the bitmap and create a down-sized smaller version
			// to save memory and avoid memory issues.
			BitmapFactory.Options mDecodingOptions =
				new BitmapFactory.Options();
			mDecodingOptions.inSampleSize = 4;
			Bitmap bitmap = BitmapFactory.decodeFile(
				imageFile.getAbsolutePath(),
				mDecodingOptions);
			if (null == bitmap)
			{
				return null;
			}

			// Save bitmap, creates a capture object (or returns null).
			MoSyncCaptureObject capture = saveBitmapAndCreateCaptureObject(bitmap);

			// Release the bitmap.
			recycleBitmap(bitmap);

			// Delete original image file if temporary.
			if (isTemporary)
			{
				imageFile.delete();
			}

			return capture;
		}
		catch (Exception ex)
		{
			ex.printStackTrace();
			return null;
		}
	}

	private static MoSyncCaptureObject saveBitmapAndCreateCaptureObject(Bitmap bitmap)
	{
		if (null == bitmap) { return null; }

		// Save bitmap.
		String imageFilePath = getTempImageUri().getPath();
		boolean success = saveBitmapToFile(bitmap, imageFilePath);
		if (!success) { return null; }

		// Create a capture object with the path.
		MoSyncCaptureObject capture =
			new MoSyncCaptureObject(CaptureType.IMAGE);
		capture.setPath(imageFilePath);

		return capture;
	}

	private static boolean saveBitmapToFile(Bitmap bitmap, String path)
	{
		if (null == bitmap) { return false; }

		try
		{
			File file = new File(path);
			OutputStream out = new FileOutputStream(file);
			boolean success = bitmap.compress(Bitmap.CompressFormat.JPEG, 90, out);
			out.flush();
			out.close();

			return success;
		}
		catch (Exception ex)
		{
			ex.printStackTrace();
			return false;
		}
	}

	/**
	 * Get the path to the latest file added to MediaStore.
	 * @return The file path.
	 */
	private static Uri getLatestFileFromMediaStore()
	{
		// Get the latest camera shot from the MediaStore.
		final String[] imageColumns = {
			MediaStore.Images.Media.DATA,
			MediaStore.Images.Media.DATE_TAKEN };
		final String imageWhere = null;
		final String[] imageArguments = null;
		final String imageOrderBy = MediaStore.Images.Media.DATE_TAKEN + " DESC";

		Cursor imageCursor = sMoSyncThread.getActivity().managedQuery(
			MediaStore.Images.Media.EXTERNAL_CONTENT_URI,
			imageColumns,
			imageWhere,
			imageArguments,
			imageOrderBy);

		// Move to the first element (latest camera shot).
		if (imageCursor.moveToFirst())
		{
			// Check that it is newer than the time stamp
			// taken when launching the camera activity.
			long dateTaken = imageCursor.getLong(
				imageCursor.getColumnIndex(
					MediaStore.Images.Media.DATE_TAKEN));
			if (dateTaken > sImageCaptureStarted)
			{
				// It is newer. Get the path and return a file object.
				String path = imageCursor.getString(
					imageCursor.getColumnIndex(
						MediaStore.Images.Media.DATA));
				// We should not call close on a managed cursor.
				//imageCursor.close();
				return Uri.parse("file://" + path);
			}
		}

		return null;
	}

	/**
	 * Handle the result after a video was recorded and the control
	 * went back to MoSync activity.
	 * @param data
	 */
	public static void handleVideo(Intent data)
	{
		String videoPath = null;

		sVideoUri = data.getData();

		if ( sVideoUri == null )
		{
			Log.e("@@MoSync","Capture error video is null");
			return;
		}

		// Get the name of the recorded video.
		String[] projection = { MediaColumns.DATA };

		Cursor cursor = sMoSyncThread.getActivity().managedQuery(
				sVideoUri, projection, null, null, null);
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
		}

		// Create handle.
		//int dataHandle = sMoSyncThread.nativeCreatePlaceholder();
		// TODO: We use a local handle counter for now, to avoid
		// using up the placeholder handle space unnecessarily.
		// It is tricky to release a handle created with
		// nativeCreatePlaceholder as the code stands now.
		// Look into this further.
		int dataHandle = (++ sHandleCounter);

		// Create and store a new CaptureObject.
		MoSyncCaptureObject capture = new MoSyncCaptureObject(CaptureType.VIDEO);
		capture.setPath(videoPath);
		sCaptureObjects.put(dataHandle, capture);

		// Post MoSync event.
		postEventNotificationReceived(MA_CAPTURE_EVENT_TYPE_VIDEO, dataHandle);
	}

	/**
	 * Handle capture cancel.
	 * Post event to the MoSync queue.
	 */
	public static void handleCaptureCanceled()
	{
		// Clear out the image uri.
		sImageUri = null;

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

		sMoSyncThread.postEvent(event);
	}

	/**
	 * Start video recording.
	 */
	private void recordVideo()
	{
		Log.i("@@MoSync","maCaptureAction: Video recording started");
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
			getActivity().startActivityForResult(videoIntent, MoSync.CAPTURE_MODE_RECORD_VIDEO_REQUEST);
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
		Intent cameraIntent = new Intent(
			android.provider.MediaStore.ACTION_IMAGE_CAPTURE);

		/*
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
		/*
		 * Try to save image to specified file, although some devices
		 * might ignore this setting.
		 */
		sImageUri = getTempImageUri();
		if (null != sImageUri)
		{
			cameraIntent.putExtra(MediaStore.EXTRA_OUTPUT, sImageUri);
		}

		// Used if we need to get the image from the media store.
		sImageCaptureStarted = new Date().getTime();

		// Launch the camera application.
		getActivity().startActivityForResult(
			cameraIntent,
			MoSync.CAPTURE_MODE_TAKE_PICTURE_REQUEST);
	}

	/**
	 * Create a temporary file to store the image. This way the Gallery should not be used.
	 * On some phones this has no effect, that's why we check the output file path, and we
	 * delete it if necessary.
	 * @return The image Uri.
	 */
	private static Uri getTempImageUri()
	{
		try
		{
			File storageDir = Environment.getExternalStorageDirectory();
			File tempDir = new File(storageDir.getAbsolutePath() + "/tmp/");
			if (!tempDir.exists())
			{
				tempDir.mkdir();
			}

			File tempFile = File.createTempFile(
				CAPTURE_TEMP_DEFAULT_NAME, ".jpg", tempDir);

			return Uri.fromFile(tempFile);
		}
		catch (Exception ex)
		{
			Log.e("@@MoSync", "Capture: Can't create file to take picture!");
			ex.printStackTrace();
			return null;
		}
	}

	private static boolean copyFile(File sourceFile, File destFile)
	{
		try
		{
			FileChannel source = null;
			FileChannel destination = null;

			if (!destFile.exists())
			{
				destFile.createNewFile();
			}

			source = new FileInputStream(sourceFile).getChannel();
			destination = new FileOutputStream(destFile).getChannel();
			destination.transferFrom(source, 0, source.size());

			if (source != null) { source.close(); }
			if (destination != null) { destination.close(); }
		}
		catch (Exception ex)
		{
			return false;
		}

		return true;
	}

	/*
	 * @brief Recycles the bitmap
	 *
	 * @param	bitmap	The bitmap which is being recycled
	 */
	private static void recycleBitmap(final Bitmap bitmap)
	{
		sMoSyncThread.getActivity().runOnUiThread(new Runnable()
		{
			public void run()
			{
				bitmap.recycle();
			}
		});
		return;
	}

	// TODO: Code is not used, delete commented out code when not needed.
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
	/*
	int maCaptureWriteImage(int handle, String fullPathBuffer, int fullPathBufSize)
	{
		Log.e("@@MoSync", "maCaptureWriteImage");

		MoSyncCaptureObject capture = sCaptureObjects.get(handle);
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

		try
		{
			fOut = new FileOutputStream(file);
		}
		catch (FileNotFoundException e)
		{
			e.printStackTrace();
			return MA_CAPTURE_RES_FILE_INVALID_NAME;
		}

		Bitmap bitmap = capture.mBitmap;
		if ( bitmap != null )
		{
			try
			{
				// Write a compressed version of the bitmap to the specified output stream.
				bitmap.compress(Bitmap.CompressFormat.JPEG, 100, fOut);
				// Create a thumbnail for the image.
				Uri.parse(MediaStore.Images.Media.insertImage(sContentResolver,
						file.getAbsolutePath(), null, null));
				fOut.flush();
				fOut.close();
			}
			catch(FileNotFoundException e1)
			{
				e1.printStackTrace();
				return MA_CAPTURE_RES_FILE_INVALID_NAME;
			}
			catch(IOException e2)
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
	*/

	// TODO: Method is not used, delete commented out code when not needed.
	/**
	 * Handle the result after a picture was taken( the control went back to MoSync).
	 * @param intent
	 */
	/*
	public static void original_version_of_handlePicture(Intent data)
	{
		File imageFile = null;
		Bitmap photo = null;

		// Get the path of the latest saved image in MediaStore.
		// Some versions of Android save to the MediaStore as well.
		// We don't know what name Android will give either, so we
		// search for this manually.

		imageFile = getLatestFile();
		// Second option:
		// imageFile = convertImageUriToFile(mImageUri, mMoSyncThread.getActivity());

		if ( imageFile == null )
		{
			Log.e("@@MoSync", "Capture error. The captured file does not exist anymore");
			return;
		}

		String filePath = imageFile.getAbsolutePath();
		Log.e("@@MoSync", "Capture: The file path of the capture is "+ filePath);
		sImageUri = Uri.fromFile(imageFile);

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
		int dataHandle = sMoSyncThread.nativeCreatePlaceholder();
		// Store the image in the NativeUI image table.
		sImageTable.put(dataHandle, new ImageCache(null, photo));
		NativeUI.setImageTable(sImageTable);

		// Create and store a new CaptureObject.
		MoSyncCaptureObject capture = new MoSyncCaptureObject(CaptureType.IMAGE);
		capture.setData(sImageUri);
		capture.mBitmap = photo;
		capture.mPath = filePath;
		sCaptureObjects.put(dataHandle, capture);

		// Free objects.
		photo = null;
		imageFile = null;
		capture = null;

		// Post MoSync event.
		postEventNotificationReceived(MA_CAPTURE_EVENT_TYPE_IMAGE, dataHandle);
	}
	*/

	// TODO: Method is not used, delete commented out code when not needed.
	/**
	 * Construct a file from a specified Uri.
	 * @param imageUri The uri.
	 * @param activity Underlying activity.
	 * @return The file.
	 */
	/*
	private static File convertImageUriToFile (Uri imageUri, Activity activity)
	{
		Cursor cursor = null;
		try {
			String [] proj={ MediaColumns.DATA };
			cursor = activity.managedQuery( imageUri,
					proj,  // Which columns to return
					null,  // WHERE clause; which rows to return (all rows)
					null,  // WHERE clause selection arguments (none)
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
	*/

	// TODO: Method is not used, delete commented out code when not needed.
	/**
	 * Create a temporary file and return it.
	 * @param part The file name.
	 * @param ext The file extension.
	 * @return the temporary file.
	 */
	/*
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
	*/

	// TODO: Method is not used, delete commented out code when not needed.
	/**
	 * Create a temporary file to store the image. This way the Gallery should not be used.
	 * On some phones this has no effect, that's why we check the output file path, and we
	 * delete it if necessary.
	 * @return The image Uri.
	 */
	/*
	public static Uri original_version_of_getTempImageUri()
	{
		try
		{
			// Place where to store captured image.
			File photo = createTemporaryFile(CAPTURE_TEMP_DEFAULT_NAME, ".jpg");
			// The absolute path of the temp file will have this format:
			// /mnt/sdcard/.temp/MoSyncTempFile-358002145.jpg
			Uri imgUri = Uri.fromFile(photo);
			return imgUri;
		}
		catch (Exception e)
		{
			Log.e("@@MoSync", "Capture: Can't create file to take picture!");
			return null;
		}
	}
	*/

	// TODO: Method is not used, delete commented out code when not needed.
	/*
	 * @brief Function that creates a Bitmap from uri.
	 * With this method we cannot obtain a bitmap in sample size.
	 * @param	uri The image uri.
	 * @return	The created Bitmap, null if it failed.
	 */
	/*
	private static Bitmap grabImage(Uri imageUri)
	{
		Bitmap bitmap = null;
		try
		{
			bitmap = MediaStore.Images.Media.getBitmap(
					sContentResolver, imageUri);
		} catch (FileNotFoundException e) {
			Log.e("@@MoSync","Capture error: failed to load capture.");
		  e.printStackTrace();
		} catch (IOException e) {
			Log.e("@@MoSync","Capture error: failed to load capture.");
			e.printStackTrace();
		}
		return bitmap;
	}
	*/
}