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

#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <ma.h>
#include <MAUtil/String.h>
#include <MAUtil/Map.h>
#include <MAUtil/Environment.h>


namespace MoSync {
	namespace UI {

	using MAUtil::String;
	using MAUtil::Vector;
	using MAUtil::Map;
	using MAUtil::CustomEventListener;

	class UIItem {
	public:
		void setProperty(const String& property, const String& value);
		void setProperty(const String& property, int value);
		void setProperty(const String& property, float value);
		int getPropertyInt(const String& property) const;
		String getPropertyString(const String& property) const;
		static UIItem* itemByHandle(MAHandle handle);
		const String& getType() const;
		MAHandle getHandle() const;
	protected:
		MAHandle mHandle;
		String mType;
		static Map<MAHandle, UIItem*> sItems;
	};

	class Widget : public UIItem {
	public:
		static int getLastError();
		Widget(const String& widgetType);
		void addChild(Widget* widget);
	};

	class Screen : public UIItem, public CustomEventListener {
	public:
		Screen(const String& title = "");
		virtual ~Screen() {}
		virtual void show();
		virtual void hide();

		virtual void willAppear();

		void setMainWidget(Widget* widget);
		virtual void customEvent(const MAEvent& event);

		virtual void buttonClicked(UIItem* item) {}
		virtual void editBoxReturned(UIItem* item) {}
		virtual void searchBarClicked(UIItem* item, int button) {}
		virtual void checkboxStateChanged(UIItem* item, bool checked) {}
		virtual void tabBarIndexChanged(UIItem* item, int index) {}
		virtual void listboxItemSelected(UIItem* item, int index) {}
		virtual void glViewReady(UIItem* item) {}
	};

	class TabScreen : public Screen {
	public:
		TabScreen(const String& title);
		virtual ~TabScreen() {}
		void addScreen(Screen* screen);
		void setActiveTab(int index);
	};

	class StackScreen : public Screen {
	public:
		StackScreen();
		virtual ~StackScreen() {}
		void pushScreen(Screen* screen);
		void pop();
		void customEvent(const MAEvent& event);
		int stackSize();
	private:
		Vector<Screen*> mStack;
	};
	} // namespace UI
} // namespace MoSync

#endif
