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
	public ButtonListener,
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
	 * Creates and adds the visible area get/set layout to the screen.
	 */
	void createVisibleAreaLayout();

	/**
	 * Creates and adds the map pin add/remove layout to the screen.
	 */
	void createMapPinLayout();

	/**
	 * Creates and adds the zoom level layout to the screen.
	 */
	void createZoomLevelLayout();

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

    /**
	 * This method is called when the user clicks on a map pin.
	 * @param mapPin The map pin object that generated the event.
	 */
	virtual void mapPinClicked(MapPin* mapPin);

    /**
     * This method is called if the touch-up event was inside the
     * bounds of the button.
     * @param button The button object that generated the event.
     */
    virtual void buttonClicked(Widget* button);

private:
	/**
	 * Main layout.
	 */
	VerticalLayout* mMainLayout;

	/**
	 * Horizontal layout that contains the set/get visible area buttons.
	 */
	HorizontalLayout* mVisibleAreaLayout;

	/**
	 * Horizontal layout that contains the zoom level title and edit box.
	 */
	HorizontalLayout* mZoomLevelLayout;

	/**
	 * Horizontal layout that contains the add/remove pin buttons.
	 */
	HorizontalLayout* mPinsLayout;

	/**
	 * The application title label
	 */
	Label* mTitleLabel;

	/**
	 * Label used to view the visible area coordinates on the map.
	 */
	Label* mVisibleAreaCoordinatesLabel;

	/**
	 * Button used to add a pin on the map.
	 */
	Button* mAddPinToMap;

	/**
	 * Button used to remove a pin from the map.
	 */
	Button* mRemovePinFromMap;

	/**
	 * Button used to get the visible area coordinates of the map and
	 * show the to the user.
	 */
	Button* mGetVisibleAreaButton;

	/**
	 * Button used to set the visible area on the map (the map will zoom and scroll
	 * when the button is clicked).
	 */
	Button* mSetVisibleAreaButton;

	/**
	 * Button used to set the zoom level of the map.
	 */
	Button *mSetZoomLevel;

	/**
	 * Button used to center the map.
	 */
	Button *mSetCenterButton;

	/**
	 * EditBox used to set the map zoom level.
	 */
	EditBox* mZoomLevelEditBox;

	/**
	 * The map on which all the operations will take place.
	 */
	Map* mMap;

	/**
	 * A example map pin to be added on the map.
	 */
	MapPin* mMapPin;
};


#endif /* MAINSCREEN_H_ */
