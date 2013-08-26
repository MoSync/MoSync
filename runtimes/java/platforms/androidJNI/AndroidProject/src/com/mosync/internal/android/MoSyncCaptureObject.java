/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

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