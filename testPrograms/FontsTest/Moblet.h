/*
 * Moblet.h
 *
 *  Created on: May 11, 2011
 *      Author: emma tresanszki
 */

#ifndef MOBLET_H_
#define MOBLET_H_

// Include the syscalls library.
#include <maapi.h>

// Include the library for the event manager,
// which manages the application main loop.
#include <MAUtil/Moblet.h>

namespace Test
{

/*
 * The Moblet that will manage the application and handle events
 */
class TestMoblet : public MAUtil::Moblet
{
	/*
	 * ctor
	 */
	TestMoblet();

public:

	void testFont (int family, int style, int size, int top);

	void checkFont(int left, int top,MAHandle font);
	/**
	 * Get the single instance of this Moblet.
	 * @return The Moblet instance.
	 */
	static TestMoblet *getInstance();

	/**
	 * Method called when a key is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode);

	/**
	 * This method is called when the application is closed.
	 */
	void closeEvent();

private:
	static TestMoblet *mInstance;

};

}// namespace Test

#endif /* MOBLET_H_ */
