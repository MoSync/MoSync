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
 * @file ProgressBar.h
 * @author Bogdan Iusco
 *
 * \brief An instance of ProgressBar is used for displaying the progress of a
 * task over time.
 */

#ifndef NATIVEUI_PROGRESS_BAR_H_
#define NATIVEUI_PROGRESS_BAR_H_

#include "Widget.h"

namespace NativeUI
{
    /**
     * \brief An instance of ProgressBar is used for displaying the progress of
     * a task over time.
     */
    class ProgressBar : public Widget
    {
    public:
        /**
         * Constructor.
         */
        ProgressBar();

        /**
         * Destructor.
         */
        virtual ~ProgressBar();

        /**
         * Set the upper range of the progress bar.
         * @param maximumValue An positive integer that specifies the
         * upper range.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the maximumValue value
         *   was invalid.
         */
        virtual int setMaximumValue(const int maximumValue);

        /**
         * Get the upper range of the progress bar.
         * @return The upper range of the progress bar.
         */
        virtual int getMaximumValue();

        /**
         * Set the current progress value to the progress bar.
         * @param progressValue An integer that specifies the new progress,
         * between 0 and maximum value.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the progressValue value
         *   was invalid.
         */
        virtual int setProgress(const int progressValue);

        /**
         * Get the current progress value to the progress bar.
         * @return The current progress value.
         */
        virtual int getProgress();

        /**
         * Increase the progress bar's progress value with the specified amount.
         * @param increaseValue An positive integer that specifies the amount
         * by which the progress should be increased.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the increaseValue value
         *   was invalid.
         */
        virtual int increaseProgress(const int increaseValue);

    };

} // namespace NativeUI

#endif /* NATIVEUI_PROGRESS_BAR_H_ */

/*! @} */
