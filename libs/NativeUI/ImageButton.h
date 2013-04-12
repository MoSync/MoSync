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

/*! \addtogroup NativeUILib
 *  @{
 */

/**
 *  @defgroup NativeUILib Native UI Library
 *  @{
 */

/**
 * @file ImageButton.h
 * @author Bogdan Iusco
 *
 * \brief Class for image buttons.
 * A image button intercepts touch events and notifies the observers.
 * Methods for setting the text, alignment, font size and color are inherited
 * from TextWidget.
 */

#ifndef NATIVEUI_IMAGE_BUTTON_H_
#define NATIVEUI_IMAGE_BUTTON_H_

#include "TextWidget.h"

namespace NativeUI
{

    // Forward declaration.
    class ButtonListener;

    /**
     * \brief Class for image buttons.
     *
     * A image button intercepts touch events and notifies the observers.
     * Methods for setting the text, alignment, font size and color are inherited
     * from TextWidget.
     */
    class ImageButton : public TextWidget
    {
    public:
        /**
         * Constructor.
         */
        ImageButton();

        /**
         * Destructor.
         */
        virtual ~ImageButton();

        /**
         * Set the text displayed by the widget.
         * Note: It is not available on Android, as native
         * image buttons do not have text attached.
         * @param text The given text.
         */
        virtual void setText(const MAUtil::String& text);

        /**
         * Get the text displayed by the widget.
         * Note: It is not available on Android, as native
         * image buttons do not have text attached.
         * @return The text displayed by the widget.
         */
        virtual MAUtil::String getText();

		/**
		 * Set the foreground image of the button. This won't be scaled at all.
		 * Setting this value will change the image path value to an empty string.
		 * @param image MoSync handle to an uncompressed image resource.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the property could be set. In this case the
		 * foreground image path value will set to contain an empty string.
		 * - #MAW_RES_INVALID_PROPERTY_VALUE if the image handle was invalid.
		 */
		virtual int setImage(MAHandle image);

		/**
		 * Get the foreground image of the button.
		 * @return A value greater than zero representing a MoSync handle to an
		 * uncompressed image resource, or zero if the foreground image was created
		 * using set image path function.
		 */
		virtual MAHandle getImage();

		/**
		 * Set the foreground image that will be displayed using a file path.
		 * Setting this value will change the foreground image handle value to zero.
		 * @param imagePath Path to an image file.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the property could be set. In this case the
		 * foreground image handle with be set to zero.
		 * set to zero.
		 * - #MAW_RES_INVALID_PROPERTY_VALUE if the image path was invalid.
		 */
		int setImagePath(const MAUtil::String& imagePath);

		/**
		 * Get the foreground image path.
		 * @return An image path if the displayed foreground image was created
		 * using a file path, or an empty string otherwise.
		 */
		MAUtil::String getImagePath();

        /**
         * Sets the background image. This will be scaled to fit the whole
         * widget (not keeping the aspect).
         * Setting this value will change the background image path value to an
         * empty string.
         * @param image MoSync handle to an uncompressed image resource.
         * @return Any of the following result codes:
         * - #MAW_RES_OK if the property could be set.
         * - #MAW_RES_INVALID_PROPERTY_VALUE if the image handle was invalid.
         */
        virtual int setBackgroundImage(MAHandle image);

		/**
		 * Get the background image of the button.
		 * @return A value greater than zero representing a MoSync handle to an
		 * uncompressed image resource, or zero if the background image was created
		 * using set background image path function.
		 */
		MAHandle getBackgroundImage();

		/**
		 * Set the background image that will be displayed using a file path.
		 * Setting this value will change the background image handle value to zero.
		 * @param imagePath Path to an image file.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the property could be set.
		 * - #MAW_RES_INVALID_PROPERTY_VALUE if the image path was invalid.
		 */
		int setBackgroundImagePath(const MAUtil::String& imagePath);

		/**
		 * Get the background image path.
		 * @return An image path if the displayed background image was created
		 * using a file path, or an empty string otherwise.
		 */
		MAUtil::String getBackgroundImagePath();

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
		int setPressedImage(MAHandle image);

		/**
		 * Get the pressed image handle.
		 * @return An image handle if the displayed pressed image was created
		 * using a handle, or zero otherwise.
		 */
		MAHandle getPressedImage();

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
		int setPressedImagePath(const MAUtil::String& imagePath);

		/**
		 * Get the pressed image path.
		 * @return An image path if the displayed pressed image was created
		 * using a file path, or an empty string otherwise.
		 */
		MAUtil::String getPressedImagePath();

        /**
         * Add an button event listener.
         * @param listener The listener that will receive button events.
         */
        virtual void addButtonListener(ButtonListener* listener);

        /**
         * Remove the button listener.
         * @param listener The listener that receives button events.
         */
        virtual void removeButtonListener(ButtonListener* listener);

    protected:
        /**
         * This method is called when there is an event for this widget.
         * It passes on the event to all widget's listeners.
         * @param widgetEventData The data for the widget event.
         */
        virtual void handleWidgetEvent(MAWidgetEventData* widgetEventData);

    private:
        /**
         * Array with button listeners.
         */
        MAUtil::Vector<ButtonListener*> mButtonListeners;
    };

} // namespace NativeUI

#endif /* NATIVEUI_IMAGE_BUTTON_H_ */

/*! @} */
