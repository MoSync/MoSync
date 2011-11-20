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
 * @brief This represents the ListViewItem implementation for the NativeUI
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

namespace MoSync
{
    namespace NativeUI
    {
        /**
         * The ListViewItem class defines the attributes and behavior of the
         * items that appear in ListView objects.
         */
        public class ListViewItem : WidgetBaseWindowsPhone
        {
            /**
             * The TextBlock widget that will hold the text of the list view item
             */
            protected System.Windows.Controls.TextBlock mText;

            /**
             * Image wiget for the icon
             */
            protected System.Windows.Controls.Image mIcon;

            /**
             * Stech object that defines the way in which the icon will be streched
             */
            protected System.Windows.Media.Stretch mStretch;

            /**
            * The Grid object holding the icon and the text
            */
            protected System.Windows.Controls.Grid mGrid;

            /**
             * The The Grid will have one row and two colums. First column for the icon, 
             * the second for text
             */
            protected RowDefinition mRow;
            protected ColumnDefinition mColumn1;
            protected ColumnDefinition mColumn2;

            /**
            * Constructor 
            */
            public ListViewItem()
            {             
                mGrid = new System.Windows.Controls.Grid();

                mIcon = new System.Windows.Controls.Image();
                mIcon.VerticalAlignment = VerticalAlignment.Center;
                mStretch = new System.Windows.Media.Stretch();

                mText = new System.Windows.Controls.TextBlock();
                mText.TextWrapping = TextWrapping.NoWrap;
                mText.Margin = new Thickness(10);
                mText.VerticalAlignment = VerticalAlignment.Center;
                mText.TextAlignment = TextAlignment.Left;           
              
                mColumn1 = new ColumnDefinition();
                mColumn1.Width = new GridLength(1, GridUnitType.Auto);

                mColumn2 = new ColumnDefinition();
                mColumn2.Width = new GridLength(1, GridUnitType.Star);

                mRow = new RowDefinition();
                mRow.Height = new GridLength(1, GridUnitType.Auto);

                mGrid.RowDefinitions.Add(mRow);
                mGrid.ColumnDefinitions.Add(mColumn1);
                mGrid.ColumnDefinitions.Add(mColumn2);

                Grid.SetRow(mIcon, 0);
                Grid.SetColumn(mIcon, 0);

                Grid.SetRow(mText, 0);
                Grid.SetColumnSpan(mText, 1);
                Grid.SetColumn(mText, 1);

                mGrid.Children.Add(mIcon);
                mGrid.Children.Add(mText);             
                
                mView = mGrid;
            }

            /**
             * Implementation of the "Text" property. 
             * Sets the text that will appear on the list view item
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LIST_VIEW_ITEM_TEXT)]
            public String Text
            {
                set
                {
                    mText.Text = value;
                }
                get
                {
                    return mText.Text;
                }
            }

            /**
             * Implementation of the "Icon" property. 
             * Sets the Icon that will appear on the list view item, on the left of the text
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LIST_VIEW_ITEM_ICON)]
            public String Icon
            {
                set
                {                  
                    int val = Int32.Parse(value);
                    Resource res = mRuntime.GetResource(MoSync.Constants.RT_IMAGE, val);
                    if (null != res && res.GetInternalObject() != null)
                    {
                      
                        mIcon.Width = mText.Height;
                        mIcon.Height = mText.Height;
                        mIcon.Margin = new Thickness(mText.Margin.Left, mText.Margin.Top, 0, mText.Margin.Bottom);
                        mStretch = System.Windows.Media.Stretch.Fill;
                        mIcon.Stretch = mStretch;                    

                        System.Windows.Media.Imaging.BitmapSource bmpSource =
                        (System.Windows.Media.Imaging.BitmapSource)(res.GetInternalObject());

                        mIcon.Source = bmpSource;                         
                    }
                }
            }

            /**
             * The "AccessoryType" property. 
             * Has no effect. Available only on iOS.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LIST_VIEW_ITEM_ACCESSORY_TYPE)]
            public String AccessoryType
            {
                set
                {
                    //not available for Windows Phone
                }
            }

            /**
             * The implementation of the "FontColor" property. 
             * Sets the font color of the item's text
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LIST_VIEW_ITEM_FONT_COLOR)]
            public String FontColor
            {
                set
                {
                    System.Windows.Media.SolidColorBrush brush;
                    MoSync.Util.convertStringToColor(value, out brush);
                    mText.Foreground = brush;
                }
            }

            /**
             * The implementation of the "FontSize" property. 
             * Sets the font size of the item's text
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_LIST_VIEW_ITEM_FONT_SIZE)]
            public String FontSize
            {
                set
                {
                    double size = Double.Parse(value);
                    mText.FontSize = size;
                }
            }
        }
    }
}