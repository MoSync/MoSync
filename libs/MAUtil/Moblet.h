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

/** \file Moblet.h
* \brief C++ event manager, a base class for applications.
*
* The Moblet class provides an object-oriented wrapper around the
* C event system in MoSync.
*
* \author Patrick Broman
*
*/

#ifndef _SE_MSAB_MAUTIL_MOBLET_H_
#define _SE_MSAB_MAUTIL_MOBLET_H_

#include <ma.h>
#include <maassert.h>
#include "Environment.h"
#include "Vector.h"

namespace MAUtil {

	/**
	* \brief A C++ event manager, a base class for applications.
	*
	*	A Moblet manages the application main loop for you.
 	* To use Moblet: inherit it, construct an instance and call Moblet::run().

	* You'll probably want to implement your own keyPressEvent() and keyReleaseEvent().

	* Code that isn't supposed to be executed in response to an event should be placed
	* in a TimerListener or IdleListener that's registered with the Moblet.
	*/
	class Moblet : public Environment, public KeyListener, public PointerListener, public CloseListener, public CustomEventListener {
	public:
		/**
		* This function is called with a \link #MAK_FIRST MAK_ code \endlink when a key is pressed.
		*/
		virtual void keyPressEvent(int keyCode, int nativeCode) {}
		/**
		* This function is called with a \link #MAK_FIRST MAK_ code \endlink when a key is released.
		*/
		virtual void keyReleaseEvent(int keyCode, int nativeCode) {}

		/**
		* This function is called with a coordinate when a pointer is pressed.
		*/
		virtual void pointerPressEvent(MAPoint2d p) {}

		/**
		*  This function is called with a coordinate when a pointer is moved.
		*/
		virtual void pointerMoveEvent(MAPoint2d p) {}

		/**
		*  This function is called with a coordinate when a pointer is released.
		*/
		virtual void pointerReleaseEvent(MAPoint2d p) {}

		/**
		* This function is called with a coordinate when a pointer is pressed.
		*/
		virtual void multitouchPressEvent(MAPoint2d p, int touchId) {}

		/**
		*  This function is called with a coordinate when a pointer is moved.
		*/
		virtual void multitouchMoveEvent(MAPoint2d p, int touchId) {}

		/**
		*  This function is called with a coordinate when a pointer is released.
		*/
		virtual void multitouchReleaseEvent(MAPoint2d p, int touchId) {}

		/**
		* This function is called when the Close event is recieved.
		* The default implementation just calls close().
		*/
		virtual void ATTRIBUTE(noreturn, closeEvent());

		/**
		* This function is called when an event that Moblet doesn't recognize is recieved.
		*/
		virtual void customEvent(const MAEvent& event) {}

		/**
		* Runs a Moblet.
		* This function only returns if someone sets Moblet::mRun to false.
		* Example: <tt>Moblet::run(new MyMoblet());</tt>
		*/
		static void run(Moblet* moblet);

		/**
		* Shuts down the running application.
		*/
		static void ATTRIBUTE(noreturn, close());

		/**
		* Stops the running moblet.
		*/
		void exit();

	protected:
		/**
		* Moblet's constructor is protected, forcing you to inherit the class.
		* Causes a panic if another Moblet already exists.
		* There can be only one.
		*/
		Moblet();
		/**
		* Controls the continued execution of the Moblet's event 
		* loop. Setting it to 'false' will cause Moblet::run() to return
		* upon completing its current iteration.
		*/
		bool mRun;

		/**
		* Moblet's destructor calls close(), ensuring that a Moblet-based application
		* doesn't run without a live Moblet.
		*/
		virtual ~Moblet() { exit(); }

	private:
		void runPendingTimers();
		int timeToNextTimer();
		Moblet(const Moblet& m);
		Moblet& operator=(const Moblet& m);
	};
}

#endif	//_SE_MSAB_MAUTIL_MOBLET_H_
