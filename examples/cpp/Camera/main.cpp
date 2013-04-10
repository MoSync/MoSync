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
		/* FIXME: remove this
		 * The constructor creates the user interface.
		 */
		NativeUIMoblet()
		{
			mAppController = new MoSyncCamController();

			mAppController->displayMainScreen();
		}

		/* FIXME: remove this
		 * Destructor.
		 */
		virtual ~NativeUIMoblet()
		{
			delete mAppController;
		}

		/* FIXME: remove this comment
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
		/* FIXME: remove this comment
		 * Application controller
		 */
		MoSyncCamController* mAppController;	// FIXME: very dangerous naming, keep camera controller
	};
} // CameraDemo

/* FIXME: remove this
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	// FIXME: this is a bad habbit which should not be promoted by MoSync developers.
	// it's one line of code which does almost everything together with a leak - even if we rely on OS for cleanup.
	Moblet::run(new MoSyncCamera::NativeUIMoblet());
	
	// ALTERNATIVE:
	MoSyncCamera::NativeUIMoblet *cameraMoblet = new MoSyncCamera::NativeUIMoblet();
	
	// here you have the opportunity to check if construction went well
	
	Moblet::run( cameraMoblet );
	
	// here you have the oppotunity to release objects, timers and so on... aka clean-up
	delete cameraMoblet;
	
	// and only now have a gracefull exit.
	return 0;
}
