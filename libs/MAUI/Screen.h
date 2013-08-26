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
* \file Screen.h 
* \brief A full screen, a container for widgets
* \author Patrick Broman and Niklas Nummelin
*/

#ifndef _MAUI_SCREEN_H_
#define _MAUI_SCREEN_H_

#include "Widget.h"
#include <MAUtil/Environment.h>

/**
* \brief MoSync graphical user interface classes.
*
* A Screen represents a whole screen display that can contain MAUI widgets
* (Label, ListBox, Image, etc.) in a Layout.
*/

namespace MAUI {

	using namespace MAUtil;

	/** \brief A displayable screen; a container for MAUI widgets.
	  *
	  * A Screen is a logically distinct part of a MAUI interface.
	  * MAUI applications typically consist of a number of screens
	  * that the user can navigate between.
	  **/

	class Screen : public KeyListener, public PointerListener {
	public:
		Screen();
		/** Causes this screen to be shown and all other screens in
		  * the application to be hidden.
		  **/
		virtual void show();
		/** Sets the main widget for this screen. The widget will
		  * be resized to match the screen resolution of the device **/
		void setMain(Widget* main);
		/** Returns the main widget of this screen **/
		Widget* getMain();
		/** keylistener implementation to overload when inheriting 
		  * your own screens.
		  **/
		virtual void keyPressEvent(int keyCode, int nativeCode) {}
		/** keylistener implementation to overload when inheriting 
		  * your own screens.
		  **/
		virtual void keyReleaseEvent(int keyCode, int nativeCode) {}
		/** pointerlistener implementation to overload when inheriting 
		  * your own screens.
		  **/
		virtual void pointerPressEvent(MAPoint2d point) {}
		/** pointerlistener implementation to overload when inheriting 
		  * your own screens.
		  **/
		virtual void pointerReleaseEvent(MAPoint2d point) {}
		/** pointerlistener implementation to overload when inheriting 
		  * your own screens.
		  **/
		virtual void pointerMoveEvent(MAPoint2d point) {}

		/**
		* Does not delete the main Widget.
		*/
		~Screen();
	protected:
		virtual void hide();
		static Screen* currentScreen;
		Widget* main;
	};

}

#endif	//_MAUI_SCREEN_H_
