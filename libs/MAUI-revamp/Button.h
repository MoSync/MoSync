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
* \file Button.h
* \brief Button widget
* \author Patrick Broman and Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_BUTTON_H_
#define _SE_MSAB_MAUI_BUTTON_H_

#include "Label.h"
#include "Style.h"
#include <MAUtil/String.h>
#include <MAUtil/ListenerSet.h>

namespace MAUI {

	/**
	 * Forward declaration of Button
	 */ 
	class Button;
	
	/**
	 * @brief ButtonListener
	 *
	 * The onButtonEvent function which will be called when the button
	 * has been pressed on and when it has been released.
	 */
	class ButtonListener {
	public:
		/**
		 * @brief The button event function
		 *
		 * This function will be notifed when it's button has been pressed
		 * or released.
		 *
		 * @param button	The button which sends the event
		 * @param pressed	If the key was pressed or released
		 */
		virtual void onButtonEvent(Button* button, bool pressed) = 0;
	};

	/**
	 * @brief MAUI button
	 */
	class Button : public Label {
	public:
		/**
		* @brief Constructor
		*
		* @param x 			The x position of the button
		* @param y			The y position of the button
		* @param width		The width of the the button
		* @param height 	The height of the the button
		* @param caption	The caption of the button
		*/
		Button(int x=0, int y=0, int width=0,
				int height=0, const String& caption="");

		bool keyPressed(int keyCode, int nativeCode);
		bool keyReleased(int keyCode, int nativeCode);

		/**
		* Overloaded pointerPressed from Widget
		*/
		bool pointerPressed(MAPoint2d point, int id);
		
		/**
		* Overloaded pointerMoved from Widget
		*/
		bool pointerMoved(MAPoint2d point, int id);
		
		/**
		* Overloaded pointerReleased from Widget
		*/
		bool pointerReleased(MAPoint2d point, int id);

		/**
		* Overloaded isTransparent from Widget
		*/
		virtual bool isTransparent() const;
		
		/**
		* Overloaded setFocused from Widget
		*/
		void setFocused(bool focused=true);

		/**
		* @brief Adds a button listener for this button
		*
		* @param listener	The listener class which will
		*					receive the button events
		*/
		void addButtonListener(ButtonListener* listener);
		
		/**
		* @brief Removes a button listener
		*
		* @param listener	The listener which shall be removed
		*/
		void removeButtonListener(ButtonListener* listener);

	protected:
	
		/**
		* @brief Sets the style properties
		*/
		void restyle();
		
		/**
		* @brief Draws the button
		*/
		void drawWidget();
		
		bool mPressed;
		int mStartX, mStartY;

		/**
		* The skin that will be used when this button is in focus
		* and is pressed
		*/
		Drawable *mSkinFocusedPressed;
		
		/**
		* The skin that will be used when this button is in focus
		* and is not pressed
		*/
		Drawable *mSkinFocusedReleased;
		
		/**
		* The skin that will be used when this button hasn't got focus
		*/
		Drawable *mSkinUnfocusedReleased;

		/**
		* The set of listeners which will receive notifications
		* from this button
		*/
		ListenerSet<ButtonListener> mListeners;
	};

	/**
	 * Available properties:
	 * paddingLeft = IntegerProperty
	 * paddingRight = IntegerProperty
	 * paddingTop = IntegerProperty
	 * paddingBottom = IntegerProperty
	 * marginLeft = IntegerProperty
	 * marginRight = IntegerProperty
	 * marginTop = IntegerProperty
	 * marginBottom = IntegerProperty
	 * backgroundSkinFocused = DrawableProperty
									(may be null, in which it's not used)
	 * backgroundSkinUnfocused = DrawableProperty
									(may be null, in which it's not used)
	 * skinFocusedPressed = DrawableProperty
	 * skinFocusedReleased = DrawableProperty
	 * skinUnfocusedReleased = DrawableProperty
	 */
	class ButtonStyle : public LabelStyle {
	public:
		ButtonStyle(DrawableProperty* focusedPressed,
			DrawableProperty* focusedReleased,
			DrawableProperty* unfocusedReleased,
			FontProperty* font);
	};

}

#endif /* _SE_MSAB_MAUI_BUTTON_H_ */

