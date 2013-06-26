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
 * @file Widget.cpp
 * @author Mikael Kindborg, Emma Tresanszki and Bogdan Iusco.
 *
 * Widget is the base class of all widgets. This class is
 * used to wrap native widget handles and provides common
 * methods for widgets and event support.
 */

#include <mavsprintf.h>
#include <mastdlib.h>
#include <madmath.h>

#include "Widget.h"
#include "WidgetManager.h"
#include "WidgetEventListener.h"

namespace NativeUI
{

    /**
     * Constructor is protected because actual widget instances
     * should be subclasses of this class.
     * @widgetType The string constant that identifies the widget type
     * (one of the MAW_ constants).
     */
    Widget::Widget(const MAUtil::String& widgetType) :
        mWidgetManager(WidgetManager::getInstance()),
        mLastError(LastError()),
		mData(NULL)
    {
        mWidgetHandle = maWidgetCreate(widgetType.c_str());
        if ( mWidgetHandle == MAW_RES_INVALID_TYPE_NAME )
        {
			setLastErrorCode(mWidgetHandle, "maWidgetCreate");
        }
        mWidgetManager->registerWidget(mWidgetHandle, this);
    }

    /**
     * Destructor.
     */
    Widget::~Widget()
    {
        mWidgetManager->unregisterWidget(mWidgetHandle);
        mWidgetEventListeners.clear();
        MAWidgetHandle widgetHandle = mWidgetHandle;

        markWidgetHandleAsDeleted();

        int countChildren = mChildren.size();
        for (int i = 0; i < countChildren; i++)
        {
            delete mChildren[i];
        }

        if (NULL != widgetHandle)
        {
            maWidgetDestroy(widgetHandle);
        }
    }

    /**
     * Set my widget handle and my chilren's widget
     * handles to NULL to mark that it has been deleted.
     */
    void Widget::markWidgetHandleAsDeleted()
    {
        mWidgetHandle = NULL;

        for (int i = 0; i < mChildren.size(); ++i)
        {
            mChildren[i]->markWidgetHandleAsDeleted();
        }
    }

    /**
     * @return The handle of the widget.
     */
    MAWidgetHandle Widget::getWidgetHandle() const
    {
        return mWidgetHandle;
    }

    /**
     * Set a widget string property.
     * @param property A string representing which property to set.
     * @param value The string value which will be assigned to the property.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
     * - #MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
     * - #MAW_RES_ERROR otherwise.
     */
    int Widget::setProperty(
        const MAUtil::String& property,
        const MAUtil::String& value)
    {
		int resultCode =
			maWidgetSetProperty(
            mWidgetHandle,
            property.c_str(),
            value.c_str());

		if ( resultCode < 0 )
		{
			setLastErrorCode(resultCode, property);
		}
		return resultCode;
    }

    /**
     * Set a widget integer property.
     * @param property A string representing which property to set.
     * @param value The integer value which will be assigned to the property.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
     * - #MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
     * - #MAW_RES_ERROR otherwise.
     */
    int Widget::setPropertyInt(
        const MAUtil::String& property,
        const int value)
    {
        char buffer[BUF_SIZE];
        sprintf(buffer, "%d", value);
        int errCode =
			maWidgetSetProperty(mWidgetHandle, property.c_str(), buffer);

        if ( errCode != MAW_RES_OK )
        {
			setLastErrorCode(errCode, property);
        }
        return errCode;
    }

    /**
     * Set a widget float property.
     * @param property A string representing which property to set.
     * @param value The float value which will be assigned to the property.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the property could be set.
     * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
     * - #MAW_RES_INVALID_PROPERTY_NAME if the property name was invalid.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
     * - #MAW_RES_ERROR otherwise.
     */
    int Widget::setPropertyFloat(
        const MAUtil::String& property, float value)
    {
        char buffer[BUF_SIZE];
        sprintf(buffer, "%f", value);
        int errCode =
			maWidgetSetProperty(mWidgetHandle, property.c_str(), buffer);

        if ( errCode != MAW_RES_OK )
        {
			setLastErrorCode(errCode, property);
        }
        return errCode;
    }

