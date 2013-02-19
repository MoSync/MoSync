/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/**
 * @file SettingsScreen.h
 * @author Ali Sarrafi
 *
 * This application provides a very basic example of how to work
 * with Native UI and the Camera API
 */

// Include MoSync syscalls.
#include <maapi.h>

// Include NativeUI class interface
#include <NativeUI/Widgets.h>

#ifndef SETTINGSSCREEN_H_
#define SETTINGSSCREEN_H_

using namespace NativeUI;

/**
 * A class that encapsulates the behavior of Settings Screen
 */
class SettingsScreen : public ButtonListener
{
public:
	SettingsScreen();
	virtual ~SettingsScreen();
	virtual void buttonClicked(Widget* button);
	void initializeUI(StackScreen* stackScreen);
	void createUI();
	void setFlashSupported(bool flashSupported);
	void pushSettingsScreen();
	int getCurrentCamera();
	void setCurrentCamera(int cameraIndex);
	const char* getFlashMode();
	const char* getModeForIndex(int index);

private:
	bool mFlashSupported;
	int mFlashModeIndex;
	int mNumCameras;
	int mCurrentCamera;
	Screen* mScreen;
	Button* mSwapCameraButton;
	Button* mOKButton;
	Button* mFlashModeButton;
	VerticalLayout *mMainLayoutWidget;
	StackScreen* mStackScreen;
};

#endif /* SETTINGSSCREEN_H_ */
