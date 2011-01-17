package com.mosync.internal.android;

import java.util.Hashtable;

import com.mosync.internal.android.MoSyncThread.ImageCache;
import com.mosync.java.android.MoSync;
import com.mosync.nativeui.core.NativeUI;
import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.util.AsyncWait;

import android.app.Activity;
import android.util.Log;

/**
 * Wrapper for Native UI Syscalls to avoid cluttering
 * the MoSyncSyscalls file, also to call the native ui
 * functions in the UI thread.
 */
public class MoSyncNativeUI
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
	 * Internal wrapper for maWidgetCreate that runs
	 * the call in the UI thread.
	 */
	public int maWidgetCreate(final String type)
	{
		Log.i("MoSync", "MoSyncThread.maWidgetCreate");
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
		Log.i("MoSync", "MoSyncThread.maWidgetDestroy");
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
		Log.i("MoSync", "MoSyncThread.maWidgetAddChild");
		
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
		final int parentHandle, 
		final int childHandle)
	{
		Log.i("MoSync", "MoSyncThread.maWidgetRemoveChild");
		try
		{
			final AsyncWait<Integer> waiter = new AsyncWait<Integer>();
			getActivity().runOnUiThread(new Runnable() 
			{
				public void run()
				{
					int result = mNativeUI.maWidgetRemove(
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
	 * Internal wrapper for maWidgetScreenShow that runs
	 * the call in the UI thread.
	 */
	public int maWidgetScreenShow(final int screenHandle)
	{
		Log.i("MoSync", "MoSyncThread.maWidgetScreenShow");
		try
		{
			final AsyncWait<Integer> waiter = new AsyncWait<Integer>();
			getActivity().runOnUiThread(new Runnable() 
			{
				public void run()
				{
					int result = mNativeUI.maWidgetScreenShow(screenHandle);
					((MoSync) getActivity()).setRootView( 
						mNativeUI.getRootView( ) );
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
		Log.i("MoSync", "MoSyncThread.maWidgetSetProperty");
		
		// Bind and invalidate must be called on the main mosync thread,
		// since all opengl calls will be called from that thread.
		if( key.equals( Types.WIDGET_PROPERTY_BIND ) || key.equals( Types.WIDGET_PROPERTY_INVALIDATE ) )
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
		Log.i("MoSync", "MoSyncThread.maWidgetGetProperty");
		
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
}