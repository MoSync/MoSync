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
 * @file ImageButton.cpp
 * @author Bogdan Iusco
 *
 * Class for image buttons.
 * A image button intercepts touch events and notifies the observers.
 * Methods for setting the text, alignment, font size and color are inherited
 * from TextWidget.
 */

#include "ImageButton.h"
#include "ButtonListener.h"

namespace NativeUI
{
    /**
     * Constructor.
     */
    ImageButton::ImageButton() : TextWidget(MAW_IMAGE_BUTTON)
    {
    }

    /**
     * Destructor.
     */
    ImageButton::~ImageButton()
    {
        mButtonListeners.clear();
    }

    /**
     * Set the text displayed by the widget.
     * Note: It is not available on Android, as native
     * image buttons do not have text attached.
     * @param text The given text.
     */
    void ImageButton::setText(const MAUtil::String& text)
    {
        this->setProperty(MAW_IMAGE_BUTTON_TEXT, text);
    }

    /**
     * Get the text displayed by the widget.
     * Note: It is not available on Android, as native
     * image buttons do not have text attached.
     * @return The text displayed by the widget.
     */
     MAUtil::String ImageButton::getText()
     {
         return this->getPropertyString(MAW_IMAGE_BUTTON_TEXT);
     }

   /**
	* Set the foreground image of the button. This won't be scaled at all.
	* @param image MoSync handle to an uncompressed image resource.
	* @return Any of the following result codes:
	* - #MAW_RES_OK if the property could be set. In this case the image
	* path value will set to contain an empty string.
	* - #MAW_RES_INVALID_PROPERTY_VALUE if the image handle was invalid.
	*/
	int ImageButton::setImage(MAHandle image)
	{
		int res = setPropertyInt(MAW_IMAGE_BUTTON_IMAGE, image);
		mImageHandle = image;
		return res;
	}

	/**
	 * Get the foreground image of the button.
	 * @return Any of the following result codes:
	 * - image MoSync handle to an uncompressed image resource.
	 * - #MAW_RES_INVALID_PROPERTY_VALUE if the image handle was invalid
	 * - 0 if the image path was set successfully.
	 */
	MAHandle ImageButton::getImage()
	{
		return mImageHandle;
	}

	/**
	 * Set the foreground image that will be displayed using a file path.
	 * @param imagePath Path to an image file.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the property could be set. In this case the
	 * foreground image handle with be set to zero.
	 * set to zero.
	 * - #MAW_RES_INVALID_PROPERTY_VALUE if the image path was invalid.
	 */
	int ImageButton::setImagePath(const MAUtil::String& imagePath)
	{
		int result = this->setProperty(MAW_IMAGE_BUTTON_IMAGE_PATH, imagePath);
		if (result == MAW_RES_OK)
		{
			mImageHandle = 0;
		}
		return result;
	}

	/**
	 * Get the foreground image path.
	 * @return An image path if the displayed foreground image was created
	 * using a file path, or an empty string otherwise.
	 */
	MAUtil::String ImageButton::getImagePath()
	{
		return this->getPropertyString(MAW_IMAGE_BUTTON_IMAGE_PATH);
	}

    /**
     * Sets the background image. This will be scaled to fit the whole
     * widget (not keeping the aspect).
     * @param image MoSync handle to an uncompressed image resource.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the image handle was invalid.
     */
    int ImageButton::setBackgroundImage(MAHandle image)
    {
        return setPropertyInt(MAW_IMAGE_BUTTON_BACKGROUND_IMAGE, image);
    }

	/**
	 * Set the background image that will be displayed using a file path.
	 * @param imagePath Path to an image file.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the property could be set.
	 * - #MAW_RES_INVALID_PROPERTY_VALUE if the image path was invalid.
	 */
	int ImageButton::setBackgroundImagePath(const MAUtil::String& imagePath)
	{
		return this->setProperty(
			MAW_IMAGE_BUTTON_BACKGROUND_IMAGE_PATH,
			imagePath);
	}

