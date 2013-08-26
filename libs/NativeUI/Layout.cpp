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
		setProperty(MAW_VERTICAL_LAYOUT_SCROLLABLE, (isScrollable ? "true" : "false") );
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
