/* Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

package com.mosync.internal.android;

import java.util.Hashtable;

import android.app.Activity;
import android.util.Log;
import android.view.View;

import com.mosync.internal.android.MoSyncThread.ImageCache;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.java.android.MoSync;
import com.mosync.nativeui.core.NativeUI;
import com.mosync.nativeui.core.NativeUI.RootViewReplacedListener;
import com.mosync.nativeui.ui.widgets.Widget;
import com.mosync.nativeui.util.AsyncWait;
import com.mosync.nativeui.ui.custom.MoSyncImagePicker;
import com.mosync.nativeui.ui.custom.MoSyncOptionsDialog;

/**
 * Wrapper for Native UI Syscalls to avoid cluttering
 * the MoSyncSyscalls file, also to call the native ui
 * functions in the UI thread.
 */
public class MoSyncNativeUI implements RootViewReplacedListener
{
	/**
	 * The MoSync thread object.
	 */
	MoSyncThread mMoSyncThread;

	private NativeUI mNativeUI;

	/**
	 * Constructor.
	 * @param thread The MoSync thread.
	 */
	public MoSyncNativeUI(
		MoSyncThread thread, 
		Hashtable<Integer, ImageCache> imageResources)
	{
		mMoSyncThread = thread;
		mNativeUI = new NativeUI(mMoSyncThread, getActivity());
		mNativeUI.setRootViewReplacedListener(this);
		NativeUI.setImageTable(imageResources);
	}

	/**
	 * @return The Activity object.
	 */
	private Activity getActivity()
	{
		return mMoSyncThread.getActivity();
	}
	
	/**
	 * Sets the default MoSync canvas view, so that it is possible
	 * to switch back to it from native UI.
	 * 
	 * @param mosyncScreen The MoSync canvas view.
	 */
	public void setMoSyncScreen(MoSyncView moSyncView)
	{
		mNativeUI.setMoSyncScreen( moSyncView );
	}
	
	/**
	 * Internal wrapper for maWidgetCreate that runs
	 * the call in the UI thread.
	 */
	public int maWidgetCreate(final String type)
	{
		try
		{
			final AsyncWait<Integer> waiter = new AsyncWait<Integer>();
			getActivity().runOnUiThread(new Runnable() 
			{
				public void run()
				{
					int result = mNativeUI.maWidgetCreate(type);
					waiter.setResult(result);
				}
			});
			return waiter.getResult();
		}
		catch(InterruptedException ie)
		{
			Log.i("MoSync", "Native UI update interrupted.");
			return -1;
		}
	}
	
	/**
	 * Internal wrapper for maWidgetDestroy that runs
	 * the call in the UI thread.
	 */
	public int maWidgetDestroy(final int widget)
	{
		try
		{
			final AsyncWait<Integer> waiter = new AsyncWait<Integer>();
			getActivity().runOnUiThread(new Runnable() 
			{
				public void run()
				{
					int result = mNativeUI.maWidgetDestroy(widget);
					waiter.setResult(result);
				}
			});
			return waiter.getResult();
		}
		catch(InterruptedException ie)
		{
			return -1;
		}
	}
	
	/**
	 * Internal wrapper for maWidgetAddChild that runs
	 * the call in the UI thread.
	 */
	public int maWidgetAddChild(
		final int parentHandle, 
		final int childHandle)
	{
		try
		{
			final AsyncWait<Integer> waiter = new AsyncWait<Integer>();
			getActivity().runOnUiThread(new Runnable() 
			{
				public void run()
				{
					int result = mNativeUI.maWidgetAdd(
						parentHandle, childHandle);
					waiter.setResult(result);
				}
			});
			return waiter.getResult();
		}
		catch(InterruptedException ie)
		{
			return -1;
		}
	}
	
