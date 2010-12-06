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

/** \file Environment.h
* \brief Cross-platform event handling
*/

#ifndef _MAUTIL_ENVIRONMENT_H_
#define _MAUTIL_ENVIRONMENT_H_

#ifndef MOBILEAUTHOR
#include <ma.h>
#else
#include "../../mosync-trunk/intlibs/helpers/cpp_defs.h"
#include "../../mosync-trunk/intlibs/helpers/cpp_maapi.h"
#include <stdio.h>
#define MAAPI_H
#endif

#include <maassert.h>
#include "Vector.h"
#include "ListenerSet.h"

namespace MAUtil {
	/**
	* \brief A listener for keypad events.
	*
	* It uses \link #MAK_FIRST MAK_ \endlink codes.
	* \see Environment::addKeyListener()
	*/
	class KeyListener {
	public:
		virtual ~KeyListener();
		virtual void keyPressEvent(int keyCode, int nativeCode);
		virtual void keyReleaseEvent(int keyCode, int nativeCode);

		/**
		* \param character A Unicode character.
		*/
		virtual void charEvent(uint character);

		//deprecated
		virtual void keyPressEvent(int keyCode) {}
		virtual void keyReleaseEvent(int keyCode) {}
	};

	/**
	* \brief A listener for pointer events.
	* \see Environment::addPointerListener()
	*/
	class PointerListener {
	public:
		virtual ~PointerListener();
		virtual void pointerPressEvent(MAPoint2d p) = 0;
		virtual void pointerMoveEvent(MAPoint2d p) = 0;
		virtual void pointerReleaseEvent(MAPoint2d p) = 0;
	};

	/**
	* \brief A listener for Bluetooth Discovery events.
	* \see Environment::setBluetoothListener()
	*/
	class BluetoothListener {
	public:	
		virtual void bluetoothEvent(int state) = 0;
	};

	/**
	* \brief A listener for Connection events.
	* \see Environment::setConnListener()
	*/
	class ConnListener {
	public:	
		virtual void connEvent(const MAConnEventData& data) = 0;
	private:
		MAHandle _mConn;
		friend class Environment;
	};

	/**
	* \brief A listener for the Close event.
	* \see Environment::addCloseListener()
	*/
	class CloseListener {
	public:
		virtual void closeEvent() = 0;
	};

	/**
	* \brief A listener whose function is called whenever nothing else has to be done.
	* \see Environment::addIdleListener()
	*/
	class IdleListener {
	public:
		/**
		* This function is called once per event loop.
		*/
		virtual void idle() = 0;
	};

	/**
	* \brief A listener for timer events.
	* \see Environment::addTimer()
	*/
	class TimerListener {
	public:
		virtual void runTimerEvent() = 0;
	};

	/**
	* \brief A listener for focus events.
	* \see Environment::addFocusListener()
	*/
	class FocusListener {
	public:
		virtual void focusLost() = 0;
		virtual void focusGained() = 0;
	};

	/**
	* \brief A listener for custom events.
	* \see Environment::addCustomEventListener()
	*/		
	class CustomEventListener {
	public:
		virtual void customEvent(const MAEvent& event) = 0;
	};
	
	/**
	* \brief A listener for maTextBox events.
	* \see Environment::addTextBoxListener()
	*/	
	class TextBoxListener {
	public:
		/**
		 * \param 'result' One of the \link #MA_TB_RES_OK MA_TB_RES \endlink codes.
		 * \param 'textLength' The length, in characters, of the finished text. Does not include the terminating zero.
		 */
		virtual void textBoxClosed(int result, int textLength) = 0;
	};
	
	/**
	* \brief A base class for cross-platform event managers.
	*/
	class Environment {
	public:
		/**
		* Adds the specified listener to the end of the list,
		* unless it is already in the list.
		*/
		void addFocusListener(FocusListener* kl);
		
		/**
		* Removes the specified listener from the list, if it's in it.
		*/
		void removeFocusListener(FocusListener* kl);

		/** Queries whether the specified listener is in the
		  * list of key listeners.
		  **/
		bool isFocusListener(FocusListener* kl);

		/**
		* Adds the specified listener to the end of the list,
		* unless it is already in the list.
		*/
		void addKeyListener(KeyListener* kl);

		/**
		* Removes the specified listener from the list, if it's in it.
		*/
		void removeKeyListener(KeyListener* kl);

		/** Queries whether the specified listener is in the
		  * list of key listeners.
		  **/
		bool isKeyListener(KeyListener* kl);

		/**
		* Adds the specified listener to the end of the list,
		* unless it is already in the list.
		*/
		void addPointerListener(PointerListener* pl);

		/**
		* Removes the specified listener from the list, if it's in it.
		*/
		void removePointerListener(PointerListener* pl);

		/** Queries whether the specified listener is in the
		  * list of key listeners.
		  **/
		bool isPointerListener(PointerListener* pl);

		/**
		* Sets the Bluetooth listener. Any previously set listener is replaced.
		* \param btl The listener.
		*/
		void setBluetoothListener(BluetoothListener* btl);

		/**
		* Removes the Bluetooth listener, if any.
		*/
		void removeBluetoothListener();

		/**
		* Sets the listener for a connection.
		* Only one listener per connection is allowed, but the same ConnListener
		* can be used with several connections.
		*/
		void setConnListener(MAHandle conn, ConnListener* cl);

