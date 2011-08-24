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
 * @file Image.h
 * @author Bogdan Iusco
 *
 * Class for images.
 */

#include "Image.h"

namespace NativeUI
{
    /**
     * Constructor.
     */
    Image::Image(): Widget(MAW_IMAGE)
    {
    }

    /**
     * Destructor.
     */
    Image::~Image()
    {
    }

    /**
     * Set the image that will be displayed.
     * @param image MoSync handle to an uncompressed image resource.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the image handle was invalid.
     */
    int Image::setImage(MAHandle image)
    {
        return this->setPropertyInt(MAW_IMAGE_IMAGE, image);
    }

    /**
     * Set the type of scaling that should be applied to the image.
     * @param scaleMode The type of scaling.
     */
    void Image::setScaleMode(ImageScaleMode scaleMode)
    {
        MAUtil::String scaleModeString;
        switch (scaleMode)
        {
            case IMAGE_SCALE_NONE:
                scaleModeString = "none";
                break;
            case IMAGE_SCALE_XY:
                scaleModeString = "scaleXY";
                break;
            case IMAGE_SCALE_PRESERVE_ASPECT:
                scaleModeString = "scalePreserveAspect";
        }

        this->setProperty(MAW_IMAGE_SCALE_MODE, scaleModeString);
    }

} // namespace NativeUI
