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
