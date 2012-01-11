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
 * @file MoSyncStackScreen.cs
 * @author Ciprian Filipas
 *
 * @brief This represents the PanoramaView implementation for the NativeUI
 *        component on Windows Phone 7, language c#
 * @note The Icon property cannot be implemented on Windows Phone
 * @platform WP 7.1
 **/
using System;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;

namespace MoSync
{
    namespace NativeUI
    {
        public class PanoramaView : Screen
        {
            protected Microsoft.Phone.Controls.Panorama mPanorama;

            /**
             * The constructor
             */
            public PanoramaView()
            {
                mPanorama = new Microsoft.Phone.Controls.Panorama();

                mPage.Content = mPanorama;
            }

            /**
             * Override of the AddChild function, add a "tab" to the Screen
             * @param child IWidget the "child" that needs to be removed
             */
            public override void AddChild(IWidget child)
            {
                if (child is Screen)
                {
                    MoSync.Util.RunActionOnMainThreadSync(() =>
                    {
                        mPanorama.Items.Add(new Microsoft.Phone.Controls.PanoramaItem
                        {
                            Header = ((child as Screen).View as Microsoft.Phone.Controls.PhoneApplicationPage).Title,
                            Content = (child as Screen).View
                        });
                    });
                }
                mChildren.Add(child);
            }

            /**
             * Override of the RemoveChild function, removes a "tab" from the Screen
             * @param child IWidget the "child" that needs to be removed
             */
            public override void RemoveChild(IWidget child)
            {
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    for (int i = 0; i < mPanorama.Items.Count; i++)
                    {
                        if ((mPanorama.Items[i] as Microsoft.Phone.Controls.PanoramaItem).Content.Equals((child as Screen).View))
                        {
                            mPanorama.Items.RemoveAt(i);
                            break;
                        }
                    }
                });
                mChildren.Remove(child);
            }

            /**
             * Override of the RemoveChild function, removes a "tab" from the Screen
             * @param index int the index of the "child" that needs to be removed
             */
            public override void RemoveChild(int index)
            {
                if (0 <= index && mChildren.Count > index)
                {
                    MoSync.Util.RunActionOnMainThreadSync(() =>
                    {
                        mPanorama.Items.RemoveAt(index);
                    });
                    mChildren.RemoveAt(index);
                }
            }

            /**
             * MAW_PANORAMA_VIEW_BACKGROUND_IMAGE property implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_PANORAMA_VIEW_BACKGROUND_IMAGE)]
            public String BackgroundImage
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
                            object internalObj = res.GetInternalObject();
                            System.Windows.Media.Imaging.BitmapSource bmpSource;
                            if (internalObj is Memory)
                            {
                                bmpSource = (System.Windows.Media.Imaging.BitmapSource)(Util.CreateWriteableBitmapFromMemory(internalObj as Memory));
                            }
                            else
                            {
                                bmpSource = (System.Windows.Media.Imaging.BitmapSource)internalObj;
                            }

                            //The ImageBrush standard object gets that as a source
                            System.Windows.Media.ImageBrush imgBrush = new System.Windows.Media.ImageBrush();
                            imgBrush.ImageSource = bmpSource;

                            //The panorama gets the brush as a background
                            mPanorama.Background = imgBrush;
                        }
                    }
                }
            }

            /**
             * MAW_PANORAMA_VIEW_TITLE property implementation
             * In order to avoid the property hiding from Screen you have to specify
             * the new keyword in front of the property
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_PANORAMA_VIEW_TITLE)]
            public new String Title
            {
                set
                {
                    mPanorama.Title = value;
                }
            }

            /**
             * MAW_PANORAMA_VIEW_CURRENT_SCREEN property implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_PANORAMA_VIEW_CURRENT_SCREEN)]
            public String CurrentScreen
            {
                set
                {
                    int val = 0;
                    if (Int32.TryParse(value, out val))
                    {
                        if(-1 < val && mPanorama.Items.Count > val)
                            mPanorama.DefaultItem = mPanorama.Items[val];
                    }
                }
                get
                {
                    return mPanorama.SelectedIndex.ToString();
                }
            }
        }
    }
}
