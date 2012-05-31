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
 * @file MoSyncCustomNumberPicker.cs
 * @author Filipas Ciprian
 *
 * @brief This represents the native custom Number Picker implementation
 **/

using System;
using System.ComponentModel;
using System.Diagnostics.CodeAnalysis;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Controls.Primitives;

namespace MoSync
{
    namespace NativeUI
    {
        public class CustomNumberPicker : System.Windows.Controls.TextBox
        {
            // The phone application frame refference.
            private PhoneApplicationFrame mFrame;

            // Used for saving the content when the navigation occurs.
            private object mFrameContentWhenOpened;

            // Used for saving the NavigationInTransition when the navigation occurs.
            private NavigationInTransition mSavedNavigationInTransition;

            // Used for saving the NavigationOutTransition when the navigation occurs.
            private NavigationOutTransition mSavedNavigationOutTransition;

            // Refference to the CustomNumberPickerPage.
            private mosyncRuntime.Views.CustomNumberPickerPage mNumberPickerPage;

            // Min and Max values.
            private int mMin = 0;
            private int mMax = 100;

            // The Value changed event handler.
            public event EventHandler<NumberPickerValueChangedEventArgs> ValueChanged;

            // The constructor
            public CustomNumberPicker()
            {
                this.IsReadOnly = true;
                this.Tap += OnNumberButtonClick;
            }

            /**
             * @author Ciprian Filipas
             * @brief Gets or sets the int value.
             */
            [TypeConverter(typeof(int))]
            [SuppressMessage("Microsoft.Naming", "CA1721:PropertyNamesShouldNotMatchGetMethods", Justification = "Matching the use of Value as a Picker naming convention.")]
            public int? Value
            {
                get { return (int)GetValue(ValueProperty); }
                set
                {
                    if (value >= Min && value <= Max)
                    {
                        SetValue(ValueProperty, value);
                        this.Text = value.Value.ToString();
                    }
                    else throw new MoSync.InvalidPropertyValueException();
                }
            }

            /**
             * @author Ciprian Filipas
             * @brief Identifies the Value DependencyProperty.
             */
            public static readonly DependencyProperty ValueProperty = DependencyProperty.Register(
                "Value", typeof(int?), typeof(CustomNumberPicker), new PropertyMetadata(null, OnValueChanged));

            /**
             * @authro Ciprian Filipas
             * @brief Callback function for the dependency property ValueProperty.
             * @param1 depObj DependencyObject.
             * @param2 evtArgs DependencyProvertyChangedEventArgs.
             */
            private static void OnValueChanged(DependencyObject depObj, DependencyPropertyChangedEventArgs evtArgs)
            {
                ((CustomNumberPicker)depObj).OnValueChanged((int?)evtArgs.OldValue, (int?)evtArgs.NewValue);
            }

            /**
             * @author Ciprian Filipas
             * @brief Function called from the callback of the ValueProperty dependency property identifieng the
             *        involved values (old and new).
             * @param1 oldValue int? nullable int storing the old value
             * @param2 newValue int? nullable int storing the new value
             */
            private void OnValueChanged(int? oldValue, int? newValue)
            {
                OnValueChanged(new NumberPickerValueChangedEventArgs(oldValue, newValue));
            }

            /**
             * @brief The OnValueChanged handler accessible to the further children classes.
             *        A nicer interface with a proper Event Args class.
             */
            protected virtual void OnValueChanged(NumberPickerValueChangedEventArgs e)
            {
                var handler = ValueChanged;
                if (null != handler)
                {
                    handler(this, e);
                }
            }

            /**
             * @author Ciprian Filipas
             * @brief handler for the textBox click.
             */
            private void OnNumberButtonClick(object sender, System.Windows.Input.GestureEventArgs e)
            {
                e.Handled = true;
                OpenPickerPage();
            }

