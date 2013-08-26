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

package com.mosync.nativeui.ui.ads;

/**
 * Event sent by a banner widget.
 * @author emma tresanszki
 */
public class AdsEvent
{
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

	/************************ Class members ************************/

	// Banner's handle that sends the event.
	public int mBannerHandle;
	// One of the #MA_ADS_EVENT constants.
	public int mBannerEventType;
	// If bannerEventType is #MA_ADS_EVENT_FAILED this code will describe the error.
	public int mErrCode;
}
