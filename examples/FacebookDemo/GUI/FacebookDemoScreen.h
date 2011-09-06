/*
 * FacebookDemoScreen.h
 *
 *  Created on: Aug 8, 2011
 *      Author: gabi
 */

#ifndef FACEBOOKDEMOSCREEN_H_
#define FACEBOOKDEMOSCREEN_H_

#include <MAUtil/Environment.h>
#include <NativeUI/Screen.h>

namespace FacebookDemoGUI
{

class FacebookDemoScreen: public NativeUI::Screen, public MAUtil::KeyListener
{
public:
	FacebookDemoScreen(FacebookDemoScreen *prevScreen);
	virtual void show();
	void receiveKeyEvents(bool receive);

	/**
	 * Called when a key is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode);

	FacebookDemoScreen *getPreviousScreen();

	virtual ~FacebookDemoScreen();

protected:
	virtual void back();

protected:
	FacebookDemoScreen *mPreviousScreen;
	bool				mBusy;
};


}//namespace FacebookDemoGUI


#endif /* FACEBOOKDEMOSCREEN_H_ */
