/*
Copyright (C) 2011 MoSync AB

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

/*! \addtogroup NativeUILib
 *  @{
 */

/**
 *  @defgroup NativeUILib Native UI Library
 *  @{
 */

/**
 * @file DatePicker.h
 * @author Emma Tresanszki
 *
 * \brief An instance of DatePicker allow to an users to select dates.
 * For date picker events see DatePickerListener.
 */

#ifndef NATIVEUI_DATE_PICKER_H_
#define NATIVEUI_DATE_PICKER_H_

#include "Widget.h"

    // Forward declaration.
    struct tm;
namespace NativeUI
{

    // Forward declaration.
    class DatePickerListener;

/**
 * \brief Struct to hold a date.
 */
	struct Date{
		int day; // Range: 1-31.
		int month; // Range: 1-12.
		int year;
		Date(int aDay, int aMonth, int anYear):
			day(aDay),month(aMonth),year(anYear){};
		Date(){};
	};

	/**
	 * \brief A Date Picker is a widget for selecting a date.
	 *
	 * The date picker is initialized with system date.
	 * The date can be selected by a year, month, and day spinners.
	 * The minimal and maximal date from which dates to be selected
	 * can be customized.
	 * The picker is initialized with the current system date.
	 *
	 * Note: The default minimal date on Android is 01/01/1900.
	 * Note: The default maximal date on Android is 12/31/2100.
	 * Note: This is not available on Windows Phone 7.
	 */
	class DatePicker : public Widget
	{
	public:
		/**
		 * Constructor.
		 */
		DatePicker();

		/**
		 * Destructor.
		 */
		virtual ~DatePicker();

		/**
		 * Set the current date of the picker.
		 * @param date A date struct that specifies a valid date.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the property could be set.
		 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
		 * - #MAW_RES_ERROR otherwise.
		 */
		virtual int setDate(const struct Date date);

		/**
		 * Get the currently displayed date.
		 * @return A Date struct that specifies the displayed date.
		 */
		virtual Date getDate();

		/**
		 * Set the current year of the picker.
		 * @param year An int that specifies a valid year.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the property could be set.
		 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
		 * - #MAW_RES_ERROR otherwise.
		 */
		virtual int setYear(const int year);

		/**
		 * Get the currently displayed year.
		 * @return An int that specifies the current year.
		 */
		virtual int getYear();

		/**
		 * Set the current month of the picker.
		 * @param month An int that specifies the current month.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the property could be set.
		 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
		 * - #MAW_RES_ERROR otherwise.
		 */
		virtual int setMonth(const int month);

		/**
		 * Get the currently displayed month.
		 * @return An int that specifies the current month.
		 */
		virtual int getMonth();

		/**
		 * Set the current day of month.
		 * @param dayOfMonth An int that specifies the current day of month.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the property could be set.
		 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
		 * - #MAW_RES_ERROR otherwise.
		 */
		virtual int setDayOfMonth(const int dayOfMonth);

		/**
		 * Get the currently displayed day.
		 * @return An int that specifies the current day of month.
		 */
		virtual int getDayOfMonth();

		/**
		 * Set the minimum date of the picker.
		 * Note: this property is not yet available for Android.
		 * @param minDate A struct that specifies the minimum date.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the property could be set.
		 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
		 * - #MAW_RES_FEATURE_NOT_AVAILABLE if the widget type is not supported
		 * by the curent framework version.
		 * - #MAW_RES_ERROR otherwise.
		 */
		virtual int setMinDate( const struct Date minDate);

		/**
		 * Get the minimum date of the picker.
		 * Note: this property is not yet available for Android.
		 * @return A Date struct that specifies the minimum date.
		 */
		virtual struct Date getMinDate();

		/**
		 * Set the maximum date of the picker.
		 * Note: this property is not yet available for Android.
		 * Note: The default maximal date on Android is 12/31/2100.
		 * @param maxDate A struct that specifies the maximum date.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the property could be set.
		 * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
		 * - #MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
		 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
		 * - #MAW_RES_FEATURE_NOT_AVAILABLE if the widget type is not supported
		 * by the curent framework version.
		 * - #MAW_RES_ERROR otherwise.
		 */
		virtual int setMaxDate( const struct Date maxDate);

		/**
		 * Get the maximum value of the picker.
		 * Note: this property is not yet available for Android.
		 * @return A Date struct that specifies the minimum date.
		 */
		virtual struct Date getMaxDate();

        /**
         * Add an date picker event listener.
         * @param listener The listener that will receive date picker events.
         */
        virtual void addDatePickerListener(DatePickerListener* listener);

        /**
         * Remove the date picker listener.
         * @param listener The listener that receives date picker events.
         */
        virtual void removeDatePickerListener(DatePickerListener* listener);

    protected:
        /**
         * This method is called when there is an event for this widget.
         * It passes on the event to all widget's listeners.
         * @param widgetEventData The data for the widget event.
         */
        virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData);

    private:
        /**
         * Array with date picker listeners.
         */
        MAUtil::Vector<DatePickerListener*> mDatePickerListeners;

        /**
         * Auxiliary data used for the getters
         */
        Date* mMaxDate;
        Date* mMinDate;
        Date* mDisplayedDate;
	};

} // namespace NativeUI

#endif

/*! @} */
