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
 * @date 1 Mar 2012
 *
 * @brief Application entry point.
 */

#include <MAUtil/Moblet.h>

#include "Controller/ApplicationController.h"


/**
 * Moblet to be used as a template for a Native UI application.
 */
class NativeUIMoblet : public MAUtil::Moblet
{
public:
	/**
	 * The constructor creates the user interface.
	 */
	NativeUIMoblet()
	{
		mApplicationController = new SensorNativeUI::ApplicationController();
	}

	/**
	 * Destructor.
	 */
	virtual ~NativeUIMoblet()
	{
		delete mApplicationController;
	}

	void keyPressEvent(int keyCode, int nativeCode)
	{
		if (MAK_BACK == keyCode || MAK_0 == keyCode)
		{
			// Call close to exit the application.
			close();
		}
	}

private:
	/**
	 * Application controller.
	 */
	SensorNativeUI::ApplicationController* mApplicationController;
};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	MAUtil::Moblet::run(new NativeUIMoblet());
	return 0;
}
