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

#include <windows.h>

#include "Vibration.h"	

namespace Base 
{
	namespace Vibration
	{
		/**
		 * Determines the type of vibration.
		 *
		 * OFF - Turns off the vibration.
		 * ON - Turns on the vibration.
		 * BLINK - Blinks the vibration according to the given
		 *         on and off cycles.
		 */
		enum VibratorState {OFF = 0, ON = 1, BLINK=2};

		/**
		 * The integer specifying the vibrator LED.
         *
		 * Note: This is not guaranteed to be the default vibrator LED on all
		 *       devices.
		 */
		static const int VIBRATOR_LED = 1;

		/**
		 * The ID of the vibrator timer.
		 */
		static const int IDT_VIBRATOR_TIMER = 5;

		/**
		 * Function that is called when the timer should stop.
		 *
		 * @see TimerProc, http://msdn.microsoft.com/en-us/library/ms644907(v=VS.85).aspx.
		 */
		static VOID CALLBACK VibrateTimerCallback(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime);

		bool available()
		{
			NLED_COUNT_INFO nci;
			NLedGetDeviceInfo(NLED_COUNT_INFO_ID, (PVOID) &nci);

			return ((INT) nci.cLeds) >= 2;
		}

		bool onFor(int vibratorOnTimeMs)
		{
			if(!available())
			{
				return false;
			}

			NLED_SETTINGS_INFO nsi;
			nsi.LedNum = (INT) VIBRATOR_LED;
			nsi.TotalCycleTime = (INT) 1000 * vibratorOnTimeMs;
			nsi.OnTime = (INT) 1000 * vibratorOnTimeMs;
			nsi.OffTime = (INT) 0;
			nsi.OffOnBlink = (INT) ON;
			nsi.MetaCycleOn = (INT) 1;
			nsi.MetaCycleOff = (INT) 0;

			BOOL couldStart = NLedSetDevice(NLED_SETTINGS_INFO_ID, &nsi);
			if(!couldStart)
			{
				return false;
			}

			/**
			 * Create a timer and turn off the timer after the specified
			 * number of miliseconds. This is because several devices does
			 * not support the BLINK option.
			 * 
			 * Note: Any old timer running will be overwritten.
			 */ 
			UINT_PTR newId = SetTimer(NULL, IDT_VIBRATOR_TIMER, vibratorOnTimeMs, (TIMERPROC) VibrateTimerCallback);
			if(newId != 0)
			{
				return true;
			}
			else
			{
				/* If we do not have the timer, the vibration wont stop,
				 * so stop it immedietley. */
				stop();
				return false;
			}
		}

		void stop()
		{
			NLED_SETTINGS_INFO nsi;

			if(!available())
			{
				return;
			}

			nsi.LedNum = (INT) VIBRATOR_LED;
			nsi.OffOnBlink = (INT) OFF;

			NLedSetDevice(NLED_SETTINGS_INFO_ID, &nsi);
		}

		VOID CALLBACK VibrateTimerCallback(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime)
		{
			stop();
			KillTimer(hwnd, idTimer);
		}
	};
};