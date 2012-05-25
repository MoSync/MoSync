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
 * @file CustomNumberPickerPage.xaml.cs
 * @author Filipas Ciprian
 *
 * @brief This represents the partial implementation for the CustomNumberPickerPageClass.
 *
 * @platform WP 7.1
 **/

using System;
using System.Windows;
using System.Windows.Controls;

using Microsoft.Phone.Controls;
using Microsoft.Phone.Controls.Primitives;

using MoSync;

namespace mosyncRuntime.Views
{
    public partial class CustomNumberPickerPage : PhoneApplicationPage
    {
        // The min and max values.
        private int mMin = 0, mMax = 100;

        // The next value that will be saved
        private int mNextValue = 0;

        // The current value
        private int? mValue;

        // The primary selector refference.
        private LoopingSelector mPrimarySelectorPart;

        // The constructor.
        public CustomNumberPickerPage()
        {
            InitializeComponent();

            // Creating the data sources.
            this.PrimarySelector.DataSource = new BoundedNumberDataSource();

            (this.ApplicationBar.Buttons[0] as Microsoft.Phone.Shell.ApplicationBarIconButton).Click += OnDoneButtonClick;
            (this.ApplicationBar.Buttons[1] as Microsoft.Phone.Shell.ApplicationBarIconButton).Click += OnCancelButtonClick;

            InitializeNumberPickerpage(PrimarySelector);
        }

        /**
         * @author Ciprian Filipas
         * @brief This function is used for initializing the looping selector
         */
        protected void InitializeNumberPickerpage(LoopingSelector primarySelector)
        {
            if (null == primarySelector)
            {
                throw new ArgumentOutOfRangeException("primarySelector");
            }

            mPrimarySelectorPart = primarySelector;

            mPrimarySelectorPart.DataSource.SelectionChanged += OnDataSourceSelectionChanged;
        }

        /**
         * @author Ciprian Filipas
         * @brief The SelectionChanged event handler for the primary selector data source.
         */
        private void OnDataSourceSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            // Extract the DataSource.
            BoundedNumberDataSource source = (BoundedNumberDataSource)sender;

            // If we have a value and it is valid the _nextDate is updated.
            if (source.SelectedItem != null && this.Value.HasValue)
            {
                mNextValue = (int)source.SelectedItem;
            }

            this.mPrimarySelectorPart.DataSource.SelectedItem = mNextValue;
        }

        /**
         * @author Ciprian Filipas
         * @brief The Done button click event handler.
         */
        private void OnDoneButtonClick(object sender, EventArgs e)
        {
            mValue = mNextValue;
            ClosePickerPage();
        }

        /**
         * @author Ciprian Filipas
         * @brief The Cancel button click event handler.
         */
        private void OnCancelButtonClick(object sender, EventArgs e)
        {
            mValue = null;
            ClosePickerPage();
        }

        /**
         * @author Ciprian Filipas
         * @brief This function closes the picker page by navigating back
         */
        private void ClosePickerPage()
        {
            NavigationService.GoBack();
        }

        /**
         * @author: Filipas Ciprian
         * @brief: This is an override for the OnNavigatedTo function; it handles the parameter extraction from
         *         the URI and sets the mMin, mMax values.
         * @param: e System.Windows.Navigation.NavigationEventArgs the event arguments.
         */
        protected override void OnNavigatedTo(System.Windows.Navigation.NavigationEventArgs e)
        {
            String parameter1 = null, parameter2 = null;

            // Extract the parameters from the URI
            if (NavigationContext.QueryString.Keys.Contains("Max")) parameter1 = NavigationContext.QueryString["Max"];
            if (NavigationContext.QueryString.Keys.Contains("Min")) parameter2 = NavigationContext.QueryString["Min"];

            // If the parameters exist then create the min, max instances
            if (null != parameter1 && null != parameter2)
            {

                if (Int32.TryParse(parameter2, out mMin) == false)
                    mMin = 0;
                if (Int32.TryParse(parameter1, out mMax) == false)
                    mMax = 100;
            }
            // Else create them with the default values
            else
            {
                mMin = 0;
                mMax = 100;
            }

            // The LoopingSelectors need to be aware of the mMin and mMax dates
            (this.PrimarySelector.DataSource as BoundedNumberDataSource).Min = mMin;
            (this.PrimarySelector.DataSource as BoundedNumberDataSource).Max = mMax;

            // Call the base
            base.OnNavigatedTo(e);
        }

        /**
         * @author: Filipas Ciprian
         * @brief: Value property
         */
        public int? Value
        {
            get { return mValue; }
            set
            {
                mValue = value;
                mPrimarySelectorPart.DataSource.SelectedItem = (int)value;
            }
        }
    }
}