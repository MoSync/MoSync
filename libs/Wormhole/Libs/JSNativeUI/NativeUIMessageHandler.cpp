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
 * @file NativeUIMessageHandler.cpp
 * @author Ali Sarrafi
 *
 * Implementation of PhoneGap calls made from JavaScript.
 */

#include <mastdlib.h> // C string conversion functions
#include <conprint.h>
#include <MAUtil/String.h>
#include "NativeUIMessageHandler.h"

// NameSpaces we want to access.
using namespace MAUtil; // Class Moblet, String
using namespace NativeUI; // WebView widget

namespace Wormhole
{
	/**
	 * Constructor.
	 */
	NativeUIMessageHandler::NativeUIMessageHandler(NativeUI::WebView* webView) :
		mWebView(webView)
	{
		//We have added this class as a custom event listener so it
		//can forward all of the custom events to JavaScript
		Environment::getEnvironment().addCustomEventListener(this);
	}

	/**
	 * Destructor.
	 */
	NativeUIMessageHandler::~NativeUIMessageHandler()
	{
		// Nothing needs to be explicitly destroyed.
	}

	/**
	 * Implementation of standard API exposed to JavaScript
	 * This function is used to detect different messages from JavaScript
	 * and call the respective function in MoSync.
	 *
	 * @return true if stream was handled, false if not.
	 */
	bool NativeUIMessageHandler::handleMessage(Wormhole::MessageStream& stream)
	{
		char buffer[1024];
		char * action = (char*)stream.getNext();
		// Widget Handling Calls
		if(0 == strcmp("maWidgetCreate", action))
		{
			char* widgetType = (char*)stream.getNext();
			char* widgetID = (char*)stream.getNext();
			char* callbackID = (char*)stream.getNext();
			int numParams = stringToInteger(stream.getNext());

			MAWidgetHandle widget = maWidgetCreate(widgetType);
			if(widget <= 0)
			{
				sprintf(buffer,"'%s', %d", callbackID, widget);
				sendNativeUIError(buffer);
			}
			else
			{
				if(numParams > 0)
				{
					for(int i = 0; i < numParams/2; i++)
					{
						char* property = (char*)stream.getNext();
						char* value = (char*)stream.getNext();
						maWidgetSetProperty(widget, property, value);
					}
				}
				//We use a special callback for widget creation
				sprintf(
					buffer,
					"mosync.nativeui.createCallback('%s', '%s', %d)",
					callbackID,
					widgetID,
					widget);
				mWebView->callJS(buffer);
			}

		}
		else if(0 == strcmp("maWidgetDestroy", action))
		{
			MAWidgetHandle widget = stringToInteger(stream.getNext());
			char* callbackID = (char*)stream.getNext();

			int res = maWidgetDestroy(widget);
			if(res < 0)
			{
				sprintf(buffer,"'%s', %d", callbackID, res);
				sendNativeUIError(buffer);
			}
			else
			{
				sprintf(buffer,"'%s', %d", callbackID, res);
				sendNativeUISuccess(buffer);
			}
		}
		else if(0 == strcmp("maWidgetAddChild", action) )
		{
			MAWidgetHandle parent = stringToInteger(stream.getNext());
			MAWidgetHandle child = stringToInteger(stream.getNext());
			char* callbackID = (char*)stream.getNext();
			int res = maWidgetAddChild(parent, child);
			if(res < 0)
			{
				sprintf(buffer,"'%s', %d", callbackID, res);
				sendNativeUIError(buffer);
			}
			else
			{
				sprintf(buffer,"'%s', %d", callbackID, res);
				sendNativeUISuccess(buffer);
			}
		}
		else if(0 == strcmp("maWidgetInsertChild", action))
		{
			MAWidgetHandle parent = stringToInteger((char*)stream.getNext());
			MAWidgetHandle child = stringToInteger((char*)stream.getNext());
			int index = stringToInteger((char*)stream.getNext());
			char* callbackID = (char*)stream.getNext();
			int res = maWidgetInsertChild(parent, child, index);
			if(res < 0)
			{
				sprintf(buffer,"'%s', %d", callbackID, res);
				sendNativeUIError(buffer);
			}
			else
			{
				sprintf(buffer,"'%s', %d", callbackID, res);
				sendNativeUISuccess(buffer);
			}
		}
		else if(0 == strcmp("maWidgetRemoveChild", action))
		{
			MAWidgetHandle child = stringToInteger(stream.getNext());
			char* callbackID = (char*)stream.getNext();

			int res = maWidgetRemoveChild(child);
			if(res < 0)
			{
				sprintf(buffer,"'%s', %d", callbackID, res);
				sendNativeUIError(buffer);
			}
			else
			{
				sprintf(buffer,"'%s', %d", callbackID, res);
				sendNativeUISuccess(buffer);
			}
		}
		else if(0 == strcmp("maWidgetModalDialogShow", action))
		{
			MAWidgetHandle dialogHandle =
				stringToInteger(stream.getNext());
			char* callbackID = (char*)stream.getNext();
			int res = maWidgetModalDialogShow(dialogHandle);
			if(res < 0)
			{
				sprintf(buffer,"'%s', %d", callbackID, res);
				sendNativeUIError(buffer);
			}
			else
			{
				sprintf(buffer,"'%s', %d", callbackID, res);
				sendNativeUISuccess(buffer);
			}
		}
		else if(0 == strcmp("maWidgetModalDialogHide", action))
		{
			MAWidgetHandle dialogHandle =
					stringToInteger(stream.getNext());
			char* callbackID = (char*)stream.getNext();
			int res = maWidgetModalDialogHide(dialogHandle);
			if(res < 0)
			{
				sprintf(buffer,"'%s', %d", callbackID, res);
				sendNativeUIError(buffer);
			}
			else
			{
				sprintf(buffer,"'%s', %d", callbackID, res);
				sendNativeUISuccess(buffer);
			}
		}
		else if(0 == strcmp("maWidgetScreenShow", action))
		{
			MAWidgetHandle screenHandle =
					stringToInteger(stream.getNext());
			char* callbackID = (char*)stream.getNext();
			int res = maWidgetScreenShow(screenHandle);
			if(res < 0)
			{
				sprintf(buffer,"'%s', %d", callbackID, res);
				sendNativeUIError(buffer);
			}
			else
			{
				sprintf(buffer,"'%s', %d", callbackID, res);
				sendNativeUISuccess(buffer);
			}
		}
		else if(0 == strcmp("maWidgetStackScreenPush", action))
		{
			MAWidgetHandle stackScreen =
					stringToInteger(stream.getNext());
			MAWidgetHandle newScreen =
					stringToInteger(stream.getNext());
			char* callbackID = (char*)stream.getNext();
			int res = maWidgetStackScreenPush(stackScreen, newScreen);
			if(res < 0)
			{
				sprintf(buffer,"'%s', %d", callbackID, res);
				sendNativeUIError(buffer);
			}
			else
			{
				sprintf(buffer,"'%s', %d", callbackID, res);
				sendNativeUISuccess(buffer);
			}
		}
		else if(0 == strcmp("maWidgetStackScreenPop", action))
		{
			MAWidgetHandle stackScreen =
					stringToInteger(stream.getNext());
			char* callbackID = (char*)stream.getNext();
			int res = maWidgetStackScreenPop(stackScreen);
			if(res < 0)
			{
				sprintf(buffer,"'%s', %d", callbackID, res);
				sendNativeUIError(buffer);
			}
			else
			{
				sprintf(buffer,"'%s', %d", callbackID, res);
				sendNativeUISuccess(buffer);
			}
		}
		else if(0 == strcmp("maWidgetSetProperty", action))
		{
			MAWidgetHandle widget =
					stringToInteger(stream.getNext());
			char *property = (char*)stream.getNext();
			char *value = (char*)stream.getNext();
			char* callbackID = (char*)stream.getNext();
			int res = maWidgetSetProperty(widget, property, value);
			if(res < 0)
			{
				sprintf(buffer,"'%s', %d", callbackID, res);
				sendNativeUIError(buffer);
			}
			else
			{
				sprintf(buffer,"'%s', %d", callbackID, res);
				sendNativeUISuccess(buffer);
			}
		}
		else if(0 == strcmp("maWidgetGetProperty", action))
		{
			char value[1024];
			MAWidgetHandle widget =
					stringToInteger(stream.getNext());
			char* property = (char*)stream.getNext();
			char* callbackID = (char*)stream.getNext();

			int res = maWidgetGetProperty(widget, property, value, 1024);
			if(res < 0)
			{
				sprintf(buffer,"'%s', %d", callbackID, res);
				sendNativeUIError(buffer);
			}
			else
			{
				sprintf(buffer,"'%s', '%s', '%s'", callbackID, property, value);
				sendNativeUISuccess(buffer);
			}
		}

		// Tell the WebView that we have processed the stream, so that
		// it can send the next one.

		char replyScript[1024];
		char * mosyncCallBackId = (char*)stream.getNext();
		if(mosyncCallBackId != NULL)
		{
			sprintf(
					replyScript,
					"mosync.bridge.reply(%s)",
					mosyncCallBackId);
			mWebView->callJS(replyScript);
		}

		return true;
	}

