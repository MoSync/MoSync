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
 * @file Layout.h
 * @author Emma Tresanszki
 *
 * \brief Class for layouts.
 */

#ifndef NATIVEUI_LAYOUT_H_
#define NATIVEUI_LAYOUT_H_

#include <Ads/Banner.h>

#include "Widget.h"

namespace NativeUI
{
	/**
	 * \brief Class for layouts.
	 */
	class Layout : public Widget
	{
	public:
		/**
		 * Destructor.
		 */
		virtual ~Layout();

		/**
		 * Sets whether the layout can be scrollable or not.
		 * Note: make sure you call this after you add all the children to it;
		 *       it works only for vertical scrolling;
		 * otherwise it would have no effect.
		 * @param isScrollable If true, the layout can be scrollable.
		 */
		virtual void setScrollable(const bool isScrollable);

		/**
		 * Set the left padding.
		 *
		 * @param value The left padding in pixels.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the property could be set.
		 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
		 * - #MAW_RES_ERROR otherwise.
		 */
		virtual int setPaddingLeft( const int value);

		/**
		 * Set the top padding.
		 *
		 * @param value The top padding in pixels.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the property could be set.
		 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
		 * - #MAW_RES_ERROR otherwise.
		 */
		virtual int setPaddingTop( const int value);

		/**
		 * Set the right padding.
		 *
		 * @param value The right padding in pixels.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the property could be set.
		 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
		 * - #MAW_RES_ERROR otherwise.
		 */
		virtual int setPaddingRight( const int value);

		/**
		 * Set the bottom padding.
		 *
		 * @param value The bottom padding in pixels.
		 * @return Any of the following result codes:
		 * - #MAW_RES_OK if the property could be set.
		 * - #MAW_RES_INVALID_PROPERTY_VALUE if the property value was invalid.
		 * - #MAW_RES_ERROR otherwise.
		 */
		virtual int setPaddingBottom( const int value);

		/**
		 * Ads a banner as a child of this widget.
		 * @param banner The banner tat will be added.
		 */
		virtual void addBanner(Ads::Banner* banner);

		/**
		 * Remove a child banner from its parent(but does not destroy it).
		 * @param banner The banner to be removed.
		 */
		virtual void removeBanner(Ads::Banner* banner);

    protected:
        /**
         * Constructor is protected because actual widget instances
         * should be subclasses of this class.
         * @param layoutType The string constant that identifies the widget type
         *             (one of the MAW_ constants).
         */
        Layout(const MAUtil::String& layoutType);

    protected:
        /**
         * List of child banners.
         */
        MAUtil::Vector<Ads::Banner*> mBanner;
	};

} // namespace NativeUI

#endif

/*! @} */
