/* Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
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

