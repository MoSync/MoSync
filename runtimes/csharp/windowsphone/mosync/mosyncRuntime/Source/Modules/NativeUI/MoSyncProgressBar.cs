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
 * @file MoSyncProgressBar.cs
 * @author Rata Gabriela
 *
 * @brief This represents the ProgressBar Widget implementation for the NativeUI
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
using System.Windows.Media.Animation;

namespace MoSync
{
    namespace NativeUI
    {

        /**
         * The ProgressBar class, used for displaying the progress of a task over time.
         */
        public class ProgressBar : WidgetBaseWindowsPhone
        {
            /**
             * The native ProgressBar
             */
            protected System.Windows.Controls.ProgressBar mProgressBar;

            /**
             * Constructor
             */
            public ProgressBar()
            {
                mProgressBar = new System.Windows.Controls.ProgressBar();
                mProgressBar.IsIndeterminate = false;
                mView = mProgressBar;
            }

            /**
             * Implementation of the "max" property
             * Sets the upper range of the progress bar
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_PROGRESS_BAR_MAX)]
            public String Max
            {
                set
                {
                    int maxValue = 0;
                    if (int.TryParse(value, out maxValue))
                    {
                        // only positive integers are accepted
                        if (0 > maxValue) maxValue = 0;

                        mProgressBar.Maximum = maxValue;
                    }
                }
            }

            /**
             * Implementation of the "progress" property
             * set: sets the current progress value
             *      if the user value is greater than the max value, the new value will be the max value
             * get: returns the current progress value
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_PROGRESS_BAR_PROGRESS)]
            public String Progress
            {
                set
                {
                    int val = 0;
                    if (int.TryParse(value, out val))
                    {
                        mProgressBar.Value = (val <= mProgressBar.Maximum) ? val : mProgressBar.Maximum;
                    }
                }

                get
                {
                    return mProgressBar.Value.ToString();
                }
            }

            /**
             * Implemention of the "incrementProgress" property
             * Increases the progress value with the specified amount.
             * If the new value is greater than the max, than the set value is the max value.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_PROGRESS_BAR_INCREMENT_PROGRESS)]
            public String IncrementProgress
            {
                set
                {
                    int incrementVal = 0;
                    if (int.TryParse(value, out incrementVal))
                    {
                        double newValue = mProgressBar.Value + incrementVal;
                        mProgressBar.Value = (newValue > mProgressBar.Maximum) ? mProgressBar.Maximum : newValue;
                    }
                }
            }

        }
    }
}