            /**
             * @author Ciprian Filipas
             * @brief Function responsible with the open action for the picker page.
             */
            private void OpenPickerPage()
            {
                if (null == mFrame)
                {
                    // Hook up to necessary events and navigate
                    mFrame = Application.Current.RootVisual as PhoneApplicationFrame;
                    if (null != mFrame)
                    {
                        mFrameContentWhenOpened = mFrame.Content;

                        // Save and clear host page transitions for the upcoming "popup" navigation
                        UIElement frameContentWhenOpenedAsUIElement = mFrameContentWhenOpened as UIElement;
                        if (null != frameContentWhenOpenedAsUIElement)
                        {
                            mSavedNavigationInTransition = TransitionService.GetNavigationInTransition(frameContentWhenOpenedAsUIElement);
                            TransitionService.SetNavigationInTransition(frameContentWhenOpenedAsUIElement, null);
                            mSavedNavigationOutTransition = TransitionService.GetNavigationOutTransition(frameContentWhenOpenedAsUIElement);
                            TransitionService.SetNavigationOutTransition(frameContentWhenOpenedAsUIElement, null);
                        }

                        mFrame.Navigated += OnFrameNavigated;
                        mFrame.NavigationStopped += OnFrameNavigationStoppedOrFailed;
                        mFrame.NavigationFailed += OnFrameNavigationStoppedOrFailed;

                        NumberPickerPageCustomUriString customString = new NumberPickerPageCustomUriString();
                        customString.Max = Max;
                        customString.Min = Min;

                        mFrame.Navigate(new Uri(customString.UriString, UriKind.RelativeOrAbsolute));
                    }
                }
            }

            /**
             * @authro Ciprian Filipas
             * @brief handler for the Frame.Navigated event.
             */
            private void OnFrameNavigated(object sender, NavigationEventArgs e)
            {
                if (e.Content == mFrameContentWhenOpened)
                {
                    ClosePickerPage();
                }
                else if (null == mNumberPickerPage)
                {
                    mNumberPickerPage = e.Content as mosyncRuntime.Views.CustomNumberPickerPage;
                    if (null != mNumberPickerPage)
                    {
                        mNumberPickerPage.Value = Value;
                    }
                }
            }

            /**
             * @author Ciprian Filipas
             * @brief handler for the Frame.NavigationStopped & Frame.NavigationFailed events.
             */
            private void OnFrameNavigationStoppedOrFailed(object sender, EventArgs e)
            {
                ClosePickerPage();
            }

            /**
             * @author Filipas Ciprian
             * @brief Function responsible with the closing action for the picker page.
             */
            private void ClosePickerPage()
            {
                // Unhook from events
                if (null != mFrame)
                {
                    mFrame.Navigated -= OnFrameNavigated;
                    mFrame.NavigationStopped -= OnFrameNavigationStoppedOrFailed;
                    mFrame.NavigationFailed -= OnFrameNavigationStoppedOrFailed;

                    // Restore host page transitions for the completed "popup" navigation
                    UIElement frameContentWhenOpenedAsUIElement = mFrameContentWhenOpened as UIElement;
                    if (null != frameContentWhenOpenedAsUIElement)
                    {
                        TransitionService.SetNavigationInTransition(frameContentWhenOpenedAsUIElement, mSavedNavigationInTransition);
                        mSavedNavigationInTransition = null;
                        TransitionService.SetNavigationOutTransition(frameContentWhenOpenedAsUIElement, mSavedNavigationOutTransition);
                        mSavedNavigationOutTransition = null;
                    }

                    mFrame = null;
                    mFrameContentWhenOpened = null;
                }
                // Commit the value if available
                if (null != mNumberPickerPage)
                {
                    if (mNumberPickerPage.Value.HasValue)
                    {
                        Value = mNumberPickerPage.Value.Value;
                    }
                    mNumberPickerPage = null;
                }
            }

            /**
             * @author Ciprian Filipas
             * @brief The Min property
             */
            public int Min
            {
                set
                {
                    if (value < mMax)
                    {
                        mMin = value;
                        if (this.Value.Value < mMin)
                        {
                            this.Value = mMin;
                        }
                    }
                    else throw new MoSync.InvalidPropertyValueException();
                }
                get
                {
                    return mMin;
                }
            }

            /**
             * @author Ciprian Filipas
             * @brief The Max property
             */
            public int Max
            {
                set
                {
                    if (value > mMin)
                    {
                        mMax = value;
                        if (this.Value.Value > mMax)
                        {
                            this.Value = mMax;
                        }
                    }
                    else throw new MoSync.InvalidPropertyValueException();
                }
                get
                {
                    return mMax;
                }
            }
        }
    }
}
