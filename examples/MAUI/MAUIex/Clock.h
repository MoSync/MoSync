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
