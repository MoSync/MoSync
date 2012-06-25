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

/*! \addtogroup NativeUILib
 *  @{
 */

/**
 *  @defgroup NativeUILib Native UI Library
 *  @{
 */

/**
 * @file RadioGroupListener.h
 * @author Emma
 *
 * \brief Listener for RadioGroup events.
 */

#ifndef NATIVEUI_RADIO_GROUP_LISTENER_H_
#define NATIVEUI_RADIO_GROUP_LISTENER_H_

namespace NativeUI
{
    // Forward declaration.
    class RadioGroup;

    /**
     * \brief Listener for RadioGroup events.
     */
    class RadioGroupListener
    {
    public:
        /**
         * This method is called when a radio button from a radio group is selected.
         * When the selection is cleared, index is -1 and radioButton is null.
         * @param radioGroup The radio group object that generated the event.
         * @param index The index of the selected radio button inside this group.
         * @param radioButton The radio button handle that generated the event.
         */
        virtual void radioButtonSelected(
            RadioGroup* radioGroup,
            int index,
            RadioButton* radioButton) = 0;
    };

} // namespace NativeUI

#endif /* NATIVEUI_RADIO_GROUP_LISTENER_H_ */

/*! @} */
