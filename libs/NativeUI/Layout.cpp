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
 * @file Layout.cpp
 * @author Emma Tresanszki
 *
 * Class for layouts.
 */

#include "Layout.h"
#include "WidgetUtil.h"

namespace NativeUI
{

    /**
     * Constructor is protected because actual widget instances
     * should be subclasses of this class.
     * @param layoutType The string constant that identifies the widget type
     *             (one of the MAW_ constants).
     */
    Layout::Layout(const MAUtil::String& layoutType) :
        Widget(layoutType)
    {
    }

    /**
     * Destructor.
     */
    Layout::~Layout()
     {
        for (int i = 0; i < mBanner.size(); ++i)
        {
            delete mBanner[i];
        }

        mBanner.clear();
     }

	/**
	 * Sets whether the layout can be scrollable or not.
	 * Note: make sure you call this after you add all the children to it,
	 * otherwise it would have no effect.
	 * @param isScrollable If true, the layout can be scrollable.
	 */
    void Layout::setScrollable(const bool isScrollable)
    {
		setProperty("isScrollable", (isScrollable ? "true" : "false") );
    }

	/**
	 * Set the left padding.
	 *
	 * @param value The left padding in pixels.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the property could be set.
	 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
	 * - #MAW_RES_ERROR otherwise.
	 */
	int Layout::setPaddingLeft( const int value)
	{
		return this->setPropertyInt(MAW_HORIZONTAL_LAYOUT_PADDING_LEFT, value);
	}

	/**
	 * Set the top padding.
	 *
	 * @param value The top padding in pixels.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the property could be set.
	 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
	 * - #MAW_RES_ERROR otherwise.
	 */
	int Layout::setPaddingTop( const int value)
	{
		return this->setPropertyInt(MAW_HORIZONTAL_LAYOUT_PADDING_TOP, value);
	}

	/**
	 * Set the right padding.
	 *
	 * @param value The right padding in pixels.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the property could be set.
	 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
	 * - #MAW_RES_ERROR otherwise.
	 */
	int Layout::setPaddingRight( const int value)
	{
		return this->setPropertyInt(MAW_HORIZONTAL_LAYOUT_PADDING_RIGHT, value);
	}

	/**
	 * Set the bottom padding.
	 *
	 * @param value The bottom padding in pixels.
	 * @return Any of the following result codes:
	 * - #MAW_RES_OK if the property could be set.
	 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
	 * - #MAW_RES_ERROR otherwise.
	 */
	int Layout::setPaddingBottom( const int value)
	{
		return this->setPropertyInt(MAW_HORIZONTAL_LAYOUT_PADDING_BOTTOM, value);
	}

	/**
	 * Ads a banner as a child of this widget.
	 * @param banner The banner tat will be added.
	 */
	void Layout::addBanner(Ads::Banner* banner)
	{
		mBanner.add(banner);
		maAdsAddBannerToLayout(
			banner->getHandle(),
			this->getWidgetHandle());
	}

	/**
	 * Remove a child banner from its parent(but does not destroy it).
	 * @param banner The banner to be removed.
	 */
	void Layout::removeBanner(Ads::Banner* banner)
	{
        for (int i = 0; i< mBanner.size(); i++)
        {
            if (banner == mBanner[i])
            {
                mBanner.remove(i);
                break;
            }
        }

        maAdsRemoveBannerFromLayout(
            banner->getHandle(),
            this->getWidgetHandle());
	}

} // namespace NativeUI
