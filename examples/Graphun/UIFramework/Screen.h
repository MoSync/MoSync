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
		Widget(const String& widgetType);
		void addChild(Widget* widget);
	};

	class Screen : public UIItem, public CustomEventListener {
	public:
		Screen(const String& title = "");
		virtual void show();
		virtual void hide();

		virtual void willAppear();

		void setMainWidget(Widget* widget);
		virtual void customEvent(const MAEvent& event);

		virtual void buttonClicked(UIItem* item) {}
		virtual void searchBarClicked(UIItem* item, int button) {}
		virtual void checkboxStateChanged(UIItem* item, bool checked) {}
		virtual void tabBarIndexChanged(UIItem* item, int index) {}
		virtual void listboxItemSelected(UIItem* item, int index) {}
		virtual void glViewReady(UIItem* item) {}
	};

	class TabScreen : public Screen {
	public:
		TabScreen(const String& title);
		void addScreen(Screen* screen);
		void setActiveTab(int index);
	};

	class StackScreen : public Screen {
	public:
		StackScreen();
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
