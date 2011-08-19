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
 * @file ActivityIndicator.h
 * @author Bogdan Iusco
 *
 * An instance of ActivityIndicator is used for displaying the indeterminate
 * progress of a task over time.
 */

#ifndef NATIVEUI_ACTIVITY_INDICATOR_H_
#define NATIVEUI_ACTIVITY_INDICATOR_H_

#include "Widget.h"

namespace NativeUI
{
    /**
     * An instance of ActivityIndicator is used for displaying the indeterminate
     * progress of a task over time.
     */
    class ActivityIndicator : public Widget
    {
    public:
        /**
         * Constructor.
         */
        ActivityIndicator();

        /**
         * Destructor.
         */
        virtual ~ActivityIndicator();

        /**
         * Show the activity indicator.
         */
        virtual void show();

        /**
         * Hide the activity indicator.
         */
        virtual void hide();
    };

} // namespace NativeUI

#endif /* NATIVEUI_ACTIVITY_INDICATOR_H_ */
