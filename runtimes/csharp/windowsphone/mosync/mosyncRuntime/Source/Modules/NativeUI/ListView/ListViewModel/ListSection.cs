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
        public class ListSection<T> : ObservableCollection<T>, INotifyPropertyChanged
        {
            public event PropertyChangedEventHandler PropertyChanged;

            const double DEFAULT_HEADER_FONT_SIZE = 32.0;
            const double DEFAULT_FOOTER_FONT_SIZE = 32.0;

            #region Private class members

            private string footerText = "";

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

                this.HeaderTextHorizontalAlignment = HorizontalAlignment.Left;
                this.HeaderTextVerticalAlignment = VerticalAlignment.Center;
                this.FooterTextHorizontalAlignment = HorizontalAlignment.Left;
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
                get;
                set;
            }

            public string Header
            {
                get;
                set;
            }

            public string Footer
            {
                get
                {
                    return footerText;
                }
                set
                {
                    footerText = value;
                    if (footerText != "")
                    {
                        FooterVisibility = Visibility.Visible;
                    }
                }
            }

            public Brush HeaderColor
            {
                get;
                set;
            }

            public Brush FooterColor
            {
                get;
                set;
            }

            public Visibility FooterVisibility
            {
                get;
                set;
            }

            /**
             * The color of the items that appear after the user clicks on the header.
             */
            public Brush GroupHeaderColor
            {
                get;
                set;
            }

            /**
             * The vertical alignment of the header text.
             */
            public VerticalAlignment HeaderTextVerticalAlignment
            {
                get;
                set;
            }

            /**
             * The vertical alignment of the footer text.
             */
            public VerticalAlignment FooterTextVerticalAlignment
            {
                get;
                set;
            }

            /**
             * The horizontal alignment of the header text.
             */
            public HorizontalAlignment HeaderTextHorizontalAlignment
            {
                get;
                set;
            }

            /**
             * The horizontal alignment of the footer text.
             */
            public HorizontalAlignment FooterTextHorizontalAlignment
            {
                get;
                set;
            }

            /**
             * The font size of the header text.
             */
            public double HeaderFontSize
            {
                get;
                set;
            }

            /**
             * The font size of the footer text.
             */
            public double FooterFontSize
            {
                get;
                set;
            }

            /**
             * The font color of the header text.
             */
            public Brush HeaderFontColor
            {
                get;
                set;
            }

            /**
             * The font color of the footer text.
             */
            public Brush FooterFontColor
            {
                get;
                set;
            }

            /**
             * The font family of the header text.
             */
            public FontFamily HeaderFontFamily
            {
                get;
                set;
            }

            /**
             * The font family of the footer text.
             */
            public FontFamily FooterFontFamily
            {
                get;
                set;
            }

            /**
             * The font weight of the header text.
             */
            public FontWeight HeaderFontWeight
            {
                get;
                set;
            }

            /**
             * The font weight of the footer text.
             */
            public FontWeight FooterFontWeight
            {
                get;
                set;
            }

            /**
             * The font style of the header text.
             */
            public FontStyle HeaderFontStyle
            {
                get;
                set;
            }

            /**
             * The font style of the footer text.
             */
            public FontStyle FooterFontStyle
            {
                get;
                set;
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
