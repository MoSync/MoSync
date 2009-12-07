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

/** \file CharInput.h
* \brief C-based text input system with C++ wrappers.
*/

#ifndef _SE_MSAB_MAUTIL_CHARACTER_INPUT_H_
#define _SE_MSAB_MAUTIL_CHARACTER_INPUT_H_

#include <ma.h>

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * Definitions for the different input modes. 
 */
#define CI_MODE_CHAR_MODE_LOWERCASE 0
#define CI_MODE_CHAR_MODE_UPPERCASE 1
#define CI_MODE_CHAR_MODE_NUMBERS 2

/** 
 * Reset system. 
 */
void CharInput_Reset(void);

/** 
 * Notify the charinput system that a key has been pressed with the keycode 'keyCode'. 
 */
void CharInput_Pressed(int keyCode);

/** 
 * Notify the charinput system that a key has been released with the keycode 'keyCode'. 
 */
void CharInput_Released(int keyCode);

/** 
 * The user must implement a StartTimerCallback and a StopTimerCallback.
 * These are used so that the system can create a timer. When the timer has 
 * reached its timeout it must call the CharInputCallback passed to the 
 * StartTimerCallback. 
 */
typedef void (*CharInputCallback)(void);

/** 
 * The StartTimerCallback is used for starting the timer, the timer should call the argument
 * CharInputCallback after time 'time'. 
 */
typedef void (*StartTimerCallback)(CharInputCallback, int time);

/** 
 * The StopTimerCallback is used for stopping the timer. 
 */
typedef void (*StopTimerCallback)(void);

/** 
 * This function is used to register the timer callbacks, these are used to be able to start and stop a 
 * system specific timer. The user should start a timer when the 'startCallback' is called and stop that 
 * timer if 'stopCallback' is called. 
 */
void CharInput_RegisterTimerCallback(StartTimerCallback startCallback, StopTimerCallback stopCallback);

/** 
 * Use this to retrieve information about what chars can be 
 * currently selected. 
 */
const char* CharInput_getCurrentCharList(void);

/** 
 * Use this to get where in the list the current char is. 
 */
int CharInput_getCurrentCharListIndex(void);

/** 
 * This is a callback used to notify the user about character input updates. 
 */
typedef void (*CharCallback)(char c, void *userData);

/** 
 * Use this function to register a callback that is called everytime a character change has happened. 
 */
void CharInput_RegisterCharacterChangedCallback(CharCallback callback, void *userData);

/** 
 * Use this function to register a callback that is called everytime a character has been deployed. 
 */
void CharInput_RegisterCharacterDeployedCallback(CharCallback callback, void *userData);

/**
* force the currently selected character to be deployed.
*/
void CharInput_ForceDeployment(void);

/**
* Set character input mode using one of the defines: CI_MODE_CHAR_MODE_LOWERCASE, CI_MODE_CHAR_MODE_UPPERCASE, CI_MODE_CHAR_MODE_NUMBERS.
*/
void CharInput_setMode(int charMode);

/** 
 * Get the character input mode.
 */
int CharInput_getMode(void);

/**
* In standard mode, only the 0-9, # and * keys are used.
* This is suitable for devices with such keypads.
*
* In qwerty mode, all keys are used.
* This is suitable for devices with qwerty keyboards.
*/
void CharInput_setQwerty(BOOL on);

/** Get the qwerty mode. */
BOOL CharInput_getQwerty(void);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include "ListenerSet.h"
#include "Environment.h"

namespace MAUtil {

/**
 * A listener for character input events.	
 */
class CharInputListener {
public:
	virtual void characterChanged(char c) = 0;
	virtual void characterDeployed(char c) = 0;
};


/**
 * \brief A C++ wrapper for the text input system.

 * This can only be used together with the Moblet system as it depends
 * on functionality in Environment (key input and timers).

 * This class is a singleton. Retrieve its instance with getCharInput().
 */
class CharInput : public KeyListener, TimerListener {
public:

	/** Enumerator for the different input modes. */
	enum CharMode {
		LOWERCASE = 0,
		UPPERCASE = 1,
		NUMBERS = 2
	};

	/** is a CharInputListener? */
	bool isCharInputListener(CharInputListener *listener);

	/** Add a CharInputListener to the system. */
	void addCharInputListener(CharInputListener *listener);

	/** Remove a CharInputListener from the system. */
	void removeCharInputListener(CharInputListener *listener);

	/** Inherited function from class Keylistener, updates the system. */
	virtual void keyPressEvent(int keyCode);

	/** Inherited function from class Keylistener, updates the system. */
	virtual void keyReleaseEvent(int keyCode);

	/** Inherited function from class TimerListener, updates the system. */
	void runTimerEvent();

	/** Get the charinput singleton. (only one instance can exist). */
	static CharInput& getCharInput();

	/** Force deployment of the currently selected character. */
	void forceDeployment();

	/** Set the character input mode. */
	void setMode(CharMode charMode);

	/** Get the character input mode. */
	CharMode getMode() const;

	/** \copydoc CharInput_setQwerty */
	void setQwerty(bool on);

	/** \copydoc CharInput_getQwerty */
	bool getQwerty() const;

private:
	CharInput();

	void enable();
	void disable();
	bool enabled;

	static CharInput *charInputSingleton;

	static void startTimer(CharInputCallback c, int time);
	static void stopTimer();
	static void characterChangedCallback(char c, void *userData);
	static void characterDeployedCallback(char c, void *userData);

	CharInputCallback charInputCallback;
	ListenerSet<CharInputListener> charInputListeners;
};

} // end namespace MAUtil

#endif /* __cplusplus */

#endif /* _SE_MSAB_MAUTIL_CHARACTER_INPUT_H_ */
