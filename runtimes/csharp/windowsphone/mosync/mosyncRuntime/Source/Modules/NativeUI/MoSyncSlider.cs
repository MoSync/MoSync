using Microsoft.Phone.Controls;
using System.Windows.Controls;
using System.Windows;
using System.Windows.Navigation;
using System;
using System.Text.RegularExpressions;
using System.Reflection;

namespace MoSync
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

        public Slider()
        {
            mSlider = new System.Windows.Controls.Slider();

            mSlider.Maximum = DEFAULT_MAX_VALUE;
            mSlider.Minimum = DEFAULT_MIN_VALUE;

            mMaxValue = DEFAULT_MAX_VALUE;
            mMinValue = DEFAULT_MIN_VALUE;
            mProgressValue = DEFAULT_MIN_VALUE;

            mView = mSlider;
        }

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

        [MoSyncWidgetProperty(MoSync.Constants.MAW_SLIDER_INCREASE_VALUE)]
        public string IncreaseValue
        {
            set
            {
                int val = Int32.Parse(value);
                Value = (mProgressValue + val).ToString();
            }
        }

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