	/**
	 * Handles custom events generated by NativeUI Widgets.
	 */
	void NativeUIMessageHandler::customEvent(const MAEvent& event)
	{
		char buffer[1024];

		if(event.type == EVENT_TYPE_WIDGET)
		{
			MAWidgetEventData *data = (MAWidgetEventData*)event.data;
			MAWidgetHandle widget = data->widgetHandle;

			if((widget == mWebView->getWidgetHandle()))
			{
				return;
			}
			int firstParameter = data->dayOfMonth;
			int secondParameter = data->month;
			int thirdParameter = data->year;

			String eventType;
			// Translate the event type to JavaScript eventTypes
			switch(data->eventType)
			{
				case MAW_EVENT_POINTER_PRESSED:
					eventType = "PointerPressed";
					break;
				case MAW_EVENT_POINTER_RELEASED:
					eventType = "PointerReleased";
					break;
				case MAW_EVENT_CONTENT_LOADED:
					eventType = "ContentLoaded";
					break;
				case MAW_EVENT_CLICKED:
					eventType = "Clicked";
					break;
				case MAW_EVENT_ITEM_CLICKED:
					eventType = "ItemClicked";
					break;
				case MAW_EVENT_TAB_CHANGED:
					eventType = "TabChanged";
					break;
				case MAW_EVENT_GL_VIEW_READY:
					eventType = "GLViewReady";
					break;
				case MAW_EVENT_WEB_VIEW_URL_CHANGED:
					eventType = "WebViewURLChanged";
					break;
				case MAW_EVENT_STACK_SCREEN_POPPED:
					eventType = "StackScreenPopped";
					break;
				case MAW_EVENT_SLIDER_VALUE_CHANGED:
					eventType = "SliderValueChanged";
					break;
				case MAW_EVENT_DATE_PICKER_VALUE_CHANGED:
					eventType = "DatePickerValueChanged";
					break;
				case MAW_EVENT_NUMBER_PICKER_VALUE_CHANGED:
					eventType = "NumberPickerValueChanged";
					break;
				case MAW_EVENT_VIDEO_STATE_CHANGED:
					eventType = "VideoStateChanged";
					break;
				case MAW_EVENT_EDIT_BOX_EDITING_DID_BEGIN:
					eventType = "EditBoxEditingDidBegin";
					break;
				case MAW_EVENT_EDIT_BOX_EDITING_DID_END:
					eventType = "EditBoxEditingDidEnd";
					break;
				case MAW_EVENT_EDIT_BOX_TEXT_CHANGED:
					eventType = "EditBoxTextChanged";
					break;
				case MAW_EVENT_EDIT_BOX_RETURN:
					eventType = "EditBoxReturn";
					break;
				case MAW_EVENT_WEB_VIEW_CONTENT_LOADING:
					eventType = "WebViewContentLoading";
					break;
				case MAW_EVENT_WEB_VIEW_HOOK_INVOKED:
					eventType = "WebViewHookInvoked";
					break;
				case MAW_EVENT_DIALOG_DISMISSED:
					eventType = "DialogDismissed";
					break;
			}
			// TODO: Micke says: Here we could add a string parameter for
			// "WebViewHookInvoked" events to be able to pass data to
			// the main (hidden) WebView using mosync.bridge.sendRaw.
			sprintf(buffer,
				"mosync.nativeui.event(%d, \"%s\", %d, %d, %d)",
				widget,
				eventType.c_str(),
				firstParameter,
				secondParameter,
				thirdParameter);
			mWebView->callJS(buffer);
		}
	}

	void NativeUIMessageHandler::sendNativeUIError(const char *data)
	{
		char script[2048];
		sprintf(script, "mosync.nativeui.error(%s)", data);
		mWebView->callJS(script);
	}

	void NativeUIMessageHandler::sendNativeUISuccess(const char *data)
	{
		char script[2048];
		sprintf(script, "mosync.nativeui.success(%s)", data);
		mWebView->callJS(script);
	}
} // namespace
