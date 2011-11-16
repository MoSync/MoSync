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
        public class Screen : WidgetBaseWindowsPhone, IScreen
        {
            protected PhoneApplicationPage mPage;
            public Screen()
            {
                mPage = new PhoneApplicationPage();
                mView = mPage;
            }

            public override void AddChild(IWidget child)
            {
                base.AddChild(child);
                WidgetBaseWindowsPhone w = (WidgetBaseWindowsPhone)child;
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    mPage.Content = w.View;
                    ((System.Windows.FrameworkElement)w.View).Width = ScreenSize.SCREEN_WIDTH;
                    ((System.Windows.FrameworkElement)w.View).Height = ScreenSize.SCREEN_HEIGHT;
                });
            }

            public void Show()
            {
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    PhoneApplicationFrame frame = (PhoneApplicationFrame)Application.Current.RootVisual;
                    frame.Content = mPage;
                });
            }
        }
    }
}
