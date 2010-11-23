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

#ifndef _VIBRATION_H_
#define _VIBRATION_H_

/**
 * On some windows platforms the NLed.h header is not included, but the
 * actual functions are available. Therefore the contents relevant to the 
 * vibration functionality is included below.
 */
#ifndef NLED_COUNT_INFO_ID
extern "C"
{
#define NLED_COUNT_INFO_ID 0
#define NLED_SUPPORTS_INFO_ID 1
#define NLED_SETTINGS_INFO_ID 2

	struct NLED_COUNT_INFO
	{
		UINT cLeds;
	};

	struct NLED_SUPPORTS_INFO
	{
		UINT LedNum;
		LONG lCycleAdjust;
		BOOL fAdjustTotalCycleTime;
		BOOL fAdjustOnTime;
		BOOL fAdjustOffTime;
		BOOL fMetaCycleOn;
		BOOL fMetaCycleOff;
	};

	struct NLED_SETTINGS_INFO
	{
		UINT LedNum;
		INT OffOnBlink;
		LONG TotalCycleTime;
		LONG OnTime;
		LONG OffTime;
		INT MetaCycleOn;
		INT MetaCycleOff;
	};

	BOOL NLedGetDeviceInfo(INT nID, PVOID pOutput);
	BOOL NLedSetDevice(INT nID, PVOID pOutput);
}
#endif

namespace Base
{
	/**
	 * This namespace handles the vibration of a phone. Since the vibration
	 * functionality is stateless this is a namespace rather than a class.
	 */
	namespace Vibration
	{
		/**
		 * Determines whether the given device supports the vibrator LED.
		 *
		 * @return true if the device supports the vibrator LED, false
		 *         otherwise.
		 */
		bool available();

		/**
		 * Turns on the vibrator for the given number of miliseconds, and
		 * then turns it off. If a vibrator is not available this function
		 * will just return.
		 *
		 * @param vibratorOnTimeMs The number of miliseconds that the vibrator
		 *                         will be turned on.
		 * @return true if the vibration is started, false otherwise.
		 */
		bool onFor(int vibratorOnTimeMs);

		/**
		 * Stops the vibrator regardless whether it is on or off.
		 */
		void stop();
	}
}

#endif /* _VIBRATION_H_ */