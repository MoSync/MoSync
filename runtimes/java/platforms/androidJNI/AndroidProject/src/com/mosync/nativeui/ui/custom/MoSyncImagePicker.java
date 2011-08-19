/* Copyright (C) 2011 MoSync AB

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

package com.mosync.nativeui.ui.custom;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_IMAGE_PICKER;

import java.io.File;
import java.io.FilenameFilter;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.res.TypedArray;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.Gallery;
import android.widget.ImageView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ImageView.ScaleType;
import android.widget.LinearLayout;
import android.widget.Toast;

import com.mosync.internal.android.MoSyncThread;
import com.mosync.java.android.R;


/*
 *  Helpers for the maOpenImageBrowser syscall.
 */
public class MoSyncImagePicker
{
	// Events for when the Ok or Cancel button are hit.
	final int PICKER_CANCELED = 0;
	final int PICKER_READY = 1;

	//-------------------------- IMPLEMENTATION --------------------------//

	/**
	 * Constructor.
	 * @param thread The MoSync thread.
	 */
	public MoSyncImagePicker(MoSyncThread thread)
	{
		mMoSyncThread = thread;
	}

	/*
	 * Open a custom dialog with a gallery view and preview
	 * for the selected item.
	 */
	public void loadGallery(final String title)
	{
		// Initialize the selected image handle.
		mImageHandle = -1;

		AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
		builder.setCancelable(true);
		builder.setTitle(title);

		builder.setNegativeButton("Cancel",
		        new DialogInterface.OnClickListener() {

			        @Override
			        public void onClick(DialogInterface dialog, int which)
			        {
				        postImagePickerEvent(PICKER_CANCELED);
			        }
		        });

		builder.setPositiveButton("Ok", new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int which)
			{
				postImagePickerEvent(PICKER_READY);
			}
		});

		// Get the images.
		if ( getImages() )
		{

			// Provide a linear layout with a gallery and a preview of the
			// selected item.
			LinearLayout layout = new LinearLayout(getActivity());
			layout.setOrientation(LinearLayout.VERTICAL);
			layout.setVerticalGravity(Gravity.CENTER);
			layout.setHorizontalGravity(Gravity.CENTER);
			layout.setLayoutParams(new LinearLayout.LayoutParams(
			        LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));

			Gallery gallery = new Gallery(getActivity());
			gallery.setLayoutParams(new LayoutParams(LayoutParams.FILL_PARENT,
			        LayoutParams.WRAP_CONTENT));

			// Fill the gallery view with images.
			gallery.setAdapter(new CustomAdapter(getActivity()));
			layout.addView(gallery);

			final ImageView preview = new ImageView(getActivity());
			preview.setScaleType(ScaleType.FIT_XY);
			preview.setLayoutParams(new LayoutParams(300,250));
//			preview.setLayoutParams(new LayoutParams(LayoutParams.FILL_PARENT,
//					LayoutParams.FILL_PARENT));

            Bitmap bitmap = BitmapFactory.decodeFile(mImagePaths[0]);
            preview.setImageBitmap(bitmap);

			layout.addView(preview);

			gallery.setOnItemClickListener(new OnItemClickListener() {

				@Override
				public void onItemClick(AdapterView<?> arg0, View v,
				        int position, long id)
				{
					Toast.makeText(getActivity(), mImageFiles[position].getName() ,
					        Toast.LENGTH_SHORT).show();

					// Refresh the preview image.
		            Bitmap bitmap = BitmapFactory.decodeFile(mImagePaths[position]);
		            preview.setImageBitmap(bitmap);

		            // Save the handle of the selected item as an event.
		            mImageHandle = getSelectedImageHandle();
				}
			});

			builder.setView(layout);
		}
		else
		{
			builder.setMessage("No images were found on the device!");
		}
		AlertDialog alertDialog = builder.create();

		// Display the dialog until user provides a selection, or cancels the
		// request.
		alertDialog.show();
	}

	/**
	 * Post a message to the MoSync event queue.
	 * The state is: Ready or Canceled.
	 */
	private void postImagePickerEvent(int state)
	{
		int[] event = new int[3];
		event[0] = EVENT_TYPE_IMAGE_PICKER;
		event[1] = state;
		// If Cancel is clicked, the handle is -1.
		event[2] = getSelectedImageHandle();

		mMoSyncThread.postEvent(event);
	}

	/*
	 * Get the list of all images on the device.
	 */
	private boolean getImages()
	{
		// Check for files that are directly into the sd card
		File imagesDir = new File("/sdcard/pics/"); // Environment.getDataDirectory();

		mImageFiles = imagesDir.listFiles(new FilenameFilter(){

			@Override
            public boolean accept(File dir, String filename)
            {
				return ((filename.endsWith(".jpg"))||(filename.endsWith(".jpeg"))||(filename.endsWith(".png")));
            }
		});

		if (mImageFiles != null )
		{
			mImagePaths =  new String[mImageFiles.length];

			// Load items.
			ProgressDialog progress = ProgressDialog.show(getActivity(), "", "Please wait..");
			progress.setCancelable(false);

			for(int i = 0 ; i< mImageFiles.length; i++)
			{
				mImagePaths[i] = mImageFiles[i].getAbsolutePath();
			}
			progress.dismiss();
			return true;
		}
		else
		{
			return false;
		}
	}

	/**
	 * @return The Activity object.
	 */
	private Activity getActivity()
	{
		return mMoSyncThread.getActivity();
	}

    /**
     * Get the handle of the selected item.
     * Further, post it in a EVENT_TYPE_IMAGE_PICKER event.
     * @return The new handle.
     */
    private int getSelectedImageHandle()
    {
        // Create handle.
        int dataHandle = mMoSyncThread.nativeCreatePlaceholder();
//    	createData(dataHandle, size);
//    	writedata

        return dataHandle;
    }

    public class CustomAdapter extends BaseAdapter
    {
        int mGalleryItemBackg;
        private Context mContext;

        public CustomAdapter(Context c)
        {
            mContext = c;
            TypedArray typArray = getActivity().obtainStyledAttributes(R.styleable.GalleryTheme);

            // Set the theme with a frame surrounding gallery items.
            mGalleryItemBackg = typArray.getResourceId(R.styleable.GalleryTheme_android_galleryItemBackground, 0);
            typArray.recycle();
        }

        public int getCount()
        {
            return mImagePaths.length;
        }

        public Object getItem(int position)
        {
            return position;
        }

        public long getItemId(int position)
        {
            return position;
        }

        public View getView(int position, View convertView, ViewGroup parent)
        {
            ImageView imgView = new ImageView(mContext);

            Bitmap bitmap = BitmapFactory.decodeFile(mImagePaths[position]);
            imgView.setImageBitmap(bitmap);
            imgView.setLayoutParams(new Gallery.LayoutParams(LayoutParams.WRAP_CONTENT,
			        LayoutParams.WRAP_CONTENT));
//            		(200, 150));
            imgView.setScaleType(ImageView.ScaleType.FIT_XY);
            imgView.setBackgroundResource(mGalleryItemBackg);

            return imgView;
        }
    }

    //--------------------------   Members   --------------------------//

	/**
	 * The MoSync thread object.
	 */
    private MoSyncThread mMoSyncThread;

    /**
     * List of absolute paths to all images on the device.
     */
	private String[] mImagePaths;

	/**
	 * List of all image files of the device.
	 */
	private File[] mImageFiles;

	/**
	 * The handle of the selected image.
	 */
	private int mImageHandle = -1;
}