    /**
     * Get a widget property value as an integer, setting also the result code.
     * @param property A string representing which property to set.
     * @param resultCode Will contain the result code of the syscall.
     *                   Can be any of the following result codes:
     *                   - #MAW_RES_OK.
     *                   - #MAW_RES_INVALID_HANDLE if the handle was invalid.
     *                   - #MAW_RES_INVALID_PROPERTY_NAME if the property
     *                     name was invalid.
     *                   - #MAW_RES_INVALID_PROPERTY_VALUE if the property
     *                     value was invalid.
     *                   - #MAW_RES_INVALID_STRING_BUFFER_SIZE if the buffer
     *                     size was too small.
     *                   - #MAW_RES_ERROR otherwise.
     * @return The property value.
     */
    int Widget::getPropertyInt(
        const MAUtil::String& property, int& resultCode)
    {
        char buffer[BUF_SIZE];
        resultCode = maWidgetGetProperty(
            mWidgetHandle,
            property.c_str(),
            buffer,
            BUF_SIZE);

        if ( resultCode >= 0 )
        {
            resultCode = MAW_RES_OK;
        }
        else
        {
			setLastErrorCode(resultCode, property);
        }
        return atoi(buffer);
    }

    /**
     * Get a widget property value as an integer.
     * @param property A string representing which property to get.
     * @return The property value.
     */
    int Widget::getPropertyInt(
        const MAUtil::String& property)
    {
        char buffer[BUF_SIZE];
        int resultCode =
			maWidgetGetProperty(
			mWidgetHandle,
			property.c_str(),
			buffer, BUF_SIZE);

        if ( resultCode < 0 )
        {
			setLastErrorCode(resultCode, property);
        }

        return atoi(buffer);
    }

    /**
     * Get a widget property value as a float, setting also the result code.
     * @param property A string representing which property to set.
     * @param resultCode Will contain the result code of the syscall.
     *                   Can be any of the following result codes:
     *                   - #MAW_RES_OK.
     *                   - #MAW_RES_INVALID_HANDLE if the handle was invalid.
     *                   - #MAW_RES_INVALID_PROPERTY_NAME if the property
     *                     name was invalid.
     *                   - #MAW_RES_INVALID_PROPERTY_VALUE if the property
     *                     value was invalid.
     *                   - #MAW_RES_INVALID_STRING_BUFFER_SIZE if the buffer
     *                     size was too small.
     *                   - #MAW_RES_ERROR otherwise.
     * @return The property value.
     */
    float Widget::getPropertyFloat(
        const MAUtil::String& property, int& resultCode)
    {
        char buffer[BUF_SIZE];
        resultCode = maWidgetGetProperty(
            mWidgetHandle,
            property.c_str(),
            buffer,
            BUF_SIZE);

        if ( resultCode >= 0 )
        {
            resultCode = MAW_RES_OK;
        }
        else
        {
			setLastErrorCode(resultCode, property);
        }

        double value = atof(buffer);
        return (float) value;
    }

    /**
     * Get a widget property value as a float.
     * @param property A string representing which property to get.
     * @return The property value.
     */
    float Widget::getPropertyFloat(
        const MAUtil::String& property)
    {
        MAUtil::String propertyValue = getPropertyString(property);
        double value = atof(propertyValue.c_str());
        return (float) value;
    }

    /**
     * Get a widget property as a string, setting also the result code.
     * @param property A string representing which property to set.
     * @param resultCode Will contain the result code of the syscall.
     *                   Can be any of the following result codes:
     *                   - #MAW_RES_OK no error occurred.
     *                   - #MAW_RES_INVALID_HANDLE if the handle was invalid.
     *                   - #MAW_RES_INVALID_PROPERTY_NAME if the property
     *                     name was invalid.
     *                   - #MAW_RES_INVALID_PROPERTY_VALUE if the property
     *                     value was invalid.
     *                   - #MAW_RES_INVALID_STRING_BUFFER_SIZE if the buffer
     *                     size was too small.
     *                   - #MAW_RES_ERROR otherwise.
     * @return The property value.
     */
    MAUtil::String Widget::getPropertyString(
        const MAUtil::String& property,
        int& resultCode)
    {
        char buffer[BUF_SIZE];
        resultCode = maWidgetGetProperty(
            mWidgetHandle,
            property.c_str(),
            buffer,
            BUF_SIZE);

        if ( resultCode >= 0 )
        {
            resultCode = MAW_RES_OK;
        }
        else
        {
			setLastErrorCode(resultCode, property);
        }

        return buffer;
    }

