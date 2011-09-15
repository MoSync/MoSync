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
 * @file ProgressBar.h
 * @author Bogdan Iusco
 *
 * An instance of ProgressBar is used for displaying the progress of a task
 * over time.
 */

#ifndef NATIVEUI_PROGRESS_BAR_H_
#define NATIVEUI_PROGRESS_BAR_H_

#include "Widget.h"

namespace NativeUI
{
    /**
     * An instance of ProgressBar is used for displaying the progress of a task
     * over time.
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
