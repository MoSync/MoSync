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
 * @file MapPin.cpp
 * @author Spiridon Alexandru
 *
 * @brief Class that represents a pin on the map (it has two coordinates: latitude and longitude).
 */

#define MAP_EPS 0.000001

#include "MapPin.h"
#include <madmath.h>

namespace NativeUI
{
	/**
	 * Constructor
	 */
	MapPin::MapPin() : Widget(MAW_MAP_PIN)
	{
	}

	/**
	 * Constructor: creates a MapPin object based on a given location.
	 * @param location The location of the map pin.
	 */
	MapPin::MapPin(const Location& location) : Widget(MAW_MAP_PIN), Location(location.getLatitude(), location.getLongitude())
	{
		this->setProperty(MAW_MAP_PIN_LATITUDE, MAUtil::doubleToString(location.getLatitude()));
		this->setProperty(MAW_MAP_PIN_LONGITUDE, MAUtil::doubleToString(location.getLongitude()));
	}

	/**
	 * Constructor: creates a MapPin object based on a given latitude and longitude.
	 * @param latitude The latitude of the map pin.
	 * @param longitude The longitde of the map pin.
	 */
	MapPin::MapPin(double latitude, double longitude) : Widget(MAW_MAP_PIN), Location(latitude, longitude)
	{
		this->setProperty(MAW_MAP_PIN_LATITUDE, MAUtil::doubleToString(latitude));
		this->setProperty(MAW_MAP_PIN_LONGITUDE, MAUtil::doubleToString(longitude));
	}

	/**
	 * Destructor.
	 */
	MapPin::~MapPin()
	{
	}

	/**
	 * Setter for the map pin text.
	 */
	void MapPin::setText(const MAUtil::String& text)
	{
		this->setProperty(MAW_MAP_PIN_TEXT, text);
	}

	/**
	 * Getter for the map pin text.
	 */
	MAUtil::String MapPin::getText()
	{
		return this->getPropertyString(MAW_MAP_PIN_TEXT);
	}

	/**
	 * Setter for the location member.
	 * @param location The new location for the map point.
	 */
	void MapPin::setLocation(const Location& location)
	{
		this->setProperty(MAW_MAP_PIN_LATITUDE, MAUtil::doubleToString(location.getLatitude()));
		this->setProperty(MAW_MAP_PIN_LONGITUDE, MAUtil::doubleToString(location.getLongitude()));
		this->setLatitude(location.getLatitude());
		this->setLongitude(location.getLongitude());
	}

	/**
	 * Add a map pin event listener.
	 * @param listener The listener that will receive map pin events.
	 */
	void MapPin::addMapPinListener(MapPinListener* listener)
	{
		addListenerToVector(mMapPinListeners, listener);
	}

	/**
	 * Remove a map pin event listener.
	 * @param listener The listener that receives map pin events.
	 */
	void MapPin::removeMapPinListener(MapPinListener* listener)
	{
		removeListenerFromVector(mMapPinListeners, listener);
	}

	/**
     * This method is called when there is an event for this widget.
     * It passes on the event to all widget's listeners.
     * @param widgetEventData The data for the widget event.
     */
    void MapPin::handleWidgetEvent(MAWidgetEventData* widgetEventData)
    {
        Widget::handleWidgetEvent(widgetEventData);

        if (MAW_EVENT_MAP_PIN_CLICKED == widgetEventData->eventType)
        {
            for (int i = 0; i < mMapPinListeners.size(); i++)
            {
                mMapPinListeners[i]->mapPinClicked(this);
            }
        }
    }

	/**
	 * Equality operator.
	 */
	bool MapPin::operator==(const MapPin &mapPin) const
	{
		if (fabs(mLatitude - mapPin.getLatitude()) < MAP_EPS &&
			fabs(mLongitude - mapPin.getLongitude()) < MAP_EPS)
		{
			return true;
		}
		return false;
	}
} // end of NativeUI namespace
