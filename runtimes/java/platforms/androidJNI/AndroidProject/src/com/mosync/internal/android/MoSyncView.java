/* Copyright (C) 2010 MoSync AB

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

import static com.mosync.internal.android.MoSyncHelpers.SYSLOG;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_KEY_PRESSED;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_KEY_RELEASED;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_POINTER_DRAGGED;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_POINTER_PRESSED;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_POINTER_RELEASED;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_SCREEN_CHANGED;
import static com.mosync.internal.generated.MAAPI_consts.MAK_BACK;
import static com.mosync.internal.generated.MAAPI_consts.MAK_CLEAR;
import static com.mosync.internal.generated.MAAPI_consts.MAK_DOWN;
import static com.mosync.internal.generated.MAAPI_consts.MAK_FIRE;
import static com.mosync.internal.generated.MAAPI_consts.MAK_LEFT;
import static com.mosync.internal.generated.MAAPI_consts.MAK_MENU;
import static com.mosync.internal.generated.MAAPI_consts.MAK_RIGHT;
import static com.mosync.internal.generated.MAAPI_consts.MAK_SOFTLEFT;
import static com.mosync.internal.generated.MAAPI_consts.MAK_SOFTRIGHT;
import static com.mosync.internal.generated.MAAPI_consts.MAK_UP;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.text.InputType;
import android.util.Log;
import android.view.KeyCharacterMap;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.inputmethod.BaseInputConnection;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputConnection;
import android.view.inputmethod.InputMethodManager;

/**
 * MoSyncView class
 */
public class MoSyncView extends SurfaceView implements SurfaceHolder.Callback
{
	public SurfaceHolder mSurfaceHolder;
	
	private Context mContext;
	private MoSyncThread mMoSyncThread;

	class CustomInputConnection extends BaseInputConnection
	{
		private MoSyncView mMoSyncView;
		public CustomInputConnection(View targetView, boolean fullEditor, MoSyncView mosyncView)
		{
			super(targetView, fullEditor);
			mMoSyncView = mosyncView;
		}
		
		public boolean performEditorAction (int actionCode)
		{
			InputMethodManager mgr = (InputMethodManager) mContext.getSystemService(Context.INPUT_METHOD_SERVICE);
			mgr.hideSoftInputFromWindow(mMoSyncView.getWindowToken(), 0);
			return true;
		}
	}

	/**
	* MoSyncView Constructor
	* @param context		The Android Activity Context
	* @param moSyncThread	The thread in which the runtime is running
	*/
	public MoSyncView(Context context, MoSyncThread moSyncThread) throws Exception
	{
		super(context);
		
		SYSLOG("Constructor");
		
		mContext = context;
		mMoSyncThread = moSyncThread;

		// register our interest in hearing about changes to our surface
		mSurfaceHolder = getHolder();

		// Use accelerated surfaces if available
		int mode = 0;
		try
		{
			mode = 1;
			mSurfaceHolder.setType(android.view.SurfaceHolder.SURFACE_TYPE_HARDWARE);
		}
		catch(Exception e)
		{
			try
			{
				mode = 2;
				mSurfaceHolder.setType(android.view.SurfaceHolder.SURFACE_TYPE_GPU);
			}
			catch(Exception e2)
			{
				mSurfaceHolder.setType(android.view.SurfaceHolder.SURFACE_TYPE_NORMAL);
			}
		}

		switch(mode)
		{
			case 1: Log.i("MoSync", "Hardware accelerated surface"); break;
			case 2: Log.i("MoSync", "GPU accelerated surface"); break;
			default: Log.i("MoSync", "No hardware acceleration available");
		}
				
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
		SYSLOG("onSizeChanged w:" + w + " h:" + h + " oldw:" + oldw + " oldh:" + oldh);
		
		super.onSizeChanged(w, h, oldw, oldh);
	}
	
	/*
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
		
		// Allocate new screen buffer.
		mMoSyncThread.updateSurfaceSize(width, height);
		
		// Post screen changed event.
		// TODO: Should this be done also initially, when the surface is
		// first created, or only when it is changed after it has been created?
		// See issue 931.
		SYSLOG("Posting EVENT_TYPE_SCREEN_CHANGED to MoSync");
		int[] event = new int[1];
		event[0] = EVENT_TYPE_SCREEN_CHANGED;
		mMoSyncThread.postEvent(event);
	}

	/*
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
		
		if (!mMoSyncThread.isAlive())
		{
			mMoSyncThread.updateSurfaceSize(getWidth(), getHeight());
			mMoSyncThread.initSyscalls();
			mMoSyncThread.start();
		}
		else
		{
			mMoSyncThread.updateScreen();
		}
	}
	
	/*
	* Function that is called directly before the surface is destroyed  
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

	protected void onFocusChanged(boolean gainFocus, int direction, Rect previouslyFocusedRect)
	{
		SYSLOG("onFocusChanged");
	}

/*	
	public int getKeys() {
		return mKeyState;
	}
*/	
	public boolean onTouchEvent(MotionEvent motionEvent)
	{
		SYSLOG("onTouchEvent");
		
		int[] event = new int[3];
		
		event[0] = EVENT_TYPE_POINTER_RELEASED;
		event[1] = (int)motionEvent.getX();
		event[2] = (int)motionEvent.getY();
		
		switch (motionEvent.getAction())
		{
			case 0:
				event[0] = EVENT_TYPE_POINTER_PRESSED;
				break;
			case 1:
				event[0] = EVENT_TYPE_POINTER_RELEASED;
				break;
			case 2:
				event[0] = EVENT_TYPE_POINTER_DRAGGED;
				break;
			default:
				return false;
		}
		
		mMoSyncThread.postEvent(event);
		
		return true;
	}

