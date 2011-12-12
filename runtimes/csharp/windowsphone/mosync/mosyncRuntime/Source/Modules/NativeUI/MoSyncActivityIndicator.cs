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
                mActivityIndicator.IsIndeterminate = true;
                //mActivityIndicator.Visibility = Visibility.Collapsed;

                mView = mActivityIndicator;
            }

            /**
             * Implemention of the "InProgress" property
             * Hide or show the ActivityIndicator
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_ACTIVITY_INDICATOR_IN_PROGRESS)]
            public bool inProgress
            {
                set
                {
                    mActivityIndicator.IsIndeterminate = value;
                    mActivityIndicator.Visibility = Visibility.Visible;//(true == value) ? Visibility.Visible : Visibility.Collapsed;
                }
            }

        } // end of class ActivityIndicator
    } // end of namespace NativeUI
} // end of namespace MoSync