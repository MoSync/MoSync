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
 * @file MoSyncOptionsBoxModule.cs
 * @author Spiridon Alexandru
 *
 * @brief This represents the Options Box module implementation on the windows phone 7.1 platform. The option box
 * is the application bar with the application bar menu enabled. It contains a cancel button (has a default icon)
 * and a couple other label buttons that represent the choices presented to the user.
 *
 * @platform WP 7.1
 **/
using System;
using System.Windows;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using System.IO;
using System.Collections.Generic;

namespace MoSync
{
    public class OptionsBoxModule : IIoctlModule
    {
        IApplicationBar mApplicationBar;
        // we need to keep the buttons in order to know which one was clicked
        List<ApplicationBarMenuItem> mApplicationBarMenuItems;
        Runtime mRuntime;

        public void Init(Ioctls ioctls, Core core, Runtime runtime)
        {
            /**
		     * Displays a special kind of dialog that has a list of possible choices.
		     * The list of options is displayed as buttons on iOS, and as text views on Android.
		     * By clicking any option the dialog gets dismissed and a #EVENT_TYPE_OPTIONS_BOX_BUTTON_CLICKED event is sent back.
		     *
		     * \param title The dialog title.
		     * \param destructiveButtonTitle The destructive button text. This is an iOS specific feature: it has different color than the other options,
		     * and it indicates that it's action has destructive behaviour. On Android it is treated and it looks like a normal option.
		     * \param cancelButtonTitle The dialog's Cancel button text. If left empty, the dialog is not cancelable.
		     * \param otherButtonTitles The address to the buffer that stores the list of options.
		     * \param otherButtonTitlesSize The size of the buffer, in bytes.
		     */
            ioctls.maOptionsBox = delegate(int _title, int _destructiveButtonTitle, int _cancelButtonTitle, int _otherButtonTitles, int _otherButtonTitlesSize)
            {
                mApplicationBarMenuItems = new List<ApplicationBarMenuItem>();
                mRuntime = runtime;

                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    // get the current application page
                    PhoneApplicationPage currentPage = (((PhoneApplicationFrame)Application.Current.RootVisual).Content as PhoneApplicationPage);

                    // create the application bar and enable the application bar menu which will contain the option buttons
                    currentPage.ApplicationBar = new ApplicationBar();
                    currentPage.ApplicationBar.Mode = ApplicationBarMode.Default;
                    currentPage.ApplicationBar.Opacity = 1.0;
                    currentPage.ApplicationBar.IsVisible = true;
                    currentPage.ApplicationBar.IsMenuEnabled = true;
                    mApplicationBar = currentPage.ApplicationBar;

                    // the cancel button will be an application bar button with a default icon
                    String cancelButtonTitle = core.GetDataMemory().ReadWStringAtAddress(_cancelButtonTitle);
                    ApplicationBarIconButton cancelButton = new ApplicationBarIconButton();
                    // by not specifying the uri path to the button icon, a default one will be set
                    cancelButton.IconUri = new Uri("", UriKind.Relative);
                    cancelButton.Text = cancelButtonTitle;
                    cancelButton.Click += new EventHandler(cancelButton_Click);
                    currentPage.ApplicationBar.Buttons.Add(cancelButton);

                    createOptionButtons(core, _otherButtonTitles);

                    // the destructive button will be the last application bar menu item
                    String destructiveButtonTitle = core.GetDataMemory().ReadWStringAtAddress(_destructiveButtonTitle);
                    ApplicationBarMenuItem destructiveMenuItem = new ApplicationBarMenuItem();
                    destructiveMenuItem.Text = destructiveButtonTitle;
                    destructiveMenuItem.Click += new EventHandler(destructiveButton_Click);
                    currentPage.ApplicationBar.MenuItems.Add(destructiveMenuItem);
                });

                return MoSync.Constants.MAW_RES_OK;
            };
        } // end of Init

        /**
         * Creates the application bar menu items and ads them to the application bar
         * @param _core: the Core class provides helper functions that read/write from/to a certain memory address
         * @param _buttonTitles: an integet representing the address of the button titles buffer start
         */
        private void createOptionButtons(Core _core, int _buttonTitles)
        {
            /**
             * Read an array of string from a given address for a specified field.
             * Strings will be stored in mOtherButtonTitles array.
             * @param address The specified address.
             *                The address must have the following structure:
             *                     - the first element must be a 4-byte int that specifies
             *                       the number of strings that can be read.
             *                     - first null terminated string(UTF-16 encoding).
             *                     - second null terminated string(UTF-16 encoding).
             *                     - etc
             * @param size The size of the buffer buffer from where the string will be read.
             */
            int address = _buttonTitles;
            int numberOfTitles = _core.GetDataMemory().ReadInt32(address);
            address += sizeof(int);
            for (int i = 0; i < numberOfTitles; i++)
            {
                String applicationMenuButtonTitle = _core.GetDataMemory().ReadWStringAtAddress(address);
                // the encoding is UTF16 so the address of the next string is after all the characters and a null
                address += applicationMenuButtonTitle.Length * sizeof(char) + sizeof(char);

                ApplicationBarMenuItem applicationBarMenuItem = new ApplicationBarMenuItem();
                applicationBarMenuItem.Text = applicationMenuButtonTitle;
                applicationBarMenuItem.Click += new EventHandler(applicationBarMenuItem_Click);
                mApplicationBar.MenuItems.Add(applicationBarMenuItem);

                mApplicationBarMenuItems.Add(applicationBarMenuItem);
            }
        }

        /**
         * Called when the user clicks on the cancel button.
         * @param sender: the button that was clicked
         * @param e: some event data
         */
        private void cancelButton_Click(object sender, EventArgs e)
        {
            /**
             * post the event to MoSync runtime - the cancel button is considered to be the
             * last button
             */
            postApplicationBarMenuEvent(mApplicationBarMenuItems.Count + 1);

            mApplicationBar.IsVisible = false;
        }

        /**
         * Called when the user clicks on a application bar menu button.
         * @param sender: the button that was clicked
         * @param e: some event data
         */
        private void applicationBarMenuItem_Click(object sender, EventArgs e)
        {
            int index = 1;
            foreach (ApplicationBarMenuItem item in mApplicationBarMenuItems)
            {
                if (sender.Equals(item))
                {
                    break;
                }
                index++;
            }
            /**
             * post the event to MoSync runtime
             */
            postApplicationBarMenuEvent(index);

            mApplicationBar.IsVisible = false;
        }

        /**
         * Called when the user clicks on the destructive button.
         * @param sender: the button that was clicked
         * @param e: some event data
         */
        private void destructiveButton_Click(object sender, EventArgs e)
        {
            /**
             * post the event to MoSync runtime - the destructive button is considered to have the o
             * button index
             */
            postApplicationBarMenuEvent(0);

            mApplicationBar.IsVisible = false;
        }

        /**
         * Posts the event to the mosync runtime.
         * @param index: the button clicked index
         */
        private void postApplicationBarMenuEvent(int index)
        {
            Memory eventData = new Memory(16);
            // set the main event type: EVENT_TYPE_OPTIONS_BOX_BUTTON_CLICKED
            const int MAWidgetEventData_widgetEventType = 0;
            const int MAWidgetEventData_buttonIndex = 4;
            eventData.WriteInt32(MAWidgetEventData_widgetEventType, MoSync.Constants.EVENT_TYPE_OPTIONS_BOX_BUTTON_CLICKED);
            eventData.WriteInt32(MAWidgetEventData_buttonIndex, index);
            mRuntime.PostEvent(new Event(eventData));
        }
    } // end of options box menu module
} // end of mosync namespace
