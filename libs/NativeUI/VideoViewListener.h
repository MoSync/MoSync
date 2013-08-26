/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/*! \addtogroup NativeUILib
 *  @{
 */

/**
 *  @defgroup NativeUILib Native UI Library
 *  @{
 */

/**
 * @file VideoViewListener.h
 * @author Bogdan Iusco
 *
 * \brief Interface for video view event listeners.
 *
 */

#ifndef NATIVEUI_VIDEOVIEWLISTENER_H_
#define NATIVEUI_VIDEOVIEWLISTENER_H_

namespace NativeUI
{
    // Forward declaration.
    class VideoView;

    /**
     * \brief Interface for video view event listeners.
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
         * NOTE: there is a slightly different behaviour depending on the platform:
         *  - on iOS the source is loaded into memory when
         *  MAW_VIDEO_VIEW_STATE_PLAYING is received.
         *   - on Android the source is loaded into memory when
         *  MAW_VIDEO_VIEW_STATE_SOURCE_READY is received.
         */
        virtual void videoViewStateChanged(
            VideoView* videoView,
            const int videoViewState) = 0;
    };

} // namespace NativeUI

#endif /* NATIVEUI_VIDEOVIEWLISTENER_H_ */

/*! @} */
