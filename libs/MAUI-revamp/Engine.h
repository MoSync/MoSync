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

/** 
* \file Engine.h 
* \brief MAUI widget manager, useful for developing custom widgets.
* \author Patrick Broman and Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_ENGINE_H_
#define _SE_MSAB_MAUI_ENGINE_H_

#include "Widget.h"
#include "Font.h"
#include "Style.h"

#include <MAUtil/Map.h>
#include <MAUtil/Vector.h>
#include <MAUtil/String.h>
#include <MAUtil/Environment.h>

namespace MAUI {

	/**
	 * \brief TODO: Document the purpose of this class.
	 */
	class OverlayListener {
	public:
		
		/**
		 * \brief TODO: Document the purpose of this enum.
		 */
		enum OutsideResponse {
			/// Don't do anything else with the event.
			eBreak,
			/// Process the event with the underlying Widget.
			eProceed,
		};

		/**
		 * Called when a pointerPressed event outside an active overlay 
		 * is received. The default response (if you don't pass an 
		 * OverlayListener to showOverlay()) is eProceed.
		 */
		virtual OutsideResponse pointerPressedOutsideOverlay(int x, int y) = 0;
	};

	/** 
	 * \brief Widget manager engine.
	 *
	 * This class manages the MAUI widget system. It is mainly of interest when
	 * when developing custom widgets.
	 *
	 * This class is a singleton.
	 */
	class Engine : public IdleListener, public FocusListener {
	public:
		/** 
		 * Sets the main widget of the application, 
		 * constituting the root of the UI tree.
		 * \param main The root widget of the UI tree.
		 */
		void setMain(Widget* main);

		Widget* getMain() { return mMain; }
		
		/**
		 * Destructor. The constructor is private.
		 */
		virtual ~Engine();

		/**
		 * TODO: Document this method.
		 */
		void idle();
		
		/** 
		 * Widgets call this method when they require repainting. 
		 * It will cause any dirty widgets to be redrawn in the next 
		 * iteration of the event loop.
		 */
		void requestUIUpdate();
		
		/**
		 * TODO: Improve this comment and document when this method is called
		 * and what it does.
		 *
		 * Added this because graphics can be invalidated on some devices when 
		 * the focus is lost...
		 */
		void focusLost();
		
		/**
		 * TODO: Document this method.
		 */
		void focusGained();

		/**
		 * Performs repainting.
		 */ 
		void repaint();
		
		/** 
		 * Returns a reference to the single instance of this class, using lazy
		 * initialization.
		 * \return The singtle instance of the Engine class.
      	 */
		static Engine& getSingleton();

		/**
		 * Returns a pointer to the current overlay widget, or NULL if 
		 * no overlay is shown.
		 * TODO: Document what an "overlay" is and what "current" means.
		 * \param position TODO: Document.
		 * \return The current overlay widget.
		 */
		Widget* currentOverlay(Point& position);

		/** 
		 * Show an overlay Widget. The top left corner is at 
		 * position \a x and \a y.
		 * \param x The left coordinate of the overlay widget.
		 * \param y The top coordinate of the overlay widget.
		 * \param overlay The overlay widget.
		 * \param listener TODO: Document param.
		 */
		void showOverlay(
			int x, 
			int y, 
			Widget* overlay, 
			OverlayListener* listener = NULL);

		/** 
		 * TODO: Document method.
		 * \param x TODO: Document param.
		 * \param y TODO: Document param.
		 * \param overlay The overlay widget.
		 * \return TODO: Document return value.
		 */
		OverlayListener::OutsideResponse fireOverlayEvent(int x, int y);
		
		/**
		 * Hide the currently shown overlay. 
		 */
		void hideOverlay();

		/**
		 * Set default widget style.
		 * If style already has been set, it just overwrites the element 
		 * (not deleting the pointer).
		 * \widgetType TODO: Document param.
		 * \style TODO: Document param.
		 */
		void setDefaultStyle(const String& widgetType, Style* style);
		
		/**
		 * Get default widget style.
		 * \widgetType TODO: Document param.
		 * \return TODO: Document return value.
		 */
		Style* getDefaultStyle(const String& widgetType);

		/**
		 * TODO: Document method. What is the "console"?
		 * \display If true display the console, if false hide it.
		 */
		void setDisplayConsole(bool display=true);

	protected:
		/**
		 * TODO: Document instance variable. 
		 */
		Widget *mOverlay;
		
		/**
		 * TODO: Document instance variable. 
		 */
		Point mOverlayPosition;
		
		/**
		 * TODO: Document instance variable. 
		 */
		OverlayListener* mOverlayListener;

		/**
		 * The root of the UI tree.
		 */
		Widget* mMain;

		/**
		 * The single instance of this class.
		 */
		static Engine* mSingletonPtr;

		/**
		 * TODO: Document instance variable. 
		 */
		bool mCharacterInputActive;

		/**
		 * TODO: Document instance variable. 
		 * TODO: Possibly change this to hash map.
		 */
		MAUtil::Map<String, Style*> mDefaultStyles;

		/**
		 * TODO: Document instance variable. 
		 */
		bool mDisplayConsole;

	private:
		/**
		 * Constructor.
		 */
		Engine();
	};
}

#endif /*_SE_MSAB_MAUI_ENGINE_H_ */
