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

/*! \addtogroup NativeUILib
 *  @{
 */

/**
 *  @defgroup NativeUILib Native UI Library
 *  @{
 */

/**
 * @file CameraPreview.h
 * @author Emma Tresanszki
 *
 * \brief An instance of CameraPreview allows users to visualize the camera controller.
 */

#ifndef NATIVEUI_CAMERAPREVIEW_H_
#define NATIVEUI_CAMERAPREVIEW_H_

#include "Widget.h"

namespace NativeUI
{
	/** \brief An instance of CameraPreview allows users to visualize the
	 * camera controller.
	 * Note: This is not available on Windows Phone 7.
	 *
	 * A Camera Preview is a widget that is used to display  a live view
	 * of the camera rendered by the viewfinder.
	 * It has no specific properties (besides the base widget properties)
	 * attached to it because the camera is mostly handled via syscalls.
	 * So this widget is only used to enable the camera controller to be
	 * mapped with UI in devices that support Native UI.
	 * After creating the widget, remember to bind it to the camera controller
	 * via a call to #maCameraSetPreview (passing the value from it's
	 * getWidgetHandle() method), or by calling the bindToCurrentCamera() method.
	 */
	class CameraPreview : public Widget
	{
	public:
		/**
		 * Constructor.
		 */
		CameraPreview();

		/**
         * Bind the preview to the currently selected camera.
		 * @return 1 on success or 0 for failure
         */
        virtual int bindToCurrentCamera();

		/**
		 * Destructor.
		 */
		virtual ~CameraPreview();

	};
} // namespace NativeUI

#endif /* NATIVEUI_CAMERAPREVIEW_H_ */

/*! @} */
