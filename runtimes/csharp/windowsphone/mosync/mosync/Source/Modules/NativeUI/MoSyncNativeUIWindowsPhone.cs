using Microsoft.Phone.Controls;
using System.Windows.Controls;
using System.Windows;
using System.Windows.Navigation;
using System;

namespace MoSync
{
    public class WidgetBaseWindowsPhone : WidgetBase
    {
        protected UIElement mView;

        public WidgetBaseWindowsPhone()
            : base()
        {
        }
    };

    public class Screen : WidgetBaseWindowsPhone, IScreen
    {
        public Screen()
        {
            mView = new PhoneApplicationPage();
        }

        public void Show()
        {
            PhoneApplicationPage page = (PhoneApplicationPage)mView;
            page.NavigationService.Navigate(new Uri(page.Name, UriKind.Relative));
        }
    }

    public class NativeUIWindowsPhone : NativeUI
    {
        private PhoneApplicationFrame mFrame;

        public NativeUIWindowsPhone() : base()
        {
            // this should always be a PhoneApplicationFrame.
            mFrame = (PhoneApplicationFrame)Application.Current.RootVisual;
        }
    }
}