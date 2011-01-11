package com.mosync.internal.android;

import android.graphics.Bitmap;
import android.graphics.Canvas;

public final class ImageCache
{
	public Bitmap mBitmap;
	public Canvas mCanvas;

	public ImageCache(Canvas canvas, Bitmap bitmap)
	{
		mCanvas = canvas;
		mBitmap = bitmap;
	}
}