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
 * @file WebView.cpp
 * @author Emma Tresanszki
 *
 * Class for web views.
 */

#include "WebView.h"

#include "WebViewListener.h"

namespace NativeUI
{
	/**
	 * Constructor.
	 */
	WebView::WebView() :
		Widget(MAW_WEB_VIEW)
	{
	}

	/**
	 * Destructor.
	 */
	WebView::~WebView()
	{
        mWebViewListeners.clear();
	}

	/**
	 * Open a web page.
	 * @param url The URL open.
	 */
	void WebView::openURL(const MAUtil::String& url)
	{
		this->setProperty(MAW_WEB_VIEW_URL, url.c_str());
	}

	/*
	 * Get the currently displayed url in the web view.
	 * @return The displayed url.
	 */
	MAUtil::String WebView::getURL()
	{
		return this->getPropertyString(MAW_WEB_VIEW_URL);
	}

	/**
	 * Set the currently displayed HTML data in the web view.
	 * @param html A valid html data.
	 */
	void WebView::setHtml(const MAUtil::String& html)
	{
		this->setProperty(MAW_WEB_VIEW_HTML, html);
	}

	/**
	 * Set the base URL used by the web-view when loading relative paths.
	 * The value of this URL is used when setting the #MAW_WEB_VIEW_URL and
	 * #MAW_WEB_VIEW_HTML properties. The default value for this property
	 * points to the Assets folder in the local file system
	 * ("file://pathToLocalFileSystem/Assets/").
	 *
	 * @note Use: #maGetSystemProperty ("mosync.path.local.url") to find
	 * the baseURL for the local file system.
	 * @param baseUrl Any valid URL schema.
	 *
	 */
	void WebView::setBaseUrl(const MAUtil::String& baseUrl)
	{
		this->setProperty(MAW_WEB_VIEW_BASE_URL, baseUrl);
	}

	/**
	 * Get the base URL used by the web-view when loading relative paths.
	 * The value of this URL is used when setting the #MAW_WEB_VIEW_URL and
	 * #MAW_WEB_VIEW_HTML properties. The default value for this property
	 * points to the Assets folder in the local file system
	 * ("file://pathToLocalFileSystem/Assets/").
	 *
	 * @return The url.
	 *
	 */
	MAUtil::String WebView::getBaseUrl()
	{
		return this->getPropertyString(MAW_WEB_VIEW_BASE_URL);
	}


	/**
	* Set the pattern used to "soft hook" urls, to get notified
	* when pages are being loaded.
	*
	* When this pattern matches a url that is being requested
	* to load in the WebView, event #MAW_EVENT_WEB_VIEW_HOOK_INVOKED
	* is sent, and the page loads NORMALLY.
	*
	* Note that when receiving event #MAW_EVENT_WEB_VIEW_HOOK_INVOKED
	* you MUST deallocate the urlData handle of the event by calling
	* maDestroyObject on the handle. Destroy the handle when you are done
	* reading the url data, to avoid that memory will get used up.
	*
	* The pattern is a url pattern specified using regular expression
	* syntax, according to the ICU regular expression standard, using
	* complete match. For example, the ".*" matching expression is used
	* to match any url string. To match all HTTP requests, use the
	* pattern "http://.*". To match all requests for a specific domain,
	* use a pattern like ".*google.com.*". For further infomation, see:
	* http://userguide.icu-project.org/strings/regexp
	* When typing a C-string with a pattern that contains a matching
	* expression with a backslash, make sure to escape the backslash,
	* for example "\B" should be "\\B".
	*
	* By setting the pattern to an empty string, the soft hook mechanism
	* is turned off, and #MAW_EVENT_WEB_VIEW_HOOK_INVOKED is not sent.
	*
	* Both "soft" hooks and "hard" hooks can be enabled simultaneously,
	* but only one hook pattern can be used for each type of hook. When
	* setting a new hook pattern, the old hook is replaced.
	*
	* Note that urls opened using openURL method are NOT hooked.
	* This way of loading a page is excluded from the hook mechanism to
	* prevent "loops" when loading pages.
	*
	* @param url A string with a url pattern.
	* Example:
	*   // Hook all urls.
	*	setSoftHook(".*");
	*
	*   // Hook no urls.
	*	setSoftHook("");
	*
	*   // Hook urls that start with "mosync:".
	*	setSoftHook("mosync://.*");
	*/
	void WebView::setSoftHook(const MAUtil::String& url)
	{
		this->setProperty(MAW_WEB_VIEW_SOFT_HOOK, url);
	}

