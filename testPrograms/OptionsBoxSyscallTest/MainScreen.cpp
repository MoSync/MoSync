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
 * @file MainScreen.cpp
 * @author Bogdan Iusco.
 */

#include "MainScreen.h"

#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>

static const wchar_t* sOtherButtonTitles[] =
{
    L"Button 2",
    L"Button 3",
};

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	Screen(),
	mMainLayout(NULL),
	mButton(NULL)
{
	createMainLayout();
	mButton->addButtonListener(this);

	OptionsBox* optionsBox = OptionsBox::getInstance();
	optionsBox->addOptionsBoxListener(this);

}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
	mButton->removeButtonListener(this);

	OptionsBox* optionsBox = OptionsBox::getInstance();
	optionsBox->removeOptionsBoxListener(this);
}

/**
 * This method is called when there is an touch-down event for
 * a button.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonPressed(Widget* button)
{
	if (button == mButton)
	{
		printf("button event");
	}

}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(0xC1FFC1);
	Screen::setMainWidget(mMainLayout);

	mButton = new Button();
	mButton->setText("Show Options dialog");
	mButton->wrapContentHorizontally();
	mMainLayout->addChild(mButton);

	mLabel = new Label();
	mMainLayout->addChild(mLabel);

}

/**
 * This method is called when there is an touch-up event for
 * a button.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonReleased(Widget* button)
{
    if (button == mButton)
    {
        printf("mButton pointerReleasedEvent");
    }
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonClicked(Widget* button)
{
	if (button == mButton)
	{
		printf("mButton buttonClickedEvent");
		OptionsBox* optionsBox = OptionsBox::getInstance();
		optionsBox->resetDialog();
		optionsBox->setTitle(L"My dialog");
		optionsBox->setDestructiveButtonTitle(L"Delete");
		optionsBox->setDestructiveButtonVisible(true);
		optionsBox->setCancelButtonTitle(L"Cancel");
		optionsBox->setCancelButtonVisible(true);
		optionsBox->addButton(L"Button 1");
		optionsBox->addButton(L"Button 2");
		MAUtil::WString btn3 = L"Button 3";
		optionsBox->addButton(btn3);
		optionsBox->show();
	}
}

/**
 * Write n wchar-arrays to buffer.
 * @param buffer The given buffer.
 * The first value in buffer will be the number of wchar-arrays(n argument).
 * @param src Contains the n wchar-arrays that will be written into buffer.
 * @return The number of written bytes.
 */
int MainScreen::writeWCharArraysToBuf(
    void* buffer,
    const wchar_t** src,
    const int n)
{
    char* charBuffer = (char*) buffer;
    *(int*) charBuffer = n;
    wchar_t* dst = (wchar_t*) (charBuffer + sizeof(int));
    for (int i = 0; i < n; i++)
    {
        const wchar_t* array = src[i];
        while (true)
        {
            *dst = *array;
            dst++;
            if (*array == 0)
                break;
            array++;
        }
    }

    return ((char*) dst - charBuffer + sizeof(int));
}

/**
 * This method is called when the destructive button from options
 * dialog was clicked.
 */
void MainScreen::optionsBoxDestructiveButtonClicked()
{
	printf("destructive button pressed");
	mLabel->setText("Destructive button was hit");
}

/**
 * This method is called when the cancel button from options dialog was
 * clicked.
 */
void MainScreen::optionsBoxCancelButtonClicked()
{
	printf("cancel button pressed");
	mLabel->setText("cancel was hit");
}

/**
 * This method is called when a button from options dialog was
 * clicked.
 * This method is not called if the destructive or cancel button were
 * clicked.
 * @param buttonIndex The index of the button that was clicked.
 * @param buttonTitle The title of the button that was clicked.
 */
void MainScreen::optionsBoxButtonClicked(
		const int buttonIndex,
		const MAUtil::WString& buttonTitle)
{
	printf("%S button pressed. buttonIndex = %d", buttonTitle.pointer(), buttonIndex);
	mLabel->setText("Button on index " + MAUtil::integerToString(buttonIndex) + " was hit");
}
