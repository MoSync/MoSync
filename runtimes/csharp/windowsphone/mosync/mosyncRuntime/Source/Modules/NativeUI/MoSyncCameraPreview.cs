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
 * @file MoSyncCameraPreview.cs
 * @author Niklas Nummelin
 *
 * @brief This represents the Camera Preview Widget implementation for the NativeUI
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
        //The Image class implementation
        public class CameraPreview : WidgetBaseWindowsPhone
        {
            //Standard WP Image control
			protected System.Windows.Controls.Canvas mViewFinder;
            protected System.Windows.Controls.Grid mLayout;

            /**
             * The constructor
             */
			public CameraPreview()
            {
                mLayout = new Grid();
                mLayout.RowDefinitions.Add(new RowDefinition());
                mLayout.RowDefinitions[0].Height = new GridLength(1, GridUnitType.Star);

                mViewFinder = new System.Windows.Controls.Canvas();
                mViewFinder.Opacity = 0.0;

                mLayout.Children.Add(mViewFinder);

                Grid.SetRow(mViewFinder, 0);

                View = mLayout;
            }

            public void StopViewFinder()
            {
                mViewFinder.Opacity = 0.0;
            }

            public void StartViewFinder()
            {
                mViewFinder.Opacity = 1.0;
            }

            public void SetViewFinderContent(System.Windows.Media.Brush content)
            {
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    mViewFinder.Background = content;
                });
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
        } // end of CameraPreview class
    } // end of NativeUI namespace
} // end of MoSync namespace