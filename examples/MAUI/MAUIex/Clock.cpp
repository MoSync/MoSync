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

#include "Clock.h"
#include <matime.h>
#include <mavsprintf.h>
#include <mastdlib.h>
#include <MAUtil/Graphics.h>

#define PI 3.14159265

namespace MAUI {

		enum eTimeType {
			LOCAL = 0,
			UTC = 1
		};

		enum Type {
			DIGITAL,
			ANALOGUE
		};


	Clock::Clock(int x, int y, int width, int height, Widget *parent) : 
			Widget(x, y, width, height, parent),
			color(0xffffff),
			type(ANALOGUE),
			font(0)
			{
		setTimeType(LOCAL);
		setDrawBackground(false);
	
		//MAUtil::Environment::getEnvironment().addTimer(this, 1000, -1);
		//requestRepaint();
	}

	Clock::Clock(int x, int y, int width, int height, Widget* parent, Font *font) 
		: Widget(x, y, width, height, parent),
		  color(0xffffff),
		  type(ANALOGUE),
		  font(font)
	{
		setTimeType(LOCAL);
		setDrawBackground(false);
	
		//TODO: synchronize timer so it runs very soon after second-switch.
		//MAUtil::Environment::getEnvironment().addTimer(this, 1000, -1);
		//requestRepaint();
	}

	Clock::~Clock() {
		MAUtil::Environment::getEnvironment().removeTimer(this);
	}

	void Clock::setEnabled(bool enabled) {
		Widget::setEnabled(enabled);
		if(!enabled) {
			MAUtil::Environment::getEnvironment().removeTimer(this);
		} else {
			MAUtil::Environment::getEnvironment().addTimer(this, 1000, -1);
			requestRepaint();
			currentTime = getTime();
		}
	}

	void Clock::setFont(Font *font) {
		this->font = font;
		requestRepaint();
	}

	static void drawLine(int x, int y, int start, int end, double angle, int col) {
		double c = cos(angle);
		double s = sin(angle);
		double sx = (double)x + c*(double)start;
		double sy = (double)y - s*(double)start;
		double dx = (double)x + c*(double)end;
		double dy = (double)y - s*(double)end;
		maSetColor(col);
		Gfx_line((int)(sx+0.5), (int)(sy+0.5), (int)(dx+0.5), (int)(dy+0.5));
	}

	void Clock::drawDigitalClock(int hour, int min, int sec) {
		if(!font) return;
		char digitalString[32];
		sprintf(digitalString, "%.2d:%.2d:%.2d", hour, min, sec);

		MAExtent e = font->getStringDimensions(digitalString);
		int width = EXTENT_X(e);
		int height = EXTENT_Y(e);

		font->drawString(digitalString, 
			(paddedBounds.width>>1) - (width>>1), 
			(paddedBounds.height>>1) - (height>>1));

	}

	void Clock::drawAnalogueClock(int hour, int min, int sec) {

		double minAngleDelta = (2.0*PI)/60.0;
		double hourAngleDelta = (2.0*PI)/12.0;
		double secAngleDelta = minAngleDelta;

		int radius = paddedBounds.width < paddedBounds.height ? paddedBounds.width : paddedBounds.height;
		radius >>= 1;	
		double angle;
		int cx = (paddedBounds.width/2);
		int cy = (paddedBounds.height/2);

		// draw seconds
		angle = (2.0*PI)/4.0;
		for(int i = 0; i < 60; i++) {
			if((i%5)==0) {
				drawLine(cx, cy, radius-8, radius, angle, color);

			} else {
				drawLine(cx, cy, radius-4, radius, angle, color);
			}
			angle+=secAngleDelta;
		}

		// draw second indicator
		drawLine(cx, cy, 0, radius, (2.0*PI)/4.0 - secAngleDelta*sec, color);

		// draw minute indicator
		drawLine(cx, cy, 0, (radius>>1)+(radius>>2), (2.0*PI)/4.0 - minAngleDelta*min, color);		
		
		// draw hour indicator
		drawLine(cx, cy, 0, (radius>>1), (2.0*PI)/4.0 - hourAngleDelta*hour - 
			((double)min/60.0)*hourAngleDelta, color);	
	}

	void Clock::drawWidget() {
		tm timeStruct;
		split_time(currentTime, &timeStruct);

		int hour = timeStruct.tm_hour;
		int min = timeStruct.tm_min;
		int sec = timeStruct.tm_sec;

		switch(type) {
			case ANALOGUE:
				drawAnalogueClock(hour, min, sec);
				break;
			case DIGITAL:
				drawDigitalClock(hour, min, sec);
				break;
		}
	}

	void Clock::setColor(int color) {
		this->color = color;
		requestRepaint();
	}
		
	void Clock::setTimeType(eTimeType timeType) {
		this->timeType = timeType;
		currentTime = getTime();
		requestRepaint();
	}

	int Clock::getTime() {
		switch(timeType) {
		case LOCAL:
			return maLocalTime();
		case UTC:
			return maTime();
		default:
			return 0;
		}
	}

	void Clock::setType(eType type) {
		this->type = type;
		requestRepaint();
	}

	void Clock::runTimerEvent() {
		currentTime++;
		time_t t = getTime();
		if(abs(int(t - currentTime)) > 1)
			currentTime = t;
		requestRepaint();
	}
}
