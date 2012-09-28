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
using System.Collections.Generic;

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
            private HorizontalAlignment mHeaderHorizontalAlignment;
            private VerticalAlignment mHeaderVerticalAlignment;
            private SolidColorBrush mHeaderFontColor;
            private double mHeaderFontSize;
            private FontFamily mHeaderFontFamily;
            private FontWeight mHeaderFontWeight;
            private FontStyle mHeaderFontStyle;

            // footer properties
            private string mFooter;
            private SolidColorBrush mFooterBackgroundColor;
            private HorizontalAlignment mFooterHorizontalAlignment;
            private VerticalAlignment mFooterVerticalAlignment;
            private SolidColorBrush mFooterFontColor;
            private double mFooterFontSize;
            private FontFamily mFooterFontFamily;
            private FontWeight mFooterFontWeight;
            private FontStyle mFooterFontStyle;

            // the current list view style
            ListView.ListViewStyle mListViewStyle = MoSync.Constants.MAW_LIST_VIEW_STYLE_SUBTITLE;

            #endregion

            #region Constructor

            /**
			 * Constructor
			 */
            public ListViewSection()
            {

            }

            #endregion

            #region Public methods

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
             * Rebuilds the parent list view model.
             */
            public void RebuildParentListSync()
            {
                if (mParent is ListView)
                {
                    ListView parentListView = mParent as ListView;
                    MoSync.Util.RunActionOnMainThreadSync(() =>
                    {
                        parentListView.RebuildList();
                    });
                }
            }

            /**
             * Rebuilds the parent list view model.
             */
            public void RebuildParentList()
            {
                if (mParent is ListView)
                {
                    ListView parentListView = mParent as ListView;
                    parentListView.RebuildList();
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
                    RebuildParentListSync();
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
                    RebuildParentListSync();
                }
            }

            /**
             * Override of the WidgetBase RemoveChild function
             */
            public override void RemoveChild(int index)
            {
                base.RemoveChild(index);
                RebuildParentListSync();
            }

            /**
             * Override the WidgetBase RemoveChild function.
             */
            public override void RemoveChild(IWidget child)
            {
                base.RemoveChild(child);
                RebuildParentListSync();
            }

            #endregion

            #region Properties

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
                    RebuildParentList();
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
                    RebuildParentList();
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
                    MoSync.Util.convertStringToColor(value, out brush);
                    mHeaderBackgroundColor = brush;
                    RebuildParentList();
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
                        mHeaderHorizontalAlignment = HorizontalAlignment.Left;
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_RIGHT))
                    {
                        mHeaderHorizontalAlignment = HorizontalAlignment.Right;
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER))
                    {
                        mHeaderHorizontalAlignment = HorizontalAlignment.Center;
                    }
                    else throw new InvalidPropertyValueException();
                    RebuildParentList();
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
                    RebuildParentList();
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
                    System.Windows.Media.SolidColorBrush brush;
                    MoSync.Util.convertStringToColor(value, out brush);
                    mHeaderFontColor = brush;
                    RebuildParentList();
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
                    RebuildParentList();
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
                    RebuildParentList();
                }
            }

            /**
             * MAW_LIST_VIEW_SECTION_FOOTER property implementation
             * // TODO SA: implement properly
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
                    RebuildParentList();
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
                    System.Windows.Media.SolidColorBrush brush;
                    MoSync.Util.convertStringToColor(value, out brush);
                    mFooterBackgroundColor = brush;
                    RebuildParentList();
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
                        mFooterHorizontalAlignment = HorizontalAlignment.Left;
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_RIGHT))
                    {
                        mFooterHorizontalAlignment = HorizontalAlignment.Right;
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER))
                    {
                        mFooterHorizontalAlignment = HorizontalAlignment.Center;
                    }
                    else throw new InvalidPropertyValueException();
                    RebuildParentList();
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
                    RebuildParentList();
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
                    System.Windows.Media.SolidColorBrush brush;
                    MoSync.Util.convertStringToColor(value, out brush);
                    mFooterFontColor = brush;
                    RebuildParentList();
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
                    RebuildParentList();
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
                    RebuildParentList();
                }
            }

            #endregion
        } // end of ListView class
    } // end of NativeUI namespace
} // end of MoSync namespace
