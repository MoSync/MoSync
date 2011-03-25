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
import com.mosync.nativeui.util.AsyncWait;

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
		mNativeUI = new NativeUI(getActivity());
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
	 * Internal wrapper for maWidgetSetProperty that runs
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

	@Override
	public void rootViewReplaced(View newRoot)
	{
		((MoSync) getActivity()).setRootView( newRoot );
	}
}