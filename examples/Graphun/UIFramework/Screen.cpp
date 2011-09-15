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

#include <ma.h>
#include <IX_WIDGET.h>
#include "Screen.h"

#include <stdio.h>
#include <stdlib.h>

#include <MAUtil/Environment.h>


namespace MoSync {
	namespace UI {

	using MAUtil::Environment;
	Map<MAHandle, UIItem*> UIItem::sItems;


	int maWidgetSetProperty(MAHandle handle, const char *property, int value) {
		char buffer[256];
		snprintf(buffer, 256, "%d", value);
		::maWidgetSetProperty(handle, property, buffer);
		return 0;
	}

	int maWidgetSetProperty(MAHandle handle, const char *property, float value) {
		char buffer[256];
		snprintf(buffer, 256, "%f", value);
		::maWidgetSetProperty(handle, property, buffer);
		return 0;
	}

	void UIItem::setProperty(const String& property, const String& value) {
		::maWidgetSetProperty(mHandle, property.c_str(), value.c_str());
	}

	void UIItem::setProperty(const String& property, int value) {
		maWidgetSetProperty(mHandle, property.c_str(), value);
	}

	void UIItem::setProperty(const String& property, float value) {
		maWidgetSetProperty(mHandle, property.c_str(), value);
	}

	int UIItem::getPropertyInt(const String& property) const {
		char ret[256];
		maWidgetGetProperty(mHandle, property.c_str(), ret, 256);
		return atoi(ret);
	}

	String UIItem::getPropertyString(const String& property) const {
		char ret[256];
		maWidgetGetProperty(mHandle, property.c_str(), ret, 256);
		return ret;
	}

	UIItem* UIItem::itemByHandle(MAHandle handle) {
		return sItems[handle];
	}

	const String& UIItem::getType() const {
		return mType;
	}

	MAHandle UIItem::getHandle() const {
		return mHandle;
	}


	Widget::Widget(const String& widgetType) {
		mType = widgetType;
		mHandle = maWidgetCreate(widgetType.c_str());
		sItems[mHandle] = this;
	}

	void Widget::addChild(Widget* widget) {
		maWidgetAddChild(mHandle, widget->mHandle);
	}

	Screen::Screen(const String& title) {
		mType = "Screen";
		mHandle = maWidgetCreate("Screen");
		sItems[mHandle] = this;
		::maWidgetSetProperty(mHandle, "title", title.c_str());
	}

	void Screen::show() {
		maWidgetScreenShow(mHandle);
		willAppear();
	}


	void Screen::willAppear() {
		Environment::getEnvironment().addCustomEventListener(this);
	}

	void Screen::hide() {
		Environment::getEnvironment().removeCustomEventListener(this);
	}

	void Screen::setMainWidget(Widget* widget) {
		maWidgetAddChild(mHandle, widget->getHandle());
	}

	void Screen::customEvent(const MAEvent& event) {
		if(event.type == EVENT_TYPE_WIDGET) {
			MAWidgetEventData* data = (MAWidgetEventData*)event.data;
			switch(data->eventType) {
				case MAW_EVENT_CLICKED: {
					UIItem* item = Widget::itemByHandle(data->widgetHandle);
					String type = item->getType();

					if(type == "CheckBox") {
						checkboxStateChanged(item, item->getPropertyString("checked") == "true");
					}
					else if(type == "ImageButton" || type == "Button" || type == "NavBar" || type == "EditBox")
						buttonClicked(item);
				}
				break;
				case MAW_EVENT_EDIT_BOX_RETURN:
					UIItem* item = Widget::itemByHandle(data->widgetHandle);
					editBoxReturned(item);
				break;
				case MAW_EVENT_ITEM_CLICKED: {
					UIItem* item = Widget::itemByHandle(data->widgetHandle);
					listboxItemSelected(item, data->listItemIndex);
				}
				break;
				case MAW_EVENT_GL_VIEW_READY: {
					UIItem* item = Widget::itemByHandle(data->widgetHandle);
					glViewReady(item);
				}
				break;
			}
		}
	}

	TabScreen::TabScreen(const String& title) {
		mType = "TabScreen";
		mHandle = maWidgetCreate("TabScreen");
		sItems[mHandle] = this;
		setProperty("title", title.c_str());
	}

	void TabScreen::addScreen(Screen* screen) {
		maWidgetAddChild(mHandle, screen->getHandle());
	}

	void TabScreen::setActiveTab(int index) {
		maWidgetSetProperty(mHandle, "selectedTab", index);
	}

	StackScreen::StackScreen() {
		mType = "StackScreen";
		mHandle = maWidgetCreate("StackScreen");
		
		// check if this is supported, NativeUI is only
		// supported on Android and iOS devices/emulators
		if (-1 == mHandle)
		{
			maPanic(0, "NativeUI is only available on Android and iOS.");
		}
		
		sItems[mHandle] = this;
	}

	void StackScreen::pushScreen(Screen* screen) {
		if(mStack.size()) {
			mStack[mStack.size()-1]->hide();
		}
		maWidgetStackScreenPush(mHandle, screen->getHandle());
		screen->willAppear();
		mStack.add(screen);
	}

	void StackScreen::customEvent(const MAEvent& event) {
		if(event.type == EVENT_TYPE_WIDGET) {
			MAWidgetEventData* data = (MAWidgetEventData*)event.data;
			switch(data->eventType) {
				case MAW_EVENT_STACK_SCREEN_POPPED: {
					if(mStack.size()<=1) return;
					mStack[mStack.size()-1]->hide();
					mStack.resize(mStack.size()-1);
					if(!mStack.size()) return;
					mStack[mStack.size()-1]->willAppear();
				}
				break;

				default:
					Screen::customEvent(event);
				break;
			}
		}
	}


	void StackScreen::pop() {
		//if(!mStack.size()) return;
		//mStack[mStack.size()-1]->hide();
		//mStack.resize(mStack.size()-1);
		if(mStack.size()<=1) return;
		maWidgetStackScreenPop(mHandle);
		//if(!mStack.size()) return;
		//mStack[mStack.size()-1]->willAppear();
	}
	
	int StackScreen::stackSize()
	{
		return mStack.size();
	}

	} // namespace UI
} // namespace MoSync
