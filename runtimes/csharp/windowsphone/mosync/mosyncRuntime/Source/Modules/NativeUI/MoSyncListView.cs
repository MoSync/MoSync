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
 * @file MoSyncListViewItem.cs
 * @author Rata Gabriela
 *
 * @brief This represents the ListView implementation for the NativeUI
 *        component on Windows Phone 7, language C#
 *
 * @platform WP 7.1
 **/
using Microsoft.Phone.Controls;
using System.Windows.Controls;
using System.Windows;
using System.Windows.Navigation;
using System;
using System.Text.RegularExpressions;
using System.Reflection;
using System.Collections.Generic;


namespace MoSync
{
	namespace NativeUI
	{
        /**
         * The ListView class that displays a list of items, containing text and an image
         */
		public class ListView : WidgetBaseWindowsPhone
		{
            /**
             * A ListBox object that will hold the items
             */
			protected System.Windows.Controls.ListBox mList;

            /**
             * Constructor
             */
			public ListView()
			{
                mList = new System.Windows.Controls.ListBox(); 

                mView = mList;

                mList.Tap += new EventHandler<System.Windows.Input.GestureEventArgs>(
                    delegate(Object from, System.Windows.Input.GestureEventArgs evt)
                    {
                        //create a Memory object of 8 Bytes
                        Memory eventData = new Memory(12);

                        //starting with the 0 Byte we write the eventType
                        const int MAWidgetEventData_eventType = 0;
                        //starting with the 4th Byte we write the widgetHandle
                        
                        const int MAWidgetEventData_widgetHandle = 4;

                        //starting with the 8th Byte we write the selectedIndex                        
                        const int MAWidgetEventData_selectedIndex = 8;

                        int selIndex = mList.SelectedIndex;

                        eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_ITEM_CLICKED);
                        eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);

                        if (selIndex > 0)
                        {
                            eventData.WriteInt32(MAWidgetEventData_selectedIndex, selIndex);
                            //posting a CustomEvent
                            mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                        }                        
                    });
			}

            /**
             * Override of the WidgetBase AddChild function
             */
			public override void AddChild(IWidget child)
			{
                base.AddChild(child);
				MoSync.Util.RunActionOnMainThreadSync(() =>
				{
					System.Windows.Controls.ListBoxItem item = new System.Windows.Controls.ListBoxItem();
					WidgetBaseWindowsPhone widget = (child as WidgetBaseWindowsPhone);
					item.Content = widget.View;
                    mList.Items.Add(item);
				});
			}

            /**
             * Override of the WidgetBase InsertChild function
             */
			public override void InsertChild(IWidget child, int index)
			{
				base.InsertChild(child, index);
				MoSync.Util.RunActionOnMainThreadSync(() =>
				{
					System.Windows.Controls.ListBoxItem item = new System.Windows.Controls.ListBoxItem();
					WidgetBaseWindowsPhone widget = (child as WidgetBaseWindowsPhone);
					item.Content = widget.View;
					mList.Items.Insert(index, item);
				});
			}

            /**
             * Override of the WidgetBase RemoveChild function
             */
			public override void RemoveChild(int index)
			{
				base.RemoveChild(index);
				MoSync.Util.RunActionOnMainThreadSync(() =>
				{
					mList.Items.RemoveAt(index);
				});
			}

            /**
             * Override of the WidgetBase RemoveChild function
             */
			public override void RemoveChild(IWidget child)
			{
				int index = base.mChildren.IndexOf(child);
				if (index >= 0)
				{
					this.RemoveChild(index);
				}
			}
		}
	}
}