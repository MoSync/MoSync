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
 * @file MoSyncNumberPicker.cs
 * @author Filipas Ciprian
 *
 * @brief This represents the partial implementation for the CustomNumberPickerPageClass.
 *
 * @platform WP 7.1
 **/

using System;
using System.Collections;
using System.Collections.Generic;
using System.Windows.Controls;
using System.Globalization;

using Microsoft.Phone.Controls.Primitives;
using WP7Contrib.View.Controls.DateTimePicker;
using MoSync;

namespace mosyncRuntime.Views
{
    public partial class CustomDatePickerPage
    {
        // Default values for max and min year.
        private const int mMaxYear = 3000;
        private const int mMinYear = 1600;

        // Min date and Max date refferences.
        private DateTime minDate, maxDate;

        // The next value of the picker; this.value will be set on mNextDate when the done button
        // gets clicked.
        private DateTime mNextDate;

        // Constructor.
        public CustomDatePickerPage()
        {
            this.InitializeComponent();

            // Creating the data sources.
            this.PrimarySelector.DataSource = new BoundedYearDataSource();
            this.SecondarySelector.DataSource = new BoundedMonthDataSource();
            this.TertiarySelector.DataSource = new BoundedDayDataSource();

            // Handler for the AltSelectionChanged; this is inherited from the DataSource class
            ((DataSource)this.PrimarySelector.DataSource).AltSelectionChanged += this.HandleAltDataSourceSelectionChangedPrimary;
            ((DataSource)this.SecondarySelector.DataSource).AltSelectionChanged += this.HandleAltDataSourceSelectionChangedSecondary;
            ((DataSource)this.TertiarySelector.DataSource).AltSelectionChanged += this.HandleAltDataSourceSelectionChangedTertiary;

            // Done button handler
            if (this.ApplicationBar.Buttons.Count > 1)
            {
                (this.ApplicationBar.Buttons[0] as Microsoft.Phone.Shell.ApplicationBarIconButton).Click += new EventHandler(
                    delegate(object sender, EventArgs e)
                    {
                        this.Value = mNextDate;
                    });
            }

            // Initializing the datetime picker page; this is inherited from DateTimePickerPageBase
            this.InitializeDateTimePickerPage(this.PrimarySelector, this.SecondarySelector, this.TertiarySelector);
        }

        /**
         * @author: Filipas Ciprian
         * @brief: This is an override for the OnNavigatedTo function; it handles the parameter extraction from
         *         the URI and creates the minDate, maxDate instances.
         * @param: e System.Windows.Navigation.NavigationEventArgs the event arguments.
         */
        protected override void OnNavigatedTo(System.Windows.Navigation.NavigationEventArgs e)
        {
            String parameter1 = null, parameter2 = null;

            // Extract the parameters from the URI
            if(NavigationContext.QueryString.Keys.Contains("MaxDate")) parameter1 = NavigationContext.QueryString["MaxDate"];
            if(NavigationContext.QueryString.Keys.Contains("MinDate")) parameter2 = NavigationContext.QueryString["MinDate"];

            // If the parameters exist then create the minDate, maxDate instances
            if (null != parameter1 && null != parameter2)
            {
                minDate = new DateTime(Int32.Parse(parameter2.Split('/')[0]), Int32.Parse(parameter2.Split('/')[1]), Int32.Parse(parameter2.Split('/')[2]));
                maxDate = new DateTime(Int32.Parse(parameter1.Split('/')[0]), Int32.Parse(parameter1.Split('/')[1]), Int32.Parse(parameter1.Split('/')[2]));
            }
            // Else create them with the default values
            else
            {
                maxDate = new DateTime(mMaxYear, 12, 31);
                minDate = new DateTime(mMinYear, 1, 1);
            }

            // The LoopingSelectors need to be aware of the min and max dates
            (this.PrimarySelector.DataSource as BoundedYearDataSource).MinDate = new SimpleDateStorage(minDate.Year, minDate.Month, minDate.Day);
            (this.PrimarySelector.DataSource as BoundedYearDataSource).MaxDate = new SimpleDateStorage(maxDate.Year, maxDate.Month, maxDate.Day);
            (this.SecondarySelector.DataSource as BoundedMonthDataSource).MinDate = new SimpleDateStorage(minDate.Year, minDate.Month, minDate.Day);
            (this.SecondarySelector.DataSource as BoundedMonthDataSource).MaxDate = new SimpleDateStorage(maxDate.Year, maxDate.Month, maxDate.Day);
            (this.TertiarySelector.DataSource as BoundedDayDataSource).MinDate = new SimpleDateStorage(minDate.Year, minDate.Month, minDate.Day);
            (this.TertiarySelector.DataSource as BoundedDayDataSource).MaxDate = new SimpleDateStorage(maxDate.Year, maxDate.Month, maxDate.Day);

            // Call the base
            base.OnNavigatedTo(e);
        }

