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

#include "CharInput.h"

namespace MAUtil {

	CharInput *CharInput::charInputSingleton = NULL;

	CharInput::CharInput() :
		enabled(false), charInputListeners(false)
		{
		if(charInputSingleton) {
			PANIC_MESSAGE("The application tried to instantiate more than one CharInput. There can be only one per application.");
		}
		CharInput_RegisterCharacterChangedCallback(CharInput::characterChangedCallback, NULL);
		CharInput_RegisterCharacterDeployedCallback(CharInput::characterDeployedCallback, NULL);
		CharInput_RegisterTimerCallback(CharInput::startTimer, CharInput::stopTimer);

		charInputSingleton = this;
	}

	CharInput& CharInput::getCharInput() {
		if(!charInputSingleton) {
			return *(new CharInput());
		} else {
			return *charInputSingleton;
		}
	}

	void CharInput::enable() {
		CharInput_Reset();
		Environment::getEnvironment().addKeyListener(this);
	}

	void CharInput::disable() {
		Environment::getEnvironment().removeKeyListener(this);
		Environment::getEnvironment().removeTimer(this);
	}

	bool CharInput::isCharInputListener(CharInputListener *listener) {
		return charInputListeners.contains(listener);
	}

	void CharInput::addCharInputListener(CharInputListener *listener) {
		charInputListeners.add(listener);
		if(charInputListeners.size()>0 && !enabled) enable();
	}

	void CharInput::removeCharInputListener(CharInputListener *listener) {
		charInputListeners.remove(listener);
		if(charInputListeners.size()==0 && enabled) disable();
	}

	void CharInput::keyPressEvent(int keyCode, int nativeCode) {
		CharInput_Pressed(keyCode);
	}

	void CharInput::keyReleaseEvent(int keyCode, int nativeCode) {
		CharInput_Released(keyCode);
	}

	void CharInput::charEvent(uint character) {
		CharInput_Char(character);
	}

	void CharInput::runTimerEvent() {
		charInputCallback();
	}

	void CharInput::startTimer(CharInputCallback c, int time) {
		Environment::getEnvironment().addTimer(charInputSingleton, time, 1);
		charInputSingleton->charInputCallback = c;
	}

	void CharInput::stopTimer() {
		Environment::getEnvironment().removeTimer(charInputSingleton);
	}

	void CharInput::characterChangedCallback(char c, void *userData) {
		charInputSingleton->charInputListeners.setRunning(true);
		ListenerSet_each(CharInputListener, i, charInputSingleton->charInputListeners) {
			(*i).characterChanged(c);
		}
		charInputSingleton->charInputListeners.setRunning(false);
	}

	void CharInput::characterDeployedCallback(char c, void *userData) {
		charInputSingleton->charInputListeners.setRunning(true);
		ListenerSet_each(CharInputListener, i, charInputSingleton->charInputListeners) {
			(*i).characterDeployed(c);
		}
		charInputSingleton->charInputListeners.setRunning(false);
	}

	void CharInput::forceDeployment() {
		CharInput_ForceDeployment();
	}

	void CharInput::setMode(CharInput::CharMode charMode) {
		CharInput_setMode((int)charMode);
	}

	CharInput::CharMode CharInput::getMode() const {
		return (CharInput::CharMode)CharInput_getMode();
	}

	void CharInput::setQwerty(bool on) {
		CharInput_setQwerty(on);
	}

	bool CharInput::getQwerty() const {
		return !!CharInput_getQwerty();
	}

} // namespace MAUtil
