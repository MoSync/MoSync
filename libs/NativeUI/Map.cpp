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
 * @file Map.cpp
 * @author Spiridon Alexandru
 *
 * Map widget wrapper class.
 */

#include "Map.h"
#include "MapListener.h"
#include <maapi.h>

using namespace std;

namespace NativeUI
{
    /**
     * Constructor.
     */
    Map::Map(): Widget(MAW_MAP)
    {
    }

	/**
	 * Constructor with the google maps and bing maps api key as parameters (on the iOS platform the api key is not required but the
	 * developer must provide a google api key for Android and a bing key for the Windows Phone platform).
	 * @param googleApiKey The google maps api key.
	 * @param bingApiKey The bing maps api key.
	 */
	Map::Map(const MAUtil::String& googleApiKey, const MAUtil::String& bingApiKey) : Widget(MAW_MAP)
	{
		this->setProperty(MAW_MAP_API_KEY_GOOGLE, googleApiKey);
		this->setProperty(MAW_MAP_API_KEY_BING, bingApiKey);
	}

    /**
     * Destructor.
     */
    Map::~Map()
    {
        mMapListeners.clear();
    }

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
	int Map::addMapPin(MapPin* pin)
	{
		if (!containsPin(pin))
		{
			pin->addMapPinListener(this);
			return Widget::addChild(pin);
		}
		return MAW_RES_ERROR;
	}

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
	int Map::removeMapPin(MapPin* pin)
	{
		if (containsPin(pin))
		{
			pin->removeMapPinListener(this);
			return Widget::removeChild(pin);
		}
		return MAW_RES_ERROR;
	}

	/**
	 * Checks if a pin is already on the map.
	 */
	bool Map::containsPin(MapPin* mapPin)
	{
		for (int i = 0; i < countChildWidgets(); i++)
		{
			MapPin* currentMapPin = (MapPin*)getChild(i);
			if (*currentMapPin == *mapPin)
			{
				return true;
			}
		}
		return false;
	}

	/**
	 * Set the map type: road (only road, railway etc. structure visible) or satellite (terrain visible).
	 * @param mapType One of the MapType constants (MAP_TYPE_ROAD or MAP_TYPE_SATELLITE).
	 */
	void Map::setMapType(MapType mapType)
	{
		this->setPropertyInt(MAW_MAP_TYPE, mapType);
	}

	/**
	 * Gets the current map type (MAP_TYPE_ROAD or MAP_TYPE_SATELLITE).
	 */
	MapType Map::getMapType()
	{
		return (MapType)this->getPropertyInt(MAW_MAP_TYPE);
	}

	/**
	 * Sets the zoom level of the map.
	 * @param zoomLevel An integer between 0 (MAP_MIN_ZOOM_LEVEL) and 21 (MAP_MAX_ZOOM_LEVEL)(these are the
	 * google maps min/max zoom levels - the values for bing maps are between 1 and 21 so by setting the
	 * value 0 or 1 is equivalent on the windows phone platform).
	 */
	void Map::setZoomLevel(int zoomLevel)
	{
		if (zoomLevel >= MAP_MIN_ZOOM_LEVEL && zoomLevel <= MAP_MAX_ZOOM_LEVEL)
		{
			this->setPropertyInt(MAW_MAP_ZOOM_LEVEL, zoomLevel);
		}
		else if (zoomLevel < MAP_MIN_ZOOM_LEVEL)
		{
			this->setPropertyInt(MAW_MAP_ZOOM_LEVEL, MAP_MIN_ZOOM_LEVEL);
		}
		else if (zoomLevel > MAP_MAX_ZOOM_LEVEL)
		{
			this->setPropertyInt(MAW_MAP_ZOOM_LEVEL, MAP_MAX_ZOOM_LEVEL);
		}
	}

	/**
	 * Gets the zoom level of the map (an integer between MAP_MIN_ZOOM_LEVEL - 0 and MAP_MAX_ZOOM_LEVEL - 21).
	 */
	int Map::getZoomLevel()
	{
		return this->getPropertyInt(MAW_MAP_ZOOM_LEVEL);
	}

	/**
	 * Sets the center of the map.
	 * @param center A MapLocation that contains the center information (latitude, longitude and zoom level).
	 */
	 void Map::setCenter(const MapLocation& center)
	 {
		// set the latitude, longitude and zoom level properties of the map center
		this->setProperty(MAW_MAP_CENTER_LATITUDE, MAUtil::doubleToString(center.getLatitude()));
		this->setProperty(MAW_MAP_CENTER_LONGITUDE, MAUtil::doubleToString(center.getLongitude()));
		this->setProperty(MAW_MAP_CENTER_ZOOM_LEVEL, MAUtil::integerToString(center.getZoomLevel()));
	 }

	/**
	 * Sets the interraction enabled property for the map.
	 * @param interractionEnabled If true, the map can be scrolled/zoomed.
	 */
	void Map::setInterractionEnabled(bool interractionEnabled)
	{
		if (interractionEnabled)
		{
			this->setProperty(MAW_MAP_INTERRACTION_ENABLED, "true");
		}
		else
		{
			this->setProperty(MAW_MAP_INTERRACTION_ENABLED, "false");
		}
	}

