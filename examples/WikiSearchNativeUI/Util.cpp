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
 * @file Util.cpp
 * @author Emma Tresanszki
 *
 * This file contains the utility functions for the application.
 */

// Include util class for integer to string conversions.
#include <MAUtil/util.h>

// Include library for string conversions.
#include <mastdlib.h>
#include <mactype.h>
#include <maxtoa.h>

// Include the resources for images.
//#include "MAHeaders.h"

// Include header file for this class.
#include "Util.h"

namespace WikiNativeUI
{

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
MAWidgetHandle createLabel(int width, const char* text,
		int fontColor, int fontSize)
{
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

	maWidgetSetProperty(
		label, MAW_LABEL_TEXT_VERTICAL_ALIGNMENT, MAW_ALIGNMENT_CENTER);
	maWidgetSetProperty(
		label, MAW_LABEL_TEXT_HORIZONTAL_ALIGNMENT, MAW_ALIGNMENT_CENTER);

	return label;
}

/**
 * Utility function to create a button.
 * Create a button with centered text.
 * By default, font size is 15, and color DARK GREY.
 * @param text The text for the button.
 * @param fontColor The font color.
 * @param fontSize The font size.
 * @param width The widget width.
 * @param height The widget height.
 */
MAWidgetHandle createButton(const char* text, int fontColor, int fontSize,
	int width, int height)
{
	MAWidgetHandle button = maWidgetCreate(MAW_BUTTON);
	maWidgetSetProperty(button, MAW_BUTTON_TEXT, text);

	// Set the widget size.
	setWidgetProperty(button, MAW_WIDGET_WIDTH, width);
	setWidgetProperty(button, MAW_WIDGET_HEIGHT, height);

	maWidgetSetProperty(
		button, MAW_BUTTON_TEXT_VERTICAL_ALIGNMENT, MAW_ALIGNMENT_CENTER);
	maWidgetSetProperty(
		button, MAW_BUTTON_TEXT_HORIZONTAL_ALIGNMENT, MAW_ALIGNMENT_CENTER);

	setWidgetProperty(button,MAW_BUTTON_FONT_SIZE, fontSize);
	setWidgetProperty(button,MAW_BUTTON_FONT_COLOR, fontColor, 16);

	return button;
}

/**
 * Utility function to create a progress bar.
 * @return Handle to the new progress bar.
 */
MAWidgetHandle createProgressBar()
{
	MAWidgetHandle progressBar = maWidgetCreate(MAW_PROGRESS_BAR);
	setWidgetProperty(
		progressBar, MAW_WIDGET_WIDTH, MAW_CONSTANT_FILL_AVAILABLE_SPACE);
	setWidgetProperty(
		progressBar, MAW_WIDGET_HEIGHT, MAW_CONSTANT_WRAP_CONTENT);

	return progressBar;
}

/**
 * Utility function to create a slider control.
 * @return Handle to the new slider.
 */
MAWidgetHandle createSlider()
{
	MAWidgetHandle slider = maWidgetCreate(MAW_SLIDER);
	setWidgetProperty(
		slider, MAW_WIDGET_WIDTH, MAW_CONSTANT_FILL_AVAILABLE_SPACE);
	setWidgetProperty(
		slider, MAW_WIDGET_HEIGHT, MAW_CONSTANT_WRAP_CONTENT);

	return slider;
}

/**
 * Utility function to create a vertical empty "spacer".
 * @param width Spacer width.
 * @param height Spacer height.
 * @return Handle to the new spacer.
 */
MAWidgetHandle createSpacer(int width, int height)
{
	MAWidgetHandle spacer = maWidgetCreate(MAW_VERTICAL_LAYOUT);

	// Set the size for this layout.
	setWidgetProperty(spacer, MAW_WIDGET_WIDTH , width);
	setWidgetProperty(spacer, MAW_WIDGET_HEIGHT, height);

	return spacer;
}

/**
 * Utility function to create a list view.
 * @param width List View width.
 * @param height List View height.
 * @return Handle to the new list view.
 */
MAWidgetHandle createListView(int width, int height)
{
	MAWidgetHandle listView = maWidgetCreate(MAW_LIST_VIEW);

	// Set the size for this list view.
	setWidgetProperty(listView, MAW_WIDGET_WIDTH, width);
	setWidgetProperty(listView, MAW_WIDGET_HEIGHT, height);

	return listView;
}

/**
 * Utility function to create an edit box.
 * @param width Edit box width.
 * @param height Edit box height.
 * @return Handle to the new edit box.
 */
MAWidgetHandle createEditBox(int width, int height)
{
	MAWidgetHandle editBox = maWidgetCreate(MAW_EDIT_BOX);

	// Set the size for this edit box.
	setWidgetProperty(editBox, MAW_WIDGET_WIDTH, width);
	setWidgetProperty(editBox, MAW_WIDGET_HEIGHT, height);

	return editBox;
}

/**
 * Utility function to create a check box.
 * @return Handle to the new check box.
 */
MAWidgetHandle createCheckBox()
{
	MAWidgetHandle checkBox = maWidgetCreate(MAW_CHECK_BOX);

	return checkBox;
}

/**
 * Utility function to create a web view.
 * @param width Web view width.
 * @param height Web view height.
 * @return Handle to the new web view.
 */
MAWidgetHandle createWebView(int width, int height)
{
	MAWidgetHandle webView = maWidgetCreate(MAW_WEB_VIEW);

	// Set the size for this widget.
	setWidgetProperty(webView, MAW_WIDGET_WIDTH, width);
	setWidgetProperty(webView, MAW_WIDGET_HEIGHT, height);

	return webView;
}

/**
 * Utility function to set sizes of a widget.
 * @param widget The widget handle.
 * @param width The widget width.
 * @param height The widget height.
 */
void setWidgetSize(MAWidgetHandle widget, int width, int height)
{
	char buf[10];

	itoa(width, buf, 10);
	maWidgetSetProperty(widget, MAW_WIDGET_WIDTH, buf);
	itoa(height, buf, 10);
	maWidgetSetProperty(widget, MAW_WIDGET_HEIGHT, buf);
}

/**
 * Set a text for a label.
 * @param aLabel Handle to the label.
 * @param aText Label text.
 */
void setLabelText(MAWidgetHandle aLabel, MAUtil::String aText)
{
	maWidgetSetProperty(aLabel,MAW_LABEL_TEXT, aText.c_str());
}

/**
 * Set a text for a button.
 * @param aButton Handle to the button.
 * @param aText Button text.
 */
void setButtonText(MAWidgetHandle aButton, MAUtil::String aText)
{
	maWidgetSetProperty(aButton, MAW_BUTTON_TEXT, aText.c_str());
}

/**
 * Apply an int property for a widget.
 * @param aWidget The handle for the widget.
 * @param aProperty A string representing which property to set.
 * @param aValue The value which will be assigned to the property.
 * @param base  The output radix.
 *
 * @return Any of the following result codes:
 * - #MAW_RES_OK if the property could be set.
 * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
 * - #MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
 * - #MAW_RES_ERROR otherwise.
 */
int setWidgetProperty(MAWidgetHandle aWidget, const char* aProperty,
	int aValue, int base)
{
	// Buffer for property values.
	char buf[10];

	itoa( aValue, buf, base);
	// Apply the property to the widget.
	return maWidgetSetProperty(aWidget, aProperty, buf);
}

/**
 * Utility functions for string handling
 * Converts the strings from xml into more human-readable strings
 */

/**
 * Replaces special characters.
 * @param The buffer that needs to be parsed.
 */
void formatToUnicode(String& input)
{
  /* If the string is less than 4 characters, there is nothing to be replaced.
   */
  if (input.length() <= 4) {
	  return;
  }

  // Just in case there is nothing to replace.
  if (input.findFirstOf('&') == input.npos) {
	  return;
  }

  // Replace the punctuation characters.
  replaceString(input, "&euro;", "€");
  replaceString(input, "&pound;", "£");
  replaceString(input, "&laquo;", "«");
  replaceString(input, "&raquo;", "»");
  replaceString(input, "&bull;", "•");
  replaceString(input, "&dagger;", "†");
  replaceString(input, "&copy;", "©");
  replaceString(input, "&reg;", "®");
  replaceString(input, "&deg;", "°");
  replaceString(input, "&micro;", "µ");
  replaceString(input, "&middot;", "·");
  replaceString(input, "&ndash;", "–");
  replaceString(input, "&mdash;", "—");
  replaceString(input, "&iexcl;", "¡");
  replaceString(input, "&iquest;", "¿");
  replaceString(input, "&nbsp;", " ");

  replaceString(input, "&amp;", "&");
  replaceString(input, "&quot;", "\"");
  replaceString(input, "&lt;", "<");
  replaceString(input, "&gt;", ">");
  replaceString(input, "&rdquo;", "”");

  char* p = (char*)input.c_str();

  int ccstart = input.find("&#");
  int ccend;
  int ccode;
  char com[2];
  com[1] = 0;
  char* compointer = com;
  while(ccstart != input.npos)
  {
    ccend = ccstart + 2;
    while(isdigit(input.c_str()[ccend]))ccend++;

    ccode = stringToInteger(input.substr(ccstart + 2, ccend - ccstart -2));
    if(ccode < 255)
      com[0] = (char)ccode;
    else
      com[0] = 0;

    // Remove semicolons.
    if(input.c_str()[ccend] == ';')
      {
        ccend++;
      }

    /*
	 * replaceString(input, input.substr(ccstart, ccend - ccstart), String(com));
	 * the above might be faster than the bellow one
	 */
    strReplace(p, input.substr(ccstart, ccend - ccstart).c_str(), compointer);

    input = p;
    ccstart = input.find("&#");
  }
}

/**
 * Remove the span tags, keep only the search string.
 * @param The buffer that needs to be parsed.
 */
void trimSpanTags(String& input)
{
	int openTag = input.find("<span class='searchmatch'>", 0);

	while(openTag != input.npos)
	{
		int closeTag = input.find("</span>", openTag + 1);
	    // get the content of span
	    String match = input.substr(openTag+26, closeTag - openTag - 26);

	    if(closeTag != input.npos && closeTag > openTag)
		{
			replaceString(
				input, input.substr(openTag, closeTag - openTag +7 ), match);
		}

	    openTag = input.find("<span class='searchmatch'>", openTag);
	}
}

/**
 * Trim bold tags.
 * The b tags are encountered at the end, and contain dots ...
 * ( this is because the input is just a snippet, not the whole article)
 */
void trimBoldTags(String& input)
{
	int openTag = input.find("<b>", 0);

	while(openTag != input.npos)
	{
	    int closeTag = input.find("</b>", openTag + 1);
	    // get the content of bold

	    if(closeTag != input.npos && closeTag > openTag)
		{
			replaceString(
				input,
				input.substr(openTag, closeTag - openTag +4 ),
				String("..."));
	    }

	    openTag = input.find("<b>", openTag);
	}
}

/**
 * Replaces the "remove" string with the "replacement" one in the "original" string.
 * Insert and remove operations require more time than the + operator.
 * Replace for std::string was not ported for mosync string.
 * @param The original string.
 * @param The string to be removed from the original one.
 * @param The replacement.
 */
void replaceString(String& original, const String& remove,
	const String& replacement)
{
	int lenght = remove.length();
	int iBegin = original.find(remove, 0);

	while (iBegin != original.npos)
	{
		original =
			original.substr(0, iBegin) +
			replacement +
			original.substr(iBegin + lenght);
		iBegin = original.find(remove, iBegin + 1);
	}
}

/*
 * st*rReplaceFirst
 */
char* strReplaceFirst(char *str, const char* what, const char* to)
{
    int i, j, k, m, n, delta;
    int n1, n2, n3;
    char* p, *q;
    if (!str || !what)
        return 0;
    if (!to)
        to = "";
    n1 = strlen(str);
    n2 = strlen(what);
    n = n1 - n2 + 1;
    for (i = 0; i < n; ++i)
    {
        for (j = 0; j < n2 && what[j] == str[i+j]; ++j)
            ;
        if (j == n2) // found
        {
            n3 = strlen(to);
            delta = n3 - n2;
            m = n1 - i - n2;
            if (delta < 0) /* move left */
            {
                p = str + (i + n2 + delta);
                q = p - delta;
                for (k = 0; k <= m; ++k)
                    p[k] = q[k];
            }
            else if (delta > 0) /* move right */
            {
                q = str + n1 - m;
                p = q + delta;
                for (k = m; k >= 0; --k)
                    p[k] = q[k];
            }
            for (k = 0; k < n3; ++k)
                str[i+k] = to[k];
            return str + i + n3;
        }
    }
    return 0;
}

/**
 * strReplace
 */
char* strReplace(char* str, const char* what, const char* to)
{
    char* p = str;
    while ((p=strReplaceFirst(p,what,to)) != 0)
        ;
    return str;
}

} // namespace WikiNativeUI
