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
 * @file MoSyncSlider.cs
 * @author Ciprian Filipas
 *
 * @brief This represents the Slider Widget implementation for the NativeUI
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
        //The slider class
        public class Slider : WidgetBaseWindowsPhone
        {
            protected System.Windows.Controls.Slider mSlider;
            protected int mMaxValue;
            protected int mMinValue;
            protected int mProgressValue;

            public static int DEFAULT_MAX_VALUE = 100;
            public static int DEFAULT_MIN_VALUE = 0;

            //the constructor
            public Slider()
            {
                mSlider = new System.Windows.Controls.Slider();

                mSlider.Maximum = DEFAULT_MAX_VALUE;
                mSlider.Minimum = DEFAULT_MIN_VALUE;

                mMaxValue = DEFAULT_MAX_VALUE;
                mMinValue = DEFAULT_MIN_VALUE;
                mProgressValue = DEFAULT_MIN_VALUE;

                mView = mSlider;

                //the event handler
                mSlider.ValueChanged += new RoutedPropertyChangedEventHandler<double>(
                    delegate(Object from, RoutedPropertyChangedEventArgs<double> arg)
                    {
                        ////click event needs a memory chunk of 12 bytes
                        Memory eventData = new Memory(12);

                        //starting with the 0 Byte we write the eventType
                        const int MAWidgetEventData_eventType = 0;
                        //starting with the 4 Byte we write the sliderValue
                        const int MAWidgetEventData_sliderValue = 4;
                        //starting with the 8 Byte we write the widgetHandle
                        const int MAWidgetEventData_widgetHandle = 8;

                        mProgressValue = ((Int32)arg.NewValue);

                        eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_SLIDER_VALUE_CHANGED);
                        eventData.WriteInt32(MAWidgetEventData_sliderValue, mProgressValue);
                        eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                        //posting a CustomEvent
                        mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                    });
            }

            //MAW_SLIDER_MAX implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_SLIDER_MAX)]
            public string Max
            {
                set
                {
                    int maxVal;
                    if(Int32.TryParse(value, out maxVal))
                    {
                        if (0 <= maxVal)
                        {
                            mMaxValue = maxVal;
                            if (maxVal < mProgressValue)
                            {
                                mProgressValue = maxVal;
                                Value = maxVal.ToString();
                            }
                            mSlider.Maximum = mMaxValue;
                        }
                        else
                        {
                            mMaxValue = 0;
                            mSlider.Maximum = mMaxValue;
                        }
                    }
                }
                get
                {
                    return mMaxValue.ToString();
                }
            }

            //MAW_SLIDER_VALUE implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_SLIDER_VALUE)]
            public string Value
            {
                set
                {
                    int val;
                    if (Int32.TryParse(value, out val))
                    {
                        if (val <= mMaxValue && val >= mMinValue)
                        {
                            mProgressValue = val;
                            mSlider.Value = mProgressValue;
                        }
                        else if (val > mMaxValue)
                        {
                            mSlider.Value = mMaxValue;
                            mProgressValue = mMaxValue;
                        }
                        else if (val < mMinValue)
                        {
                            mSlider.Value = mMinValue;
                            mProgressValue = mMinValue;
                        }
                    }
                }
                get
                {
                    return mProgressValue.ToString();
                }
            }

            //MAW_SLIDER_INCREASE_VALUE implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_SLIDER_INCREASE_VALUE)]
            public string IncreaseValue
            {
                set
                {
                    int val;
                    if (Int32.TryParse(value, out val))
                    {
                        Value = (mProgressValue + val).ToString();
                    }
                }
            }

            //MAW_SLIDER_DECREASE_VALUE implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_SLIDER_DECREASE_VALUE)]
            public string DecreaseValue
            {
                set
                {
                    int val;
                    if (Int32.TryParse(value, out val))
                    {
                        Value = (mProgressValue - val).ToString();
                    }
                }
            }
        }
    }
}
