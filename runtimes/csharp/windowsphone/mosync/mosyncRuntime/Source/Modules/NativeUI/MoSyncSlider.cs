using Microsoft.Phone.Controls;
using System.Windows.Controls;
using System.Windows;
using System.Windows.Navigation;
using System;
using System.Text.RegularExpressions;
using System.Reflection;

namespace MoSync
{
    namespace NativeUI
    {
        //The slider class
        public class Slider : WidgetBaseWindowsPhone
        {
            protected System.Windows.Controls.Slider mSlider;
            protected int mMaxValue;
            protected int mMinValue;
            protected int mProgressValue;

            public static int DEFAULT_MAX_VALUE = 100;
            public static int DEFAULT_MIN_VALUE = 0;

            //the constructor
            public Slider()
            {
                mSlider = new System.Windows.Controls.Slider();

                mSlider.Maximum = DEFAULT_MAX_VALUE;
                mSlider.Minimum = DEFAULT_MIN_VALUE;

                mMaxValue = DEFAULT_MAX_VALUE;
                mMinValue = DEFAULT_MIN_VALUE;
                mProgressValue = DEFAULT_MIN_VALUE;

                mView = mSlider;

                //the event handler
                mSlider.ValueChanged += new RoutedPropertyChangedEventHandler<double>(
                    delegate(Object from, RoutedPropertyChangedEventArgs<double> arg)
                    {
                        Memory eventData = new Memory(12);

                        const int MAWidgetEventData_eventType = 0;
                        const int MAWidgetEventData_sliderValue = 4;
                        const int MAWidgetEventData_widgetHandle = 8;

                        mProgressValue = ((Int32)arg.NewValue);

                        eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_SLIDER_VALUE_CHANGED);
                        eventData.WriteInt32(MAWidgetEventData_sliderValue, mProgressValue);
                        eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                        mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                    });
            }

            //MAW_SLIDER_MAX implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_SLIDER_MAX)]
            public string Max
            {
                set
                {
                    int maxVal = Int32.Parse(value);
                    if (maxVal >= 0)
                    {
                        mMaxValue = maxVal;
                        if (maxVal < mProgressValue)
                        {
                            mProgressValue = maxVal;
                            Value = maxVal.ToString();
                        }
                        mSlider.Maximum = mMaxValue;
                    }
                    else
                    {
                        mMaxValue = 0;
                        mSlider.Maximum = mMaxValue;
                    }
                }
                get
                {
                    return mMaxValue.ToString();
                }
            }

            //MAW_SLIDER_VALUE implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_SLIDER_VALUE)]
            public string Value
            {
                set
                {
                    int val = Int32.Parse(value);

                    if (val <= mMaxValue && val >= mMinValue)
                    {
                        mProgressValue = val;
                        mSlider.Value = mProgressValue;
                    }
                    else if (val > mMaxValue)
                    {
                        mSlider.Value = mMaxValue;
                        mProgressValue = mMaxValue;
                    }
                    else if (val < mMinValue)
                    {
                        mSlider.Value = mMinValue;
                        mProgressValue = mMinValue;
                    }
                }
                get
                {
                    return mProgressValue.ToString();
                }
            }

            //MAW_SLIDER_INCREASE_VALUE implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_SLIDER_INCREASE_VALUE)]
            public string IncreaseValue
            {
                set
                {
                    int val = Int32.Parse(value);
                    Value = (mProgressValue + val).ToString();
                }
            }

            //MAW_SLIDER_DECREASE_VALUE implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_SLIDER_DECREASE_VALUE)]
            public string DecreaseValue
            {
                set
                {
                    int val = Int32.Parse(value);
                    Value = (mProgressValue - val).ToString();
                }
            }
        }
    }
}
