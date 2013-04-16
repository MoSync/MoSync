/*
 Copyright (C) 2013 MoSync AB

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
 * @file MoSyncCamController.h
 * @author Mircea Vasiliniuc
 * @date 16 March 2013
 *
 * @brief Application's main controller.
 */

#ifndef MOSYNCCAM_CONTROLLER_H_
#define MOSYNCCAM_CONTROLLER_H_

#include "Screens/CameraScreen.h"
#include "Screens/ImageViewerScreen.h"

namespace MoSyncCamera
{
	class MoSyncCamController:
		public CameraScreenObserver,
		public ImageViewerScreenObserver
	{
	public:
		MoSyncCamController();

		~MoSyncCamController();

		/**
		 * Shows the first screen of the application.
		 */
		void displayMainScreen();

		/**
		 * Displays a snapshot on a separate screen.
		 *
		 * @param imageDataHandle Snapshot image data handle.
		 */
		void displaySnapshot(const MAHandle& imageDataHandle);

		/**
		 * Dismisses the screen showing the snapshot.
		 */
		void dismissSnapshot();

	private:
		/**
		 * Sets the screen transition used when showing/dismissing
		 * a screen.
		 */
		MAWScreenTransitionType setScreenTransitions();

		/**
		 * Retains the displayed screen handle.
		 *
		 * @param currentScreen The screen that will be displayed.
		 */
		void setCurrentScreen( const NativeUI::Screen& currentScreen );

		/**
		 * Returns if the screen given as parameter is displayed.
		 *
		 * @param currentScreen the screen that will be verified.
		 *
		 * @return true if the screen is already displayed, false otherwise.
		 */
		bool isDisplayed( const NativeUI::Screen& currentScreen );

	private:
		CameraScreen* mCameraScreen;

		/**
		 * Snapshot viewer screen.
		 */
		ImageViewerScreen* mImageViewerScreen;

		/**
		 * Transition type used when displaying
		 * screens.
		 */
		MAWScreenTransitionType mForwardTransition;

		/**
		 * Transition type used when dismissing screens.
		 */
		MAWScreenTransitionType mBackwardTransition;

		/**
		 * Used to prevent displaying the same screen twice.
		 */
		MAHandle mDisplayedScreen;
	};
} // MoSyncCamera

#endif /* MOSYNCCAM_CONTROLLER_H_ */
