
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
