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

namespace MoSync
{
    namespace NativeUI
    {
        public class StackScreen : Screen
        {
            protected System.Collections.Generic.Stack<IScreen> mStack;

            //The constructor
            public StackScreen()
            {
                mStack = new System.Collections.Generic.Stack<IScreen>();
            }

            //Override the AddChild function
            public override void AddChild(IWidget child)
            {
                MoSync.Util.RunActionOnMainThreadSync(() =>
                    {
                        mPage.Content = (child as NativeUI.WidgetBaseWindowsPhone).View;
                    });
                //Manualy add the child to the children array
                mChildren.Add(child);
            }

            //The push implementation
            public void Push(IScreen _newScreen)
            {
                if (_newScreen is Screen)
                {
                    mStack.Push(_newScreen);
                    //Manualy add the child to the screen
                    AddChild((_newScreen as Screen));
                }
            }

            //The pop implementation
            public void Pop()
            {
                //If the stack has more than one item pop it and post the MAW_EVENT_STACK_SCREEN_POPPED event
                if (1 < mStack.Count)
                {
                    //STACK_SCREEN_POPPED event needs a memory chunk of 16 bytes
                    Memory eventData = new Memory(16);

                    const int MAWidgetEventData_eventType = 0;
                    const int MAWidgetEventData_widgetHandle = 4;
                    const int MAWidgetEventData_fromHandle = 8;
                    const int MAWidgetEventData_toHandle = 12;

                    eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_STACK_SCREEN_POPPED);
                    eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                    eventData.WriteInt32(MAWidgetEventData_fromHandle, (mStack.Pop() as Screen).GetHandle());
                    eventData.WriteInt32(MAWidgetEventData_toHandle, (mStack.Peek() as Screen).GetHandle());
                    //posting a CustomEvent
                    mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                }

                //If the stack is not empty show the top element of the stack
                if (0 < mStack.Count)
                {
                    (View as Microsoft.Phone.Controls.PhoneApplicationPage).Content = (mStack.Peek() as NativeUI.WidgetBaseWindowsPhone).View;
                }
            }

            //Returns the number of elements from the stack
            public int StackCount()
            {
                return mStack.Count;
            }
        }
    }
}