	/**
	* Set the pattern used to "hard hook" urls, to get notified
	* and prevent loading of pages.
	*
	* Hard hooks are useful for communicating events from a WebView,
	* for example by setting document.location to a url string in
	* JavaScript. Example: document.location = 'mosync://ExitApp'
	* The application can then examine the url data and take action
	* depending on the url content.
	*
	* When this pattern matches a url that is being requested
	* to load in the WebView, event #MAW_EVENT_WEB_VIEW_HOOK_INVOKED
	* is sent, and page loading is ABORTED.
	*
	* Note that when receiving event #MAW_EVENT_WEB_VIEW_HOOK_INVOKED
	* you MUST deallocate the urlData handle of the event by calling
	* maDestroyObject on the handle. Destory the handle when you are done
	* reading the url data, to avoid that memory will get used up.
	*
	* The pattern is a url pattern specified using regular expression
	* syntax, according to the ICU regular expression standard, using
	* complete match. For example, the ".*" matching expression is used
	* to match any url string. To match all HTTP requests, use the
	* pattern "http://.*". To match all requests for a specific domain,
	* use a pattern like ".*google.com.*". For further infomation, see:
	* http://userguide.icu-project.org/strings/regexp
	* When typing a C-string with a pattern that contains a matching
	* expression with a backslash, make sure to escape the backslash,
	* for example "\B" should be "\\B".
	*
	* By setting the pattern to an empty string, the soft hook mechanism
	* is turned off, and #MAW_EVENT_WEB_VIEW_HOOK_INVOKED is not sent.
	*
	* Both "soft" hooks and "hard" hooks can be enabled simultaneously,
	* but only one hook pattern can be used for each type of hook. When
	* setting a new hook pattern, the old hook is replaced.
	*
	* The "hard" hook property takes precedence over the "soft" hook
	* property. When both properties are set,
	* #MAW_EVENT_WEB_VIEW_HOOK_INVOKED is sent once, for the
	* "hard" hook type.
	*
	* Note that urls opened using openURL method are NOT hooked.
	* This way of loading a page is excluded from the hook mechanism to
	* prevent "loops" when loading pages.
	*
	* @param url A string with a url pattern.
	* Example:
	*   // Hook urls that start with "mosync:".
	*	setHardHook("mosync://.*");
	*/
	void WebView::setHardHook(const MAUtil::String& url)
	{
		this->setProperty(MAW_WEB_VIEW_HARD_HOOK, url);
	}

	/**
	* Enable the zoom controls of the web view.
	*/
	void WebView::enableZoom()
	{
		this->setProperty(MAW_WEB_VIEW_ENABLE_ZOOM, "true");
	}

	/**
	* Disable the zoom controls of the web view.
	*/
	void WebView::disableZoom()
	{
		this->setProperty(MAW_WEB_VIEW_ENABLE_ZOOM, "false");
	}

	/*
	 * Navigate back the browsing history.
	 */
	void WebView::navigateBack()
	{
		this->setProperty(MAW_WEB_VIEW_NAVIGATE, "back");
	}

	/*
	 * Navigate forward the browsing history.
	 */
	void WebView::navigateForward()
	{
		this->setProperty(MAW_WEB_VIEW_NAVIGATE, "forward");
	}

