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
using System.Collections.Generic;
using Microsoft.Phone.Controls.Maps.Platform;

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

            // we need to keep the zoom level of the map center in order
            // to correctly center the map
            int mCenterZoomLevel = 0;

            // used to center the map on an area described by the upper left corner and the lower right corner
            System.Device.Location.GeoCoordinate mVisibleAreaUpperLeftCorner;

            // used to center the map on an area described by the upper left corner and the lower right corner
            System.Device.Location.GeoCoordinate mVisibleAreaLowerRightCorner;

            /**
             * Constructor
             */
            public Map()
            {
                mBingMap = new Microsoft.Phone.Controls.Maps.Map();

                mVisibleAreaUpperLeftCorner = new System.Device.Location.GeoCoordinate();
                mVisibleAreaLowerRightCorner = new System.Device.Location.GeoCoordinate();

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
             * Adds a pushpin on the map.
             */
            public override void AddChild(IWidget child)
            {
                if (child is MapPin)
                {
                    MoSync.Util.RunActionOnMainThreadSync(() =>
                    {
                        MapPin pin = (MapPin)child;
                        mBingMap.Children.Add(pin.Pushpin);
                    });
                }
            }

            /**
             * Removes a pushpin from the map.
             */
            public override void RemoveChild(IWidget child)
            {
                if (child is MapPin)
                {
                    MoSync.Util.RunActionOnMainThreadSync(() =>
                    {
                        MapPin pin = (MapPin)child;
                        mBingMap.Children.Remove(pin.Pushpin);
                    });
                }
            }

            /**
             * Property for setting the map credentials (api key).
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_MAP_API_KEY_BING)]
            public string ApiKey
            {
                set
                {
                    mBingMap.CredentialsProvider = new ApplicationIdCredentialsProvider(value);
                }
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
                    else if (MoSync.Constants.MAW_MAP_TYPE_SATELLITE == value)
                    {
                        mBingMap.Mode = new AerialMode();
                    }
                    else
                    {
                        throw new InvalidPropertyValueException();
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
                    else if (value.Equals("false"))
                    {
                        mBingMap.IsHitTestVisible = false;
                    }
                    else
                    {
                        throw new InvalidPropertyValueException();
                    }
                }
                get
                {
                    if (mBingMap.IsHitTestVisible)
                    {
                        return "true";
                    }
                    else
                    {
                        return "false";
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

            /**
             * Property for setting the map center latitude coordinate.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_MAP_CENTER_LATITUDE)]
            public string CenterLatitude
            {
                set
                {
                    double latitude;
                    if (Double.TryParse(value, out latitude))
                    {
                        mBingMap.Center.Latitude = latitude;
                    }
                    else
                    {
                        throw new InvalidPropertyValueException();
                    }
                }
            }

            /**
             * Property for setting the map center longitude coordinate.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_MAP_CENTER_LONGITUDE)]
            public string CenterLongitude
            {
                set
                {
                    double longitude;
                    if (Double.TryParse(value, out longitude))
                    {
                        mBingMap.Center.Longitude = longitude;
                    }
                    else
                    {
                        throw new InvalidPropertyValueException();
                    }
                }
            }

            /**
              * Property for setting the map center zoom level.
              */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_MAP_CENTER_ZOOM_LEVEL)]
            public string CenterZoomLevel
            {
                set
                {
                    int zoomLevel;
                    if (Int32.TryParse(value, out zoomLevel))
                    {
                        mCenterZoomLevel = zoomLevel;
                    }
                    else
                    {
                        throw new InvalidPropertyValueException();
                    }
                }
            }


            /**
             * Property for centering the map.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_MAP_CENTERED)]
            public String Centered
            {
                set
                {
                    if (value.Equals("true"))
                    {
                        List<System.Device.Location.GeoCoordinate> points = new List<System.Device.Location.GeoCoordinate>();
                        points.Add(mBingMap.Center);
                        mBingMap.SetView(LocationRect.CreateLocationRect(points));
                        mBingMap.ZoomLevel = mCenterZoomLevel;
                    }
                    else if (value.Equals("false"))
                    {
                        // do nothing
                    }
                    else
                    {
                        throw new InvalidPropertyValueException();
                    }
                }
            }

            /**
             * Property for setting the upper left corner latitude of the visible area.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_MAP_VISIBLE_AREA_UPPER_LEFT_CORNER_LATITUDE)]
            public String VisibleAreaUpperLeftCornerLatitude
            {
                set
                {
                    double latitude;
                    if (Double.TryParse(value, out latitude))
                    {
                        mVisibleAreaUpperLeftCorner.Latitude = latitude;
                    }
                    else
                    {
                        throw new InvalidPropertyValueException();
                    }
                }
                get
                {
                    return mVisibleAreaUpperLeftCorner.Latitude.ToString();
                }
            }

            /**
             * Property for setting the upper left corner longitude of the visible area.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_MAP_VISIBLE_AREA_UPPER_LEFT_CORNER_LONGITUDE)]
            public String VisibleAreaUpperLeftCornerLongitude
            {
                set
                {
                    double longitude;
                    if (Double.TryParse(value, out longitude))
                    {
                        mVisibleAreaUpperLeftCorner.Longitude = longitude;
                    }
                    else
                    {
                        throw new InvalidPropertyValueException();
                    }
                }
                get
                {
                    return mVisibleAreaUpperLeftCorner.Longitude.ToString();
                }
            }

            /**
             * Property for setting the lower right corner latitude of the visible area.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_MAP_VISIBLE_AREA_LOWER_RIGHT_CORNER_LATITUDE)]
            public String VisibleAreaLowerRightCornerLatitude
            {
                set
                {
                    double latitude;
                    if (Double.TryParse(value, out latitude))
                    {
                        mVisibleAreaLowerRightCorner.Latitude = latitude;
                    }
                    else
                    {
                        throw new InvalidPropertyValueException();
                    }
                }
                get
                {
                    return mVisibleAreaLowerRightCorner.Latitude.ToString();
                }
            }

            /**
             * Property for setting the lower right corner longitude of the visible area.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_MAP_VISIBLE_AREA_LOWER_RIGHT_CORNER_LONGITUDE)]
            public String VisibleAreaLowerRightCornerLongitude
            {
                set
                {
                    double longitude;
                    if (Double.TryParse(value, out longitude))
                    {
                        mVisibleAreaLowerRightCorner.Longitude = longitude;
                    }
                    else
                    {
                        throw new InvalidPropertyValueException();
                    }
                }
                get
                {
                    return mVisibleAreaLowerRightCorner.Longitude.ToString();
                }
            }

            /**
             * Property for centering the map on a previously set visible area.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_MAP_CENTERED_ON_VISIBLE_AREA)]
            public String CenteredOnVisibleArea
            {
                set
                {
                    if (value.Equals("true"))
                    {
                        List<System.Device.Location.GeoCoordinate> points = new List<System.Device.Location.GeoCoordinate>();
                        points.Add(mVisibleAreaUpperLeftCorner);
                        points.Add(mVisibleAreaLowerRightCorner);
                        mBingMap.SetView(LocationRect.CreateLocationRect(points));
                    }
                    else if (value.Equals("false"))
                    {
                        // do nothing
                    }
                    else
                    {
                        throw new InvalidPropertyValueException();
                    }
                }
            }
        } // end of Map class
    } // end of NativeUI namespace
} // end of MoSync namespace
