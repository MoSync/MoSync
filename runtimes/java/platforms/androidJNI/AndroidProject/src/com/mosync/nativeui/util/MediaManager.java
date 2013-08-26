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

package com.mosync.nativeui.util;

import static com.mosync.internal.android.MoSyncHelpers.SYSLOG;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_MEDIA_EXPORT_FINISHED;
import static com.mosync.internal.generated.MAAPI_consts.MA_MEDIA_RES_IMAGE_EXPORT_FAILED;
import static com.mosync.internal.generated.MAAPI_consts.MA_MEDIA_RES_OK;
import static com.mosync.internal.generated.MAAPI_consts.MA_MEDIA_TYPE_IMAGE;
import android.graphics.Bitmap;
import android.provider.MediaStore;
import android.text.TextUtils;

import com.mosync.internal.android.MoSyncThread;
import com.mosync.nativeui.core.NativeUI;

public class MediaManager {

	static public int exportImageToPhotoGallery(final int imageHandle, final String imageName)
	{
		int returnCode = MA_MEDIA_RES_IMAGE_EXPORT_FAILED;

		final Bitmap image = NativeUI.getBitmap( imageHandle );

		if ( (null != image) &&
				!TextUtils.isEmpty(imageName) )
		{
			dispatchImageExport(image, imageHandle, imageName);
			returnCode = MA_MEDIA_RES_OK;
		}
		return returnCode;
	}

	static private void dispatchImageExport(final Bitmap image, final int imageHandle, final String imageName)
	{
		// Dispatch the image export.
		new Thread(new Runnable()
		{
			public void run()
			{
				int returnCode = MA_MEDIA_RES_OK;

				try
				{
					String resultedUrl =
							MediaStore.Images.Media.insertImage(MoSyncThread.getInstance().getActivity().getContentResolver(), image, imageName , "");

					if ( null == resultedUrl )
					{
						returnCode = MA_MEDIA_RES_IMAGE_EXPORT_FAILED;
					}
				}
				catch (Exception e)
				{
					SYSLOG(e.toString());
					returnCode = MA_MEDIA_RES_IMAGE_EXPORT_FAILED;
				}
				finally
				{
					postMediaEvent(MA_MEDIA_TYPE_IMAGE, imageHandle, returnCode);
				}
			}
		}).start();
	}

	static public void postMediaEvent(int mediaType, int mediaHandle, int returnCode)
	{
		int[] event = new int[4];
		event[0] = EVENT_TYPE_MEDIA_EXPORT_FINISHED;
		event[1] = mediaType;
		event[2] = mediaHandle;
		event[3] = returnCode;
		MoSyncThread.getInstance().postEvent(event);
	}
}