    /**
     * Get a widget property as a string.
     * @param property A string representing which property to set.
     * @return The property value.
     */
    MAUtil::String Widget::getPropertyString(
        const MAUtil::String& property)
    {
        int resultCode;
        return getPropertyString(property,resultCode);
    }

    /**
     * Add a widget as a child of this widget.
     * @param widget The widget that will be added.
     * The ownership of the widget is passed to this function.
     * When the parent widget will be destroyed, all child widgets
     * will be deleted.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the child could be added to the parent.
     * - #MAW_RES_INVALID_HANDLE if any of the handles were invalid.
     * - #MAW_RES_INVALID_LAYOUT if the widget was added to a non-layout.
     * - #MAW_RES_ERROR if it could not be added for some other reason.
     */
    int Widget::addChild(Widget* widget)
    {
		if (widget == NULL)
		{
			return MAW_RES_ERROR;
		}

        // Add the widget object to the list of children.
        mChildren.add(widget);

        // Add the widget handle as a child, this will add
        // the native widget as a child.
        int resultCode =
			maWidgetAddChild(mWidgetHandle, widget->getWidgetHandle());

        if ( resultCode < 0 )
        {
			setLastErrorCode(resultCode, "addChild");
        }
        return resultCode;
    }

    /**
     * Insert a widget as a child of this widget, at a given position.
     * @param widget The widget to be added.
     * The ownership of the widget is passed to this function.
     * When the parent widget will be destroyed, all child widgets
     * will be deleted.
     * @param index The position on which to add the child.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the child could be added to the parent.
     * - #MAW_RES_INVALID_HANDLE if any of the handles were invalid.
     * - #MAW_RES_INVALID_INDEX if the index was out of bounds.
     * - #MAW_RES_INVALID_LAYOUT if the widget was added to a non-layout.
     * - #MAW_RES_ERROR if it could not be added for some other reason.
     */
    int Widget::insertChild(Widget* widget,const int index)
    {
		if (widget == NULL)
		{
			return MAW_RES_ERROR;
		}

		if (index < 0 || index > mChildren.size())
		{
			return MAW_RES_INVALID_INDEX;
		}

        mChildren.insert(index, widget);

        // Add the widget handle as a child, this will add
        // the native widget as a child.
        int resultCode =
			maWidgetInsertChild(
            mWidgetHandle,
            widget->getWidgetHandle(),
            index);

        if ( resultCode < 0 )
        {
			setLastErrorCode(resultCode, "insertChild");
        }
        return resultCode;
    }

    /**
     * Remove a child widget from its parent (but does not destroy it).
     * Removing a currently visible top-level widget causes the MoSync view
     * to become visible.
     * When the parent widget will be destroyed, the removed child widget will not
     * be deleted (this responsibility is left to the developer).
     * @param widget The widget to be removed.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the child could be removed from the parent.
     * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
     * - #MAW_RES_ERROR otherwise.
     */
    int Widget::removeChild(Widget* widget)
    {
		if (widget == NULL)
		{
			return MAW_RES_ERROR;
		}

        for (int i = 0; i< mChildren.size(); i++)
        {
            if (widget == mChildren[i])
            {
                mChildren.remove(i);
                break;
            }
        }
        int resultCode = maWidgetRemoveChild(widget->getWidgetHandle());

        if ( resultCode < 0 )
        {
			setLastErrorCode(resultCode, "removeChild");
        }
        return resultCode;
    }

    /**
     * Get a child widget of this widget.
     * @param index The index of the child.
     * @return The child widget at the given index.
     * The ownership of the result is not passed to the caller.
     */
    Widget* Widget::getChild(const int index) const
    {
        if (0 > index || index >= mChildren.size())
        {
            return NULL;
        }

        return mChildren[index];
    }

    /**
     * Get the number of children widgets.
     * @return The number of children widgets.
     */
    int Widget::countChildWidgets() const
    {
        return mChildren.size();
    }

    /**
     * Set the left/top position of the widget.
     * @param left The left coordinate.
     * @param top The top coordinate.
     */
    void Widget::setPosition(const int left, const int top)
    {
        setPropertyInt(MAW_WIDGET_LEFT, left);
        setPropertyInt(MAW_WIDGET_TOP, top);
    }

