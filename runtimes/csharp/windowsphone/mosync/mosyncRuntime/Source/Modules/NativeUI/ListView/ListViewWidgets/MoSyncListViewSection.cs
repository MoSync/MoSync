/* Copyright (C) 2012 MoSync AB

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
 * @file MoSyncListViewSection.cs
 * @author Spiridon Alexandru
 *
 * @brief This represents the ListViewSection implementation for the NativeUI
 *        component on Windows Phone 7, language C#
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
		/**
		 * The ListViewSection class that holds a group of items. This widget can be
         * added as a child to a Alphabetical list or a Segmented list.
		 */
        public class ListViewSection : WidgetBaseWindowsPhone
        {
            #region Private members

            // section title (appears when the header is clicked)
            private string mTitle;

            // header properties
            private string mHeader;
            private SolidColorBrush mHeaderBackgroundColor;
            private TextAlignment mHeaderHorizontalAlignment;
            private VerticalAlignment mHeaderVerticalAlignment;
            private SolidColorBrush mHeaderFontColor;
            private double mHeaderFontSize;
            private FontFamily mHeaderFontFamily;
            private FontWeight mHeaderFontWeight;
            private FontStyle mHeaderFontStyle;

            // footer properties
            private string mFooter;
            private SolidColorBrush mFooterBackgroundColor;
            private TextAlignment mFooterHorizontalAlignment;
            private VerticalAlignment mFooterVerticalAlignment;
            private SolidColorBrush mFooterFontColor;
            private double mFooterFontSize;
            private FontFamily mFooterFontFamily;
            private FontWeight mFooterFontWeight;
            private FontStyle mFooterFontStyle;

            // the group header appears when the user clicks on a section header (inside the
            // long list selector)
            private SolidColorBrush mGroupHeaderBackgroundColor;

            // the current list view style
            ListView.ListViewStyle mListViewStyle = MoSync.Constants.MAW_LIST_VIEW_STYLE_SUBTITLE;

            #endregion

            #region Constructor

            /**
			 * Constructor
			 */
            public ListViewSection()
            {
                this.ItemsSourceSectionIndex = -1;
            }

            #endregion

            #region Public methods

            #region Header getters
            public SolidColorBrush GetHeaderBackgroundColor()
            {
                return mHeaderBackgroundColor;
            }

            public TextAlignment GetHeaderHorizontalAlignment()
            {
                return mHeaderHorizontalAlignment;
            }

            public VerticalAlignment GetHeaderVerticalAlignment()
            {
                return mHeaderVerticalAlignment;
            }

            public SolidColorBrush GetHeaderFontColor()
            {
                return mHeaderFontColor;
            }

            public double GetHeaderFontSize()
            {
                return mHeaderFontSize;
            }

            public FontFamily GetHeaderFontFamily()
            {
                return mHeaderFontFamily;
            }

            public FontWeight GetHeaderFontWeight()
            {
                return mHeaderFontWeight;
            }

            public FontStyle GetHeaderFontStyle()
            {
                return mHeaderFontStyle;
            }

            #endregion

            #region Footer getters

            public SolidColorBrush GetFooterBackgroundColor()
            {
                return mFooterBackgroundColor;
            }

            public TextAlignment GetFooterHorizontalAlignment()
            {
                return mFooterHorizontalAlignment;
            }

            public VerticalAlignment GetFooterVerticalAlignment()
            {
                return mFooterVerticalAlignment;
            }

            public SolidColorBrush GetFooterFontColor()
            {
                return mFooterFontColor;
            }

            public double GetFooterFontSize()
            {
                return mFooterFontSize;
            }

            public FontFamily GetFooterFontFamily()
            {
                return mFooterFontFamily;
            }

            public FontWeight GetFooterFontWeight()
            {
                return mFooterFontWeight;
            }

            public FontStyle GetFooterFontStyle()
            {
                return mFooterFontStyle;
            }

            #endregion

            #region Group Header getters

            public SolidColorBrush GetGroupHeaderBackgroundColor()
            {
                return mGroupHeaderBackgroundColor;
            }

            #endregion

            /**
             * Returns a child.
             */
            public IWidget GetChild(int index)
            {
                if (index < mChildren.Count)
                {
                    return mChildren[index];
                }
                return null;
            }

            /**
             * Gets the parent and asks it to reload the current section synchronized with
             * the main thread.
             */
            public void ReloadParentSectionSync()
            {
                if (mParent is ListView)
                {
                    ListView parentListView = mParent as ListView;
                    MoSync.Util.RunActionOnMainThreadSync(() =>
                    {
                        parentListView.ReloadListSection(this);
                    });
                }
            }

            /**
             * Gets the parent and asks it to reload the current section.
             */
            public void ReloadParentSection()
            {
                if (mParent is ListView && this.ItemsSourceSectionIndex != -1)
                {
                    ListView parentListView = mParent as ListView;
                    parentListView.ReloadListSection(this);
                }
            }

            /**
             * Gets the parent and asks it to reload an item within the current section.
             */
            public void ReloadParentItem(ListViewItem item)
            {
                if (mParent is ListView && this.ItemsSourceSectionIndex != -1)
                {
                    ListView parentListView = mParent as ListView;
                    parentListView.ReloadListItem(this, item);
                }
            }

            /**
             * Gets the parent and asks it to reload all the items of this section synchronized
             * with the main thread.
             */
            public void ReloadParentSectionMembersSync()
            {
                if (mParent is ListView)
                {
                    ListView parentListView = mParent as ListView;
                    MoSync.Util.RunActionOnMainThreadSync(() =>
                    {
                        parentListView.ReloadListSectionMembers(this);
                    });
                }
            }

            #endregion

            #region Overridden methods

            /**
			 * Override of the WidgetBase AddChild function
             * @param child The child to be added.
			 */
            public override void AddChild(IWidget child)
            {
                if (child is ListViewItem)
                {
                    base.AddChild(child);

                    // if the section was added to the list view, the child should
                    // be visible inside the long list selector items source
                    // aswell
                    ReloadParentSectionMembersSync();
                }
            }

            /**
             * Override of the WidgetBase InsertChild function
             */
            public override void InsertChild(IWidget child, int index)
            {
                if (child is ListViewItem)
                {
                    base.InsertChild(child, index);
                    // if the section was added to the list view, the child should
                    // be visible inside the long list selector items source
                    // aswell
                    ReloadParentSectionMembersSync();
                }
            }

            /**
             * Override of the WidgetBase RemoveChild function
             */
            public override void RemoveChild(int index)
            {
                base.RemoveChild(index);
                // if the section was added to the list view, the child should
                // be removed from the long list selector items source
                // aswell
                ReloadParentSectionMembersSync();
            }

            /**
             * Override the WidgetBase RemoveChild function.
             */
            public override void RemoveChild(IWidget child)
            {
                base.RemoveChild(child);
                // if the section was added to the list view, the child should
                // be removed from the long list selector items source
                // aswell
                ReloadParentSectionMembersSync();
            }

            #endregion

            #region Properties

            /**
             * The section index withing the parent items source (withing the long list selector items source).
             */
            public int ItemsSourceSectionIndex
            {
                get;
                set;
            }

            /**
             * A getter/setter for the list view style.
             */
            public ListView.ListViewStyle ListStyle
            {
                get
                {
                    return mListViewStyle;
                }
                set
                {
                    mListViewStyle = value;
                }
            }

            /**
             * The section index within the parent list,
             */
            public int SectionIndex
            {
                get;
                set;
            }

            /**
             * Returns the numeber of children.
             */
            public int ChildrenCount
            {
                get
                {
                    return mChildren.Count;
                }
            }

            #endregion

            #region MoSync Widget Properties

            /**
             * MAW_LIST_VIEW_SECTION_TITLE property implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LIST_VIEW_SECTION_TITLE)]
            public string Title
            {
                get
                {
                    return mTitle;
                }
                set
                {
                    mTitle = value;
                    ReloadParentSection();
                }
            }

            /**
             * MAW_LIST_VIEW_SECTION_HEADER property implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LIST_VIEW_SECTION_HEADER)]
            public string Header
            {
                get
                {
                    return mHeader;
                }
                set
                {
                    mHeader = value;
                    ReloadParentSection();
                }
            }

            /**
             * MAW_LIST_VIEW_SECTION_HEADER_BACKGROUND property implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LIST_VIEW_SECTION_HEADER_BACKGROUND)]
            public string HeaderBackgroundColor
            {
                set
                {
                    System.Windows.Media.SolidColorBrush brush;
                    try
                    {
                        MoSync.Util.convertStringToColor(value, out brush);
                        mHeaderBackgroundColor = brush;
                        mGroupHeaderBackgroundColor = brush;
                        ReloadParentSection();
                    }
                    catch
                    {
                        throw new InvalidPropertyValueException();
                    }
                }
            }

            /**
             * MAW_LIST_VIEW_SECTION_HEADER_HORIZONTAL_ALIGNMENT property implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LIST_VIEW_SECTION_HEADER_HORIZONTAL_ALIGNMENT)]
            public string HeaderTextHorizontalAlignment
            {
                set
                {
                    if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_LEFT))
                    {
                        mHeaderHorizontalAlignment = TextAlignment.Left;
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_RIGHT))
                    {
                        mHeaderHorizontalAlignment = TextAlignment.Right;
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER))
                    {
                        mHeaderHorizontalAlignment = TextAlignment.Center;
                    }
                    else throw new InvalidPropertyValueException();
                    ReloadParentSection();
                }
            }

            /**
             * MAW_LIST_VIEW_SECTION_HEADER_VERTICAL_ALIGNMENT property implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LIST_VIEW_SECTION_HEADER_VERTICAL_ALIGNMENT)]
            public string HeaderTextVerticalAlignment
            {
                set
                {
                    if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_TOP))
                    {
                        mHeaderVerticalAlignment = VerticalAlignment.Top;
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_BOTTOM))
                    {
                        mHeaderVerticalAlignment = VerticalAlignment.Bottom;
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER))
                    {
                        mHeaderVerticalAlignment = VerticalAlignment.Center;
                    }
                    else throw new InvalidPropertyValueException();
                    ReloadParentSection();
                }
            }

            /**
             * MAW_LIST_VIEW_SECTION_HEADER_FONT_COLOR property implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LIST_VIEW_SECTION_HEADER_FONT_COLOR)]
            public string HeaderFontColor
            {
                set
                {
                    try
                    {
                        System.Windows.Media.SolidColorBrush brush;
                        MoSync.Util.convertStringToColor(value, out brush);
                        mHeaderFontColor = brush;
                        ReloadParentSection();
                    }
                    catch
                    {
                        throw new InvalidPropertyValueException();
                    }
                }
            }

            /**
             * MAW_LIST_VIEW_SECTION_HEADER_FONT_SIZE property implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LIST_VIEW_SECTION_HEADER_FONT_SIZE)]
            public double HeaderFontSize
            {
                set
                {
                    mHeaderFontSize = value;
                    ReloadParentSection();
                }
            }

            /**
             * MAW_LIST_VIEW_SECTION_HEADER_FONT_HANDLE property implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LIST_VIEW_SECTION_HEADER_FONT_HANDLE)]
            public int HeaderFontHandle
            {
                set
                {
                    FontModule.FontInfo fontInfo =
                        mRuntime.GetModule<FontModule>().GetFont(value);

                    mHeaderFontFamily = fontInfo.family;
                    mHeaderFontWeight = fontInfo.weight;
                    mHeaderFontStyle = fontInfo.style;
                    ReloadParentSection();
                }
            }

            /**
             * MAW_LIST_VIEW_SECTION_FOOTER property implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LIST_VIEW_SECTION_FOOTER)]
            public string Footer
            {
                get
                {
                    return mFooter;
                }
                set
                {
                    mFooter = value;
                    ReloadParentSection();
                }
            }

            /**
             * MAW_LIST_VIEW_SECTION_FOOTER_BACKGROUND property implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LIST_VIEW_SECTION_FOOTER_BACKGROUND)]
            public string FooterBackgroundColor
            {
                set
                {
                    try
                    {
                        System.Windows.Media.SolidColorBrush brush;
                        MoSync.Util.convertStringToColor(value, out brush);
                        mFooterBackgroundColor = brush;
                        ReloadParentSection();
                    }
                    catch
                    {
                        throw new InvalidPropertyValueException();
                    }
                }
            }

            /**
             * MAW_LIST_VIEW_SECTION_FOOTER_HORIZONTAL_ALIGNMENT property implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LIST_VIEW_SECTION_FOOTER_HORIZONTAL_ALIGNMENT)]
            public string FooterTextHorizontalAlignment
            {
                set
                {
                    if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_LEFT))
                    {
                        mFooterHorizontalAlignment = TextAlignment.Left;
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_RIGHT))
                    {
                        mFooterHorizontalAlignment = TextAlignment.Right;
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER))
                    {
                        mFooterHorizontalAlignment = TextAlignment.Center;
                    }
                    else throw new InvalidPropertyValueException();
                    ReloadParentSection();
                }
            }

            /**
             * MAW_LIST_VIEW_SECTION_FOOTER_VERTICAL_ALIGNMENT property implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LIST_VIEW_SECTION_FOOTER_VERTICAL_ALIGNMENT)]
            public string FooterTextVerticalAlignment
            {
                set
                {
                    if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_TOP))
                    {
                        mFooterVerticalAlignment = VerticalAlignment.Top;
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_BOTTOM))
                    {
                        mFooterVerticalAlignment = VerticalAlignment.Bottom;
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER))
                    {
                        mFooterVerticalAlignment = VerticalAlignment.Center;
                    }
                    else throw new InvalidPropertyValueException();
                    ReloadParentSection();
                }
            }

            /**
             * MAW_LIST_VIEW_SECTION_FOOTER_FONT_COLOR property implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LIST_VIEW_SECTION_FOOTER_FONT_COLOR)]
            public string FooterFontColor
            {
                set
                {
                    try
                    {
                        System.Windows.Media.SolidColorBrush brush;
                        MoSync.Util.convertStringToColor(value, out brush);
                        mFooterFontColor = brush;
                        ReloadParentSection();
                    }
                    catch
                    {
                        throw new InvalidPropertyValueException();
                    }
                }
            }

            /**
             * MAW_LIST_VIEW_SECTION_FOOTER_FONT_SIZE property implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LIST_VIEW_SECTION_FOOTER_FONT_SIZE)]
            public double FooterFontSize
            {
                set
                {
                    mFooterFontSize = value;
                    ReloadParentSection();
                }
            }

            /**
             * MAW_LIST_VIEW_SECTION_FOOTER_FONT_HANDLE property implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LIST_VIEW_SECTION_FOOTER_FONT_HANDLE)]
            public int FooterFontHandle
            {
                set
                {
                    FontModule.FontInfo fontInfo =
                        mRuntime.GetModule<FontModule>().GetFont(value);

                    mFooterFontFamily = fontInfo.family;
                    mFooterFontWeight = fontInfo.weight;
                    mFooterFontStyle = fontInfo.style;
                    ReloadParentSection();
                }
            }

            #endregion
        } // end of ListView class
    } // end of NativeUI namespace
} // end of MoSync namespace
