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
 * @author ovidel and Spiridon Alexandru
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
         * VideoView class defines the attributes and behavior of a "video player" without any default
         * visual interface.
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
                // called when the movie has finished the playback
                mMediaElement.MediaEnded += new RoutedEventHandler(
                    delegate(object from, RoutedEventArgs args)
                    {
                        // post the event to MoSync runtime
                        Memory eventData = new Memory(12);
                        // set the main event type: MAW_EVENT_VIDEO_STATE_CHANGED
                        const int MAWidgetEventData_widgetEventType = 0;
                        const int MAWidgetEventData_widgetHandle = 4;
                        // set the banner event type: MAW_VIDEO_VIEW_STATE_FINISHED
                        const int MAWidgetEventData_eventType = 8;
                        eventData.WriteInt32(MAWidgetEventData_widgetEventType, MoSync.Constants.MAW_EVENT_VIDEO_STATE_CHANGED);
                        eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                        eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_VIDEO_VIEW_STATE_FINISHED);
                        mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                    }
                ); // end of mMediaElement.MediaEnded

                // MAW_VIDEO_VIEW_STATE_SOURCE_READY
                // called when the media stream has been validated and opened, and the file headers have been read
                mMediaElement.MediaOpened += new RoutedEventHandler(
                   delegate(object from, RoutedEventArgs args)
                   {
                       Memory eventData = new Memory(12);
                       // set the main event type: MAW_EVENT_VIDEO_STATE_CHANGED
                       const int MAWidgetEventData_widgetEventType = 0;
                       const int MAWidgetEventData_widgetHandle = 4;
                       // set the banner event type: MAW_VIDEO_VIEW_STATE_SOURCE_READY
                       const int MAWidgetEventData_eventType = 8;
                       eventData.WriteInt32(MAWidgetEventData_widgetEventType, MoSync.Constants.MAW_EVENT_VIDEO_STATE_CHANGED);
                       eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                       eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_VIDEO_VIEW_STATE_SOURCE_READY);
                       mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                   }
               ); // end of mMediaElement.MediaOpened

                // MAW_VIDEO_VIEW_STATE_INTERRUPTED
                // called when there is an error associated with the media source.
                mMediaElement.MediaFailed += new EventHandler<ExceptionRoutedEventArgs>(
                   delegate(object from, ExceptionRoutedEventArgs args)
                   {
                       // post the event to MoSync runtime
                       Memory eventData = new Memory(12);
                       // set the main event type: MAW_EVENT_VIDEO_STATE_CHANGED
                       const int MAWidgetEventData_widgetEventType = 0;
                       const int MAWidgetEventData_widgetHandle = 4;
                       // set the banner event type: MAW_VIDEO_VIEW_STATE_INTERRUPTED
                       const int MAWidgetEventData_eventType = 8;
                       eventData.WriteInt32(MAWidgetEventData_widgetEventType, MoSync.Constants.MAW_EVENT_VIDEO_STATE_CHANGED);
                       eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                       eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_VIDEO_VIEW_STATE_INTERRUPTED);
                       mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                   }
               ); // end of mMediaElement.MediaFailed
            }

            /**
             * Property for setting the path to a local video file.
             * set: a String containing the path to the video file.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_VIDEO_VIEW_PATH)]
            public String Path
            {
                set
                {
                    Uri mediaUri;
                    // we try to create the uri from the string passed to the setter
                    if (Uri.TryCreate(value, UriKind.Relative, out mediaUri))
                    {
                        mMediaElement.Source = mediaUri;
                    }
                }
            }

            /**
             * Property for setting the url to a video file.
             * set: a String containing the url to the video file.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_VIDEO_VIEW_URL)]
            public String Url
            {
                set
                {
                    Uri mediaUri;
                    // we try to create the uri from the string passed to the setter
                    if (Uri.TryCreate(value, UriKind.Absolute, out mediaUri))
                    {
                        mMediaElement.Source = mediaUri;
                    }
                }
            }

            /**
             * Property for controlling the playback of a video file.
             * set: an int describing the action to be taken.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_VIDEO_VIEW_ACTION)]
            public int Action
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
             * set: an int representing the milliseconds from the beginning of the video.
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_VIDEO_VIEW_SEEK_TO)]
            public int SeekTo
            {
                set
                {
                    // go to desired position if seeking is possible and if
                    // the current position to be set is valid (is not greater than the duration of the media)
                    if (mMediaElement.CanSeek && value < mMediaElement.NaturalDuration.TimeSpan.TotalMilliseconds)
                    {
                        mMediaElement.Position = TimeSpan.FromMilliseconds(value);
                    }
                }
            }

            /**
             * Property for getting the duration of the current video
             * get: an int representing video's duration in milliseconds
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_VIDEO_VIEW_DURATION)]
            public int Duration
            {
                get
                {
                    int myDuration = 0;
                    if (null != mMediaElement.Source)
                    {
                        // get duration and then transform it into milliseconds
                        Duration currentMediaDuration = mMediaElement.NaturalDuration;
                        myDuration = (int)currentMediaDuration.TimeSpan.TotalMilliseconds;
                    }

                    return myDuration;
                }
            }

            /**
             * Property for getting the buffer percentage of the current video
             * get: an int representing video's buffered percentage
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_VIDEO_VIEW_BUFFER_PERCENTAGE)]
            public int BufferPercentage
            {
                get
                {
                    // the BufferingProgress returns a double value from the interval [0,1]
                    // by multiplying this value with 100 we get the buffered percentage
                    int bufferPercentage = (int)(mMediaElement.BufferingProgress * 100);
                    return bufferPercentage;
                }
            }

            /**
             * Property for getting the current position in time of the current video
             * get: an int representing video's current position in time (measured in seconds)
             */
            [MoSyncWidgetProperty(MoSync.Constants.MAW_VIDEO_VIEW_CURRENT_POSITION)]
            public int CurrentPosition
            {
                get
                {
                    // get the current position in seconds
                    int seconds = (int)mMediaElement.Position.TotalSeconds;
                    return seconds;
                }
            }

            // play the video and MAW_VIDEO_VIEW_STATE_PLAYING
            private void playVideo()
            {
                // play it
                mMediaElement.Play();

                // post the event to MoSync runtime
                Memory eventData = new Memory(12);
                // set the main event type: MAW_EVENT_VIDEO_STATE_CHANGED
                const int MAWidgetEventData_widgetEventType = 0;
                const int MAWidgetEventData_widgetHandle = 4;
                // set the banner event type: MAW_VIDEO_VIEW_STATE_PLAYING
                const int MAWidgetEventData_eventType = 8;
                eventData.WriteInt32(MAWidgetEventData_widgetEventType, MoSync.Constants.MAW_EVENT_VIDEO_STATE_CHANGED);
                eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_VIDEO_VIEW_STATE_PLAYING);
                mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
            }

            // pause the video and MAW_VIDEO_VIEW_STATE_PAUSED
            private void pauseVideo()
            {
                if (mMediaElement.CanPause)
                {
                    // tea break
                    mMediaElement.Pause();

                    // post the event to MoSync runtime
                    Memory eventData = new Memory(12);
                    // set the main event type: MAW_EVENT_VIDEO_STATE_CHANGED
                    const int MAWidgetEventData_widgetEventType = 0;
                    const int MAWidgetEventData_widgetHandle = 4;
                    // set the banner event type: MAW_VIDEO_VIEW_STATE_PAUSED
                    const int MAWidgetEventData_eventType = 8;
                    eventData.WriteInt32(MAWidgetEventData_widgetEventType, MoSync.Constants.MAW_EVENT_VIDEO_STATE_CHANGED);
                    eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                    eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_VIDEO_VIEW_STATE_PAUSED);
                    mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                }
            }

            // stop the video and MAW_VIDEO_VIEW_STATE_STOPPED
            private void stopVideo()
            {
                // stop it
                mMediaElement.Stop();

                // post the event to MoSync runtime
                Memory eventData = new Memory(12);
                // set the main event type: MAW_EVENT_VIDEO_STATE_CHANGED
                const int MAWidgetEventData_widgetEventType = 0;
                const int MAWidgetEventData_widgetHandle = 4;
                // set the banner event type: MAW_VIDEO_VIEW_STATE_STOPPED
                const int MAWidgetEventData_eventType = 8;
                eventData.WriteInt32(MAWidgetEventData_widgetEventType, MoSync.Constants.MAW_EVENT_VIDEO_STATE_CHANGED);
                eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_VIDEO_VIEW_STATE_STOPPED);
                mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
            }
        } // end of class VideoView
    } // end of namespace NativeUI
} // end of namespace MoSync
