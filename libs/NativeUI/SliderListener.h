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
 * @file SliderListener.h
 * @author Bogdan Iusco
 *
 * \brief Listener for Slider events.
 */

#ifndef NATIVEUI_SLIDER_LISTENER_H_
#define NATIVEUI_SLIDER_LISTENER_H_

namespace NativeUI
{
    // Forward declaration.
    class Slider;

    /**
     * \brief Listener for Slider events.
     */
    class SliderListener
    {
    public:
        /**
         * This method is called when the value of the slider was modified by
         * the user.
         * @param slider The slider object that generated the event.
         * @param sliderValue The new slider's value.
         */
        virtual void sliderValueChanged(
            Slider* slider,
            const int sliderValue) = 0;
    };

} // namespace NativeUI

#endif /* NATIVEUI_SLIDER_LISTENER_H_ */
