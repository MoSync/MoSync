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

import java.util.List;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_CLOSE;
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
import static com.mosync.internal.generated.MAAPI_consts.MAK_SEARCH;
import static com.mosync.internal.generated.MAAPI_consts.MAK_SOFTLEFT;
import static com.mosync.internal.generated.MAAPI_consts.MAK_SOFTRIGHT;
import static com.mosync.internal.generated.MAAPI_consts.MAK_UP;
import static com.mosync.internal.generated.MAAPI_consts.MA_SCREEN_ORIENTATION_PORTRAIT_UP;
import static com.mosync.internal.generated.MAAPI_consts.MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN;
import static com.mosync.internal.generated.MAAPI_consts.MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT;
import static com.mosync.internal.generated.MAAPI_consts.MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT;


import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.KeyCharacterMap;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.View;
import android.view.Window;

import com.mosync.internal.android.EventQueue;
import com.mosync.internal.android.Mediator;
import com.mosync.internal.android.MoSyncCapture;
import com.mosync.internal.android.MoSyncImagePicker;
import com.mosync.internal.android.MoSyncMultiTouchHandler;
import com.mosync.internal.android.MoSyncSingleTouchHandler;
import com.mosync.internal.android.MoSyncThread;
import com.mosync.internal.android.MoSyncTouchHandler;
import com.mosync.internal.android.MoSyncView;
import com.mosync.internal.android.extensions.MoSyncExtensionLoader;
import com.mosync.internal.android.billing.Consts;
import com.mosync.internal.android.billing.PurchaseManager;
import com.mosync.internal.android.nfc.MoSyncNFCForegroundUtil;
import com.mosync.internal.android.nfc.MoSyncNFCService;
import com.mosync.internal.android.notifications.LocalNotificationsManager;
import com.mosync.internal.android.notifications.LocalNotificationsService;
import com.mosync.internal.android.notifications.PushNotificationsManager;
import com.mosync.internal.android.notifications.PushNotificationsUtil;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.core.NativeUI;
import com.mosync.nativeui.ui.widgets.OptionsMenuItem;
import com.mosync.nativeui.ui.widgets.ScreenWidget;
import com.mosync.nativeui.ui.widgets.StackScreenWidget;
import com.mosync.nativeui.ui.widgets.TabScreenWidget;
import com.mosync.nativeui.ui.widgets.Widget;
import com.mosync.nativeui.util.ScreenTransitions;

/**
 * Main MoSync activity
 *
 * As long as this activity is running the application is running.
 */
public class MoSync extends Activity
{
	/**
	 * Activity request codes for Camera intent.
	 */
	public static final int CAPTURE_MODE_RECORD_VIDEO_REQUEST = 0;
	public static final int CAPTURE_MODE_STOP_RECORDING_REQUEST = 1;
	public static final int CAPTURE_MODE_TAKE_PICTURE_REQUEST = 2;

	/**
	 * Activity request code for Gallery intent.
	 */
	public static final int PICK_IMAGE_REQUEST = 3;

	private MoSyncThread mMoSyncThread;
	private MoSyncView mMoSyncView;
	//private Intent mMoSyncServiceIntent;
	private boolean mHasDeterminedTouchCapabilities = false;
	private MoSyncTouchHandler mTouchHandler;
	private BroadcastReceiver mShutdownListener;
	private boolean mEventTypeCloseHasBeenSent = false;
	private MoSyncNFCForegroundUtil nfcForegroundHandler;
	/**
	 * Keep the current screen rotation, and check it againts new
	 * values retrieved when configuration changes.
	 */
	private static int mScreenRotation = Surface.ROTATION_0;

	/**
	 * Sets screen and window properties.
	 * Creates and initializes the MoSync thread
	 * Calls the createMoSyncView method and then shows the created view.
	 */
    @Override
    public void onCreate(Bundle savedInstanceState)
	{
		Log.i("MoSync", "MoSync onCreate");

		super.onCreate(savedInstanceState);

		try {
			// If triggered by an NFC event, must handle it this way.
			MoSyncNFCService.handleNFCIntent(this, getIntent());
			nfcForegroundHandler = MoSyncNFCForegroundUtil.create(this);
		}catch(Throwable t){
			SYSLOG("No NFC");
		}

		// Initialize.
		mMoSyncView = null;

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
			finish();
			return;
		}

