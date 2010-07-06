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

#include "Environment.h"
#include <maassert.h>
#include <conprint.h>

namespace MAUtil {
	PointerListener::~PointerListener() {
		bool isListener = Environment::getEnvironment().isPointerListener(this);
		if(isListener)
			Environment::getEnvironment().removePointerListener(this);
	}

	KeyListener::~KeyListener() {
		bool isListener = Environment::getEnvironment().isKeyListener(this);
		if(isListener)
			Environment::getEnvironment().removeKeyListener(this);
	}
	void KeyListener::keyPressEvent(int keyCode, int nativeCode) {}
	void KeyListener::keyReleaseEvent(int keyCode, int nativeCode) {}

	Environment::TimerEventInstance::TimerEventInstance(TimerListener* tl, int aPeriod, int aNumTimes) 
		: e(tl), period(aPeriod), numTimes(aNumTimes)
	{
		addTime = maGetMilliSecondCount();
		nextInvoke = addTime + period;
	}


	Environment* Environment::sEnvironment = NULL;

	Environment::Environment() 
		: mKeyListeners(false),
		mPointerListeners(false),
		mBtListener(NULL),
		mConnListeners(false),
		mIdleListeners(false),
		mTimerEvents(true),
		mFocusListeners(false),
		mVKListeners(false)
	{
		if(sEnvironment)
			PANIC_MESSAGE("The application tried to instantiate more than one Environment. "
			"There can be only one per application.");
		sEnvironment = this;
	}

	Environment& Environment::getEnvironment() {
		if(!sEnvironment)
			PANIC_MESSAGE("The application tried to get an Environment, "
			"but none has been instantiated.");
		return *sEnvironment;
	}

	Environment::~Environment() {
	}

	void Environment::addFocusListener(FocusListener* fl) {
		//MAASSERT(sEnvironment == this);
		mFocusListeners.add(fl);
	}

	void Environment::removeFocusListener(FocusListener* fl) {
		//MAASSERT(sEnvironment == this);
		mFocusListeners.remove(fl);
	}

	void Environment::addVKListener(VKListener *vkl) {
		mVKListeners.add(vkl);
	}

	void Environment::removeVKListener(VKListener *vkl) {
		mVKListeners.remove(vkl);
	}

	bool Environment::isFocusListener(FocusListener* fl) {
		return mFocusListeners.contains(fl);
	}

	void Environment::addKeyListener(KeyListener* kl) {
		//MAASSERT(sEnvironment == this);
		mKeyListeners.add(kl);
	}

	void Environment::removeKeyListener(KeyListener* kl) {
		//MAASSERT(sEnvironment == this);
		mKeyListeners.remove(kl);
	}

	bool Environment::isKeyListener(KeyListener* kl) {
		return mKeyListeners.contains(kl);
	}

	void Environment::addPointerListener(PointerListener* pl) {
		//MAASSERT(sEnvironment == this);
		mPointerListeners.add(pl);
	}

	void Environment::removePointerListener(PointerListener* pl) {
		//MAASSERT(sEnvironment == this);
		mPointerListeners.remove(pl);
	}

	bool Environment::isPointerListener(PointerListener* pl) {
		return mPointerListeners.contains(pl);
	}

	// There can be only one. If not, panic comes.
	void Environment::setBluetoothListener(BluetoothListener* btl) {
		//MAASSERT(sEnvironment == this);
		//this test should be removed in release verions
		if(mBtListener && btl)
			PANIC_MESSAGE("Only one Bluetooth listener can be set at any time. "
			"The application was attempting to register two.");
		mBtListener = btl;
	}
	// equivalent to setBluetoothListener(NULL)
	void Environment::removeBluetoothListener() {
		//MAASSERT(sEnvironment == this);
		//this test should be removed in release verions
		if(!mBtListener)
			PANIC_MESSAGE("The application tried to unregister a Bluetooth listener "
			"when none was registered.");
		mBtListener = NULL;
	}

	// Only one listener per connection is allowed, but the same ConnectionListener
	// can be used for several connections.
	void Environment::setConnListener(MAHandle conn, ConnListener* cl) {
		//MAASSERT(sEnvironment == this);
		ListenerSet_each(ConnListener, itr, mConnListeners) {
			if(itr->_mConn == conn) {
				PANIC_MESSAGE("The application tried to set a connection listener for a "
					"connection that already had a listener");
			}
		}
		cl->_mConn = conn;
		mConnListeners.add(cl);
	}
	void Environment::removeConnListener(MAHandle conn) {
		ListenerSet_each(ConnListener, itr, mConnListeners) {
			if(itr->_mConn == conn) {
				mConnListeners.remove(&*itr);
			}
		}
	}

	void Environment::addCloseListener(CloseListener* cl) {
		//MAASSERT(sEnvironment == this);
		Vector_each(CloseListener*, i, mCloseListeners)
			if(*i == cl) return;
		mCloseListeners.add(cl);
	}

	void Environment::removeCloseListener(CloseListener* cl) {
		//MAASSERT(sEnvironment == this);
		Vector_each(CloseListener*, i, mCloseListeners) 
			if(*i == cl) {
				mCloseListeners.remove(i);
				return;
			}
	}

