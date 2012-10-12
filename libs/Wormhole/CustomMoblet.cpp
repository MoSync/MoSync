/*
Copyright (C) 2012 MoSync AB

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

/**
 * @file CustomMoblet.cpp
 *
 * @brief Event manager, base class for applications.
 *
 * This is a modified version of class MAUtil/Moblet that
 * allows for more flexibility. It is more extension friendly
 * than the original version and provides more hook methods.
 *
 * @author Mikael Kindborg
 */

#include "CustomMoblet.h"

using namespace MAUtil;

namespace Wormhole
{
CustomMoblet::CustomMoblet() :
	mRun(true)
{
	addKeyListener(this);
	addPointerListener(this);
	addCloseListener(this);
	addCustomEventListener(this);
}

CustomMoblet::~CustomMoblet()
{
}

void CustomMoblet::enterEventLoop()
{
	CustomMoblet* moblet = this;

	mRun = true;

	while (moblet->mRun)
	{
		MAEvent event;
		while (maGetEvent(&event))
		{
			bool handled = handleEvent(event);
			if (!handled)
			{
				processEvent(event);
			}
		}

		moblet->runPendingTimers();
		moblet->runIdleListeners();

		if (moblet->mIdleListeners.size() == 0)
		{
			int ttnt = moblet->timeToNextTimer();
			// Don't do maWait(0) because that waits indefinitely.
			if (ttnt > 0)
			{
				maWait(ttnt);
			}
		}
	}
}

void CustomMoblet::exitEventLoop()
{
	mRun = false;
}

void CustomMoblet::exit()
{
	exitEventLoop();
}

void CustomMoblet::run(CustomMoblet* moblet)
{
	moblet->enterEventLoop();
}

void CustomMoblet::closeEvent()
{
	close();
}

void CustomMoblet::close()
{
	maExit(0);
}

void CustomMoblet::processEvent(const MAEvent& event)
{
	CustomMoblet* moblet = this;

	switch (event.type)
	{
		case EVENT_TYPE_CLOSE:
			moblet->fireCloseEvent();
			break;
		case EVENT_TYPE_FOCUS_GAINED:
			moblet->fireFocusGainedEvent();
			break;
		case EVENT_TYPE_FOCUS_LOST:
			moblet->fireFocusLostEvent();
			break;
		case EVENT_TYPE_KEY_PRESSED:
			moblet->fireKeyPressEvent(event.key, event.nativeKey);
			break;
		case EVENT_TYPE_KEY_RELEASED:
			moblet->fireKeyReleaseEvent(event.key, event.nativeKey);
			break;
		case EVENT_TYPE_CHAR:
			moblet->fireCharEvent(event.character);
			break;
		case EVENT_TYPE_POINTER_PRESSED:
			if (event.touchId == 0)
			{
				moblet->firePointerPressEvent(event.point);
			}
			moblet->fireMultitouchPressEvent(
				event.point,
				event.touchId);
			break;
		case EVENT_TYPE_POINTER_DRAGGED:
			if (event.touchId == 0)
			{
				moblet->firePointerMoveEvent(event.point);
			}
			moblet->fireMultitouchMoveEvent(
				event.point,
				event.touchId);
			break;
		case EVENT_TYPE_POINTER_RELEASED:
			if (event.touchId == 0)
			{
				moblet->firePointerReleaseEvent(event.point);
			}
			moblet->fireMultitouchReleaseEvent(
				event.point,
				event.touchId);
			break;
		case EVENT_TYPE_CONN:
			moblet->fireConnEvent(event.conn);
			break;
		case EVENT_TYPE_BT:
			moblet->fireBluetoothEvent(event.state);
			break;
		case EVENT_TYPE_TEXTBOX:
			moblet->fireTextBoxListeners(
				event.textboxResult,
				event.textboxLength);
			break;
		case EVENT_TYPE_SENSOR:
			moblet->fireSensorListeners(event.sensor);
			break;
		default:
			moblet->fireCustomEventListeners(event);
	}
}

/**
 * This method always returns >= 0.
 */
int CustomMoblet::timeToNextTimer()
{
	int now = maGetMilliSecondCount();
	int minTime = -1;
	ListenerSet_each(TimerEventInstance, tei, mTimerEvents)
	{
		int ttn = tei->nextInvoke - now;
		if (ttn < 0)
		{
			ttn = 0;
		}
		if (minTime == -1)
		{
			minTime = ttn;
		}
		else
		{
			minTime = minTime < ttn ? minTime : ttn;
		}
	}
	return minTime;
}

void CustomMoblet::runPendingTimers()
{
	int now = maGetMilliSecondCount();

	// Run all timer events and remove those that have run
	// their number of times. Run them at most one time each,
	// as to allow for periods <= 0.
	mTimerEvents.setRunning(true);
	ListenerSet_each(TimerEventInstance, tei, mTimerEvents)
	{
		if (now >= tei->nextInvoke)
		{
			tei->e->runTimerEvent();
			if (tei->numTimes > 0)
			{
				tei->numTimes--;
				if (tei->numTimes == 0)
				{
					mTimerEvents.remove(&*tei);
				}
			}
			tei->nextInvoke += tei->period;
		}
	}
	mTimerEvents.setRunning(false);
}

} // namespace
