/* Copyright (C) 2012 MoSync AB

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
 * @file MoSyncSearchBar.cs
 * @author Spiridon Alexandru
 *
 * @brief This represents the SearchBar Widget implementation for the NativeUI
 *        component on Windows Phone 7, language C#
 *
 * @platform WP 7.1
 **/
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
        /**
         * The SearchBar class defines the attributes and behavior of a Search bar widget.
         * A search bar allows the user to input text and is used for filtering (a list for example)
         * while the user types (no search/cancel buttons are present on this widget).
         */
        public class SearchBar : WidgetBaseWindowsPhone
        {
            /**
             * TextBox control (receives user input)
             */
            protected System.Windows.Controls.TextBox mSearchBar;

            /**
             * if set to true, indicates that all the text is a watermark/placeholder
             */
            protected bool mIsWatermarkMode;

            /**
             * if true, indicates that this is the first char entered so delete
             * the watermark/placeholder
             */
            protected bool mFirstChar;

            /**
             * save the "normal" brush when working with the watermark
             */
            protected Brush mForegroundColor;

            /**
             * The watermark/placeholder text
             */
            protected String mPlaceholderText;

            /**
             * The placeholder font color
             */
            SolidColorBrush mWaterMarkBrush;

            /**
             * Constructor
             */
            public SearchBar()
            {
                mSearchBar = new System.Windows.Controls.TextBox();

                mView = mSearchBar;

                mIsWatermarkMode = false;
                mPlaceholderText = "";

                // by default, the placeholder font color is gray
                mWaterMarkBrush = new SolidColorBrush();
                mWaterMarkBrush.Color = Colors.Gray;

                mForegroundColor = mSearchBar.Foreground;

                /**
                 * @brief Sent from the Search bar when it gains focus(the user selects the widget).
                 * The virtual keyboard is shown.
                 */
                mSearchBar.GotFocus += new RoutedEventHandler(
                    delegate(object from, RoutedEventArgs args)
                    {
                        /**
                          * simulating the placeholder/watermark
                          */

                        // if watermark present and no user char has been entered
                        if (mIsWatermarkMode && mFirstChar)
                        {
                            // move the cursor to the first position
                            mSearchBar.Select(0, 0);
                        }
                    }
                ); // end of mEditBox.GotFocus


                /**
                 * @brief Sent from the Search bar when it loses focus.
                 * The virtual keyboard is hidden.
                 */
                mSearchBar.LostFocus += new RoutedEventHandler(
                    delegate(object from, RoutedEventArgs args)
                    {
                        /**
                         * simulating the placeholder/watermark
                         */

                        // if no text has been entered by the user than leave the watermark text
                        if (mSearchBar.Text.Equals(""))
                        {
                            setWatermarkMode(true);
                        }
                    }
                ); // end of mEditBox.LostFocus


                /**
                  * @brief Sent from the Search bar when the text was changed.
                  * As the search bar allows searching while the user types, this is
                  * the place where we send the search event (the MAW_EVENT_CLICKED with
                  * the search button click 0 (0 = OK, 1 = CANCEL).
                  * The cancel search is not handled on the windows phone platform.
                  */
                mFirstChar = true;
                mSearchBar.TextInputStart += new TextCompositionEventHandler(
                    delegate(object from, TextCompositionEventArgs args)
                    {
                        /**
                          * simulating the placeholder/watermark
                          */
                        if (mFirstChar)
                        {
                            setWatermarkMode(false);
                        }

                        /**
                         * post the event to MoSync runtime
                         * MAW_EVENT_CLICKED + search button clicked = 0 (0 = OK, 1 = CANCEL)
                         */
                        Memory eventData = new Memory(12);
                        const int MAWidgetEventData_eventType = 0;
                        const int MAWidgetEventData_widgetHandle = 4;
                        const int MAWidgetEventData_searchButtonClicked = 8;
                        eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_CLICKED);
                        eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                        eventData.WriteInt32(MAWidgetEventData_searchButtonClicked, 0);
                        mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                    }
                ); // end of TextInputStart
            }


            /**
             * Property for setting and getting the text that the Search bar contains.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_SEARCH_BAR_TEXT)]
            public String Text
            {
                set
                {
                    // if we're in watermark mode and the text is changed, we need to
                    // enter the normal search bar mode
                    if (mIsWatermarkMode)
                    {
                        setWatermarkMode(false);
                    }
                    mSearchBar.Text = value;
                }
                get
                {
                    if (mIsWatermarkMode)
                    {
                        // if we're in watermark mode, the Search bar contains no text
                        return "";
                    }
                    return mSearchBar.Text;
                }
            }


            /**
             * Property for setting the default text that the search bar will contain when first displayed.
             *
             * Since the Watermark is not yet implemented we use the solution suggested by MS
             * (http://msdn.microsoft.com/en-us/library/system.windows.controls.textbox(v=vs.95).aspx)
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_SEARCH_BAR_PLACEHOLDER)]
            public String Placeholder
            {
                set
                {
                    mIsWatermarkMode = true;

                    mSearchBar.Foreground = mWaterMarkBrush;
                    mPlaceholderText = value;

                    mSearchBar.Text = mPlaceholderText;
                }
            }

            /**
             * Property for showing/hidding the keyboard
             * set: accepts a String containg the values "true" and "false"
             *
             * Windows Phone 7 doesn't have a way to hide the keyboard other than setting focus to
             * another control that doesn't accept input, thus hiding the keyboard.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_SEARCH_BAR_SHOW_KEYBOARD)]
            public String ShowKeyboard
            {
                set
                {
                    if (value.Equals("true"))
                    {
                        mSearchBar.Focus();
                    }
                    else if (value.Equals("false"))
                    {
                        // we need to focus another, non-input element in order to hide the keyboard
                        try
                        {
                            // the parent is most probable to exist and be a non-input control
                            System.Windows.Controls.Control ctrl = (System.Windows.Controls.Control)mSearchBar.Parent;
                            ctrl.Focus();
                        }
                        catch
                        {
                        }
                    }
                }
            }

            /*
             * Switches the search bar to the watermark mode.
             * @param watermarkMode: indicates if we need to switch to watermarkMode or not.
             * Values: true (switch to watermark mode), false (switch to edit mode)
             */
            protected void setWatermarkMode(bool watermarkMode)
            {
                if (watermarkMode)
                {
                    Placeholder = mPlaceholderText;
                    mFirstChar = true;
                    mIsWatermarkMode = true;
                }
                else
                {
                    mIsWatermarkMode = false;
                    mFirstChar = false;
                    mSearchBar.Text = "";

                    // change the foreground to "normal" for user input
                    mSearchBar.Foreground = mForegroundColor;
                }
            }
        }
    }
}
