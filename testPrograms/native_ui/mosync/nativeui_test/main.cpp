#include <maapi.h>

#include <mavsprintf.h>

#include <IX_WIDGET.h>

#include "MAHeaders.h"

const char * WIDGET_TYPE_BUTTON = "Button";
const char * WIDGET_TYPE_LABEL = "Label";
const char * WIDGET_TYPE_LIST = "ListView";
const char * WIDGET_TYPE_LIST_ITEM = "ListViewItem";
const char * WIDGET_TYPE_LAYOUT_VERTICAL = "VerticalLayout";
const char * WIDGET_TYPE_LAYOUT_HORIZONTAL = "HorizontalLayout";
const char * WIDGET_TYPE_SCREEN = "Screen";
const char * WIDGET_TYPE_WEB_VIEW = "WebView";
const char * WIDGET_TYPE_TAB_SCREEN = "TabScreen";
const char * WIDGET_TYPE_NAV_SCREEN = "NavScreen";

const char * WIDGET_PROPERTY_WIDTH = "width";
const char * WIDGET_PROPERTY_HEIGHT = "height";

const char * WIDGET_PROPERTY_PADDING_LEFT = "left";
const char * WIDGET_PROPERTY_PADDING_TOP = "top";
const char * WIDGET_PROPERTY_PADDING_RIGHT = "right";
const char * WIDGET_PROPERTY_PADDING_BOTTOM = "bottom";

const char * WIDGET_PROPERTY_HALIGNMENT = "horizontalAlignment";
const char * WIDGET_PROPERTY_VALIGNMENT = "verticalAlignment";
const char * WIDGET_PROPERTY_TEXT = "text";

const char * WIDGET_PROPERTY_BACKGROUND_COLOR = "backgroundColor";
const char * WIDGET_PROPERTY_FONT_COLOR = "fontColor";
const char * WIDGET_PROPERTY_FONT_SIZE = "fontSize";

const char * WIDGET_HORIZONTAL_LEFT = "left";
const char * WIDGET_HORIZONTAL_CENTER = "center";
const char * WIDGET_HORIZONTAL_RIGHT = "right";

const char * WIDGET_VERTICAL_TOP = "top";
const char * WIDGET_VERTICAL_CENTER = "center";
const char * WIDGET_VERTICAL_BOTTOM = "bottom";

int maWidgetSetPropertyInt(MAHandle handle, const char *property, int value)
{
	char buffer[256];
	sprintf( buffer, "%d", value );
	maWidgetSetProperty( handle, property, buffer );
}

void TestApp1() {
	int screenHeight = EXTENT_Y( maGetScrSize( ) );

	int screen = maWidgetCreate( WIDGET_TYPE_SCREEN );

	// Create root layout
	int layout = maWidgetCreate( WIDGET_TYPE_LAYOUT_VERTICAL );
	maWidgetSetPropertyInt( layout, WIDGET_PROPERTY_WIDTH, -1 );
	maWidgetSetPropertyInt( layout, WIDGET_PROPERTY_HEIGHT, -1 );

	// Create header with some properties.
	int header = maWidgetCreate( WIDGET_TYPE_LABEL );
	maWidgetSetPropertyInt(header, WIDGET_PROPERTY_HEIGHT, 60 );
	maWidgetSetPropertyInt(header, WIDGET_PROPERTY_WIDTH, -1 );
	maWidgetSetProperty(header, WIDGET_PROPERTY_TEXT, "Header" );
	maWidgetSetProperty(header, WIDGET_PROPERTY_HALIGNMENT, "center" );
	maWidgetSetProperty(header, WIDGET_PROPERTY_VALIGNMENT, "center" );
	maWidgetSetProperty(header, WIDGET_PROPERTY_FONT_COLOR, "ffffff" );
	maWidgetSetProperty(header, WIDGET_PROPERTY_FONT_SIZE, "20.0" );
	maWidgetAddChild( layout , header );

	// Create List
	int list = maWidgetCreate( WIDGET_TYPE_LIST );
	maWidgetSetPropertyInt( list, WIDGET_PROPERTY_HEIGHT, screenHeight - 60 );
	maWidgetSetPropertyInt( list, WIDGET_PROPERTY_WIDTH, -1 );
	maWidgetAddChild( layout, list );

	// Add 10 buttons to the list
	for(int i = 0; i < 10; i++)
	{
		int listItem = maWidgetCreate( WIDGET_TYPE_LIST_ITEM );
		maWidgetSetProperty( listItem, "text", "Click me" );
		maWidgetSetPropertyInt( listItem, "image", R_ICON );
		maWidgetAddChild( list, listItem );
	}

	// Add the tree to the root
	maWidgetAddChild( screen, layout );
	maWidgetScreenShow( screen );

	int webScreen = maWidgetCreate( WIDGET_TYPE_SCREEN );

	int webView = maWidgetCreate( WIDGET_TYPE_WEB_VIEW );
	maWidgetSetPropertyInt(webView, WIDGET_PROPERTY_WIDTH, -1 );
	maWidgetSetPropertyInt(webView, WIDGET_PROPERTY_HEIGHT, -1 );

	maWidgetAddChild( webScreen, webView );

	// Wait for close event
	while(1)
	{
		MAEvent event;

		maWait( 0 );
		maGetEvent( &event );

		if( event.type == EVENT_TYPE_FOCUS_LOST )
		{
			maExit( 0 );
		}
		else if( event.type == EVENT_TYPE_WIDGET )
		{
			MAWidgetEventData *eventData = (MAWidgetEventData *) event.data;


			char buffer[100];
			sprintf( buffer, "Last clicked %d\n", eventData->widgetHandle );
			maWidgetSetProperty( header, WIDGET_PROPERTY_TEXT, buffer );

			maWidgetSetProperty( webView, "url", "http://www.google.se/" );
			maWidgetScreenShow( webScreen );
		}
	}
}

