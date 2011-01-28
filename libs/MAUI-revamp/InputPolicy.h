/* Copyright (C) 2010 MoSync AB

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
* \file InputPolicy.h
* \brief Handling of input policies.
* TODO: Document what an input policy is.
* \author Niklas Nummelin
*/

#ifndef _INPUT_POLICY_H_
#define _INPUT_POLICY_H_

#include <maapi.h>

namespace MAUI {
	class Widget;

	/** 
	 * Direction constants.
	 * TODO: DOcument how NONE is used?
	 */
	enum Direction {
			LEFT,
			RIGHT,
			UP,
			DOWN,
			NONE
	};

	/**
	 * Map a keycode to a direction constant.
	 * \param keyCode The keycode.
	 * \return The direction for the keyCode.
	 */
	Direction mapKeyCodeToDirection(int keyCode);

	/**
	 * Class that defines an input policy.
	 * TODO: Describe to the reader of this code what
	 * an input policy is.
	 */
	class InputPolicy {
	public:
		/**
		 * Constructor.
		 * \param owner TODO: Document param.
		 */
		InputPolicy(Widget* owner);
		
		/**
		 * Set the owner of this input policy.
		 * TODO: Document what an owner is.
		 * \param owner TODO: Document param.
		 */
		void setOwner(Widget *owner);
		
		/**
		 * Get the owner of this input policy.
		 * TODO: Document what an owner is.
		 * \return TODO: Document return value.
		 */
		Widget* getOwner();

		/**
		 * TODO: Document the return value, current comment might be wrong.
		 *
		 * Called when a key is pressed. Override in the subclass
		 * that defines the input policy.
		 * \param keyCode The key code for the key.
		 * \param nativeCode The nativekey code for the key.
		 * \return true to keep focus, false if focus should be lost.
		 */
		virtual bool keyPressed(int keyCode, int nativeCode);
		
		/**
		 * Called when a key is released. Override in the subclass
		 * that defines the input policy.
		 * \param keyCode The key code for the key.
		 * \param nativeCode The nativekey code for the key.
		 * \return TODO: Document return value.
		 */
		virtual bool keyReleased(int keyCode, int nativeCode);

		/**
		 * TODO: Document the return value, current comment might be wrong.
		 *
		 * Called when the pointer is pressed. Override in the subclass
		 * that defines the input policy.
		 * \param point The pointer position.
		 * \param id TODO: Document param.
		 * \return true to keep focus, false if focus should be changed.
		 */
		virtual bool pointerPressed(MAPoint2d point, int id);
		
		/**
		 * TODO: Document the return value, current comment might be wrong.
		 *
		 * Called when the pointer is pressed. Override in the subclass
		 * that defines the input policy.
		 * \param point The pointer position.
		 * \param id TODO: Document param.
		 * \return true to keep focus, false if focus should be changed.
		 */
		virtual bool pointerMoved(MAPoint2d point, int id);
		
		/**
		 * TODO: Document the return value, current comment might be wrong.
		 *
		 * Called when the pointer is pressed. Override in the subclass
		 * that defines the input policy.
		 * \param point The pointer position.
		 * \param id TODO: Document param.
		 * \return true to keep focus, false if focus should be changed.
		 */
		virtual bool pointerReleased(MAPoint2d point, int id);

	protected:
		/**
		 * The owner of the widget.
		 */
		Widget *mOwner;
	};

	/**
	 * Class that defined the default input policy.
	 * Delegate events to the owner widget.
	 */
	class DefaultInputPolicy : public InputPolicy {
	public:
		/**
		 * Constructor.
		 * \param owner TODO: Document param.
		 */
		DefaultInputPolicy(Widget* owner);
		
		/**
		 * TODO: Document the return value, current comment might be wrong.
		 *
		 * Called when a key is pressed. Override in the subclass
		 * that defines the input policy.
		 * \param keyCode The key code for the key.
		 * \param nativeCode The nativekey code for the key.
		 * \return true to keep focus, false if focus should be lost.
		 */
		virtual bool keyPressed(int keyCode, int nativeCode);
		
		/**
		 * Called when a key is released. Override in the subclass
		 * that defines the input policy.
		 * \param keyCode The key code for the key.
		 * \param nativeCode The nativekey code for the key.
		 * \return TODO: Document return value.
		 */
		virtual bool keyReleased(int keyCode, int nativeCode);
		
		/**
		 * TODO: Document the return value, current comment might be wrong.
		 *
		 * Called when the pointer is pressed. Override in the subclass
		 * that defines the input policy.
		 * \param point The pointer position.
		 * \param id TODO: Document param.
		 * \return true to keep focus, false if focus should be changed.
		 */
		virtual bool pointerPressed(MAPoint2d p, int id);
		
		/**
		 * TODO: Document the return value, current comment might be wrong.
		 *
		 * Called when the pointer is pressed. Override in the subclass
		 * that defines the input policy.
		 * \param point The pointer position.
		 * \param id TODO: Document param.
		 * \return true to keep focus, false if focus should be changed.
		 */
		virtual bool pointerMoved(MAPoint2d p, int id);
		
		/**
		 * TODO: Document the return value, current comment might be wrong.
		 *
		 * Called when the pointer is pressed. Override in the subclass
		 * that defines the input policy.
		 * \param point The pointer position.
		 * \param id TODO: Document param.
		 * \return true to keep focus, false if focus should be changed.
		 */
		virtual bool pointerReleased(MAPoint2d p, int id);
	};
}

#endif // _INPUT_POLICY_H_
