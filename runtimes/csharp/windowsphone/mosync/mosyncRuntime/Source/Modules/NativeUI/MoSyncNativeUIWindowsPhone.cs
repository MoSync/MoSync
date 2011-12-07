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
 * @file MoSyncNativeUIWindowsPhone.cs
 * @author Niklas Nummelin, Ciprian Filipas
 *
 * @brief This represents the WidgetBaseWindowsPhone implementation for the NativeUI
 *         component on Windows Phone 7, the base class for all the custom MoSync Widgets,
 *         language c#
 *
 * @platform WP 7.1
 **/

using Microsoft.Phone.Controls;
using System.Windows.Controls;
using System.Windows;
using System.Windows.Navigation;
using System;
using System.Text.RegularExpressions;
using System.Reflection;

namespace MoSync
{
    namespace NativeUI
    {
        public class ScreenSize
        {
            public static int SCREEN_HEIGHT = 800;
            public static int SCREEN_WIDTH = 480;
        }
        public class WidgetBaseWindowsPhone : WidgetBase
        {
            protected UIElement mView;

            //Fill available space flags
            public bool fillSpaceHorizontalyEnabled;
            public bool fillSpaceVerticalyEnabled;

            public UIElement View
            {
                get { return mView; }
                set { mView = value; }
            }

