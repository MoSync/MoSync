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

        public UIElement View
        {
            get { return mView; }
            set { mView = value; }
        }

        [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_LEFT)]
        public double Left
        {
            get { return (double)mView.GetValue(Canvas.LeftProperty); }
            set 
            { 
                //mView.SetValue(Canvas.LeftProperty, value); 
            }
        }

        [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_TOP)]
        public double Top
        {
            get { return (double)mView.GetValue(Canvas.TopProperty); }
            set 
            { 
                //mView.SetValue(Canvas.TopProperty, value); 
            }
        }

        [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_WIDTH)]
        public double Width
        {
            get { return (double)mView.GetValue(Canvas.WidthProperty); }
            set 
            {
                //mView.SetValue(Canvas.WidthProperty, value); 
            }
        }

        [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_HEIGHT)]
        public double Height
        {
            get { return (double)mView.GetValue(Canvas.HeightProperty); }
            set {
                //mView.SetValue(Canvas.HeightProperty, value);
            }
        }

        public WidgetBaseWindowsPhone()
            : base()
        {
        }
    };

    public class WebView : WidgetBaseWindowsPhone
    {
        [MoSyncWidgetProperty(MoSync.Constants.MAW_WEB_VIEW_URL)]
        public String url
        {
            set
            {
                WebBrowser webBrowser = (WebBrowser)mView;
                webBrowser.Navigate(new Uri(value, UriKind.Relative));
            }
        }

        public WebView()
        {
            mView = new Microsoft.Phone.Controls.WebBrowser();
        }

    }

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
            });
        }

        public void Show()
        {
            //mPage.NavigationService.Navigate(new Uri(mPage.Name, UriKind.Relative));
            //System.Windows.Markup.XamlReader(
            MoSync.Util.RunActionOnMainThreadSync(() =>
            {
                PhoneApplicationFrame frame = (PhoneApplicationFrame)Application.Current.RootVisual;
                frame.Content = mPage;
            });
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