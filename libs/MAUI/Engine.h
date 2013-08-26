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
* \file Engine.h 
* \brief MAUI widget manager, useful for developing custom widgets
* \author Patrick Broman and Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_ENGINE_H_
#define _SE_MSAB_MAUI_ENGINE_H_

#include "Widget.h"
#include "Font.h"

#include <MAUtil/Vector.h>
#include <MAUtil/Environment.h>

namespace MAUI {

	/** \brief Widget manager.
	  *
	  * This class manages the MAUI widget system. It is mainly of interest when
	  * when developing custom widgets.
	**/

	class Engine : public IdleListener, public FocusListener {
	public:
		enum {
			MAX_WIDGET_DEPTH = 16
		};

		/** Sets the widget that is main to the application, constituting the root of the UI tree **/
		void setMain(Widget* main);
		
		virtual ~Engine();

		/** Sets the default font to be used by Widgets when they are
          * not assigned any particular font.
		  **/
		void setDefaultFont(Font* systemFont);

		/** Returns the MAUI-wide default font **/
		Font* getDefaultFont();

		/** Sets the default skin to be used by Widgets when they are
          * not assigned any particular skin.
		  **/
		void setDefaultSkin(WidgetSkin* systemSkin);

		/** Returns the MAUI-wide default skin **/
		WidgetSkin* getDefaultSkin();

		void idle();
		
		/** Widgets call this function when they require repainting. It will cause 
		  * any dirty widgets to be redrawn in the next iteration of the event loop.
		  **/
		void requestUIUpdate();
		
		// added this because graphics can be invalidated on some devices when the focus is lost...
		void focusLost();
		void focusGained();

		/** Actually performs repainting **/ 
		void repaint();
		
		/** Returns a reference to the single instance of this class, using lazy
		  * initialization.
      	  **/
		static Engine& getSingleton();

		/* is an overlay shown? */
		bool isOverlayShown();

		/* shows the overlay (passed as an argument). Put the top left
		corner at position x and y. */
		void showOverlay(int x, int y, Widget *overlay);
		
		/* hide the currently shown overlay. */
		void hideOverlay();

	protected:
		Widget *overlay;
		Point overlayPosition;

		Widget* main;
		Font* defaultFont;
		WidgetSkin* defaultSkin;

		static Engine* singletonPtr;

		bool characterInputActive;

	private:
		Engine();
	};
}

#endif /*_SE_MSAB_MAUI_ENGINE_H_ */
