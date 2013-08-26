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

/**
 * @file ThirdScreen.h
 * @author emma
 *
 */

#ifndef THIRD_SCREEN_H_
#define THIRD_SCREEN_H_

#include <MAUtil/Vector.h>
#include "NativeUI/Widgets.h"

using namespace NativeUI;

class ThirdScreen :
	public Screen,
	EditBoxListener
{
public:

	/**
	 * Constructor.
	 */
	ThirdScreen();

	/**
	 * Destructor.
	 */
	virtual ~ThirdScreen();

private:
	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();
    /**
     * This method is called when the return button was pressed.
     * On iphone platform the virtual keyboard is not hidden after
     * receiving this event.
     * @param editBox The edit box object that generated the event.
     */
    virtual void editBoxReturn(EditBox* editBox);

    /**
     * This method is called when an edit box gains focus.
     * The virtual keyboard is shown.
     * Only for iphone platform.
     * @param editBox The edit box object that generated the event.
     */
    virtual void editBoxEditingDidBegin(EditBox* editBox);

    /**
     * This method is called when an edit box loses focus.
     * The virtual keyboard is hidden.
     * Only for iphone platform.
     * @param editBox The edit box object that generated the event.
     */
    virtual void editBoxEditingDidEnd(EditBox* editBox);
private:
    Screen* mScreen;
	/**
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;
	EditBox* mEditBox;
    /**
     * The list view widget.
     */
    ListView* mEventsList;
};

#endif