	/**
	 * Get the background image path.
	 * @return An image path if the displayed background image was created
	 * using a file path, or an empty string otherwise.
	 */
	MAUtil::String ImageButton::getBackgroundImagePath()
	{
		return this->getPropertyString(MAW_IMAGE_BUTTON_BACKGROUND_IMAGE_PATH);
	}

	/**
	 * Set an image that will be displayed by the widget after user clicks on it.
	 * On iOS platform this property is named highlighted state.
	 * If this property is set, when the user taps on the widget its background image will
	 * be replaced by the image set here.
	 * Setting this property will reset the pressed image path value.
	 * @param image Handle to an image.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the property could be set.
	 * - #MAW_RES_INVALID_PROPERTY_VALUE if the handle was invalid.
	 */
	int ImageButton::setPressedImage(MAHandle image)
	{
		return this->setPropertyInt(
			MAW_IMAGE_BUTTON_PRESSED_IMAGE,
			image);
	}

	/**
	 * Get the pressed image handle.
	 * @return An image handle if the displayed pressed image was created
	 * using a handle, or zero otherwise.
	 */
	MAHandle ImageButton::getPressedImage()
	{
		return this->getPropertyInt(MAW_IMAGE_BUTTON_PRESSED_IMAGE);
	}

	/**
	 * Set a path to a image file that will be displayed by the widget
	 * after user clicks on it.
	 * On iOS platform this property is named highlighted state.
	 * If this property is set when the user taps on the widget, its background image will
	 * be replaced by the image set here.
	 * Setting this property will reset the pressed image value.
	 * @param imagePath Path to an image file.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the property could be set.
	 * - #MAW_RES_INVALID_PROPERTY_VALUE if the image path was invalid.
	 */
	int ImageButton::setPressedImagePath(const MAUtil::String& imagePath)
	{
		return this->setProperty(
			MAW_IMAGE_BUTTON_PRESSED_IMAGE_PATH,
			imagePath);
	}

	/**
	 * Get the pressed image path.
	 * @return An image path if the displayed pressed image was created
	 * using a file path, or an empty string otherwise.
	 */
	MAUtil::String ImageButton::getPressedImagePath()
	{
		return this->getPropertyString(MAW_IMAGE_BUTTON_PRESSED_IMAGE_PATH);
	}

    /**
     * Add an button event listener.
     * @param listener The listener that will receive button events.
     */
    void ImageButton::addButtonListener(ButtonListener* listener)
    {
        addListenerToVector(mButtonListeners, listener);
    }

    /**
     * Remove the button listener.
     * @param listener The listener that receives button events.
     */
    void ImageButton::removeButtonListener(ButtonListener* listener)
    {
        removeListenerFromVector(mButtonListeners, listener);
    }

    /**
     * This method is called when there is an event for this widget.
     * It passes on the event to all widget's listeners.
     * @param widgetEventData The data for the widget event.
     */
    void ImageButton::handleWidgetEvent(MAWidgetEventData* widgetEventData)
    {
        Widget::handleWidgetEvent(widgetEventData);

        if (MAW_EVENT_POINTER_PRESSED == widgetEventData->eventType)
        {
            for (int i = 0; i < mButtonListeners.size(); i++)
            {
                mButtonListeners[i]->buttonPressed(this);
            }
        }
        else if (MAW_EVENT_POINTER_RELEASED == widgetEventData->eventType)
        {
            for (int i = 0; i < mButtonListeners.size(); i++)
            {
                mButtonListeners[i]->buttonReleased(this);
            }
        }
        else if (MAW_EVENT_CLICKED == widgetEventData->eventType)
        {
            for (int i = 0; i < mButtonListeners.size(); i++)
            {
                mButtonListeners[i]->buttonClicked(this);
            }
        }
    }

} // namespace NativeUI
