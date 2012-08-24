/* Copyright (C) 2012 MoSync AB

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
using System.Text.RegularExpressions;

namespace MoSync
{
    public class AdsModule : IIoctlModule
    {
        protected MoSync.NativeUI.Ad mAd;

        public void Init(Ioctls ioctls, Core core, Runtime runtime)
        {
            /**
            * @brief Creates a new banner.
            * @param bannerSize One of the MA_ADS_SIZE_ constants. Only for Android and WP7.1 platforms.
            * @param publisherID Only for Android and WP 7.1 platforms.
            * This param is ignored on iOS platform.
            *
            * @note A banner is a widget type object.
            * For more info see Widget API.
            *
            * @returns
            *  - #MA_ADS_RES_UNSUPPORTED if ads are not supported on current system.
            *  - #MA_ADS_RES_ERROR if a error occurred while creating the banner widget.
            *  - a handle to a new banner widget(the handle value is >= 0).
            */
            ioctls.maAdsBannerCreate = delegate(int _bannerSize, int _publisherID)
            {
                MoSync.Util.RunActionOnMainThreadSync(() =>
                    {
                        mAd = new NativeUI.Ad();

                        // If the banner size is a known windows phone 7 size, we set it.
                        // The default value is 480*80 (XX-Large banner).
                        switch (_bannerSize)
                        {
                            case MoSync.Constants.MA_ADS_SIZE_WP7_XLARGE:
                                mAd.Width = 300;
                                mAd.Height = 50;
                                break;
                            case MoSync.Constants.MA_ADS_SIZE_WP7_XXLARGE:
                                mAd.Width = 480;
                                mAd.Height = 80;
                                break;
                            default:
                                mAd.Width = 480;
                                mAd.Height = 80;
                                break;
                        }

                        // the publisherID for windows phone contains two components separated by '|'.
                        // The first one represents the application ID and the second one the ad unit ID.
                        // The publisher ID structure(ex): f532778c-7db5-4a8b-a292-a45a684ed890
                        // The ad unit ID structure(ex): 81103
                        String publisherID = core.GetDataMemory().ReadStringAtAddress(_publisherID);
                        string[] values = publisherID.Split('|');
                        // only if both values are present we set the properties
                        if (2 == values.Length)
                        {
                            mAd.ApplicationID = values[0];
                            mAd.AdUnitID = values[1];
                        }
                    }
                );

                int handle = runtime.GetModule<NativeUIModule>().AddWidget(mAd);
                // if the handles is smaller than 0, the widget was not added to the layout
                if (handle < 0)
                {
                    return MoSync.Constants.MA_ADS_RES_ERROR;
                }
                mAd.SetHandle(handle);
                mAd.SetRuntime(runtime);

                return handle;
            };

            /**
            * @brief Destroy a banner.
            *
            * @param bannerHandle Handle to a banner.
            *
            * @returns One of the next constants:
            * - #MA_ADS_RES_OK if no error occurred.
            * - #MA_ADS_RES_INVALID_BANNER_HANDLE if the banner handle is invalid.
            */
            ioctls.maAdsBannerDestroy = delegate(int _bannerHandler)
            {
                if (!isHandleValid(runtime, _bannerHandler))
                {
                    return MoSync.Constants.MA_ADS_RES_INVALID_BANNER_HANDLE;
                }

                mAd = null;

                return MoSync.Constants.MA_ADS_RES_OK;
            };

            /**
            * @brief Add a banner to a layout widget.
            *
            * @param bannerHandle Handle to a banner.
            * @param layoutHandle Handle to a layout.
            *
            * @returns One of the next constants:
            * - #MA_ADS_RES_OK if no error occurred.
            * - #MA_ADS_RES_INVALID_BANNER_HANDLE if the banner handle is invalid.
            * - #MA_ADS_RES_INVALID_LAYOUT_HANDLE if the layout handle is invalid.
            */
            ioctls.maAdsAddBannerToLayout = delegate(int _bannerHandle, int _layoutHandle)
            {
                // we first check if both the banner and the layout are widgets with a valid handle
                if (!isHandleValid(runtime, _bannerHandle))
                {
                    return MoSync.Constants.MA_ADS_RES_INVALID_BANNER_HANDLE;
                }
                if (!isHandleValid(runtime, _layoutHandle))
                {
                    return MoSync.Constants.MA_ADS_RES_INVALID_LAYOUT_HANDLE;
                }

                // add the banner to the parent widget
                runtime.GetModule<NativeUIModule>().GetWidget(_layoutHandle).AddChild(
                    runtime.GetModule<NativeUIModule>().GetWidget(_bannerHandle));
                // set the parent of the banner to be the layout on which is added
                mAd.SetParent(runtime.GetModule<NativeUIModule>().GetWidget(_layoutHandle));

                return MoSync.Constants.MA_ADS_RES_OK;
            };

            /**
            * @brief Remove a banner from a layout widget.
            *
            * @param bannerHandle Handle to a banner.
            * @param layoutHandle Handle to a layout.
            *
            * @returns One of the next constants:
            * - #MA_ADS_RES_OK if no error occurred.
            * - #MA_ADS_RES_INVALID_BANNER_HANDLE if the banner handle is invalid.
            * - #MA_ADS_RES_INVALID_LAYOUT_HANDLE if the layout handle is invalid.
            */
            ioctls.maAdsRemoveBannerFromLayout = delegate(int _bannerHandle, int _layoutHandle)
            {
                // we first check if both the banner and the layout are widgets with a valid handle
                if (!isHandleValid(runtime, _bannerHandle))
                {
                    return MoSync.Constants.MA_ADS_RES_INVALID_BANNER_HANDLE;
                }
                if (!isHandleValid(runtime, _layoutHandle))
                {
                    return MoSync.Constants.MA_ADS_RES_INVALID_LAYOUT_HANDLE;
                }

                runtime.GetModule<NativeUIModule>().GetWidget(_layoutHandle).RemoveChild(
                    runtime.GetModule<NativeUIModule>().GetWidget(_bannerHandle));

                return MoSync.Constants.MA_ADS_RES_OK;
            };

            /**
            * @brief Set a banner property.
            *
            * @param bannerHandle Handle to the banner.
            * @param property A string representing which property to set.
            * @param value The value that will be assigned to the property.
            *
            * @returns One of the next result codes:
            * - #MA_ADS_RES_OK if no error occurred.
            * - #MA_ADS_RES_INVALID_BANNER_HANDLE if the banner handle is invalid.
            * - #MA_ADS_RES_INVALID_PROPERTY_NAME if the property name is not valid.
            * - #MA_ADS_RES_INVALID_PROPERTY_VALUE if the property value is not valid.
            */
            ioctls.maAdsBannerSetProperty = delegate(int _bannerHandle, int _property, int _value)
            {
                // check if the banner is a widget with a valid handle
                MoSync.NativeUI.Ad ad = (MoSync.NativeUI.Ad)runtime.GetModule<NativeUIModule>().GetWidget(_bannerHandle);
                if (!isHandleValid(runtime, _bannerHandle))
                {
                    return MoSync.Constants.MA_ADS_RES_INVALID_BANNER_HANDLE;
                }

                String property = core.GetDataMemory().ReadStringAtAddress(_property);
                // based on the string 'property' we set the ones that can be set on WP 7.1
                // if a property is not available, we return MA_ADS_RES_INVALID_PROPERTY_NAME
                string value = "";
                int intValue = -1;
                switch (property)
                {
                    case MoSync.Constants.MA_ADS_HEIGHT:
                        value = core.GetDataMemory().ReadStringAtAddress(_value);
                        intValue = -1;
                        int.TryParse(value,out intValue);
                        if (intValue >= 0)
                        {
                            MoSync.Util.RunActionOnMainThreadSync(() =>
                                {
                                    mAd.Height = intValue;
                                }
                            );
                        }
                        else
                        {
                            return MoSync.Constants.MA_ADS_RES_INVALID_PROPERTY_VALUE;
                        }
                        break;
                    case MoSync.Constants.MA_ADS_WIDTH:
                        value = core.GetDataMemory().ReadStringAtAddress(_value);
                        intValue = -1;
                        int.TryParse(value, out intValue);
                        if (intValue >= 0)
                        {
                            MoSync.Util.RunActionOnMainThreadSync(() =>
                                {
                                    mAd.Width = intValue;
                                }
                            );
                        }
                        else
                        {
                            return MoSync.Constants.MA_ADS_RES_INVALID_PROPERTY_VALUE;
                        }
                        break;
                    case MoSync.Constants.MA_ADS_VISIBLE:
                        value = core.GetDataMemory().ReadStringAtAddress(_value).ToLower();
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
                        break;
                    case MoSync.Constants.MA_ADS_ENABLED:
                        value = core.GetDataMemory().ReadStringAtAddress(_value).ToLower();
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
                        break;
                    case MoSync.Constants.MA_ADS_COLOR_BG:
                        value = core.GetDataMemory().ReadStringAtAddress(_value);
                        if (!IsHexColor(value))
                        {
                            return MoSync.Constants.MA_ADS_RES_INVALID_PROPERTY_VALUE;
                        }
                        MoSync.Util.RunActionOnMainThreadSync(() =>
                            {
                                mAd.BackgroundColor = value;
                            }
                        );
                        break;
                    case MoSync.Constants.MA_ADS_COLOR_BORDER:
                        value = core.GetDataMemory().ReadStringAtAddress(_value);
                        if (!IsHexColor(value))
                        {
                            return MoSync.Constants.MA_ADS_RES_INVALID_PROPERTY_VALUE;
                        }
                        MoSync.Util.RunActionOnMainThreadSync(() =>
                            {
                                mAd.BorderColor = value;
                            }
                        );
                        break;
                    case MoSync.Constants.MA_ADS_COLOR_TEXT:
                        value = core.GetDataMemory().ReadStringAtAddress(_value);
                        if (!IsHexColor(value))
                        {
                            return MoSync.Constants.MA_ADS_RES_INVALID_PROPERTY_VALUE;
                        }
                        MoSync.Util.RunActionOnMainThreadSync(() =>
                            {
                                mAd.TextColor = value;
                            }
                        );
                        break;
                    default:
                        return MoSync.Constants.MA_ADS_RES_INVALID_PROPERTY_NAME;
                }

                return MoSync.Constants.MA_ADS_RES_OK;
            };

            /**
            * @brief Retrieves a specified property from the given banner.
            *
            * @param bannerHandle Handle to the banner.
            * @param property A string representing for which property to get the value.
            * @param value A buffer that will hold the value of the property, represented as a string.
            * @param bufSize Size of the buffer.
            *
            * @returns One of the next result codes:
            * - #MA_ADS_RES_OK if no error occurred.
            * - #MA_ADS_RES_INVALID_BANNER_HANDLE if the banner handle is invalid.
            * - #MA_ADS_RES_INVALID_PROPERTY_NAME if the property name is not valid.
            * - #MA_ADS_RES_INVALID_STRING_BUFFER_SIZE if the buffer size was to small.
            */
            ioctls.maAdsBannerGetProperty = delegate(int _bannerHandle, int _property, int _value, int _bufSize)
            {
                MoSync.NativeUI.Ad ad = (MoSync.NativeUI.Ad)runtime.GetModule<NativeUIModule>().GetWidget(_bannerHandle);
                if (!isHandleValid(runtime, _bannerHandle))
                {
                    return MoSync.Constants.MA_ADS_RES_INVALID_BANNER_HANDLE;
                }

                String property = core.GetDataMemory().ReadStringAtAddress(_property);
                string stringvalue = "";
                switch (property)
                {
                    case MoSync.Constants.MA_ADS_HEIGHT:
                        stringvalue = "";
                        MoSync.Util.RunActionOnMainThreadSync(() =>
                            {
                                stringvalue = ((int)mAd.Height).ToString();
                            }
                        );
                        core.GetDataMemory().WriteStringAtAddress(
                            _value,
                            stringvalue,
                            _bufSize);
                        break;
                    case MoSync.Constants.MA_ADS_WIDTH:
                        stringvalue = "";
                        MoSync.Util.RunActionOnMainThreadSync(() =>
                            {
                                stringvalue = ((int)mAd.Width).ToString();
                            }
                        );
                        core.GetDataMemory().WriteStringAtAddress(
                            _value,
                            stringvalue,
                            _bufSize);
                        break;
                    case MoSync.Constants.MA_ADS_VISIBLE:
                        stringvalue = "";
                        MoSync.Util.RunActionOnMainThreadSync(() =>
                            {
                                stringvalue = mAd.Visible;
                            }
                        );
                        core.GetDataMemory().WriteStringAtAddress(
                            _value,
                            stringvalue,
                            _bufSize);
                        break;
                    case MoSync.Constants.MA_ADS_ENABLED:
                        stringvalue = "";
                        MoSync.Util.RunActionOnMainThreadSync(() =>
                            {
                                stringvalue = mAd.Enabled;
                            }
                        );
                        core.GetDataMemory().WriteStringAtAddress(
                            _value,
                            stringvalue,
                            _bufSize);
                        break;
                    case MoSync.Constants.MA_ADS_COLOR_BG:
                        stringvalue = "";
                        MoSync.Util.RunActionOnMainThreadSync(() =>
                            {
                                stringvalue = mAd.BackgroundColor.ToString();
                            }
                        );
                        core.GetDataMemory().WriteStringAtAddress(
                            _value,
                            stringvalue,
                            _bufSize);
                        break;
                    case MoSync.Constants.MA_ADS_COLOR_BORDER:
                        stringvalue = "";
                        MoSync.Util.RunActionOnMainThreadSync(() =>
                            {
                                stringvalue = mAd.BorderColor.ToString();
                            }
                        );
                        core.GetDataMemory().WriteStringAtAddress(
                            _value,
                            stringvalue,
                            _bufSize);
                        break;
                    case MoSync.Constants.MA_ADS_COLOR_TEXT:
                        stringvalue = "";
                        MoSync.Util.RunActionOnMainThreadSync(() =>
                            {
                                stringvalue = mAd.TextColor.ToString();
                            }
                        );
                        core.GetDataMemory().WriteStringAtAddress(
                            _value,
                            stringvalue,
                            _bufSize);
                        break;
                    default:
                        return MoSync.Constants.MA_ADS_RES_INVALID_PROPERTY_NAME;
                }

                return MoSync.Constants.MA_ADS_RES_OK;
            };
        }

        /**
         * Tests if a string is a valid hex color (ex '0xff23aa').
         * @param color The color string to be tested.
         * @returns true is the string represents a correct hex color and false otherwise.
         */
        private bool IsHexColor(string color)
        {
            string upperColor = color.ToUpper();
            Regex hexColor = new Regex("[0][X][0-9,A-F]{6}");
            if (hexColor.IsMatch(upperColor))
            {
                return true;
            }

            return false;
        }

        /*
         * Checks if a handle is a valid handler (a valid handle shouldn't be negative).
         * @param runtime The current runtime
         * @param handle The handle to be checked
         */
        private bool isHandleValid(Runtime runtime, int handle)
        {
            if (runtime.GetModule<NativeUIModule>().GetWidget(handle).GetHandle() < 0)
            {
                return false;
            }
            return true;
        }
    }
}