void TestApp2() {
	int screenHeight = EXTENT_Y( maGetScrSize( ) );

	int listScreen = maWidgetCreate( WIDGET_TYPE_SCREEN );

	// Create root layout
	int layout = maWidgetCreate( WIDGET_TYPE_LAYOUT_VERTICAL );
	maWidgetSetPropertyInt( layout, WIDGET_PROPERTY_WIDTH, -1 );
	maWidgetSetPropertyInt( layout, WIDGET_PROPERTY_HEIGHT, -1 );

	// Create header with some properties.
	int header = maWidgetCreate( WIDGET_TYPE_LABEL );
	maWidgetSetPropertyInt(header, WIDGET_PROPERTY_HEIGHT, 60 );
	maWidgetSetPropertyInt(header, WIDGET_PROPERTY_WIDTH, -1 );
	maWidgetSetProperty(header, WIDGET_PROPERTY_TEXT, "Header" );
	maWidgetSetProperty(header, WIDGET_PROPERTY_HALIGNMENT, "center" );
	maWidgetSetProperty(header, WIDGET_PROPERTY_VALIGNMENT, "center" );
	maWidgetSetProperty(header, WIDGET_PROPERTY_FONT_COLOR, "ffffff" );
	maWidgetSetProperty(header, WIDGET_PROPERTY_FONT_SIZE, "20.0" );
	maWidgetAddChild( layout , header );

	// Create List
	int list = maWidgetCreate( WIDGET_TYPE_LIST );
	maWidgetSetPropertyInt( list, WIDGET_PROPERTY_HEIGHT, screenHeight - 60 );
	maWidgetSetPropertyInt( list, WIDGET_PROPERTY_WIDTH, -1 );
	maWidgetAddChild( layout, list );

	// Add 10 buttons to the list
	for(int i = 0; i < 10; i++)
	{
		int listItem = maWidgetCreate( WIDGET_TYPE_LIST_ITEM );
		maWidgetSetProperty( listItem, "text", "Click me" );
		maWidgetSetPropertyInt( listItem, "image", R_ICON );
		maWidgetAddChild( list, listItem );
	}

	// Add the tree to the root
	maWidgetAddChild( listScreen, layout );

	int webScreen = maWidgetCreate( WIDGET_TYPE_SCREEN );

	int webView = maWidgetCreate( WIDGET_TYPE_WEB_VIEW );
	maWidgetSetPropertyInt(webView, WIDGET_PROPERTY_WIDTH, -1 );
	maWidgetSetPropertyInt(webView, WIDGET_PROPERTY_HEIGHT, -1 );
	maWidgetAddChild( webScreen, webView );

	maWidgetSetProperty(listScreen, "title", "list");
	maWidgetSetProperty(webScreen, "title", "web");
	maWidgetSetPropertyInt( listScreen, "icon", R_STAR_ICON );
	maWidgetSetPropertyInt( webScreen, "icon", R_IPHONE_ICON );

	int tabScreen = maWidgetCreate(WIDGET_TYPE_TAB_SCREEN);
	maWidgetAddChild(tabScreen, listScreen);
	maWidgetAddChild(tabScreen, webScreen);

	maWidgetScreenShow(tabScreen);

	// Wait for close event
	while(1)
	{
		MAEvent event;

		maWait( 0 );
		maGetEvent( &event );

		if( event.type == EVENT_TYPE_FOCUS_LOST )
		{
			maExit( 0 );
		}
		else if( event.type == EVENT_TYPE_WIDGET )
		{
			/*
			MAWidgetEventData *eventData = (MAWidgetEventData *) event.data;
			char buffer[100];
			sprintf( buffer, "Last clicked %d\n", eventData->widgetHandle );
			maWidgetSetProperty( header, WIDGET_PROPERTY_TEXT, buffer );

			maWidgetSetProperty( webView, "url", "http://www.google.se/" );
			maWidgetScreenShow( webScreen );
			*/
		}
	}
}


/*
 * main.cpp
 *
 *  Created on: Dec 8, 2010
 *      Author: fmattias
 */
extern "C" int MAMain()
{
	TestApp2();
	return 0;

}
