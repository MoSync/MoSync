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
             * The Popup control used as a dialog view
             */
            protected Popup mDialogPopUp;

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

            /**
             * Constructor
             */
            public ModalDialog()
            {
                // create the dialog view
                mDialogView = new StackPanel();
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
                mDialogPopUp = new Popup();
                mDialogPopUp.Child = new StackPanel(); ;
                ((StackPanel)mDialogPopUp.Child).Children.Add(mDialogView);
                ((StackPanel)mDialogPopUp.Child).Background = GetColorFromHexa(dialogViewOverlayColor);

                mView = mDialogPopUp;
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
             */
            public void ShowDialog(bool show)
            {
                mDialogPopUp.IsOpen = show;
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
        }
    }
}
