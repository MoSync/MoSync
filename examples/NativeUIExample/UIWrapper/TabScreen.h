
	class TabScreen : public Screen {
	public:
		TabScreen(const String& title);
		void addScreen(Screen* screen);
		void setActiveTab(int index);
	};
