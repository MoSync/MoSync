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
 * @file EditBox.cs
 * @author Rata Gabriela
 *
 * @brief This represents the EditBox Widget implementation for the NativeUI
 *        component on Windows Phone 7, language C#
 *
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
        /**
         * The EditBox class defines the attributes and behavior of a EditBox widget
         */
        public class EditBox : WidgetBaseWindowsPhone
        {
            /**
             * TextBox control (receives user input)
             */
            protected System.Windows.Controls.TextBox mEditBox;

            /**
            * if set to true, indicates that all the words typed by the user will start with capital letters
            */
            protected bool mInitialCapsWord;

            /**
             * if set to true, indicates that all the sentences will start with capital letters
             */
            protected bool mInitialCapsSentence;

            /**
             * Helper function that sets the input mode of the edit box
             * @param scopeValue: indicates the type of input that is expected from the user.
             * Can have values like: Url, FullFilePath, FileName, EmailUserName, PostalCode, Password, Numeric
             */
            protected void setInputMode(System.Windows.Input.InputScopeNameValue scopeValue)
            {
                System.Windows.Input.InputScope keyboard = new System.Windows.Input.InputScope();
                System.Windows.Input.InputScopeName scopeName = new System.Windows.Input.InputScopeName();

                scopeName.NameValue = scopeValue;
                keyboard.Names.Add(scopeName);
                mEditBox.InputScope = keyboard;
            }

            /**
             * Constructor
             */
            public EditBox()
            {
                mEditBox = new System.Windows.Controls.TextBox();
                View = mEditBox;
                mInitialCapsWord = false;
                mInitialCapsSentence = false;
            }

            /**
             * Property for setting and getting the text that the edit box contains.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_EDIT_BOX_TEXT)]
            public String Text
            {
                set
                {
                    mEditBox.Text = value;
                }
                get
                {
                    return mEditBox.Text;
                }
            }

            /**
            * Property for setting the default text that the edit box will contain when first displayed
            */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_EDIT_BOX_PLACEHOLDER)]
            public String Placeholder
            {
                set
                {
                    mEditBox.Text = value;
                }
            }

            /**
             * Property for showing/hidding the keyboard
             * set: accepts a String containg the values "true" and "false"
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_EDIT_BOX_SHOW_KEYBOARD)]
            public String ShowKeyboard
            {
                set
                {
                    //todo: implement
                }
            }

            /**
            * Property for setting the input mode for the edit box.
            * set: accepts a String containg the values "text" or "password"
            */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_EDIT_BOX_EDIT_MODE)]
            public String EditMode
            {
                set
                {
                    if (value != "password" && value != "text")
                        return;

                    System.Windows.Input.InputScope keyboard = new System.Windows.Input.InputScope();
                    System.Windows.Input.InputScopeName scopeName = new System.Windows.Input.InputScopeName();
                    if (value == "password")
                    {
                        scopeName.NameValue = System.Windows.Input.InputScopeNameValue.Password;
                    }
                    if (value == "text")
                    {
                        scopeName.NameValue = System.Windows.Input.InputScopeNameValue.Text;
                    }
                    keyboard.Names.Add(scopeName);
                    mEditBox.InputScope = keyboard;
                }
            }

            /**
             * Property for setting the input mode for the edit box.
             * The values have to be the constants defined in IX_WIDGET.h (WidgetEditBoxConstants)  MAW_EDIT_BOX_TYPE_ANY,
             * MAW_EDIT_BOX_TYPE_EMAILADDR, MAW_EDIT_BOX_TYPE_EMAILADDR, MAW_EDIT_BOX_TYPE_PHONENUMBER ect.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_EDIT_BOX_INPUT_MODE)]
            public String InputMode
            {
                set
                {
                    int inputType = Int32.Parse(value);
                    switch (inputType)
                    {
                        case 0:             //MAW_EDIT_BOX_TYPE_ANY
                            setInputMode(System.Windows.Input.InputScopeNameValue.Default);
                            break;
                        case 1:             //MAW_EDIT_BOX_TYPE_EMAILADDR
                            setInputMode(System.Windows.Input.InputScopeNameValue.EmailSmtpAddress);
                            break;
                        case 2:             //MAW_EDIT_BOX_TYPE_NUMERIC
                            setInputMode(System.Windows.Input.InputScopeNameValue.NumberFullWidth);
                            break;
                        case 3:             //MAW_EDIT_BOX_TYPE_PHONENUMBER
                            setInputMode(System.Windows.Input.InputScopeNameValue.TelephoneNumber);
                            break;
                        case 4:             //MAW_EDIT_BOX_TYPE_URL
                            setInputMode(System.Windows.Input.InputScopeNameValue.Url);
                            break;
                        case 5:             //MAW_EDIT_BOX_TYPE_DECIMAL
                            setInputMode(System.Windows.Input.InputScopeNameValue.Digits); //???????
                            break;
                        case 6:             //MAW_EDIT_BOX_TYPE_SINGLE_LINE
                            mEditBox.TextWrapping = TextWrapping.NoWrap;
                            break;
                    }
                }
            }
        }
    }
}
