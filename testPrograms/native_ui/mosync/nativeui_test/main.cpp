#include <maapi.h>

#include <IX_WIDGET.h>

const char * WIDGET_TYPE_BUTTON = "Button";
const char * WIDGET_TYPE_LABEL = "Label";
const char * WIDGET_TYPE_SCREEN = "Screen";
const char * WIDGET_TYPE_LIST_VIEW = "ListView";
const char * WIDGET_TYPE_LIST_VIEW_CELL = "ListViewCell";
const char * WIDGET_TYPE_LAYOUT_VERTICAL = "VerticalLayout";
const char * WIDGET_TYPE_LAYOUT_HORIZONTAL = "HorizontalLayout";

const char * WIDGET_PROPERTY_WIDTH = "width";
const char * WIDGET_PROPERTY_HEIGHT = "height";

const char * WIDGET_PROPERTY_PADDING_LEFT = "left";
const char * WIDGET_PROPERTY_PADDING_TOP = "top";
const char * WIDGET_PROPERTY_PADDING_RIGHT = "right";
const char * WIDGET_PROPERTY_PADDING_BOTTOM = "bottom";

const char * WIDGET_PROPERTY_HALIGNMENT = "halignment";
const char * WIDGET_PROPERTY_VALIGNMENT = "valignment";
const char * WIDGET_PROPERTY_TEXT = "text";

const char * WIDGET_PROPERTY_BACKGROUND_COLOR = "backgroundColor";
const char * WIDGET_PROPERTY_FONT_COLOR = "fontColor";

const char * WIDGET_HORIZONTAL_LEFT = "left";
const char * WIDGET_HORIZONTAL_CENTER = "center";
const char * WIDGET_HORIZONTAL_RIGHT = "right";

const char * WIDGET_VERTICAL_TOP = "top";
const char * WIDGET_VERTICAL_CENTER = "center";
const char * WIDGET_VERTICAL_BOTTOM = "bottom";

/*
 * main.cpp
 *
 *  Created on: Dec 8, 2010
 *      Author: fmattias
 */
extern "C" int MAMain()
{
	// Create root layout
	int layout = maWidgetCreate( WIDGET_TYPE_LAYOUT_VERTICAL );

	// Create header with some properties.
	int header = maWidgetCreate( WIDGET_TYPE_LABEL );
	maWidgetSetProperty(header, WIDGET_PROPERTY_HEIGHT, "60" );
	maWidgetSetProperty(header, WIDGET_PROPERTY_TEXT, "Header" );
	maWidgetSetProperty(header, WIDGET_PROPERTY_HALIGNMENT, "center" );
	maWidgetSetProperty(header, WIDGET_PROPERTY_VALIGNMENT, "center" );
	maWidgetSetProperty(header, WIDGET_PROPERTY_FONT_COLOR, "ffffff" );
	maWidgetAddChild( layout , header );

	// Create List
	int list = maWidgetCreate( WIDGET_TYPE_LIST_VIEW );
	maWidgetAddChild( layout, list );

	// Add 10 buttons to the list
	for(int i = 0; i < 10; i++)
	{
		int listItem = maWidgetCreate( WIDGET_TYPE_BUTTON );
		maWidgetSetProperty( listItem, WIDGET_PROPERTY_TEXT, "Click me" );
		maWidgetSetProperty( listItem, WIDGET_PROPERTY_HEIGHT, "60" );
		maWidgetSetProperty( listItem, WIDGET_PROPERTY_WIDTH, "100" );
		maWidgetAddChild( list, listItem );
	}

	MAHandle mainScreen = maWidgetCreate(WIDGET_TYPE_SCREEN);

	/*
	int listItem = maWidgetCreate( WIDGET_TYPE_BUTTON );
	maWidgetSetProperty( listItem, WIDGET_PROPERTY_TEXT, "Click me" );
	maWidgetSetProperty( listItem, WIDGET_PROPERTY_HEIGHT, "60" );
	maWidgetSetProperty( listItem, WIDGET_PROPERTY_WIDTH, "100" );
	maWidgetAddChild( mainScreen, listItem );
	*/

	// Add the tree to the root
	maWidgetAddChild( mainScreen, layout );

	maWidgetScreenShow(mainScreen);

	// Wait for close event
	while(1)
	{
		MAEvent event;

		maWait(0);
		maGetEvent( &event );

		if( event.type == EVENT_TYPE_FOCUS_LOST )
		{
			maExit( 0 );
		}
	}
}
