using System;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;

namespace MoSync
{
    namespace NativeUI
    {
        public class NumberPickerValueChangedEventArgs : EventArgs
        {
            public NumberPickerValueChangedEventArgs(int? oldValue, int? newValue)
            {
                OldValue = oldValue;
                NewValue = newValue;
            }

            public int? OldValue { get; private set; }
            public int? NewValue { get; private set; }
        }
    }
}
