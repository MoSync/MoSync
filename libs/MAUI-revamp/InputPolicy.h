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
