#include <maapi.h>

#include <mavsprintf.h>

#include <IX_WIDGET.h>

#include "MAHeaders.h"

const char * WIDGET_TYPE_BUTTON = "Button";
const char * WIDGET_TYPE_LABEL = "Label";
const char * WIDGET_TYPE_GL_VIEW = "GLView";

const char * WIDGET_TYPE_LIST = "ListView";
const char * WIDGET_TYPE_LIST_ITEM = "ListViewItem";
const char * WIDGET_TYPE_LAYOUT_VERTICAL = "VerticalLayout";
const char * WIDGET_TYPE_LAYOUT_HORIZONTAL = "HorizontalLayout";
const char * WIDGET_TYPE_SCREEN = "Screen";
const char * WIDGET_TYPE_TAB_SCREEN = "TabScreen";
const char * WIDGET_TYPE_WEB_VIEW = "WebView";
const char * WIDGET_TYPE_NAV_SCREEN = "NavScreen";

const char * WIDGET_PROPERTY_WIDTH = "width";
const char * WIDGET_PROPERTY_HEIGHT = "height";

const char * WIDGET_PROPERTY_PADDING_LEFT = "left";
const char * WIDGET_PROPERTY_PADDING_TOP = "top";
const char * WIDGET_PROPERTY_PADDING_RIGHT = "right";
const char * WIDGET_PROPERTY_PADDING_BOTTOM = "bottom";

const char * WIDGET_PROPERTY_HALIGNMENT = "horizontalAlignment";
const char * WIDGET_PROPERTY_VALIGNMENT = "verticalAlignment";

const char * WIDGET_PROPERTY_TEXT_HORIZONTAL_ALIGNMENT = "textHorizontalAlignment";
const char * WIDGET_PROPERTY_TEXT_VERTICAL_ALIGNMENT = "textVerticalAlignment";
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

int createListScreen();
int createWebScreen();
int createScreen(const char *title, int icon, const char *text);
int createOpenGLScreen();

/*
 * main.cpp
 *
 *  Created on: Dec 8, 2010
 *      Author: fmattias
 */
extern "C" int MAMain()
{
	int screenHeight = EXTENT_Y( maGetScrSize( ) );

	int tabScreen = maWidgetCreate( WIDGET_TYPE_TAB_SCREEN );

	int listScreen = createListScreen( );
	maWidgetAddChild( tabScreen, listScreen );

	int webScreen = createWebScreen( );
	maWidgetAddChild( tabScreen, webScreen );

	int startScreen = createScreen( "Start", R_START, "Start..." );
	maWidgetAddChild( tabScreen, startScreen );

	int resultsScreen = createScreen( "Results", R_RESULTS, "Results..." );
	maWidgetAddChild( tabScreen, resultsScreen );

	int openglScreen = createOpenGLScreen();
	maWidgetAddChild(tabScreen, openglScreen);

	maWidgetScreenShow( tabScreen );

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
			maWidgetSetPropertyInt( tabScreen, "currentTab", 1 );
		}
		else if(event.type == EVENT_TYPE_CLOSE) {
			maExit( 0 );
		}
	}
}

int createListScreen()
{
	int listScreen = maWidgetCreate( WIDGET_TYPE_SCREEN );
	maWidgetSetProperty( listScreen, "title", "RSS" );
	maWidgetSetPropertyInt( listScreen, "icon", R_NEWS );

	// Create List
	int list = maWidgetCreate( WIDGET_TYPE_LIST );
	maWidgetSetPropertyInt( list, WIDGET_PROPERTY_HEIGHT, -1 );
	maWidgetSetPropertyInt( list, WIDGET_PROPERTY_WIDTH, -1 );

	// Add 10 buttons to the list
	for(int i = 0; i < 10; i++)
	{
		int listItem = maWidgetCreate( WIDGET_TYPE_LIST_ITEM );
		maWidgetSetProperty( listItem, "text", "Click me" );
		maWidgetSetPropertyInt( listItem, "icon", R_ICON );
		maWidgetAddChild( list, listItem );
	}

	// Add the tree to the root
	maWidgetAddChild( listScreen, list );

	return listScreen;
}

int createWebScreen()
{
	int webScreen = maWidgetCreate( WIDGET_TYPE_SCREEN );
	maWidgetSetProperty( webScreen, "title", "Web" );
	maWidgetSetPropertyInt( webScreen, "icon", R_TV );

	int webView = maWidgetCreate( WIDGET_TYPE_WEB_VIEW );
	maWidgetSetPropertyInt( webView, WIDGET_PROPERTY_WIDTH, -1 );
	maWidgetSetPropertyInt( webView, WIDGET_PROPERTY_HEIGHT, -1 );
	maWidgetSetProperty( webView, "url", "http://www.google.se/" );

	maWidgetAddChild( webScreen, webView );

	return webScreen;
}

int createOpenGLScreen()
{
	int screen = maWidgetCreate( WIDGET_TYPE_SCREEN );
	maWidgetSetProperty( screen, "title", "OpenGL" );
	int gl = maWidgetCreate(WIDGET_TYPE_GL_VIEW);
	maWidgetAddChild( screen, gl );
	return screen;
}



int createScreen(const char *title, int icon, const char *text)
{
	int screen = maWidgetCreate( WIDGET_TYPE_SCREEN );
	maWidgetSetProperty( screen, "title", title );
	maWidgetSetPropertyInt( screen, "icon", icon );

	int textView = maWidgetCreate( WIDGET_TYPE_LABEL );
	maWidgetSetProperty( textView, WIDGET_PROPERTY_TEXT, text );

	maWidgetAddChild( screen, textView );

	return screen;
}
