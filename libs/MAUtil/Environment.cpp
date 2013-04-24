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

#define BUF_SIZE 256

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
	void KeyListener::charEvent(uint character) {}

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
		mCustomEventListeners(false),
		mTextBoxListeners(false),
		mSensorListeners(false),
		mOrientationListeners(false),
		mCameraListeners(false),
		mMediaExportListeners(false),
		mCurrentPlatform(OS_UNKNOWN)
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

	// There can be only one listener.
	void Environment::setBluetoothListener(BluetoothListener* btl) {
		//MAASSERT(sEnvironment == this);
		mBtListener = btl;
	}
	// equivalent to setBluetoothListener(NULL)
	void Environment::removeBluetoothListener() {
		//MAASSERT(sEnvironment == this);
		mBtListener = NULL;
	}

	// Only one listener per connection is allowed, but the same ConnectionListener
	// can be used for several connections.
	void Environment::setConnListener(MAHandle conn, ConnListener* cl) {
		if (NULL == cl) {
			PANIC_MESSAGE("Environment::setConnListener: The listener must not be NULL");
		}
		
		//MAASSERT(sEnvironment == this);
		
		// First remove the existing listener, if any.
		removeConnListener(conn);
		
		// Set the listener for the connection.
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
		Vector_each(CloseListener*, i, mCloseListeners) {
			if(*i == cl) return;
		}
		mCloseListeners.add(cl);
	}

	void Environment::removeCloseListener(CloseListener* cl) {
		//MAASSERT(sEnvironment == this);
		Vector_each(CloseListener*, i, mCloseListeners) {
			if(*i == cl) {
				mCloseListeners.remove(i);
				return;
			}
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
	
	void Environment::addCustomEventListener(CustomEventListener* cl) {
		//MAASSERT(sEnvironment == this);
		mCustomEventListeners.add(cl);
	}

	void Environment::removeCustomEventListener(CustomEventListener* cl) {
		//MAASSERT(sEnvironment == this);
		mCustomEventListeners.remove(cl);
	}

	void Environment::addTextBoxListener(TextBoxListener* tl) {
		//MAASSERT(sEnvironment == this);
		mTextBoxListeners.add(tl);
	}

	void Environment::removeTextBoxListener(TextBoxListener* tl) {
		//MAASSERT(sEnvironment == this);
		mTextBoxListeners.remove(tl);
	}	

	void Environment::addSensorListener(SensorListener* tl) {
		//MAASSERT(sEnvironment == this);
		mSensorListeners.add(tl);
	}

	void Environment::removeSensorListener(SensorListener* tl) {
		//MAASSERT(sEnvironment == this);
		mSensorListeners.remove(tl);
	}

	void Environment::addOrientationListener(OrientationListener* ol) {
		mOrientationListeners.add(ol);
	}

	void Environment::removeOrientationListener(OrientationListener* ol) {
		mOrientationListeners.remove(ol);
	}

	void Environment::addCameraListener(CameraListener* camListener)
	{
		mCameraListeners.add(camListener);
	}

	void Environment::removeCameraListener(CameraListener* camListener)
	{
		mCameraListeners.remove(camListener);
	}


	PLATFORM_TYPE Environment::getCurrentPlatform()
	{
		if ( OS_UNKNOWN == mCurrentPlatform )
		{
			// Used in determining the platform.
			char platform[BUF_SIZE];

			maGetSystemProperty("mosync.device.OS", platform, BUF_SIZE);

			if (strcmp(platform, "Android") == 0)
			{
				mCurrentPlatform = OS_ANDROID;
			}
			else if (strcmp(platform, "iPhone OS") == 0)
			{
				mCurrentPlatform = OS_IOS;
			}
			else
			{
				mCurrentPlatform = OS_WIN;
			}
		}
		return mCurrentPlatform;
	}

	void Environment::addMediaExportListener(MediaExportListener* meListener) {
		mMediaExportListeners.add(meListener);
	}

	void Environment::removeMediaExportListener(MediaExportListener* meListener) {
		mMediaExportListeners.remove(meListener);
	}

	void Environment::fireFocusGainedEvent() {
		//MAASSERT(sEnvironment == this);
		mFocusListeners.setRunning(true);
		ListenerSet_each(FocusListener, i, mFocusListeners) {
			i->focusGained();
		}
		mFocusListeners.setRunning(false);
	}

	void Environment::fireFocusLostEvent() {
		//MAASSERT(sEnvironment == this);
		mFocusListeners.setRunning(true);
		ListenerSet_each(FocusListener, i, mFocusListeners) {
			i->focusLost();
		}
		mFocusListeners.setRunning(false);
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

	void Environment::fireCharEvent(uint character) {
		mKeyListeners.setRunning(true);
		ListenerSet_each(KeyListener, i, mKeyListeners) {
			i->charEvent(character);
		}
		mKeyListeners.setRunning(false);
	}

	void Environment::firePointerPressEvent(MAPoint2d p) {
		//MAASSERT(sEnvironment == this);
		mPointerListeners.setRunning(true);
		ListenerSet_each(PointerListener, i, mPointerListeners) {
			i->pointerPressEvent(p);
		}
		mPointerListeners.setRunning(false);
	}

	void Environment::firePointerMoveEvent(MAPoint2d p) {
		//MAASSERT(sEnvironment == this);
		mPointerListeners.setRunning(true);
		ListenerSet_each(PointerListener, i, mPointerListeners) {
			i->pointerMoveEvent(p);
		}
		mPointerListeners.setRunning(false);
	}

	void Environment::firePointerReleaseEvent(MAPoint2d p) {
		//MAASSERT(sEnvironment == this);
		mPointerListeners.setRunning(true);
		ListenerSet_each(PointerListener, i, mPointerListeners) {
			i->pointerReleaseEvent(p);
		}
		mPointerListeners.setRunning(false);
	}

	void Environment::fireMultitouchPressEvent(MAPoint2d p, int touchId) {
		//MAASSERT(sEnvironment == this);
		mPointerListeners.setRunning(true);
		ListenerSet_each(PointerListener, i, mPointerListeners) {
			i->multitouchPressEvent(p, touchId);
		}
		mPointerListeners.setRunning(false);
	}

	void Environment::fireMultitouchMoveEvent(MAPoint2d p, int touchId) {
		//MAASSERT(sEnvironment == this);
		mPointerListeners.setRunning(true);
		ListenerSet_each(PointerListener, i, mPointerListeners) {
			i->multitouchMoveEvent(p, touchId);
		}
		mPointerListeners.setRunning(false);
	}

	void Environment::fireMultitouchReleaseEvent(MAPoint2d p, int touchId) {
		//MAASSERT(sEnvironment == this);
		mPointerListeners.setRunning(true);
		ListenerSet_each(PointerListener, i, mPointerListeners) {
			i->multitouchReleaseEvent(p, touchId);
		}
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
		Vector_each(CloseListener*, i, mCloseListeners) {
			(*i)->closeEvent();
		}
	}
	
	void Environment::fireCustomEventListeners(const MAEvent& e) {
		//MAASSERT(sEnvironment == this);
		mCustomEventListeners.setRunning(true);
		ListenerSet_each(CustomEventListener, i, mCustomEventListeners) {
			i->customEvent(e);
		}
		mCustomEventListeners.setRunning(false);
	}
	
	void Environment::fireTextBoxListeners(int result, int textLength) {
		//MAASSERT(sEnvironment == this);
		mTextBoxListeners.setRunning(true);
		ListenerSet_each(TextBoxListener, i, mTextBoxListeners) {
			i->textBoxClosed(result, textLength);
		}
		mTextBoxListeners.setRunning(false);
	}	

	void Environment::fireSensorListeners(MASensor a) {
		//MAASSERT(sEnvironment == this);
		mSensorListeners.setRunning(true);
		ListenerSet_each(SensorListener, i, mSensorListeners) {
			i->sensorEvent(a);
		}
		mSensorListeners.setRunning(false);
	}

	void Environment::fireOrientationChangedEvent(int screenOrientation) {
		mOrientationListeners.setRunning(true);
		ListenerSet_each(OrientationListener, i, mOrientationListeners) {
			i->orientationChanged(screenOrientation);
		}
		mOrientationListeners.setRunning(false);
	}

	void Environment::fireOrientationWillChangeEvent() {
		mOrientationListeners.setRunning(true);
		ListenerSet_each(OrientationListener, i, mOrientationListeners) {
			i->orientationWillChange();
		}
		mOrientationListeners.setRunning(false);
	}


	void Environment::fireCameraEvent(const MAEvent& cameraEvent)
	{
		mCameraListeners.setRunning(true);
		ListenerSet_each(CameraListener, i, mCameraListeners) {
			i->cameraEvent(cameraEvent);
		}
		mCameraListeners.setRunning(false);
	}

	void Environment::fireMediaExportEvent(const MAEvent& mediaExportEvent)
	{
		if ( MA_MEDIA_TYPE_IMAGE == mediaExportEvent.mediaType )
		{
			mMediaExportListeners.setRunning(true);
			ListenerSet_each(MediaExportListener, i, mMediaExportListeners)
			{
				i->imageExportToGalleryFinished(
					mediaExportEvent.mediaHandle,
					mediaExportEvent.operationResultCode);
			}
			mMediaExportListeners.setRunning(false);
		}
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
