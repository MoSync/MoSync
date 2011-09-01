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
 * @file VideoView.cpp
 * @author Bogdan Iusco
 *
 * An instance of VideoView is used for managing the playback of a movie
 * from a file or a network stream.
 * For video view events see VideoViewListener.
 */

#include "VideoView.h"
#include "VideoViewListener.h"

namespace NativeUI
{
    /**
     * Constructor.
     */
    VideoView::VideoView(): Widget(MAW_VIDEO_VIEW)
    {
    }

    /**
     * Destructor.
     */
    VideoView::~VideoView()
     {
        mVideoViewListeners.clear();
     }

    /**
     * Set the video path.
     * Note: available only for Android.
     * @param path A valid path to a video file.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the path value was invalid.
     */
    int VideoView::setPath(const MAUtil::String& path)
    {
        return this->setProperty(MAW_VIDEO_VIEW_PATH, path);
    }

    /**
     * Set the video url.
     * @param url A valid url to a video file.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the url value was invalid.
     */
    int VideoView::setURL(const MAUtil::String& url)
    {
        return this->setProperty(MAW_VIDEO_VIEW_URL, url);
    }

    /**
     * Play the video.
     * A valid path or url must be set before calling this method.
     */
    void VideoView::play()
    {
        this->setPropertyInt(
            MAW_VIDEO_VIEW_ACTION,
            MAW_VIDEO_VIEW_ACTION_PLAY);
    }

    /**
     * Pause the video.
     */
    void VideoView::pause()
    {
        this->setPropertyInt(
            MAW_VIDEO_VIEW_ACTION,
            MAW_VIDEO_VIEW_ACTION_PAUSE);
    }

    /**
     * Stop the video.
     */
    void VideoView::stop()
    {
        this->setPropertyInt(
            MAW_VIDEO_VIEW_ACTION,
            MAW_VIDEO_VIEW_ACTION_STOP);
    }

    /**
     * Get the video duration.
     * For an video url this value is known only after receiving
     * MAW_VIDEO_WIDGET_STATE_SOURCE_READY event is received.
     * @return The video's duration in seconds.
     */
    int VideoView::getDuration()
    {
        return this->getPropertyInt(MAW_VIDEO_VIEW_DURATION);
    }

    /**
     * Seek into the video.
     * @param time Valid time in milliseconds.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the time value was invalid.
     */
    int VideoView::seekTo(const int time)
    {
        return this->setPropertyInt(MAW_VIDEO_VIEW_SEEK_TO, time);
    }

    /**
     * Get the buffer percentage of the played video file.
     * Only for Android platform.
     * @return The buffer percentage of the played video file.
     */
    int VideoView::getBufferPercentage() const
    {
        return this->getPropertyInt(MAW_VIDEO_VIEW_BUFFER_PERCENTAGE);
    }

    /**
     * Get the current video time.
     * @return The current video time.
     */
    int VideoView::currentPlaybackTime() const
    {
        return this->getPropertyInt(MAW_VIDEO_VIEW_CURRENT_POSITION);
    }

    /**
     * Add an video view event listener.
     * @param listener The listener that will receive video view events.
     */
    void VideoView::addVideoViewListener(VideoViewListener* listener)
    {
        addListenerToVector(mVideoViewListeners, listener);
    }

    /**
     * Remove the video view listener.
     * @param listener The listener that receives video view events.
     */
    void VideoView::removeVideoViewListener(VideoViewListener* listener)
    {
        removeListenerFromVector(mVideoViewListeners, listener);
    }

    /**
     * This method is called when there is an event for this widget.
     * It passes on the event to all widget's listeners.
     * @param widgetEventData The data for the widget event.
     */
    void VideoView::handleWidgetEvent(MAWidgetEventData* widgetEventData)
    {
        Widget::handleWidgetEvent(widgetEventData);
        int videoViewState = widgetEventData->videoViewState;

        if (MAW_EVENT_VIDEO_STATE_CHANGED == widgetEventData->eventType)
        {
            for (int i = 0; i < mVideoViewListeners.size(); i++)
            {
                mVideoViewListeners[i]->videoViewStateChanged(
                    this,
                    videoViewState);
            }
        }
    }

} // namespace NativeUI
