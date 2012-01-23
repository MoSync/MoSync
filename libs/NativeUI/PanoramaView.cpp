/*
Copyright (C) 2011 MoSync AB

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
	void PanoramaView::SetBackgroundImage(const MAHandle imageHandle)
	{
		setPropertyInt(MAW_PANORAMA_VIEW_BACKGROUND_IMAGE, imageHandle);
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

} // namespace NativeUI
