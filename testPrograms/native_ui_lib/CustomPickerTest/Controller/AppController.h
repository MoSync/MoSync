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
 * @file AppController.h
 * @author Bogdan Iusco
 *
 * @brief Application controller.
 */

#ifndef CPT_APP_CONTROLLER_H_
#define CPT_APP_CONTROLLER_H_

// Forward declarations for NativeUI.
namespace NativeUI
{
	class TabScreen;
}

namespace CustomPickerTest
{

	// Forward declarations
	class CustomPickerScreen;
	class PropertiesScreen;

	/**
	 * @brief Application controller.
	 */
	class AppController
	{
	public:
		/**
		 * Constructor.
		 */
		AppController();

		/**
		 * Destructor.
		 */
		~AppController();

	private:
		/**
		 * Main screen shown.
		 */
		NativeUI::TabScreen* mTabScreen;

		/**
		 * Used to show the tested CustomPicker.
		 * 1st tab screen.
		 */
		CustomPickerScreen* mCustomPickerScreen;

		/**
		 * Used for testing CustomPicker properties.
		 */
		PropertiesScreen* mPropertiesScreen;

	};// end of AppController

} // end of CustomPickerTest

#endif /* CPT_APP_CONTROLLER_H_ */
