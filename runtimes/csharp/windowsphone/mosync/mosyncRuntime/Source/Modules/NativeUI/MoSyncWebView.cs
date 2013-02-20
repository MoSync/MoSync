/* Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/
/**
 * @file MoSyncWebView.cs
 * @author Niklas Nummelin, Ciprian Filipas
 *
 * @brief This represents the WebView implementation for the NativeUI
 *        component on Windows Phone 7, language c#
 * @note The HorizontalScrollBarEnabled, VerticalScrollBarEnabled, EnableZoom properties are not
 *       available in Windows Phone 7 for a WebBrowser controll.
 * @platform WP 7.1
 **/

using Microsoft.Phone.Controls;
using System.Windows.Controls;
using System.Windows;
using System.Windows.Navigation;
using System;
using System.Text.RegularExpressions;
using System.Reflection;
using System.Collections.Generic;

namespace MoSync
{
    namespace NativeUI
    {
        public class WebView : WidgetBaseWindowsPhone
        {
            //the WebBrowser object
            protected WebBrowser mWebBrowser;

            //a string containing the HardHook
            protected String mHardHook = "";

            //a string containing the SoftHook
            protected String mSoftHook = "";

            //this is used when loading relative paths
            protected String mBaseURL = "";

            /**
             * Because on the wp7 platform there is no proper way to get the browser history, we need
             * to manage it ourselves.
             * The algorithm is as follows:
             *  - every time we navigate to a new page, the uri is added to the mHistoryStack and
             *  mHistoryStackIndex increases
             *  - if we press the back button, we check if mHistoryIndex is greater than one:
             *      - if it is, we can navigate backwards, we decrease the mHistoryStackIndex,
             *      we set the mFromHistory to true, we navigate to the uri specified by mHistoryStackIndex - 1
             *      and we cancel the event (we handled it ourselves)
             *      - it it's not, the back event isn't canceled and the application will
             *      handle the back button press
             *  - when the browser has sent the 'Navigated' event we check if we navigated from history or not:
             *      - if not, it means that we only need to keep the uri-s from 0 to mHistoryStackIndex - 1 so
             *      we ditch all the uri-s in the interval [mHistoryStackIndex, mHistoryStack.Count - 1]
             *  @code
             *      mHistoryStack.RemoveRange(mHistoryStackIndex, mHistoryStack.Count - mHistoryStackIndex);
             *  @endcode
             *      and then we add the new uri to the stack and we increase mHistoryStackIndex;
             *      - regardless if we navigated from history or not, at the end of the 'Navigated' event
             *      handler, we set 'mFromHistory' to false (we navigated once from history, the next navigation
             *      might be to a new page or another one from history if the back button is pressed again)
             *  A forward navigation through history was also added and it behaves almost the same way as the
             *  backward one.
             */
            // keeps track of all the uri-s we visited
            List<Uri> mHistoryStack;

            // keeps track of the current location inside the history stack
            int mHistoryStackIndex;

            //navigation boolean, set on true while back is handled
            private bool mFromHistory;

            //boolean set on true if the web browser gains focus
            private bool mFocused = false;

            //MAW_WEB_VIEW_URL property implementation
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
                        Uri uri;
                        if (value.StartsWith("http://") || value.StartsWith("www."))
                        {
                            if (value.StartsWith("http://") == false)
                            {
                                string newUri = "http://" + value;
                                uri = new Uri(newUri, UriKind.Absolute);
                            }
                            else
                            {
                                uri = new Uri(value, UriKind.Absolute);
                            }

                        }
                        else if (!mBaseURL.Equals(""))
                        {
                            //external web page
                            if (mBaseURL.StartsWith("http://"))
                                uri = new Uri(mBaseURL + value, UriKind.Absolute);
                            else
                            {
                                uri = new Uri(value, UriKind.Relative);
                            }
                        }
                        else
                        {
                            uri = new Uri(value, UriKind.Relative);
                        }

                        mWebBrowser.Navigate(uri);