    /**
     * Set the left position of the widget.
     * @param left The left coordinate.
     */
     void Widget::setLeftPosition(const int left)
     {
        setPropertyInt(MAW_WIDGET_LEFT, left);
     }

    /**
     * Set the top position of the widget.
     * @param top The top coordinate.
     */
    void Widget::setTopPosition(const int top)
    {
        setPropertyInt(MAW_WIDGET_TOP, top);
    }

    /**
     * Set the size of the widget.
     * @param width The width value.
     * @param height The height value.
     */
    void Widget::setSize(const int width,const int height)
    {
        setPropertyInt(MAW_WIDGET_WIDTH, width);
        setPropertyInt(MAW_WIDGET_HEIGHT, height);
    }

    /**
     * Get the width of the widget.
     * @return The width value.
     */
    int Widget::getWidth()
    {
		return getPropertyInt(MAW_WIDGET_WIDTH);
    }

    /**
     * Get the height of the widget.
     * @return The height value.
     */
    int Widget::getHeight()
    {
		return getPropertyInt(MAW_WIDGET_HEIGHT);
    }

    /**
     * Set the width of the widget.
     * @param width The width value.
     */
    void Widget::setWidth(const int width)
    {
        setPropertyInt(MAW_WIDGET_WIDTH, width);
    }

    /**
     * Set the height of the widget.
     *  @param height The height value.
     */
    void Widget::setHeight(const int height)
    {
        setPropertyInt(MAW_WIDGET_HEIGHT, height);
    }

    /**
     * Size the width of this widget to fill the
     * available space in the parent widget horizontally.
     */
    void Widget::fillSpaceHorizontally()
    {
        setPropertyInt(MAW_WIDGET_WIDTH, MAW_CONSTANT_FILL_AVAILABLE_SPACE);
    }

    /**
     * Size the height of this widget to fill the
     * available space in the parent widget vertically.
     */
    void Widget::fillSpaceVertically()
    {
        setPropertyInt(MAW_WIDGET_HEIGHT, MAW_CONSTANT_FILL_AVAILABLE_SPACE);
    }

    /**
     * Size the width of this widget to wrap the
     * content horizontally.
     */
    void Widget::wrapContentHorizontally()
    {
        setPropertyInt(MAW_WIDGET_WIDTH, MAW_CONSTANT_WRAP_CONTENT);
    }

    /**
     * Size the height of this widget to wrap the
     * content vertically.
     */
    void Widget::wrapContentVertically()
    {
        setPropertyInt(MAW_WIDGET_HEIGHT, MAW_CONSTANT_WRAP_CONTENT);
    }

    /**
     * Set the background color of the widget.
     * @param color A hexadecimal color value, e.g. 0xFF0000.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the value was set.
     * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
     * - #MAW_RES_ERROR otherwise.
     */
    int Widget::setBackgroundColor(const int color)
    {
        char buffer[BUF_SIZE];
        sprintf(buffer, "0x%.6X", color);
        return setProperty(MAW_WIDGET_BACKGROUND_COLOR, buffer);
    }

    /**
     * Set the background color of the widget.
     * @param red Red component (range 0-255).
     * @param green Green component (range 0-255).
     * @param blue Blue component (range 0-255).
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the value was set.
     * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
     * - #MAW_RES_ERROR otherwise.
     */
    int Widget::setBackgroundColor(
        const int red, const int green, const int blue)
    {
        char buffer[BUF_SIZE];
        sprintf(buffer, "0x%.2X%.2X%.2X", red, green, blue);
        return setProperty(MAW_WIDGET_BACKGROUND_COLOR, buffer);
    }

    /**
     * Set the background gradient of the widget.
     * This is available only for Android.
     * @param firstColor First color in hexadecimal, e.g. 0xFF0000.
     * @param secondColor Second color in hexadecimal, e.g. 0x0000FF.
     * @return Any of the following result codes:
     * - #MAW_RES_OK if the value was set.
     * - #MAW_RES_INVALID_HANDLE if the handle was invalid.
     * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
     * - #MAW_RES_ERROR otherwise.
     */
    int Widget::setBackgroundGradient(
        const int firstColor,
        const int secondColor)
    {
        char bufferColor1[BUF_SIZE];
        char bufferColor2[BUF_SIZE];
        sprintf(bufferColor1, "0x%.6X", firstColor);
        sprintf(bufferColor2, "0x%.6X", secondColor);

        MAUtil::String gradient ="";
        gradient += bufferColor1;
        gradient += ",";
        gradient += bufferColor2;

        return setProperty(MAW_WIDGET_BACKGROUND_GRADIENT, gradient);
    }

