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
