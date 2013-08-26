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

#include "Moblet.h"
#include <conprint.h>

namespace MAUtil {

	Moblet::Moblet() : mRun(true) {
		//printf("Moblet::Moblet - %x", this);
		//environment = this;
		addKeyListener(this);
		addPointerListener(this);
		addCloseListener(this);
		addCustomEventListener(this);
	}

#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif

	// returns time to next timer, a value >= -1
	// -1 is returned if there are no timers.
	int Moblet::timeToNextTimer() {
		int now = maGetMilliSecondCount();
		int minTime = -1;
		ListenerSet_each(TimerEventInstance, tei, mTimerEvents) {
			int ttn = tei->nextInvoke - now;
			if(ttn < 0)
				ttn = 0;
			if(minTime == -1) {
				minTime = ttn;
			} else {
				minTime = MIN(minTime, ttn);
			}
		}
		return minTime;
	}

	void Moblet::runPendingTimers() {
		int now = maGetMilliSecondCount();
		// run all timer events and remove those that have run their number of times.
		// run them at most one time each, as to allow for periods <= 0.
		mTimerEvents.setRunning(true);
		ListenerSet_each(TimerEventInstance, tei, mTimerEvents) {
			if(now >= tei->nextInvoke) {
				tei->e->runTimerEvent();
				if(tei->numTimes > 0) {
					tei->numTimes--;
					if(tei->numTimes == 0) {
						mTimerEvents.remove(&*tei);
					}
				}
				tei->nextInvoke += tei->period;
			}
		}
		mTimerEvents.setRunning(false);
	}

	void Moblet::run(Moblet* moblet) {
		while(moblet->mRun) {
			MAEvent event;
			while(maGetEvent(&event)) {
				switch(event.type) {
					case EVENT_TYPE_CLOSE:
						moblet->fireCloseEvent();
						moblet->exit();
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
							moblet->firePointerPressEvent(event.point);
						moblet->fireMultitouchPressEvent(event.point, event.touchId);
						break;
					case EVENT_TYPE_POINTER_DRAGGED:
						if (event.touchId == 0)
							moblet->firePointerMoveEvent(event.point);
						moblet->fireMultitouchMoveEvent(event.point, event.touchId);
						break;
					case EVENT_TYPE_POINTER_RELEASED:
						if (event.touchId == 0)
							moblet->firePointerReleaseEvent(event.point);
						moblet->fireMultitouchReleaseEvent(event.point, event.touchId);
						break;
					case EVENT_TYPE_CONN:
						moblet->fireConnEvent(event.conn);
						break;
					case EVENT_TYPE_BT:
						moblet->fireBluetoothEvent(event.state);
						break;
					case EVENT_TYPE_TEXTBOX:
						moblet->fireTextBoxListeners(event.textboxResult, event.textboxLength);
						break;
					case EVENT_TYPE_SENSOR:
						moblet->fireSensorListeners(event.sensor);
						break;
					case EVENT_TYPE_ORIENTATION_DID_CHANGE:
						moblet->fireOrientationChangedEvent(event.orientation);
						break;
					case EVENT_TYPE_ORIENTATION_WILL_CHANGE:
						moblet->fireOrientationWillChangeEvent();
						break;
				    case EVENT_TYPE_CAMERA_SNAPSHOT:
						moblet->fireCameraEvent(event);
						break;
				    case EVENT_TYPE_CAMERA_PREVIEW:
				        moblet->fireCameraEvent(event);
				        // We need to fire a custom event for backwards compatibility.
				        moblet->fireCustomEventListeners(event);
				        break;
					case EVENT_TYPE_MEDIA_EXPORT_FINISHED:
						moblet->fireMediaExportEvent(event);
						break;
					default:
						moblet->fireCustomEventListeners(event);
				}
			}

			moblet->runPendingTimers();

#if 0	//unstable in the face of removals
			for(int i=moblet->idleListeners.size()-1; i >= 0; i--)
				moblet->idleListeners[i]->idle();
#else
			moblet->runIdleListeners();
#endif

			if ((moblet->mIdleListeners.size() == 0) && (moblet->mRun)) {

				int ttnt = moblet->timeToNextTimer();

				// if ttnt == 0, we shouldn't do maWait(0), since that
				// doesn't mean waiting no time....
				if(ttnt)
					maWait(ttnt);
			}

		}	//while(1)
	}

	void Moblet::closeEvent() {
		close();
	}

	void Moblet::close() {
		maExit(0);
	}

	void Moblet::exit() {
		mRun = false;
	}
}
