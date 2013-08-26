/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/**
 * @file MainScreen.h
 * @author Spiridon Alexandru
 */

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

#include <maapi.h>
#include <MAUtil/util.h>
#include <MAUtil/Vector.h>

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
	 * A vector containing map pins with the coordinates randomized.
	 */
	MAUtil::Vector<MapPin*> mMapPins;
};


#endif /* MAINSCREEN_H_ */
