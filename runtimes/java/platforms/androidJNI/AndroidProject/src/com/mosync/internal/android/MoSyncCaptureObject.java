package com.mosync.internal.android;


import android.net.Uri;

import com.mosync.internal.android.MoSyncCapture.CaptureType;

/**
 * Class for holding capture object information.
 * After a photo is captured a new CaptureObject is created and it
 * contains the imageUri, the path.
 * After a video is recorded a new CaptureObject is created and it
 * contains the videoUri the absolute path to it etc.
 * @author emma.
 *
 */
public class MoSyncCaptureObject
{

	/**
	 * Constructor.
	 * @param type
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
	 *
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

	public CaptureType mCaptureType;

	public String mPath = null;

	private Uri mUri = null;
}