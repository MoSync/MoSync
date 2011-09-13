/*
Copyright (C) 2011 MoSync AB

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
 * @file VideoView.h
 * @author Bogdan Iusco
 *
 * An instance of VideoView is used for managing the playback of a movie
 * from a file or a network stream.
 * For video view events see VideoViewListener.
 */

#ifndef NATIVEUI_VIDEO_VIEW_H_
#define NATIVEUI_VIDEO_VIEW_H_

#include "Widget.h"

namespace NativeUI
{

    // Forward declaration.
    class VideoViewListener;

    /**
     * An instance of VideoView is used for managing the playback of a movie
     * from a file or a network stream.
     * For video view events see VideoViewListener.
     */
    class VideoView : public Widget
    {
    public:
        /**
         * Constructor.
         */
        VideoView();

        /**
         * Destructor.
         */
        virtual ~VideoView();

        /**
         * Set the video path.
         * Note: available only for Android.
         * @param path A valid path to a video file.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the path value was invalid.
         */
        virtual int setPath(const MAUtil::String& path);

        /**
         * Set the video url.
         * @param url A valid url to a video file.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the url value was invalid.
         */
        virtual int setURL(const MAUtil::String& url);

        /**
         * Play the video.
         * A valid path or url must be set before calling this method.
         */
        virtual void play();

        /**
         * Pause the video.
         */
        virtual void pause();

        /**
         * Stop the video.
         */
        virtual void stop();

        /**
         * Get the video duration.
         * On Android: this value is known only after
         *     MAW_VIDEO_WIDGET_STATE_SOURCE_READY event is received.
         * On iOS: this value can be retrieved after
         *     MAW_VIDEO_STATE_PLAYING event is received.
         * @return The video's duration in seconds.
         */
        virtual int getDuration();

        /**
         * Seek into the video.
         * @param time Valid time in milliseconds.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the time value was invalid.
         */
        virtual int seekTo(const int time);

        /**
         * Get the buffer percentage of the played video file.
         * Only for Android platform.
         * @return The buffer percentage of the played video file.
         */
        virtual int getBufferPercentage() const;

        /**
         * Get the current video time.
         * @return The current video time.
         */
        virtual int currentPlaybackTime() const;

        /**
         * Add an video view event listener.
         * @param listener The listener that will receive video view events.
         */
        virtual void addVideoViewListener(VideoViewListener* listener);

        /**
         * Remove the video view listener.
         * @param listener The listener that receives video view events.
         */
        virtual void removeVideoViewListener(VideoViewListener* listener);

    protected:
        /**
         * This method is called when there is an event for this widget.
         * It passes on the event to all widget's listeners.
         * @param widgetEventData The data for the widget event.
         */
        virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData);

    private:
        /**
         * Array with video view listeners.
         */
        MAUtil::Vector<VideoViewListener*> mVideoViewListeners;
    };

} // namespace NativeUI

#endif /* NATIVEUI_VIDEO_VIEW_H_ */