       /**
        * @author: Filipas Ciprian
        * @brief: Handle the alt selection change event for the primary (year) LoopingSelector.
        * @param1: sender object the object that triggered the event.
        * @param2: e SelectionChangedEventArgs the event arguments.
        */
        private void HandleAltDataSourceSelectionChangedPrimary(object sender, SelectionChangedEventArgs e)
        {
            // Extract the DataSource.
            DataSource source = (DataSource)sender;

            // If we have a value and it is valid the mNextDate is updated.
            if (source.SelectedItem != null && this.Value.HasValue)
            {
                if (null != mNextDate)
                {
                    // Check if the currently set day remains valid after changing the year value otherwise update it.
                    // (e.g. 29 Feb 2012 during an year change to 2011 will generate the 28 Feb 2011 date).
                    if (mNextDate.Day <= DateTime.DaysInMonth(((DateTimeWrapper)source.SelectedItem).DateTime.Year, mNextDate.Month))
                        mNextDate = new DateTime(((DateTimeWrapper)source.SelectedItem).DateTime.Year, mNextDate.Month, mNextDate.Day);
                    else
                        mNextDate = new DateTime(((DateTimeWrapper)source.SelectedItem).DateTime.Year, mNextDate.Month, DateTime.DaysInMonth(((DateTimeWrapper)source.SelectedItem).DateTime.Year, mNextDate.Month));
                }
                else mNextDate = new DateTime(((DateTimeWrapper)source.SelectedItem).DateTime.Year, ((DateTimeWrapper)source.SelectedItem).DateTime.Month, ((DateTimeWrapper)source.SelectedItem).DateTime.Day);
            }

            // Update the year for the loopingSelectors.DataSource.SelectedItem if necesarly.
            if (null == this.PrimarySelector.DataSource.SelectedItem)
                this.PrimarySelector.DataSource.SelectedItem = new DateTimeWrapper(mNextDate);
            else if (!(this.PrimarySelector.DataSource.SelectedItem as DateTimeWrapper).DateTime.Equals(mNextDate))
                this.PrimarySelector.DataSource.SelectedItem = new DateTimeWrapper(mNextDate);

            if(null == this.SecondarySelector.DataSource.SelectedItem)
                this.SecondarySelector.DataSource.SelectedItem = new DateTimeWrapper(mNextDate);
            else if (!(this.SecondarySelector.DataSource.SelectedItem as DateTimeWrapper).DateTime.Equals(mNextDate) &&
                    (this.SecondarySelector.DataSource.SelectedItem as DateTimeWrapper).DateTime.Year != mNextDate.Year)
                this.SecondarySelector.DataSource.SelectedItem = new DateTimeWrapper(mNextDate);

            if (null == this.TertiarySelector.DataSource.SelectedItem)
                this.TertiarySelector.DataSource.SelectedItem = source.SelectedItem;
            else if (!(this.TertiarySelector.DataSource.SelectedItem as DateTimeWrapper).DateTime.Equals(mNextDate) &&
                     (this.TertiarySelector.DataSource.SelectedItem as DateTimeWrapper).DateTime.Year != (mNextDate.Year))
                this.TertiarySelector.DataSource.SelectedItem = new DateTimeWrapper(mNextDate);
        }

