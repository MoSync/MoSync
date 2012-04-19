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
 * @file MoSyncModalDialogModule.cs
 * @author Spiridon Alexandru
 *
 * @brief This represents the Modal Dialog module implementation
 *        on Windows Phone 7.1, language c#
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
using Microsoft.Phone.Controls;
using MoSync.NativeUI;

namespace MoSync
{
    public class ModalDialogModule : IIoctlModule
    {
        public void Init(Ioctls ioctls, Core core, Runtime runtime)
        {
            /**
	         * Shows a dialog widget.
	         * \param _dialogHandle The handle of the dialog that will be shown.
	         *
	         * \returns Any of the following result codes:
	         * - #MAW_RES_OK if the child could be removed from the parent.
	         * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
	         * - #MAW_RES_ERROR otherwise.
	         */
            ioctls.maWidgetModalDialogShow = delegate(int _dialogHandle)
            {
                if (!isHandleValid(runtime, _dialogHandle))
                {
                    return MoSync.Constants.MAW_RES_INVALID_HANDLE;
                }

                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    // show the dialog
                    ((ModalDialog)runtime.GetModule<NativeUIModule>().GetWidget(_dialogHandle)).ShowDialog(true);
                });

                return MoSync.Constants.MAW_RES_OK;
            };

            /**
	         * Hides/Dismisses a currently displayed dialog.
	         * \param _dialogHandle The handle of the dialog that will be hidden.
	         *
	         * \returns Any of the following result codes:
	         * - #MAW_RES_OK if the child could be removed from the parent.
	         * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
	         * - #MAW_RES_ERROR otherwise.
	         */
            ioctls.maWidgetModalDialogHide = delegate(int _dialogHandle)
            {
                if (!isHandleValid(runtime, _dialogHandle))
                {
                    return MoSync.Constants.MAW_RES_INVALID_HANDLE;
                }

                MoSync.Util.RunActionOnMainThreadSync(() =>
                {
                    // hide the dialog
                    ((ModalDialog)runtime.GetModule<NativeUIModule>().GetWidget(_dialogHandle)).ShowDialog(false);
                });

                return MoSync.Constants.MAW_RES_OK;
            };
        }

        /*
         * Checks if a handle is a valid handler (a valid handle shouldn't be negative).
         * @param runtime The current runtime
         * @param handle The handle to be checked
         */
        private bool isHandleValid(Runtime runtime, int handle)
        {
            if (runtime.GetModule<NativeUIModule>().GetWidget(handle).GetHandle() < 0)
            {
                return false;
            }
            return true;
        }
    }
}
