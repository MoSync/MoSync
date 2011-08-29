/*
 * NativeScreen.h
 *
 *  Created on: May 11, 2011
 *      Author: emma tresanszki
 */

#ifndef NATIVESCREEN_H_
#define NATIVESCREEN_H_

// Include MoSync NativeUI API.
#include <IX_WIDGET.h>

// Include the library for event handling listener.
#include "MAUtil/Environment.h"

#include <MAUtil/String.h>

namespace Test
{

// Colors used for fonts & background.
enum Colors
{
	SEA_GREEN = 0xC1FFC1 ,
	DARK_SEA_GREEN = 0x9BCD9B ,
	LIGHT_GRAY = 0xD9D9D9 ,
	BLUE       = 0x104E8B
};

/*
 * Native UI screen for testing some widgets.
 */
class NativeScreen :
	public MAUtil::CustomEventListener
{
public:
	/**
	 * Constructor for the main screen class.
	 * Here instance variables are initialized.
	 */
	NativeScreen();

	/**
	 * The destructor deletes the main widget
	 * and all of its children.
	 */
	virtual ~NativeScreen();

	/**
	 * Show the screen with the game user interface.
	 * NOTE: If we had more screens, it could have been necessary
	 * to do some UI initialization here, before the screen is displayed.
	 */
	void showScreen();

	/**
	 * Handle widget events.
	 * @param event A MoSync event data structure.
	 */
	void customEvent(const MAEvent& event);

	/**
	 * Handle widget click events.
	 * @param widgetHandle The handle (reference) to the widget
	 * that was clicked.
	 */
	void widgetClicked(MAHandle widgetHandle);

	/**
	 * Utility function to create a label.
	 * The height of the label is computed automatically.
	 * @param width Label width.
	 * @param fontColor Text color.
	 * @param fontSize Text size.
	 * @param text Label text.
	 * @return Handle to the new label.
	 */
	MAWidgetHandle createLabel(
		int width,
		int fontColor,
		int fontSize,
		const char* text);

	/*
	 * Set a text for a label.
	 * @param aLabel Handle to the label.
	 * @param aText Label text.
	 */
	void setLabelText(MAWidgetHandle aLabel, MAUtil::String aText);

	/*
	 * Apply an int property for a widget.
	 * @param aWidget The handle for the widget.
	 * @param aProperty A string representing which property to set.
	 * @param aValue The value which will be assigned to the property.
	 * @param base  The output radix. Default is 10, 16 is to be used for color properties.
	 *
	 * * \returns Any of the following result codes:
	 * - #MAW_RES_OK if the property could be set.
	 * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
	 * - #MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
	 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
	 * - #MAW_RES_ERROR otherwise.
	 */
	int setWidgetProperty(MAWidgetHandle aWidget, const char* aProperty, int aValue, int base=10);

	/**
	 * Creates a button with centered text by default,
	 * font size is 20, and color is DARK GREY.
	 * @param text The button text.
	 * @return Handle to the new button.
	 */
	MAWidgetHandle createButton(const char* text, int width, int height);

	/**
	 * Utility function to create an image widget with the given resource.
	 * @param imageResource The image resource handle to be used for the widget.
	 * @param width Widget width.
	 * @param height Widget height.
	 * @return Handle to the new image widget.
	 */
	MAWidgetHandle createImageWidget(
		MAHandle imageResource,
		int width,
		int height);

	/*
	 * Utility function to create a list view.
	 * @param width List View width.
	 * @param height List View height.
	 * @return Handle to the new list view.
	 */
	MAWidgetHandle createListView(int width, int height);

	/**
	 * Utility function to create a vertical empty "spacer".
	 * @param width Spacer width.
	 * @param height Spacer height.
	 * @param backgroundColor
	 * @return Handle to the new spacer.
	 */
	MAWidgetHandle createSpacer(int width, int height,int backgroundColor);

	/*
	 * Sets the font size and the padding size depending on the screen size.
	 */
	void setSizes();

private: // methods
	/**
	 * Create all the widgets in the UI.
	 * The start button, count down label, flickering label,
	 * weapon buttons, rival weapon, spacer, score label,
	 * exit button.
	 */
	MAWidgetHandle createMainLayout();

	/**
	 * Layout the widgets (portrait mode).
	 */
	void setupUI();


private: // members
	/** NativeUI widgets handles. */
	MAWidgetHandle mScreen;

	/** Main screen layout). */
	MAWidgetHandle mMainLayout;

	/**
	 * Label
	 */
	MAWidgetHandle mLabel;
	MAWidgetHandle mButton;

	MAWidgetHandle mPreview;

	MAHandle mExitButton;

	/** Screen size. */
	int mScreenWidth;
	int mScreenHeight;

};

} // namespace Test

#endif /* NATIVESCREEN_H_ */
