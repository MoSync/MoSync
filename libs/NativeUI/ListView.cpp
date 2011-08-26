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
 * @file ListView.cpp
 * @author Bogdan Iusco
 *
 * An instance of ListView is used for displaying list of information.
 * For list view events see ListViewListener.
 */

#include "ListView.h"
#include "ListViewListener.h"

namespace NativeUI
{
    /**
     * Constructor.
     */
    ListView::ListView(): Widget(MAW_LIST_VIEW)
    {
        this->fillSpaceHorizontally();
        this->fillSpaceVertically();
    }

    /**
     * Destructor.
     */
     ListView::~ListView()
     {
         mListViewListeners.clear();
     }

    /**
     * Add an list view event listener.
     * @param listener The listener that will receive list view events.
     */
    void ListView::addListViewListener(ListViewListener* listener)
    {
        addListenerToVector(mListViewListeners, listener);
    }

    /**
     * Remove the list view listener.
     * @param listener The listener that receives list view events.
     */
    void ListView::removeListViewListener(ListViewListener* listener)
    {
        removeListenerFromVector(mListViewListeners, listener);
    }

     /**
      * This method is called when there is an event for this widget.
      * It passes on the event to all widget's listeners.
      * @param widgetEventData The data for the widget event.
      */
    void ListView::handleWidgetEvent(MAWidgetEventData* widgetEventData)
    {
        Widget::handleWidgetEvent(widgetEventData);

        if (MAW_EVENT_ITEM_CLICKED == widgetEventData->eventType)
        {
            int itemClickedIndex = widgetEventData->listItemIndex;
            ListViewItem* listViewItem = (ListViewItem*)
                this->getChild(itemClickedIndex);

            for (int i = 0; i < mListViewListeners.size(); i++)
            {
                mListViewListeners[i]->listViewItemClicked(
                    this,
                    listViewItem);
            }
        }
    }

} // namespace NativeUI
