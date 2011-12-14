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
        public class ApplicationBarButton : WidgetBaseWindowsPhone
        {
            protected System.Uri mIconUri;
            protected System.String mText;

            public ApplicationBarButton()
            {
                mView = null;
            }

            public String IconUri
            {
                set
                {
                    mIconUri = new Uri(value, UriKind.Relative);
                }
                get
                {
                    return mIconUri.ToString();
                }
            }

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
