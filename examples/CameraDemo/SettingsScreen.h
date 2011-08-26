// Include MoSync syscalls.
#include <maapi.h>
#include <IX_WIDGET.h>
#ifndef SETTINGSSCREEN_H_
#define SETTINGSSCREEN_H_



class SettingsScreen
{
public:
	SettingsScreen():currentCamera(0)
	{
		//do nothing
	}
	virtual ~SettingsScreen()
	{
		maWidgetDestroy(mScreen);
	}

	void customEvent(const MAEvent& event);

	int initialize(MAHandle stackScreen, MAHandle previewWidget);

	void pushSettingsScreen();

	int getCurrentCamera();

	char * getFLashMode();

	bool isViewed;

	int flashSupported;

	int numCameras;

private:

	void createUI();

	char * getModeForIndex(int index);

	MAHandle mScreen;

	MAHandle mflashTitle;

	MAHandle mSwapCameraButton;

	MAHandle mOKButton;

	MAHandle mFlashModeButton;

	MAHandle mMainLayoutWidget;

	MAHandle mStackScreen;

	MAHandle mPreviewWidget;



	int zoomSupported;



	int flashModeIndex;

	int currentCamera;



};


#endif /* SETTINGSSCREEN_H_ */
