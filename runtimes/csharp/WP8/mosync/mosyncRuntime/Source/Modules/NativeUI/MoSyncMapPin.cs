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
 * @file MoSyncMapPin.cs
 * @author Spiridon Alexandru
 *
 * @brief This represents a map pin that can be added on a map widget.
 *        component on Windows Phone 7, language C#
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
using Microsoft.Phone.Controls;
using Microsoft.Phone.Controls.Maps;
using System.Collections.Generic;

namespace MoSync
{
    namespace NativeUI
    {
        /**
         * The MapPin class defines the attributes and behavior of a map pushpin.
         */
        public class MapPin : WidgetBaseWindowsPhone
        {
            Microsoft.Phone.Controls.Maps.Pushpin mPushpin;

            /**
             * Constructor
             */
            public MapPin()
            {
                mPushpin = new Microsoft.Phone.Controls.Maps.Pushpin();
                mPushpin.Location = new System.Device.Location.GeoCoordinate();

                mView = mPushpin;

                mPushpin.MouseLeftButtonDown += new MouseButtonEventHandler(
                    delegate(object from, MouseButtonEventArgs args)
                    {
                        /**
                         * post the event to MoSync runtime
                         */
                        Memory eventData = new Memory(8);
                        const int MAWidgetEventData_eventType = 0;
                        const int MAWidgetEventData_widgetHandle = 4;
                        eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_MAP_PIN_CLICKED);
                        eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                        mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                    }
                );
            }

            /**
             * Property for getting the Pushpin object in order to add it on the map.
             */
            public Pushpin Pushpin
            {
                get
                {
                    return mPushpin;
                }
            }

            /**
             * Property for setting the map pin latitude coordinate.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_MAP_PIN_LATITUDE)]
            public string Latitude
            {
                set
                {
                    double latitude;
                    if (Double.TryParse(value, out latitude))
                    {
                        mPushpin.Location.Latitude = latitude;
                    }
                    else
                    {
                        throw new InvalidPropertyValueException();
                    }
                }
            }

            /**
             * Property for setting the map pin longitude coordinate.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_MAP_PIN_LONGITUDE)]
            public string Longitude
            {
                set
                {
                    double longitude;
                    if (Double.TryParse(value, out longitude))
                    {
                        mPushpin.Location.Longitude = longitude;
                    }
                    else
                    {
                        throw new InvalidPropertyValueException();
                    }
                }
            }

            /**
             * Property for setting and getting the map pin text.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_MAP_PIN_TEXT)]
            public string Text
            {
                set
                {
                    mPushpin.Content = value;
                }
                get
                {
                    return (string)mPushpin.Content;
                }
            }
        } // end of MapPin class
    } // end of NativeUI namespace
} // end of MoSync namespace
