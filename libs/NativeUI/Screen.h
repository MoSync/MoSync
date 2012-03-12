/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/*! \addtogroup NativeUILib
 *  @{
 */

/**
 *  @defgroup NativeUILib Native UI Library
 *  @{
 */

/**
 * @file Screen.h
 * @author Mikael Kindborg
 *
 * \brief Class that represents a visible screen. Only one screen
 * is visible at a time.
 */

#ifndef NATIVEUI_SCREEN_H_
#define NATIVEUI_SCREEN_H_

#include "Widget.h"

namespace NativeUI
{

	/**
	 * \brief Class that represents a visible screen.
	 */
	class Screen : public Widget
	{
	public:
		/**
		 * Constructor.
		 */
		Screen();

		/**
		 * Destructor.
		 */
		virtual ~Screen();

		/**
		 * Set the title of the screen.
		 * The title is used by tab screen to display a text on the
		 * tab indicator.
		 * @param title The screen title.
		 */
		void setTitle(const MAUtil::String& title);

		/**
		 * Set the icon of the screen. The icon is displayed
		 * on tab screens.
		 * Note: this is not available on Windows Phone 7.
		 * @param imageHandle Handle to an image with the icon.
		 * @return The result code.
		 */
		int setIcon(const MAHandle imageHandle);

		/**
		 * Set the main widget of the screen.
		 * Note: A screen can only have one main widget.
		 * Use a layout as main widget and add child
		 * widgets to the layout.
		 * @param widget The widget to be set as main widget.
		 * The ownership of the widget is passed to this function.
		 * When the screen will be destroyed, the widget and it's child widgets
		 * will be deleted.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the child could be added to the parent.
		 * - #MAW_RES_INVALID_LAYOUT if the widget was added to a non-layout.
		 * - #MAW_RES_ERROR if it could not be added for some other reason.
		 */
		int setMainWidget(Widget* widget);

		/**
		 * Show a screen. Only one screen at a time is visible.
		 * The previous screen will be hidden when showing a screen.
		 * Note: This method is only applicable to screens.
		 */
		virtual void show();

		/**
		 * Called just before the screen begins rotating.
		 * Subclasses may override this method to perform additional actions
		 * immediately prior to the rotation.
		 */
		virtual void orientationWillChange();

		/**
		 * Called after the screen has finished rotating.
		 * Subclasses may override this method to perform additional actions
		 * after the rotation.
		 */
		virtual void orientationDidChange();

	protected:
		/**
		 * Protected because only subclasses should use this constructor.
		 * @widgetType The string constant that identifies the widget type
		 * (one of the MAW_ constants).
		 */
		Screen(const MAUtil::String& widgetType);

        /**
         * This method is called when there is an event for this widget.
         * It passes on the event to all widget's listeners.
         * @param widgetEventData The data for the widget event.
         */
        void handleWidgetEvent(MAWidgetEventData* widgetEventData);
	};

} // namespace NativeUI

#endif

/*! @} */
