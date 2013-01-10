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
 * @file MoSyncDialogView.cs
 * @author Spiridon Alexandru
 *
 * @brief This represents the Dialog View Widget implementation for the NativeUI
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
using System.Windows.Controls.Primitives;
using Microsoft.Phone.Controls;

namespace MoSync
{
    namespace NativeUI
    {
        /**
         * The Dialog view class defines the attributes and behavior of a Dialog view widget.
         * The dialog view allows the developer to add it's own controls on a view that appears on
         * top of the main application layer.
         */
        public class ModalDialog : WidgetBaseWindowsPhone
        {
            /*
             * The canvas used as a background for the dialog view. It contains the
             * stack panel that holds all the controls and behaves like a transparent overlay over the
             * screen that presented the dialog view.
             */
            protected Canvas mDialogBackground;

            /*
             * The stack panel control that is used to encapsulate the dialog view
             * controls. A stackpanel was used for a better and quicker arrangement
             * of controls inside the dialog.
             */
            protected System.Windows.Controls.StackPanel mDialogView;

            /*
             * The text block used as a title for the dialog view
             */
            protected TextBlock titleTextBlock;

            // the background color for the dialog view
            private static String dialogViewBackgroundColor = "#FF1F1F1F";

            // the font color for the title
            private static String dialogViewTitleFontColor = "#FFFFFFFF";

            // the distance between the dialog view stack panel and the screen edge
            private static int marginDistance = 12;

            // the background overlay color (the background color that's behind the dialog view)
            private static String dialogViewOverlayColor = "#7F000000";

            // keeps track of the visible state of the dialog view
            private static bool visible;

            /**
             * Constructor
             */
            public ModalDialog()
            {
                // create the dialog view
                mDialogView = new StackPanel();

                dialogViewBackgroundColor = Application.Current.Resources["PhoneBackgroundColor"].ToString();

                mDialogView.Background = GetColorFromHexa(dialogViewBackgroundColor);
                // remove the bottom margin from the total height of the dialog view
                mDialogView.Height = Application.Current.Host.Content.ActualHeight - marginDistance;
                // remove the margins from the total width of the dialog view (remove 2 * marginDistance)
                mDialogView.Width = Application.Current.Host.Content.ActualWidth - marginDistance * 2;
                mDialogView.Margin = new Thickness(marginDistance, 0, marginDistance, marginDistance);

                // the title text block
                titleTextBlock = new TextBlock();
                titleTextBlock.Width = Application.Current.Host.Content.ActualWidth;
                /*
                 * PhoneTextLargeStyle description:
                 * BasedOn: PhoneTextBlockBase
                 * FontFamily: PhoneFontFamilySemiLight
                 * FontSize: PhoneFontSizeLarge
                 */
                titleTextBlock.Style = (Style)Application.Current.Resources["PhoneTextLargeStyle"];
                titleTextBlock.TextWrapping = TextWrapping.Wrap;
                titleTextBlock.Foreground = GetColorFromHexa(dialogViewTitleFontColor);
                // the title text block has it's own margin distance inside the dialog view stack panel
                titleTextBlock.Margin = new Thickness(marginDistance);

                mDialogView.Children.Add(titleTextBlock);

                // create the transparent background of the popup control
                mDialogBackground = new Canvas();
                mDialogBackground.Background = GetColorFromHexa(dialogViewOverlayColor);
                mDialogBackground.Children.Add(mDialogView);

                // the dialog is not visible at creation
                visible = false;

                mView = mDialogBackground;

                // we need to change the width and the height of the mDialogView manually when the orientation changes
                (Application.Current.RootVisual as Microsoft.Phone.Controls.PhoneApplicationFrame).OrientationChanged += new EventHandler<Microsoft.Phone.Controls.OrientationChangedEventArgs>(OrientationChangedHandler);
            }


            // The AddChild implementation
            public override void AddChild(IWidget child)
            {
                base.AddChild(child);

                WidgetBaseWindowsPhone w = (WidgetBaseWindowsPhone)child;
                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    mDialogView.Children.Add(w.View);
                });
            }

            /**
             * Property for setting and getting the title of the dialog view.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_MODAL_DIALOG_TITLE)]
            public String Title
            {
                set
                {
                    titleTextBlock.Text = value;
                }
                get
                {
                    return titleTextBlock.Text;
                }
            }

            /*
             * Method used to show/dismiss the dialog view.
             * @param show: if show is true, the dialog will be shown and hidden otherwise.
             */
            public void ShowDialog(bool show)
            {
                mDialogBackground.Visibility = (show ? Visibility.Visible : Visibility.Collapsed);
                Grid mainScreenGrid = (((Application.Current.RootVisual as PhoneApplicationFrame).Content as PhoneApplicationPage).Content as Grid);
                if (mDialogBackground.Visibility == Visibility.Visible)
                {
                    if (!mainScreenGrid.Children.Contains(mDialogBackground))
                    {
                        mainScreenGrid.Children.Add(mDialogBackground);
                    }
                }
                else
                {
                    if (mainScreenGrid.Children.Contains(mDialogBackground))
                    {
                        mainScreenGrid.Children.Remove(mDialogBackground);
                    }
                }
            }

            /**
             * Function used to return a SolidColorBrush from a hexadecimal color string
             */
            public static SolidColorBrush GetColorFromHexa(string hexaColor)
            {
                return new SolidColorBrush(
                    Color.FromArgb(
                        Convert.ToByte(hexaColor.Substring(1, 2), 16),
                        Convert.ToByte(hexaColor.Substring(3, 2), 16),
                        Convert.ToByte(hexaColor.Substring(5, 2), 16),
                        Convert.ToByte(hexaColor.Substring(7, 2), 16)
                    )
                );
            }

            /**
             * The Orientation changed event handler
             * Currently it contains the functionality for the orientation changed event.
             * @param from Object the object that triggers the event
             * @param args Microsoft.Phone.Controls.OrientationChangedEventArgs the event arguments
             */
            public void OrientationChangedHandler(object from, Microsoft.Phone.Controls.OrientationChangedEventArgs args)
            {
                // We dont need to change the background canvas width/height because they're automatically
                // redimensioned when the mDialogView child changes it's shape.
                if (args.Orientation == PageOrientation.Landscape || args.Orientation == PageOrientation.LandscapeLeft ||
                    args.Orientation == PageOrientation.LandscapeRight)
                {
                    mDialogView.Width = Application.Current.Host.Content.ActualHeight - 2 * marginDistance;
                    mDialogView.Height = Application.Current.Host.Content.ActualWidth - marginDistance;
                }
                else
                {
                    mDialogView.Width = Application.Current.Host.Content.ActualWidth - 2 * marginDistance;
                    mDialogView.Height = Application.Current.Host.Content.ActualHeight - marginDistance;
                }
            }

            /**
             * Dialog view visible property.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_VISIBLE)]
            public new string Visible
            {
                get
                {
                    return visible.ToString().ToLower();
                }
                set
                {
                    bool boolValue;
                    if (Boolean.TryParse(value, out boolValue))
                    {
                        visible = boolValue;
                    }
                }
            }

            /**
             * Dialog view Width property implementation.Because this widget is presented
             * fullscreen, this property is read-only.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_WIDTH)]
            public new double Width
            {
                get
                {
                    return mDialogView.Width;
                }
            }

            /**
             * Dialog view Height property implementation. Because this widget is presented
             * fullscreen, this property is read-only.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_WIDGET_HEIGHT)]
            public new double Height
            {
                get
                {
                    return mDialogView.Height;
                }
            }
        }
    }
}
