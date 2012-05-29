/*
Copyright (C) 2012 MoSync AB

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
 * @file MainScreen.h
 * @author Spiridon Alexandru
 */

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

#include <maapi.h>
#include <MAUtil/util.h>

// Include all the wrappers.
#include <NativeUI/Widgets.h>

using namespace NativeUI;

/**
 * Class that creates a screen that displays an ListView.
 */
class MainScreen:
	public Screen,
	public MapListener
{
public:
	/**
	 * Constructor.
	 */
	MainScreen();

	/**
	 * Destructor.
	 */
	~MainScreen();

private:
	/**
	 * Creates and adds main layout to the screen.
	 */
	void createMainLayout();

    /**
	 * This method is called when the zoom level of the map changes (at a double tap
	 * on the google maps for example).
     * @param map The map object that generated the event.
     */
    virtual void mapZoomLevelChanged(Map* map);

    /**
	 * This method is called when the visible region on the map is changed (on a drag/scroll
	 * for example).
	 * @param map The map object that generated the event.
	 */
    virtual void mapRegionChanged(Map* map);

private:
	/**
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;
	Label* mTitleLabel;
	Map* mMap;
};


#endif /* MAINSCREEN_H_ */
