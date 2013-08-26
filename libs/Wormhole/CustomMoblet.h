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
 * @file CustomMoblet.h
 *
 * @brief Event manager, base class for applications.
 *
 * This is a modified version of class MAUtil/Moblet that
 * allows for more flexibility. It is more extension friendly
 * than the original version and provides more hook methods.
 *
 * @author Mikael Kindborg
 */

#ifndef WORMHOLE_CUSTOM_MOBLET_H_
#define WORMHOLE_CUSTOM_MOBLET_H_

#include <ma.h>
#include <maassert.h>
#include <MAUtil/Environment.h>
#include <MAUtil/Vector.h>

namespace Wormhole
{
/**
 * This class handles events. Subclass it for use in your application.
 * It is a modified version of class MAUtil::Moblet. The class is a
 * singleton, you can only have one instance of it.
 */
class CustomMoblet :
	public MAUtil::Environment,
	public MAUtil::KeyListener,
	public MAUtil::PointerListener,
	public MAUtil::CloseListener,
	public MAUtil::CustomEventListener
{
public:
	/**
	 * Constructor.
	 */
	CustomMoblet();

	/**
	 * Destructor.
	 */
	virtual ~CustomMoblet();

	/**
	 * Empty implementation of event handling methods.
	 */
	virtual void keyPressEvent(int keyCode, int nativeCode) {}
	virtual void keyReleaseEvent(int keyCode, int nativeCode) {}
	virtual void pointerPressEvent(MAPoint2d p) {}
	virtual void pointerMoveEvent(MAPoint2d p) {}
	virtual void pointerReleaseEvent(MAPoint2d p) {}
	virtual void multitouchPressEvent(MAPoint2d p, int touchId) {}
	virtual void multitouchMoveEvent(MAPoint2d p, int touchId) {}
	virtual void multitouchReleaseEvent(MAPoint2d p, int touchId) {}

	/**
	 * Passes event to the otherEvent method (I thought "otherEvent"
	 * was a better name than "customEvent").
	 */
	virtual void customEvent(const MAEvent& event) {}

	/**
	 * This function is called when the Close event is recieved.
	 * The default implementation calls close(), which exits the app.
	 */
	virtual void ATTRIBUTE(noreturn, closeEvent());

	/**
	 * Exits app by default.
	 */
	static void ATTRIBUTE(noreturn, close());

	/**
	 * This method is called for all events, it provides a hook
	 * for the subclass to handle any event. Override this method
	 * and return true to stop further event processing by the Moblet,
	 * return false to let the Moblet handle the event. This method
	 * provides a hook to block events and do custom event processing,
	 * or to do some processing before the Moblet handles the event.
	 * Default implementation does nothing and returns false.
	 */
	virtual bool handleEvent(const MAEvent& event) { return false; }

	/**
	 * Do standard processing of the event. You should seldom need to
	 * override this method, use the hook handleEvent instead.
	 */
	virtual void processEvent(const MAEvent& event);

	/**
	 * Enter the event loop.
	 */
	virtual void enterEventLoop();

	/**
	 * Exit the event loop.
	 */
	virtual void exitEventLoop();

	/**
	 * Exit the event loop. Calls exitEventLoop.
	 */
	virtual void exit();

	/**
	 * Methods used internally for timers.
	 */
	virtual void runPendingTimers();
	virtual int timeToNextTimer();

	/**
	 * Calls enterEventLoop on a moblet. For backwards compatibility
	 * with class Moblet.
	 * @param moblet The moblet to call enterEventLoop on.
	 */
	static void run(CustomMoblet* moblet);

protected:
	/**
	 * Exits the event loop when false. Used internally.
	 * Avoid modifying directly, call exitEventLoop instead.
	 */
	bool mRun;

}; // class

} // namespace

#endif
