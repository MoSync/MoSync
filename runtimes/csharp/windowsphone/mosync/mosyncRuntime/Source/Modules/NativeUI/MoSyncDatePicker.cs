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
 * @file MoSyncDatePicker.cs
 * @author Rata Gabriela
 *
 * @brief This represents the DatePicker implementation for the NativeUI
 *        component on Windows Phone 7, language C#
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
        public class DatePicker : WidgetBaseWindowsPhone
        {
            protected Microsoft.Phone.Controls.DatePicker mDatePicker;

            protected DateTime mMaxDate;
            protected DateTime mMinDate;

            public DatePicker()
            {
                mDatePicker = new Microsoft.Phone.Controls.DatePicker();
                mView = mDatePicker;

                mMaxDate = DateTime.MaxValue;
                mMinDate = DateTime.MinValue;
                mDatePicker.ValueChanged += new EventHandler<DateTimeValueChangedEventArgs>(
                    delegate(object from, DateTimeValueChangedEventArgs args)
                    {
                        Memory eventData = new Memory(20);

                        const int MAWidgetEventData_eventType = 0;
                        const int MAWidgetEventData_widgetHandle = 4;
                        const int MAWidgetEventDate_value_dayOfMonth = 8;
                        const int MAWidgetEventDate_value_month = 12;
                        const int MAWidgetEventDate_value_year = 16;
                        eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_DATE_PICKER_VALUE_CHANGED);
                        eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                        eventData.WriteInt32(MAWidgetEventDate_value_dayOfMonth, mDatePicker.Value.Value.Day);
                        eventData.WriteInt32(MAWidgetEventDate_value_month, mDatePicker.Value.Value.Month);
                        eventData.WriteInt32(MAWidgetEventDate_value_year, mDatePicker.Value.Value.Year);

                        mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                    });
            }

            [MoSyncWidgetProperty(MoSync.Constants.MAW_DATE_PICKER_MAX_DATE_YEAR)]
            public int MaxDateYear
            {
                set
                {
                    try
                    {
                        mMaxDate = mMaxDate.AddYears(-1 * (mMaxDate.Year - value));
                    }
                    catch
                    {
                        throw new InvalidPropertyValueException();
                    }
                }
                get
                {
                    return mMaxDate.Year;
                }
            }

            [MoSyncWidgetProperty(MoSync.Constants.MAW_DATE_PICKER_MAX_DATE_MONTH)]
            public int MaxDateMonth
            {
                set
                {
                    if (value <= 12 && value >= 1)
                    {
                        mMaxDate = mMaxDate.AddMonths(-1 * (mMaxDate.Month - value));
                    }
                    else throw new InvalidPropertyValueException();
                }
                get
                {
                    return mMaxDate.Month;
                }
            }

            [MoSyncWidgetProperty(MoSync.Constants.MAW_DATE_PICKER_MAX_DATE_DAY)]
            public int MaxDateDay
            {
                set
                {
                    int month = mMaxDate.Month;

                    mMaxDate = mMaxDate.AddDays(value - mMaxDate.Day);

                    //if the month have changed it means that the day value was not valid.
                    if (month != mMaxDate.Month) throw new InvalidPropertyValueException();
                }
                get
                {
                    return mMaxDate.Day;
                }
            }

            [MoSyncWidgetProperty(MoSync.Constants.MAW_DATE_PICKER_MIN_DATE_YEAR)]
            public int MinDateYear
            {
                set
                {
                    try
                    {
                        mMinDate = mMinDate.AddYears(-1 * (mMinDate.Year - value));
                    }
                    catch
                    {
                        throw new InvalidPropertyValueException();
                    }
                }
                get
                {
                    return mMinDate.Year;
                }
            }

            [MoSyncWidgetProperty(MoSync.Constants.MAW_DATE_PICKER_MIN_DATE_MONTH)]
            public int MinDateMonth
            {
                set
                {
                    if (value <= 12 && value >= 1)
                    {
                        mMinDate = mMinDate.AddMonths(-1 * (mMinDate.Month - value));
                    }
                    else throw new InvalidPropertyValueException();
                }
                get
                {
                    return mMinDate.Month;
                }
            }

            [MoSyncWidgetProperty(MoSync.Constants.MAW_DATE_PICKER_MIN_DATE_DAY)]
            public int MinDateDay
            {
                set
                {
                    int month = mMinDate.Month;

                    mMinDate = mMinDate.AddDays(value - mMinDate.Day);

                    //if the month have changed it means that the day value was not valid.
                    if (month != mMinDate.Month) throw new InvalidPropertyValueException();
                }
                get
                {
                    return mMinDate.Day;
                }
            }

            [MoSyncWidgetProperty(MoSync.Constants.MAW_DATE_PICKER_YEAR)]
            public int Year
            {
                set
                {
                    System.DateTime? myVal = mDatePicker.Value;
                    if (myVal.HasValue)
                    {
                        mDatePicker.Value = mDatePicker.Value.Value.AddYears(-1 * (mDatePicker.Value.Value.Year - value));
                    }
                }

                get
                {
                    System.DateTime? myVal = mDatePicker.Value;
                    if (myVal.HasValue)
                    {
                        return myVal.GetValueOrDefault().Year;
                    }
                    return 0;
                }
            }//end Year

            [MoSyncWidgetProperty(MoSync.Constants.MAW_DATE_PICKER_MONTH)]
            public int Month
            {
                set
                {
                    System.DateTime? myVal = mDatePicker.Value;
                    if (myVal.HasValue)
                    {
                        int year = mDatePicker.Value.Value.Year;

                        mDatePicker.Value = mDatePicker.Value.Value.AddMonths(-1 * (mDatePicker.Value.Value.Month - value));

                        //if the year have changed it means that the day value was not valid.
                        if (year != mDatePicker.Value.Value.Year) throw new InvalidPropertyValueException();
                    }
                }

                get
                {
                    System.DateTime? myVal = mDatePicker.Value;
                    if (myVal.HasValue)
                    {
                        return myVal.GetValueOrDefault().Month;
                    }
                    return 0;
                }
            }//end Month

            [MoSyncWidgetProperty(MoSync.Constants.MAW_DATE_PICKER_DAY_OF_MONTH)]
            public int dayOfMonth
            {
                set
                {
                    System.DateTime? myVal = mDatePicker.Value;
                    if (myVal.HasValue)
                    {
                        int month = mDatePicker.Value.Value.Month;

                        mDatePicker.Value = mDatePicker.Value.Value.AddDays(-1 * (mDatePicker.Value.Value.Day - value));

                        //if the month have changed it means that the day value was not valid.
                        if (month != mDatePicker.Value.Value.Month) throw new InvalidPropertyValueException();
                    }
                }

                get
                {
                    System.DateTime? myVal = mDatePicker.Value;
                    if (myVal.HasValue)
                    {
                        return myVal.GetValueOrDefault().Day;
                    }
                    return 0;
                }
            }//end Day

        }//end DatePicker

    } //end NativeUI
}//end MoSync