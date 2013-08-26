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
