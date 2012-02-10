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
 * @file MoSyncAd.cs
 * @author Spiridon Alexandru
 *
 * @brief This represents the Ad implementation for the NativeUI
 *        component on Windows Phone 7.1, language c#
 *
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
using Microsoft.Advertising.Mobile.UI;
using Microsoft.Advertising.Mobile;

namespace MoSync
{
    namespace NativeUI
    {
        public class Ad : WidgetBaseWindowsPhone
        {
            //Standard WP Image control
            protected Microsoft.Advertising.Mobile.UI.AdControl mAd;

            /**
             * The constructor
             */
            public Ad()
            {
                mAd = new Microsoft.Advertising.Mobile.UI.AdControl("1e187355-ffe2-4842-bf48-8bd96d89739d",
                                                                    "81674",
                                                                    true);

                mAd.Width = 480;
                mAd.Height = 80;
                mAd.HorizontalAlignment = HorizontalAlignment.Left;
                mAd.VerticalAlignment = VerticalAlignment.Top;
                View = mAd;
            }
        }
    }
}
