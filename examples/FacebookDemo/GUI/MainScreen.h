/*
 * MainMenu.h
 *
 *  Created on: Sep 3, 2011
 *      Author: gabi
 */

#ifndef MAINMENU_H_
#define MAINMENU_H_


#include <MAUtil/Moblet.h>
#include "ListScreen.h"

namespace FacebookDemoGUI
{

class MainScreen: public ListScreen
{
public:
	MainScreen(MAUtil::Moblet *moblet);
	/**
	 * Override of KeyListener
	 * Called when a key is pressed.
	 *@param keyCode - the code of the key pressed
	 */
	virtual void keyPressEvent(int keyCode, int nativeCode);

	virtual void buttonClicked(Widget* button);

private:
	void closeApplication();

private:
	MAUtil::Moblet *mAppMoblet;
};

}

#endif /* MAINMENU_H_ */
