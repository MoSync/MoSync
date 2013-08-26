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
	/*
	 * Enum containing the available platforms.
	 */
	typedef enum
	{
		OS_IOS = 0,
		OS_ANDROID,
		OS_WIN,
		OS_UNKNOWN
	} PLATFORM_TYPE;

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
		virtual void multitouchPressEvent(MAPoint2d p, int touchId) {};
		virtual void multitouchMoveEvent(MAPoint2d p, int touchId) {};
		virtual void multitouchReleaseEvent(MAPoint2d p, int touchId) {};
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
	* \brief A listener for maSensor events.
	* \see Environment::addSensorListener()
	*/
	class SensorListener {
	public:
		/**
		 * \param 'a' Acceleration minus G on the x, y and z axis .
		 */
		virtual void sensorEvent(MASensor a) = 0;
	};

	/**
	* \brief A listener for orientation changes.
	* Note: If the application uses NativeUI, it is preffered
	* to use Screen::addScreenListener() instead, so that
	* all events will be received for specific screens.
	* \see Environment::addOrientationListener()
	*/
	class OrientationListener {
	public:
		/**
		* Called after the screen has finished rotating.
		* \param 'screenOrientation' One of the
		* \link #MA_SCREEN_ORIENTATION_PORTRAIT MA_SCREEN_ORIENTATION \endlink codes.
		*/
		virtual void orientationChanged(int orientation) = 0;
		/**
		* Send by current screen just before it begins rotating.
		* Note: available only on iOS platform.
		*/
		virtual void orientationWillChange() {};
	};


	/**
	 * \brief A listener for media export operations.
	 * \see Environment::addMediaExportListener().
	 */
	 class MediaExportListener {
	 public:
		/**
		 * Called after the operation of exporting/saving an image to the
		 * photo gallery of the device. This event is triggered by the
		 * #maSaveImageToDeviceGallery() call.
		 *
		 * Note: In order to obtain this notification you must register using
		 * \link Environment::addMediaExportListener() \endlink.
		 * Please remember to
		 * unregister when finished using \link Environment::removeMediaExportListener() \endlink.
		 *
		 * \param imageHandle Handle of the image that was the subject of the
		 * export operation.
		 * \param resultCode The result code of the operation of exporting an image to gallery.
		 * Possible values can be found \link #MA_MEDIA_RES_OK here \endlink.
		 */
		virtual void imageExportToGalleryFinished( const MAHandle& imageHandle,
			int resultCode ) {};
	 };

	/**
     * \brief A listener for camera events.
     * \see Environment::addCameraListener()
     *
     * Note: If the application uses NativeUI, use listener specific to the
     * #Camera object.
     */
    class CameraListener
    {
    public:
        /**
         * Called when a camera event was triggered.
         *
         * \param cameraEvent a camera related event.
         */
        virtual void cameraEvent(const MAEvent& cameraEvent) = 0;
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

		void addSensorListener(SensorListener* tl);
		void removeSensorListener(SensorListener* tl);

		void addOrientationListener(OrientationListener* ol);
		void removeOrientationListener(OrientationListener* ol);

		void addCameraListener(CameraListener* camListener);
		void removeCameraListener(CameraListener* camListener);

		/**
		 * Retrieves the current platform.
		 */
		PLATFORM_TYPE getCurrentPlatform();

		/**
		 * Adds listener for media export events.
		 */
		void addMediaExportListener(MediaExportListener* meListener);
		/**
		 * Removes media export events listener.
		 */
		void removeMediaExportListener(MediaExportListener* meListener);

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
		* Calls multitouchPressEvent() of all registered PointerListeners with the specified \a keyCode.
		*/
		void fireMultitouchPressEvent(MAPoint2d p, int touchId);
		/**
		* Calls multitouchMoveEvent() of all registered PointerListeners with the specified \a keyCode.
		*/
		void fireMultitouchMoveEvent(MAPoint2d p, int touchId);
		
		/**
		* Calls multitouchReleaseEvent() of all registered PointerListeners with the specified \a keyCode.
		*/
		void fireMultitouchReleaseEvent(MAPoint2d p, int touchId);

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
		* Calls the registered sensor listeners, if any.
		*/
		void fireSensorListeners(MASensor a);

		/**
		* Calls the registered ConnListener, if any, for the MAHandle specified by \a data.
		*/
		void fireConnEvent(const MAConnEventData& data);
		
		/**
		* Calls all registered CloseListeners.
		*/
		void fireCloseEvent();

		/**
		* Calls orientationChanged() of all registered orientation listeners.
		*/
		void fireOrientationChangedEvent(int screenOrientation);

		/**
		* Calls orientationWillChange() of all registered orientation listeners.
		*/
		void fireOrientationWillChangeEvent();

		/**
		* Calls cameraEvent() of all registered camera listeners.
		*/
		void fireCameraEvent(const MAEvent& cameraEvent);

		/**
		 * Calls specific method of all media export listeners. The method called depends
		 * on the media export operation.
		 */
		void fireMediaExportEvent(const MAEvent& mediaExportEvent);

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
		ListenerSet<SensorListener> mSensorListeners;
		ListenerSet<OrientationListener> mOrientationListeners;
		ListenerSet<CameraListener> mCameraListeners;
		ListenerSet<MediaExportListener> mMediaExportListeners;
		PLATFORM_TYPE mCurrentPlatform;
private:
		static Environment* sEnvironment;
	};
}

#endif //_MAUTIL_ENVIRONMENT_H_
