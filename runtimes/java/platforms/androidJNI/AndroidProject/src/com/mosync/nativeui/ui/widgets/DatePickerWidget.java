package com.mosync.nativeui.ui.widgets;

import android.util.Log;
import android.widget.DatePicker;

import java.lang.reflect.*;
import java.util.Calendar;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.LongConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;
import com.mosync.nativeui.util.properties.FeatureNotAvailableException;

import static com.mosync.internal.android.MoSyncHelpers.SYSLOG;

/**
 * A Date Picker is a widget for selecting a date. The date can be selected by a
 * year, month, and day spinners. A compatibility check is done for newer
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

	// Static initializer to call initCompatibility, which does the
	// methods lookup.
	static {
		initCompatibility();
	}

	/*
	 * Do a one-time lookup and cache the results in Method objects.
	 */
	private static void initCompatibility() {
		Class myDatePickerClass = null;
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
			SYSLOG("initCompatibility mDatePicker_setMaxDate fail");
		}

		// Search for setMinDate into the DatePicker class.
		try {
			mDatePicker_setMinDate = myDatePickerClass.getMethod("setMinDate",
					new Class[] { Long.TYPE });
		} catch (NoSuchMethodException nsme) {
			SYSLOG("initCompatibility mDatePicker_setMinDate fail");
		}

		// Search for getMaxDate into the DatePicker class.
		try {
			mDatePicker_getMaxDate = myDatePickerClass.getMethod("getMaxDate",
					new Class[] { null });
		} catch (NoSuchMethodException nsme) {
			SYSLOG("initCompatibility mDatePicker_getMaxDate fail");
		}

		// Search for getMinDate into the DatePicker class.
		try {
			mDatePicker_getMinDate = myDatePickerClass.getMethod("getMinDate",
					new Class[] { null });
		} catch (NoSuchMethodException nsme) {
			SYSLOG("initCompatibility mDatePicker_getMinDate fail");
		}
	}

	/**   Wrap the new methods.   */

	private void setMaxDate(DatePicker datePicker, long maxValue)
			throws FeatureNotAvailableException
	{
		SYSLOG("In setMaxDate");
		try {
			mDatePicker_setMaxDate.invoke(datePicker, maxValue);
		} catch (InvocationTargetException ite) {
			SYSLOG("in setMaxDate InvocationTargetException catched");
			throw new FeatureNotAvailableException("setMaxDate");
		} catch (IllegalAccessException ie) {
			SYSLOG("in setMaxDate IllegalAccessException catched");
			throw new FeatureNotAvailableException("setMaxDate");
		} catch (IllegalArgumentException iae) {
			SYSLOG("in setMaxDate IllegalArgumentException catched");
			throw new FeatureNotAvailableException("setMaxDate");
		} catch (Exception ex){
			throw new FeatureNotAvailableException("setMaxDate");
		}
	}

	private void setMinDate(DatePicker datePicker, long minValue)
			throws FeatureNotAvailableException
	{
		SYSLOG("In setMinDate");
		try {
			mDatePicker_setMinDate.invoke(datePicker, minValue);
		} catch (InvocationTargetException ite) {
			throw new FeatureNotAvailableException("setMinDate");
		} catch (IllegalAccessException ie) {
			System.err.println("unexpected " + ie);
			throw new FeatureNotAvailableException("setMinDate");
		} catch (IllegalArgumentException iae) {
			throw new FeatureNotAvailableException("setMinDate");
		} catch (Exception ex){
			SYSLOG("crashed with Exception");
			throw new FeatureNotAvailableException("setMinDate");
		}
	}

	private long getMaxDate(DatePicker datePicker)
			throws FeatureNotAvailableException
	{
		SYSLOG("in getMaxDate");
		long result = 0;
		try {
			result = (Long) mDatePicker_getMaxDate.invoke(datePicker,
					(Object[]) null);
			return result;
		} catch (InvocationTargetException ite) {
			SYSLOG("in getMaxDate InvocationTargetException catched");
			throw new FeatureNotAvailableException("getMaxDate");
		} catch (IllegalAccessException ie) {
			SYSLOG("in getMaxDate IllegalAccessException catched");
			throw new FeatureNotAvailableException("getMaxDate");
		} catch (IllegalArgumentException iae) {
			SYSLOG("in getMaxDate IllegalArgumentException catched");
			throw new FeatureNotAvailableException("getMaxDate");
		} catch (Exception ex) {
			SYSLOG("in getMaxDate Exception catched");
			throw new FeatureNotAvailableException("getMaxDate");
		}
	}

	private long getMinDate(DatePicker datePicker)
			throws FeatureNotAvailableException
	{
		SYSLOG("in getMinDate");
		long result = 0;
		try {
			result = (Long) mDatePicker_getMaxDate.invoke(datePicker,
					(Object[]) null);
			return result;
		} catch (InvocationTargetException ite) {
			SYSLOG("in getMinDate InvocationTargetException catched");
			throw new FeatureNotAvailableException("getMinDate");
		} catch (IllegalAccessException ie) {
			SYSLOG("in getMinDate IllegalAccessException catched");
			throw new FeatureNotAvailableException("getMinDate");
		} catch (IllegalArgumentException iae) {
			SYSLOG("in getMinDate IllegalArgumentException catched");
			throw new FeatureNotAvailableException("getMinDate");
		} catch (Exception ex) {
			SYSLOG("in getMinDate Exception catched");
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
			// Will throw FeatureNotAvailableException on older devices.
			return Long.toString( getMaxDate(datePicker) );
		}
		else if (property.equals(IX_WIDGET.MAW_DATE_PICKER_MIN_DATE))
		{
			// Will throw FeatureNotAvailableException on older devices.
			return Long.toString( getMinDate(datePicker) );
		}
		else
		{
		return super.getProperty(property);
		}
	}

}
