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

            // the corresponding list section
            ListSection<ListItem> mSection;

            #endregion

            #region Constructor

            /**
			 * Constructor
			 */
            public ListViewSection()
            {
                mSection = new ListSection<ListItem>();
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
                    MoSync.Util.RunActionOnMainThreadSync(() =>
                    {
                        ListViewItem widget = (child as ListViewItem);
                        ListItem newItem = new ListItem();
                        newItem.Title = widget.Text;
                        newItem.ImageSource = widget.IconImageSource;
                        // TODO SA: add subtitle property
                        newItem.Subtitle = "subtitle";
                        newItem.SubtitleVisibility = Visibility.Visible;
                        newItem.BackgroundColor = new SolidColorBrush(Colors.Black);
                        newItem.GroupBy = this.Title;
                        newItem.Height = widget.Height;
                        newItem.Width = widget.Width;

                        widget.ListItemData = newItem;
                        mSection.Add(newItem);

                        // we need to update the parent UI the parent exists (it might not if the child was
                        // created before the parent)
                        ListView parentListView = (ListView)this.GetParent();
                        if (parentListView != null)
                        {
                            parentListView.UpdateSection(this.SectionIndex, this.mSection);
                        }
                    });
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
                    MoSync.Util.RunActionOnMainThreadSync(() =>
                    {
                        ListViewItem widget = (child as ListViewItem);
                        ListItem newItem = new ListItem();
                        newItem.Title = widget.Text;
                        newItem.ImageSource = widget.IconImageSource;
                        // TODO SA: add subtitle property
                        newItem.Subtitle = "subtitle";
                        newItem.SubtitleVisibility = Visibility.Visible;
                        newItem.BackgroundColor = new SolidColorBrush(Colors.Black);
                        newItem.GroupBy = this.Title;
                        newItem.Height = widget.Height;
                        newItem.Width = widget.Width;

                        widget.ListItemData = newItem;
                        mSection.Insert(index, newItem);

                        // we need to update the parent UI the parent exists (it might not if the child was
                        // created before the parent)
                        ListView parentListView = (ListView)this.GetParent();
                        if (parentListView != null)
                        {
                            parentListView.UpdateSection(this.SectionIndex, this.mSection);
                        }
                    });
                }
            }

            /**
             * Override of the WidgetBase RemoveChild function
             */
            public override void RemoveChild(int index)
            {
                base.RemoveChild(index);
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    mSection.RemoveAt(index);

                    // we need to update the parent UI the parent exists (it might not if the child was
                    // created before the parent)
                    ListView parentListView = (ListView)this.GetParent();
                    if (parentListView != null)
                    {
                        parentListView.UpdateSection(this.SectionIndex, this.mSection);
                    }
                });
            }

            /**
             * Override the WidgetBase RemoveChild function.
             */
            public override void RemoveChild(IWidget child)
            {
                List<IWidget> children = this.mChildren;
                base.RemoveChild(child);
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    mSection.Remove(((ListViewItem)child).ListItemData);

                    // we need to update the parent UI the parent exists (it might not if the child was
                    // created before the parent)
                    ListView parentListView = (ListView)this.GetParent();
                    if (parentListView != null)
                    {
                        parentListView.UpdateSection(this.SectionIndex, this.mSection);
                    }
                });
            }

            #endregion

            #region Properties

            /**
             * A getter for the items inside the section - will be used by the ListView
             * to present its content.
             */
            public ListSection<ListItem> SectionData
            {
                get
                {
                    return mSection;
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
                    return mSection.Title;
                }
                set
                {
                    mSection.Title = value;
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
                    return mSection.Header;
                }
                set
                {
                    mSection.Header = value;
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
                    mSection.HeaderColor = brush;
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
                        mSection.HeaderTextHorizontalAlignment = HorizontalAlignment.Left;
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_RIGHT))
                    {
                        mSection.HeaderTextHorizontalAlignment = HorizontalAlignment.Right;
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER))
                    {
                        mSection.HeaderTextHorizontalAlignment = HorizontalAlignment.Center;
                    }
                    else throw new InvalidPropertyValueException();
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
                        mSection.HeaderTextVerticalAlignment = VerticalAlignment.Top;
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_BOTTOM))
                    {
                        mSection.HeaderTextVerticalAlignment = VerticalAlignment.Bottom;
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER))
                    {
                        mSection.HeaderTextVerticalAlignment = VerticalAlignment.Center;
                    }
                    else throw new InvalidPropertyValueException();
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
                    mSection.HeaderFontColor = brush;
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
                    mSection.HeaderFontSize = value;
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

                    mSection.HeaderFontFamily = fontInfo.family;
                    mSection.HeaderFontWeight = fontInfo.weight;
                    mSection.HeaderFontStyle = fontInfo.style;
                }
            }

            /**
             * MAW_LIST_VIEW_SECTION_FOOTER property implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LIST_VIEW_SECTION_FOOTER)]
            public string Footer
            {
                get;
                set;
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
                    mSection.FooterColor = brush;
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
                        mSection.FooterTextHorizontalAlignment = HorizontalAlignment.Left;
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_RIGHT))
                    {
                        mSection.FooterTextHorizontalAlignment = HorizontalAlignment.Right;
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER))
                    {
                        mSection.FooterTextHorizontalAlignment = HorizontalAlignment.Center;
                    }
                    else throw new InvalidPropertyValueException();
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
                        mSection.FooterTextVerticalAlignment = VerticalAlignment.Top;
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_BOTTOM))
                    {
                        mSection.FooterTextVerticalAlignment = VerticalAlignment.Bottom;
                    }
                    else if (value.Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER))
                    {
                        mSection.FooterTextVerticalAlignment = VerticalAlignment.Center;
                    }
                    else throw new InvalidPropertyValueException();
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
                    mSection.FooterFontColor = brush;
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
                    mSection.FooterFontSize = value;
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

                    mSection.FooterFontFamily = fontInfo.family;
                    mSection.FooterFontWeight = fontInfo.weight;
                    mSection.FooterFontStyle = fontInfo.style;
                }
            }

            #endregion
        } // end of ListView class
    } // end of NativeUI namespace
} // end of MoSync namespace
