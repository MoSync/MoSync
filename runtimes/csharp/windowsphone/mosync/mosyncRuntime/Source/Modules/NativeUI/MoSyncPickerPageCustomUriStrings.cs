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
 * @file MoSyncPickerPageCustomUriStrings.cs
 * @author Filipas Ciprian
 *
 * @brief This file stores some helper classes. This classes are responsable for creating
 *        propper navigation uri strings for difference custom picker pages.
 *
 * @platform WP 7.1
 **/

using System;

namespace MoSync
{
    namespace NativeUI
    {
        /**
         * @author: Filipas Ciprian
         * @brief: This class represents a custom Uri string which adds the propper parameters to the navigation
         *        URI used for sending information to the custom number picker page.
         */
        public sealed class NumberPickerPageCustomUriString
        {
            public int Max
            {
                set;
                get;
            }

            public int Min
            {
                set;
                get;
            }

            // The final URI String.
            private String mUriString;

            // The base URI String
            private String mBaseUri = "/mosyncRuntime;component/Views/CustomNumberPickerPage.xaml";

            // Constructor.
            public NumberPickerPageCustomUriString()
            {
                mUriString = mBaseUri;
                Max = 0;
                Min = 100;
            }

            // Getter for the URI String. It adds the DateTime custom strings to the URI for the Max/Min date values.
            public String UriString
            {
                get
                {
                    return mUriString = mBaseUri + "?Max=" +
                                        Max +
                                        "&Min=" +
                                        Min;
                }
            }
        }

        /**
         * @author: Filipas Ciprian
         * @brief: This class represents a custom Uri string which adds the prpoper parameters to the navigation
         *        URI used for sending information to the custom date picker page.
         */
        public class DatePickerPageCustomUriString
        {

            public DateTime MaxDate
            {
                set;
                get;
            }

            public DateTime MinDate
            {
                set;
                get;
            }

            // The final URI String.
            private String mUriString;

            // The base URI String
            private String mBaseUri = "/mosyncRuntime;component/Views/CustomDatePickerPage.xaml";

            // Constructor.
            public DatePickerPageCustomUriString()
            {
                mUriString = mBaseUri;
                MaxDate = DateTime.MaxValue;
                MinDate = DateTime.MinValue;
            }

            // Getter for the URI String. It adds the DateTime custom strings to the URI for the Max/Min date values.
            public String UriString
            {
                get
                {
                    return mUriString = mBaseUri + "?MaxDate=" +
                                        MaxDate.Year.ToString() + "/" +
                                        MaxDate.Month.ToString() + "/" +
                                        MaxDate.Day.ToString() +
                                        "&MinDate=" +
                                        MinDate.Year.ToString() + "/" +
                                        MinDate.Month.ToString() + "/" +
                                        MinDate.Day.ToString();
                }
            }
        }
    }
}
