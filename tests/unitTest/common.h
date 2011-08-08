/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

#ifndef COMMON_H
#define COMMON_H

#include <MATest/Test.h>
#include <MAUtil/Environment.h>
#include <MAUtil/CharInput.h>

using namespace MATest;
using namespace MAUtil;

#define TK_YES MAK_SOFTLEFT
#define TK_NO MAK_SOFTRIGHT
#define RGB(r, g, b) (((r) << 16) | ((g) << 8) | (b))
#define BLACK 0
#define GREEN 0x00FF00
#define RED 0xFF0000
#define BLUE 0x0000FF
#define WHITE 0xFFFFFF
#define DATA_SIZE (4*1024)

enum eFuncReturnCodes
{
	FUNC_SYSCALL_NOT_SUPPORTED = -4,
	FUNC_SYSCALL_ERROR = -3,
	FUNC_OUT_OF_MEMORY_ALLOC = -2,
	FUNC_OUT_OF_MEMORY_RESOURCE = -1,
	FUNC_OK = 0
};

/**
 * @brief Testcase class which are controlled either by keys or touch.
 */
class KeyBaseCase : public TestCase, public KeyListener,
					public PointerListener
{
public:
	KeyBaseCase(const String& name) : TestCase(name), mHasErrors(false)
	{}

	/**
	 * @brief When the test is opened the key and touch listeners are added
	 */
	virtual void open()
	{
		Environment::getEnvironment().addKeyListener(this);
		Environment::getEnvironment().addPointerListener(this);
	}

	/**
	 * @brief When closed the listeners are removed
	 */
	virtual void close()
	{
		Environment::getEnvironment().removeKeyListener(this);
		Environment::getEnvironment().removePointerListener(this);
	}

	/**
	 * @brief Handles the pointer pressed events
	 *
	 * Pointer press events are not handled
	 */
	void pointerPressEvent(MAPoint2d p)
	{
	}

	/**
	 * @brief Handles the pointer moved events
	 *
	 * Pointer moved events are not handled
	 */
	void pointerMoveEvent(MAPoint2d p)
	{
	}

	/**
	 * @brief Handles the pointer released event
	 *
	 * If the test has reported an error the next test will be
	 * shown since the result has already been reported.
	 * If no error was recorded the left portion of the screen
	 * will trigger a succes, while the right will trigger failure.
	 */
	void pointerReleaseEvent(MAPoint2d p)
	{
		if(!mHasErrors)
		{
			int res = ((p.x - (EXTENT_X(maGetScrSize())/ 2)) < 0) ? TK_YES : TK_NO;
			assert(name, res == TK_YES);
		}
		suite->runNextCase();
	}

	/**
	 * @brief Checks the key code and sends the corresponding succes code
	 *
	 * If left soft key was pressed a success will be logged,
	 * while right soft key will report a failure.
	 */
	void checkYesNo(int keyCode);

	/**
	 * @brief Shows an error message on the screen
	 *
	 * This screen shows an error message if any of the
	 * tests failed to allocate memory, or failed to
	 * generate a new resource. The messeage is based
	 * on which error code which was recieved. The error
	 * codes are any of the eFuncReturnCodes code.
	 *
	 * @param errorMessage	The error message to be shown
	 */
	void showErrorScreen(int errorCode);

protected:
	bool mHasErrors;
};

/**
 * @brief Class that handles a KeyBaseCase test
 *
 * The class gets a function pointer, which is the actual
 * test it will run. This class runs the function and handles
 * key presses.
 */
template<int func()> class TemplateCase : public KeyBaseCase {
public:
	TemplateCase(const String& name) : KeyBaseCase(name) {}

	/**
	 * @brief Runs the test and executes the test function
	 *
	 * If the test function return with FUNC_OK it means
	 * the the test was successfully shown. Any other return
	 * code will flag that this test had an error, report
	 * that it failed and show the error screen.
	 */
	virtual void start()
	{
		int returnCode = func();
		if(FUNC_OK != returnCode)
		{
			mHasErrors = true;
			assert(name, false);
			showErrorScreen(returnCode);
		}
	}

	/**
	 * @brief Handles key pressed events
	 *
	 * Key press events are not handled.
	 */
	virtual void keyPressEvent(int keyCode)
	{
	}

	/**
	 * @brief Handles key released events
	 *
	 * If an error was not reported while the test function was running
	 * the checkYesNo function will be called, and the result of that
	 * will be reported.
	 * If an error did occur, the next test will be shown.
	 */
	virtual void keyReleaseEvent(int keyCode)
	{
		if(!mHasErrors)
			checkYesNo(keyCode);

		suite->runNextCase();
	}
};

class CharInputBaseCase : public TestCase, public CharInputListener {
public:
	CharInputBaseCase(const String& name) : TestCase(name) {}

	//TestCase
	virtual void open() {
		MAUtil::CharInput::getCharInput().addCharInputListener(this);
	}
	virtual void close() {
		MAUtil::CharInput::getCharInput().removeCharInputListener(this);
	}
};

template<int func()> class TemplateCharInputCase : public CharInputBaseCase {
public:
	TemplateCharInputCase(const String& name) : CharInputBaseCase(name) {}

	//TestCase
	virtual void start() {
		func();
	}

	// CharInputListener
	virtual void characterChanged(char c)
	{
	}

	virtual void characterDeployed(char c)
	{
	}
};


struct Dimensions {
	unsigned short width, height;
};

void clearScreen(int color=BLACK);

#endif	//COMMON_H
