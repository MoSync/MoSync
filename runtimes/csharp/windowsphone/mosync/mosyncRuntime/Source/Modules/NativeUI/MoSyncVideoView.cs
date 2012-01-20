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
 * @file MoSyncVideoView.cs
 * @author ovidel
 *
 * @brief VideoView Widget implementation for the NativeUI
 *        component on Windows Phone 7, language C#
 *
 * @platform WP 7.1
 **/

using System;
using System.Net;
using System.ComponentModel;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Threading;
using System.Windows.Media;
using System.Windows.Shapes;

namespace MoSync
{
    namespace NativeUI
    {

        /**
        * VideoView class defines the attributes and behavior of a "video player"
        */
        public class VideoView : WidgetBaseWindowsPhone
        {
            private MediaElement mMediaElement;

            /**
             * Constructor
             */
            public VideoView()
            {
                mMediaElement = new MediaElement();

                mView = mMediaElement;

                /**
                 * the delegates that respond to the widget's events
                 */

                // MAW_VIDEO_VIEW_STATE_FINISHED
                mMediaElement.MediaEnded += new RoutedEventHandler(
                    delegate(object from, RoutedEventArgs args)
                    {
                        // post the event to MoSync runtime
                        Memory eventData = new Memory(8);
                        const int MAWidgetEventData_eventType = 0;
                        const int MAWidgetEventData_widgetHandle = 4;
                        eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_VIDEO_VIEW_STATE_FINISHED);
                        eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                        mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                    }
                ); // end of mMediaElement.MediaEnded

                 // MAW_VIDEO_VIEW_STATE_SOURCE_READY
                 mMediaElement.MediaOpened += new RoutedEventHandler(
                    delegate(object from, RoutedEventArgs args)
                    {
                        // post the event to MoSync runtime
                        Memory eventData = new Memory(8);
                        const int MAWidgetEventData_eventType = 0;
                        const int MAWidgetEventData_widgetHandle = 4;
                        eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_VIDEO_VIEW_STATE_SOURCE_READY);
                        eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                        mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                    }
                ); // end of mMediaElement.MediaOpened

                 // MAW_VIDEO_VIEW_STATE_INTERRUPTED
                 mMediaElement.MediaFailed += new EventHandler<ExceptionRoutedEventArgs>(
                    delegate(object from, ExceptionRoutedEventArgs args)
                    {
                        // post the event to MoSync runtime
                        Memory eventData = new Memory(8);
                        const int MAWidgetEventData_eventType = 0;
                        const int MAWidgetEventData_widgetHandle = 4;
                        eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_VIDEO_VIEW_STATE_INTERRUPTED);
                        eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                        mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                    }
                ); // end of mMediaElement.MediaFailed
            }


            /**
             * Property for setting the url to a video file.
             * set: a String containing the url to the video file.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_VIDEO_VIEW_URL)]
            public String url
            {
                set
                {
                    setURI(value);
                }
            }


            /**
             * Property for setting the path to a local video file.
             * set: a String containing the path to the video file.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_VIDEO_VIEW_PATH)]
            public String path
            {
                set
                {
                    setURI(value);
                }
            }



            /**
             * Property for controlling the playback of a video file.
             * set: an int describing the action to be taken.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_VIDEO_VIEW_ACTION)]
            public int action
            {
                set
                {
                    switch (value)
                    {
                        case MoSync.Constants.MAW_VIDEO_VIEW_ACTION_PLAY:
                            playVideo();
                            break;
                        case MoSync.Constants.MAW_VIDEO_VIEW_ACTION_STOP:
                            stopVideo();
                            break;
                        case MoSync.Constants.MAW_VIDEO_VIEW_ACTION_PAUSE:
                            pauseVideo();
                            break;
                    }
                }
            }


            /**
             * Property for moving the playing head.
             * set: an int representing the milliseconds from the beggining of the video.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_VIDEO_VIEW_SEEK_TO)]
            public int seekTo
            {
                set
                {
                    // go to desired position
                }
            }


            /**
             * Property for getting the duration of the current video
             * get: an int representing video's duration in seconds
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_VIDEO_VIEW_DURATION)]
            public int duration
            {
                get
                {
                    int myDuration = 0;
                    if (null != mMediaElement.Source)
                    {
                        // get duration
                    }

                    return myDuration;
                }
            }


            /**
             * Property for setting the action type to be appliede .
             * set: a String containing the path to the video file.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_VIDEO_VIEW_PATH)]
            public String Path
            {
                set
                {
                    setURI(value);
                }
            }


            // helper method for setting the video's URI
            private void setURI(String myURI)
            {
                // set video here
            }

            // play the video and MAW_VIDEO_VIEW_STATE_PLAYING
            private void playVideo()
            {
                // play it
                mMediaElement.Play();

                // post the playing event
                Memory eventData = new Memory(8);
                const int MAWidgetEventData_eventType = 0;
                const int MAWidgetEventData_widgetHandle = 4;
                eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_VIDEO_VIEW_STATE_PLAYING);
                eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
            }

            // pause the video and MAW_VIDEO_VIEW_STATE_PAUSED
            private void pauseVideo()
            {
                // tea break
                mMediaElement.Pause();

                // announce the event
                Memory eventData = new Memory(8);
                const int MAWidgetEventData_eventType = 0;
                const int MAWidgetEventData_widgetHandle = 4;
                eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_VIDEO_VIEW_STATE_PAUSED);
                eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
            }

            // stop the video and MAW_VIDEO_VIEW_STATE_STOPPED
            private void stopVideo()
            {
                // stop it
                mMediaElement.Stop();

                // announce the event
                Memory eventData = new Memory(8);
                const int MAWidgetEventData_eventType = 0;
                const int MAWidgetEventData_widgetHandle = 4;
                eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_VIDEO_VIEW_STATE_STOPPED);
                eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
            }

            private Uri _SelectedVideoProperty;
            public Uri SelectedVideoProperty
            {
                get
                {
                    return this._SelectedVideoProperty;
                }
                set
                {
                    this._SelectedVideoProperty = value;
                    this.NotifyPropertyChanged("SelectedVideoProperty");
                }
            }


            // Utility

            public event PropertyChangedEventHandler PropertyChanged;

            private void NotifyPropertyChanged(String info)
            {
                if (PropertyChanged != null)
                {
                    PropertyChanged(this, new PropertyChangedEventArgs(info));
                }
            }

        } // end of class VideoView
    } // end of namespace NativeUI
} // end of namespace MoSync
