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

package com.mosync.nativeui.ui.ads;

/**
 * Event sent by a banner widget.
 * @author emma tresanszki
 */
public class AdsEvent
{
	// Banner's handle that sends the event.
	public int mBannerHandle;
	// One of the #MA_ADS_EVENT constants.
	public int mBannerEventType;
	// If bannerEventType is #MA_ADS_EVENT_FAILED this code will describe the error.
	public int mErrCode;

	public AdsEvent(int type, int handle, int errorCode)
	{
		this.mBannerEventType = type;
		this.mBannerHandle = handle;
		this.mErrCode = errorCode;
	}

	@Override
	public String toString()
	{
		// Print error message.
		if ( this.mErrCode < 0 )
			return "Ad " + mBannerHandle + " event: " + mBannerEventType + ". Error code: " + mErrCode;

		return "Ad " + mBannerHandle + " event: " + mBannerEventType;
	}
}
