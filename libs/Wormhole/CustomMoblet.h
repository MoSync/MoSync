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
	 * Exit the event loop. For backwards compatibility
	 * with class Moblet.
	 */
	virtual void exit();

	/**
	 * Calls enterEventLoop on a moblet. For backwards compatibility
	 * with class Moblet.
	 * @param moblet The moblet to call enterEventLoop on.
	 */
	static void run(CustomMoblet* moblet);

	/**
	 * Methods used internally for timers.
	 */
	void runPendingTimers();
	int timeToNextTimer();

	/**
	 * Exits the event loop when false. Used internally.
	 * Avoid modifying directly, call exitEventLoop instead.
	 */
	bool mRun;

}; // class

} // namespace

#endif