						/*
						String initialScript =
						"window.onerror = function(e)\n" +
						"{\n" +
						"window.external.Notify(\"Error:\" + JSON.stringify(e));\n" +
						"};\n" +
						"if(typeof window.console == \"undefined\")\n" +
						"{\n" +
						"window.console = {\n" +
						"log:function(str) {\n" +
						"window.external.Notify(\"Info:\" + str);\n" +
						"}\n" +
						"};\n" +
						"}\n";

						mWebBrowser.InvokeScript("execScript", new string[] { initialScript });
						*/
                    }
                }
                get
                {
                    return mWebBrowser.Source.ToString();
                }
            }

            //MAW_WEB_VIEW_HTML property implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WEB_VIEW_HTML)]
            public String Html
            {
                set
                {
                    mWebBrowser.NavigateToString(value);
                }
            }

            //MAW_WEB_VIEW_HARD_HOOK property implementation
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

            //MAW_WEB_VIEW_SOFT_HOOK property implementation
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

            //MAW_WEB_VIEW_BASE_URL property implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WEB_VIEW_BASE_URL)]
            public String BaseUrl
            {
                set
                {
                    mBaseURL = value;

                    if(mBaseURL.StartsWith("file://"))
                    {
                        // Removing the "file://" it has no meaning on windows phone 7 since the navigation
                        // inside the isolatedStorage is done relatively
                        mBaseURL = mBaseURL.Remove(0, 7);
                    }
                    mWebBrowser.Base = mBaseURL;
                }
                get
                {
                    return mBaseURL;
                }
            }

            //MAW_WEB_VIEW_NAVIGATE property implementation
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WEB_VIEW_NAVIGATE)]
            public String Navigate
            {
                set
                {
                    if (value.Equals("back"))
                    {
                        // navigate backwards into history
                        mHistoryStackIndex -= 1;
                        mFromHistory = true;
                        mWebBrowser.Navigate(mHistoryStack[mHistoryStackIndex - 1]);
                    }
                    else if (value.Equals("forward"))
                    {
                        // if there are still pages to navigate to, navigate forward into history
                        if (mHistoryStackIndex < mHistoryStack.Count)
                        {
                            mHistoryStackIndex += 1;
                            mFromHistory = true;
                            mWebBrowser.Navigate(mHistoryStack[mHistoryStackIndex - 1]);
                        }
                    }
                    else throw new InvalidPropertyValueException();
                }
            }

            public void BackKeyPressHandler(object from, System.ComponentModel.CancelEventArgs args)
            {
                try
                {
                    // if we can go back into the history stack, cancel the
                    // event and handle the navigation ourselves
                    if (mHistoryStackIndex > 1 && mFocused)
                    {
                        Navigate = "back";
                        args.Cancel = true;
                    }
                }
                catch
                {
                    //supress the error
                }
            }

            //the contructor
            public WebView()
            {
                mWebBrowser = new Microsoft.Phone.Controls.WebBrowser();
                mView = mWebBrowser;
                mWebBrowser.IsScriptEnabled = true;

                mWebBrowser.IsGeolocationEnabled = true;

                mHistoryStack = new List<Uri>();
                // there's nothing inside the history stack until we navigated to a page
                mHistoryStackIndex = 0;
                mFromHistory = false;

                (Application.Current.RootVisual as Microsoft.Phone.Controls.PhoneApplicationFrame).BackKeyPress += new EventHandler<System.ComponentModel.CancelEventArgs>(BackKeyPressHandler);

                mWebBrowser.GotFocus += new RoutedEventHandler(
                    delegate(object from, RoutedEventArgs args)
                    {
                        mFocused = true;
                    });

                mWebBrowser.LostFocus += new RoutedEventHandler(
                    delegate(object from, RoutedEventArgs args)
                    {
                        mFocused = false;
                    });

                fillSpaceHorizontalyEnabled = false;
                fillSpaceVerticalyEnabled = false;

                //adding an event handler for the script notify
                mWebBrowser.ScriptNotify += new EventHandler<NotifyEventArgs>(
                    delegate(object from, NotifyEventArgs args)
                    {
                        String str = args.Value;
                        //MoSync.Util.Log(str + "\n");

                        int hookType = MoSync.Constants.MAW_CONSTANT_SOFT;

                        //the MAW_EVENT_WEB_VIEW_HOOK_INVOKED needs a chunk of 16 bytes of memory
                        Memory eventData = new Memory(16);
                        const int MAWidgetEventData_eventType = 0;
                        const int MAWidgetEventData_widgetHandle = 4;
                        const int MAWidgetEventData_hookType = 8;
                        const int MAWidgetEventData_urlData = 12;

                        //constructing the urlData
                        System.IO.MemoryStream urlData = new System.IO.MemoryStream(str.Length + 1);
						byte[] strBytes = System.Text.UTF8Encoding.UTF8.GetBytes(str);
						urlData.Write(strBytes, 0, strBytes.Length);

                        eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_WEB_VIEW_HOOK_INVOKED);
                        eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                        eventData.WriteInt32(MAWidgetEventData_hookType, hookType);
                        eventData.WriteInt32(MAWidgetEventData_urlData, mRuntime.AddResource(
                            new Resource(urlData, MoSync.Constants.RT_BINARY, true)));
                        mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                    });

                mWebBrowser.LoadCompleted += new LoadCompletedEventHandler(
                    delegate(object from, NavigationEventArgs args)
                    {
                        //Note that this event occurs when the content is completely loaded
                        //which means that this is the place to post the custom MoSync event
                        //MAW_EVENT_WEB_VIEW_CONTENT_LOADING to signal that the loading process
                        //is completed

                        //sending the MAW_EVENT_WEB_VIEW_CONTENT_LOADING with the MAW_CONSTANT_DONE parameter

                        //the MAW_EVENT_WEB_VIEW_CONTENT_LOADING needs a chunk of 12 bytes of memory
                        Memory eventData = new Memory(12);
                        const int MAWidgetEventData_eventType = 0;
                        const int MAWidgetEventData_widgetHandle = 4;
                        const int MAWidgetEventData_status = 8;
                        eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_WEB_VIEW_CONTENT_LOADING);
                        eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                        eventData.WriteInt32(MAWidgetEventData_status, MoSync.Constants.MAW_CONSTANT_DONE);
                        mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                    });

                mWebBrowser.Navigated += new EventHandler<NavigationEventArgs>(
                    delegate(object from, NavigationEventArgs args)
                    {
                        //Note that when this event is called it means that the current page has been
                        //navigated to which implies that the content will start loading so this is the
                        //place to post the custom MoSync event MAW_EVENT_WEB_VIEW_CONTENT_LOADING to
                        //signal that the process has started

                        //sending the MAW_EVENT_WEB_VIEW_CONTENT_LOADING with the MAW_CONSTANT_STARTED parameter

                        //the MAW_EVENT_WEB_VIEW_CONTENT_LOADING needs a chunk of 12 bytes of memory
                        Memory eventData = new Memory(12);
                        const int MAWidgetEventData_eventType = 0;
                        const int MAWidgetEventData_widgetHandle = 4;
                        const int MAWidgetEventData_status = 8;

                        eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_WEB_VIEW_CONTENT_LOADING);
                        eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                        eventData.WriteInt32(MAWidgetEventData_status, MoSync.Constants.MAW_CONSTANT_STARTED);

                        mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);

                        if (!mFromHistory)
                        {
                            if (mHistoryStackIndex < mHistoryStack.Count)
                            {
                                mHistoryStack.RemoveRange(mHistoryStackIndex, mHistoryStack.Count - mHistoryStackIndex);
                            }
                            mHistoryStack.Add(args.Uri);
                            mHistoryStackIndex += 1;
                        }
                        // always set the mFromHistory to false because the next navigation could be to a new page
                        // (if it's a page loaded by pressing the back button, mFromHistory will be true so we
                        // won't need to digth uris; if it's a new page, we will ditch the history in the interval
                        // [mHistoryStackIndex, mHistoryStack.Count - 1] and we'll add the new page to the updated
                        // stack)
                        mFromHistory = false;
                    });
            }
        }
    }
}
