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
        public class WebView : WidgetBaseWindowsPhone
        {
            protected WebBrowser mWebBrowser;
            protected String mHardHook = "";
            protected String mSoftHook = "";
            protected bool mHorizontalScrollBarEnabled = false;
            protected bool mVerticalScrollBarEnabled = false;
            protected bool mEnabledZoom = true;
            protected String mNavigatedFrom = "";
            protected String mNavigatingTo = "";
            
            //this is used when loading relative paths
            protected String mBaseURL = "";

            [MoSyncWidgetProperty(MoSync.Constants.MAW_WEB_VIEW_URL)]
            public String Url
            {
                set
                {
                    if (value.StartsWith("javascript:"))
                    {
                        int startIndex = "javascript:".Length;
                        String script = value.Substring(startIndex, value.Length - startIndex);
                        // mWebBrowser.InvokeScript("eval", new string[]{script});
                        mWebBrowser.InvokeScript("execScript", new string[] { script });
                    }
                    else
                    {
                        Uri uri = new Uri(value, UriKind.Relative);
                        mWebBrowser.Navigate(uri);
                    }
                }
            }

            [MoSyncWidgetProperty(MoSync.Constants.MAW_WEB_VIEW_HTML)]
            public String Html
            {
                set
                {
                    mWebBrowser.NavigateToString(value);
                }
            }

            [MoSyncWidgetProperty(MoSync.Constants.MAW_WEB_VIEW_HARD_HOOK)]
            public String HardHook
            {
                set
                {
                    mHardHook = value;
                }
                get
                {
                    return mHardHook;
                }
            }

            [MoSyncWidgetProperty(MoSync.Constants.MAW_WEB_VIEW_SOFT_HOOK)]
            public String SoftHook
            {
                set
                {
                    mSoftHook = value;
                }

                get
                {
                    return mSoftHook;
                }
            }

            [MoSyncWidgetProperty(MoSync.Constants.MAW_WEB_VIEW_BASE_URL)]
            public String BaseUrl
            {
                set
                {
                    mWebBrowser.Base = mBaseURL;
                }
                get
                {
                    return mBaseURL;
                }
            }

            [MoSyncWidgetProperty(MoSync.Constants.MAW_WEB_VIEW_HORIZONTAL_SCROLL_BAR_ENABLED)]
            public String HoziontalScrollBarEnabled
            {
                set
                {
                }
                get
                {
                    return mHorizontalScrollBarEnabled.ToString();
                }
            }

            [MoSyncWidgetProperty(MoSync.Constants.MAW_WEB_VIEW_VERTICAL_SCROLL_BAR_ENABLED)]
            public String VerticalScrollBarEnabled
            {
                set
                {
                    
                }
                get
                {
                    return mVerticalScrollBarEnabled.ToString();
                }
            }

            [MoSyncWidgetProperty(MoSync.Constants.MAW_WEB_VIEW_ENABLE_ZOOM)]
            public String EnableZoom
            {
                set
                {
                    bool.TryParse(value,out mEnabledZoom);
                }
                get
                {
                    return mEnabledZoom.ToString();
                }
            }

            [MoSyncWidgetProperty(MoSync.Constants.MAW_WEB_VIEW_NAVIGATE)]
            public String Navigate
            {
                set
                {
                    if (value.Equals("back"))
                    {
                        mWebBrowser.Navigate(new Uri(mNavigatedFrom));
                    }
                    else if (value.Equals("forward"))
                    {
                        mWebBrowser.Navigate(new Uri(mNavigatingTo));
                    }
                }
            }
            public WebView()
            {
                mWebBrowser = new Microsoft.Phone.Controls.WebBrowser();
                mView = mWebBrowser;
                mWebBrowser.IsScriptEnabled = true;

                mWebBrowser.ScriptNotify += new EventHandler<NotifyEventArgs>(
                    delegate(object from, NotifyEventArgs args)
                    {
                        String str = args.Value;
                        MoSync.Util.Log(str);

                        int hookType = 0;

                        if (Regex.IsMatch(str, mHardHook))
                        {
                            hookType = MoSync.Constants.MAW_CONSTANT_HARD;
                        }
                        else if (Regex.IsMatch(str, mSoftHook))
                        {
                            hookType = MoSync.Constants.MAW_CONSTANT_SOFT;
                        }
                        else
                        {
                            return;
                        }

                        Memory eventData = new Memory(16);
                        const int MAWidgetEventData_eventType = 0;
                        const int MAWidgetEventData_widgetHandle = 4;
                        const int MAWidgetEventData_hookType = 8;
                        const int MAWidgetEventData_urlData = 12;

                        Memory urlData = new Memory(str.Length + 1);
                        urlData.WriteStringAtAddress(0, str, str.Length + 1);

                        eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_WEB_VIEW_HOOK_INVOKED);
                        eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                        eventData.WriteInt32(MAWidgetEventData_hookType, hookType);
                        eventData.WriteInt32(MAWidgetEventData_urlData, mRuntime.AddResource(
                            new Resource(urlData, MoSync.Constants.RT_BINARY)));
                        mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                    }
                );

                mWebBrowser.Navigated += new EventHandler<NavigationEventArgs>(
                    delegate(object from, NavigationEventArgs args)
                    {
                        mNavigatedFrom = args.Uri.ToString();
                    });
                mWebBrowser.Navigating += new EventHandler<NavigatingEventArgs>(
                   delegate(object from, NavigatingEventArgs args)
                   {
                       mNavigatingTo = args.Uri.ToString();
                   });
            }
        }
    }
}
