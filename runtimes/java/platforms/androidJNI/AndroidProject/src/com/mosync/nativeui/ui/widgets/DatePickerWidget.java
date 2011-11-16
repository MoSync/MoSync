package com.mosync.nativeui.ui.widgets;

import java.lang.reflect.Method;

import android.util.Log;
import android.widget.DatePicker;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.LongConverter;
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

	// Initialize the methods.
	public static Method mDatePicker_getMaxDate = null;
	public static Method mDatePicker_getMinDate = null;
	public static Method mDatePicker_setMaxDate = null;
	public static Method mDatePicker_setMinDate = null;

	// Static initializer , which does the methods lookup.
	static {
		initCompatibility();
	}

	/*
	 * Do a one-time lookup and cache the results in Method objects.
	 */
    private static void initCompatibility() {
		Class<?> myDatePickerClass = null;
        try {
            myDatePickerClass = Class.forName("android.widget.DatePicker");
         }
         catch (Throwable e) {
            System.err.println(e);
         }

		// Search for setMaxDate into the DatePicker class.
		try {
			mDatePicker_setMaxDate = myDatePickerClass.getMethod(
					"setMaxDate", new Class[] { Long.TYPE });
			/* success, this is a newer device */
		} catch (NoSuchMethodException nsme) {
			/* failure, must be older device */
			Log.i("MoSync", "initCompatibility mDatePicker_setMaxDate fail");
		}

		// Search for setMinDate into the DatePicker class.
		try {
			mDatePicker_setMinDate = myDatePickerClass.getMethod("setMinDate",
					new Class[] { Long.TYPE });
		} catch (NoSuchMethodException nsme) {
			Log.i("MoSync", "initCompatibility mDatePicker_setMinDate fail");
		}

		// Search for getMaxDate into the DatePicker class.
		try {
			mDatePicker_getMaxDate = myDatePickerClass.getMethod("getMaxDate",
					new Class[] { null });
		} catch (NoSuchMethodException nsme) {
			Log.i("MoSync", "initCompatibility mDatePicker_getMaxDate fail");
		}

		// Search for getMinDate into the DatePicker class.
		try {
			mDatePicker_getMinDate = myDatePickerClass.getMethod("getMinDate",
					new Class[] { null });
		} catch (NoSuchMethodException nsme) {
			Log.i("MoSync", "initCompatibility mDatePicker_getMinDate fail");
		}
	}

	/**   Wrap the new methods.   */

	private void setMaxDate(DatePicker datePicker, long maxValue)
			throws FeatureNotAvailableException
	{
		try {
			mDatePicker_setMaxDate.invoke(datePicker, maxValue);
		} catch (Exception ex){
			throw new FeatureNotAvailableException("setMaxDate");
		}
	}

	private void setMinDate(DatePicker datePicker, long minValue)
			throws FeatureNotAvailableException
	{
		try {
			mDatePicker_setMinDate.invoke(datePicker, minValue);
		} catch (Exception ex){
			throw new FeatureNotAvailableException("setMinDate");
		}
	}

	private long getMaxDate(DatePicker datePicker)
			throws FeatureNotAvailableException
	{
		long result = 0;
		try {
			result = (Long) mDatePicker_getMaxDate.invoke(datePicker,
					(Object[]) null);
			return result;
		} catch (Exception ex) {
			throw new FeatureNotAvailableException("getMaxDate");
		}
	}

	private long getMinDate(DatePicker datePicker)
			throws FeatureNotAvailableException
	{
		long result = 0;
		try {
			result = (Long) mDatePicker_getMaxDate.invoke(datePicker,
					(Object[]) null);
			return result;
		} catch (Exception ex) {
			throw new FeatureNotAvailableException("getMinDate");
		}
	}

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
		else if (property.equals(IX_WIDGET.MAW_DATE_PICKER_MAX_DATE))
		{
			// It will throw the FeatureNotAvailableException on older devices.
			setMaxDate(datePicker, LongConverter.convert(value));
		}
		else if (property.equals(IX_WIDGET.MAW_DATE_PICKER_MIN_DATE) )
		{
			// It will throw further the FeatureNotAvailableException on older devices.
			setMinDate(datePicker, LongConverter.convert(value));
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
		// Call it in order to trigger onFocusChanged when you change the date
		// using the keyboard and you don't focus out from the field.
		datePicker.clearFocus();

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
		else if (property.equals(IX_WIDGET.MAW_DATE_PICKER_MAX_DATE))
		{
			// Throws FeatureNotAvailableException on older devices.
			return Long.toString( getMaxDate(datePicker) );
		}
		else if (property.equals(IX_WIDGET.MAW_DATE_PICKER_MIN_DATE))
		{
			// Throws FeatureNotAvailableException on older devices.
			return Long.toString( getMinDate(datePicker) );
		}
		else
		{
		return super.getProperty(property);
		}
	}

}
