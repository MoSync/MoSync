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
 * @file MoSyncMap.cs
 * @author Spiridon Alexandru
 *
 * @brief This represents the Map Widget implementation for the NativeUI
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

namespace MoSync
{
    namespace NativeUI
    {
        /**
         * The Map class defines the attributes and behavior of a map widget.
         */
        public class Map : WidgetBaseWindowsPhone
        {
            Microsoft.Phone.Controls.Maps.Map mBingMap;

            /**
             * Constructor
             */
            public Map()
            {
                mBingMap = new Microsoft.Phone.Controls.Maps.Map();

                mView = mBingMap;

                // occurs when the map visible area is changed (on drag/scroll)
                mBingMap.MapPan += new EventHandler<MapDragEventArgs>(
                    delegate(object from, MapDragEventArgs args)
                    {
                        /**
                         * post the event to MoSync runtime
                         */
                        Memory eventData = new Memory(8);
                        const int MAWidgetEventData_eventType = 0;
                        const int MAWidgetEventData_widgetHandle = 4;
                        eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_MAP_REGION_CHANGED);
                        eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                        mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                    }
                );

                // occurs when the map zoom level is changed (on zoom in/out)
                mBingMap.MapZoom += new EventHandler<MapZoomEventArgs>(
                    delegate(object from, MapZoomEventArgs args)
                    {
                        /**
                        * post the event to MoSync runtime
                        */
                        Memory eventData = new Memory(8);
                        const int MAWidgetEventData_eventType = 0;
                        const int MAWidgetEventData_widgetHandle = 4;
                        eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_MAP_ZOOM_LEVEL_CHANGED);
                        eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                        mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                    }
                );
            }

            /**
             * Property for setting and getting the map type (the two types are MAW_MAP_TYPE_ROAD
             * and MAW_MAP_TYPE_SATELLITE).
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_MAP_TYPE)]
            public int Type
            {
                set
                {
                    if (MoSync.Constants.MAW_MAP_TYPE_ROAD == value)
                    {
                        mBingMap.Mode = new RoadMode();
                    }
                    else
                    {
                        mBingMap.Mode = new AerialMode();
                    }
                }
                get
                {
                    if (mBingMap.Mode is RoadMode)
                    {
                        return MoSync.Constants.MAW_MAP_TYPE_ROAD;
                    }
                    return MoSync.Constants.MAW_MAP_TYPE_SATELLITE;
                }
            }


            /**
             * Property for setting the iterraction mode on the map (enabled/disabled).
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_MAP_INTERRACTION_ENABLED)]
            public String InterractionEnabled
            {
                set
                {
                    if (value.Equals("true"))
                    {
                        mBingMap.IsHitTestVisible = true;
                    }
                    else
                    {
                        mBingMap.IsHitTestVisible = false;
                    }
                }
            }

            /**
             * Property for getting and setting the zoom level of the map.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_MAP_ZOOM_LEVEL)]
            public int ZoomLevel
            {
                set
                {
                    mBingMap.ZoomLevel = (double)value;
                }
                get
                {
                    return (int)mBingMap.ZoomLevel;
                }
            }
        } // end of Map class
    } // end of NativeUI namespace
} // end of MoSync namespace
