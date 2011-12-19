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

import android.app.Activity;

import com.mosync.nativeui.ui.ads.Ads;
import com.mosync.nativeui.util.AsyncWait;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * Wrapper for Ads Syscalls to avoid cluttering the MoSyncSyscalls file.
 * @author emma tresanszki
 */
public class MoSyncAds
{

	/**
	 * Constructor Ads API.
	 * @param thread The underlying MoSync thread.
	 */
	public MoSyncAds(MoSyncThread thread)
	{
		mMoSyncThread = thread;
		mAds = new Ads(mMoSyncThread, getActivity());
	}

	/**
	 * @return The Activity object.
	 */
	private Activity getActivity() {
		return mMoSyncThread.getActivity();
	}

	/************************ Syscalls ************************/

	/**
	 * Internal function for the maAdsBannerCreate system call
	 * that runs on the UI thread.
	 */
	public int maAdsBannerCreate(final int bannerSize, final String publisherID)
	{
		try
		{
			final AsyncWait<Integer> waiter = new AsyncWait<Integer>();
			getActivity().runOnUiThread(new Runnable()
			{
				public void run()
				{
					int result = mAds.maAdsBannerCreate(bannerSize, publisherID);
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
	 * Internal function for the maAdsAddBannerToLayout system call
	 * that runs on the UI thread.
	 */
	public int maAdsAddBannerToLayout(final int bannerHandle, final int layoutHandle, final Widget layoutWidget)
	{
		try
		{
			final AsyncWait<Integer> waiter = new AsyncWait<Integer>();
			getActivity().runOnUiThread(new Runnable()
			{
				public void run()
				{
					int result = mAds.maAdsAddBannerToLayout(bannerHandle, layoutHandle, layoutWidget);
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
	 * Internal wrapper for maAdsRemoveBannerFromLayout that runs
	 * the call in the UI thread.
	 */
	public int maAdsRemoveBannerFromLayout(final int bannerHandle, final int layoutHandle, final Widget layoutWidget)
	{
		try
		{
			final AsyncWait<Integer> waiter = new AsyncWait<Integer>();
			getActivity().runOnUiThread(new Runnable()
			{
				public void run()
				{
					int result = mAds.maAdsRemoveBannerFromLayout(bannerHandle, layoutHandle, layoutWidget);
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
	 * Internal wrapper for maAdsBannerDestroy that runs
	 * the call in the UI thread.
	 */
	public int maAdsBannerDestroy(final int bannerHandle)
	{
		try
		{
			final AsyncWait<Integer> waiter = new AsyncWait<Integer>();
			getActivity().runOnUiThread(new Runnable()
			{
				public void run()
				{
					int result = mAds.maAdsBannerDestroy(bannerHandle);
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
	 * Internal wrapper for maAdsBannerSetProperty that runs
	 * the call in the UI thread.
	 */
	public int maAdsBannerSetProperty(
		final int adHandle,
		final String key,
		final String value)
	{
		// TODO check if this really should be called from the ui thread
		try
		{
			final AsyncWait<Integer> waiter = new AsyncWait<Integer>();
			getActivity().runOnUiThread(new Runnable()
			{
				public void run()
				{
					int result = mAds.maAdsBannerSetProperty(
						adHandle, key, value);
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
	 * Internal wrapper for maAdsBannerGetProperty that runs
	 * the call in the UI thread.
	 */
	public int maAdsBannerGetProperty(
		final int adHandle,
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
					int result = mAds.maAdsBannerGetProperty(adHandle, key, memBuffer, memBufferSize);
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
	/************************ Class members ************************/

	/**
	 * The MoSync thread object.
	 */
	private MoSyncThread mMoSyncThread;

	private Ads mAds;
}