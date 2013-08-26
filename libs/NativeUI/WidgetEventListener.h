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

/*! \addtogroup NativeUILib
 *  @{
 */

/**
 *  @defgroup NativeUILib Native UI Library
 *  @{
 */

/**
 * @file WidgetEventListener.h
 * @author Mikael Kindborg
 *
 * \brief Interface for widget event listeners.
 */

#ifndef NATIVEUI_WIDGET_EVENT_LISTENER_H_
#define NATIVEUI_WIDGET_EVENT_LISTENER_H_

namespace NativeUI
{

	// Forward declaration.
	class Widget;

	/**
	 * \brief Interface for widget event listeners.
	 */
	class WidgetEventListener
	{
	public:
		/**
		 * This method is called when there is an event for this widget.
		 * @param widget The widget object of the event.
		 * @param widgetEventData The low-level event data.
		 */
		virtual void handleWidgetEvent(
			Widget* widget,
			MAWidgetEventData* widgetEventData) = 0;
	};

} // namespace NativeUI

#endif /* NATIVEUI_WIDGET_EVENT_LISTENER_H_ */

/*! @} */
