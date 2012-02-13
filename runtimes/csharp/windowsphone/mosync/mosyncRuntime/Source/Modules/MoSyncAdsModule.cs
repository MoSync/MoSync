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
    public class AdsModule : IIoctlModule
    {
        protected MoSync.NativeUI.Ad mAd;

        public void Init(Ioctls ioctls, Core core, Runtime runtime)
        {
            ioctls.maAdsBannerCreate = delegate(int _bannerSize, int _publisherID)
            {
                MoSync.Util.RunActionOnMainThreadSync(() =>
                    {
                        mAd = new NativeUI.Ad();

                        // If the banner size is a known windows phone 7 size, we set it.
                        // The default value is 300*50 (X-Large banner).
                        if (_bannerSize == MoSync.Constants.MA_ADS_SIZE_WP7_XLARGE)
                        {
                            mAd.Width = 300;
                            mAd.Height = 50;
                        }
                        else if (_bannerSize == MoSync.Constants.MA_ADS_SIZE_WP7_XXLARGE)
                        {
                            mAd.Width = 480;
                            mAd.Height = 80;
                        }

                        // the publisherID for windows phone contains two components separated by '|'.
                        // The first one represents the application ID and the second one the ad unit ID.
                        String publisherID = core.GetDataMemory().ReadStringAtAddress(_publisherID);
                        string[] values = publisherID.Split('|');
                        // only if both values are present we set the properties
                        if (values.Length == 2)
                        {
                            mAd.ApplicationID = values[0];
                            mAd.AdUnitID = values[1];
                        }
                    }
                );

                int handle = runtime.GetModule<NativeUIModule>().AddWidget(mAd);
                if (handle < 0)
                {
                    return MoSync.Constants.MA_ADS_RES_ERROR;
                }

                return handle;
            };

            ioctls.maAdsBannerDestroy = delegate(int _bannerHandler)
            {
                if (runtime.GetModule<NativeUIModule>().GetWidget(_bannerHandler).GetHandle() < 0)
                {
                    return MoSync.Constants.MA_ADS_RES_INVALID_BANNER_HANDLE;
                }

                mAd = null;

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
                MoSync.NativeUI.Ad ad = (MoSync.NativeUI.Ad)runtime.GetModule<NativeUIModule>().GetWidget(_bannerHandle);
                if (runtime.GetModule<NativeUIModule>().GetWidget(_bannerHandle).GetHandle() < 0)
                {
                    return MoSync.Constants.MA_ADS_RES_INVALID_BANNER_HANDLE;
                }

                String property = core.GetDataMemory().ReadStringAtAddress(_property);
                if (property.Equals(MoSync.Constants.MA_ADS_HEIGHT))
                {
                    string value = core.GetDataMemory().ReadStringAtAddress(_value);
                }
                else if (property.Equals(MoSync.Constants.MA_ADS_WIDTH))
                {
                    string value = core.GetDataMemory().ReadStringAtAddress(_value);
                }
                else if (property.Equals(MoSync.Constants.MA_ADS_VISIBLE))
                {
                    String value = core.GetDataMemory().ReadStringAtAddress(_value);
                    if (value.Equals("true"))
                    {
                        MoSync.Util.RunActionOnMainThreadSync(() =>
                            {
                                mAd.Visible = "true";
                            }
                        );
                    }
                    else if (value.Equals("false"))
                    {
                        MoSync.Util.RunActionOnMainThreadSync(() =>
                            {
                                mAd.Visible = "false";
                            }
                        );
                    }
                    else
                    {
                        return MoSync.Constants.MA_ADS_RES_INVALID_PROPERTY_VALUE;
                    }
                }
                else if (property.Equals(MoSync.Constants.MA_ADS_ENABLED))
                {
                    String value = core.GetDataMemory().ReadStringAtAddress(_value);
                    if (value.Equals("true"))
                    {
                        MoSync.Util.RunActionOnMainThreadSync(() =>
                            {
                                mAd.Enabled = "true";
                            }
                        );
                    }
                    else if (value.Equals("false"))
                    {
                        MoSync.Util.RunActionOnMainThreadSync(() =>
                            {
                                mAd.Enabled = "false";
                            }
                        );
                    }
                    else
                    {
                        return MoSync.Constants.MA_ADS_RES_INVALID_PROPERTY_VALUE;
                    }
                }
                else if (property.Equals(MoSync.Constants.MA_ADS_TEST_DEVICE))
                {
                    return MoSync.Constants.MA_ADS_RES_UNSUPPORTED;
                }
                else if (property.Equals(MoSync.Constants.MA_ADS_REQUEST_CONTENT))
                {
                    return MoSync.Constants.MA_ADS_RES_UNSUPPORTED;
                }
                else if (property.Equals(MoSync.Constants.MA_ADS_IS_READY))
                {
                    return MoSync.Constants.MA_ADS_RES_UNSUPPORTED;
                }
                else if (property.Equals(MoSync.Constants.MA_ADS_COLOR_BG))
                {
                    string value = core.GetDataMemory().ReadStringAtAddress(_value);
                }
                else if (property.Equals(MoSync.Constants.MA_ADS_COLOR_BG_TOP))
                {
                    return MoSync.Constants.MA_ADS_RES_UNSUPPORTED;
                }
                else if (property.Equals(MoSync.Constants.MA_ADS_COLOR_BORDER))
                {
                    string value = core.GetDataMemory().ReadStringAtAddress(_value);
                    MoSync.Util.RunActionOnMainThreadSync(() =>
                        {
                            mAd.BorderColor = value;
                        }
                    );
                }
                else if (property.Equals(MoSync.Constants.MA_ADS_COLOR_LINK))
                {
                    return MoSync.Constants.MA_ADS_RES_UNSUPPORTED;
                }
                else if (property.Equals(MoSync.Constants.MA_ADS_COLOR_TEXT))
                {
                    string value = core.GetDataMemory().ReadStringAtAddress(_value);
                }
                else if (property.Equals(MoSync.Constants.MA_ADS_COLOR_URL))
                {
                    return MoSync.Constants.MA_ADS_RES_UNSUPPORTED;
                }
                else
                {
                    return MoSync.Constants.MA_ADS_RES_INVALID_PROPERTY_NAME;
                }

                return MoSync.Constants.MA_ADS_RES_OK;
            };

            ioctls.maAdsBannerGetProperty = delegate(int _bannerHandle, int _property, int _value, int _bufSize)
            {
                String property = core.GetDataMemory().ReadStringAtAddress(_property);

                return MoSync.Constants.MA_ADS_RES_OK;
            };
        }
    }
}
