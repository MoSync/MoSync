﻿/* Copyright (C) 2012 MoSync AB

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
using System.Globalization;

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

                View = mPushpin;

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
                    IFormatProvider provider = CultureInfo.InvariantCulture;
                    try
                    {
                        double latitude = double.Parse(value, provider);
                        mPushpin.Location.Latitude = latitude;
                    }
                    catch
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
                    IFormatProvider provider = CultureInfo.InvariantCulture;
                    try
                    {
                        double longitude = double.Parse(value, provider);
                        mPushpin.Location.Longitude = longitude;
                    }
                    catch
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

                if (propertyName.Equals("latitude") ||
                    propertyName.Equals("longitude"))
                {
                    IFormatProvider provider = CultureInfo.InvariantCulture;
                    try
                    {
                        double val = double.Parse(propertyValue, provider);
                    }
                    catch
                    {
                        isPropertyValid = false;
                    }
                }

                return isPropertyValid;
            }

            #endregion
        } // end of MapPin class
    } // end of NativeUI namespace
} // end of MoSync namespace
