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
 * @file MoSyncScreen.cs
 * @author Niklas Nummelin, Ciprian Filipas
 *
 * @brief This represents the Screen implementation for the NativeUI
 *        component on Windows Phone 7, language c#
 * @note The Icon property cannot be implemented on Windows Phone
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
        public class Screen : WidgetBaseWindowsPhone, IScreen
        {
            protected PhoneApplicationPage mPage;
            public Screen()
            {
                mPage = new PhoneApplicationPage();
                mView = mPage;
            }

            public override void AddChild(IWidget child)
            {
                base.AddChild(child);
                WidgetBaseWindowsPhone w = (WidgetBaseWindowsPhone)child;
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    mPage.Content = w.View;
                    ((System.Windows.FrameworkElement)w.View).Width = ScreenSize.SCREEN_WIDTH;
                    ((System.Windows.FrameworkElement)w.View).Height = ScreenSize.SCREEN_HEIGHT;
                });
            }

            public void Show()
            {
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    PhoneApplicationFrame frame = (PhoneApplicationFrame)Application.Current.RootVisual;
                    frame.Content = mPage;
                });
            }

            //MAW_SCREEN_TITLE property implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_SCREEN_TITLE)]
            public String Title
            {
                set
                {
                    mPage.Title = value;
                }
            }
        }
    }
}
