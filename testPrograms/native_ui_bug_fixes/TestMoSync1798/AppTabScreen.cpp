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
 * @file AppTabScreen.cpp
 * @author Bogdan Iusco.
 * @brief Application's tab screen class.
 */

#define FIRST_SCREEN_TITLE "First screen"
#define SECOND_SCREEN_TITLE "Second screen"

#define FIRST_SCREEN_COLOR 0x123456
#define SECOND_SCREEN_COLOR 0xFF0000

#define TAB_SCREEN_TITLE "Tab Screen"

#include "AppTabScreen.h"

/**
 * Constructor.
 */
AppTabScreen::AppTabScreen():
	TabScreen()
{
	this->setTitle(TAB_SCREEN_TITLE);

	this->createFirstScreen();
	this->createSecondScreen();

	this->addTab(mFirstScreen);
	this->addTab(mSecondScreen);
}

/**
 * Destructor.
 */
AppTabScreen::~AppTabScreen()
{

}

/**
 * Create and initialize first screen.
 */
void AppTabScreen::createFirstScreen()
{
	mFirstScreen = new Screen();
	mFirstScreen->setTitle(FIRST_SCREEN_TITLE);
	VerticalLayout* layout = new VerticalLayout();
	layout->setBackgroundColor(FIRST_SCREEN_COLOR);
	mFirstScreen->setMainWidget(layout);
}

/**
 * Create and initialize second screen.
 */
void AppTabScreen::createSecondScreen()
{
	mSecondScreen = new Screen();
	mSecondScreen->setTitle(SECOND_SCREEN_TITLE);
	VerticalLayout* layout = new VerticalLayout();
	layout->setBackgroundColor(SECOND_SCREEN_COLOR);
	mSecondScreen->setMainWidget(layout);
}
