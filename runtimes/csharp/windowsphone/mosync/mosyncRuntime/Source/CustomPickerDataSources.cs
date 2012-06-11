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
 * @file CustomPickerDataSources.cs
 * @author Filipas Ciprian
 *
 * @brief This file contains the picker source representatiosn for the custom picker looping selectors.
 *        This classes will be used in the specific picker implementations.
 * @platform WP 7.1
 **/

using System;
using System.Windows.Controls;

using WP7Contrib.View.Controls.DateTimePicker;

namespace MoSync
{
    /**
     * @author: Filipas Ciprian
     * @brief: This class is a simplified representations to a DateTime object
     *         which uses only the date part (e.g. year, month, day).
     *         It is used as an optimisation for the large DateTime object.
     */
    public class SimpleDateStorage
    {
        public int Year { set; get; }
        public int Month { set; get; }
        public int Day { set; get; }

        public SimpleDateStorage(int y, int m, int d)
        {
            Year = y;
            Month = m;
            Day = d;
        }
    }

    /**
     * @author: Filipas Ciprian
     * @brief: This class is an extension for the YearDataSource class.
     */
    public class BoundedYearDataSource : YearDataSource
    {
        //MaxDate refference
        public SimpleDateStorage MaxDate { set; get; }

        //MinDate refference
        public SimpleDateStorage MinDate { set; get; }

        public BoundedYearDataSource() { }

        /**
         * @function GetRelativeTo
         * @brief Computes the valid dates relative to the relativeDate parameter taking
         *        in account the delta parameter (-1 or 1). This function is called from the
         *        GetNext and GetPrevious functions inherited from YearDataSource class.
         * @param1 relativeDate DateTime represents the date taken in account for the
         *        computations involving the previous/next elements inside a looping selector
         * @param2 delta int represents the delta for the calculus, -1 implies the computations
         *        for a previous date and 1 for a next date.
         * @return DateTime object relative to the value given as param1 regarding param2
         */
        protected override DateTime? GetRelativeTo(DateTime relativeDate, int delta)
        {
            int nextYear, nextDay;

            // If the year is equal with the MaxDate.year (MinDate.year) value and
            // delta implies a next (previous) value request null will be returned meaning
            // that no valid value is available.
            if ((MaxDate.Year == relativeDate.Year) && delta > 0)
            {
                return null;
            }
            if ((MinDate.Year == relativeDate.Year) && delta < 0)
            {
                return null;
            }

            // Compute the nextYear value and return the new DateTime object
            nextYear = relativeDate.Year + delta;
            nextDay = Math.Min(relativeDate.Day, DateTime.DaysInMonth(nextYear, relativeDate.Month));
            return new DateTime(nextYear, relativeDate.Month, nextDay, relativeDate.Hour, relativeDate.Minute, relativeDate.Second);
        }
    }

    /**
    * @author: Filipas Ciprian
    * @brief: This class is an extension for the MonthDataSource class.
    */
    public class BoundedMonthDataSource : MonthDataSource
    {
        //MaxDate refference
        public SimpleDateStorage MaxDate { set; get; }

        //MinDate refference
        public SimpleDateStorage MinDate { set; get; }

        public BoundedMonthDataSource() { }

        /**
         * @function GetRelativeTo
         * @brief Computes the valid dates relative to the relativeDate parameter taking
         *        in account the delta parameter (-1 or 1). This function is called from the
         *        GetNext and GetPrevious functions inherited from MonthDataSource class.
         * @param1 relativeDate DateTime represents the date taken in account for the
         *        computations involving the previous/next elements inside a looping selector
         * @param2 delta int represents the delta for the calculus, -1 implies the computations
         *        for a previous date and 1 for a next date.
         * @return DateTime object relative to the value given as param1 regarding param2
         */
        protected override DateTime? GetRelativeTo(DateTime relativeDate, int delta)
        {
            Microsoft.Phone.Controls.Primitives.DateTimeWrapper currentDate = (Microsoft.Phone.Controls.Primitives.DateTimeWrapper)this.SelectedItem;

            int currentYear = currentDate.DateTime.Year;
            int monthsInYear = 12;

            // If the currentYear has the same value as the MaxDate.Year then the number of months inside this year
            // will be equal to the month number of the MaxDate.Year

            if (currentYear == MaxDate.Year)
                monthsInYear = MaxDate.Month;

            // Computing the nextMonth, taking in acount the delta (-1 or 1)
            int nextMonth = ((monthsInYear + relativeDate.Month - 1 + delta) % monthsInYear) + 1;

            // If our month makes the return date to out of bounds (regarding the Min and Max Dates
            // we return null instead of the new date; this will make the looping stop at this certain value
            if (MaxDate.Year == currentYear &&
                MaxDate.Year == MinDate.Year &&
                (nextMonth < MinDate.Month || nextMonth > MaxDate.Month))
            {
                return null;
            }

            // Compute the next day
            int nextDay = Math.Min(relativeDate.Day, DateTime.DaysInMonth(relativeDate.Year, nextMonth));

            // Return the newly calculated DateTime object
            return new DateTime(relativeDate.Year, nextMonth, nextDay, relativeDate.Hour, relativeDate.Minute, relativeDate.Second);
        }
    }

