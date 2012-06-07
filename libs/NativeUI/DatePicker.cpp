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
		Widget(MAW_DATE_PICKER),
		mMaxDate(NULL),
		mMinDate(NULL),
		mDisplayedDate(NULL)
	{
	}

	/**
	 * Destructor.
	 */
	DatePicker::~DatePicker()
	{
        mDatePickerListeners.clear();

        if(NULL != mMinDate)
			delete mMinDate;
        if(NULL != mMaxDate)
			delete mMaxDate;
        if(NULL != mDisplayedDate)
			delete mDisplayedDate;
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
		//Saving the date, in case of error this will be restored.
		int year = getYear();
		int month = getMonth();
		int day = getDayOfMonth();

		//Set the day on 1. Used to avoid errors when setting certain dates.
		int errorCode = setDayOfMonth(1);

		if(MAW_RES_OK != errorCode)
		{
			return errorCode;
		}
		else
		{
			errorCode = setYear(date.year);
			if(MAW_RES_OK != errorCode)
			{
				//reset the day.
				setDayOfMonth(day);
				return errorCode;
			}
			else
			{
				errorCode = setMonth(date.month);
				if(MAW_RES_OK != errorCode)
				{
					//reset the year and the day.
					setYear(year);
					setDayOfMonth(day);
					return errorCode;
				}
				else
				{
					errorCode = setDayOfMonth(date.day);
					if(MAW_RES_OK != errorCode)
					{
						//reset year, month and day
						setYear(year);
						setMonth(month);
						setDayOfMonth(day);

						return errorCode;
					}
				}
			}
		}
		return errorCode;
	}

	/**
	 * Get the currently displayed date.
	 * @return A Date struct that specifies the displayed date.
	 */
	Date DatePicker::getDate()
	{
		if(NULL == mDisplayedDate)
			mDisplayedDate = new Date();

		mDisplayedDate->year = this->getPropertyInt(MAW_DATE_PICKER_YEAR);
		mDisplayedDate->month = this->getPropertyInt(MAW_DATE_PICKER_MONTH);
		mDisplayedDate->day = this->getPropertyInt(MAW_DATE_PICKER_DAY_OF_MONTH);

		return *mDisplayedDate;
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
	int DatePicker::getYear()
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
	int DatePicker::getMonth()
	{
		return this->getPropertyInt(MAW_DATE_PICKER_MONTH);
	}

	/**
	 * Set the current day of month.
	 * @param dayOfMonth An int that specifies the current day of month.
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
	int DatePicker::getDayOfMonth()
	{
		return this->getPropertyInt(MAW_DATE_PICKER_DAY_OF_MONTH);
	}

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
	int DatePicker::setMinDate( const struct Date minDate)
	{
		//Saving the date, in case of error this will be restored.
		int year = this->getPropertyInt(MAW_DATE_PICKER_MIN_DATE_YEAR);
		int month = this->getPropertyInt(MAW_DATE_PICKER_MIN_DATE_MONTH);

		int errorCode = this->setPropertyInt(MAW_DATE_PICKER_MIN_DATE_YEAR, minDate.year);

		if(MAW_RES_OK != errorCode)
				return errorCode;

		errorCode = this->setPropertyInt(MAW_DATE_PICKER_MIN_DATE_MONTH, minDate.month);
		if(MAW_RES_OK != errorCode)
		{
			//reset the year
			this->setPropertyInt(MAW_DATE_PICKER_MIN_DATE_YEAR, year);
			return errorCode;
		}

		errorCode = this->setPropertyInt(MAW_DATE_PICKER_MIN_DATE_DAY, minDate.day);
		if(MAW_RES_OK != errorCode)
		{
			//reset the year
			this->setPropertyInt(MAW_DATE_PICKER_MIN_DATE_YEAR, year);
			//reset the month
			this->setPropertyInt(MAW_DATE_PICKER_MIN_DATE_MONTH, month);
			return errorCode;
		}

		return errorCode;
	}

	/**
	 * Get the minimum date of the picker.
	 * @return A Date struct that specifies the minimum date.
	 */
	Date DatePicker::getMinDate()
	{
		int resCodeY, resCodeM, resCodeD;

		if(NULL != mMinDate)
			delete mMinDate;

		mMinDate->year = this->getPropertyInt(MAW_DATE_PICKER_MIN_DATE_YEAR,resCodeY);
		mMinDate->month = this->getPropertyInt(MAW_DATE_PICKER_MIN_DATE_MONTH,resCodeM);
		mMinDate->day = this->getPropertyInt(MAW_DATE_PICKER_MIN_DATE_DAY,resCodeD);

		if ( MAW_RES_FEATURE_NOT_AVAILABLE == resCodeY ||
			 MAW_RES_FEATURE_NOT_AVAILABLE == resCodeM ||
			 MAW_RES_FEATURE_NOT_AVAILABLE == resCodeD )
		{
			mMinDate = new Date(1,1,1900);
			return *mMinDate;
		}

		return *mMinDate;
	}

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
	int DatePicker::setMaxDate( const struct Date maxDate)
	{
		//Saving the date, in case of error this will be restored.
		int year = this->getPropertyInt(MAW_DATE_PICKER_MAX_DATE_YEAR);
		int month = this->getPropertyInt(MAW_DATE_PICKER_MAX_DATE_MONTH);

		int errorCode = this->setPropertyInt(MAW_DATE_PICKER_MAX_DATE_YEAR, maxDate.year);

		if(MAW_RES_OK != errorCode)
				return errorCode;

		errorCode = this->setPropertyInt(MAW_DATE_PICKER_MAX_DATE_MONTH, maxDate.month);
		if(MAW_RES_OK != errorCode)
		{
			//reset the year
			this->setPropertyInt(MAW_DATE_PICKER_MAX_DATE_YEAR, year);
			return errorCode;
		}

		errorCode = this->setPropertyInt(MAW_DATE_PICKER_MAX_DATE_DAY, maxDate.day);
		if(MAW_RES_OK != errorCode)
		{
			//reset the year
			this->setPropertyInt(MAW_DATE_PICKER_MAX_DATE_YEAR, year);
			//reset the month
			this->setPropertyInt(MAW_DATE_PICKER_MAX_DATE_MONTH, month);
			return errorCode;
		}

		return errorCode;
	}

	/**
	 * Get the maximum value of the picker.
	 * @return A Date struct that specifies the minimum date.
	 */
	struct Date DatePicker::getMaxDate()
	{
		int resCodeY, resCodeM, resCodeD;

		if(NULL != mMaxDate)
			delete mMaxDate;

		mMaxDate->year = this->getPropertyInt(MAW_DATE_PICKER_MAX_DATE_YEAR,resCodeY);
		mMaxDate->month = this->getPropertyInt(MAW_DATE_PICKER_MAX_DATE_MONTH,resCodeM);
		mMaxDate->day = this->getPropertyInt(MAW_DATE_PICKER_MAX_DATE_DAY,resCodeD);

		if ( MAW_RES_FEATURE_NOT_AVAILABLE == resCodeY ||
			 MAW_RES_FEATURE_NOT_AVAILABLE == resCodeM ||
			 MAW_RES_FEATURE_NOT_AVAILABLE == resCodeD )
		{
			mMaxDate = new Date(1,1,2100);
			return *mMaxDate;
		}

		return *mMaxDate;
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
} // namespace NativeUI
