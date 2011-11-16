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
        public class CheckBox : WidgetBaseWindowsPhone
        {
            protected System.Windows.Controls.CheckBox mCheckBox;

            public CheckBox()
            {
                mCheckBox = new System.Windows.Controls.CheckBox();

                mView = mCheckBox;

                mCheckBox.Click += new RoutedEventHandler(
                   delegate(Object from, RoutedEventArgs evt)
                   {
                       Memory eventData = new Memory(8);

                       const int MAWidgetEventData_eventType = 0;
                       const int MAWidgetEventData_widgetHandle = 4;

                       eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_CLICKED);
                       eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                       mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                   });
            }

            public virtual void setState(bool state)
            {
                mCheckBox.IsChecked = state;
            }

            [MoSyncWidgetProperty(MoSync.Constants.MAW_CHECK_BOX_CHECKED)]
            public String Checked
            {
                get
                {
                    return mCheckBox.IsChecked.ToString();
                }

                set
                {
                    bool val = bool.Parse(value);
                    mCheckBox.IsChecked = val;
                }
            }
        }
    }
}
