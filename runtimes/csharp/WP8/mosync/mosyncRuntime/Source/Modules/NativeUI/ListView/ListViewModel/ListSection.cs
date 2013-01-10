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
 * @file ListSection.cs
 * @author Spiridon Alexandru
 *
 * @brief This represents the model of a section. A list of sections will be set as
 * the 'ItemsSource' of the 'LongListSelector' (used as a implementation for the alphabetical/segmented
 * 'ListView' for the MoSync NativeUI component on Windows Phone 7, language C#.
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
using System.Collections.ObjectModel;
using System.Collections.Generic;
using System.ComponentModel;

namespace MoSync
{
    namespace NativeUI
    {
        public class ListSection<T> : ObservableCollection<T>
        {
            public new event PropertyChangedEventHandler PropertyChanged;

            const double DEFAULT_HEADER_FONT_SIZE = 32.0;
            const double DEFAULT_FOOTER_FONT_SIZE = 32.0;

            #region Private class members

            private string mFooter = "";
            private string mTitle = "";
            private string mHeader = "";
            private Brush mHeaderColor;
            private Brush mFooterColor;
            private Visibility mFooterVisibility;
            private Brush mGroupHeaderColor;
            private VerticalAlignment mHeaderTextVerticalAlignment;
            private VerticalAlignment mFooterTextVerticalAlignment;
            private TextAlignment mHeaderTextHorizontalAlignment;
            private TextAlignment mFooterTextHorizontalAlignment;
            private double mHeaderFontSize;
            private double mFooterFontSize;
            private Brush mHeaderFontColor;
            private Brush mFooterFontColor;
            private FontFamily mHeaderFontFamily;
            private FontFamily mFooterFontFamily;
            private FontWeight mHeaderFontWeight;
            private FontWeight mFooterFontWeight;
            private FontStyle mHeaderFontStyle;
            private FontStyle mFooterFontStyle;

            #endregion

            #region Constructors

            public ListSection()
            {
                SetDefaultPropertyValues();
            }

            public ListSection(string headerTitle, IEnumerable<T> items)
            {
                this.Title = headerTitle;
                this.Footer = "";
                foreach (T item in items)
                {
                    this.Add(item);
                }
                SetDefaultPropertyValues();
            }

            public ListSection(string headerTitle, IEnumerable<T> items, Brush headerColor)
            {
                this.Title = headerTitle;
                this.Footer = "";
                foreach (T item in items)
                {
                    this.Add(item);
                }
                SetDefaultPropertyValues();
                this.HeaderColor = headerColor;
            }

            public ListSection(string headerTitle, string footerTitle, IEnumerable<T> items, Brush headerColor)
            {
                this.Title = headerTitle;
                this.Footer = footerTitle;
                foreach (T item in items)
                {
                    this.Add(item);
                }
                SetDefaultPropertyValues();
                this.HeaderColor = headerColor;
            }

            private void SetDefaultPropertyValues()
            {
                // we set the default colors according to the theme of the phone
                SolidColorBrush phoneAccentBrush = Application.Current.Resources["PhoneAccentBrush"] as SolidColorBrush;
                this.HeaderColor = phoneAccentBrush;
                this.FooterColor = phoneAccentBrush;
                this.FooterVisibility = Visibility.Collapsed;
                this.GroupHeaderColor = phoneAccentBrush;

                this.HeaderTextHorizontalAlignment = TextAlignment.Left;
                this.HeaderTextVerticalAlignment = VerticalAlignment.Center;
                this.FooterTextHorizontalAlignment = TextAlignment.Left;
                this.FooterTextVerticalAlignment = VerticalAlignment.Center;

                this.HeaderFontFamily = new FontFamily("PhoneFontFamilyNormal");
                this.FooterFontFamily = new FontFamily("PhoneFontFamilyNormal");

                this.HeaderFontSize = DEFAULT_HEADER_FONT_SIZE;
                this.FooterFontSize = DEFAULT_FOOTER_FONT_SIZE;

                this.HeaderFontColor = new SolidColorBrush(Colors.Black);
                this.FooterFontColor = new SolidColorBrush(Colors.Black);
            }

            #endregion

            public override bool Equals(object obj)
            {
                ListSection<T> that = obj as ListSection<T>;
                return (that != null) && (this.Title.Equals(that.Title));
            }

            #region Properties

            public string Title
            {
                get
                {
                    return mTitle;
                }
                set
                {
                    if (value != null)
                    {
                        mTitle = value;
                    }
                }
            }

            public string Header
            {
                get
                {
                    return mHeader;
                }
                set
                {
                    if (value != null)
                    {
                        mHeader = value;
                    }
                }
            }

            public string Footer
            {
                get
                {
                    return mFooter;
                }
                set
                {
                    mFooter = value;
                    if (mFooter != null)
                    {
                        FooterVisibility = Visibility.Visible;
                    }
                    else
                    {
                        FooterVisibility = Visibility.Collapsed;
                    }
                }
            }

            public Brush HeaderColor
            {
                get
                {
                    return mHeaderColor;
                }
                set
                {
                    if (value != null)
                    {
                        mHeaderColor = value;
                    }
                }
            }

            public Brush FooterColor
            {
                get
                {
                    return mFooterColor;
                }
                set
                {
                    if (value != null)
                    {
                        mFooterColor = value;
                    }
                }
            }

            public Visibility FooterVisibility
            {
                get
                {
                    return mFooterVisibility;
                }
                set
                {
                    mFooterVisibility = value;
                }
            }

            /**
             * The color of the items that appear after the user clicks on the header.
             */
            public Brush GroupHeaderColor
            {
                get
                {
                    return mGroupHeaderColor;
                }
                set
                {
                    if (value != null)
                    {
                        mGroupHeaderColor = value;
                    }
                }
            }

            /**
             * The vertical alignment of the header text.
             */
            public VerticalAlignment HeaderTextVerticalAlignment
            {
                get
                {
                    return mHeaderTextVerticalAlignment;
                }
                set
                {
                    mHeaderTextVerticalAlignment = value;
                }
            }

            /**
             * The vertical alignment of the footer text.
             */
            public VerticalAlignment FooterTextVerticalAlignment
            {
                get
                {
                    return mFooterTextVerticalAlignment;
                }
                set
                {
                    mFooterTextVerticalAlignment = value;
                }
            }

            /**
             * The horizontal alignment of the header text.
             */
            public TextAlignment HeaderTextHorizontalAlignment
            {
                get
                {
                    return mHeaderTextHorizontalAlignment;
                }
                set
                {
                    mHeaderTextHorizontalAlignment = value;
                }
            }

            /**
             * The horizontal alignment of the footer text.
             */
            public TextAlignment FooterTextHorizontalAlignment
            {
                get
                {
                    return mFooterTextHorizontalAlignment;
                }
                set
                {
                    mFooterTextHorizontalAlignment = value;
                }
            }

            /**
             * The font size of the header text.
             */
            public double HeaderFontSize
            {
                get
                {
                    return mHeaderFontSize;
                }
                set
                {
                    if (value > 0)
                    {
                        mHeaderFontSize = value;
                    }
                }
            }

            /**
             * The font size of the footer text.
             */
            public double FooterFontSize
            {
                get
                {
                    return mFooterFontSize;
                }
                set
                {
                    if (value > 0)
                    {
                        mFooterFontSize = value;
                    }
                }
            }

            /**
             * The font color of the header text.
             */
            public Brush HeaderFontColor
            {
                get
                {
                    return mHeaderFontColor;
                }
                set
                {
                    if (value != null)
                    {
                        mHeaderFontColor = value;
                    }
                }
            }

            /**
             * The font color of the footer text.
             */
            public Brush FooterFontColor
            {
                get
                {
                    return mFooterFontColor;
                }
                set
                {
                    if (value != null)
                    {
                        mFooterFontColor = value;
                    }
                }
            }

            /**
             * The font family of the header text.
             */
            public FontFamily HeaderFontFamily
            {
                get
                {
                    return mHeaderFontFamily;
                }
                set
                {
                    if (value != null)
                    {
                        mHeaderFontFamily = value;
                    }
                }
            }

            /**
             * The font family of the footer text.
             */
            public FontFamily FooterFontFamily
            {
                get
                {
                    return mFooterFontFamily;
                }
                set
                {
                    if (value != null)
                    {
                        mFooterFontFamily = value;
                    }
                }
            }

            /**
             * The font weight of the header text.
             */
            public FontWeight HeaderFontWeight
            {
                get
                {
                    return mHeaderFontWeight;
                }
                set
                {
                    if (value != null)
                    {
                        mHeaderFontWeight = value;
                    }
                }
            }

            /**
             * The font weight of the footer text.
             */
            public FontWeight FooterFontWeight
            {
                get
                {
                    return mFooterFontWeight;
                }
                set
                {
                    if (value != null)
                    {
                        mFooterFontWeight = value;
                    }
                }
            }

            /**
             * The font style of the header text.
             */
            public FontStyle HeaderFontStyle
            {
                get
                {
                    return mHeaderFontStyle;
                }
                set
                {
                    if (value != null)
                    {
                        mHeaderFontStyle = value;
                    }
                }
            }

            /**
             * The font style of the footer text.
             */
            public FontStyle FooterFontStyle
            {
                get
                {
                    return mFooterFontStyle;
                }
                set
                {
                    if (value != null)
                    {
                        mFooterFontStyle = value;
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

            #endregion
        }

        // TODO SA: investigate if this is necessary
        public static class EnumerableExtensions
        {
            public static ObservableCollection<T> ToObservableCollection<T>(this IEnumerable<T> collection)
            {
                ObservableCollection<T> observableCollection = new ObservableCollection<T>();
                foreach (T item in collection)
                {
                    observableCollection.Add(item);
                }

                return observableCollection;
            }
        }
    } // end of NativeUI namespace
} // end of MoSync namespace
