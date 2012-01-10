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

            //The constructor
            public Screen()
            {
                mPage = new PhoneApplicationPage();
                mView = mPage;

                /**
                 * This will add a BackKeyPress event handler to the Application.Current.RootVisual, this is application wide
                 */
                (Application.Current.RootVisual as Microsoft.Phone.Controls.PhoneApplicationFrame).BackKeyPress += new EventHandler<System.ComponentModel.CancelEventArgs>(BackKeyPressHandler);
            }

            /**
             * The BackKeyPress event handler
             * Currently it contains the functionality for the back event when a StackScreen is a child of a TabScreen
             * When this handler does not cover the functionality required it should be updated
             * @param from Object the object that triggers the event
             * @param args System.ComponentModel.CancelEventArgs the event arguments
             */
            private void BackKeyPressHandler(object from, System.ComponentModel.CancelEventArgs args)
            {
                //Will check if the event is not canceled
                if (false == args.Cancel)
                {
                    //If the caller screen is a TabScreen, otherwise pass the event to the parent
                    if (this is TabScreen)
                    {
                        Microsoft.Phone.Controls.Pivot pivot = ((this as TabScreen).mPivot);
                        //If the selected tab is a StackScreen
                        if (this.mChildren[pivot.SelectedIndex] is StackScreen)
                        {
                            //If pop is possible
                            if ((this.mChildren[pivot.SelectedIndex] as StackScreen).StackCount() > 1 && (this.mChildren[pivot.SelectedIndex] as StackScreen).GetBackButtonEnabled() == true)
                            {
                                //Do a pop and cancel the event
                                (this.mChildren[pivot.SelectedIndex] as StackScreen).Pop();
                                args.Cancel = true;
                            }
                        }
                        //If the selected tab is not a StackScreen the application should exit
                        else
                        {
                            //Remove the event handler from the TabScreen
                            (Application.Current.RootVisual as Microsoft.Phone.Controls.PhoneApplicationFrame).BackKeyPress -= BackKeyPressHandler;
                        }
                    }
                    else if(this is StackScreen && !(this.GetParent() is TabScreen))
                    {
                        if ((this as StackScreen).StackCount() > 1 && (this as StackScreen).GetBackButtonEnabled() == true)
                        {
                            //Do a pop and cancel the event
                            (this as StackScreen).Pop();
                            args.Cancel = true;
                        }
                    }
                }
            }

            //The AddChild implementation
            public override void AddChild(IWidget child)
            {
                base.AddChild(child);
                WidgetBaseWindowsPhone w = (WidgetBaseWindowsPhone)child;
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    mPage.Content = w.View;
                });
            }

            /**
             * The RemoveChild implementation
             * @param child IWidget the "child" widget that will be removed
             */
            public override void RemoveChild(IWidget child)
            {
                base.RemoveChild(child);
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    mPage.Content = null;
                    if (0 < mChildren.Count)
                    {
                        mPage.Content = (mChildren[mChildren.Count - 1] as WidgetBaseWindowsPhone).View;
                    }
                });
            }

            /**
             * The RemoveChild implementation
             * @param index int the index of the "child" widget that will be removed
             */
            public override void RemoveChild(int index)
            {
                if (0 <= index && mChildren.Count > index)
                {
                    base.RemoveChild(index);
                    MoSync.Util.RunActionOnMainThreadSync(() =>
                    {
                        if (0 < mChildren.Count)
                        {
                            mPage.Content = (mChildren[mChildren.Count - 1] as WidgetBaseWindowsPhone).View;
                        }
                    });
                }
            }

            /**
             * Show function implementation
             */
            public void Show()
            {
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    PhoneApplicationFrame frame = (PhoneApplicationFrame)Application.Current.RootVisual;
                    frame.Content = mPage;
                });
            }

            /**
             * MAW_SCREEN_TITLE property implementation
             */
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