	/**
	 * Set the horizontal scroll bar to be drawn.
	 * Available only on Android for the moment.
	 */
	void WebView::enableHorizontalScrollBar()
	{
		this->setProperty(MAW_WEB_VIEW_HORIZONTAL_SCROLL_BAR_ENABLED, "true");
	}

	/**
	 * Set the horizontal scroll bar not to be drawn.
	 * Available only on Android for the moment.
	 */
	void WebView::disableHorizontalScrollbar()
	{
		this->setProperty(MAW_WEB_VIEW_HORIZONTAL_SCROLL_BAR_ENABLED, "false");
	}

	/*
	 * get whether the horizontal scrollbar should be painted.
	 * @return True if the horizontal scroll bar is enabled, false otherwise.
	 * Available only on Android for the moment.
	 */
	bool WebView::isHorizontalScrollbarEnabled()
	{
        MAUtil::String value = MAUtil::lowerString(
			this->getPropertyString(MAW_WEB_VIEW_HORIZONTAL_SCROLL_BAR_ENABLED));
        if ( strcmp(value.c_str(),"true") == 0 )
        {
            return true;
        }

        return false;
	}

	/*
	 * Set the vertical scroll bar to be painted.
	 * Available only on Android for the moment.
	 */
	void WebView::enableVerticalScrollBar()
	{
		this->setProperty(MAW_WEB_VIEW_VERTICAL_SCROLL_BAR_ENABLED, "true");
	}

	/**
	 * Set the vertical scroll bar not to be painted.
	 * Available only on Android for the moment.
	 */
	void WebView::disableVerticalScrollbar()
	{
		this->setProperty(MAW_WEB_VIEW_VERTICAL_SCROLL_BAR_ENABLED, "false");
	}

	/*
	 * Gets whether the vertical scrollbar should be painted.
	 * @return True if the vertical scroll bar is enabled, false otherwise.
	 * Available only on Android for the moment.
	 */
	bool WebView::isVerticalScrollBarEnabled()
	{
        MAUtil::String value = MAUtil::lowerString(
			this->getPropertyString(MAW_WEB_VIEW_VERTICAL_SCROLL_BAR_ENABLED));
        if ( strcmp(value.c_str(),"true") == 0 )
        {
            return true;
        }

        return false;
	}

    /**
     * Get the new URL whenever the web view changes it.
     * @return The new URL.
     */
    MAUtil::String WebView::getNewURL()
    {
        return this->getPropertyString(MAW_WEB_VIEW_NEW_URL);
    }
    /**
     * Add an web view event listener.
     * @param listener The listener that will receive web view events.
     */
    void WebView::addWebViewListener(WebViewListener* listener)
    {
		addListenerToVector(mWebViewListeners, listener);
    }

    /**
     * Remove the web view listener.
     * @param listener The listener that receives web view events.
     */
    void WebView::removeWebViewListener(WebViewListener* listener)
    {
		removeListenerFromVector(mWebViewListeners, listener);
    }

    /**
     * This method is called when there is an event for this widget.
     * It passes on the event to all widget's listeners.
     * @param widgetEventData The data for the widget event.
     */
    void WebView::handleWidgetEvent(MAWidgetEventData* widgetEventData)
    {
        Widget::handleWidgetEvent(widgetEventData);

        if ( MAW_EVENT_WEB_VIEW_CONTENT_LOADING == widgetEventData->eventType)
        {
            for (int i = 0; i < mWebViewListeners.size(); i++)
            {
                mWebViewListeners[i]->webViewContentLoading(
                    this,
                    widgetEventData->status);
            }
        }
        else if (MAW_EVENT_WEB_VIEW_HOOK_INVOKED == widgetEventData->eventType)
        {
		int hookType = widgetEventData->hookType;
			MAHandle url = widgetEventData->urlData;

            for (int i = 0; i < mWebViewListeners.size(); i++)
            {
                mWebViewListeners[i]->webViewHookInvoked(
					this,
					hookType,
					url);
            }
            maDestroyObject(url);
        }
    }

} // namespace NativeUI
