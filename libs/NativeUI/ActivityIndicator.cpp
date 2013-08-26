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
 * @file ActivityIndicator.cpp
 * @author Bogdan Iusco
 *
 * An instance of ActivityIndicator is used for displaying the indeterminate
 * progress of a task over time.
 */

#include "ActivityIndicator.h"

namespace NativeUI
{
    /**
     * Constructor.
     */
    ActivityIndicator::ActivityIndicator(): Widget(MAW_ACTIVITY_INDICATOR)
    {
    }

    /**
     * Destructor.
     */
    ActivityIndicator::~ActivityIndicator()
     {
     }

    /**
     * Show the activity indicator.
     */
    void ActivityIndicator::show()
    {
        this->setProperty(MAW_ACTIVITY_INDICATOR_IN_PROGRESS, "true");
    }

    /**
     * Hide the activity indicator.
     */
    void ActivityIndicator::hide()
    {
        this->setProperty(MAW_ACTIVITY_INDICATOR_IN_PROGRESS, "false");
    }

} // namespace NativeUI
