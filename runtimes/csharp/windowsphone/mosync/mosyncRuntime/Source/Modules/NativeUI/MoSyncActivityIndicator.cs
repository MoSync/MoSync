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
 * @file MoSyncActivityIndicator.cs
 * @author ovidiu
 *
 * @brief This represents the Activity Indicator Widget implementation for the NativeUI
 *        component on Windows Phone 7, language C#
 *
 * @platform WP 7.1
 **/


using System;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;

using Microsoft.Phone.Controls;

namespace MoSync
{
    namespace NativeUI
    {

        /**
         * The ProgressBar class, used for displaying the progress of a task over time.
         */
        public class ActivityIndicator : WidgetBaseWindowsPhone
        {
            /**
             * The native ProgressBar
             */
            protected Microsoft.Phone.Controls.PerformanceProgressBar mActivityIndicator;

            /**
             * Constructor
             */
            public ActivityIndicator()
            {
                mActivityIndicator = new Microsoft.Phone.Controls.PerformanceProgressBar();
                mActivityIndicator.IsIndeterminate = false;
                mActivityIndicator.Visibility = Visibility.Collapsed;

                View = mActivityIndicator;

                setHorizontalSizePolicyFlags(true, false);
                setVerticalSizePolicyFlags(false, true);
            }

            /**
             * Implemention of the "InProgress" property
             * Hide or show the ActivityIndicator
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_ACTIVITY_INDICATOR_IN_PROGRESS)]
            public String InProgress
            {
                set
                {
                    bool myValue;
                    if (bool.TryParse(value, out myValue))
                    {
                        mActivityIndicator.IsIndeterminate = myValue;
                        mActivityIndicator.Visibility = (myValue) ? Visibility.Visible : Visibility.Collapsed;
                    }
                    else throw new InvalidPropertyValueException();
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
                bool isBasePropertyValid = WidgetBaseWindowsPhone.ValidateProperty(propertyName, propertyValue);
                if (isBasePropertyValid == false)
                {
                    return false;
                }

                return true;
            }

            #endregion
        } // end of class ActivityIndicator
    } // end of namespace NativeUI
} // end of namespace MoSync