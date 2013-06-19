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
 * @file ListItem.cs
 * @author Spiridon Alexandru
 *
 * @brief This represents the model of a list item. A list of items will be contained by a 'ListSection' and a
 * list of 'ListSection' will be set as the the 'ItemsSource' of the 'LongListSelector' (used as a
 * implementation for the alphabetical/segmented 'ListView' for the MoSync NativeUI component on
 * Windows Phone 7, language C#.
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
using System.ComponentModel;
using System.Security.Cryptography;
using System.Text;

namespace MoSync
{
    namespace NativeUI
    {
        public class ListItem : INotifyPropertyChanged
        {
            public event PropertyChangedEventHandler PropertyChanged;

            const double DEFAULT_ITEM_FONT_SIZE = 19.0;

            #region Private members

            private Brush mBackgroundColor;
            private Brush mFontColor;
            private Brush mSubtitleFontColor;
            private double mFontSize;
            private String mTitle;
            private String mSubtitle;
            private Visibility mSubtitleVisibility;
            private ImageSource mImageSource;

            // if the values are not set, the size of the Item is the same
            // as the size of its content (mContent)
            private double mHeight;
            private double mWidth;

            // the current list view style
            private ListView.ListViewStyle mListStyle = MoSync.Constants.MAW_LIST_VIEW_STYLE_SUBTITLE;

            // a unique string to be used as a hash key for maping the model item with the coresponding
            // mosync ListViewItem
            private string mUniqueID;

            #endregion

            #region Constructors

            public ListItem()
            {
                mBackgroundColor = new SolidColorBrush(Colors.Black);
                mTitle = "";
                mSubtitle = "";
                // the equivalent of xaml 'Auto' - the item resizez according to its content
                mHeight = double.NaN;
                mWidth = double.NaN;
                this.FontSize = DEFAULT_ITEM_FONT_SIZE;
                this.FontColor = new SolidColorBrush(Colors.White);
                this.SubtitleFontColor = new SolidColorBrush(Colors.White);

                mUniqueID = GetUniqueKey();
            }

            #endregion

            #region Properties

            public string UniqueID
            {
                get
                {
                    return mUniqueID;
                }
            }

            public double Height
            {
                get
                {
                    return mHeight;
                }
                set
                {
                    mHeight = value;
                    OnPropertyChanged("Height");
                }
            }

            public double Width
            {
                get
                {
                    return mWidth;
                }
                set
                {
                    mWidth = value;
                    OnPropertyChanged("Width");
                }
            }

            public String Title
            {
                get
                {
                    return mTitle;
                }
                set
                {
                    mTitle = value;
                    OnPropertyChanged("Title");
                }
            }

            public String Subtitle
            {
                get
                {
                    return mSubtitle;
                }
                set
                {
                    mSubtitle = value;
                    OnPropertyChanged("Subtitle");
                }
            }

            /**
             * A getter/setter for the list view style.
             */
            public ListView.ListViewStyle ListStyle
            {
                get
                {
                    return mListStyle;
                }
                set
                {
                    mListStyle = value;
                    if (mListStyle == ListView.ListViewStyle.Subtitle)
                    {
                        this.SubtitleVisibility = Visibility.Visible;
                    }
                    else
                    {
                        this.SubtitleVisibility = Visibility.Collapsed;
                    }
                }
            }

            public Visibility SubtitleVisibility
            {
                get
                {
                    return mSubtitleVisibility;
                }
                set
                {
                    mSubtitleVisibility = value;
                    OnPropertyChanged("SubtitleVisibility");
                }
            }

            public ImageSource ImageSource
            {
                get
                {
                    return mImageSource;
                }
                set
                {
                    if (mImageSource != null)
                    {
                        mImageSource = value;
                        OnPropertyChanged("ImageSource");
                    }
                }
            }

            public string GroupBy
            {
                get;
                set;
            }

            public Brush BackgroundColor
            {
                get
                {
                    return mBackgroundColor;
                }
                set
                {
                    if (mBackgroundColor != null)
                    {
                        mBackgroundColor = value;
                        OnPropertyChanged("BackgroundColor");
                    }
                }
            }

            public Brush FontColor
            {
                get
                {
                    return mFontColor;
                }
                set
                {
                    if (value != null)
                    {
                        mFontColor = value;
                        OnPropertyChanged("FontColor");
                    }
                }
            }

            public Brush SubtitleFontColor
            {
                get
                {
                    return mSubtitleFontColor;
                }
                set
                {
                    if (value != null)
                    {
                        mSubtitleFontColor = value;
                        OnPropertyChanged("SubtitleFontColor");
                    }
                }
            }

            public double FontSize
            {
                get
                {
                    return mFontSize;
                }
                set
                {
                    if (value > 0)
                    {
                        mFontSize = value;
                        OnPropertyChanged("FontSize");
                    }
                }
            }

            #endregion

            #region Private methods

            private void OnPropertyChanged(string property)
            {
                if (PropertyChanged != null)
                {
                    PropertyChanged(this, new PropertyChangedEventArgs(property));
                }
            }

            private string GetUniqueKey()
            {
                return Guid.NewGuid().ToString();
            }

            #endregion
        }
    } // end of NativeUI namespace
} // end of MoSync namespace