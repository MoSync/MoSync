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
 * @file PanoramaView.cpp
 * @author Ciprian Filipas
 *
 * Class that represents a visible panorama view.
 */

#include "PanoramaView.h"

namespace NativeUI
{

	/**
	 * Constructor.
	 */
	PanoramaView::PanoramaView() :
		Screen(MAW_PANORAMA_VIEW)
	{
	}

	/**
	 * Destructor.
	 */
	PanoramaView::~PanoramaView()
	{
	}

	/**
	 * Set the background image of the panorama view. The background image
	 * is displayed behind the child screens.
	 * @param imageHandle Handle to an image.
	 */
	void PanoramaView::setBackgroundImage(const MAHandle imageHandle)
	{
		setPropertyInt(MAW_PANORAMA_VIEW_BACKGROUND_IMAGE, imageHandle);
	}

	/**
	 * Get the background image of the panorama view.
	 * @return A value greater than zero representing a MoSync handle to an
	 * uncompressed image resource, or zero if the property was not set.
	 */
	MAHandle PanoramaView::getBackgroundImage()
	{
		return getPropertyInt(MAW_PANORAMA_VIEW_BACKGROUND_IMAGE);
	}

	/**
	 * Add a new screen to the panorama view.
     * @param screen The screen to be add at the panorama view.
	 */
	void PanoramaView::addScreen(Screen* screen)
	{
		this->addChild(screen);
	}

	/**
	 * Show a given screen.
     * @param index The index of the screen to show.
     * Index starts at zero.
	 */
	void PanoramaView::setActiveScreen(const int& index)
	{
		this->setPropertyInt(MAW_PANORAMA_VIEW_CURRENT_SCREEN, index);
	}

	/**
	 * Returns the index of the current screen.
     * Index starts at zero.
     * @return The index of the current screen.
	 */
	int PanoramaView::getActiveScreen()
	{
		return (this->getPropertyInt(MAW_TAB_SCREEN_CURRENT_TAB));
	}

	/**
	 * Check if panorama view is shown.
	 * @return true if the panorama view is visible, false otherwise.
	 */
	bool PanoramaView::isShown()
	{
        MAUtil::String value = this->getPropertyString(MAW_PANORAMA_VIEW_IS_SHOWN);
        return (strcmp(value.c_str(), "true") == 0) ? true : false;
	}

} // namespace NativeUI