    /**
    * @author: Filipas Ciprian
    * @brief: This class is an extension for the MonthDataSource class.
    */
    public class BoundedDayDataSource : DayDataSource
    {
        //MaxDate refference
        public SimpleDateStorage MaxDate { set; get; }

        //MinDate refference
        public SimpleDateStorage MinDate { set; get; }

        public BoundedDayDataSource() {}

        /**
         * @function GetRelativeTo
         * @brief Computes the valid dates relative to the relativeDate parameter taking
         *        in account the delta parameter (-1 or 1). This function is called from the
         *        GetNext and GetPrevious functions inherited from DayDataSource class.
         * @param1 relativeDate DateTime represents the date taken in account for the
         *        computations involving the previous/next elements inside a looping selector
         * @param2 delta int represents the delta for the calculus, -1 implies the computations
         *        for a previous date and 1 for a next date.
         * @return DateTime object relative to the value given as param1 regarding param2
         */
        protected override DateTime? GetRelativeTo(DateTime relativeDate, int delta)
        {
            Microsoft.Phone.Controls.Primitives.DateTimeWrapper currentDate = (Microsoft.Phone.Controls.Primitives.DateTimeWrapper)this.SelectedItem;

            int currentMonth = currentDate.DateTime.Month;
            int currentYear = currentDate.DateTime.Year;

            // If the date that we try to compute will be larger than the max date.
            if ((MaxDate.Day == relativeDate.Day) && (MaxDate.Month == currentMonth) &&
                (MaxDate.Year == currentYear) && (delta > 0))
            {
                return null;
            }

            // If the date we try to compute will be smaller than the min date.
            if ((MinDate.Day == relativeDate.Day) && (MinDate.Month == currentMonth) &&
                (MinDate.Year == currentYear) && (delta < 0))
            {
                return null;
            }

            // Compute the days in month value.
            int daysInMonth = DateTime.DaysInMonth(currentYear, currentMonth);

            // Compute the next day value.
            int nextDay = ((((daysInMonth + relativeDate.Day) - 1) + delta) % daysInMonth) + 1;

            // The special case in which we have the max year / monthe equal with the min year / month and basically
            // we have just a day range
            if (MaxDate.Year == currentYear && MaxDate.Month == currentMonth &&
                MaxDate.Year == MinDate.Year && MaxDate.Month == MinDate.Month &&
                (nextDay < MinDate.Day || nextDay > MaxDate.Day))
            {
                return null;
            }

            if ((MaxDate.Year == currentYear && MaxDate.Month == currentMonth && nextDay > MaxDate.Day) || (MinDate.Year == currentYear && MinDate.Month == currentMonth && nextDay < MinDate.Day))
                return null;

            return new DateTime(currentYear, currentMonth, nextDay, relativeDate.Hour, relativeDate.Minute, relativeDate.Second);
        }
    }

    /**
    * @author: Filipas Ciprian
    * @brief: This class is an extension for the ILoopingSelectorDataSource class. It represents
    *         the data source for the number picker selector
    */
    public class BoundedNumberDataSource : Microsoft.Phone.Controls.Primitives.ILoopingSelectorDataSource
    {
        // The currently selected item
        private int _selectedItem;

        // Max
        public int Max { set; get; }

        // Min
        public int Min  { set; get; }

        // Constructor
        public BoundedNumberDataSource() { }

        /**
         * @author Ciprian Filipas
         * @brief SelectedItem property
         */
        public object SelectedItem
        {
            // Getter
            get
            {
                return _selectedItem;
            }
            // Setter
            set
            {
                int val = (int)value;
                // If the value is different from the currenlty selected item
                if (val != _selectedItem)
                {
                    // Save the previous
                    int previousSelectedItem = _selectedItem;
                    _selectedItem = (int)value;

                    // Set the handler
                    var handler = SelectionChanged;

                    // If the handler exists call it
                    if (null != handler)
                    {
                        handler(this, new SelectionChangedEventArgs(new object[] { previousSelectedItem }, new object[] { _selectedItem }));
                    }
                }
            }
        }

        /**
         * @author Ciprian Filipas
         * @brief Gets the next value relative to the give one
         * @param1 relativeTo int relative value required in the computations
         */
        public object GetNext(object relativeTo)
        {
            if (((int)relativeTo + 1) <= Max)
                return ((int)relativeTo + 1);
            return null;
        }

        /**
         * @author Ciprian Filipas
         * @brief Gets the previous value relative to the give one
         * @param1 relativeTo int relative value required in the computations
         */
        public object GetPrevious(object relativeTo)
        {
            if (((int)relativeTo - 1) >= Min)
                return ((int)relativeTo - 1);
            return null;
        }

        // Event handler for the SelectionChanged event
        public event EventHandler<SelectionChangedEventArgs> SelectionChanged;
    }
}
