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
 * @file ListView.h
 * @author Bogdan Iusco
 *
 * \brief An instance of ListView is used for displaying list of information.
 * For list view events see ListViewListener.
 */

#ifndef NATIVEUI_LIST_VIEW_H_
#define NATIVEUI_LIST_VIEW_H_

#include "Widget.h"

namespace NativeUI
{

    // Forward declaration.
    class ListViewListener;

    /**
     * \brief An instance of ListView is used for displaying list of information.
     *
     * For list view events see ListViewListener.
     */
    class ListView : public Widget
    {
    public:
        /**
         * Constructor.
         */
        ListView();

        /**
         * Destructor.
         */
        virtual ~ListView();

        /**
         * Add an list view event listener.
         * @param listener The listener that will receive list view events.
         */
        virtual void addListViewListener(ListViewListener* listener);

        /**
         * Remove the list view listener.
         * @param listener The listener that receives list view events.
         */
        virtual void removeListViewListener(ListViewListener* listener);

    protected:
        /**
         * This method is called when there is an event for this widget.
         * It passes on the event to all widget's listeners.
         * @param widgetEventData The data for the widget event.
         */
        virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData);

    private:
        /**
         * Array with list view listeners.
         */
        MAUtil::Vector<ListViewListener*> mListViewListeners;
    };

} // namespace NativeUI

#endif /* NATIVEUI_LIST_VIEW_H_ */