	/**
	 * Internal wrapper for maWidgetInsertChild that runs
	 * the call in the UI thread.
	 */
	public int maWidgetInsertChild(
		final int parentHandle, 
		final int childHandle,
		final int index)
	{
		try
		{
			final AsyncWait<Integer> waiter = new AsyncWait<Integer>();
			getActivity().runOnUiThread(new Runnable() 
			{
				public void run()
				{
					int result = mNativeUI.maWidgetInsertChild(
						parentHandle, childHandle, index);
					waiter.setResult(result);
				}
			});
			return waiter.getResult();
		}
		catch(InterruptedException ie)
		{
			return -1;
		}
	}
	
	/**
	 * Internal wrapper for maWidgetRemoveChild that runs
	 * the call in the UI thread.
	 */
	public int maWidgetRemoveChild( 
		final int childHandle)
	{
		try
		{
			final AsyncWait<Integer> waiter = new AsyncWait<Integer>();
			getActivity().runOnUiThread(new Runnable() 
			{
				public void run()
				{
					int result = mNativeUI.maWidgetRemove(childHandle);
					waiter.setResult(result);
				}
			});
			return waiter.getResult();
		}
		catch(InterruptedException ie)
		{
			return -1;
		}
	}

	/**
	 * Internal wrapper for maWidgetDialogShow that runs
	 * the call in the UI thread.
	 */
	public int maWidgetDialogShow(final int dialogHandle)
	{
		try
		{
			final AsyncWait<Integer> waiter = new AsyncWait<Integer>();
			getActivity().runOnUiThread(new Runnable()
			{
				public void run()
				{
					int result = mNativeUI.maWidgetDialogShow(dialogHandle);
					waiter.setResult(result);
				}
			});
			return waiter.getResult();
		}
		catch(InterruptedException ie)
		{
			return -1;
		}
	}

	/**
	 * Internal wrapper for maWidgetDialogHide that runs
	 * the call in the UI thread.
	 */
	public int maWidgetDialogHide(final int dialogHandle)
	{
		try
		{
			final AsyncWait<Integer> waiter = new AsyncWait<Integer>();
			getActivity().runOnUiThread(new Runnable()
			{
				public void run()
				{
					int result = mNativeUI.maWidgetDialogHide(dialogHandle);
					waiter.setResult(result);
				}
			});
			return waiter.getResult();
		}
		catch(InterruptedException ie)
		{
			return -1;
		}
	}

	/**
	 * Internal wrapper for maWidgetScreenShow that runs
	 * the call in the UI thread.
	 */
	public int maWidgetScreenShow(final int screenHandle)
	{
		try
		{
			final AsyncWait<Integer> waiter = new AsyncWait<Integer>();
			getActivity().runOnUiThread(new Runnable() 
			{
				public void run()
				{
					int result = mNativeUI.maWidgetScreenShow(screenHandle);
					waiter.setResult(result);
				}
			});
			return waiter.getResult();
		}
		catch(InterruptedException ie)
		{
			return -1;
		}
	}
	
	/**
	 * Internal wrapper for maWidgetStackScreenPush that runs
	 * the call in the UI thread.
	 */
	public int maWidgetStackScreenPush(
		final int stackScreenHandle, 
		final int newScreenHandle)
	{
		try
		{
			final AsyncWait<Integer> waiter = new AsyncWait<Integer>();
			getActivity().runOnUiThread(new Runnable() 
			{
				public void run()
				{
					int result = mNativeUI.maWidgetStackScreenPush(
							stackScreenHandle, newScreenHandle);
					waiter.setResult(result);
				}
			});
			return waiter.getResult();
		}
		catch(InterruptedException ie)
		{
			return -1;
		}
	}
	
