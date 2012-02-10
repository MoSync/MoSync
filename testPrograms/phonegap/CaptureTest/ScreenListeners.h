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
 * @file MainScreen.h
 * @author Bogdan Iusco
 *
 * @brief File containing screen listeners.
 */

#ifndef SCREENLISTENERS_H_
#define SCREENLISTENERS_H_

/**
 * Listener for SettingsScreen.
 */
class SettingsScreenListener
{
public:
	/**
	 * Used for showing the video quality screen.
	 */
	virtual void showVideoQualityScreen() = 0;

	/**
	 * Used for showing a screen containing a taken image.
	 */
	virtual void showImageScreen() = 0;

	/**
	 * Used for showing a screen containing the latest recorded video.
	 */
	virtual void showVideoScreen() = 0;

	/**
	 * Used for showing the flash mode screen.
	 */
	virtual void showFlashModeScreen() = 0;
};


#endif /* SCREENLISTENERS_H_ */
