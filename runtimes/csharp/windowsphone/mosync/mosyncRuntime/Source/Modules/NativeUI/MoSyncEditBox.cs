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
        public class EditBox : WidgetBaseWindowsPhone
        {
            protected System.Windows.Controls.TextBox mEditBox;
            protected bool mInitialCapsWord;
            protected bool mInitialCapsSentence;

            protected void setInputMode(System.Windows.Input.InputScopeNameValue scopeValue)
            {
                System.Windows.Input.InputScope keyboard = new System.Windows.Input.InputScope();
                System.Windows.Input.InputScopeName scopeName = new System.Windows.Input.InputScopeName();

                scopeName.NameValue = scopeValue;
                keyboard.Names.Add(scopeName);
                mEditBox.InputScope = keyboard;
            }

            public EditBox()
            {
                mEditBox = new System.Windows.Controls.TextBox();
                View = mEditBox;
                mInitialCapsWord = false;
                mInitialCapsSentence = false;
            }

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

            [MoSyncWidgetProperty(MoSync.Constants.MAW_EDIT_BOX_PLACEHOLDER)]
            public String Placeholder
            {
                set
                {
                    mEditBox.Text = value;
                }
            }

            //showKeyboard "true" or "false"
            [MoSyncWidgetProperty(MoSync.Constants.MAW_EDIT_BOX_SHOW_KEYBOARD)]
            public String ShowKeyboard
            {
                set
                {
                    //todo: implement
                }
            }


            //validvalue Either 'password' or 'text'.
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