		/**
		* Removes the listener for a connection, if any.
		*/
		void removeConnListener(MAHandle conn);

		/**
		* Adds a listener for the Close event.
		* Adds the specified listener to the end of the list,
		* unless it is already in the list.
		*/
		void addCloseListener(CloseListener* cl);
		
		/**
		* Removes a listener for the Close event.
		* Removes the specified listener from the list, if it's in it.
		* Must not be called from within closeEvent().
		*/
		void removeCloseListener(CloseListener* cl);

		/**
		* Adds the specified IdleListener to the end of the list,
		* unless it is already in the list.
		*
		* IdleListeners are run again and again while they are registered.
		* Take care, as constant processor usage will drain battery power quickly.
		*/
		virtual void addIdleListener(IdleListener* il);
		
		/**
		* Removes the specified IdleListener from the list, if it's in it.
		*/
		virtual void removeIdleListener(IdleListener* il);

		/**
		* Adds a timer. This causes the runTimerEvent() function of the specified TimerListener
		* to be called a number of times, at a fixed rate.
		* The timer is then removed automatically.
		*
		* You can only have one timer per listener,
		* so if a timer with the specified listener is already active,
		* it is overwritten.
		*
		* \param tl The TimerListener to use.
		* \param period The timer's average period, in milliseconds.
		* \param numTimes The number of periods that should pass before the timer is removed.
		* May be zero or negative. If so, the timer is never removed automatically.
		*/
		void addTimer(TimerListener* tl, int period, int numTimes);
		
		/**
		* Removes the timer associated with the specified listener, if any.
		*/
		void removeTimer(TimerListener* tl);
		
		/**
		* Adds the specified listener to the end of the list,
		* unless it is already in the list.
		*/
		void addCustomEventListener(CustomEventListener* cl);

		/**
		* Removes the specified listener from the list, if it's in it.
		*/
		void removeCustomEventListener(CustomEventListener* cl);
		
		/**
		* Adds the specified listener to the end of the list,
		* unless it is already in the list.
		*/
		void addTextBoxListener(TextBoxListener* tl);

		/**
		* Removes the specified listener from the list, if it's in it.
		*/
		void removeTextBoxListener(TextBoxListener* tl);		

		/**
		* Returns a reference to the Environment.
		* Causes a panic if no Environment exists.
		*/
		static Environment& getEnvironment();

		virtual ~Environment();
		
	protected:
		/**
		* Causes a panic if another Environment already exists.
		* There can be only one.
		*/
		Environment();
		
		/**
		* Calls focusGained() of all registered FocusListeners.
		*/
		void fireFocusGainedEvent();
		
		/**
		* Calls focusLost() of all registered FocusListeners.
		*/
		void fireFocusLostEvent();
		
		/**
		* Calls keyPressEvent() of all registered KeyListeners with the specified \a keyCode.
		*/
		void fireKeyPressEvent(int keyCode, int nativeCode);
		
		/**
		* Calls keyReleaseEvent() of all registered KeyListeners with the specified \a keyCode.
		*/
		void fireKeyReleaseEvent(int keyCode, int nativeCode);
		
		/**
		* Calls charEvent() of all registered KeyListeners with the specified \a character.
		*/
		void fireCharEvent(uint character);
		
		/**
		* Calls pointerPressEvent() of all registered PointerListeners with the specified \a keyCode.
		*/
		void firePointerPressEvent(MAPoint2d p);
		/**
		* Calls pointerMoveEvent() of all registered PointerListeners with the specified \a keyCode.
		*/
		void firePointerMoveEvent(MAPoint2d p);
		
		/**
		* Calls pointerReleaseEvent() of all registered PointerListeners with the specified \a keyCode.
		*/
		void firePointerReleaseEvent(MAPoint2d p);
		
		/**
		* Calls the registered BluetoothListener, if any.
		*/
		void fireBluetoothEvent(int state);
		
		
		/**
		* Calls the registered custom event listeners, if any.
		*/		
		void fireCustomEventListeners(const MAEvent& e);
		
		/**
		* Calls the registered text box listeners, if any.
		*/		
		void fireTextBoxListeners(int result, int textLength);
				
		
		/**
		* Calls the registered ConnListener, if any, for the MAHandle specified by \a data.
		*/
		void fireConnEvent(const MAConnEventData& data);
		
		/**
		* Calls all registered CloseListeners.
		*/
		void fireCloseEvent();

		/**
		* Calls all registered IdleListeners once each.
		*/
		void runIdleListeners();

		/**
		* \brief A timer event.
		*/
		class TimerEventInstance {
		public:
			TimerEventInstance(TimerListener* tl, int period, int numTimes); 
			TimerListener* e;
			int addTime;
			int period;
			int numTimes;
			int nextInvoke;
		};

		ListenerSet<KeyListener> mKeyListeners;
		ListenerSet<PointerListener> mPointerListeners;
		BluetoothListener* mBtListener;
		Vector<CloseListener*> mCloseListeners;
		ListenerSet<ConnListener> mConnListeners;
		ListenerSet<IdleListener> mIdleListeners;
		ListenerSet<TimerEventInstance> mTimerEvents;
		ListenerSet<FocusListener> mFocusListeners;
		ListenerSet<CustomEventListener> mCustomEventListeners;
		ListenerSet<TextBoxListener> mTextBoxListeners;		
private:
		static Environment* sEnvironment;
	};
}

#endif //_MAUTIL_ENVIRONMENT_H_