            /**
             * MAW_WIDGET_LEFT implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_LEFT)]
            public double Left
            {
                get { return (double)mView.GetValue(Canvas.LeftProperty); }
                set
                {
                    mView.SetValue(Canvas.LeftProperty, value);
                }
            }

            /**
             * MAW_WIDGET_TOP implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_TOP)]
            public double Top
            {
                get { return (double)mView.GetValue(Canvas.TopProperty); }
                set
                {
                    mView.SetValue(Canvas.TopProperty, value);
                }
            }

            /**
             * MAW_WIDGET_WIDTH implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_WIDTH)]
            public double Width
            {
                get { return (double)mView.GetValue(Canvas.WidthProperty); }
                set
                {
                    if (value > 0)
                    {
                        mView.SetValue(Canvas.WidthProperty, value);
                        fillSpaceHorizontalyEnabled = false;
                    }
                    else if(-1 == value)
                    {
                        mView.SetValue(Canvas.HorizontalAlignmentProperty, HorizontalAlignment.Stretch);
                        fillSpaceHorizontalyEnabled = true;

                        //In case of setting the widget property after it is added
                        //to a parent widget

                        Type objType = mView.GetType();
                        if (objType.GetProperty("Parent") != null)
                        {
                            var property = objType.GetProperty("Parent");
                            Object parent = property.GetValue(mView, null);
                            if (null != parent && parent is Grid)
                            {
                                int columnIndex = Grid.GetColumn((FrameworkElement)mView);
                                (parent as Grid).ColumnDefinitions[columnIndex].Width = new GridLength(1, GridUnitType.Star);
                            }
                        }
                    }
                    else if (-2 == value)
					{
                        mView.SetValue(Canvas.HorizontalAlignmentProperty, HorizontalAlignment.Center);
                        fillSpaceHorizontalyEnabled = false;

                        //In case of setting the widget property after it is added
                        //to a parent widget

                        Type objType = mView.GetType();
                        if (objType.GetProperty("Parent") != null)
                        {
                            var property = objType.GetProperty("Parent");
                            Object parent = property.GetValue(mView, null);
                            if (null != parent && parent is Grid)
                            {
                                int columnIndex = Grid.GetColumn((FrameworkElement)mView);
                                (parent as Grid).ColumnDefinitions[columnIndex].Width = new GridLength(1, GridUnitType.Auto);
                            }
                        }
                    }
                }
            }

            /**
             * MAW_WIDGET_HEIGHT implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_HEIGHT)]
            public double Height
            {
                get { return (double)mView.GetValue(Canvas.HeightProperty); }
                set
                {
                    if (value > 0)
                    {
                        mView.SetValue(Canvas.HeightProperty, value);
                        fillSpaceVerticalyEnabled = false;
                    }
                    else if(-1 == value)
                    {
                        mView.SetValue(Canvas.VerticalAlignmentProperty, VerticalAlignment.Stretch);
                        fillSpaceVerticalyEnabled = true;

                        //In case of setting the widget property after it is added
                        //to a parent widget
                        Type objType = mView.GetType();
                        if (objType.GetProperty("Parent") != null)
                        {
                            var property = objType.GetProperty("Parent");
                            Object parent = property.GetValue(mView, null);
                            if (null != parent && parent is Grid)
                            {
                                int rowIndex = Grid.GetRow((FrameworkElement)mView);
                                (parent as Grid).RowDefinitions[rowIndex].Height = new GridLength(1, GridUnitType.Star);
                            }
                        }
                    }
                    else if (-2 == value)
                    {
                        mView.SetValue(Canvas.VerticalAlignmentProperty, VerticalAlignment.Center);
                        fillSpaceVerticalyEnabled = false;

                        //In case of setting the widget property after it is added
                        //to a parent widget
                        Type objType = mView.GetType();
                        if (objType.GetProperty("Parent") != null)
                        {
                            var property = objType.GetProperty("Parent");
                            Object parent = property.GetValue(mView, null);
                            if (null != parent && parent is Grid)
                            {
                                int rowIndex = Grid.GetRow((FrameworkElement)mView);
                                (parent as Grid).RowDefinitions[rowIndex].Height = new GridLength(1, GridUnitType.Auto);
                            }
                        }
                    }
                }
            }

            /**
             * MAW_WIDGET_BACKGROUND_COLOR implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_BACKGROUND_COLOR)]
            public string BackgroundColor
            {
                set
                {
                    System.Windows.Media.SolidColorBrush brush;
                    MoSync.Util.convertStringToColor(value, out brush);
                    if (View is System.Windows.Controls.Control) ((System.Windows.Controls.Control)View).Background = brush;
                    else if (View is System.Windows.Controls.Panel) ((System.Windows.Controls.Panel)View).Background = brush;
                }
            }

            /**
             * MAW_WIDGET_ENABLED implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_ENABLED)]
            public string Enabled
            {
                set
                {
                    Type objType = mView.GetType();
                    if (objType.GetProperty("IsEnabled") != null)
                    {
                        bool val = Boolean.Parse(value);
                        var property = objType.GetProperty("IsEnabled");
                        property.SetValue(mView, val, null);
                    }
                }
                get
                {
                    Type objType = mView.GetType();
                    if (objType.GetProperty("IsEnabled") != null)
                    {
                        var property = objType.GetProperty("IsEnabled");
                        return (string)(property.GetValue(mView, null));
                    }
                    return null;
                }
            }

            /**
             * MAW_WIDGET_VISIBLE implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_VISIBLE)]
            public string Visible
            {
                set
                {
                    bool val = Boolean.Parse(value);
                    if (false == val) mView.Visibility = Visibility.Collapsed;
                    else mView.Visibility = Visibility.Visible;
                }
                get
                {
                    if (Visibility.Collapsed == mView.Visibility)
                    {
                        return "false";
                    }
                    else
                    {
                        return "true";
                    }
                }
            }

            /**
             * MAW_WIDGET_BACKGROUND_GRADIENT implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_BACKGROUND_GRADIENT)]
            public string BackgroundGradient
            {
                set
                {
                    System.Windows.Media.GradientStop firstGradientStop = new System.Windows.Media.GradientStop();
                    System.Windows.Media.GradientStop secondGradientStop = new System.Windows.Media.GradientStop();

                    System.Windows.Media.SolidColorBrush firstBrush;
                    Util.convertStringToColor(value.Split(',')[0], out firstBrush);

                    System.Windows.Media.SolidColorBrush secondBrush;
                    Util.convertStringToColor(value.Split(',')[1], out secondBrush);

                    firstGradientStop.Color = firstBrush.Color;
                    secondGradientStop.Color = secondBrush.Color;

                    firstGradientStop.Offset = 0.0;
                    secondGradientStop.Offset = 0.5;

                    System.Windows.Media.LinearGradientBrush brush = new System.Windows.Media.LinearGradientBrush();

                    brush.StartPoint = new Point(0.5, 0);
                    brush.EndPoint = new Point(0.5, 1);
                    brush.GradientStops.Add(firstGradientStop);
                    brush.GradientStops.Add(secondGradientStop);

                    if (View is System.Windows.Controls.Control) ((System.Windows.Controls.Control)View).Background = brush;
                    else if (View is System.Windows.Controls.Panel) ((System.Windows.Controls.Panel)View).Background = brush;
                }
            }

            /**
             * MAW_WIDGET_ALPHA implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_ALPHA)]
            public string Alpha
            {
                set
                {
                    double val = double.Parse(value);
                    mView.Opacity = val;
                }
            }

            public WidgetBaseWindowsPhone()
                : base()
            {
            }
        }

        public class NativeUIWindowsPhone : UIManager
        {
            private PhoneApplicationFrame mFrame;
            public NativeUIWindowsPhone()
                : base()
            {
                //This should always be a PhoneApplicationFrame.
                mFrame = (PhoneApplicationFrame)Application.Current.RootVisual;
            }
        }
    }
}