	void Environment::addIdleListener(IdleListener* il) {
		//MAASSERT(sEnvironment == this);
		mIdleListeners.add(il);
	}

	void Environment::removeIdleListener(IdleListener* il) {
		//MAASSERT(sEnvironment == this);
		mIdleListeners.remove(il);
	}

	void Environment::addTimer(TimerListener* tl, int period, int numTimes) {
		ASSERT_MSG(period >= 0, "invalid period");
		ListenerSet_each(TimerEventInstance, tei, mTimerEvents) {
			if(tei->e == tl) {
				tei->period = period;
				tei->numTimes = numTimes;
				tei->addTime = maGetMilliSecondCount();
				tei->nextInvoke = tei->addTime + period;
				mTimerEvents.add(&*tei);
				return;
			}
		}
		mTimerEvents.add(new TimerEventInstance(tl, period, numTimes));
	}

	void Environment::removeTimer(TimerListener* tl) {
		ListenerSet_each(TimerEventInstance, tei, mTimerEvents) {
			if(tei->e == tl) {
				mTimerEvents.remove(&*tei);
				return;
			}
		}
	}

	void Environment::fireFocusGainedEvent() {
		//MAASSERT(sEnvironment == this);
		mFocusListeners.setRunning(true);
		ListenerSet_each(FocusListener, i, mFocusListeners) 
			i->focusGained();
		mFocusListeners.setRunning(false);
	}

	void Environment::fireFocusLostEvent() {
		//MAASSERT(sEnvironment == this);
		mFocusListeners.setRunning(true);
		ListenerSet_each(FocusListener, i, mFocusListeners)
			i->focusLost();
		mFocusListeners.setRunning(false);
	}

	void Environment::fireVKShownEvent() {
		mVKListeners.setRunning(true);
		ListenerSet_each(VKListener, i, mVKListeners)
			i->keyboardShown();
		mVKListeners.setRunning(false);
	}

	void Environment::fireVKHiddenEvent() {
		mVKListeners.setRunning(true);
		ListenerSet_each(VKListener, i, mVKListeners)
			i->keyboardHidden();
		mVKListeners.setRunning(false);
	}

	void Environment::fireKeyPressEvent(int keyCode, int nativeCode) {
		//MAASSERT(sEnvironment == this);

		mKeyListeners.setRunning(true);
		ListenerSet_each(KeyListener, i, mKeyListeners) {
			i->keyPressEvent(keyCode);
			i->keyPressEvent(keyCode, nativeCode);
		}
		mKeyListeners.setRunning(false);
	}

	void Environment::fireKeyReleaseEvent(int keyCode, int nativeCode) {
		//MAASSERT(sEnvironment == this);
		mKeyListeners.setRunning(true);
		ListenerSet_each(KeyListener, i, mKeyListeners) {
			i->keyReleaseEvent(keyCode);
			i->keyReleaseEvent(keyCode, nativeCode);
		}
		mKeyListeners.setRunning(false);
	}

	void Environment::firePointerPressEvent(MAPoint2d p) {
		//MAASSERT(sEnvironment == this);
		mPointerListeners.setRunning(true);
		ListenerSet_each(PointerListener, i, mPointerListeners) 
			i->pointerPressEvent(p);
		mPointerListeners.setRunning(false);
	}

	void Environment::firePointerMoveEvent(MAPoint2d p) {
		//MAASSERT(sEnvironment == this);
		mPointerListeners.setRunning(true);
		ListenerSet_each(PointerListener, i, mPointerListeners) 
			i->pointerMoveEvent(p);
		mPointerListeners.setRunning(false);
	}

	void Environment::firePointerReleaseEvent(MAPoint2d p) {
		//MAASSERT(sEnvironment == this);
		mPointerListeners.setRunning(true);
		ListenerSet_each(PointerListener, i, mPointerListeners) 
			i->pointerReleaseEvent(p);
		mPointerListeners.setRunning(false);
	}

	void Environment::fireBluetoothEvent(int state) {
		//MAASSERT(sEnvironment == this);
		if(mBtListener) mBtListener->bluetoothEvent(state);
	}

	void Environment::fireConnEvent(const MAConnEventData& data) {
		//MAASSERT(sEnvironment == this);
		mConnListeners.setRunning(true);
		ListenerSet_each(ConnListener, itr, mConnListeners) {
			if(itr->_mConn == data.handle) {
				itr->connEvent(data);
				break;
			}
		}
		mConnListeners.setRunning(false);
	}

	void Environment::fireCloseEvent() {
		//MAASSERT(sEnvironment == this);
		Vector_each(CloseListener*, i, mCloseListeners) 
			(*i)->closeEvent();
	}

	void Environment::runIdleListeners() {
		//MAASSERT(sEnvironment == this);

		mIdleListeners.setRunning(true);
		ListenerSet_each(IdleListener, i, mIdleListeners) {
			i->idle();
		}
		mIdleListeners.setRunning(false);
	}

}
