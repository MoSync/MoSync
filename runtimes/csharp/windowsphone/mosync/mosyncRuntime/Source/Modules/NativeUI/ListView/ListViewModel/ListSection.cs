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

namespace MoSync
{
    namespace NativeUI
    {
        public class ListSection<T> : ObservableCollection<T>
        {
            #region Constructors

            public ListSection(string headerTitle, IEnumerable<T> items)
            {
                this.Title = headerTitle;
                this.FooterTitle = "";
                foreach (T item in items)
                {
                    this.Add(item);
                }
                // we set the default colors according to the theme of the phone
                SolidColorBrush phoneAccentBrush = Application.Current.Resources["PhoneAccentBrush"] as SolidColorBrush;
                this.HeaderColor = phoneAccentBrush;
                this.FooterColor = phoneAccentBrush;
                this.FooterVisibility = Visibility.Collapsed;
                this.GroupHeaderColor = phoneAccentBrush;
            }

            public ListSection(string headerTitle, IEnumerable<T> items, Brush headerColor)
            {
                this.Title = headerTitle;
                this.FooterTitle = "";
                foreach (T item in items)
                {
                    this.Add(item);
                }
                this.HeaderColor = headerColor;
                // we set the default colors according to the theme of the phone
                SolidColorBrush phoneAccentBrush = Application.Current.Resources["PhoneAccentBrush"] as SolidColorBrush;
                this.FooterColor = phoneAccentBrush;
                this.FooterVisibility = Visibility.Collapsed;
                this.GroupHeaderColor = phoneAccentBrush;
            }

            public ListSection(string headerTitle, string footerTitle, IEnumerable<T> items, Brush headerColor)
            {
                this.Title = headerTitle;
                this.FooterTitle = footerTitle;
                foreach (T item in items)
                {
                    this.Add(item);
                }
                this.HeaderColor = headerColor;
                // we set the default colors according to the theme of the phone
                SolidColorBrush phoneAccentBrush = Application.Current.Resources["PhoneAccentBrush"] as SolidColorBrush;
                this.FooterColor = phoneAccentBrush;
                this.FooterVisibility = Visibility.Collapsed;
                this.GroupHeaderColor = phoneAccentBrush;
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

            public string FooterTitle
            {
                get;
                set;
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
