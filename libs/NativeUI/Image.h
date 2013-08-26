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
 * @file Image.h
 * @author Bogdan Iusco
 *
 * \brief Class for images.
 */

#ifndef NATIVEUI_IMAGE_H_
#define NATIVEUI_IMAGE_H_

#include "Widget.h"

namespace NativeUI
{

    /**
     * \brief Types of scaling for the image.
     */
    enum ImageScaleMode
    {
        IMAGE_SCALE_NONE = 0,
        IMAGE_SCALE_XY,
        IMAGE_SCALE_PRESERVE_ASPECT
    };

    /**
     * \brief Class for images.
     */
    class Image : public Widget
    {
    public:
        /**
         * Constructor.
         */
        Image();

        /**
         * Destructor.
         */
        virtual ~Image();

        /**
         * Set the image that will be displayed.
         * Setting this value will change the image path value to an empty string.
         * @param image MoSync handle to an uncompressed image resource.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the image handle was invalid.
         */
        virtual int setImage(MAHandle image);

        /**
         * Get the displayed image handle.
         * @return A value greater than zero representing a MoSync handle to an
         * uncompressed image resource, or zero if the displayed image was created
         * using set image path function.
         */
        MAHandle getImage();

        /**
         * Set the image that will be displayed using a file path.
         * Setting this value will change the image handle value to zero.
         * @param imagePath Path to an image file.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the image path was invalid.
         */
        int setImagePath(const MAUtil::String& imagePath);

        /**
         * Get the image path.
         * @return An image path if the displayed image was created using
		 * a file path, or an enpty string otherwise.
         */
        MAUtil::String getImagePath();

        /**
         * Set the type of scaling that should be applied to the image.
         * @param scaleMode The type of scaling.
         * One of the following:
         *  #IMAGE_SCALE_NONE
         *  #IMAGE_SCALE_XY
         *  #IMAGE_SCALE_PRESERVE_ASPECT
         */
        virtual void setScaleMode(ImageScaleMode scaleMode);
    };

} // namespace NativeUI

#endif /* NATIVEUI_IMAGE_H_ */

/*! @} */
