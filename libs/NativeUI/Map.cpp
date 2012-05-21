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

namespace NativeUI
{
    /**
     * Constructor.
     */
    Map::Map(): Widget(MAW_MAP)
    {
    }

    /**
     * Destructor.
     */
    Map::~Map()
    {
        mMapListeners.clear();
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

	/** TODO - decide the structure of a map point/map region.
	 * Sets the center of the map.
	 * @param center A MapPoint that contains the center information (latitude, longitude).
	 */
	 //void Map::setCenter(MapPoint center);

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

} // namespace NativeUI
