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

/**
 * @file DatePicker.cpp
 * @author Emma Tresanszki
 *
 * An instance of DatePicker allow to an users to select dates.
 * For date picker events see DatePickerListener.
 */

// Used for converting Date struct to tm struct.
#define CONVERT_YEARS_UNIT 1900
#define CONVERT_MONTHS_UNIT 1

#include <matime.h>

#include <MAUtil/util.h>
#include <conprint.h>

#include "DatePicker.h"
#include "DatePickerListener.h"

namespace NativeUI
{

	/**
	 * Constructor.
	 */
	DatePicker::DatePicker() :
		Widget(MAW_DATE_PICKER)
	{
	}

	/**
	 * Destructor.
	 */
	DatePicker::~DatePicker()
	{
        mDatePickerListeners.clear();
	}

	/**
	 * Set the current date of the picker.
	 * @param date A date struct that specifies a valid date.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the property could be set.
	 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
	 * - #MAW_RES_ERROR otherwise.
	 */
	int DatePicker::setDate(const struct Date date)
	{
		int resultYear = setYear(date.year);
		if ( resultYear == MAW_RES_OK )
		{
			int resultMonth = setMonth(date.month);
			if (resultMonth == MAW_RES_OK )
			{
				return (setDayOfMonth(date.day));
			}
			else
			{
				return resultMonth;
			}
		}
		else
		{
			return resultYear;
		}
	}

	/**
	 * Get the currently displayed date.
	 * @return A Date struct that specifies the displayed date.
	 */
	Date DatePicker::getDate() const
	{
		Date displayedDate;
		displayedDate.year = this->getPropertyInt(MAW_DATE_PICKER_YEAR);
		displayedDate.month = this->getPropertyInt(MAW_DATE_PICKER_MONTH);
		displayedDate.day = this->getPropertyInt(MAW_DATE_PICKER_DAY_OF_MONTH);

		return displayedDate;
	}

	/**
	 * Set the current year of the picker.
	 * @param year An int that specifies a valid year.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the property could be set.
	 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
	 * - #MAW_RES_ERROR otherwise.
	 */
	int DatePicker::setYear(const int year)
	{
		return this->setPropertyInt(MAW_DATE_PICKER_YEAR, year);
	}

	/**
	 * Get the currently displayed year.
	 * @return An int that specifies the current year.
	 */
	int DatePicker::getYear() const
	{
		return this->getPropertyInt(MAW_DATE_PICKER_YEAR);
	}

	/**
	 * Set the current month of the picker.
	 * @param month An int that specifies the current month.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the property could be set.
	 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
	 * - #MAW_RES_ERROR otherwise.
	 */
	int DatePicker::setMonth(const int month)
	{
		return this->setPropertyInt(MAW_DATE_PICKER_MONTH, month);
	}

	/**
	 * Get the currently displayed month.
	 * @return An int that specifies the current month.
	 */
	int DatePicker::getMonth() const
	{
		return this->getPropertyInt(MAW_DATE_PICKER_MONTH);
	}

	/**
	 * Set the current day of month.
	 * @param day An int that specifies the current day of month.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the property could be set.
	 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
	 * - #MAW_RES_ERROR otherwise.
	 */
	int DatePicker::setDayOfMonth(const int dayOfMonth)
	{
		return this->setPropertyInt(MAW_DATE_PICKER_DAY_OF_MONTH, dayOfMonth);
	}

	/**
	 * Get the currently displayed day.
	 * @return An int that specifies the current day of month.
	 */
	int DatePicker::getDayOfMonth() const
	{
		return this->getPropertyInt(MAW_DATE_PICKER_DAY_OF_MONTH);
	}

	/**
	 * Set the minimum date of the picker.
	 * @param date A struct that specifies the minimum date.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the property could be set.
	 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
	 * - #MAW_RES_ERROR otherwise.
	 */
	int DatePicker::setMinDate( const struct Date minDate)
	{
		tm aDateStruct = this->getTmFromDateStruct(minDate);

		int dateMillisec = mktime(&aDateStruct);
		return this->setPropertyInt(MAW_DATE_PICKER_MIN_DATE, dateMillisec);
	}

