package com.mosync.internal.android;


import android.graphics.Bitmap;
import android.net.Uri;

import com.mosync.internal.android.MoSyncCapture.CaptureType;

/**
 * Class for holding capture object information.
 * After a photo is captured a new CaptureObject is created and it
 * contains the imageUri, the path.
 * After a video is recorded a new CaptureObject is created and it
 * contains the videoUri, the absolute path to it etc.
 * @author emma.
 *
 */
public class MoSyncCaptureObject
{

	/**
	 * Constructor.
	 * @param type The capture type.
	 */
	public MoSyncCaptureObject(CaptureType type)
	{
		mCaptureType = type;
	}

	/**
	 * Get the type of the capture.
	 * @return IMAGE or VIDE).
	 */
	public CaptureType getType()
	{
		return mCaptureType;
	}

	/**
	 * Set image Uri.
	 * @param uri
	 */
	public void setData(Uri uri)
	{
		mUri = uri;
	}

	/**
	 * Returns the data of this object.
	 */
	public Uri getData()
	{
		return mUri;
	}

	/************************ Class members ************************/
	// The capture type.
	public CaptureType mCaptureType;

	// Store the path also so we won't need to query MediaSource each time.
	public String mPath = null;

	// The image uri.
	private Uri mUri = null;

	// The image bitmap. Only for IMAGE type.
	public Bitmap mBitmap = null;
}