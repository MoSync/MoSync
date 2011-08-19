package com.mosync.nativeui.ui.widgets;

import android.widget.DatePicker;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;
import com.mosync.nativeui.util.properties.FeatureNotAvailableException;


/**
 * A Date Picker is a widget for selecting a date.
 * The date can be selected by a year, month, and day spinners.
 * A compatibility check is done for newer
 * methods of the Date Picker.
 *
 * @author emma tresanszki
 */
public class DatePickerWidget extends Widget {

	/**
	 * Constructor.
	 *
	 * @param handle
	 *            Integer handle corresponding to this instance.
	 * @param datePicker
	 *            A date picker wrapped by this widget.
	 */
	public DatePickerWidget(int handle, DatePicker datePicker)
	{
		super(handle, datePicker);
	}

	/**
	 * @see Widget.setProperty.
	 */
	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException,
			InvalidPropertyValueException,
			FeatureNotAvailableException
	{
		if (super.setProperty(property, value))
		{
			return true;
		}

		DatePicker datePicker = (DatePicker) getView();

		if (property.equals(IX_WIDGET.MAW_DATE_PICKER_YEAR))
		{
			try{
				// Make sure we catch invalid dates.
				datePicker.updateDate(
						IntConverter.convert(value),
						datePicker.getMonth(),
						datePicker.getDayOfMonth());
			} catch(RuntimeException ex)
			{
				throw new InvalidPropertyValueException(property, value);
			}
		}
		else if (property.equals(IX_WIDGET.MAW_DATE_PICKER_MONTH))
		{
			// Make sure we catch invalid dates.
			try{
			datePicker.updateDate(
					datePicker.getYear(),
					// Month starts with 0.
					IntConverter.convert(value)-1,
					datePicker.getDayOfMonth());
			} catch(RuntimeException ex)
			{
				throw new InvalidPropertyValueException(property, value);
			}

		}
		else if (property.equals(IX_WIDGET.MAW_DATE_PICKER_DAY_OF_MONTH))
		{
			// Make sure we catch invalid dates.
			try{
				datePicker.updateDate(
						datePicker.getYear(),
						datePicker.getMonth(),
						IntConverter.convert(value));
			} catch(RuntimeException ex)
			{
				throw new InvalidPropertyValueException(property, value);
			}
		}
		else
		{
			return false;
		}
		return true;
	}

	/**
	 * @see Widget.getProperty.
	 */
	@Override
	public String getProperty(String property)
			throws FeatureNotAvailableException
	{
		DatePicker datePicker = (DatePicker) getView();

		if (property.equals(IX_WIDGET.MAW_DATE_PICKER_YEAR))
		{
			return Integer.toString(datePicker.getYear());
		}
		else if (property.equals(IX_WIDGET.MAW_DATE_PICKER_MONTH))
		{
			return Integer.toString(datePicker.getMonth()+1);
		}
		else if (property.equals(IX_WIDGET.MAW_DATE_PICKER_DAY_OF_MONTH))
		{
			return Integer.toString(datePicker.getDayOfMonth());
		}
		else
		{
		return super.getProperty(property);
		}
	}

}
