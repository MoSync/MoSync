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
			public DatePicker()
			{
				mDatePicker = new Microsoft.Phone.Controls.DatePicker();
				mView = mDatePicker;

			}

			[MoSyncWidgetProperty(MoSync.Constants.MAW_DATE_PICKER_MAX_DATE)]
			public string MaxDate
			{
				set
				{

				}
			}

			[MoSyncWidgetProperty(MoSync.Constants.MAW_DATE_PICKER_MIN_DATE)]
			public string MinDate
			{
				set
				{

				}
			}

			[MoSyncWidgetProperty(MoSync.Constants.MAW_DATE_PICKER_YEAR)]
			public String Year
			{
				set
				{
					int year = 0;
					if (int.TryParse(value, out year))
					{
						System.DateTime? myVal = mDatePicker.Value;
						if (myVal.HasValue)
						{
							System.DateTime oldDate = myVal.GetValueOrDefault();
							System.DateTime newDate = new System.DateTime(year, oldDate.Month, oldDate.Day);
							mDatePicker.Value = newDate;
						}
					}
				}

				get
				{
					String currentValue = "";
					System.DateTime? myVal = mDatePicker.Value;
					if (myVal.HasValue)
					{
						System.DateTime date = myVal.GetValueOrDefault();
						currentValue = date.Year.ToString();
					}
					return currentValue;
				}
			}//end Year

			[MoSyncWidgetProperty(MoSync.Constants.MAW_DATE_PICKER_MONTH)]
			public String Month
			{
				set
				{
					int month = 0;
					if (int.TryParse(value, out month))
					{
						System.DateTime? myVal = mDatePicker.Value;
						if (myVal.HasValue)
						{
							System.DateTime oldDate = myVal.GetValueOrDefault();
							System.DateTime newDate = new System.DateTime(oldDate.Year, month, oldDate.Day);
							mDatePicker.Value = newDate;
					}
				}
			}

				get
				{
					String currentValue = "";
					System.DateTime? myVal = mDatePicker.Value;
					if (myVal.HasValue)
					{
						System.DateTime date = myVal.GetValueOrDefault();
						currentValue = date.Month.ToString();
					}
					return currentValue;
				}
			}//end Month

			[MoSyncWidgetProperty(MoSync.Constants.MAW_DATE_PICKER_DAY_OF_MONTH)]
			public String dayOfMonth
			{
				set
				{
					int day = 0;
					if (int.TryParse(value, out day))
					{
						System.DateTime? myVal = mDatePicker.Value;
						if (myVal.HasValue)
						{
							System.DateTime oldDate = myVal.GetValueOrDefault();
							System.DateTime newDate = new System.DateTime(oldDate.Year, oldDate.Month, day);
							mDatePicker.Value = newDate;
						}
					}
				}

				get
				{
					String currentValue = "";
					System.DateTime? myVal = mDatePicker.Value;
					if (myVal.HasValue)
					{
						System.DateTime date = myVal.GetValueOrDefault();
						currentValue = date.Year.ToString();
					}
					return currentValue;
				}
			}//end Day

		}//end DatePicker

	} //end NativeUI
}//end MoSync