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

package com.mosync.java.android;

import static com.mosync.internal.android.MoSyncHelpers.SYSLOG;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_FOCUS_GAINED;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_FOCUS_LOST;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_KEY_PRESSED;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_KEY_RELEASED;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_POINTER_DRAGGED;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_POINTER_PRESSED;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_POINTER_RELEASED;
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

import android.app.Activity;
import android.content.Intent;
import android.content.res.Configuration;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.KeyCharacterMap;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;

import com.mosync.internal.android.MoSyncMultiTouchHandler;
import com.mosync.internal.android.MoSyncSingleTouchHandler;
import com.mosync.internal.android.MoSyncThread;
import com.mosync.internal.android.MoSyncTouchHandler;
import com.mosync.internal.android.MoSyncView;

/**
* Main MoSync activity
*
* As long as this activity is running the application is running.
*/
public class MoSync extends Activity
{
	static public MoSyncThread mMoSyncThread;
	MoSyncView mMoSyncView;
	Intent mMoSyncServiceIntent;
	
	private boolean mHasDeterminedTouchCapabilities = false;
	private MoSyncTouchHandler mTouchHandler;
	
	/**
	 * Sets screen and window properties.
	 * Creates and initializes the MoSync thread
	 * Calls the createMoSyncView method and then shows the created view.
	 */
    @Override
    public void onCreate(Bundle savedInstanceState) 
	{
		Log.i("MoSync", "onCreate");
		
		super.onCreate(savedInstanceState);
		
		//if (theMoSyncThreadIsDead()) { return ; }
		
		// Initialize.
		mMoSyncView = null;
		
		// MoSync Android apps do not have a system title bar.
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		
		// Default screen orientation is landscape mode.
		// There is a name clash between the MoSync and the Android 
		// SCREEN_ORIENTATION_PORTRAIT constant!
		// Preprecessing of .jpp files fuck things up!
		// We therefore use the numeric value of this Android constant.
		setRequestedOrientation(1);
		
		try
		{
			// Create the thread.
			mMoSyncThread = new MoSyncThread(this, new Handler());
		}
		catch (Exception ex)
		{
			MoSyncThread.logError(
				"MoSync - Unable to create thread! " +
				"Application is closed!", 
				ex);
			//super.onDestroy();
			finish();
			return;
		}
		
		// Create the view.
		createMoSyncView();
		mMoSyncThread.setMoSyncView(mMoSyncView);
		setContentView(mMoSyncView);
    }
	
	public void setRootView(View root)
	{
		if(root == null)
		{
			Log.i("MoSync", "setRootView, root is null.");
		}
	
		setContentView(root);
	}
	
	/**
	 * Handles the new configurations when the screen rotates.
	 * @param newConfig Object that holds configuration info.
	 */
	@Override
	public void onConfigurationChanged(Configuration newConfig)
	{
		Log.i("MoSync", "onConfigurationChanged");

		super.onConfigurationChanged(newConfig);
	}
	
	@Override
    protected void onPause()
	{
		Log.i("MoSync", "onPause");
		
		super.onPause();
		
		if (theMoSyncThreadIsDead()) { return ; }
		
		SYSLOG("Posting EVENT_TYPE_FOCUS_LOST to MoSync");
		int[] event = new int[1];
		event[0] = EVENT_TYPE_FOCUS_LOST;
		mMoSyncThread.postEvent(event);
    }
	
    @Override
    protected void onStop()
	{
		Log.i("MoSync", "onStop");
		
		super.onStop();
		
		if (theMoSyncThreadIsDead()) { return ; }
		
		// The view is destroyed, inform the thread about this.
		mMoSyncThread.setMoSyncView(null);
	}

	@Override
    protected void onRestart()
	{
		Log.i("MoSync", "onRestart");
		
		super.onRestart();
		
		if (theMoSyncThreadIsDead()) { return ; }
	}
	
	@Override
    protected void onStart()
	{
		Log.i("MoSync", "onStart");
		
		super.onStart();
	}
	
	@Override
    protected void onResume()
	{
		Log.i("MoSync", "onResume");
		
		super.onResume();
		
		if (theMoSyncThreadIsDead()) { return ; }
		
		mMoSyncThread.setMoSyncView(mMoSyncView);
		
		SYSLOG("Posting EVENT_TYPE_FOCUS_GAINED to MoSync");
		int[] event = new int[1];
		event[0] = EVENT_TYPE_FOCUS_GAINED;
		mMoSyncThread.postEvent(event);
    }
	
	@Override
    protected void onDestroy()
	{
    	Log.i("MoSync", "onDestroy");
		
		super.onDestroy();
    }
	
