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

/** \file Clock.h
 *	This file contains a sample implementation of a custom widget. It's a clock
 *	that can be either analogue or digital. It also demonstrates how to use timers
 *  in order to update a widget periodically or to animate it.
 */

#ifndef _SE_MSAB_MAUI_CLOCK_H_
#define _SE_MSAB_MAUI_CLOCK_H_

#include <MAUI/Widget.h>
#include <MAUI/Font.h>
#include <MAUtil/Environment.h>
#include <matime.h>

#ifndef MOBILEAUTHOR
#include <ma.h>
#else
#include "../../mosync-trunk/intlibs/helpers/cpp_defs.h"
#include "../../mosync-trunk/intlibs/helpers/cpp_maapi.h"
#endif


namespace MAUI {

	/**
	 * An example of a custom widget.
	 */
	class Clock : public Widget, public MAUtil::TimerListener {
	public:
		enum eTimeType {
			LOCAL = 0,
			UTC = 1
		};

		enum eType {
			ANALOGUE = 0,
			DIGITAL = 1
		};

		/**
		 * "Default" constructor by MAUI conventions.
		 */
		Clock(int x, int y, int width, int height, Widget *parent=NULL);
		/**
		 * Constructor accepting an additional parameter specifying the font to
		 * use when rendering the clock widget in analogue mode.
		 */
		Clock(int x, int y, int width, int height, Widget* parent=NULL, Font *font=NULL);
		~Clock();
		/**
		 * Sets the color to use for rendering the clock.
		 * @param color the drawing color, specified as a 32-bit RGBA value.
		 */
		void setColor(int color);
		/**
		 * Determines whether to use local or UTC time.
		 * @param timeType the type of time to use.
		 * @see eTimeType
		 */
		void setTimeType(eTimeType timeType);
		/**
		 * Determines whether the clock should have an analogue or
		 * digital appearance.
		 * @param type the type of clock.
		 * @see eType
		 */
		void setType(eType type);
		/**
		 * Sets the font to use when rendering the clock in analogue mode.
		 * @param font a pointer to the font object to use.
		 */
		void setFont(Font *font);
		/**
		 * An overload of Widget's setEnabled(), which makes
		 * sure to disable or enable the timer used by the clock
		 * widget.
		 * @param enabled the state of enabledness.
		 */
		void setEnabled(bool enabled = true);
	protected:
		/**
		 * The actual rendering of the clock.
		 */
		void drawWidget();
		/**
		 * The timer callback that makes sure to update
		 * the clock every second.
		 */
		void runTimerEvent();
		/**
		 * Returns the clock's time, with respect to the
		 * currently set \a timeType.
		 */
		int getTime();
		/**
		 * Helper functions performing actual rendering
		 * of the analogue clock variant.
		 */
		void drawAnalogueClock(int hour, int min, int sec);
		/**
		 * Helper functions performing actual rendering
		 * of the digital clock variant.
		 */
		void drawDigitalClock(int hour, int min, int sec);

		int color;
		eTimeType timeType;
		eType type;
		Font *font;
		time_t currentTime;
	};

}

#endif /* _SE_MSAB_MAUI_CLOCK_H_ */
