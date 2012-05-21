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
    class Map : public Widget
    {
    public:
        /**
         * Constructor.
         */
        Map();

        /**
         * Destructor.
         */
        virtual ~Map();

		/**
		 * Set the map type: road (only road, railway etc. structure visible) or satellite (terrain visible).
		 * @param mapType One of the MapType constants (MAP_TYPE_ROAD or MAP_TYPE_SATELLITE).
		 */
		virtual void setMapType(MapType mapType);

		/**
		 * Gets the current map type (MAP_TYPE_ROAD or MAP_TYPE_SATELLITE).
		 */
		virtual MapType getMapType();

		/**
		 * Sets the zoom level of the map.
		 * @param zoomLevel An integer between 0 (MAP_MIN_ZOOM_LEVEL) and 21 (MAP_MAX_ZOOM_LEVEL)(these are the
		 * google maps min/max zoom levels - the values for bing maps are between 1 and 21 so by setting the
		 * value 0 or 1 is equivalent on the windows phone platform).
		 */
		virtual void setZoomLevel(int zoomLevel);

		/**
		 * Gets the zoom level of the map (an integer between MAP_MIN_ZOOM_LEVEL - 0 and MAP_MAX_ZOOM_LEVEL - 21).
		 */
		virtual int getZoomLevel();

		/** TODO - decide the structure of a map point/map region.
		 * Sets the center of the map.
		 * @param center A MapPoint that contains the center information (latitude, longitude).
		 */
		 //virtual void setCenter(MapPoint center);

		/**
		 * Sets the interraction enabled property for the map.
		 * @param interractionEnabled If true, the map can be scrolled/zoomed.
		 */
		virtual void setInterractionEnabled(bool interractionEnabled);

        /**
         * Add a map event listener.
         * @param listener The listener that will receive map events.
         */
        virtual void addMapListener(MapListener* listener);

        /**
         * Remove a map event listener.
         * @param listener The listener that receives map events.
         */
        virtual void removeMapListener(MapListener* listener);

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
