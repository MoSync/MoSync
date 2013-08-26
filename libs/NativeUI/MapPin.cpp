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