	/**
	 * Map Android key codes to MoSync key codes.
	 * @param keyCode
	 * @param keyEvent
	 * @return
	 */
	private final int convertToMoSyncKeyCode(int keyCode, KeyEvent keyEvent)
	{
		if (keyCode == KeyEvent.KEYCODE_DPAD_LEFT) return MAK_LEFT; 
		if (keyCode == KeyEvent.KEYCODE_DPAD_RIGHT) return MAK_RIGHT; 
		if (keyCode == KeyEvent.KEYCODE_DPAD_UP) return MAK_UP; 
		if (keyCode == KeyEvent.KEYCODE_DPAD_DOWN) return MAK_DOWN;
		if (keyCode == KeyEvent.KEYCODE_DPAD_CENTER) return MAK_FIRE; 
		if (keyCode == KeyEvent.KEYCODE_SOFT_LEFT) return MAK_SOFTLEFT; 
		if (keyCode == KeyEvent.KEYCODE_SOFT_RIGHT) return MAK_SOFTRIGHT; 
		if (keyCode == KeyEvent.KEYCODE_BACK) return MAK_BACK; 
		if (keyCode == KeyEvent.KEYCODE_MENU) return MAK_MENU;
		
		// Support for native virtual keyboard.
		if (keyCode == KeyEvent.KEYCODE_DEL) { return MAK_CLEAR; }
		KeyCharacterMap keyCharacterMap = KeyCharacterMap.load(keyEvent.getDeviceId());
		return keyCharacterMap.get(keyCode, keyEvent.getMetaState());
	}

	/**
	 * Map Android key codes to MoSync byte key codes.
	 * @param keyCode
	 * @param keyEvent
	 * @return
	 */
	/*
	private final int convertToMoSyncKeyByteCode(int keyCode) 
	{
		if (keyCode == KeyEvent.KEYCODE_0) return MAKB_0; 
		if (keyCode == KeyEvent.KEYCODE_1) return MAKB_1; 
		if (keyCode == KeyEvent.KEYCODE_2) return MAKB_2; 
		if (keyCode == KeyEvent.KEYCODE_3) return MAKB_3; 
		if (keyCode == KeyEvent.KEYCODE_4) return MAKB_4; 
		if (keyCode == KeyEvent.KEYCODE_5) return MAKB_5; 
		if (keyCode == KeyEvent.KEYCODE_6) return MAKB_6; 
		if (keyCode == KeyEvent.KEYCODE_7) return MAKB_7; 
		if (keyCode == KeyEvent.KEYCODE_8) return MAKB_8; 
		if (keyCode == KeyEvent.KEYCODE_9) return MAKB_9; 
		if (keyCode == KeyEvent.KEYCODE_STAR) return MAKB_STAR; 
		if (keyCode == KeyEvent.KEYCODE_POUND) return MAKB_POUND; 
		if (keyCode == KeyEvent.KEYCODE_CLEAR) return MAKB_CLEAR;
		if (keyCode == KeyEvent.KEYCODE_DPAD_LEFT) return MAKB_LEFT; 
		if (keyCode == KeyEvent.KEYCODE_DPAD_RIGHT) return MAKB_RIGHT; 
		if (keyCode == KeyEvent.KEYCODE_DPAD_UP) return MAKB_UP; 
		if (keyCode == KeyEvent.KEYCODE_DPAD_DOWN) return MAKB_DOWN;
		if (keyCode == KeyEvent.KEYCODE_DPAD_CENTER) return MAKB_FIRE; 
		if (keyCode == KeyEvent.KEYCODE_SOFT_LEFT) return MAKB_SOFTLEFT; 
		if (keyCode == KeyEvent.KEYCODE_SOFT_RIGHT) return MAKB_SOFTRIGHT;
		 
		return 0;
	}
	*/
	
	public boolean onKeyUp(int keyCode, KeyEvent keyEvent)
	{
		SYSLOG("onKeyUp: " + keyEvent.toString());

		int[] event = new int[3];
		
		event[0] = EVENT_TYPE_KEY_RELEASED;
		event[1] = convertToMoSyncKeyCode(keyCode, keyEvent);
		event[2] = keyCode;
	
		mMoSyncThread.postEvent(event);
		
		return true;
	}
	
	public boolean onKeyDown(int keyCode, KeyEvent keyEvent)
	{
		SYSLOG("onKeyDown: " + keyEvent.toString());

		int[] event = new int[3];
		
		event[0] = EVENT_TYPE_KEY_PRESSED;
		event[1] = convertToMoSyncKeyCode(keyCode, keyEvent);
		event[2] = keyCode;
	
		mMoSyncThread.postEvent(event);
		
		return true;
	}
	
	public InputConnection onCreateInputConnection (EditorInfo outAttrs)
	{
		outAttrs.inputType = InputType.TYPE_CLASS_NUMBER;
		CustomInputConnection ic = new CustomInputConnection(this, false, this);
		return ic;
	}
}
