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
 * @file CameraPreview.cpp
 * @author Emma Tresanszki
 *
 * An instance of CameraPreview allows users to visualize the camera controller.
 */

#include "CameraPreview.h"

namespace NativeUI
{

	/**
	 * Constructor.
	 */
	CameraPreview::CameraPreview():
		Widget(MAW_CAMERA_PREVIEW)
	{
		// Set some common default values.
		fillSpaceVertically();
		fillSpaceHorizontally();
	}

	/**
	 * Bind the preview to the currently selected camera.
	 * @return 1 on success or 0 for failure
	 */
	int CameraPreview::bindToCurrentCamera()
	{
		return maCameraSetPreview(this->getWidgetHandle());
	}

	/**
	 * Destructor.
	 */
	CameraPreview::~CameraPreview()
	{
	}

} // namespace NativeUI
