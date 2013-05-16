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
 * @brief This represents the StackScreen implementation for the NativeUI
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
        public class StackScreen : Screen
        {
            Delegate_SwitchContentDelegate mAddChildDelegate = null;

            protected System.Collections.Generic.Stack<IScreen> mStack;
            protected Boolean mBackButtonEnabled = true;

            protected int mPushTransitionType = MoSync.Constants.MAW_TRANSITION_TYPE_NONE;

            protected int mPopTransitionType = MoSync.Constants.MAW_TRANSITION_TYPE_NONE;

            /**
             * The constructor
             */
            public StackScreen() : base()
            {
                mStack = new System.Collections.Generic.Stack<IScreen>();
                mApplicationBarItemsIndexes = new Dictionary<Object, int>();
           }

            /**
             * Override the AddChild function
             */
            public override void AddChild(IWidget child)
            {
                if (child is Screen)
                {
                    MoSync.Util.RunActionOnMainThreadSync(() =>
                        {
                            mAddChildDelegate = delegate()
                            {
                                if (mPage.Children.Count > 0)
                                {
                                    mPage.Children.RemoveAt(mPage.Children.Count - 1);
                                }

                                mPage.Children.Add((child as Screen).View);
                                Grid.SetColumn(mPage.Children[mPage.Children.Count - 1] as Grid, 0);
                                Grid.SetRow(mPage.Children[mPage.Children.Count - 1] as Grid, 0);

                                ToggleApplicationBar((child as Screen));
                            };

                            MoSyncScreenTransitions.doScreenTransition(mAddChildDelegate, mPushTransitionType);
                        });

                    /**
                     * Manualy add the child to the children array
                     */
                    mChildren.Add(child);
                    (child as Screen).SetParent(this);
                }
            }

            /**
             * The push implementation
             */
            public void Push(IScreen _newScreen)
            {
                if (_newScreen is Screen)
                {
                    mStack.Push(_newScreen);
                    /**
                     * Manualy add the child to the screen
                     */
                    AddChild((_newScreen as Screen));
                }
            }

            /**
             * The pop implementation
             */
            public void Pop()
            {
                PostPopEvent();

                /**
                 * If the stack is not empty show the top element of the stack
                 */
                if (0 < mStack.Count)
                {
                    MoSync.Util.RunActionOnMainThreadSync(() =>
                    {
                        mAddChildDelegate = delegate()
                        {
                            if (mPage.Children.Count > 0)
                            {
                                mPage.Children.RemoveAt(mPage.Children.Count - 1);
                            }
                            mPage.Children.Add((mStack.Peek() as Screen).View);
                            Grid.SetColumn(mPage.Children[mPage.Children.Count - 1] as Grid, 0);
                            Grid.SetRow(mPage.Children[mPage.Children.Count - 1] as Grid, 0);

                            ToggleApplicationBar((mStack.Peek() as Screen));
                        };
                        MoSyncScreenTransitions.doScreenTransition(mAddChildDelegate, mPopTransitionType);
                    });
                }
            }

            /**
             * The pop from back call implementation
             */
            public void PopFromBackButtonPressed()
            {
                PostPopEvent();

                /**
                 * If the stack is not empty show the top element of the stack
                 */
                if (0 < mStack.Count)
                {
                    mAddChildDelegate = delegate()
                    {
                        if (mPage.Children.Count > 0)
                        {
                            mPage.Children.RemoveAt(mPage.Children.Count - 1);
                        }
                        mPage.Children.Add((mStack.Peek() as Screen).View);
                        Grid.SetColumn(mPage.Children[mPage.Children.Count - 1] as Grid, 0);
                        Grid.SetRow(mPage.Children[mPage.Children.Count - 1] as Grid, 0);
                        ToggleApplicationBar((mStack.Peek() as Screen));
                    };
                    MoSyncScreenTransitions.doScreenTransition(mAddChildDelegate, mPopTransitionType);
                }
            }

            /**
             * Pops the screen form the stack and posts the custom event
             */
            private void PostPopEvent()
            {
                /**
                 * If the stack has more than one item pop it and post the MAW_EVENT_STACK_SCREEN_POPPED event
                 */
                if (1 < mStack.Count)
                {
                    /**
                     * STACK_SCREEN_POPPED event needs a memory chunk of 16 bytes
                     */
                    Memory eventData = new Memory(16);

                    const int MAWidgetEventData_eventType = 0;
                    const int MAWidgetEventData_widgetHandle = 4;
                    const int MAWidgetEventData_fromHandle = 8;
                    const int MAWidgetEventData_toHandle = 12;

                    eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_STACK_SCREEN_POPPED);
                    eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                    eventData.WriteInt32(MAWidgetEventData_fromHandle, (mStack.Pop() as Screen).GetHandle());
                    eventData.WriteInt32(MAWidgetEventData_toHandle, (mStack.Peek() as Screen).GetHandle());

                    /**
                     * posting a CustomEvent
                     */
                    mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                }
            }

            /**
             * MAW_STACK_SCREEN_BACK_BUTTON_ENABLED property implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_STACK_SCREEN_BACK_BUTTON_ENABLED)]
            public String BackButtonEnabled
            {
                set
                {
                    if (!Boolean.TryParse(value, out mBackButtonEnabled)) throw new InvalidPropertyValueException();
                }
            }

            /**
             * MAW_STACK_SCREEN_PUSH_TRANSITION_TYPE property implementation.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_STACK_SCREEN_PUSH_TRANSITION_TYPE)]
            public String PushTransitionType
            {
                set
                {
                    if (!Int32.TryParse(value, out mPushTransitionType)) throw new InvalidPropertyValueException();
                    if (!NativeUI.MoSyncScreenTransitions.isTransitionAvailable(mPushTransitionType)) throw new InvalidPropertyValueException();
                }
            }

            /**
             * MAW_STACK_SCREEN_POP_TRANSITION_TYPE property implementation.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_STACK_SCREEN_POP_TRANSITION_TYPE)]
            public String PopTransitionType
            {
                set
                {
                    if (!Int32.TryParse(value, out mPopTransitionType)) throw new InvalidPropertyValueException();
                    if (!NativeUI.MoSyncScreenTransitions.isTransitionAvailable(mPopTransitionType)) throw new InvalidPropertyValueException();
                }
            }

            /**
             * Returns the value of the mBackButtonEnabled
             */
            public Boolean GetBackButtonEnabled()
            {
                return mBackButtonEnabled;
            }

            /**
             * Returns the number of elements from the stack
             */
            public int StackCount()
            {
                return mStack.Count;
            }

            /*
             * Toggles the application bar for the screen given as parameter.
             * @param child the screen for which the application bar should be changed / updated.
             */
            public void ToggleApplicationBar(Screen child)
            {
                bool appBarVisible = child.GetApplicationBarVisibility();
                if (appBarVisible)
                {
                    mApplicationBar = child.GetApplicationBar();
                    mApplicationBar.IsVisible = true;
                    ApplicationBarVisible = true;
                    ((Application.Current.RootVisual as Microsoft.Phone.Controls.PhoneApplicationFrame).Content as
                        Microsoft.Phone.Controls.PhoneApplicationPage).ApplicationBar = mApplicationBar;
                }
                else
                {
                    mApplicationBar = child.GetApplicationBar();
                    mApplicationBar.IsVisible = false;
                    ApplicationBarVisible = false;
                    if (((Application.Current.RootVisual as Microsoft.Phone.Controls.PhoneApplicationFrame).Content as
                        Microsoft.Phone.Controls.PhoneApplicationPage).ApplicationBar != null)
                    {
                        ((Application.Current.RootVisual as Microsoft.Phone.Controls.PhoneApplicationFrame).Content as
                        Microsoft.Phone.Controls.PhoneApplicationPage).ApplicationBar.IsVisible = false;
                    }
                }
            }

            /**
             * Handles the back button pressed event.
             * @return true if the event has been consumed, false otherwise.
             */
            public override bool HandleBackButtonPressed()
            {
                if (!(this.GetParent() is TabScreen))
                {
                    if (this.GetParent() is PanoramaView)
                    {
                        if ((this.GetParent() as PanoramaView).getSelectedScreen().Equals(this) && (this as StackScreen).StackCount() > 1)
                        {
                            (this as StackScreen).PopFromBackButtonPressed();
                            return true;
                        }
                    }
                    else if ((this as StackScreen).StackCount() > 1 && (this as StackScreen).GetBackButtonEnabled() == true)
                    {
                        //Do a pop and cancel the event.
                        (this as StackScreen).PopFromBackButtonPressed();
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
                if (mStack.Count > 0 && mStack.Peek().Equals(child))
                {
                    return true;
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

                if (propertyName.Equals("backButtonEnabled"))
                {
                    bool val;
                    if (!Boolean.TryParse(propertyValue, out val)) isPropertyValid = false;
                }
                else if (propertyName.Equals("pushTransitionType"))
                {
                    int val;
                    if (!Int32.TryParse(propertyValue, out val)) isPropertyValid = false;
                    if (!NativeUI.MoSyncScreenTransitions.isTransitionAvailable(val)) isPropertyValid = false;
                }

                return isPropertyValid;
            }

            #endregion
        } // end of StackScreen class
    } // end of NativeUI namespace
} // end of MoSync namespace
