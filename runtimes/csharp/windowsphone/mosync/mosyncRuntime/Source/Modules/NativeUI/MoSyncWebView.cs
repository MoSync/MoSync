﻿/* Copyright (C) 2011 MoSync AB

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
                        mWebBrowser.InvokeScript("eval", "history.go(-1)");
                    }
                    else if (value.Equals("forward"))
                    {
                        mWebBrowser.InvokeScript("eval", "history.go(1)");
                    }
                    else throw new InvalidPropertyValueException();
                }
            }

            //the contructor
            public WebView()
            {
                mWebBrowser = new Microsoft.Phone.Controls.WebBrowser();
                mView = mWebBrowser;
                mWebBrowser.IsScriptEnabled = true;

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
                    });
            }
        }
    }
}
