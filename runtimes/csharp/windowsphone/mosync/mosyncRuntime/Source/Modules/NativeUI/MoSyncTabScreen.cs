﻿/* Copyright (C) 2011 MoSync AB

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
 * @file MoSyncTabScreen.cs
 * @author Ciprian Filipas
 *
 * @brief This represents the TabScreen implementation for the NativeUI
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
using System.Collections.Generic;

namespace MoSync
{
    namespace NativeUI
    {
        public class TabScreen : Screen
        {
            /**
             * The TabScreen is currently implemented using a Pivot control
             */
            public Microsoft.Phone.Controls.Pivot mPivot { get; set; }

            /**
             * The constructor
             */
            public TabScreen() : base()
            {
                mPivot = new Microsoft.Phone.Controls.Pivot();

                mApplicationBarItemsIndexes = new Dictionary<Object, int>();

                //The application bar is chanded at the SelectionChanged event occurence.
                //This allows the user to have more that one application bar / tabScreen
                mPivot.LoadedPivotItem += new EventHandler<Microsoft.Phone.Controls.PivotItemEventArgs>(
                    delegate(object from, Microsoft.Phone.Controls.PivotItemEventArgs target)
                    {
                        bool appBarVisible = (this.mChildren[(from as Microsoft.Phone.Controls.Pivot).SelectedIndex] as Screen).GetApplicationBarVisibility();
                        if (appBarVisible)
                        {
                            mApplicationBar = (this.mChildren[(from as Microsoft.Phone.Controls.Pivot).SelectedIndex] as Screen).GetApplicationBar();
                            mApplicationBar.IsVisible = true;
                            ((Application.Current.RootVisual as Microsoft.Phone.Controls.PhoneApplicationFrame).Content as
                                Microsoft.Phone.Controls.PhoneApplicationPage).ApplicationBar = mApplicationBar;
                            this.SetApplicationBarVisibility(true);
                        }
                        else
                        {
                            this.SetApplicationBarVisibility(false);
                            if (((Application.Current.RootVisual as Microsoft.Phone.Controls.PhoneApplicationFrame).Content as
                                Microsoft.Phone.Controls.PhoneApplicationPage).ApplicationBar != null)
                            {
                                ((Application.Current.RootVisual as Microsoft.Phone.Controls.PhoneApplicationFrame).Content as
                                Microsoft.Phone.Controls.PhoneApplicationPage).ApplicationBar.IsVisible = false;
                            }
                        }
                    });

                //Setting the content of the View property of a Screen (which is a PhoneApplicationPage)
                //as the Pivot control
                mPage.Children.Add(mPivot);
                Grid.SetColumn(mPivot, 0);
                Grid.SetRow(mPivot, 0);
            }

            /**
             * Override of the AddChild function, add a "tab" to the Screen
             */
            public override void AddChild(IWidget child)
            {
                if (child is Screen)
                {
                    MoSync.Util.RunActionOnMainThreadSync(() =>
                        {
                            //pivotItem.Content = (child as Screen);
                            mPivot.Items.Add(new Microsoft.Phone.Controls.PivotItem
                            {
                                Header = (child as Screen).getScreenTitle,
                                Content = (child as Screen).View
                            });
                        }
                    );
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
                        for (int i = 0; i < mPivot.Items.Count; i++)
                        {
                            if ((mPivot.Items[i] as Microsoft.Phone.Controls.PivotItem).Content.Equals((child as Screen).View))
                            {
                                mPivot.Items.RemoveAt(i);
                                child.SetParent(null);
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
                        mPivot.Items.RemoveAt(index);
                    });
                    mChildren.RemoveAt(index);
                }
            }

            /**
             * Searches for a screen inside the children array, gets the proper pivot item
             * for that screen and then updates its header based on the child screen title.
             * @param childScreen The child screen that needs a title update.
             */
            public void UpdateScreenTitle(Screen childScreen)
            {
                // the index of the current screen inside the pivot control
                int index = -1;
                bool foundScreen = false;

                for (int i = 0; i < mChildren.Count; i++)
                {
                    // if a screen is inside the children array, it means it's a
                    // visible pivot item so we can increment the pivot item index
                    if (mChildren[i] is Screen)
                    {
                        index++;
                        if (mChildren[i].Equals(childScreen))
                        {
                            // we found the child screen
                            foundScreen = true;
                            break;
                        }
                    }
                }

                if (foundScreen && mPivot.Items[index] is Microsoft.Phone.Controls.PivotItem)
                {
                    Microsoft.Phone.Controls.PivotItem item = mPivot.Items[index] as Microsoft.Phone.Controls.PivotItem;
                    item.Header = childScreen.Title;
                }
            }

            /**
             * MAW_TAB_SCREEN_TITLE property implementation
             * In order to avoid the property hiding from Screen you have to specify
             * the new keyword in front of the property
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_TAB_SCREEN_TITLE)]
            public new String Title
            {
                set
                {
                    mPivot.Title = value;
                }
            }

            /**
             * MAW_TAB_SCREEN_CURRENT_TAB property implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_TAB_SCREEN_CURRENT_TAB)]
            public int CurrentTab
            {
                set
                {
                    if (value < mPivot.Items.Count)
                    {
                        mPivot.SelectedIndex = value;

                        //MAW_EVENT_TAB_CHANGED event needs a memory chunk of 12 bytes
                        Memory eventData = new Memory(12);
                        const int MAWidgetEventData_eventType = 0;
                        const int MAWidgetEventData_widgetHandle = 4;
                        const int MAWidgetEventData_currentTab = 8;
                        eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_TAB_CHANGED);
                        eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                        eventData.WriteInt32(MAWidgetEventData_currentTab, mPivot.SelectedIndex);
                        //Posting a CustomEvent
                        mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                    }
                    else throw new InvalidPropertyValueException();
                }
                get
                {
                    return mPivot.SelectedIndex;
                }
            }

            /**
             * Handles the back button pressed event.
             * @return true if the event has been consumed, false otherwise.
             */
            public override bool HandleBackButtonPressed()
            {
                Microsoft.Phone.Controls.Pivot pivot = this.mPivot;
                //If the selected tab is a StackScreen.
                if (this.mChildren[pivot.SelectedIndex] is StackScreen)
                {
                    //If pop is possible.
                    if ((this.mChildren[pivot.SelectedIndex] as StackScreen).StackCount() > 1 && (this.mChildren[pivot.SelectedIndex] as StackScreen).GetBackButtonEnabled() == true)
                    {
                        //Do a pop and cancel the event.
                        (this.mChildren[pivot.SelectedIndex] as StackScreen).PopFromBackButtonPressed();
                        return true;
                    }
                }

                return false;
            }

            /**
             * Check if a given child screen is shown.
             * @param child Given child.
             * @return true if child is currently shown, false otherwise.
             */
            public override bool isChildShown(IScreen child)
            {
                if (mPivot.Items.Count > 0)
                {
                    int index = mPivot.SelectedIndex;
                    if ((mPivot.Items[index] as Microsoft.Phone.Controls.PivotItem).Content.Equals((child as Screen).View))
                    {
                        return true;
                    }
                }
                return false;
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
                bool isPropertyValid = Screen.ValidateProperty(propertyName, propertyValue);

                if (propertyName.Equals("currentTab"))
                {
                    int val;
                    if (!Int32.TryParse(propertyValue, out val)) isPropertyValid = false;
                    else
                    {
                        if (val < 0) isPropertyValid = false;
                    }
                }

                return isPropertyValid;
            }

            #endregion
        } // end of TabScreen class
    } // end of NativeUI namespace
} // end of MoSync namespace