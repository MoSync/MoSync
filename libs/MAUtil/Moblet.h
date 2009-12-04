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

/** \file Moblet.h
* \brief A C++ event manager, a base class for applications.
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
#include <MAUtil/Environment.h>
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
	class Moblet : public Environment, public KeyListener, public PointerListener, public CloseListener {
	public:
		/**
		* This function is called with a \link #MAK_FIRST MAK_ code \endlink when a key is pressed.
		*/
		virtual void keyPressEvent(int keyCode) {}
		/**
		* This function is called with a \link #MAK_FIRST MAK_ code \endlink when a key is released.
		*/
		virtual void keyReleaseEvent(int keyCode) {}

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
		* This function is called when the Close event is recieved.
		* The default implementation just calls close().
		*/
		virtual void ATTRIB(noreturn) closeEvent();

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
#if 0	//compiler warning: destructor never exits
		/**
		* Moblet's destructor calls close(), ensuring that a Moblet-based application
		* doesn't run without a live Moblet.
		*/
		virtual ~Moblet() { close(); }
#endif	//0
	private:
		void runPendingTimers();
		int timeToNextTimer();
		Moblet(const Moblet& m);
		Moblet& operator=(const Moblet& m);
	};
}

#endif	//_SE_MSAB_MAUTIL_MOBLET_H_
