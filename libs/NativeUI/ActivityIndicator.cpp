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
