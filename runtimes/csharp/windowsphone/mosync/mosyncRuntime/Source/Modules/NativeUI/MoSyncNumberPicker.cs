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
 * @file MoSyncNumberPicker.cs
 * @author Filipas Ciprian
 *
 * @brief This represents the Number Picker (MoSync Widget) implementation for the NativeUI
 *        component on Windows Phone 7, language C#
 *
 * @platform WP 7.1
 **/

using System;

namespace MoSync
{
    namespace NativeUI
    {
        public class NumberPicker : WidgetBaseWindowsPhone
        {
            // The native component.
            private CustomNumberPicker mPicker;

            // The constructor.
            public NumberPicker()
            {
                mPicker = new CustomNumberPicker();
                mView = mPicker;

                Value = 0;

                // The ValueChanged event handler. This is when the MoSync event is triggered.
                mPicker.ValueChanged += new EventHandler<NumberPickerValueChangedEventArgs>(
                    delegate(object from, NumberPickerValueChangedEventArgs args)
                    {
                        Memory eventData = new Memory(12);

                        const int MAWidgetEventData_eventType = 0;
                        const int MAWidgetEventData_widgetHandle = 4;
                        const int MAWidgetEventDate_value = 8;
                        eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_NUMBER_PICKER_VALUE_CHANGED);
                        eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                        eventData.WriteInt32(MAWidgetEventDate_value, mPicker.Value.Value);

                        mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                    });
            }

            /**
             * @uathor Ciprian Filipas
             * @brief the MAW_NUMBER_PICKER_VALUE property implementation.
             */
            [MoSync.MoSyncWidgetProperty(MoSync.Constants.MAW_NUMBER_PICKER_VALUE)]
            public int Value
            {
                get
                {
                    return mPicker.Value.Value;
                }
                set
                {
                    mPicker.Value = value;
                }
            }

            /**
             * @uathor Ciprian Filipas
             * @brief the MAW_NUMBER_PICKER_MIN_VALUE property implementation.
             */
            [MoSync.MoSyncWidgetProperty(MoSync.Constants.MAW_NUMBER_PICKER_MIN_VALUE)]
            public int Min
            {
                get
                {
                    return mPicker.Min;
                }
                set
                {
                    mPicker.Min = value;
                }
            }

            /**
             * @uathor Ciprian Filipas
             * @brief the MAW_NUMBER_PICKER_MAX_VALUE property implementation.
             */
            [MoSync.MoSyncWidgetProperty(MoSync.Constants.MAW_NUMBER_PICKER_MAX_VALUE)]
            public int Max
            {
                get
                {
                    return mPicker.Max;
                }
                set
                {
                    mPicker.Max = value;
                }
            }
        }
    }
}