	/**
	 * Internal wrapper for maWidgetStackScreenPop that runs
	 * the call in the UI thread.
	 */
	public int maWidgetStackScreenPop( 
		final int stackScreenWidget)
	{
		try
		{
			final AsyncWait<Integer> waiter = new AsyncWait<Integer>();
			getActivity().runOnUiThread(new Runnable() 
			{
				public void run()
				{
					int result = mNativeUI.maWidgetStackScreenPop(stackScreenWidget);
					waiter.setResult(result);
				}
			});
			return waiter.getResult();
		}
		catch(InterruptedException ie)
		{
			return -1;
		}
	}
	
	
	/**
	 * Internal wrapper for maWidgetSetProperty that runs
	 * the call in the UI thread.
	 */
	public int maWidgetSetProperty(
		final int widgetHandle, 
		final String key, 
		final String value)
	{
		// Bind and invalidate must be called on the main mosync thread,
		// since all opengl calls will be called from that thread.
		if( key.equals( IX_WIDGET.MAW_GL_VIEW_BIND ) || key.equals( IX_WIDGET.MAW_GL_VIEW_INVALIDATE ) )
		{
			return mNativeUI.maWidgetSetProperty(widgetHandle, key, value);
		}
		
		try
		{
			final AsyncWait<Integer> waiter = new AsyncWait<Integer>();
			getActivity().runOnUiThread(new Runnable() 
			{
				public void run()
				{
					int result = mNativeUI.maWidgetSetProperty(
						widgetHandle, key, value);
					waiter.setResult(result);
				}
			});
			return waiter.getResult();
		}
		catch(InterruptedException ie)
		{
			return -1;
		}
	}
	
	/**
	 * Internal wrapper for maWidgetGetProperty that runs
	 * the call in the UI thread.
	 */
	public int maWidgetGetProperty(
		final int widgetHandle, 
		final String key,
		final int memBuffer,
		final int memBufferSize)
	{
		try
		{
			final AsyncWait<Integer> waiter = new AsyncWait<Integer>();
			getActivity().runOnUiThread(new Runnable() {
				public void run()
				{
					int result = mNativeUI.maWidgetGetProperty(widgetHandle, key, memBuffer, memBufferSize);
					waiter.setResult(result);
				}
			});
			return waiter.getResult();
		}
		catch(InterruptedException ie)
		{
			return -1;
		}
	}

	/**
	 * Internal wrapper for maImagePickerOpen that runs
	 * the call in the UI thread.
	 */
	public int maImagePickerOpen()
	{
		final MoSyncImagePicker imagePicker = new MoSyncImagePicker(mMoSyncThread, mNativeUI.getImageTable());
		getActivity().runOnUiThread(new Runnable() {
			public void run()
			{
				imagePicker.loadGallery();
			}
		});

		return 0;
	}

	/**
	 * Internal wrapper for maWidgetShowOptionDialog that runs
	 * the call in the UI thread.
	 * Create and show the dialog.
	 * @return MAW_RES_OK, or MAW_RES_OPTION_DIALOG_ERROR if the dialog was not created.
	 */
	public int maWidgetShowOptionDialog(final String title, final String destructiveButtonTitle,
			final String cancelButtonTitle,final int buffPointer, final int buffSize)
	{
		Log.e( "MoSync", "maWidgetShowOptionDialog" );
		final MoSyncOptionsDialog optionsDialog = new MoSyncOptionsDialog(mMoSyncThread);

		getActivity().runOnUiThread(new Runnable() {
			public void run()
			{
				// Parse the options array. Add to the array the destructiveButtonTitle at the first position,
				// The clicks on cancelButtonTitle will send MAW_EVENT_OPTION_DIALOG_BUTTON_CLICKED with the index = array length.
				final String[] options;
				options = optionsDialog.parseStringFromMemory(buffPointer, buffSize);
				optionsDialog.showDialog(title, destructiveButtonTitle, cancelButtonTitle, options);
			}
		});

		return 0;
	}

	/**
	 * Called when the back button has been pressed.
	 */
	public void handleBack()
	{
		getActivity().runOnUiThread(new Runnable() {
			public void run()
			{
				mNativeUI.handleBack();
			}
		});
	}

	@Override
	public void rootViewReplaced(View newRoot)
	{
		((MoSync) getActivity()).setRootView( newRoot );
	}
	
	public Widget getCameraPreview(final int handle)
	{
		return mNativeUI.getCameraView(handle);
	}
}