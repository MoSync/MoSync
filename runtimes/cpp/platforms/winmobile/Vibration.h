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