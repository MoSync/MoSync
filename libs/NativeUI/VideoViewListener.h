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
 * @file VideoViewListener.h
 * @author Bogdan Iusco
 *
 * Interface for video view event listeners.
 *
 */

#ifndef NATIVEUI_VIDEOVIEWLISTENER_H_
#define NATIVEUI_VIDEOVIEWLISTENER_H_

namespace NativeUI
{
    // Forward declaration.
    class VideoView;

    /**
     * Interface for video view event listeners.
     */
    class VideoViewListener
    {
    public:
        /**
         * This method is called when the state of the video has changed.
         * @param videoView The video view object that generated the event.
         * @param videoViewState The new state of the video.
         * Any of the following constants:
         * - #MAW_VIDEO_VIEW_STATE_PLAYING the video is playing.
         * - #MAW_VIDEO_VIEW_STATE_PAUSED the video is paused.
         * - #MAW_VIDEO_VIEW_STATE_STOPPED the video is stopped.
         * - #MAW_VIDEO_VIEW_STATE_SOURCE_READY the source is loaded into memory.
         * - #MAW_VIDEO_VIEW_STATE_FINISHED the video has finished playing.
         * - #MAW_VIDEO_VIEW_STATE_INTERRUPTED Playback is temporarily
         * interruped(maybe there's no data in the buffer).
         */
        virtual void videoViewStateChanged(
            VideoView* videoView,
            const int videoViewState) = 0;
    };

} // namespace NativeUI

#endif /* NATIVEUI_VIDEOVIEWLISTENER_H_ */