        /**
        * @author: Filipas Ciprian
        * @brief: Handle the alt selection change event for the secondary (month) LoopingSelector.
        * @param1: sender object the object that triggered the event.
        * @param2: e SelectionChangedEventArgs the event arguments.
        */
        private void HandleAltDataSourceSelectionChangedSecondary(object sender, SelectionChangedEventArgs e)
        {
            // Extract the DataSource.
            DataSource source = (DataSource)sender;

            // If we have a value and it is valid the mNextDate is updated.
            if (source.SelectedItem != null && this.Value.HasValue)
            {
                if (null != mNextDate)
                {
                    // Check if the currently set day remains valid after changing the month value otherwise update it.
                    // (e.g. 31 May 2012 during an month change to Feb will generate the 29 Feb 2012 date).
                    if( mNextDate.Day <= DateTime.DaysInMonth(mNextDate.Year, ((DateTimeWrapper)source.SelectedItem).DateTime.Month))
                        mNextDate = new DateTime(mNextDate.Year, ((DateTimeWrapper)source.SelectedItem).DateTime.Month, mNextDate.Day);
                    else
                        mNextDate = new DateTime(mNextDate.Year, ((DateTimeWrapper)source.SelectedItem).DateTime.Month, DateTime.DaysInMonth(mNextDate.Year, ((DateTimeWrapper)source.SelectedItem).DateTime.Month));
                }
                else mNextDate = new DateTime(((DateTimeWrapper)source.SelectedItem).DateTime.Year, ((DateTimeWrapper)source.SelectedItem).DateTime.Month, ((DateTimeWrapper)source.SelectedItem).DateTime.Day);
            }

            // Update the month for the loopingSelectors.DataSource.SelectedItem if necesarly.
            if (null == this.PrimarySelector.DataSource.SelectedItem)
                this.PrimarySelector.DataSource.SelectedItem = new DateTimeWrapper(mNextDate);
            else if (!(this.PrimarySelector.DataSource.SelectedItem as DateTimeWrapper).DateTime.Equals(mNextDate))
                this.PrimarySelector.DataSource.SelectedItem = new DateTimeWrapper(mNextDate);

            if (null == this.SecondarySelector.DataSource.SelectedItem)
                this.SecondarySelector.DataSource.SelectedItem = new DateTimeWrapper(mNextDate);
            else if (!(this.SecondarySelector.DataSource.SelectedItem as DateTimeWrapper).DateTime.Equals(mNextDate) &&
                    (this.SecondarySelector.DataSource.SelectedItem as DateTimeWrapper).DateTime.Month != mNextDate.Month)
                this.SecondarySelector.DataSource.SelectedItem = new DateTimeWrapper(mNextDate);

            if (null == this.TertiarySelector.DataSource.SelectedItem)
                this.TertiarySelector.DataSource.SelectedItem = source.SelectedItem;
            else if (!(this.TertiarySelector.DataSource.SelectedItem as DateTimeWrapper).DateTime.Equals(mNextDate) &&
                     (this.TertiarySelector.DataSource.SelectedItem as DateTimeWrapper).DateTime.Month != (mNextDate.Month))
                this.TertiarySelector.DataSource.SelectedItem = new DateTimeWrapper(mNextDate);

            // Create a DispatcherTimer that will Tick after 300 ms in order to update LoopingSelectors.
            // This is necesarly in order to avoid animation flickering and UI related bugs.
            System.Windows.Threading.DispatcherTimer dt = new System.Windows.Threading.DispatcherTimer();
            dt.Interval = new TimeSpan(0, 0, 0, 0, 300);

            // Since both Secondary and Tertiary selectors need to be updated 2 thicks are required.
            bool shouldStop = false;

            dt.Tick += new EventHandler(
                delegate(object s, EventArgs eargs)
                {
                    if(shouldStop)
                    {
                        TertiarySelector.UpdateData();
                        // If the new date is not valid, generate the closest valid date.
                        if ((TertiarySelector.DataSource.SelectedItem as DateTimeWrapper).DateTime < minDate)
                        {
                            DateTime currentDate = (TertiarySelector.DataSource.SelectedItem as DateTimeWrapper).DateTime;
                            TertiarySelector.DataSource.SelectedItem = new DateTimeWrapper(new DateTime(currentDate.Year, currentDate.Month, minDate.Day));
                        }
                        else if((TertiarySelector.DataSource.SelectedItem as DateTimeWrapper).DateTime > maxDate)
                        {
                            DateTime currentDate = (TertiarySelector.DataSource.SelectedItem as DateTimeWrapper).DateTime;
                            TertiarySelector.DataSource.SelectedItem = new DateTimeWrapper(new DateTime(currentDate.Year, currentDate.Month, maxDate.Day));
                        }

                        // Stop the dispatcher timer.
                        dt.Stop();
                    }
                    else
                    {
                        // Should stop the in the next tick after updating the Secondary selector.
                        shouldStop = true;
                        SecondarySelector.UpdateData();

                        // If the new date is not valid, generate the closest valid date.
                        if ((SecondarySelector.DataSource.SelectedItem as DateTimeWrapper).DateTime < minDate)
                        {
                            DateTime currentDate = (SecondarySelector.DataSource.SelectedItem as DateTimeWrapper).DateTime;
                            if(currentDate.Day >= minDate.Day)
                                SecondarySelector.DataSource.SelectedItem = new DateTimeWrapper(new DateTime(currentDate.Year, minDate.Month, currentDate.Day));
                            else
                                SecondarySelector.DataSource.SelectedItem = new DateTimeWrapper(new DateTime(currentDate.Year, minDate.Month, minDate.Day));
                        }
                        else if((SecondarySelector.DataSource.SelectedItem as DateTimeWrapper).DateTime > maxDate)
                        {
                            DateTime currentDate = (SecondarySelector.DataSource.SelectedItem as DateTimeWrapper).DateTime;
                            if(currentDate.Day <= maxDate.Day)
                                SecondarySelector.DataSource.SelectedItem = new DateTimeWrapper(new DateTime(currentDate.Year, maxDate.Month, currentDate.Day));
                            else
                                SecondarySelector.DataSource.SelectedItem = new DateTimeWrapper(new DateTime(currentDate.Year, maxDate.Month, maxDate.Day));
                        }
                    }
                });

            // Start the dispatcher timer.
            dt.Start();
        }

