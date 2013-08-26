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

/** \file InputManager.h
 * \brief Declaration of classes for input management
 *
 * This file contains the declaration of the classes for input management
 * in MAUI. InputManager is the basic interface used to supply
 * support for different types of input.
 *
 * \author Patrick Broman 
 */

#ifndef _SE_MSAB_MAUI_INPUTMANAGER_H_
#define _SE_MSAB_MAUI_INPUTMANAGER_H_

#include <MAUtil/Vector.h>
#include <MAUtil/RefCounted.h>
#include <MAUtil/String.h>
#include <MAUtil/Geometry.h>
#include "WidgetSkin.h"

namespace MAUI {

	using namespace MAUtil;
	
	class Widget;

/** 
 * \brief Input management for keys and pointers
 */
	
	class InputManager {
	public:
		virtual void keyPressed(int key) = 0;
		virtual void keyReleased(int key) = 0;
		virtual void pointerPressed(int x, int y) = 0;
		virtual void pointerReleased(int x, int y) = 0;
		virtual void pointerMoved(int x, int y) = 0;
	protected:
		Widget* mWidget;
	};
	
/** 
 * \brief Default input management for keys and pointers
 */
	
	class DefaultInputManager : public InputManager {
	public:
		void keyPressed(int key);
		void keyReleased(int key);
		void pointerPressed(int x, int y);
		void pointerReleased(int x, int y);
		void pointerMoved(int x, int y);
	};
}

#endif	//_SE_MSAB_MAUI_INPUTMANAGER_H_
