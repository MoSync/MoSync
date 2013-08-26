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
