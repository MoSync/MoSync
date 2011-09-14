/*
 * NativeScreen.cpp
 *
 *  Created on: May 11, 2011
 *      Author: emma tresanszki
 */


// Include library for string conversions.
#include <mastdlib.h>
#include <maapi.h>
#include <mavsprintf.h>

// Include util class for integer to string conversions.
#include <MAUtil/util.h>

// Include application's Moblet class.
#include "Moblet.h"

// Include the main screen.
#include "NativeScreen.h"

namespace Test
{

/**
 * Constructor for the main screen class.
 * Here instance variables are initialized.
 */
NativeScreen::NativeScreen() :
	mScreen(-1),
	mMainLayout(-1),
	mLabel(-1),
	mScreenWidth(-1),
	mScreenHeight(-1)
{
	// Get the screen size.
	MAExtent screenSize = maGetScrSize();
	mScreenWidth = EXTENT_X(screenSize);
	mScreenHeight = EXTENT_Y(screenSize);

	// Initialize the native UI widgets.
	// NOTE: There is no scaling required, because that is done
	// automatically by the platform at widget creation.
	setupUI();

	// Make the play screen listen for widget events.
	MAUtil::Environment::getEnvironment().addCustomEventListener(this);
}

/**
 * The destructor deletes the main widget
 * and all of its children.
 */
NativeScreen::~NativeScreen()
{
	// Delete the main widget, also deletes child widgets.
	maWidgetDestroy(mMainLayout);
}

/**
 * Show the screen with the game user interface.
 * NOTE: If we had more screens, it could have been necessary
 * to do some UI initialization here, before the screen is displayed.
 */
void NativeScreen::showScreen()
{
	maWidgetScreenShow(mScreen);
}

/**
 * Handle widget events.
 * @param event A MoSync event data structure.
 */
void NativeScreen::customEvent(const MAEvent& event)
{
	// Get the information sent by the widget.
	MAWidgetEventData* widgetEventData = (MAWidgetEventData*) event.data;

	if ( event.type == EVENT_TYPE_IMAGE_PICKER)
	{
		if ( event.imagePickerState == 1 )
		{
			// ready, get handle
			MAHandle myImage = event.imagePickerItem;

	        char buffer[256];
	        sprintf(buffer, "%d", myImage);
			int resCode = maWidgetSetProperty(mPreview, MAW_IMAGE_IMAGE, buffer);

			setLabelText(mLabel, "Preview is available");
		}
	}

	// Check that the event was a click (touch) event.
	if (widgetEventData->eventType == MAW_EVENT_CLICKED || widgetEventData->eventType == MAW_EVENT_POINTER_PRESSED)
	{
		// Handle the event emitted by the widget
		widgetClicked(widgetEventData->widgetHandle);
	}
}

/**
 * Handle widget click events.
 * @param widgetHandle The handle (reference) to the widget
 * that was clicked.
 */
void NativeScreen::widgetClicked(MAHandle widgetHandle)
{
	// Exit the application when the exit button is clicked.
	if (widgetHandle == mExitButton)
	{
		TestMoblet::getInstance()->closeEvent();
	}
	else if ( mButton == widgetHandle)
	{
		maImagePickerOpen();
	}

}

/**
 * Utility function to create a label.
 * The height of the label is computed automatically.
 * @param width Label width.
 * @param fontColor Text color.
 * @param fontSize Text size.
 * @param text Label text.
 * @return Handle to the new label.
 */
MAWidgetHandle NativeScreen::createLabel(
	int width,
	int fontColor,
	int fontSize,
	const char* text)
{
	// Create the label.
	MAWidgetHandle label = maWidgetCreate(MAW_LABEL);

	// Set the label font color.
	setWidgetProperty(label, MAW_LABEL_FONT_COLOR, fontColor, 16);

	// Set the label font size.
	setWidgetProperty(label, MAW_LABEL_FONT_SIZE, fontSize);

	// Set the widget size.
	setWidgetProperty(label, MAW_WIDGET_WIDTH, width);
	setWidgetProperty(label, MAW_WIDGET_HEIGHT, MAW_CONSTANT_WRAP_CONTENT);

	// Set the label text.
	maWidgetSetProperty(label, MAW_LABEL_TEXT, text);

	// Set text alignment.
	maWidgetSetProperty(label, MAW_LABEL_TEXT_VERTICAL_ALIGNMENT, "center");
	maWidgetSetProperty(label, MAW_LABEL_TEXT_HORIZONTAL_ALIGNMENT, "center");

	return label;
}

/*
 * Set a text for a label.
 * @param aLabel Handle to the label.
 * @param aText Label text.
 */
void NativeScreen::setLabelText(MAWidgetHandle aLabel, MAUtil::String aText)
{
	maWidgetSetProperty(aLabel,MAW_LABEL_TEXT, aText.c_str());
}

/*
 * Apply an int property for a widget.
 * @param aWidget The handle for the widget.
 * @param aProperty A string representing which property to set.
 * @param aValue The value which will be assigned to the property.
 * @param base  The output radix.
 *
 * * \returns Any of the following result codes:
 * - #MAW_RES_OK if the property could be set.
 * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
 * - #MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
 * - #MAW_RES_ERROR otherwise.
 */
int NativeScreen::setWidgetProperty(MAWidgetHandle aWidget, const char* aProperty, int aValue, int base)
{
	// Buffer for property values.
	char buf[10];

	itoa( aValue, buf, base);
	// Apply the property to the widget.
	return maWidgetSetProperty(aWidget, aProperty, buf);
}

/**
 * Creates a button with centered text by default,
 * font size is 20, and color is DARK GREY.
 * @param text The button text.
 * @return Handle to the new button.
 */
MAWidgetHandle NativeScreen::createButton(const char* text, int width, int height)
{
	// Create button.
	MAWidgetHandle button = maWidgetCreate(MAW_BUTTON);

	// Set  text and text alignment.
	maWidgetSetProperty(button, MAW_BUTTON_TEXT, text);
	maWidgetSetProperty(button, MAW_BUTTON_TEXT_VERTICAL_ALIGNMENT, "center");
	maWidgetSetProperty(button, MAW_BUTTON_TEXT_HORIZONTAL_ALIGNMENT, "center");

	setWidgetProperty(button, MAW_WIDGET_WIDTH, width);
	setWidgetProperty(button, MAW_WIDGET_HEIGHT, height);

	// Set font color.
	setWidgetProperty(button, MAW_BUTTON_FONT_COLOR, BLUE, 16);

//	setWidgetProperty(button, MAW_WIDGET_BACKGROUND_COLOR, DARK_SEA_GREEN, 16);

//	maWidgetSetProperty(button,"Gradient", " ");

	// Set font size to small.
//	setWidgetProperty(button, MAW_BUTTON_FONT_SIZE, mFontRegularSize);

	return button;
}

/**
 * Utility function to create an image widget with the given resource.
 * @param imageResource The image resource handle to be used for the widget.
 * @param width Widget width.
 * @param height Widget height.
 * @return Handle to the new image widget.
 */
MAWidgetHandle NativeScreen::createImageWidget(
	MAHandle imageResource,
	int width,
	int height)
{
	MAWidgetHandle image = maWidgetCreate(MAW_IMAGE);

	// Set the image.
	setWidgetProperty(image,MAW_IMAGE_IMAGE, imageResource);

	// Set the widget sizes.
	setWidgetProperty(image, MAW_WIDGET_WIDTH, width);
	setWidgetProperty(image, MAW_WIDGET_HEIGHT, height);

	return image;
}

/**
 * Utility function to create a vertical empty "spacer".
 * @param width Spacer width.
 * @param height Spacer height.
 * @param backgroundColor
 * @return Handle to the new spacer.
 */
MAWidgetHandle NativeScreen::createSpacer(int width, int height, int backgroundColor)
{
	MAWidgetHandle spacer = maWidgetCreate(MAW_VERTICAL_LAYOUT);

	// Set the widget size.
	setWidgetProperty(spacer, MAW_WIDGET_WIDTH, width);
	setWidgetProperty(spacer, MAW_WIDGET_HEIGHT, height);

	setWidgetProperty(spacer, MAW_WIDGET_BACKGROUND_COLOR, backgroundColor, 16);

	return spacer;
}

/**
 * Layout the widgets (portrait mode).
 */
void NativeScreen::setupUI()
{
	// Create the main screen
	mScreen = maWidgetCreate(MAW_SCREEN);

	// Create the main layout.
	mMainLayout = createMainLayout();

	// Add the main layout to the screen
	maWidgetAddChild(mScreen, mMainLayout);
}

/**
 * Create all the widgets in the UI.
 * The start button, count down label, flickering label,
 * weapon buttons, rival weapon, spacer, score label,
 * exit button.
 */
MAWidgetHandle NativeScreen::createMainLayout()
{
	// Create the main layout (the main widget that holds the other widgets).
	MAWidgetHandle mainLayout = maWidgetCreate(MAW_VERTICAL_LAYOUT);

	// Set the size for this widget.
	setWidgetProperty(mainLayout, MAW_WIDGET_WIDTH, MAW_CONSTANT_FILL_AVAILABLE_SPACE);
	setWidgetProperty(mainLayout, MAW_WIDGET_HEIGHT, MAW_CONSTANT_FILL_AVAILABLE_SPACE);

	// All the child widgets of the mainLayout will be centered.
	maWidgetSetProperty(
		mainLayout,
		MAW_VERTICAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT,
		MAW_ALIGNMENT_CENTER);
	maWidgetSetProperty(
		mainLayout,
		MAW_VERTICAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT,
		MAW_ALIGNMENT_CENTER);

	// Set a background color for the main layout.
	setWidgetProperty(mainLayout, MAW_WIDGET_BACKGROUND_COLOR, SEA_GREEN, 16);

	// Add some space before the list view.
	maWidgetAddChild(
		mainLayout,
		createSpacer(mScreenWidth, 10, BLUE));

	mLabel = maWidgetCreate(MAW_LABEL);
	setLabelText(mLabel, "No image selected");
	maWidgetAddChild(mainLayout, mLabel);

	mButton = createButton("Select image", MAW_CONSTANT_WRAP_CONTENT, MAW_CONSTANT_WRAP_CONTENT);
	maWidgetAddChild(mainLayout, mButton);

	mPreview = maWidgetCreate(MAW_IMAGE);
	maWidgetSetProperty(mPreview, MAW_WIDGET_WIDTH, "-1");
	maWidgetSetProperty(mPreview, MAW_WIDGET_HEIGHT, "-1");
//    char buffer[256];
//    sprintf(buffer, "%d", RES_IMAGE_START);
//	maWidgetSetProperty(mPreview, MAW_IMAGE_IMAGE, buffer);
	maWidgetSetProperty(mPreview, MAW_IMAGE_SCALE_MODE, "scalePreserveAspect");
	maWidgetAddChild(mainLayout, mPreview);

	maWidgetAddChild(
		mainLayout,
		createSpacer(mScreenWidth, mScreenHeight/50, BLUE));

	// Exit button in a horizontal layout.
	MAWidgetHandle bottomMenu = maWidgetCreate(MAW_HORIZONTAL_LAYOUT);
	setWidgetProperty(bottomMenu, MAW_WIDGET_WIDTH, mScreenWidth);
	setWidgetProperty(bottomMenu, MAW_WIDGET_HEIGHT, MAW_CONSTANT_FILL_AVAILABLE_SPACE);

	// Set the bottom button to half the available width.
	mExitButton = createButton("  Exit  ", mScreenWidth/2, MAW_CONSTANT_WRAP_CONTENT );
	maWidgetSetProperty(mExitButton, MAW_WIDGET_BACKGROUND_GRADIENT, "0xC1FFC1,0x9BCD9B");
	maWidgetAddChild(bottomMenu,mExitButton);
	maWidgetAddChild(mainLayout,bottomMenu);

	return mainLayout;
}

} // namespace Test
