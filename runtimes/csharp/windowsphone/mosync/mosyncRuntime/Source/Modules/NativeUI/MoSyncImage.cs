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
using System.IO.IsolatedStorage;

namespace MoSync
{
    namespace NativeUI
    {
        //The Image class implementation
        public class Image : WidgetBaseWindowsPhone
        {
            private System.Windows.Controls.Grid mParentCanvas;

            //Standard WP Image control
            protected System.Windows.Controls.Image mImage;

            //Standard stretch object
            protected System.Windows.Media.Stretch mStretch;

            // Image handle for MAW_IMAGE_IMAGE property.
            protected int mImageHandle = 0;

            // File image path for MAW_IMAGE_PATH property.
            protected String mImagePath;

            /**
             * The constructor
             */
            public Image()
            {
                mImage = new System.Windows.Controls.Image();
                mStretch = new System.Windows.Media.Stretch();

                mParentCanvas = new Grid();

                mImage.HorizontalAlignment = HorizontalAlignment.Left;
                mImage.VerticalAlignment = VerticalAlignment.Top;

                mStretch = System.Windows.Media.Stretch.None;
                mImage.Stretch = mStretch;

                mParentCanvas.Children.Add(mImage);
                View = mParentCanvas;
            }

            //MAW_IMAGE_IMAGE property implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_IMAGE_IMAGE)]
            public int ImageProperty
            {
                set
                {
                    //Get the resource with the specified handle
                    Resource res = mRuntime.GetResource(MoSync.Constants.RT_IMAGE, value);
                    if (null != res && null != res.GetInternalObject())
                    {
						//Create a BitmapSource object from the internal object of the resource loaded
						Object bmpSource = null;
						Object internalObj = res.GetInternalObject();
						if (internalObj is System.Windows.Media.Imaging.BitmapSource)
						{
							bmpSource = (System.Windows.Media.Imaging.BitmapSource)(res.GetInternalObject());
						}
						else if (internalObj is System.IO.Stream)
						{
							bmpSource = new System.Windows.Media.Imaging.WriteableBitmap(0, 0);
							(bmpSource as System.Windows.Media.Imaging.WriteableBitmap).SetSource((System.IO.Stream)res.GetInternalObject());
						}
						else if (internalObj is System.Windows.Media.Imaging.WriteableBitmap)
						{
							bmpSource = res.GetInternalObject();
						}

                        //The image standard object gets that as a source
                        mImage.Source = (System.Windows.Media.Imaging.BitmapSource)bmpSource;

                        mImageHandle = value;
                        mImagePath = "";
                    }
                    else throw new InvalidPropertyValueException();
                }
                get
                {
                    return mImageHandle;
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
                    else throw new InvalidPropertyValueException();
                }
            }

            //MAW_IMAGE_PATH property implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_IMAGE_PATH)]
            public String ImagePath
            {
                set
                {
                    //Take the store for the application (an image of the sandbox)
                    IsolatedStorageFile f = IsolatedStorageFile.GetUserStoreForApplication();

                    //Verify that the file exists on the isolated storage
                    if(f.FileExists(value))
                    {
                        try
                        {
                            //Create a file stream for the required file
                            IsolatedStorageFileStream fs = new IsolatedStorageFileStream(value, System.IO.FileMode.Open, f);

                            //Set the stream as a source for a new bitmap image
                            var image = new System.Windows.Media.Imaging.BitmapImage();
                            image.SetSource(fs);

                            //Set the newly created bitmap image for the image widget
                            mImage.Source = image;
                            mImagePath = value;
                            mImageHandle = 0;
                        }
                        catch
                        {
                            // There was a problem reading the image file.
                            throw new InvalidPropertyValueException();
                        }
                    }
                    //If the file does not exist throw an invalid property value exception
                   else throw new InvalidPropertyValueException();
                }
                get
                {
                    return mImagePath;
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
                bool isPropertyValid = WidgetBaseWindowsPhone.ValidateProperty(propertyName, propertyValue);

                if (propertyName.Equals("scaleMode"))
                {
                    if (!(propertyValue.Equals("none") ||
                        propertyValue.Equals("scaleXY") ||
                        propertyValue.Equals("scalePreserveAspect")))
                    {
                        isPropertyValid = false;
                    }
                }
                else if (propertyName.Equals("imagePath"))
                {
                    //Take the store for the application (an image of the sandbox)
                    IsolatedStorageFile f = IsolatedStorageFile.GetUserStoreForApplication();

                    //Verify that the file exists on the isolated storage
                    if (f.FileExists(propertyValue))
                    {
                        try
                        {
                            //Create a file stream for the required file
                            IsolatedStorageFileStream fs = new IsolatedStorageFileStream(propertyValue, System.IO.FileMode.Open, f);
                        }
                        catch
                        {
                            // There was a problem reading the image file.
                            isPropertyValid = false;
                        }
                    }
                    else
                    {
                        isPropertyValid = false;
                    }
                }

                return isPropertyValid;
            }

            #endregion
        } // end of Image class
    } // end of NativeUI namespace
} // end of MoSync namespace