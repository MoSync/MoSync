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
        public class ApplicationBarMenuItem : WidgetBaseWindowsPhone
        {
            /**
             * The native menu item object
             */
            protected Microsoft.Phone.Shell.ApplicationBarMenuItem mMenuItem;

            /**
             * The constructor
             */
            public ApplicationBarMenuItem()
            {
                mView = null;

                mMenuItem = new Microsoft.Phone.Shell.ApplicationBarMenuItem();
                mMenuItem.Click += new EventHandler(
                    delegate(Object from, EventArgs evt)
                    {
                        //Click event needs a memory chunk of 8 bytes
                        Memory eventData = new Memory(8);
                        const int MAWidgetEventData_eventType = 0;
                        const int MAWidgetEventData_widgetHandle = 4;
                        eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_CLICKED);
                        eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                        //Posting a CustomEvent
                        mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                    });
            }

            /**
             * MAW_APPLICATION_BAR_MENU_ITEM_TEXT property implementation
             * This is used for setting the text of the menu item
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_APPLICATION_BAR_MENU_ITEM_TEXT)]
            public String Text
            {
                set
                {
                    mMenuItem.Text = value;
                }
                get
                {
                    return mMenuItem.Text;
                }
            }

            /**
             * Function used for getting the native menu item
             * @return Microsoft.Phone.Shell.ApplicationBarMenuItem
             */
            public Microsoft.Phone.Shell.ApplicationBarMenuItem GetMenuItem()
            {
                return mMenuItem;
            }
        }
    }
}
