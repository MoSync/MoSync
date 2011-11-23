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
 * @file MoSyncButton.cs
 * @author Ciprian Filipas
 *
 * @brief This represents the Button Widget implementation for the NativeUI
 *        component on Windows Phone 7, language c#
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
        // The button class
        public class Button : WidgetBaseWindowsPhone
        {
            //The text content of the button
            protected String mText;

            //the vertical alignment of the content
            protected VerticalAlignment mTextVerticalAlignment;

            //the horizontal alignment of the content
            protected HorizontalAlignment mTextHorizontalAlignment;

            //the button controll
            protected System.Windows.Controls.Button mButton;

            //the button constructor
            public Button()
            {
                //initializing the button controll
                mButton = new System.Windows.Controls.Button();

                //set the view of the current widget as the previously instantiated button controll
                View = mButton;

                mButton.HorizontalAlignment = HorizontalAlignment.Left;
                mButton.VerticalAlignment = VerticalAlignment.Top;

                fillSpaceVerticalyEnabled = false;
                fillSpaceHorizontalyEnabled = false;

                //the click handle the button component
                mButton.Click += new RoutedEventHandler(
                    delegate(Object from, RoutedEventArgs evt)
                    {
                        //click event needs a memory chunk of 8 bytes
                        Memory eventData = new Memory(8);

                        //starting with the 0 Byte we write the eventType
                        const int MAWidgetEventData_eventType = 0;
                        //starting with the 4th Byte we write the widgetHandle
                        const int MAWidgetEventData_widgetHandle = 4;

                        eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_CLICKED);
                        eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                        //posting a CustomEvent
                        mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                    });
            }

            //MAW_BUTTON_TEXT property implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_BUTTON_TEXT)]
            public String Text
            {
                set
                {
                    mText = value;
                    mButton.Content = mText;
                }
                get
                {
                    return mText;
                }
            }

            //MAW_BUTTON_TEXT_HORIZONTAL_ALIGNMENT property implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_BUTTON_TEXT_HORIZONTAL_ALIGNMENT)]
            public string TextHorizontalAlignment
            {
                set
                {
                    //doing the proper behaviour for the required value
                    if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_LEFT))
                    {
                        mTextHorizontalAlignment = HorizontalAlignment.Left;
                        mButton.HorizontalContentAlignment = mTextHorizontalAlignment;
                    }
                    else
                    {
                        if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_RIGHT))
                        {
                            mTextHorizontalAlignment = HorizontalAlignment.Right;
                            mButton.HorizontalContentAlignment = mTextHorizontalAlignment;
                        }
                        else
                        {
                            if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER))
                            {
                                mTextHorizontalAlignment = HorizontalAlignment.Center;
                                mButton.HorizontalContentAlignment = mTextHorizontalAlignment;
                            }
                        }
                    }
                }
            }

            //MAW_BUTTON_TEXT_VERTICAL_ALIGNMENT property implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_BUTTON_TEXT_VERTICAL_ALIGNMENT)]
            public String TextVerticalAlignment
            {
                set
                {
                    if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_TOP))
                    {
                        mTextVerticalAlignment = VerticalAlignment.Top;
                        mButton.VerticalContentAlignment = mTextVerticalAlignment;
                    }
                    else
                    {
                        if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_BOTTOM))
                        {
                            mTextVerticalAlignment = VerticalAlignment.Bottom;
                            mButton.VerticalContentAlignment = mTextVerticalAlignment;
                        }
                        else
                        {
                            if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER))
                            {
                                mTextVerticalAlignment = VerticalAlignment.Center;
                                mButton.VerticalContentAlignment = mTextVerticalAlignment;
                            }
                        }
                    }
                }
            }

            //MAW_BUTTON_FONT_COLOR property implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_BUTTON_FONT_COLOR)]
            public String FontColor
            {
                set
                {
                    System.Windows.Media.SolidColorBrush brush;
                    MoSync.Util.convertStringToColor(value, out brush);
                    mButton.Foreground = brush;
                }
            }

            //MAW_BUTTON_FONT_SIZE property implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_BUTTON_FONT_SIZE)]
            public String FontSize
            {
                set
                {
                    double size;
                    if (double.TryParse(value, out size))
                    {
                        mButton.FontSize = size;
                    }
                }
            }
        }
    }
}
