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
