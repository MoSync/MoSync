/* Copyright (C) 2012 MoSync AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

package com.mosync.internal.android;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_IMAGE_PICKER;

import java.io.FileNotFoundException;
import java.util.Hashtable;

import android.content.ContentResolver;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.util.Log;

import com.mosync.internal.android.MoSyncThread.ImageCache;
import com.mosync.java.android.MoSync;

/**
 * Helper class for maImagePickerOpen syscall.
 * @author emma tresanszki
 *
 */
public class MoSyncImagePicker {

	// Events for when the Ok or Cancel button are hit.
	final static int PICKER_CANCELED = 0;
	final static int PICKER_READY = 1;

	//-------------------------- IMPLEMENTATION --------------------------//

	/**
	 * Constructor.
	 * @param thread The MoSync thread.
	 * @param imageTable The bitmap table.
	 */
	public MoSyncImagePicker(MoSyncThread thread,Hashtable<Integer, ImageCache> imageTable)
	{
		mMoSyncThread = thread;
		mImageTable = imageTable;
	}

	/**
	 * Launch the image picker activity.
	 */
	public void loadGallery()
	{
        Intent intent = new Intent();
        intent.setType("image/*");
        intent.setAction(Intent.ACTION_GET_CONTENT);

        mMoSyncThread.getActivity().startActivityForResult(intent, MoSync.PICK_IMAGE_REQUEST);
	}

	/**
	 * Handle the result after a picture was picked(the control went back to MoSync).
	 * @param intent
	 */
	public static void handleSelectedPicture(Intent data)
	{
		Uri pictureUri = data.getData();

		if (pictureUri == null)
		{
			Log.e("@@MoSync",
					"maImagePickerOpen Error: cannot get image,some error occured.");
			return;
		}
		else
		{
			ContentResolver cr = mMoSyncThread.getActivity()
					.getContentResolver();

			try {
				/**
				 * Decoding options used for bitmaps. First get the image
				 * dimensions. Based on the image size perform a scaling.
				 */
				BitmapFactory.Options bfo = new BitmapFactory.Options();
				bfo.inJustDecodeBounds = true;
				bfo.inDither = false;
				bfo.inPreferredConfig = Bitmap.Config.RGB_565;

				BitmapFactory.decodeStream(cr.openInputStream(pictureUri),
						null, bfo);

				// Calculate sample size to keep image under maxFileSize.
				int maxFileSize = 1572864; // in bytes
				int sampleSize = 1;
				long fileSize = 2 * (bfo.outWidth / sampleSize) * (bfo.outHeight / sampleSize);
				while (fileSize > maxFileSize)
				{
					sampleSize++;
					fileSize = 2 * (bfo.outWidth / sampleSize)* (bfo.outHeight / sampleSize);
				}

				/**
				 * Decode to a smaller image to save memory and run faster.
				 * Decode image using calculated sample size.
				 */
				bfo.inSampleSize = sampleSize;
				bfo.inJustDecodeBounds = false;
				Bitmap imageBitmap = BitmapFactory.decodeStream(
						cr.openInputStream(pictureUri), null, bfo);

				if (imageBitmap != null)
				{
					// Get the handle of the selected item and post event.
					postImagePickerReady(getSelectedImageHandle(imageBitmap));
				}
				else
				{
					Log.i("@@MoSync",
							"maImagePickerOpen Error: cannot decode bitmap");
					return;
				}

			} catch (FileNotFoundException e)
			{
				e.printStackTrace();
				Log.i("@@MoSync",
						"maImagePickerOpen Error: cannot find bitmap");
			}
		}
	}

    /**
     * Get the handle of the selected item.
     * Further, post it in a EVENT_TYPE_IMAGE_PICKER event.
     * @return The new handle.
     */
    private static int getSelectedImageHandle(Bitmap imageBitmap)
    {
        // Create handle.
        int dataHandle = mMoSyncThread.nativeCreatePlaceholder();

		mImageTable.put(dataHandle, new ImageCache(null, imageBitmap));

        return dataHandle;
    }

	/**
	 * Handle the result after a picture was picked( the control went back to MoSync).
	 * @param intent
	 */
	public static void handleCancelSelectPicture()
	{
		postImagePickerCanceled();
	}

	/**
	 * Post event to MoSync queue.
	 * @param imageHandle The image handle of the selected image.
	 */
	private static void postImagePickerReady(int imageHandle)
	{
		int[] event = new int[3];
		event[0] = EVENT_TYPE_IMAGE_PICKER;
		event[1] = PICKER_READY;
		// If Cancel is clicked, the handle is -1.
		event[2] = imageHandle;

		mMoSyncThread.postEvent(event);
	}

	/**
	 * Post event to MoSync queue.
	 */
	private static void postImagePickerCanceled()
	{
		int[] event = new int[3];
		event[0] = EVENT_TYPE_IMAGE_PICKER;
		event[1] = PICKER_CANCELED;
		// If Cancel is clicked, the handle is -1.
		event[2] = -1;

		mMoSyncThread.postEvent(event);
	}

    //--------------------------   Members   --------------------------//

	/**
	 * The MoSync thread object.
	 */
    private static MoSyncThread mMoSyncThread;

    /**
     * It has access to the image resource table.
     */
    private static Hashtable<Integer, ImageCache> mImageTable;
}