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
 * @file MoSyncImage.cs
 * @author Ciprian Filipas
 *
 * @brief This represents the Image Widget implementation for the NativeUI
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
        public class Image : WidgetBaseWindowsPhone
        {
            //Standard WP Image control
            protected System.Windows.Controls.Image mImage;

            //Standard stretch object
            protected System.Windows.Media.Stretch mStretch;

            /**
             * The constructor
             */
            public Image()
            {
                mImage = new System.Windows.Controls.Image();
                mStretch = new System.Windows.Media.Stretch();

                mImage.HorizontalAlignment = HorizontalAlignment.Left;
                mImage.VerticalAlignment = VerticalAlignment.Top;

                mStretch = System.Windows.Media.Stretch.None;
                mImage.Stretch = mStretch;

                View = mImage;
            }

            //MAW_IMAGE_IMAGE property implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_IMAGE_IMAGE)]
            public string ImageProperty
            {
                set
                {
                    int val;
                    if (Int32.TryParse(value, out val))
                    {
                        //Get the resource with the specified handle
                        Resource res = mRuntime.GetResource(MoSync.Constants.RT_IMAGE, val);
                        if (null != res)
                        {
                            //Create a BitmapSource object from the internal object of the resource loaded
                            System.Windows.Media.Imaging.BitmapSource bmpSource = (System.Windows.Media.Imaging.BitmapSource)(res.GetInternalObject());

                            //The image standard object gets that as a source
                            mImage.Source = bmpSource;
                        }
                    }
                }
            }

            //MAW_IMAGE_SCALE_MODE property implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_IMAGE_SCALE_MODE)]
            public string ScaleMode
            {
                set
                {
                    if (value.Equals("none"))
                    {
                        mStretch = System.Windows.Media.Stretch.None;
                        mImage.Stretch = mStretch;
                    }
                    else if (value.Equals("scaleXY"))
                    {
                        mStretch = System.Windows.Media.Stretch.Fill;
                        mImage.Stretch = mStretch;
                    }
                    else if (value.Equals("scalePreserveAspect"))
                    {
                        mStretch = System.Windows.Media.Stretch.UniformToFill;
                        mImage.Stretch = mStretch;
                    }
                }
            }
        }
    }
}