		try {
			// If triggered by a GCM/C2DM message, handle it here.
			// Call this after the MoSyncThread is created.
			if ( getIntent().getBooleanExtra(
					PushNotificationsUtil.MOSYNC_INTENT_EXTRA_NOTIFICATION, false) )
			{
				Log.e("@@MoSync","MoSync activity started after a push notification was received");
				PushNotificationsManager.handlePushNotificationIntent(getIntent());
			}
		}catch(Throwable t){
			SYSLOG("No GCM/C2DM message");
		}

		// Create the view.
		mMoSyncView = createMoSyncView();
		if (null != mMoSyncView)
		{
			mMoSyncThread.setMoSyncView(mMoSyncView);

			setContentView(mMoSyncView);
		}
		else
		{
			finish();
			return;
		}

		registerShutdownListener();
    }

    public MoSyncThread getMoSyncThread()
    {
    	return mMoSyncThread;
    }

	public void setRootView(View root)
	{
		if(root == null)
		{
			Log.i("MoSync", "setRootView, root is null.");
		}

		setContentView(root);
	}

	public void setRootViewUsingTransition(View root, int screenTransitionType, int screenTransitionDuration)
	{
		if(root == null)
		{
			Log.i("MoSync", "setRootViewUsingTransition, root is null.");
		}
		ScreenTransitions.applyScreenTransition(root, screenTransitionType, screenTransitionDuration, false);
        setContentView(root);
	}

	@Override
	public void onNewIntent(Intent intent) {
		if (MoSyncNFCService.handleNFCIntent(this, intent)) {
			Log.d("@@@ MoSync", "Discovered tag");
		}
		else if(intent.getAction().equals(LocalNotificationsService.ACTION_NOTIFICATION_RECEIVED))
		{
			Log.e("@@MoSync", "onNewIntent: Local Notification received ");
			int notificationId = intent.getIntExtra(
					LocalNotificationsService.LOCAL_NOTIFICATION_ID,
					LocalNotificationsService.LOCAL_NOTIFICATION_ID_DEFAULT);

			LocalNotificationsManager.postEventNotificationReceived(notificationId);
		}
		super.onNewIntent(intent);
	}

	/**
	 * Handles the new configurations when the screen rotates.
	 * @param newConfig Object that holds configuration info.
	 */
	@Override
	public void onConfigurationChanged(Configuration newConfig)
	{
		SYSLOG("@@MoSync onConfigurationChanged");

		super.onConfigurationChanged(newConfig);

		// Get the current rotation.
		int newRotation = getWindowManager().getDefaultDisplay().getRotation();

		SYSLOG("@@MoSync rotation = " + newRotation);

		// Has the rotation changed?
		if (mScreenRotation != newRotation)
		{
			// Save current rotation.
			mScreenRotation = newRotation;

			int screenOrientation = getScreenOrientation();
			// Post rotation event.
			ScreenWidget screen = mMoSyncThread.getUnconvertedCurrentScreen();
			if (null != screen)
			{
				// There is a NativeUI screen.
				// Post screen orientation event, handled by the NativeUI module.
				if ( screen instanceof StackScreenWidget || screen instanceof TabScreenWidget)
				{
					// Post screen orientation event, for the screen parent(TabScreen of StackScreen).
					EventQueue.getDefault().postScreenOrientationChanged(
							screen.getHandle(),
							screenOrientation);
					for(Widget child : screen.getChildren( ))
					{
						EventQueue.getDefault().postScreenOrientationChanged(
								child.getHandle(),
								screenOrientation);
					}
				}
				else
				{
					EventQueue.getDefault().postScreenOrientationChanged(
							mMoSyncThread.getCurrentScreen().getHandle(),
							screenOrientation);
				}
			}
			else
			{
				// No NativeUI screen, use the MoSync screen identifier.
				// NOTE: when no native UI screens are available, the widget related
				// event should not be sent, but in this case it's too late to remove this.
				// Post screen orientation event, handled by the NativeUI module.
				EventQueue.getDefault().postScreenOrientationChanged(
							IX_WIDGET.MAW_CONSTANT_MOSYNC_SCREEN_HANDLE,
							screenOrientation);
			}

			// Post orientation event, handled by the Moblet.
			EventQueue.getDefault().postOrientationChanged(
					screenOrientation);
		}
	}

	/**
	 * Get the screen orientation based on latest rotation.
	 * @return
	 */
	public static int getScreenOrientation()
	{
		switch(mScreenRotation)
		{
		case Surface.ROTATION_0:
			return MA_SCREEN_ORIENTATION_PORTRAIT_UP;
		case Surface.ROTATION_180:
			return MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN;
		case Surface.ROTATION_270:
			return MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT;
		case Surface.ROTATION_90:
			return MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT;
		default:
			return MA_SCREEN_ORIENTATION_PORTRAIT_UP;
		}
	}

	@Override
    protected void onStart()
	{
		Log.i("MoSync", "onStart");

		super.onStart();

		if (theMoSyncThreadIsDead()) { return ; }
	}

    @Override
    protected void onStop()
	{
		Log.i("MoSync", "onStop");
		super.onStop();

		if (theMoSyncThreadIsDead()) { return ; }

		mMoSyncThread.releaseHardware();
	}

	@Override
    protected void onResume()
	{
		SYSLOG("onResume");

		super.onResume();

		if (theMoSyncThreadIsDead()) { return ; }

		mMoSyncThread.acquireHardware();

		mMoSyncThread.onResume();

		if (nfcForegroundHandler != null) {
			nfcForegroundHandler.enableForeground();
		}

		SYSLOG("Posting EVENT_TYPE_FOCUS_GAINED to MoSync");
		int[] event = new int[1];
		event[0] = EVENT_TYPE_FOCUS_GAINED;
		mMoSyncThread.postEvent(event);
	}

	@Override
    protected void onPause()
	{
		Log.i("MoSync", "onPause");
		super.onPause();



		if (theMoSyncThreadIsDead()) { return ; }

		mMoSyncThread.onPause();
		//Moved the release hardware to the correct position seems to solve the problem.
		mMoSyncThread.releaseHardware();

		if (nfcForegroundHandler != null) {
			nfcForegroundHandler.disableForeground();
		}

		SYSLOG("Posting EVENT_TYPE_FOCUS_LOST to MoSync");
		int[] event = new int[1];
		event[0] = EVENT_TYPE_FOCUS_LOST;
		mMoSyncThread.postEvent(event);
    }

	@Override
    protected void onRestart()
	{
		Log.i("MoSync", "onRestart");

		super.onRestart();

		if (theMoSyncThreadIsDead()) { return ; }
	}

	@Override
    protected void onDestroy()
	{
		Log.i("MoSync", "onDestroy");

		super.onDestroy();


		// Tell the MoSync thread to do cleanup.
		mMoSyncThread.onDestroy();

		// Unregister the shutdown listener.
		unregisterShutdownListener();

		// Send EVENT_TYPE_CLOSE.
		// TODO: Since we have made some cleanup, everything may not
		// be available to the app. Be aware of this if doing more
		// cleanup here. Applications should save data etc on
		// EVENT_TYPE_FOCUS_LOST.
		sendCloseEvent();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        Log.e("@@MoSync", "MoSync onCreateOptionsMenu");
		menu.clear();

//		// Get the focused screen widget.
		ScreenWidget currentScreen = mMoSyncThread.getCurrentScreen();
		if ( currentScreen != null )
		{
			// Get the menu items for that screen.
			List<OptionsMenuItem> items = currentScreen.getMenuItems();
			// Add each menu item to the options menu.
			Log.e("@@MoSync","MoSync onCreateOptionsMenu, with " + items.size() + " items");
			for (int i=0; i < items.size(); i++)
			{
				MenuItem item = menu.add ( 0, items.get(i).getId(), 0, items.get(i).getTitle() );
				if ( items.get(i).hasIconFromResources() )
				{
					item.setIcon( items.get(i).getIconResId() );
				}
				else
				{
					item.setIcon( items.get(i).getIcon() );
				}
			}
//			return true;
		}
		else
			Log.e("@@MoSync"," onCreateOptionsMenu screen is NULL");
//
//		return super.onCreateOptionsMenu(menu);

//    	MenuItem item1 = menu.add(0, 1, 0, "btn");
//    	item1.setShowAsAction(MenuItem.SHOW_AS_ACTION_ALWAYS);
//    	item1.setIcon(17301555);
//
//    	MenuItem item2 = menu.add(1,0,0,"t");
//    	item2.setShowAsAction(MenuItem.SHOW_AS_ACTION_IF_ROOM);
//    	item2.setIcon(17301583);

        return super.onCreateOptionsMenu(menu);
    }

	@Override
	/**
	 * Initialize the content of the Options menu each
	 * time Menu is pressed, based on the current screen.
	 */
	public boolean onPrepareOptionsMenu(Menu menu)
	{
		Log.e("@@MoSync", "MoSync onPrepareOptionsMenu");

		// Remove all the items from the menu.
		menu.clear();

		// Get the focused screen widget.
		ScreenWidget currentScreen = mMoSyncThread.getCurrentScreen();
		if ( currentScreen != null )
		{
			Log.e("@@MoSync"," onPrepareOptionsMenu screen is not null");
			// Get the menu items for that screen.
			List<OptionsMenuItem> items = currentScreen.getMenuItems();
			Log.e("@@MoSync","MoSync onPrepareOptionsMenu, with " + items.size() + " items");
			// Add each menu item to the options menu.
			for (int i=0; i < items.size(); i++)
			{
				MenuItem item = menu.add ( 0, items.get(i).getId(), 0, items.get(i).getTitle() );

				if ( items.get(i).hasIconFromResources() )
				{
					item.setIcon( items.get(i).getIconResId() );
				}
				else if ( items.get(i).hasIconPredefined() )
				{
					item.setIcon( items.get(i).getIcon() );
				}
			}
//			return true;
		}
		else
			Log.e("@@MoSync"," onPrepareOptionsMenu screen is NULL");

		return super.onPrepareOptionsMenu(menu);
//		return false;
	}

	@Override
	/**
	 * Event received when an Options menu item is selected.
	 */
	public boolean onOptionsItemSelected(MenuItem item)
	{
		super.onOptionsItemSelected(item);

		// Get the focused screen widget.
		ScreenWidget currentScreen = mMoSyncThread.getCurrentScreen();
		if ( currentScreen != null )
		{
			EventQueue.getDefault().postOptionsMenuItemSelected(
				currentScreen.getHandle(),
				item.getItemId());

			return true;
		}

		return false;
	}

	@Override
	/**
	 * Event received when the Options menu is closed.
	 */
	public void onOptionsMenuClosed(Menu menu)
	{
		super.onOptionsMenuClosed(menu);

		// Get the focused screen widget.
		ScreenWidget currentScreen = mMoSyncThread.getCurrentScreen();
		if ( currentScreen != null )
		{
			EventQueue.getDefault().postOptionsMenuClosed(currentScreen.getHandle());
		}
	}

	/**
	 * This method is called when we get a result from a sub-activity.
	 * Specifically, it is used to get the result of a Bluetooth enable dialog,
	 * the capture API, or from the imagePicker.
	 */
	@Override
	protected void onActivityResult(
		int requestCode,
		int resultCode,
		Intent data)
	{
		// Check that this is the result of the Bluetooth enable activity.
		if (Mediator.REQUEST_ENABLE_BLUETOOTH == requestCode)
		{
			Mediator.getInstance().postBluetoothDialogClosedMessage();
		}
		else if ( resultCode == RESULT_OK &&
				requestCode == CAPTURE_MODE_RECORD_VIDEO_REQUEST )
		{
			Log.i("@@MoSync","Capture ready, control returned to MoSync activity.");
			// A video was recorded.
			MoSyncCapture.handleVideo(data);
		}
		else if ( resultCode == RESULT_OK &&
				requestCode == CAPTURE_MODE_TAKE_PICTURE_REQUEST )
		{
			Log.i("@@MoSync","Capture ready, control returned to MoSync activity.");
			// A picture was taken.
			MoSyncCapture.handlePicture(data);
		}
		else if ( resultCode == RESULT_CANCELED &&
				(requestCode == CAPTURE_MODE_TAKE_PICTURE_REQUEST || requestCode == CAPTURE_MODE_RECORD_VIDEO_REQUEST) )
		{
			Log.i("@@MoSync","Capture canceled, control returned to MoSync activity.");
			// Send MoSync event: the capture was canceled by the user.
			MoSyncCapture.handleCaptureCanceled();
		}
		else if ( resultCode == RESULT_OK &&
				requestCode == PICK_IMAGE_REQUEST )
		{
			MoSyncImagePicker.handleSelectedPicture(data);
		}
		else if ( resultCode == RESULT_CANCELED &&
				requestCode == PICK_IMAGE_REQUEST )
		{
			MoSyncImagePicker.handleCancelSelectPicture();
		}
		else if( requestCode == PurchaseManager.getCurrentRequestCode() )
		{
			SYSLOG("@@MoSync Activity onActivityResult for METHOD_REQUEST_PURCHASE");
			if ( !PurchaseManager.handleActivityResult(requestCode, resultCode, data) )
			{
				super.onActivityResult(requestCode, resultCode, data);
			}
		}
		else
		{
			super.onActivityResult(requestCode, resultCode, data);
		}
	}

	@Override
	protected void onSaveInstanceState( Bundle outState )
	{
		Log.e("@@MoSync", "onSaveInstanceState");
	}

	@Override
	protected void onRestoreInstanceState( Bundle savedInstanceState)
	{
	    Log.i( "@@MoSync", "onRestoreInstanceState");
	}

	/**
	 * Creates the MoSyncView. If it fails the Activity is destroyed.
	 * @return
	 */
	private MoSyncView createMoSyncView()
	{
		//Log.i("MoSync", "createMoSyncView");

		try
		{
			return new MoSyncView(this, mMoSyncThread);
        }
		catch (Exception ex)
		{
			MoSyncThread.logError(
				"MoSync - The MoSyncView could not be created, " +
				"the application could not start!",
				ex);
			ex.printStackTrace();
			return null;
		}
	}

	@Override
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
		if(keyCode == KeyEvent.KEYCODE_BACK)
		{
			// Pass on the back event in case we want to handle it.
			mMoSyncThread.handleBack();

			return true;
		}
		else
		{
			return super.onKeyUp(keyCode, keyEvent);
		}
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent keyEvent)
	{
		SYSLOG("onKeyDown: " + keyEvent.toString());

		// If the key is being held down we shouldn't send any more events.
		if(keyEvent.getRepeatCount() != 0) return true;

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
		if (keyCode == KeyEvent.KEYCODE_SEARCH) return MAK_SEARCH;

		// Support for native virtual keyboard.
		if (keyCode == KeyEvent.KEYCODE_DEL) { return MAK_CLEAR; }
		KeyCharacterMap keyCharacterMap = KeyCharacterMap.load(keyEvent.getDeviceId());
		return keyCharacterMap.get(keyCode, keyEvent.getMetaState());
	}

	/**
	 * onTouchEvent
	 * Receives touch events from the screen.
	 *
	 * This implementation discards touch move (dragged) events while the
	 * screen is updating. This is because move events are fired more
	 * frequently than some devices can digest the events, leading to
	 * problems when the queue just grows. Discarded events are still
	 * marked as digested for the Android OS since we don't want any other
	 * view to digest them either.
	 *
	 * Handles both single and multi touch devices.
	 *
	 * @param motionEvent	The Motion event received from the Android OS
	 *
	 * @return 				true if the event was digested
	 * 						false if it wasn't
	 */
	@Override
	public boolean onTouchEvent(MotionEvent motionEvent)
	{
		SYSLOG("onTouchEvent");

		// The first time around, see if this is multi or single touch device.
		if (!mHasDeterminedTouchCapabilities)
		{
			mHasDeterminedTouchCapabilities = true;
			try
			{
				mTouchHandler = new MoSyncMultiTouchHandler();
				//Log.i("MoSync TouchEvent","Multi touch device!");
			}
			catch(java.lang.VerifyError error)
			{
				mTouchHandler = new MoSyncSingleTouchHandler();
				//Log.i("MoSync TouchEvent","Single touch device!");
			}
		}

		int actionCode = motionEvent.getAction() & MotionEvent.ACTION_MASK;
		int index = -1;

		int eventType = 0;
		switch (actionCode)
		{
			case MotionEvent.ACTION_DOWN:
				eventType = EVENT_TYPE_POINTER_PRESSED;
				break;

			case MotionEvent.ACTION_UP:
				eventType = EVENT_TYPE_POINTER_RELEASED;
				break;

			case MotionEvent.ACTION_POINTER_DOWN:
				eventType = EVENT_TYPE_POINTER_PRESSED;
				index = (motionEvent.getAction() & 0x0000ff00) >> 0x00000008;
				break;

			case MotionEvent.ACTION_POINTER_UP:
				eventType = EVENT_TYPE_POINTER_RELEASED;
				index = (motionEvent.getAction( ) & 0x0000ff00) >> 0x00000008;
				break;

			case MotionEvent.ACTION_MOVE:
				eventType = EVENT_TYPE_POINTER_DRAGGED;
//				// While drawing, discard this event.
//				if (mMoSyncThread.mIsUpdatingScreen)
//				{
//					return true;
//				}
				break;
			default:
				// Return false to indicate that we have not handled the event.
				return false;
		}

		if (index != -1)
		{
			mTouchHandler.loadEvent(motionEvent);
			int[] eventData = mTouchHandler.parseEvent(index);
			sendPointerEvent(eventType, eventData);
		}
		else
		{
			// Get all of the events and send them to the runtime
			int numEvents = mTouchHandler.loadEvent(motionEvent);
			for (int i = 0; i < numEvents; i++)
			{
				int[] eventData = mTouchHandler.parseEvent(i);
				sendPointerEvent(eventType, eventData);
			}
		}

		// TODO: The superclass returns false, not what we want.
		// Confirm that the code change below is correct,
		// then delete commented out code.
		//return super.onTouchEvent(motionEvent);

		// Return true to indicate that we have handled the event.
		return true;
	}

	/**
	 * Send a pointer event.
	 *
	 * @param type The type of event to send, e.g. EVENT_TYPE_POINTER_MOVE.
	 * @param eventData An array containing, in order: the x position,
	 * 				 	the y position, the id of the pointer.
	 * @return
	 */
	public boolean sendPointerEvent(int type, int[] eventData)
	{
		// TO-DO : Proper error handling
		if(eventData == null)
		{
			return false;
		}

		int[] touchEvent = new int[4];
		touchEvent[0] = type;
		touchEvent[1] = eventData[0];
		touchEvent[2] = eventData[1];
		touchEvent[3] = eventData[2];

		mMoSyncThread.postEvent(touchEvent);
		return true;
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

	/**
	 * Register a broadcast receiver that listens for device shutdown events
	 * (power off the device).
	 */
    private void registerShutdownListener()
    {
    	mShutdownListener = new BroadcastReceiver()
		{
			@Override
			public void onReceive(Context context, Intent intent)
			{
				//Log.i("@@@MoSync", "@@@ ACTION_SHUTDOWN Intent = " + intent);

				// Send EVENT_TYPE_CLOSE.
				sendCloseEvent();
			}
		};

		registerReceiver(
			mShutdownListener,
			new IntentFilter(Intent.ACTION_SHUTDOWN));
    }

    /**
     * Unregister the shutdown listener.
     */
    private void unregisterShutdownListener()
    {
    	if (null != mShutdownListener)
    	{
    		unregisterReceiver(mShutdownListener);
    		mShutdownListener = null;
    	}
    }

    /**
     * Posts an EVENT_TYPE_CLOSE to the MoSync events queue.
     */
    private void sendCloseEvent()
    {
		if (!mEventTypeCloseHasBeenSent)
		{
			MoSyncExtensionLoader.getDefault().unloadExtensions();

			// Send EVENT_TYPE_CLOSE
			int[] event = new int[1];
			event[0] = EVENT_TYPE_CLOSE;
			mMoSyncThread.postEvent(event);

    		mEventTypeCloseHasBeenSent = true;
    	}
    }

}
