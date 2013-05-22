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
 * @author Rata Gabriela and Spiridon Alexandru
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
             * The available list view styles.
             */
            public enum ListViewStyle
            {
                Subtitle,
                NoSubtitle
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
             * The current list view style.
             */
            protected ListViewStyle mListViewStyle;

            /**
             * Contains the long list selector sections.
             */
            ObservableCollection<ListSection<ListItem>> mListSections;

            /**
             * Maps the every model item to the coresponding list view item widget in
             * order to get the selected ListViewItem faster.
             */
            Dictionary<string, ListViewItem> mModelToItemWidgetMap;

            /**
             * Contains the last selected item widget (when a new item is selected, we need
             * to update the last selected item properties).
             */
            ListViewItem mLastSelectedItemWidget = null;

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
                View = mList;

                // add the tap event handler on the default list (mList)
                mList.Tap += new EventHandler<System.Windows.Input.GestureEventArgs>(mList_Tap);

                mModelToItemWidgetMap = new Dictionary<string, ListViewItem>();
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
                if (mListSelectionEnabled)
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

                    for (int i = 0; i < mChildren.Count; i++)
                    {
                        if (mChildren[i] is ListViewItem)
                        {
                            (mChildren[i] as ListViewItem).ItemSelected = false;
                            if (i == selIndex)
                            {
                                (mChildren[i] as ListViewItem).ItemSelected = true;
                            }
                        }
                    }

                    eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_ITEM_CLICKED);
                    eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);

                    if (selIndex > -1)
                    {
                        eventData.WriteInt32(MAWidgetEventData_selectedIndex, selIndex);
                        //posting a CustomEvent
                        mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                    }
                }
                else
                {
                    mList.SelectedItem = null;
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

                        int sectionIndex, itemInSectionIndex;
                        GetSectionAndSelectedItemIndex(out sectionIndex, out itemInSectionIndex);

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
             * MAW_LIST_VIEW_STYLE property implementation
             * There are three available list styles:
             *  - MAW_LIST_VIEW_STYLE_SUBTITLE
             *  - MAW_LIST_VIEW_STYLE_NO_SUBTITLE
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LIST_VIEW_STYLE)]
            public int Style
            {
                get
                {
                    switch (mListViewStyle)
                    {
                        case ListViewStyle.Subtitle:
                            return MoSync.Constants.MAW_LIST_VIEW_STYLE_SUBTITLE;
                        case ListViewStyle.NoSubtitle:
                            return MoSync.Constants.MAW_LIST_VIEW_STYLE_NO_SUBTITLE;
                        default:
                            return MoSync.Constants.MAW_LIST_VIEW_STYLE_SUBTITLE;
                    }
                }
                set
                {
                    switch (value)
                    {
                        case MoSync.Constants.MAW_LIST_VIEW_STYLE_SUBTITLE:
                            mListViewStyle = ListViewStyle.Subtitle;
                            RebuildList();
                            break;
                        case MoSync.Constants.MAW_LIST_VIEW_STYLE_NO_SUBTITLE:
                            mListViewStyle = ListViewStyle.NoSubtitle;
                            RebuildList();
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
#if !LIB
				mLongListSelector.GroupItemTemplate = Application.Current.Resources["groupItemTemplate"] as DataTemplate;
#endif
                mLongListSelector.GroupHeaderTemplate = Application.Current.Resources["groupHeaderTemplate"] as DataTemplate;
                mLongListSelector.GroupFooterTemplate = Application.Current.Resources["groupFooterTemplate"] as DataTemplate;
#if !LIB
				mLongListSelector.GroupItemsPanel = Application.Current.Resources["groupHeaderItemsTemplate"] as ItemsPanelTemplate;
#endif
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
                    if (widget.FILL_SPACE_H)
                    {
                        item.HorizontalContentAlignment = HorizontalAlignment.Stretch;
                    }
                    else
                    {
                        item.HorizontalContentAlignment = HorizontalAlignment.Left;
                    }

                    if (widget.FILL_SPACE_V)
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
                    section.ListStyle = mListViewStyle;
                    ListSection<ListItem> sectionModel = CreateSectionModelFromWidget(section);
                    section.ItemsSourceSectionIndex = mListSections.Count;
                    mListSections.Add(sectionModel);
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
                base.InsertChild(section, index);
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    section.ListStyle = mListViewStyle;
                    ListSection<ListItem> sectionModel = CreateSectionModelFromWidget(section);
                    section.ItemsSourceSectionIndex = index;
                    mListSections.Insert(index, sectionModel);
                    section.SectionIndex = index;

                    // update the section indexes for all the sections after the inserted one
                    for (int i = index + 1; i < mChildren.Count; i++)
                    {
                        if (mChildren[i] is ListViewSection)
                        {
                            (mChildren[i] as ListViewSection).ItemsSourceSectionIndex++;
                        }
                    }
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

                    // update the section indexes for all the sections after the removed one
                    for (int i = index; i < mChildren.Count; i++)
                    {
                        if (mChildren[i] is ListViewSection)
                        {
                            (mChildren[i] as ListViewSection).ItemsSourceSectionIndex--;
                        }
                    }
                });
            }

            /**
             * Creates a section model from the added widget.
             */
            private ListSection<ListItem> CreateSectionModelFromWidget(ListViewSection section)
            {
                ListSection<ListItem> currentSection = new ListSection<ListItem>();
                currentSection.Title = section.Title;
                currentSection.Header = section.Header;
                currentSection.Footer = section.Footer;
                currentSection.HeaderColor = section.GetHeaderBackgroundColor();
                currentSection.HeaderFontColor = section.GetHeaderFontColor();
                currentSection.HeaderFontFamily = section.GetHeaderFontFamily();
                currentSection.HeaderFontSize = section.GetHeaderFontSize();
                currentSection.HeaderFontStyle = section.GetHeaderFontStyle();
                currentSection.HeaderFontWeight = section.GetHeaderFontWeight();
                currentSection.HeaderTextHorizontalAlignment = section.GetHeaderHorizontalAlignment();
                currentSection.HeaderTextVerticalAlignment = section.GetHeaderVerticalAlignment();
                currentSection.Footer = section.Footer;
                currentSection.FooterColor = section.GetFooterBackgroundColor();
                currentSection.FooterFontColor = section.GetFooterFontColor();
                currentSection.FooterFontFamily = section.GetFooterFontFamily();
                currentSection.FooterFontSize = section.GetFooterFontSize();
                currentSection.FooterFontStyle = section.GetFooterFontStyle();
                currentSection.FooterFontWeight = section.GetFooterFontWeight();
                currentSection.FooterTextHorizontalAlignment = section.GetFooterHorizontalAlignment();
                currentSection.FooterTextVerticalAlignment = section.GetFooterVerticalAlignment();
                currentSection.GroupHeaderColor = section.GetGroupHeaderBackgroundColor();

                for (int i = 0; i < section.ChildrenCount; i++ )
                {
                    IWidget widget = section.GetChild(i);
                    if (widget is ListViewItem)
                    {
                        ListViewItem listItem = widget as ListViewItem;
                        ListItem newItem = new ListItem();
                        newItem.Height = listItem.Height;
                        newItem.Width = listItem.Width;
                        newItem.Title = listItem.Text;
                        newItem.Subtitle = listItem.Subtitle;
                        newItem.FontSize = listItem.GetFontSize();
                        newItem.FontColor = listItem.GetFontColor();
                        newItem.SubtitleFontColor = listItem.GetSubtitleFontColor();
                        if (mListViewStyle == ListViewStyle.NoSubtitle)
                        {
                            newItem.SubtitleVisibility = Visibility.Collapsed;
                        }
                        else
                        {
                            newItem.SubtitleVisibility = Visibility.Visible;
                        }
                        newItem.ImageSource = listItem.IconImageSource;

                        listItem.ItemsSourceItemIndex = currentSection.Count;

                        currentSection.Add(newItem);

                        mModelToItemWidgetMap.Add(newItem.UniqueID, listItem);
                    }
                }

                return currentSection;
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
                View = mLongListSelector;
                mLongListSelector.ItemsSource = mListSections;
            }

            /**
             * Switches the current widget view to the list box view.
             */
            private void SwitchToListBox()
            {
                if (View != mList)
                {
                    View = mList;
                }
            }

            /**
             * Outputs the index of the section in focus and the index of the selected item
             * within that section.
             */
            private void GetSectionAndSelectedItemIndex(out int sectionIndex, out int itemIndex)
            {
                // we need to find the selected item through the LongListSelector
                // items source and through the child widgets aswell in order to
                // set its selected state
                sectionIndex = -1;
                itemIndex = -1;

                // we need to first find the selected item index and its section index
                // within the long list selector items source
                int itemsSourceSectionIndex = -1;
                int itemsSourceItemIndex = -1;

                for (int i = 0; i < mListSections.Count; i++)
                {
                    ListSection<ListItem> section = mListSections[i];

                    int index = section.IndexOf(mLongListSelector.SelectedItem as ListItem);
                    if ( index >= 0)
                    {
                        itemsSourceSectionIndex = i;
                        itemsSourceItemIndex = index;
                    }

                    // deselect all the items
                    for (int j = 0; j < section.Count; j++)
                    {
                        section[j].FontColor = mModelToItemWidgetMap[section[j].UniqueID].GetFontColor();
                        section[j].SubtitleFontColor = mModelToItemWidgetMap[section[j].UniqueID].GetSubtitleFontColor();
                    }
                }

                // select the current item
                SolidColorBrush phoneAccentBrush = Application.Current.Resources["PhoneAccentBrush"] as SolidColorBrush;
                (mLongListSelector.SelectedItem as ListItem).FontColor = phoneAccentBrush;
                (mLongListSelector.SelectedItem as ListItem).SubtitleFontColor = phoneAccentBrush;

                // set the coresponding list view item widget selection status
                ListViewItem selectedItem = mModelToItemWidgetMap[(mLongListSelector.SelectedItem as ListItem).UniqueID];
                selectedItem.ItemSelected = true;

                if (mLastSelectedItemWidget != null)
                {
                    mLastSelectedItemWidget.ItemSelected = false;
                }
                mLastSelectedItemWidget = selectedItem;

                // then we need to go through the widget children and find
                // the section/item that match the section/item indexes within
                // the items source
                for (int i = 0; i < mChildren.Count; i++)
                {
                    if (mChildren[i] is ListViewSection)
                    {
                        ListViewSection section = mChildren[i] as ListViewSection;
                        if (section.ItemsSourceSectionIndex == itemsSourceSectionIndex)
                        {
                            for (int j = 0; j < section.ChildrenCount; j++)
                            {
                                if (section.GetChild(j) is ListViewItem)
                                {
                                    ListViewItem item = section.GetChild(j) as ListViewItem;
                                    if (item.ItemsSourceItemIndex == itemsSourceItemIndex)
                                    {
                                        sectionIndex = i;
                                        itemIndex = j;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            /**
             * Rebuilds the list model.
             */
            public void RebuildList()
            {
                mListSections = new ObservableCollection<ListSection<ListItem>>();

                foreach (IWidget widget in mChildren)
                {
                    if (widget is ListViewSection)
                    {
                        ListViewSection section = widget as ListViewSection;
                        ListSection<ListItem> sectionModel = CreateSectionModelFromWidget(section);
                        mListSections.Add(sectionModel);
                    }
                }

                ReloadListData();
            }

            #endregion

            #region Helper method for updating the UI when a section has changed

            /**
             * Reloads a list section within the long list selector items source.
             */
            public void ReloadListSection(ListViewSection section)
            {
                if (mListSections.Count > section.ItemsSourceSectionIndex)
                {
                    ListSection<ListItem> sectionToReload = mListSections[section.ItemsSourceSectionIndex];
                    sectionToReload.Title = section.Title;
                    sectionToReload.Header = section.Header;
                    sectionToReload.HeaderColor = section.GetHeaderBackgroundColor();
                    sectionToReload.HeaderFontColor = section.GetHeaderFontColor();
                    sectionToReload.HeaderFontFamily = section.GetHeaderFontFamily();
                    sectionToReload.HeaderFontSize = section.GetHeaderFontSize();
                    sectionToReload.HeaderFontStyle = section.GetHeaderFontStyle();
                    sectionToReload.HeaderFontWeight = section.GetHeaderFontWeight();
                    sectionToReload.HeaderTextHorizontalAlignment = section.GetHeaderHorizontalAlignment();
                    sectionToReload.HeaderTextVerticalAlignment = section.GetHeaderVerticalAlignment();
                    sectionToReload.Footer = section.Footer;
                    sectionToReload.FooterColor = section.GetFooterBackgroundColor();
                    sectionToReload.FooterFontColor = section.GetFooterFontColor();
                    sectionToReload.FooterFontFamily = section.GetFooterFontFamily();
                    sectionToReload.FooterFontSize = section.GetFooterFontSize();
                    sectionToReload.FooterFontStyle = section.GetFooterFontStyle();
                    sectionToReload.FooterFontWeight = section.GetFooterFontWeight();
                    sectionToReload.FooterTextHorizontalAlignment = section.GetFooterHorizontalAlignment();
                    sectionToReload.FooterTextVerticalAlignment = section.GetFooterVerticalAlignment();
                    sectionToReload.GroupHeaderColor = section.GetGroupHeaderBackgroundColor();

                    mListSections[section.ItemsSourceSectionIndex] = sectionToReload;
                }
            }

            /**
             * Reloads a list item within the long list selector items source.
             */
            public void ReloadListItem(ListViewSection section, ListViewItem item)
            {
                if (mListSections.Count > section.ItemsSourceSectionIndex)
                {
                    ListSection<ListItem> sectionToReload = mListSections[section.ItemsSourceSectionIndex];
                    if (sectionToReload.Count > item.ItemsSourceItemIndex)
                    {
                        ListItem itemToReload = sectionToReload[item.ItemsSourceItemIndex];
                        itemToReload.Height = item.Height;
                        itemToReload.Width = item.Width;
                        itemToReload.Title = item.Text;
                        itemToReload.Subtitle = item.Subtitle;
                        itemToReload.FontSize = item.GetFontSize();
                        itemToReload.FontColor = item.GetFontColor();
                        itemToReload.SubtitleFontColor = item.GetSubtitleFontColor();
                        if (mListViewStyle == ListViewStyle.NoSubtitle)
                        {
                            itemToReload.SubtitleVisibility = Visibility.Collapsed;
                        }
                        else
                        {
                            itemToReload.SubtitleVisibility = Visibility.Visible;
                        }
                        itemToReload.ImageSource = item.IconImageSource;

                        sectionToReload[item.ItemsSourceItemIndex] = itemToReload;
                    }
                }
            }

            /**
             * Reloads all the section items within the long list selector items source.
             */
            public void ReloadListSectionMembers(ListViewSection section)
            {
                if (mListSections.Count > section.ItemsSourceSectionIndex)
                {
                    // TODO SA: this could be more performant
                    mListSections[section.ItemsSourceSectionIndex] = CreateSectionModelFromWidget(section);
                }
            }

            /**
             * Reloads a section at a certain index inside the long list selector items source.
             */
            public void UpdateSection(int sectionIndex, ListSection<ListItem> newSection)
            {
                if (mListSections.Count > sectionIndex)
                {
                    mListSections[sectionIndex] = newSection;
                }
            }

            #endregion

            #region Property validation methods

            /**
             * Validates a property based on the property name and property value.
             * @param propertyName The name of the property to be checked.
             * @param propertyValue The value of the property to be checked.
             * @returns true if the property is valid, false otherwise.
             */
            public new static bool ValidateProperty(string propertyName, string propertyValue)
            {
                bool isPropertyValid = WidgetBaseWindowsPhone.ValidateProperty(propertyName, propertyValue);

                if (propertyName.Equals("type"))
                {
                    int type = -1;
                    if (int.TryParse(propertyValue, out type))
                    {
                        if (!(type == MoSync.Constants.MAW_LIST_VIEW_TYPE_ALPHABETICAL ||
                            type == MoSync.Constants.MAW_LIST_VIEW_TYPE_DEFAULT ||
                            type == MoSync.Constants.MAW_LIST_VIEW_TYPE_SEGMENTED))
                        {
                            isPropertyValid = false;
                        }
                    }
                    else
                    {
                        isPropertyValid = false;
                    }
                }
                else if (propertyName.Equals("style"))
                {
                    int style = -1;
                    if (int.TryParse(propertyValue, out style))
                    {
                        if (!(style == MoSync.Constants.MAW_LIST_VIEW_STYLE_SUBTITLE ||
                            style == MoSync.Constants.MAW_LIST_VIEW_STYLE_NO_SUBTITLE))
                        {
                            isPropertyValid = false;
                        }
                    }
                    else
                    {
                        isPropertyValid = false;
                    }
                }
                else if (propertyName.Equals("allowselection"))
                {
                    bool allowSelection = false;
                    if (!Boolean.TryParse(propertyValue, out allowSelection))
                    {
                        isPropertyValid = false;
                    }
                }

                return isPropertyValid;
            }

            #endregion
        } // end of ListView class
    } // end of NativeUI namespace
} // end of MoSync namespace