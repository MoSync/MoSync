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
 * @file Button.cpp
 * @author Bogdan Iusco
 *
 * Class for buttons.
 * A button intercepts touch events and notifies the observers.
 * Methods for setting the text, alignment, font size and color are inherited
 * from TextWidget.
 */

#include "Button.h"
#include "ButtonListener.h"

namespace NativeUI
{
    /**
     * Constructor.
     */
    Button::Button(): TextWidget(MAW_BUTTON)
    {
        this->setText("Click Me!");
    }

    /**
     * Destructor.
     */
     Button::~Button()
     {
         mButtonListeners.clear();
     }

     /**
      * Set the text displayed by the widget.
      * @param text The given text.
      */
     void Button::setText(const MAUtil::String& text)
     {
         this->setProperty(MAW_BUTTON_TEXT, text);
     }

     /**
      * Get the text displayed by the widget.
      * @return The text displayed by the widget.
      */
      MAUtil::String Button::getText()
      {
          return this->getPropertyString(MAW_BUTTON_TEXT);
      }

     /**
      * Add an button event listener.
      * @param listener The listener that will receive button events.
      */
     void Button::addButtonListener(ButtonListener* listener)
     {
         addListenerToVector(mButtonListeners, listener);
     }

     /**
      * Remove the button listener.
      * @param listener The listener that receives button events.
      */
     void Button::removeButtonListener(ButtonListener* listener)
     {
         removeListenerFromVector(mButtonListeners, listener);
     }

     /**
      * This method is called when there is an event for this widget.
      * It passes on the event to all widget's listeners.
      * @param widgetEventData The data for the widget event.
      */
     void Button::handleWidgetEvent(MAWidgetEventData* widgetEventData)
     {
         Widget::handleWidgetEvent(widgetEventData);

         if (MAW_EVENT_POINTER_PRESSED == widgetEventData->eventType)
         {
             for (int i = 0; i < mButtonListeners.size(); i++)
             {
                 mButtonListeners[i]->buttonPressed(this);
             }
         }
         else if (MAW_EVENT_POINTER_RELEASED == widgetEventData->eventType)
         {
             for (int i = 0; i < mButtonListeners.size(); i++)
             {
                 mButtonListeners[i]->buttonReleased(this);
             }
         }
         else if (MAW_EVENT_CLICKED == widgetEventData->eventType)
         {
             for (int i = 0; i < mButtonListeners.size(); i++)
             {
                 mButtonListeners[i]->buttonClicked(this);
             }
         }
     }

} // namespace NativeUI
