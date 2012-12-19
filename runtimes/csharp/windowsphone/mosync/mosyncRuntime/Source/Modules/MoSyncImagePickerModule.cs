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
 * @file MoSyncImagePickerModule.cs
 * @author Spiridon Alexandru
 *
 * @brief This represents the ImagePicker implementation for the NativeUI
 *        component on Windows Phone 7, language C#.
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
using Microsoft.Phone.Tasks;
using System.Windows.Media.Imaging;
using System.IO;
using System.Runtime.InteropServices;

namespace MoSync
{
    public class ImagePickerModule : IIoctlModule
    {
        PhotoChooserTask mTask;
        // variable used to check if the photo chooser is open. If it is, we shouldn't call the
        // "Show()" method again
        bool photoTaskStarted = false;
        MoSync.Runtime runtimeReference;

        // by default, the image picker will return a handle to the image
        int eventType = MoSync.Constants.MA_IMAGE_PICKER_EVENT_RETURN_TYPE_IMAGE_HANDLE;

        public void Init(Ioctls ioctls, Core core, Runtime runtime)
        {
            /**
		     * Displays an image picker to the user.
		     * It's display depends on the platform.
		     * Note: when a selection is made a #EVENT_TYPE_IMAGE_PICKER event is sent.
		     */
            ioctls.maImagePickerOpen = delegate()
            {
                // by default, the image picker will return a handle to the image
                eventType = MoSync.Constants.MA_IMAGE_PICKER_EVENT_RETURN_TYPE_IMAGE_HANDLE;

                // we need to keep a reference of the runtime in order to add image resources
                // and send the ImagePicker event
                runtimeReference = runtime;

                return OpenImagePicker();
            };

            /**
		     * Sets the event type that the image picker will return after choosing an image.
		     * \param eventType One of the next constants:
		     * - #MA_IMAGE_PICKER_EVENT_TYPE_IMAGE_HANDLE
		     * - #MA_IMAGE_PICKER_EVENT_TYPE_IMAGE_DATA
		     */
            ioctls.maImagePickerOpenWithEventReturnType = delegate(int eventType)
            {
                // we need to keep a reference of the runtime in order to add image resources
                // and send the ImagePicker event
                runtimeReference = runtime;

                if (eventType == MoSync.Constants.MA_IMAGE_PICKER_EVENT_RETURN_TYPE_IMAGE_DATA ||
                    eventType == MoSync.Constants.MA_IMAGE_PICKER_EVENT_RETURN_TYPE_IMAGE_HANDLE)
                {
                    this.eventType = eventType;

                    return OpenImagePicker();
                }

                return MoSync.Constants.MAW_RES_ERROR;
            };
        }

        /**
         * Opens the PhotoChooserTask and sets its completed event handler.
         */
        private int OpenImagePicker()
        {
            mTask = new PhotoChooserTask();
            // a small camera icon will appear in the page application bar - this enables
            // the user to go to the camera and capture another image (which could be selected
            // by using the image picker afterwards)
            mTask.ShowCamera = true;
            mTask.Completed += PhotoChooserTaskCompleted;
            if (!photoTaskStarted)
            {
                mTask.Show();
            }
            photoTaskStarted = true;

            return MoSync.Constants.MAW_RES_OK;
        }

        /*
         * Occurs when a Chooser task is completed.
         * @param sender The object that triggered the event.
         * @param e The result containing the image selected by the user.
         */
        private void PhotoChooserTaskCompleted(object sender, PhotoResult e)
        {
            photoTaskStarted = false;
            Memory eventData = new Memory(16);
            const int MAWidgetEventData_eventType = 0;
            const int MAWidgetEventData_imagePickerState = 4;
            const int MAWidgetEventData_imageHandle = 8;
            const int MAWidgetEventData_imageEncoding = 12;
            eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.EVENT_TYPE_IMAGE_PICKER);

            if (e.TaskResult == TaskResult.OK)
            {
                // the user has selected an image -> the image picker state will be 1 - ok
                eventData.WriteInt32(MAWidgetEventData_imagePickerState, 1);

                // get a bitmap of the image selected by the user using the Image Picker
                BitmapImage img = new BitmapImage();
                img.SetSource(e.ChosenPhoto);
                switch (eventType)
                {
                    case MoSync.Constants.MA_IMAGE_PICKER_EVENT_RETURN_TYPE_IMAGE_DATA:
                        // We create a memory stream that will contain the bitmap image
                        // with jpeg encoding
                        MemoryStream memoryStream = new MemoryStream();
                        WriteableBitmap writableBitmap = new WriteableBitmap(img);
                        writableBitmap.SaveJpeg(memoryStream, img.PixelWidth, img.PixelHeight, 0, 100);

                        eventData.WriteInt32(MAWidgetEventData_imageHandle, runtimeReference.AddResource(
                            new Resource(memoryStream, MoSync.Constants.RT_BINARY, true)));
                        break;
                    default:
                        // The AddResource returns an int - the image handle that's sent with the event data
                        eventData.WriteInt32(MAWidgetEventData_imageHandle, runtimeReference.AddResource(
                            new Resource(img, MoSync.Constants.RT_IMAGE)));
                        break;
                }

                eventData.WriteInt32(MAWidgetEventData_imageEncoding, MoSync.Constants.MA_IMAGE_PICKER_ITEM_ENCODING_JPEG);
            }
            else
            {
                // the user has canceled the image selection -> the image picker state will be 0 - cancel
                eventData.WriteInt32(MAWidgetEventData_imagePickerState, 0);
            }

            runtimeReference.PostEvent(new Event(eventData));
        }
    }
}
