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

            // contains the list model items
            List<ListItem> mSectionItems;

            #endregion

            #region Constructor

            /**
			 * Constructor
			 */
            public ListViewSection()
            {
                mSectionItems = new List<ListItem>();
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
                        newItem.Content = (Grid)widget.View;
                        newItem.BackgroundColor = new SolidColorBrush(Colors.Black);
                        newItem.GroupBy = this.Title;
                        newItem.Height = widget.Height;
                        newItem.Width = widget.Width;
                        mSectionItems.Add(newItem);
                    });
                }
            }

            // TODO SA: implement the insert and remove child too

            #endregion

            #region Properties

            /**
             * A getter for the items inside the section - will be used by the ListView
             * to present its content.
             */
            public List<ListItem> SectionItems
            {
                get
                {
                    return mSectionItems;
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
                get;
                set;
            }

            /**
             * MAW_LIST_VIEW_SECTION_HEADER property implementation
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LIST_VIEW_SECTION_HEADER)]
            public string Header
            {
                get;
                set;
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

            #endregion
        } // end of ListView class
    } // end of NativeUI namespace
} // end of MoSync namespace
