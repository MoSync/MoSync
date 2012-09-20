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
using System.Collections.ObjectModel;

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
             * By default, the selection is enabled on the list. This value keeps track of
             * the selection status: if set to false, inside the selection changed event,
             * we deselect the selected item.
             */
            protected bool mListSelectionEnabled = true;

            /**
             * The LongListSelector is used when the type of the list is segmented or
             * alphabetical.
             */
            protected LongListSelector mLongListSelector;

            /**
             * By default, the selection is enabled on the long list selector. This value keeps track of
             * the selection status: if set to false, inside the selection changed event,
             * we deselect the selected item.
             */
            protected bool mLongListSelectorSelectionEnabled = true;

            /**
             * The current list view type.
             */
            protected ListViewType mListViewType;

            /**
             * Contains the long list selector sections.
             */
            ObservableCollection<ListSection<ListItem>> mListSections;

            #region Constructor

            /**
			 * Constructor
			 */
			public ListView()
			{
				mList = new System.Windows.Controls.ListBox();

                // create the LongListSelector that will be used as a segmented/alphabetical listview
                mLongListSelector = new LongListSelector();
                mLongListSelector.Height = 500;
                mLongListSelector.Width = 400;
                // apply the predefined templates on the mLongListSelector
                ApplyTemplatesOnLongListSelector();

                mListSections = new ObservableCollection<ListSection<ListItem>>();

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
                if (child is ListViewSection)
                {
                    AddListSection(child as ListViewSection);
                }
                else
                {
                    AddListItem(child);
                }
            }

            /**
             * Override of the WidgetBase InsertChild function
             */
            public override void InsertChild(IWidget child, int index)
            {
                if (child is ListViewSection)
                {
                    InsertListSection(child as ListViewSection, index);
                }
                else
                {
                    InsertListItem(child, index);
                }
            }

            /**
             * Override of the WidgetBase RemoveChild function
             */
            public override void RemoveChild(int index)
            {
                if (mListViewType == ListViewType.Default)
                {
                    RemoveListItem(index);
                }
                else
                {
                    RemoveListSection(index);
                }
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

            /**
             * Event triggered when the user clicks on a ListBox item.
             */
            void mList_Tap(object sender, System.Windows.Input.GestureEventArgs e)
            {
                // TODO SA: do nothing if selection is disabled

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

            /**
             * Event triggered when the user clicks on a ListBox section.
             */
            void mLongListSelector_SelectionChanged(object sender, SelectionChangedEventArgs e)
            {
                if (mLongListSelectorSelectionEnabled)
                {
                    // if the user presses the header, the selected item will be null and we don't have
                    // to do anything
                    if (mLongListSelector.SelectedItem != null)
                    {
                        //create a Memory object of 8 Bytes
                        Memory eventData = new Memory(16);

                        //starting with the 0 Byte we write the eventType
                        const int MAWidgetEventData_eventType = 0;

                        //starting with the 4th Byte we write the widgetHandle
                        const int MAWidgetEventData_widgetHandle = 4;

                        //starting with the 8th Byte we write the selectedSectionIndex
                        const int MAWidgetEventData_selectedSectionIndex = 8;

                        //starting with the 8th Byte we write the selectedItemIndex
                        const int MAWidgetEventData_selectedItemIndex = 12;

                        int sectionIndex = -1;
                        int itemInSectionIndex = -1;
                        bool foundItem = false;
                        for (int i = 0; i < mListSections.Count; i++)
                        {
                            ListSection<ListItem> section = mListSections[i];
                            for (int j = 0; j < section.Count; j++ )
                            {
                                ListItem item = section[j];
                                if (item.Equals(mLongListSelector.SelectedItem))
                                {
                                    sectionIndex = i;
                                    itemInSectionIndex = j;
                                    foundItem = true;
                                    break;
                                }
                            }
                            if (foundItem)
                            {
                                break;
                            }
                        }

                        eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_SEGMENTED_LIST_ITEM_CLICKED);
                        eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);

                        if (sectionIndex > -1)
                        {
                            eventData.WriteInt32(MAWidgetEventData_selectedSectionIndex, sectionIndex);
                            eventData.WriteInt32(MAWidgetEventData_selectedItemIndex, itemInSectionIndex);
                            //posting a CustomEvent
                            mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                        }
                    }
                }
                else
                {
                    // this is the only way I found to disable selection on the LongListSelector
                    mLongListSelector.SelectedItem = null;
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
                            SwitchToLongListSelector();
                            break;
                        case MoSync.Constants.MAW_LIST_VIEW_TYPE_DEFAULT:
                            mListViewType = ListViewType.Default;
                            ClearLongList();
                            SwitchToListBox();
                            break;
                        case MoSync.Constants.MAW_LIST_VIEW_TYPE_SEGMENTED:
                            mListViewType = ListViewType.Segmented;
                            ClearLongList();
                            SwitchToLongListSelector();
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
                    ReloadListData();
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
                    if (mListViewType == ListViewType.Default)
                    {
                        if (mListSelectionEnabled)
                        {
                            return "true";
                        }
                        return "false";
                    }
                    else
                    {
                        if (mLongListSelectorSelectionEnabled)
                        {
                            return "true";
                        }
                        return "false";
                    }
                }
                set
                {
                    bool allowSelection = false;
                    if (Boolean.TryParse(value, out allowSelection))
                    {
                        if (mListViewType == ListViewType.Default)
                        {
                            mListSelectionEnabled = allowSelection;
                        }
                        else
                        {
                            mLongListSelectorSelectionEnabled = allowSelection;
                            // if the allow selection is set to false, we deselect the
                            // currently selected items
                            if (!mLongListSelectorSelectionEnabled)
                            {
                                mLongListSelector.SelectedItem = null;
                            }
                        }
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
             * Applies templates on the long list selector.
             */
            private void ApplyTemplatesOnLongListSelector()
            {
                mLongListSelector.ItemTemplate = Application.Current.Resources["listItemTemplate"] as DataTemplate;
                mLongListSelector.GroupItemTemplate = Application.Current.Resources["groupItemTemplate"] as DataTemplate;
                mLongListSelector.GroupHeaderTemplate = Application.Current.Resources["groupHeaderTemplate"] as DataTemplate;
                mLongListSelector.GroupFooterTemplate = Application.Current.Resources["groupFooterTemplate"] as DataTemplate;
                mLongListSelector.GroupItemsPanel = Application.Current.Resources["groupHeaderItemsTemplate"] as ItemsPanelTemplate;
            }

            /**
             * Adds a ListViewItem to the main list.
             * @param child The list view item to be added.
             */
            private void AddListItem(IWidget child)
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
             * @param section The list view section to be added.
             */
            private void AddListSection(ListViewSection section)
            {
                base.AddChild(section);
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    mListSections.Add(section.SectionData);
                    section.SectionIndex = mListSections.Count - 1;
                });
            }

            /**
             * Inserts a list view item at a certain index.
             * @param child The widget to be added to the list.
             * @param index The index where the widget should be added.
             */
            private void InsertListItem(IWidget child, int index)
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
             * Inserts a list view section at a certain index.
             * @param child The widget to be added to the list.
             * @param index The index where the widget should be added.
             */
            private void InsertListSection(ListViewSection section, int index)
            {
                base.AddChild(section);
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    mListSections.Insert(index, section.SectionData);
                });
            }

            /**
             * Removes a list view item from the list.
             * @param index The index of the list view item to be removed.
             */
            private void RemoveListItem(int index)
            {
                base.RemoveChild(index);
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    mList.Items.RemoveAt(index);
                });
            }

            /**
             * Removes a list view section from the list.
             * @param index The index of the list view section to be removed.
             */
            private void RemoveListSection(int index)
            {
                base.RemoveChild(index);
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    mListSections.RemoveAt(index);
                });
            }

            /**
             * Removes the items from the list and reloads it.
             */
            private void ClearLongList()
            {
                mListSections.Clear();
            }

            /**
             * Reloads the data inside the list.
             */
            private void ReloadListData()
            {
                mLongListSelector.ItemsSource = null;
                mLongListSelector.ItemsSource = mListSections;
            }

            /**
             * Switches the current widget view to the long list selector view.
             */
            private void SwitchToLongListSelector()
            {
                mView = mLongListSelector;
                mLongListSelector.ItemsSource = mListSections;
            }

            /**
             * Switches the current widget view to the list box view.
             */
            private void SwitchToListBox()
            {
                if (mView != mList)
                {
                    mView = mList;
                }
            }

            #endregion

            #region Helper method for updating the UI when a section has changed

            public void UpdateSection(int sectionIndex, ListSection<ListItem> newSection)
            {
                if (mListSections.Count > sectionIndex)
                {
                    mListSections[sectionIndex] = newSection;
                }
            }

            #endregion
        } // end of ListView class
    } // end of NativeUI namespace
} // end of MoSync namespace