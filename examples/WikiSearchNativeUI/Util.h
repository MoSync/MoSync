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
 * @file Util.h
 *
 * This file contains the utility functions for the application.
 *
 * @author Emma Tresanszki
 */

#ifndef UTIL_H_
#define UTIL_H_

// Include MoSync syscalls collection.
#include <IX_WIDGET.h>

// Include library for resource handling.
#include <maapi.h>

// Include utility functions (clamps, wraps, insides, string conversions and breaks).
#include <MAUtil/util.h>

// Include library for string handling.
#include <mastring.h>

namespace WikiNativeUI
{

using namespace MAUtil;

/*
 * This header contains utility functions for creating native UI widgets.
 * Also, performs operations on strings, to convert the html content into unicode.
 *
 */

/** Colors used for UI widgets. */
enum Colors{
	/** The background color for the top layout. */
	QUARTZ     = 0xD9D9F3 ,
	/** The background color for the main layout. */
	SLATE_GRAY = 0x9FB6CD ,
	/** The color used for the labels. */
	BLUE       = 0x104E8B ,
	/** The color used for buttons and spacers. */
	DARK_GREY  = 0x333333 ,
	/** The background color used for other layouts. */
	DARK_WHITE = 0xF7F7F7
};

/** Utility sizes. */
enum WidgetSize{
	TOP_LAYOUT_HEIGHT = 80 ,
	BREAKLINE_HEIGHT  = 2 ,
	PADDING_SMALL     = 5 ,
	PADDING           = 10 ,
	FONT_SMALL        = 12 ,
	FONT_LARGE        = 15 ,
	/* The maximum value for the slider.It shouldn't be bigger, because the parsing would take a very long time. */
	SLIDER_MAX_VALUE  = 50 ,
	/** The maximum progress that the progress bar can reach. */
    PROGRESS_BAR_MAX_VALUE = 100
};

/** Categories names used in home screen. */
const MAUtil::String CATEGORY_ALL     = " All ";
const MAUtil::String CATEGORY_SPORTS  = " Sports ";
const MAUtil::String CATEGORY_NATURE  = " Nature ";
const MAUtil::String CATEGORY_NEWS    = " News ";
const MAUtil::String CATEGORY_MOVIES  = " Movies ";
const MAUtil::String CATEGORY_SCIENCE = " Science ";

/** Messages for UI screens. */
const MAUtil::String MESSAGE_WAITNOTE        	  = " Please wait....";
const MAUtil::String WARNING_NOTHING_SELECTED	  = " There is nothing selected.";
const MAUtil::String MESSAGE_PAGE_LOADING    	  = " Page is loading...";
const MAUtil::String MESSAGE_LOADING_STOPPED = " Page loading was stopped";
const MAUtil::String MESSAGE_LOADING_ERROR   = " Sorry, this page cannot be loaded.";
const MAUtil::String MESSAGE_LOADING_DONE    = " Page loaded";
const MAUtil::String MESSAGE_EDITBOX_HINT    = " Enter tags separated by comma ";
const MAUtil::String MESSAGE_SEARCH_HINT     = " Press SEARCH when ready ";
const MAUtil::String MESSAGE_NEXT_HINT       = " Press NEXT when ready ";
const MAUtil::String MESSAGE_BACK_HINT       = " Press BACK for other results ";
const MAUtil::String MESSAGE_HINT_STARTOVER  = " Press button to start over ";
const MAUtil::String MESSAGE_SELECT_ALL      = " Select All ";

/** In case of error, the engine notifies the UI with the following error messages: **/
/** This message is used in case an error occurs in the engine, at connection handling. **/
const MAUtil::String ERROR_ANY = "An error occurred.";
/** In case the http connection cannot be initialized. **/
const MAUtil::String ERROR_NO_CONNECTION = "No Internet connection!";
/** This message is used in case the results received from server are empty. **/
const MAUtil::String ERROR_NO_RESULTS = "There are no results.";
/** The results cannot be parsed. **/
const MAUtil::String ERROR_INVALID_DATA = "Invalid results.Error:";

/*
 * Utility functions for creating NativeUI widgets.
 */

/**
 * Utility function to create a label.
 * The height of the label is computed automatically.
 * @param width Label width.
 * @param text Label text.
 * @param fontColor Text color.
 * @param fontSize Text size.
 *
 * @return Handle to the new label.
 */
MAWidgetHandle createLabel(int width, const char* text,int fontColor=BLUE, int fontSize=15);

/**
 * Utility function to create a vertical empty "spacer".
 * @param width Spacer width.
 * @param height Spacer height.
 * @return Handle to the new spacer.
 */
MAWidgetHandle createSpacer(int width, int height);

/*
 * Utility function to create a list view.
 * @param width List View width.
 * @param height List View height.
 * @return Handle to the new list view.
 */
MAWidgetHandle createListView(int width, int height);

/*
 * Utility function to create a web view.
 * @param width Web view width.
 * @param height Web view height.
 * @return Handle to the new web view.
 */
MAWidgetHandle createWebView(int width, int height);

/*
 * Utility function to create an edit box.
 * @param width Edit box width.
 * @param height Edit box height.
 * @return Handle to the new edit box.
 */
MAWidgetHandle createEditBox(int width, int height);

/*
 * Utility function to create a check box.
 * @return Handle to the new check box.
 */
MAWidgetHandle createCheckBox();

/*
 * Utility function to create a button.
 * Create a button with centered text.
 * By default, font size is 15, and color DARK GREY.
 * @param text The text for the button.
 * @param fontColor The font color.
 * @param fontSize The font size.
 * @param width The widget width.
 * @param height The widget height.
 */
MAWidgetHandle createButton(const char* text, int fontColor=DARK_GREY, int fontSize=FONT_LARGE,
		                    int width = MAW_CONSTANT_WRAP_CONTENT, int height=MAW_CONSTANT_WRAP_CONTENT);


/*
 * Utility function to create a progress bar.
 * @return Handle to the new progress bar.
 */
MAWidgetHandle createProgressBar();

/*
 * Utility function to create a slider control.
 * @return Handle to the new slider.
 */
MAWidgetHandle createSlider();

/*
 * Set a text for a label.
 * @param aLabel Handle to the label.
 * @param aText Label text.
 */
void setLabelText(MAWidgetHandle aLabel, MAUtil::String aText);

/*
 * Set a text for a button.
 * @param aButton Handle to the button.
 * @param aText Button text.
 */
void setButtonText(MAWidgetHandle aButton, MAUtil::String aText);

/*
 * Utility function to set sizes of a widget.
 * @param widget The widget handle.
 * @param width The widget width.
 * @param height The widget height.
 */
void setWidgetSize(MAWidgetHandle widget, int width, int height);

/*
 * Apply an int property for a widget.
 * @param aWidget The handle for the widget.
 * @param aProperty A string representing which property to set.
 * @param aValue The value which will be assigned to the property.
 * @param base  The output radix. Default is 10, 16 is to be used for color properties.
 *
 * @return Any of the following result codes:
 * - #MAW_RES_OK if the property could be set.
 * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
 * - #MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
 * - #MAW_RES_ERROR otherwise.
 */
int setWidgetProperty(MAWidgetHandle aWidget, const char* aProperty, int aValue, int base=10);

/*
 * Utility functions for string handling
 * Converts the strings from xml into more human-readable strings.
 */

/*
 * Replaces special characters.
 * @param The buffer that needs to be parsed.
 */
void formatToUnicode(String& input);

/*
 * Remove the span tags, keep only the search string.
 * @param The buffer that needs to be parsed.
 */
void trimSpanTags(String& input);

/*
 * Trim bold tags.
 * The b tags are encountered at the end, and contain dots ...
 * ( this is because the input is just a snippet, not the whole article)
 * @param The buffer that needs to be trimmed.
 */
void trimBoldTags(String& input);

/*
 * Replace substrings from a string.
 *
 */
char* strReplace(char* original, const char* remove, const char* replacement);

/*
 * strreaplceFirst
 */
char* strReplaceFirst(char* original, const char* remove, const char* replacement);

/*
*  Replaces the "remove" string with the "replacement" one in the "original" string.
*      insert and remove operations require more time than the + operator
 *     replace for std::string was not ported for mosync string
*  @param The original string.
*  @param The string to be removed from the original one.
*  @param The replacement.
*/
void replaceString(String& original, const String& toRemove, const String& replacement);

} // namespace WikiNativeUI

#endif /* UTIL_H_ */
