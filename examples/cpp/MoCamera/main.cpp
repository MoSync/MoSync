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

#include <ma.h>
#include <MAUtil/Moblet.h>
#include <NativeUI/WidgetUtil.h>

using namespace MAUtil;
using namespace NativeUI;

#include "MoCameraController.h"

namespace MoSyncCamera
{
	/**
	 * Moblet to be used as a template for a Native UI application.
	 */
	class CameraMoblet : public Moblet
	{
	public:
		CameraMoblet()
		{
			mMoSyncCameraController = new MoCameraController();

			mMoSyncCameraController->displayMainScreen();
		}

		virtual ~CameraMoblet()
		{
			delete mMoSyncCameraController;
		}

		/**
		 * Called when a key is pressed.
		 */
		void keyPressEvent(int keyCode, int nativeCode)
		{
			if (MAK_BACK == keyCode || MAK_0 == keyCode)
			{
				mMoSyncCameraController->handleBackPress();
			}
		}

	private:
		MoCameraController* mMoSyncCameraController;
	};
} // MoSyncCamera

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	MoSyncCamera::CameraMoblet *cameraMoblet = new MoSyncCamera::CameraMoblet();

	Moblet::run( cameraMoblet );

	delete cameraMoblet;

	return 0;
}
