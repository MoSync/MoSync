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

import static com.mosync.internal.android.MoSyncHelpers.SYSLOG;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_SCREEN_CHANGED;
import android.app.Activity;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * MoSyncView class
 */
public class MoSyncView extends SurfaceView implements SurfaceHolder.Callback
{
	public SurfaceHolder mSurfaceHolder;

	private Context mContext;
	private MoSyncThread mMoSyncThread;

	/**
	 * MoSyncView Constructor
	 * @param context		The Android Activity Context
	 * @param moSyncThread	The thread in which the runtime is running
	 */
	public MoSyncView(Context context, MoSyncThread moSyncThread)
		throws Exception
	{
		super(context);

		SYSLOG("Constructor");

		mContext = context;
		mMoSyncThread = moSyncThread;

		// Register our interest in hearing about changes to our surface.
		mSurfaceHolder = getHolder();

		// Use accelerated surfaces if available.
		int mode = 0;
		try
		{
			mode = 1;
			mSurfaceHolder.setType(
				android.view.SurfaceHolder.SURFACE_TYPE_HARDWARE);
		}
		catch(Exception e)
		{
			try
			{
				mode = 2;
				mSurfaceHolder.setType(
					android.view.SurfaceHolder.SURFACE_TYPE_GPU);
			}
			catch(Exception e2)
			{
				mSurfaceHolder.setType(
					android.view.SurfaceHolder.SURFACE_TYPE_NORMAL);
			}
		}
/*
		switch (mode)
		{
			case 1: Log.i("MoSync", "Hardware accelerated surface"); break;
			case 2: Log.i("MoSync", "GPU accelerated surface"); break;
			default: Log.i("MoSync", "No hardware acceleration available");
		}
*/
		mSurfaceHolder.addCallback(this);

		setVisibility(VISIBLE);

		setFocusableInTouchMode(true);
	}

	/**
	 * Called during layout when the size of this view has changed.
	 * If you were just added to the view hierarchy, you're called
	 * with the old values of 0.
	 * @param w Current width of this view.
	 * @param w h Current height of this view.
	 * @param w oldw Old width of this view.
	 * @param w oldh Old height of this view.
	 */
	@Override
	public void onSizeChanged(int w, int h, int oldw, int oldh)
	{
		SYSLOG("onSizeChanged w:" + w + " h:" + h
			+ " oldw:" + oldw + " oldh:" + oldh);

		super.onSizeChanged(w, h, oldw, oldh);
	}

	/**
	 * This function is called directly after a change to the surface.
	 *
	 * Here we post event EVENT_TYPE_SCREEN_CHANGED to MoSync.
	 *
	 * @param holder SurfaceHolder object, supplied by Android.
	 * @param format new surface format, supplied by Android.
	 * @param width new surface width, supplied by Android.
	 * @param height new surface height, supplied by Android.
	 */
	@Override
	public void surfaceChanged(
		SurfaceHolder holder,
		int format,
		int width,
		int height)
	{
		SYSLOG("surfaceChanged");

		//Log.i("@@@@@", "surfaceChanged");

		// Allocate new screen buffer.
		mMoSyncThread.updateSurfaceSize(width, height);

		// Post screen changed event.
		// TODO: Should this be done also initially, when the surface is
		// first created, or only when it is changed after it has been created?
		// See issue 931.
		// Resolved: Should NOT be done initially! But docs should be updated!
		SYSLOG("Posting EVENT_TYPE_SCREEN_CHANGED to MoSync");
		int[] event = new int[1];
		event[0] = EVENT_TYPE_SCREEN_CHANGED;
		mMoSyncThread.postEvent(event);
	}

	/**
	 * Function which is called by Android directly after the surface
	 * has been created.
	 *
	 * NOTE: Here we start the MoSync thread.
	 *
	 * @param holder SurfaceHolder object, supplied by Android.
	 */
	@Override
	public void surfaceCreated(SurfaceHolder holder)
	{
		SYSLOG("surfaceCreated");

		//Log.i("@@@@@", "surfaceCreated");

		if (!mMoSyncThread.isAlive())
		{
			//Log.i("@@@@@", "surfaceCreated 1");
			mMoSyncThread.initSyscalls();
			mMoSyncThread.updateSurfaceSize(getWidth(), getHeight());
			mMoSyncThread.start();
		}
		else
		{
			//Log.i("@@@@@", "surfaceCreated 2");
			mMoSyncThread.updateScreen();
		}
	}

	/**
	 * Method that is called directly before the surface is destroyed.
	 */
	public void surfaceDestroyed(SurfaceHolder holder)
	{
		SYSLOG("surfaceDestroyed");
	}

	@Override
	protected void onDraw(Canvas canvas)
	{
		SYSLOG("onDraw");
		super.onDraw(canvas);
	}

	protected void onFocusChanged(
		boolean gainFocus,
		int direction,
		Rect previouslyFocusedRect)
	{
		SYSLOG("onFocusChanged");
	}

	public boolean onTouchEvent(MotionEvent motionEvent)
	{
		return ((Activity) mContext).onTouchEvent(motionEvent);
	}
}