	/**
	 * Centers the map around the map center.
	 */
	void Map::centerMap()
	{
		this->setProperty(MAW_MAP_CENTERED, "true");
	}

	/**
	 * Scrolls and zooms the map on the area provided.
	 * @param upperLeftCornerLatitude The upper left corner latitude of the area.
	 * @param upperLeftCornerLongitude The upper left corner longitude of the area.
	 * @param lowerRightCornerLatitude The lower right corner latitude of the area.
	 * @param lowerRightCornerLongitude The lower right corner longitude of the area.
	 */
	void Map::setVisibleArea(double upperLeftCornerLatitude, double upperLeftCornerLongitude,
						double lowerRightCornerLatitude, double lowerRightCornerLongitude)
	{
		// set the visible area coordinates (upper left corner and lower left corner coordinates)
		this->setProperty(MAW_MAP_VISIBLE_AREA_UPPER_LEFT_CORNER_LATITUDE, MAUtil::doubleToString(upperLeftCornerLatitude,6));
		this->setProperty(MAW_MAP_VISIBLE_AREA_UPPER_LEFT_CORNER_LONGITUDE, MAUtil::doubleToString(upperLeftCornerLongitude,6));
		this->setProperty(MAW_MAP_VISIBLE_AREA_LOWER_RIGHT_CORNER_LATITUDE, MAUtil::doubleToString(lowerRightCornerLatitude,6));
		this->setProperty(MAW_MAP_VISIBLE_AREA_LOWER_RIGHT_CORNER_LONGITUDE, MAUtil::doubleToString(lowerRightCornerLongitude,6));
		// center the map on the visible area
		this->setProperty(MAW_MAP_CENTERED_ON_VISIBLE_AREA, "true");
	}

	/**
	 * Gets the current map visible area.
	 * @return A vector containing two location points: the upper left corner and the lower right corner if
	 * the value from the runtime is corrent and NULL otherwise.
	 */
	MAUtil::Vector<Location> Map::getVisibleArea()
	{
		MAUtil::Vector<Location> corners;

		MAUtil::String upperLeftCornerLatitude = this->getPropertyString(MAW_MAP_VISIBLE_AREA_UPPER_LEFT_CORNER_LATITUDE);
		MAUtil::String upperLeftCornerLongitude = this->getPropertyString(MAW_MAP_VISIBLE_AREA_UPPER_LEFT_CORNER_LONGITUDE);
		MAUtil::String lowerRightCornerLatitude = this->getPropertyString(MAW_MAP_VISIBLE_AREA_LOWER_RIGHT_CORNER_LATITUDE);
		MAUtil::String lowerRightCornerLongitude = this->getPropertyString(MAW_MAP_VISIBLE_AREA_LOWER_RIGHT_CORNER_LONGITUDE);

		Location upperLeftCorner, lowerRightCorner;

		upperLeftCorner.setLatitude(MAUtil::stringToDouble(upperLeftCornerLatitude));
		upperLeftCorner.setLongitude(MAUtil::stringToDouble(upperLeftCornerLongitude));
		lowerRightCorner.setLatitude(MAUtil::stringToDouble(lowerRightCornerLatitude));
		lowerRightCorner.setLongitude(MAUtil::stringToDouble(lowerRightCornerLongitude));
		corners.add(upperLeftCorner);
		corners.add(lowerRightCorner);

		return corners;
	}

    /**
     * Add a map event listener.
     * @param listener The listener that will receive map events.
     */
    void Map::addMapListener(MapListener* listener)
    {
        addListenerToVector(mMapListeners, listener);
    }

    /**
     * Remove a map listener.
     * @param listener The listener that receives map events.
     */
    void Map::removeMapListener(MapListener* listener)
    {
        removeListenerFromVector(mMapListeners, listener);
    }

    /**
     * This method is called when there is an event for this widget.
     * It passes on the event to all widget's listeners.
     * @param widgetEventData The data for the widget event.
     */
    void Map::handleWidgetEvent(MAWidgetEventData* widgetEventData)
    {
        Widget::handleWidgetEvent(widgetEventData);

        if (MAW_EVENT_MAP_ZOOM_LEVEL_CHANGED == widgetEventData->eventType)
        {
            for (int i = 0; i < mMapListeners.size(); i++)
            {
                mMapListeners[i]->mapZoomLevelChanged(this);
            }
        }
        else if (MAW_EVENT_MAP_REGION_CHANGED == widgetEventData->eventType)
        {
            for (int i = 0; i < mMapListeners.size(); i++)
            {
                mMapListeners[i]->mapRegionChanged(this);
            }
        }
    }

	/**
	 * This method is called when the user clicks on a map pin.
	 * @param mapPin The map pin object that generated the event.
	 */
	void Map::mapPinClicked(MapPin* mapPin)
	{
		for (int i = 0; i < mMapListeners.size(); i++)
		{
			mMapListeners[i]->mapPinClicked(mapPin);
		}
	}

} // namespace NativeUI
