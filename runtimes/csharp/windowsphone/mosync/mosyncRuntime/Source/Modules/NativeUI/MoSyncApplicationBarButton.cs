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
 * @file MoSyncApplicationBarButton.cs
 * @author Ciprian Filipas
 *
 * @brief This represents the application bar button widget implementation for the NativeUI
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
using Microsoft.Phone;

namespace MoSync
{
    namespace NativeUI
    {
        public class ApplicationBarButton : WidgetBaseWindowsPhone
        {
            /**
             * Native button object
             */
            protected Microsoft.Phone.Shell.ApplicationBarIconButton mButton;

            /**
             * The uri of the icon
             */
            protected System.Uri mIconUri;

            /**
             * The constructor
             */
            public ApplicationBarButton()
            {
                mView = null;

                mButton = new Microsoft.Phone.Shell.ApplicationBarIconButton();
                mButton.IconUri = new Uri("", UriKind.Relative);

                //The click handle the button component
                mButton.Click += new EventHandler(
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
             * The MAW_APPLICATION_BAR_BUTTON_ICON property implementations
             * This is used for setting the icon uri for the button
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_APPLICATION_BAR_BUTTON_ICON)]
            public String Icon
            {
                set
                {
                    if(value.StartsWith("/ApplicationBarIcons"))
                    {
                        mIconUri = new Uri(value, UriKind.Relative);
                    }
                    else
                    {
                        if (value.StartsWith("/"))
                        {
                            mIconUri = new Uri("/ApplicationBarIcons" + value, UriKind.Relative);
                        }
                        else
                        {
                            mIconUri = new Uri("/ApplicationBarIcons/" + value, UriKind.Relative);
                        }
                    }
                    mButton.IconUri = mIconUri;
                }
            }

            /**
            * The MAW_APPLICATION_BAR_BUTTON_TEXT property implementations
            * This is used for setting the text for the button
            */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_APPLICATION_BAR_BUTTON_TEXT)]
            public String Text
            {
                set
                {
                    mButton.Text = value;
                }
                get
                {
                    return mButton.Text;
                }
            }

            /**
             * Function used for getting the native object
             * @return Microsoft.Phone.Shell.ApplicationBarIconButton
             */
            public Microsoft.Phone.Shell.ApplicationBarIconButton GetButton()
            {
                return mButton;
            }
        }
    }
}