    /**
     * Show or hide the widget.
     * @param visible If true shows the widget, otherwise hides it.
     */
    void Widget::setVisible(const bool visible)
    {
		setProperty(MAW_WIDGET_VISIBLE, (visible ? "true" : "false") );
    }

    /**
     * Check if the widget is visible.
     * @return True if is visible, false otherwise.
     */
    bool Widget::isVisible()
    {
        MAUtil::String value = MAUtil::lowerString(
			this->getPropertyString(MAW_WIDGET_VISIBLE));
        if ( strcmp(value.c_str(),"true") == 0 )
        {
            return true;
        }

        return false;
    }

    /**
     * Enable the widget.
     * @param enable If true enables the widget, otherwise disables it.
     */
    void Widget::setEnabled(bool enable)
    {
        setProperty(MAW_WIDGET_ENABLED, (enable ? "true" : "false") );
    }

    /**
     * Check if the widget is enabled.
     * @return True if enabled, false otherwise.
     */
    bool Widget::isEnabled()
    {
        MAUtil::String value = MAUtil::lowerString(
			this->getPropertyString(MAW_WIDGET_ENABLED));
        if ( strcmp(value.c_str(),"true") == 0 )
        {
            return true;
        }

        return false;
    }

    /**
     * Set widget's alpha value.
     * @value Floating-point number in the range 0.0 to 1.0, where 0.0
     * represents totally transparent and 1.0 represents totally opaque.
     * If the value is out of this range the alpha value is not set.
     */
    void Widget::setAlpha(const float value)
    {
        if (value >=  0.0 && value <= 1.0)
        {
            this->setPropertyFloat(MAW_WIDGET_ALPHA, value);
        }
    }

    /**
     * Get widget's alpha value.
     * @return Floating-point number in the range 0.0 to 1.0, where 0.0
     * represents totally transparent and 1.0 represents totally opaque.
     */
    float Widget::getAlpha()
    {
         MAUtil::String alphaString = this->getPropertyString(MAW_WIDGET_ALPHA);
         double alpha = atof(alphaString.c_str());
         return (float) alpha;
    }

    /**
     * Add an event listener for this widget.
     * @param listener The listener that will receive
     * widget events for this widget.
     */
    void Widget::addEventListener(WidgetEventListener* listener)
    {
        addListenerToVector(mWidgetEventListeners, listener);
    }

    /**
     * Remove the event listener for this widget.
     * @param listener The listener that receives widget events
     * for this widget.
     */
    void Widget::removeEventListener(WidgetEventListener* listener)
    {
       removeListenerFromVector(mWidgetEventListeners, listener);
    }

    /**
     * This method is called when there is an event for this widget.
     * It passes on the event to all widget's listeners.
     * Note: You can either use an event listener or override this
     * method in a subclass to handle events.
     * @param widgetEventData The data for the widget event.
     */
    void Widget::handleWidgetEvent(MAWidgetEventData* widgetEventData)
    {
        for (int i = 0; i < mWidgetEventListeners.size(); i++)
        {
            mWidgetEventListeners[i]->handleWidgetEvent(this, widgetEventData);
        }
    }

    /**
     * Get a handle to the last error that occurred.
     * @return a LastError structure that holds details about the error.
     */
    LastError Widget::getLastError()
    {
		return mLastError;
    }

    /**
     * Save the last error information.
     * @param errCode The error code.
     * @param cause The method/property that caused the error.
     */
    void Widget::setLastErrorCode(int& errCode, const MAUtil::String& cause)
    {
		mLastError.errorCode = errCode;
		mLastError.errorCause = cause;
    }

	/**
	 * Set a pointer to any data you want to associate the widget to.
	 */
	void Widget::setData(void *data)
	{
		mData = data;
	}

	/**
	 * Get a pointer to the data, the widget is associated to. (default: NULL)
	 */
	void* Widget::getData()
	{
		return mData;
	}

} // namespace NativeUI
