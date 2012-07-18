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

/*! \addtogroup NativeUILib
 *  @{
 */

/**
 *  @defgroup NativeUILib Native UI Library
 *  @{
 */

/**
 * @file Map.h
 * @author Spiridon Alexandru
 *
 * @brief Class for the map widget.
 * Shows the map.
 */

#ifndef NATIVEUI_MAP_H_
#define NATIVEUI_MAP_H_

#include "Widget.h"
#include "MapLocation.h"
#include "MapPin.h"
#include "MapPinListener.h"
#include <MAUtil/Vector.h>

namespace NativeUI
{
    // Forward declaration.
    class MapListener;

	#define MAP_MIN_ZOOM_LEVEL 0
	#define MAP_MAX_ZOOM_LEVEL 21

    /**
     * @brief The map can be presented in two modes: the road mode (doesn't contain real terrain images
	 * taken from satellite and shows the road, railway etc. structure of a certain area) and the satellite
	 * mode (the terrain can be visible).
     */
    enum MapType
    {
        MAP_TYPE_ROAD = 0,
        MAP_TYPE_SATELLITE
    };

    /**
     * @brief Map widget wrapper class.
     *
     * This widget contains a google(android, ios)/bing(windows phone) map, allows basic
	 * user interraction (scroll/drag, zoom), handles the map events, forwards them to all
	 * the registered listeners and offers the possibility to add an overlay (a annotation/pin
	 * for example).
     */
    class Map : public Widget,
				public MapPinListener
    {
    public:
        /**
         * Constructor.
         */
        Map();

		/**
		 * Constructor with the google maps and bing maps api key as parameters (on the iOS platform the api key is not required but the
		 * developer must provide a google api key for Android and a bing key for the Windows Phone platform).
		 * @param googleApiKey The google maps api key.
		 * @param bingApiKey The bing maps api key.
		 */
		Map(const MAUtil::String& googleApiKey, const MAUtil::String& bingApiKey);

        /**
         * Destructor.
         */
        ~Map();

		/**
         * Add a map pin as a view on the map.
         * @param pin The map pin that will be added.
         * The ownership of the map pin is passed to this function.
         * When the parent map will be destroyed, all child map pins
         * will be deleted.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the map pin could be added to the parent.
         * - #MAW_RES_INVALID_HANDLE if any of the handles were invalid.
         * - #MAW_RES_ERROR if it could not be added for some other reason.
         */
        int addMapPin(MapPin* pin);

		/**
         * Remove a map pin from its parent map (but does not destroy it).
         * When the parent map will be destroyed, the child map pin will not
         * be deleted.
         * @param pin The map pin to be removed.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the map pin could be removed from the parent map.
         * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
         * - #MAW_RES_ERROR otherwise.
         */
        int removeMapPin(MapPin* pin);

		/**
		 * Set the map type: road (only road, railway etc. structure visible) or satellite (terrain visible).
		 * @param mapType One of the MapType constants (MAP_TYPE_ROAD or MAP_TYPE_SATELLITE).
		 */
		void setMapType(MapType mapType);

		/**
		 * Gets the current map type (MAP_TYPE_ROAD or MAP_TYPE_SATELLITE).
		 */
		MapType getMapType();

		/**
		 * Sets the zoom level of the map.
		 * @param zoomLevel An integer between 0 (MAP_MIN_ZOOM_LEVEL) and 21 (MAP_MAX_ZOOM_LEVEL)(these are the
		 * google maps min/max zoom levels - the values for bing maps are between 1 and 21 so by setting the
		 * value 0 or 1 is equivalent on the windows phone platform).
		 */
		void setZoomLevel(int zoomLevel);

		/**
		 * Gets the zoom level of the map (an integer between MAP_MIN_ZOOM_LEVEL - 0 and MAP_MAX_ZOOM_LEVEL - 21).
		 */
		int getZoomLevel();

		/**
		 * Sets the center of the map.
		 * @param center A MapLocation that contains the center information (latitude, longitude and zoom level).
		 */
		void setCenter(const MapLocation& center);

		/**
		 * Sets the interraction enabled property for the map.
		 * @param interractionEnabled If true, the map can be scrolled/zoomed.
		 */
		void setInterractionEnabled(bool interractionEnabled);

		/**
		 * Centers the map around the map center.
		 */
		void centerMap();

		/**
		 * Scrolls and zooms the map on the area provided.
		 * @param upperLeftCornerLatitude The upper left corner latitude of the area.
		 * @param upperLeftCornerLongitude The upper left corner longitude of the area.
		 * @param lowerRightCornerLatitude The lower right corner latitude of the area.
		 * @param lowerRightCornerLongitude The lower right corner longitude of the area.
		 */
		void setVisibleArea(double upperLeftCornerLatitude, double upperLeftCornerLongitude,
							double lowerRightCornerLatitude, double lowerRightCornerLongitude);

		/**
		 * Gets the current map visible area.
		 * @return A vector containing two location points: the upper left corner and the lower right corner if
		 * the value from the runtime is corrent and NULL otherwise.
		 */
		MAUtil::Vector<Location> getVisibleArea();

        /**
         * Add a map event listener.
         * @param listener The listener that will receive map events.
         */
        void addMapListener(MapListener* listener);

        /**
         * Remove a map event listener.
         * @param listener The listener that receives map events.
         */
        void removeMapListener(MapListener* listener);

		/**
		 * This method is called when the user clicks on a map pin.
         * @param mapPin The map pin object that generated the event.
         */
		virtual void mapPinClicked(MapPin* mapPin);
	private:
		/**
		 * Checks if a pin is already on the map.
		 */
		bool containsPin(MapPin* mapPin);

    protected:
        /**
         * This method is called when there is an event for this widget.
         * It passes on the event to all widget's listeners.
         * @param widgetEventData The data for the widget event.
         */
        virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData);

    private:
        /**
         * Array with map listeners.
         */
        MAUtil::Vector<MapListener*> mMapListeners;
    };

} // namespace NativeUI

#endif /* NATIVEUI_MAP_H_ */

/*! @} */
