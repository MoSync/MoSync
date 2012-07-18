package com.mosync.internal.android;

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
	 * Capture type.
	 */
	private CaptureType mCaptureType;

	/**
	 * Path to captured file.
	 */
	private String mPath = null;

	// TODO: Not used. Delete.
	// The image uri.
	//private Uri mUri = null;

	// The image bitmap. Only for IMAGE type.
	//public Bitmap mBitmap = null;

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
	 * Get the path to the captured object.
	 */
	public String getPath()
	{
		return mPath;
	}

	/**
	 * Set the path to the captured object.
	 */
	public void setPath(String path)
	{
		mPath = path;
	}

	// TODO: Not used. Delete.
//	/**
//	 * Set image Uri.
//	 * @param uri
//	 */
//	private void setData(Uri uri)
//	{
//		mUri = uri;
//	}
//
//	/**
//	 * Returns the data of this object.
//	 */
//	private Uri getData()
//	{
//		return mUri;
//	}
}