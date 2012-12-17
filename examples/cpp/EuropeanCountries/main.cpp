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
 * @file main.cpp
 * @author Bogdan Iusco
 *
 * @brief Application entry point.
 */

#include <MAUtil/Moblet.h>

#include "Controller/Controller.h"

namespace EuropeanCountries
{

	/**
	 * Application moblet.
	 */
	class NativeUIMoblet : public MAUtil::Moblet
	{
	public:
		/**
		 * Constructor.
		 */
		NativeUIMoblet()
		{
			mController = new EuropeanCountries::Controller();
		}

		/**
		 * Called when a key is pressed.
		 */
		void keyPressEvent(int keyCode, int nativeCode)
		{
			if (MAK_BACK == keyCode || MAK_0 == keyCode)
			{
				mController->backButtonPressed();
			}
		}

	private:
		/**
		 * Application controller.
		 */
		Controller* mController;
	};

} // end of EuropeanCountries

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	MAUtil::Moblet::run(new EuropeanCountries::NativeUIMoblet());
	return 0;
}
