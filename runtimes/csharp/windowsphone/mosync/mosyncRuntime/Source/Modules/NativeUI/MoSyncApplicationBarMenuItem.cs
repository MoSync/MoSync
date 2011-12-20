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
        public class ApplicationBarMenuItem : WidgetBaseWindowsPhone
        {
            protected String mText;

            public ApplicationBarMenuItem()
            {
                mView = null;
            }

            [MoSyncWidgetProperty(MoSync.Constants.MAW_APPLICATION_BAR_MENU_ITEM_TEXT)]
            public String Text
            {
                set
                {
                    mText = value;
                }
                get
                {
                    return mText;
                }
            }
        }
    }
}
