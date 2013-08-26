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

/**
 * @file ProgressBar.cpp
 * @author Bogdan Iusco
 *
 * An instance of ProgressBar is used for displaying the progress of a task
 * over time.
 */

#include "ProgressBar.h"

namespace NativeUI
{
    /**
     * Constructor.
     */
    ProgressBar::ProgressBar(): Widget(MAW_PROGRESS_BAR)
    {
    }

    /**
     * Destructor.
     */
    ProgressBar::~ProgressBar()
     {
     }

    /**
     * Set the upper range of the progress bar.
     * @param maximumValue An positive integer that specifies the
     * upper range.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the maximumValue value
     *   was invalid.
     */
    int ProgressBar::setMaximumValue(const int maximumValue)
    {
        return this->setPropertyInt(MAW_PROGRESS_BAR_MAX, maximumValue);
    }

    /**
     * Get the upper range of the progress bar.
     * @return The upper range of the progress bar.
     */
    int ProgressBar::getMaximumValue()
    {
        return this->getPropertyInt(MAW_PROGRESS_BAR_MAX);
    }

    /**
     * Set the current progress value to the progress bar.
     * @param progressValue An integer that specifies the new progress,
     * between 0 and maximum value.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the progressValue value
     *   was invalid.
     */
    int ProgressBar::setProgress(const int progressValue)
    {
        return this->setPropertyInt(MAW_PROGRESS_BAR_PROGRESS, progressValue);
    }

    /**
     * Get the current progress value to the progress bar.
     * @return The current progress value.
     */
    int ProgressBar::getProgress()
    {
        return this->getPropertyInt(MAW_PROGRESS_BAR_PROGRESS);
    }

    /**
     * Increase the progress bar's progress value with the specified amount.
     * @param increaseValue An positive integer that specifies the amount
     * by which the progress should be increased.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the increaseValue value
     *   was invalid.
     */
    int ProgressBar::increaseProgress(const int increaseValue)
    {
        return this->setPropertyInt(
            MAW_PROGRESS_BAR_INCREMENT_PROGRESS,
            increaseValue);
    }

} // namespace NativeUI
