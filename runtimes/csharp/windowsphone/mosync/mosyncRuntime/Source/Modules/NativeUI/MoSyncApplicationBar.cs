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
 * @file MoSyncApplicationBarMenuItem.cs
 * @author Ciprian Filipas
 *
 * @brief This represents the application bar menu item widget implementation for the NativeUI
 *        component on Windows Phone 7, language c#
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

namespace MoSync
{
    namespace NativeUI
    {
        public class ApplicationBar : WidgetBaseWindowsPhone
        {
            /**
             * The native application bar object
             */
            protected Microsoft.Phone.Shell.ApplicationBar mApplicationBar;

            /**
             * The constructor
             */
            public ApplicationBar()
            {
                mApplicationBar = new Microsoft.Phone.Shell.ApplicationBar();

                mView = null;
            }

            /**
             * The override for the AddChild function
             * @param child IWidget
             * note: that the child must be an ApplicationBarButton or an ApplicationBarMenuItem
             *       otherwise it will be ignored
             */
            public override void AddChild(IWidget child)
            {
                if (child is ApplicationBarButton)
                {
                    mApplicationBar.Buttons.Add((child as ApplicationBarButton).GetButton());
                }
                else if (child is ApplicationBarMenuItem)
                {
                    mApplicationBar.MenuItems.Add((child as ApplicationBarMenuItem).GetMenuItem());
                }
            }

            /**
             * Function for getting the application bar native object
             * @return Microsoft.Phone.Shell.ApplicationBar
             */
            public Microsoft.Phone.Shell.ApplicationBar GetApplicationBar()
            {
                return mApplicationBar;
            }

            /**
             * MAW_APPLICATION_BAR_IS_VISIBLE property implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_APPLICATION_BAR_IS_VISIBLE)]
            public String IsVisible
            {
                set
                {
                    bool val = false;
                    if (bool.TryParse(value, out val))
                    {
                        mApplicationBar.IsVisible = val;
                    }
                }
                get
                {
                    return mApplicationBar.IsVisible.ToString();
                }
            }
        }
    }
}