	/**
	 * Get the minimum date of the picker.
	 * @return A Date struct that specifies the minimum date.
	 */
	Date DatePicker::getMinDate()
	{
		if ( MAW_RES_FEATURE_NOT_AVAILABLE ==
			this->getPropertyInt(MAW_DATE_PICKER_MIN_DATE))
		{
			Date defaultDate = Date(1,1,1990);
			return defaultDate;
		}

        int minDateMilliseconds =
            this->getPropertyInt(MAW_DATE_PICKER_MIN_DATE);
        tm *dateStruct = 0;
        split_time(minDateMilliseconds, dateStruct);

        Date date = this->getDateFromTmStruct(*dateStruct);
        return date;
	}

	/**
	 * Set the maximum date of the picker.
	 * @param date A struct that specifies the maximum date.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the property could be set.
	 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
	 * - #MAW_RES_ERROR otherwise.
	 */
	int DatePicker::setMaxDate( const struct Date maxDate)
	{
		tm aDateStruct = this->getTmFromDateStruct(maxDate);
		int dateMillisec = mktime(&aDateStruct);
		return this->setPropertyInt(MAW_DATE_PICKER_MAX_DATE, dateMillisec);
	}

	/**
	 * Get the maximum value of the picker.
	 * @return A Date struct that specifies the minimum date.
	 */
	struct Date DatePicker::getMaxDate()
	{
		if ( MAW_RES_FEATURE_NOT_AVAILABLE ==
			this->getPropertyInt(MAW_DATE_PICKER_MAX_DATE))
		{
			Date defaultDate = Date(12,31,2100);
			return defaultDate;
		}

	    int maxDateMilliseconds =
	        this->getPropertyInt(MAW_DATE_PICKER_MAX_DATE);
	    tm *dateStruct = 0;
	    split_time(maxDateMilliseconds, dateStruct);

	    Date date = this->getDateFromTmStruct(*dateStruct);
	    return date;
	}

    /**
     * Add an date picker event listener.
     * @param listener The listener that will receive date picker events.
     */
    void DatePicker::addDatePickerListener(DatePickerListener* listener)
    {
        addListenerToVector(mDatePickerListeners, listener);
    }

    /**
     * Remove the date picker listener.
     * @param listener The listener that receives date picker events.
     */
    void DatePicker::removeDatePickerListener(DatePickerListener* listener)
    {
        removeListenerFromVector(mDatePickerListeners, listener);
    }

    /**
     * This method is called when there is an event for this widget.
     * It passes on the event to all widget's listeners.
     * @param widgetEventData The data for the widget event.
     */
    void DatePicker::handleWidgetEvent(MAWidgetEventData* widgetEventData)
    {
        Widget::handleWidgetEvent(widgetEventData);

        if (MAW_EVENT_DATE_PICKER_VALUE_CHANGED == widgetEventData->eventType)
        {
            int day = widgetEventData->dayOfMonth;
            int month = widgetEventData->month;
            int year = widgetEventData->year;
            struct Date date = Date(day, month, year);

            for (int i = 0; i < mDatePickerListeners.size(); i++)
            {
                mDatePickerListeners[i]->datePickerValueChanged(
                    this,
                    date);
            }
        }
    }

    /**
     * Convert a tm struct to a Date struct.
     * @param tmStruct The given tm type struct.
     * @return The converted Date type struct.
     */
    Date DatePicker::getDateFromTmStruct(const tm tmStruct)
    {
        Date date;
        date.day = tmStruct.tm_mday;
        date.month = tmStruct.tm_mon + CONVERT_MONTHS_UNIT;
        date.year = tmStruct.tm_year + CONVERT_YEARS_UNIT;
        return date;
    }

    /**
     * Convert a Date struct to a tm struct.
     * @param date The given Date type struct.
     * @return The converted tm type struct.
     */
    tm DatePicker::getTmFromDateStruct(const Date date)
    {
        tm tmStruct;
        tmStruct.tm_mday = date.day;
        tmStruct.tm_mon = date.month - CONVERT_MONTHS_UNIT;
        tmStruct.tm_year = date.year - CONVERT_YEARS_UNIT;
        return tmStruct;
    }

} // namespace NativeUI
