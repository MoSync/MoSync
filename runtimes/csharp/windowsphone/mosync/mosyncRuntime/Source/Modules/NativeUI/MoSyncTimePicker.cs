using System;

namespace MoSync
{
    namespace NativeUI
    {
        public class TimePicker : WidgetBaseWindowsPhone
        {

            private Microsoft.Phone.Controls.TimePicker mTimePicker;

            public TimePicker()
            {
                mTimePicker = new Microsoft.Phone.Controls.TimePicker();

                CurrentHour = 0;
                CurrentMinute = 0;

                View = mTimePicker;

                mTimePicker.ValueChanged += new EventHandler<Microsoft.Phone.Controls.DateTimeValueChangedEventArgs>(
                    delegate(object sender, Microsoft.Phone.Controls.DateTimeValueChangedEventArgs args)
                    {
                        Memory eventData = new Memory(16);

                        const int MAWidgetEventData_eventType = 0;
                        const int MAWidgetEventData_widgetHandle = 4;
                        const int MAWidgetEventDate_value_hours = 8;
                        const int MAWidgetEventDate_value_minutes = 12;
                        eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_TIME_PICKER_VALUE_CHANGED);
                        eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                        eventData.WriteInt32(MAWidgetEventDate_value_hours, mTimePicker.Value.Value.Hour);
                        eventData.WriteInt32(MAWidgetEventDate_value_minutes, mTimePicker.Value.Value.Minute);

                        mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                    });
            }

            [MoSync.MoSyncWidgetProperty(MoSync.Constants.MAW_TIME_PICKER_CURRENT_HOUR)]
            public int CurrentHour
            {
                get
                {
                    return mTimePicker.Value.Value.Hour;
                }
                set
                {
                    if (24 > value && -1 < value)
                    {
                        DateTime val = mTimePicker.Value.Value;
                        val = val.AddHours(-1 * (val.Hour - value));
                    }
                    else throw new InvalidPropertyValueException();
                }
            }

            [MoSync.MoSyncWidgetProperty(MoSync.Constants.MAW_TIME_PICKER_CURRENT_MINUTE)]
            public int CurrentMinute
            {
                get
                {
                    return mTimePicker.Value.Value.Minute;
                }
                set
                {
                    if (60 > value && -1 < value)
                    {
                        DateTime val = mTimePicker.Value.Value;
                        val = val.AddMinutes(-1 * (val.Minute - value));
                    }
                    else throw new InvalidPropertyValueException();
                }
            }

            #region Property validation methods

            /**
             * Validates a property based on the property name and property value.
             * @param propertyName The name of the property to be checked.
             * @param propertyValue The value of the property to be checked.
             * @returns true if the property is valid, false otherwise.
             */
            public new static bool ValidateProperty(string propertyName, string propertyValue)
            {
                bool isPropertyValid = WidgetBaseWindowsPhone.ValidateProperty(propertyName, propertyValue);

                if (propertyName.Equals("currentHour") || propertyName.Equals("currentMinute"))
                {
                    int val;
                    if (!int.TryParse(propertyValue, out val))
                    {
                        isPropertyValid = false;
                    }

                    if (propertyName.Equals("currentHour"))
                    {
                        if (val >= 24 || val < 0)
                        {
                            isPropertyValid = false;
                        }
                    }
                    else if (propertyName.Equals("currentMinute"))
                    {
                        if (val >= 60 || val < 0)
                        {
                            isPropertyValid = false;
                        }
                    }
                }

                return isPropertyValid;
            }

            #endregion
        } // end of TimePicker class
    } // end of NativeUI namespace
} // end of MoSync namespace
