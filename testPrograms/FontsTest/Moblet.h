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
#include <MAUtil/String.h>
#include <MAUtil/Vector.h>

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

	void testLoadWithName();

	void testLoadDefault();

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

	virtual void pointerPressEvent(MAPoint2d p);

	/**
	 * This method is called when the application is closed.
	 */
	void closeEvent();

private:
	static TestMoblet *mInstance;

	MAUtil::Vector<MAUtil::String> fontNames;
	MAUtil::String message;
	int nrFonts;
	// mShowing = 1 for list of fonts
	// mShowing = 2 for test maFontLoad
	// mShowing = 3 for load default combinations
	int mShowing;

};

}// namespace Test

#endif /* MOBLET_H_ */