	/**
	 * Creates the MoSyncView. If it fails the Activity is destroyed.
	 */
	private void createMoSyncView()
	{
		Log.i("MoSync", "createMoSyncView");
		
		try 
		{
			mMoSyncView = new MoSyncView(this, mMoSyncThread);
        } 
		catch (Exception ex) 
		{
			MoSyncThread.logError(
				"MoSync - The MoSyncView could not be created, " +
				"the application could not start!", 
				ex);
			finish();
		}
	}
	
	public boolean onKeyUp(int keyCode, KeyEvent keyEvent)
	{
		SYSLOG("onKeyUp: " + keyEvent.toString());

		int[] event = new int[3];
		
		event[0] = EVENT_TYPE_KEY_RELEASED;
		event[1] = convertToMoSyncKeyCode(keyCode, keyEvent);
		event[2] = keyCode;
	
		mMoSyncThread.postEvent(event);
		
		// We need to intercept the back key otherwise the
		// activity will be terminated.
		if( keyCode == KeyEvent.KEYCODE_BACK )
		{
			return true;
		}
		else
		{
			return super.onKeyUp( keyCode, keyEvent );
		}
	}
	
	public boolean onKeyDown(int keyCode, KeyEvent keyEvent)
	{
		SYSLOG("onKeyDown: " + keyEvent.toString());

		int[] event = new int[3];
		
		event[0] = EVENT_TYPE_KEY_PRESSED;
		event[1] = convertToMoSyncKeyCode(keyCode, keyEvent);
		event[2] = keyCode;
	
		mMoSyncThread.postEvent(event);
		
		// We need to intercept the back key otherwise the
		// activity will be terminated.
		if(keyCode == KeyEvent.KEYCODE_BACK)
		{
			return true;
		}
		else
		{
			return super.onKeyDown( keyCode, keyEvent );
		}
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
	 * onTouchEvent
	 * Receives touch events from the screen.
	 * 
	 * This implementation is build so that it won't receive dragged touch 
	 * events while the screen is updating. This is because the events are
	 * fired more often than some devices can digest the events, leading to
	 * problems when the queue just grows. Discarded events are still 
	 * marked as digested for the Android OS since we don't want any other 
	 * view to digest them either.
	 * 
	 * Handles both single and multi touch devices.
	 * 
	 * @param motionEvent	The Motion event received from the Android OS
	 * 
	 * @return 				True if the event was digested
	 * 						False if it wasn't
	 */
	public boolean onTouchEvent(MotionEvent motionEvent)
	{
		SYSLOG("onTouchEvent");
		
		// The first time around, see if this is multi or single touch device
		if(!mHasDeterminedTouchCapabilities)
		{
			mHasDeterminedTouchCapabilities = true;
			try
			{
				mTouchHandler = new MoSyncMultiTouchHandler();
				Log.i("MoSync TouchEvent","Multi touch device!");
			}
			catch(java.lang.VerifyError error)
			{
				mTouchHandler = new MoSyncSingleTouchHandler();
				Log.i("MoSync TouchEvent","Single touch device!");
			}
		}
		
		int[] touchEvent = new int[4];
		
		int actionCode = motionEvent.getAction() & MotionEvent.ACTION_MASK;
		
		switch (actionCode)
		{
			case MotionEvent.ACTION_DOWN:
				touchEvent[0] = EVENT_TYPE_POINTER_PRESSED;
				break;
			case MotionEvent.ACTION_UP:
				touchEvent[0] = EVENT_TYPE_POINTER_RELEASED;
				break;
			case MotionEvent.ACTION_MOVE:
				touchEvent[0] = EVENT_TYPE_POINTER_DRAGGED;
				// While drawing, discard this event
				if(mMoSyncThread.mIsUpdatingScreen) return true;
				break;
			default:
				return false;
		}
		
		// Get all of the events and send them to the runtime
		int numEvents = mTouchHandler.loadEvent(motionEvent);
		for( int i = 0; i < numEvents; i++)
		{
			int[] eventData = mTouchHandler.parseEvent(i);
			
			// TO-DO : Proper error handling
			if(eventData == null) return false;
			
			touchEvent[1] = eventData[0];
			touchEvent[2] = eventData[1];
			touchEvent[3] = eventData[2];
			
			mMoSyncThread.postEvent(touchEvent);
		}
		
		return super.onTouchEvent(motionEvent);
	}

	/**
	 * In case of maPanic the MoSync thread exits
	 * and is dead. In this case we should finish
	 * the application.
	 */
	private boolean theMoSyncThreadIsDead()
	{
		if (null != mMoSyncThread && mMoSyncThread.isDead())
		{
			MoSyncService.stopService();
			finish();
			return true;
		}
		else
		{
			return false;
		}
	}
}
