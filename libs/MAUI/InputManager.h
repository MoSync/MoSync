/* Copyright (C) 2009 Mobile Sorcery AB

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
