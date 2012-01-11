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
 * @brief This represents the VideoView Widget implementation for the NativeUI
 *        component on Windows Phone 7, language C#
 *
 * @platform WP 7.1
 **/

using System;
using System.Net;
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
        public class MoSyncVideoView : WidgetBaseWindowsPhone
        {
            private MediaElement mMediaElement;
            private FrameworkElement mSeekControl;
            private DispatcherTimer mProgressTimer;


            /**
             * Constructor
             */
            public MoSyncVideoView()
            {
                mMediaElement = new MediaElement();

                mMediaElement.Stretch = Stretch.Fill;
                mMediaElement.Volume = 10;
                mMediaElement.Source = new Uri("http://qthttp.apple.com.edgesuite.net/1010qwoeiuryfg/sl.m3u8");

                //this.progressTimer = new DispatcherTimer();
                //this.progressTimer.Interval = TimeSpan.FromSeconds(1);
                //this.progressTimer.Tick += new EventHandler(this.ProgressTimer_Tick);

                //SetCurrentPosition();

                // Play Video
                mMediaElement.Play();
                //progressTimer.Start();
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
                // set video
                try
                {
                    mMediaElement.Source = new Uri(myURI);
                }
                catch (ArgumentNullException /*e*/)
                {
                    mMediaElement.Source = null;
                }
                catch (UriFormatException /*e*/)
                {
                    mMediaElement.Source = null;
                }

                // Stop Progress Timer
                if (mProgressTimer != null)
                {
                    if (mProgressTimer.IsEnabled)
                    {
                        mProgressTimer.Stop();
                    }
                }
            }

            // play the video
            private void playVideo()
            {
                mMediaElement.Play();
            }

            // pause the video
            private void pauseVideo()
            {
                mMediaElement.Pause();
            }

            // stop the video
            private void stopVideo()
            {
                mMediaElement.Stop();
            }

        }
    } // end of namespace NativeUI
} // end of namespace MoSync
