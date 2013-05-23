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
 * @file MoSyncAd.cs
 * @author Spiridon Alexandru
 *
 * @brief This represents the Ad implementation for the NativeUI
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
using Microsoft.Advertising.Mobile.UI;
using Microsoft.Advertising.Mobile;
using System.Text;

namespace MoSync
{
    namespace NativeUI
    {
        /**
         * The Ad class defines the attributes and behavior of an Ad. It maps the
         * MoSync Ad with the AdControl present on the wp7.1 platform.
         */
        public class Ad : WidgetBaseWindowsPhone
        {
            //Standard WP Image control
            protected Microsoft.Advertising.Mobile.UI.AdControl mAd;

            /**
             * The constructor
             */
            public Ad()
            {
                mAd = new Microsoft.Advertising.Mobile.UI.AdControl();

                //add the event handlers
                mAd.ErrorOccurred += new EventHandler<Microsoft.Advertising.AdErrorEventArgs>(adControlErrorOccurred);
                mAd.IsEngagedChanged += new EventHandler(adControlEngagedChanged);
                mAd.AdRefreshed += new EventHandler(adControlRefreshed);

                mAd.HorizontalAlignment = HorizontalAlignment.Left;
                mAd.VerticalAlignment = VerticalAlignment.Top;
                View = mAd;
            }

            /**
             * Raised when the AdControl encounters an error.
             */
            void adControlErrorOccurred(object sender, Microsoft.Advertising.AdErrorEventArgs e)
            {
                int errorCode = 0;
                if (e.Error.Message.Contains("HTTP") && e.Error.Message.Contains("404"))
                {
                    errorCode = MoSync.Constants.MA_ADS_ERROR_NETWORK;
                }
                else // if (e.Error.Message.Contains("No ad available"))
                {
                    errorCode = MoSync.Constants.MA_ADS_ERROR_NO_FILL;
                }

                /**
                 * post the event to MoSync runtime
                 */
                Memory eventData = new Memory(16);
                // set the main event type: EVENT_TYPE_ADS_BANNER
                const int MAWidgetEventData_widgetEventType = 0;
                const int MAWidgetEventData_widgetHandle = 4;
                // set the banner event type: MA_ADS_EVENT_FsAILED
                const int MAWidgetEventData_eventType = 8;
                const int MAWidgetEventData_errorCode = 12;
                eventData.WriteInt32(MAWidgetEventData_widgetEventType, MoSync.Constants.EVENT_TYPE_ADS_BANNER);
                eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MA_ADS_EVENT_FAILED);
                eventData.WriteInt32(MAWidgetEventData_errorCode, errorCode);
                mRuntime.PostEvent(new Event(eventData));
            }

            /*
             * Raised when the user clicks the ad and the action dialog box appears.
             * This event pauses the application when the action dialog box appears; for example, when the user clicks the ad.
             */
            void adControlEngagedChanged(object sender, EventArgs args)
            {
                /**
                 * post the event to MoSync runtime
                 */
                Memory eventData = new Memory(16);
                // set the main event type: EVENT_TYPE_ADS_BANNER
                const int MAWidgetEventData_widgetEventType = 0;
                const int MAWidgetEventData_widgetHandle = 4;
                // set the banner event type: MA_ADS_EVENT_ON_LEAVE_APPLICATION
                const int MAWidgetEventData_eventType = 8;
                const int MAWidgetEventData_errorCode = 12;
                eventData.WriteInt32(MAWidgetEventData_widgetEventType, MoSync.Constants.EVENT_TYPE_ADS_BANNER);
                eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MA_ADS_EVENT_ON_LEAVE_APPLICATION);
                // the error code is 0 in this case
                eventData.WriteInt32(MAWidgetEventData_errorCode, 0);
                mRuntime.PostEvent(new Event(eventData));
            }

