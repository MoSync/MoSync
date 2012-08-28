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
using System.Windows.Media;

using System.Linq;

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
             * The available list view types.
             */
            protected enum ListViewType
            {
                Default,
                Alphabetical,
                Segmented
            };

			/**
			 * A ListBox object that will hold the items
			 */
            protected System.Windows.Controls.ListBox mList;

            /**
             * The LongListSelector is used when the type of the list is segmented or
             * alphabetical.
             */
            protected LongListSelector mLongListSelector;

            /**
             * The current list view type.
             */
            protected ListViewType mListViewType;

            /* TODO - should contain the long list selector items and sections */

            List<ListSection<ListItem>> mListSections;

            List<ListItem> source;
            IList<ListSection<ListItem>> list;

            #region Constructor

            /**
			 * Constructor
			 */
			public ListView()
			{
				mList = new System.Windows.Controls.ListBox();

                // create the LongListSelector that will be used as a segmented/alphabetical listview
                mLongListSelector = new LongListSelector();
                // apply the predefined templates on the mLongListSelector
                mLongListSelector.ItemTemplate = Application.Current.Resources["listItemTemplate"] as DataTemplate;
                mLongListSelector.GroupItemTemplate = Application.Current.Resources["groupItemTemplate"] as DataTemplate;
                mLongListSelector.GroupHeaderTemplate = Application.Current.Resources["groupHeaderTemplate"] as DataTemplate;
                mLongListSelector.GroupFooterTemplate = Application.Current.Resources["groupFooterTemplate"] as DataTemplate;
                mLongListSelector.GroupItemsPanel = Application.Current.Resources["groupHeaderItemsTemplate"] as ItemsPanelTemplate;

                mListSections = new List<ListSection<ListItem>>();

                // add the selection changed event handler on the mLongListSelector
                mLongListSelector.SelectionChanged += new SelectionChangedEventHandler(mLongListSelector_SelectionChanged);

                // if the default constructor is called, the list view is of type MAW_LIST_VIEW_TYPE_DEFAULT
                mListViewType = ListViewType.Default;
                mView = mList;

                // add the tap event handler on the default list (mList)
                mList.Tap += new EventHandler<System.Windows.Input.GestureEventArgs>(mList_Tap);
            }

            #endregion

            #region Overridden methods

            /**
			 * Override of the WidgetBase AddChild function
			 */
            public override void AddChild(IWidget child)
            {
                if (child is ListViewItem)
                {
                    AddListItem(child as ListViewItem);
                }
                else
                {
                    AddListSection(child as ListViewSection);
                }
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

            #endregion

            #region Events

            void mList_Tap(object sender, System.Windows.Input.GestureEventArgs e)
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

                if (selIndex > -1)
                {
                    eventData.WriteInt32(MAWidgetEventData_selectedIndex, selIndex);
                    //posting a CustomEvent
                    mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                }
            }

            void mLongListSelector_SelectionChanged(object sender, SelectionChangedEventArgs e)
            {
                // TODO SA: send the event to the MoSync runtime

                // if the user presses the header, the selected item will be null and we don't have
                // to do anything
                if (mLongListSelector.SelectedItem != null)
                {
                /*    GridLength len1 = new GridLength(50);
                    ColumnDefinition col1 = new ColumnDefinition { Width = len1 };
                    (mLongListSelector.SelectedItem as ListItem).Content.ColumnDefinitions.Add(col1);
                    TextBlock testTB = new TextBlock();
                    testTB.Text = "CLICK!";
                    Grid.SetColumn(testTB, (mLongListSelector.SelectedItem as ListItem).Content.ColumnDefinitions.Count);
                    (mLongListSelector.SelectedItem as ListItem).Content.Children.Add(testTB); */
                }
            }

            #endregion

            #region Properties

            /**
             * MAW_LIST_VIEW_TYPE property implementation
             * There are three available list types:
             *  - MAW_LIST_VIEW_TYPE_DEFAULT
             *  - MAW_LIST_VIEW_TYPE_ALPHABETICAL
             *  - MAW_LIST_VIEW_TYPE_SEGMENTED
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LIST_VIEW_TYPE)]
            public int Type
            {
                get
                {
                    switch (mListViewType)
                    {
                        case ListViewType.Alphabetical:
                            return MoSync.Constants.MAW_LIST_VIEW_TYPE_ALPHABETICAL;
                        case ListViewType.Default:
                            return MoSync.Constants.MAW_LIST_VIEW_TYPE_DEFAULT;
                        case ListViewType.Segmented:
                            return MoSync.Constants.MAW_LIST_VIEW_TYPE_SEGMENTED;
                        default:
                            return MoSync.Constants.MAW_LIST_VIEW_TYPE_DEFAULT;
                    }
                }
                set
                {
                    switch (value)
                    {
                        case MoSync.Constants.MAW_LIST_VIEW_TYPE_ALPHABETICAL:
                            mListViewType = ListViewType.Alphabetical;
                            ClearLongList();
                            SwitchToAlphabeticalListView();
                            break;
                        case MoSync.Constants.MAW_LIST_VIEW_TYPE_DEFAULT:
                            mListViewType = ListViewType.Default;
                            ClearLongList();
                            SwitchToSegmentedListView();
                            break;
                        case MoSync.Constants.MAW_LIST_VIEW_TYPE_SEGMENTED:
                            mListViewType = ListViewType.Segmented;
                            break;
                        default:
                            throw new InvalidPropertyValueException();
                    }
                }
            }

            /**
             * MAW_LIST_VIEW_MODE property implementation
             * There are two available modes but only one on the windows phone platform:
             *  - MAW_LIST_VIEW_MODE_DISPLAY
             *  - MAW_LIST_VIEW_MODE_EDIT
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LIST_VIEW_MODE)]
            public int Mode
            {
                get
                {
                    return MoSync.Constants.MAW_LIST_VIEW_MODE_DISPLAY;
                }
                set
                {
                    // nothing to do here - the only mode available is the DISPLAY one
                }
            }

            /**
             * MAW_LIST_VIEW_RELOAD_DATA property implementation (when set, it reloads the
             * list data) - setonly
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LIST_VIEW_RELOAD_DATA)]
            public string ReloadData
            {
                set
                {
                    // TODO SA: reload the list data
                }
            }

            /**
             * MAW_LIST_VIEW_ALLOW_SELECTION property implementation
             * @validvalue 'true' or 'false'
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LIST_VIEW_ALLOW_SELECTION)]
            public string AllowSelection
            {
                get
                {
                    // TODO SA: return true if selection is allowed and false otherwise

                    return "true";
                }
                set
                {
                    bool allowSelection = false;
                    if (Boolean.TryParse(value, out allowSelection))
                    {
                        // TODO SA: allow or not the selection inside the list view
                    }
                    else
                    {
                        throw new InvalidPropertyValueException();
                    }
                }
            }

            #endregion

            #region Private methods

            /**
             * Adds a ListViewItem to the main list.
             */
            private void AddListItem(ListViewItem child)
            {
                base.AddChild(child);
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    System.Windows.Controls.ListBoxItem item = new System.Windows.Controls.ListBoxItem();
                    WidgetBaseWindowsPhone widget = (child as WidgetBaseWindowsPhone);
                    item.Content = widget.View;
                    if (widget.fillSpaceHorizontalyEnabled)
                    {
                        item.HorizontalContentAlignment = HorizontalAlignment.Stretch;
                    }
                    else
                    {
                        item.HorizontalContentAlignment = HorizontalAlignment.Left;
                    }

                    if (widget.fillSpaceVerticalyEnabled)
                    {
                        item.VerticalContentAlignment = VerticalAlignment.Stretch;
                    }
                    else
                    {
                        item.VerticalContentAlignment = VerticalAlignment.Center;
                    }
                    mList.Items.Add(item);
                });
            }

            /**
             * Adds a ListViewSection to the LongListSelector.
             */
            private void AddListSection(ListViewSection section)
            {
                ListSection<ListItem> newSection = null;
                MoSync.Util.RunActionOnMainThreadSync(() =>
                    {
                        newSection = new ListSection<ListItem>(section.Title,
                                                               section.SectionItems);
                    });
                mListSections.Add(newSection);
            }

            private void ClearLongList()
            {
                mListSections.Clear();
                this.ReloadData = "true";
            }

            /**
             * Switches the current widget view to a alphabetical list view.
             */
            private void SwitchToAlphabeticalListView()
            {
                mView = mLongListSelector;
                mLongListSelector.ItemsSource = mListSections;
            }

            /**
             * Switches the current widget view to a segmented list view.
             */
            private void SwitchToSegmentedListView()
            {
                mView = mLongListSelector;
                mLongListSelector.ItemsSource = mListSections;
            }

            #endregion
        } // end of ListView class
    } // end of NativeUI namespace
} // end of MoSync namespace