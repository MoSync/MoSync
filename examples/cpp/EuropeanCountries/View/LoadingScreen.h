/*
 Copyright (C) 2012 MoSync AB

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
 * @file LoadingScreen.h
 * @author Bogdan Iusco
 *
 * @brief NativeUI Screen used to show an ActivityIndicatior.
 * It does not interact with the user, used only when the application
 * is reading data from files.
 */

#ifndef COUNTRIES_LOADING_SCREEN_H_
#define COUNTRIES_LOADING_SCREEN_H_

#include <NativeUI/Screen.h>

namespace NativeUI
{
	class VerticalLayout;
}

namespace EuropeanCountries
{

	/**
	 * @brief Show an ActivityIndicator to the user.
	 */
	class LoadingScreen:
		public NativeUI::Screen
	{
	public:
		/**
		 * Constructor.
		 */
		LoadingScreen();

	private:
		/**
		 * Create screen's UI.
		 */
		void createUI();

	private:
		/**
		 * Screen's main layout.
		 */
		NativeUI::VerticalLayout* mMainLayout;
	}; // end of LoadingScreen

} // end of EuropeanCountries

#endif /* COUNTRIES_LOADING_SCREEN_H_ */