            /*
             * Raised when the AdControl receives a new ad.
             */
            void adControlRefreshed(object sender, EventArgs args)
            {
                /**
                 * post the event to MoSync runtime
                 */
                Memory eventData = new Memory(16);
                // set the main event type: EVENT_TYPE_ADS_BANNER
                const int MAWidgetEventData_widgetEventType = 0;
                const int MAWidgetEventData_widgetHandle = 4;
                // set the banner event type: MA_ADS_EVENT_LOADED
                const int MAWidgetEventData_eventType = 8;
                const int MAWidgetEventData_errorCode = 12;
                eventData.WriteInt32(MAWidgetEventData_widgetEventType, MoSync.Constants.EVENT_TYPE_ADS_BANNER);
                eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MA_ADS_EVENT_LOADED);
                // the error code is 0 in this case
                eventData.WriteInt32(MAWidgetEventData_errorCode, 0);
                mRuntime.PostEvent(new Event(eventData));
            }

            /**
             * Property for setting the ad unit id.
             */
            public string AdUnitID
            {
                set { mAd.AdUnitId = value; }
            }

            /**
             * Property for setting the application id.
             */
            public string ApplicationID
            {
                set { mAd.ApplicationId = value; }
            }

            /**
             * Property for enabling/disabling the ad control.
             */

            public new string Enabled
            {
                get
                {
                    if (mAd.IsEnabled)
                    {
                        return "true";
                    }
                    return "false";
                }
                set
                {
                    if (value.Equals("true"))
                    {
                        mAd.IsEnabled = true;
                    }
                    else
                    {
                        mAd.IsEnabled = false;
                    }
                }
            }

            /**
             * Property for setting the border color.
             */
            public string BorderColor
            {
                get { return GetStringFromColor((SolidColorBrush)mAd.BorderBrush); }
                set { mAd.BorderBrush = GetColorFromHexa(value); }
            }

            /**
             * Property for setting the text color.
             */
            public string TextColor
            {
                get { return GetStringFromColor((SolidColorBrush)mAd.Foreground); }
                set { mAd.Foreground = GetColorFromHexa(value); }
            }

            /**
             * Property for setting the background color.
             */
            public new string BackgroundColor
            {
                get { return GetStringFromColor((SolidColorBrush)mAd.Background); }
                set { mAd.Background = GetColorFromHexa(value); }
            }

            /**
             * Creates a SolidColorBrush from a string that contains a color
             * represented in hexa (ex: '0xff3421').
             */
            public SolidColorBrush GetColorFromHexa(string hexaColor)
            {
                return new SolidColorBrush(
                    Color.FromArgb(
                        Convert.ToByte("FF", 16),
                        Convert.ToByte(hexaColor.Substring(2, 2), 16),
                        Convert.ToByte(hexaColor.Substring(4, 2), 16),
                        Convert.ToByte(hexaColor.Substring(6, 2), 16)
                    )
                );
            }

            /**
             * Creates a hex string (ex: '0xad11F1') from a SolidColorBrush.
             */
            public string GetStringFromColor(SolidColorBrush color)
            {
                StringBuilder sb = new StringBuilder(6);
                sb.AppendFormat("{0:x2}", color.Color.R);
                sb.AppendFormat("{0:x2}", color.Color.G);
                sb.AppendFormat("{0:x2}", color.Color.B);
                string stringColor = sb.ToString().ToUpper();
                return "0x" + stringColor;
            }

            #region Property validation methods

            /**
             * Validates a property based on the property name and property value.
             * @param propertyName The name of the property to be checked.
             * @param propertyValue The value of the property to be checked.
             * @returns true if the property is valid, false otherwise.
             */
            public new static bool ValidateProperty(string propertyName, string propertyValue)
            {
                bool isPropertyValid = WidgetBaseWindowsPhone.ValidateProperty(propertyName, propertyValue);

                return isPropertyValid;
            }

            #endregion
        } // end of Ad class
    } // end of NativeUI namespace
} // end of MoSync namespace
