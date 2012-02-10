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
/**
 * @file MoSyncAdsModule.cs
 * @author Spiridon Alexandru
 *
 * @brief This represents the Ad module implementation for the Ads
 *        component on Windows Phone 7.1, language c#
 *
 * @platform WP 7.1
 **/

using System;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;

namespace MoSync
{
    public class MoSyncAdsModule : IIoctlModule
    {
        protected MoSync.NativeUI.Ad mAd;

        public void Init(Ioctls ioctls, Core core, Runtime runtime)
        {
            ioctls.maAdsBannerCreate = delegate(int _bannerSize, int _publisherID)
            {
                MoSync.Util.RunActionOnMainThreadSync(() =>
                    {
                        mAd = new NativeUI.Ad();
                    }
                );
                int handle = runtime.GetModule<NativeUIModule>().AddWidget(mAd);

                return handle;
            };

            ioctls.maAdsBannerDestroy = delegate(int _bannerHandler)
            {
                if (runtime.GetModule<NativeUIModule>().GetWidget(_bannerHandler).GetHandle() < 0)
                {
                    return MoSync.Constants.MA_ADS_RES_INVALID_BANNER_HANDLE;
                }

                // TODO: destroy

                return MoSync.Constants.MA_ADS_RES_OK;
            };

            ioctls.maAdsAddBannerToLayout = delegate(int _bannerHandle, int _layoutHandle)
            {
                if (runtime.GetModule<NativeUIModule>().GetWidget(_bannerHandle).GetHandle() < 0)
                {
                    return MoSync.Constants.MA_ADS_RES_INVALID_BANNER_HANDLE;
                }
                if (runtime.GetModule<NativeUIModule>().GetWidget(_layoutHandle).GetHandle() < 0)
                {
                    return MoSync.Constants.MA_ADS_RES_INVALID_LAYOUT_HANDLE;
                }

                runtime.GetModule<NativeUIModule>().GetWidget(_layoutHandle).AddChild(
                    runtime.GetModule<NativeUIModule>().GetWidget(_bannerHandle));

                return MoSync.Constants.MA_ADS_RES_OK;
            };

            ioctls.maAdsRemoveBannerFromLayout = delegate(int _bannerHandle, int _layoutHandle)
            {
                if (runtime.GetModule<NativeUIModule>().GetWidget(_bannerHandle).GetHandle() < 0)
                {
                    return MoSync.Constants.MA_ADS_RES_INVALID_BANNER_HANDLE;
                }
                if (runtime.GetModule<NativeUIModule>().GetWidget(_layoutHandle).GetHandle() < 0)
                {
                    return MoSync.Constants.MA_ADS_RES_INVALID_LAYOUT_HANDLE;
                }

                runtime.GetModule<NativeUIModule>().GetWidget(_layoutHandle).RemoveChild(
                    runtime.GetModule<NativeUIModule>().GetWidget(_bannerHandle));

                return MoSync.Constants.MA_ADS_RES_OK;
            };

            ioctls.maAdsBannerSetProperty = delegate(int _bannerHandle, int _property, int _value)
            {
                return MoSync.Constants.MA_ADS_RES_OK;
            };

            ioctls.maAdsBannerGetProperty = delegate(int _bannerHandle, int _property, int _value, int _bufSize)
            {
                return MoSync.Constants.MA_ADS_RES_OK;
            };
        }
    }
}