        /**
        * @author: Filipas Ciprian
        * @brief: Handle the alt selection change event for the tertiary (day) LoopingSelector.
        * @param1: sender object the object that triggered the event.
        * @param2: e SelectionChangedEventArgs the event arguments.
        */
        private void HandleAltDataSourceSelectionChangedTertiary(object sender, SelectionChangedEventArgs e)
        {
            // Extract the DataSource.
            DataSource source = (DataSource)sender;

            // If we have a value and it is valid the mNextDate is updated.
            if (source.SelectedItem != null && this.Value.HasValue)
            {
                if (null != mNextDate)
                {
                    mNextDate = new DateTime(mNextDate.Year, mNextDate.Month, ((DateTimeWrapper)source.SelectedItem).DateTime.Day);
                }
                else mNextDate = new DateTime(((DateTimeWrapper)source.SelectedItem).DateTime.Year, ((DateTimeWrapper)source.SelectedItem).DateTime.Month, ((DateTimeWrapper)source.SelectedItem).DateTime.Day);
            }

            // Update the day for the loopingSelectors.DataSource.SelectedItem if necesarly.
            if (null == this.PrimarySelector.DataSource.SelectedItem)
                this.PrimarySelector.DataSource.SelectedItem = new DateTimeWrapper(mNextDate);
            else if (!(this.PrimarySelector.DataSource.SelectedItem as DateTimeWrapper).DateTime.Equals(mNextDate))
                this.PrimarySelector.DataSource.SelectedItem = new DateTimeWrapper(mNextDate);

            if (null == this.SecondarySelector.DataSource.SelectedItem)
                this.SecondarySelector.DataSource.SelectedItem = new DateTimeWrapper(mNextDate);
            else if (!(this.SecondarySelector.DataSource.SelectedItem as DateTimeWrapper).DateTime.Equals(mNextDate) &&
                    (this.SecondarySelector.DataSource.SelectedItem as DateTimeWrapper).DateTime.Day != mNextDate.Day)
                this.SecondarySelector.DataSource.SelectedItem = new DateTimeWrapper(mNextDate);

            if (null == this.TertiarySelector.DataSource.SelectedItem)
                this.TertiarySelector.DataSource.SelectedItem = source.SelectedItem;
            else if (!(this.TertiarySelector.DataSource.SelectedItem as DateTimeWrapper).DateTime.Equals(mNextDate) &&
                     (this.TertiarySelector.DataSource.SelectedItem as DateTimeWrapper).DateTime.Day != (mNextDate.Day))
                this.TertiarySelector.DataSource.SelectedItem = new DateTimeWrapper(mNextDate);
        }

        /**
         * @brief: Gets a sequence of LoopingSelector parts ordered according to culture string for date/time formatting.
         * @return LoopingSelectors ordered by culture-specific priority.
         */
        protected override IEnumerable<LoopingSelector> GetSelectorsOrderedByCulturePattern()
        {
            return GetSelectorsOrderedByCulturePattern(CultureInfo.CurrentCulture.DateTimeFormat.ShortDatePattern.ToUpperInvariant(), new char[] { 'Y', 'M', 'D' }, new LoopingSelector[] { this.PrimarySelector, this.SecondarySelector, this.TertiarySelector });
        }
    }
}