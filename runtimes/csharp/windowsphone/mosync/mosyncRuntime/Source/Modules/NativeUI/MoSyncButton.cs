using Microsoft.Phone.Controls;
using System.Windows.Controls;
using System.Windows;
using System.Windows.Navigation;
using System;
using System.Text.RegularExpressions;
using System.Reflection;

namespace MoSync
{
    // The button class
    public class Button : WidgetBaseWindowsPhone
    {
        //The text content of the button
        protected String mText;

        //the vertical alignment of the content
        protected VerticalAlignment mTextVerticalAlignment;

        //the horizontal alignment of the content
        protected HorizontalAlignment mTextHorizontalAlignment;

        //the button controll
        protected System.Windows.Controls.Button mButton;

        //the button constructor
        public Button()
        {
            //initializing the button controll
            mButton = new System.Windows.Controls.Button();
            //set the view of the current widget as the previously instantiated button controll
            View = mButton;
            mButton.HorizontalAlignment = HorizontalAlignment.Left;
            mButton.VerticalAlignment = VerticalAlignment.Top;

            fillSpaceVerticalyEnabled = false;
            fillSpaceHorizontalyEnabled = false;

            mButton.Click += new RoutedEventHandler(
                delegate(Object from, RoutedEventArgs evt)
                {
                    Memory eventData = new Memory(8);

                    const int MAWidgetEventData_eventType = 0;
                    const int MAWidgetEventData_widgetHandle = 4;

                    eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_POINTER_PRESSED);
                    eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                    mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                });
        }

        //MAW_BUTTON_TEXT property implementation
        [MoSyncWidgetProperty(MoSync.Constants.MAW_BUTTON_TEXT)]
        public String Text
        {
            set
            {
                mText = value;
                mButton.Content = mText;
            }
            get
            {
                return mText;
            }
        }

        //MAW_BUTTON_TEXT_HORIZONTAL_ALIGNMENT property implementation
        [MoSyncWidgetProperty(MoSync.Constants.MAW_BUTTON_TEXT_HORIZONTAL_ALIGNMENT)]
        public String TextHorizontalAlignment
        {
            set
            {
                //doing the proper behaviour for the required value
                String a = value.ToString();
                if (value.ToString().Equals(MoSync.Constants.MAW_ALIGNMENT_LEFT))
                {
                    mTextHorizontalAlignment = HorizontalAlignment.Left;
                    mButton.HorizontalContentAlignment = mTextHorizontalAlignment;
                }
                else
                {
                    if (value.ToString().Equals(MoSync.Constants.MAW_ALIGNMENT_RIGHT))
                    {
                        mTextHorizontalAlignment = HorizontalAlignment.Right;
                        mButton.HorizontalContentAlignment = mTextHorizontalAlignment;
                    }
                    else
                    {
                        if (value.ToString().Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER))
                        {
                            mTextHorizontalAlignment = HorizontalAlignment.Center;
                            mButton.HorizontalContentAlignment = mTextHorizontalAlignment;
                        }
                    }
                }
            }
        }

        //MAW_BUTTON_TEXT_VERTICAL_ALIGNMENT property implementation
        [MoSyncWidgetProperty(MoSync.Constants.MAW_BUTTON_TEXT_VERTICAL_ALIGNMENT)]
        public String TextVerticalAlignment
        {
            set
            {
                if (value.ToString().Equals(MoSync.Constants.MAW_ALIGNMENT_TOP))
                {
                    mTextVerticalAlignment = VerticalAlignment.Top;
                    mButton.VerticalContentAlignment = mTextVerticalAlignment;
                }
                else
                {
                    if (value.ToString().Equals(MoSync.Constants.MAW_ALIGNMENT_BOTTOM))
                    {
                        mTextVerticalAlignment = VerticalAlignment.Bottom;
                        mButton.VerticalContentAlignment = mTextVerticalAlignment;
                    }
                    else
                    {
                        if (value.ToString().Equals(MoSync.Constants.MAW_ALIGNMENT_CENTER))
                        {
                            mTextVerticalAlignment = VerticalAlignment.Center;
                            mButton.VerticalContentAlignment = mTextVerticalAlignment;
                        }
                    }
                }
            }
        }

        //MAW_BUTTON_FONT_COLOR property implementation
        [MoSyncWidgetProperty(MoSync.Constants.MAW_BUTTON_FONT_COLOR)]
        public String FontColor
        {
            set
            {
                System.Windows.Media.SolidColorBrush brush;
                MoSync.Util.convertStringToColor(value, out brush);
                mButton.Foreground = brush;
            }
        }

        //MAW_BUTTON_FONT_SIZE property implementation
        [MoSyncWidgetProperty(MoSync.Constants.MAW_BUTTON_FONT_SIZE)]
        public String FontSize
        {
            set
            {
                double size = double.Parse(value);
                mButton.FontSize = size;
            }
        }
    }
}
