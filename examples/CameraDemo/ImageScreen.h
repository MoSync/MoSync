// Include MoSync syscalls.
#include <maapi.h>
#include <IX_WIDGET.h>
#ifndef IMAGESCREEN_H_
#define IMAGESCREEN_H_



class ImageScreen
{
public:
	ImageScreen():isViewed(false)
	{
		//do nothing
	}
	virtual ~ImageScreen()
	{
		maWidgetDestroy(mScreen);
	}

	void customEvent(const MAEvent& event);

	int initialize(MAHandle stackScreen);

	void pushImageScreen();

	void setImageDataHandle(MAHandle dataHandle);

	bool isViewed;

private:

	void createUI();


	MAHandle mScreen;


	MAHandle mOKButton;

	MAHandle mMainLayoutWidget;

	MAHandle mStackScreen;

	MAHandle mImageWidget;

	MAHandle dataHandle;

	MAHandle imageHandle;


};


#endif /* IMAGESCREEN_H_ */
