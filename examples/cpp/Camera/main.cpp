#include <ma.h>
#include <mavsprintf.h>
#include <MAUtil/Moblet.h>
#include <NativeUI/WidgetUtil.h>

using namespace MAUtil;
using namespace NativeUI;

#include "MoSyncCamController.h"

namespace MoSyncCamera
{
	/**
	 * Moblet to be used as a template for a Native UI application.
	 */
	class NativeUIMoblet : public Moblet
	{
	public:
		/**
		 * The constructor creates the user interface.
		 */
		NativeUIMoblet()
		{
			mAppController = new MoSyncCamController();

			mAppController->displayMainScreen();
		}

		/**
		 * Destructor.
		 */
		virtual ~NativeUIMoblet()
		{
			delete mAppController;
		}

		/**
		 * Called when a key is pressed.
		 */
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
		 * Application controller
		 */
		MoSyncCamController* mAppController;
	};
} // CameraDemo

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	Moblet::run(new MoSyncCamera::NativeUIMoblet());
	return 0;
}
