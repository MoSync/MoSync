#include <ma.h>
#include <mavsprintf.h>
#include <MAUtil/Moblet.h>
#include <NativeUI/Widgets.h>
#include <NativeUI/WidgetUtil.h>

#include "Utils.h"

using namespace MAUtil;
using namespace NativeUI;

#define UPDATE_INTERVAL 		2000

#define MAIN_SCREEN_BGK_COLOR 	0xFFFFFF
#define CONTAINER_BGK_COLOR 	0xBA44DD
#define LABEL_BGK_COLOR 		0x33A33B
#define LABEL_FONT_COLOR 		0x000000
#define AUX_COLOR 				0xDADADA

#define SHORT_TEXT_SAMPLE "Simple label."
#define LONG_TEXT_SAMPLE "This is a simple label example which SHOULD extend the space I gave it, and thus resulting in a chopped of label on iOS."
#define DESCRIPTION_FONT 12

/**
 * Moblet to be used as a template for a Native UI application.
 */
class NativeUIMoblet : public Moblet, public ButtonListener, TimerListener, PointerListener
{
	typedef enum
	{
		HORIZONTAL_LAYOUT = 0,
		VERTICAL_LAYOUT
	} LayoutType;

	typedef enum
	{
		FILL_HORIZONTALLY_FILL_VERTICALLY = 10,
		FILL_HORIZONTALLY_WRAP_VERTICALLY,
		WRAP_HORIZONTALLY_FILL_VERTICALLY,
		WRAP_HORIZONTALLY_WRAP_VERTICALLY
	} SizePolicy;

	typedef enum
	{
		SHORT_TEXT = 100,
		LONG_TEXT,
	} WidgetProperty;

public:
	/**
	 * The constructor creates the user interface.
	 */
	NativeUIMoblet():
		mScreen(NULL),
		mFontSize(TestUtils::getFontSize()),
		mContainerLayoutTypeCounter(HORIZONTAL_LAYOUT),
		mContainerSizePolicyCounter(FILL_HORIZONTALLY_FILL_VERTICALLY),
		mLabelSizePolicyCounter(FILL_HORIZONTALLY_FILL_VERTICALLY),
		mLabelPropertyCounter(SHORT_TEXT),
		mLayoutsCounter(1),
		mIsFrozen(TRUE)
	{
		fillNamesMap();
		createUI();
	}

	/**
	 * Destructor.
	 */
	virtual ~NativeUIMoblet()
	{
		// All the children will be deleted.
		delete mScreen;
	}

	void fillNamesMap()
	{
		mNamesMap.insert(HORIZONTAL_LAYOUT, "Horizontal layout");
		mNamesMap.insert(VERTICAL_LAYOUT, "Vertical layout");
		mNamesMap.insert(FILL_HORIZONTALLY_FILL_VERTICALLY, "Fill Horizontal/Fill Vertical");
		mNamesMap.insert(FILL_HORIZONTALLY_WRAP_VERTICALLY, "Fill Horizontal/Wrap Vertical");
		mNamesMap.insert(WRAP_HORIZONTALLY_FILL_VERTICALLY, "Wrap Horizontal/Fill Vertical");
		mNamesMap.insert(WRAP_HORIZONTALLY_WRAP_VERTICALLY, "Wrap Horizontal/Wrap Vertical");
		mNamesMap.insert(SHORT_TEXT, "Short text");
		mNamesMap.insert(LONG_TEXT, "Long text");
	}

	/**
	 * Create the user interface.
	 */
	void createUI() {
		mScreen = new Screen();
		mScreen->setBackgroundColor(MAIN_SCREEN_BGK_COLOR);

		printCounters("createUI");

		// For the first setup.
		setMainScreen();

		mScreen->show();
	}

	void setMainScreen()
	{
		// Because of issue http://jira.mosync.com/browse/MOSYNC-2732 the hole layout must be created.
		VerticalLayout* subContent = new VerticalLayout();
		setRoundPadding(subContent);

		Layout* warningContainer = NULL;
		(VERTICAL_LAYOUT == mContainerLayoutTypeCounter)? warningContainer = new VerticalLayout() :
				warningContainer = new HorizontalLayout();

		setupMainContainer(warningContainer, mContainerSizePolicyCounter);
		addChildrenToMainContainer(warningContainer, mLabelSizePolicyCounter, mLabelPropertyCounter);

		addDescriptionWidget(subContent);
		subContent->addChild(warningContainer);
		//addCheckboxWidget(mMainLayout);

		mScreen->setMainWidget(subContent);
	}

	void setupMainContainer(Layout* aTargetLayout, int aSizePolicy)
	{
		switch(aSizePolicy)
		{
		case FILL_HORIZONTALLY_FILL_VERTICALLY:
			aTargetLayout->fillSpaceHorizontally();
			aTargetLayout->fillSpaceVertically();
			break;
		case FILL_HORIZONTALLY_WRAP_VERTICALLY:
			aTargetLayout->fillSpaceHorizontally();
			aTargetLayout->wrapContentVertically();
			break;
		case WRAP_HORIZONTALLY_FILL_VERTICALLY:
			aTargetLayout->wrapContentHorizontally();
			aTargetLayout->fillSpaceVertically();
			break;
		case WRAP_HORIZONTALLY_WRAP_VERTICALLY:
			aTargetLayout->wrapContentHorizontally();
			aTargetLayout->wrapContentVertically();
			break;
		default:
			printf("EXCEPTION - setupMainContainer - It should not reach this point!");
			break;
		}
		aTargetLayout->setBackgroundColor(CONTAINER_BGK_COLOR);
		setRoundPadding(aTargetLayout);
	}

	void addChildrenToMainContainer(Layout* aTargetLayout, int aSizePolicy, int aChildrenStrategy)
	{
		Label* warningLbl = new Label();

		switch (aChildrenStrategy)
		{
		case SHORT_TEXT:
			warningLbl->setText(SHORT_TEXT_SAMPLE);
			break;
		case LONG_TEXT:
			warningLbl->setText(LONG_TEXT_SAMPLE);
			break;
		default:
			break;
		}

		switch(aSizePolicy)
		{
		case FILL_HORIZONTALLY_FILL_VERTICALLY:
			warningLbl->fillSpaceHorizontally();
			warningLbl->fillSpaceVertically();
			break;
		case FILL_HORIZONTALLY_WRAP_VERTICALLY:
			warningLbl->fillSpaceHorizontally();
			warningLbl->wrapContentVertically();
			break;
		case WRAP_HORIZONTALLY_FILL_VERTICALLY:
			warningLbl->wrapContentHorizontally();
			warningLbl->fillSpaceVertically();
			break;
		case WRAP_HORIZONTALLY_WRAP_VERTICALLY:
			warningLbl->wrapContentHorizontally();
			warningLbl->wrapContentVertically();
			break;
		default:
			printf("EXCEPTION - addChildrenToMainContainer - It should not reach this point!");
			break;
		}
		//warningLbl->setMaxNumberOfLines(0);

		warningLbl->setFontSize(mFontSize);
		warningLbl->setFontColor(LABEL_FONT_COLOR);
		warningLbl->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		warningLbl->setBackgroundColor(LABEL_BGK_COLOR);

		//TODO Make this dynamic.
		Button* button = new Button();
		button->setText("Button");
		button->wrapContentHorizontally();
		button->wrapContentVertically();

		aTargetLayout->addChild(warningLbl);
		(VERTICAL_LAYOUT == mContainerLayoutTypeCounter)?
				TestUtils::addVerticalSpacerToLayout(aTargetLayout) :
				TestUtils::addHorizontalSpacerToLayout(aTargetLayout);

		aTargetLayout->addChild(button);
	}

	void runTimerEvent()
	{
		if ( updateCounters() )
		{
			// Counters reached last layout.
			printf("runTimerEvent - Last configuration reached");
			// Stop generating layouts.
			MAUtil::Environment::removeTimer(this);
			return;
		}
		mLayoutsCounter++;
		printCounters("runTimerEvent");
		setMainScreen();
	}

	/*
	 * @return True if counters reached last configuration, false otherwise.
	 */
	bool updateCounters()
	{
		// TODO Think about improving the counter mechanism.
		mLabelPropertyCounter++;
		if ( mLabelPropertyCounter > LONG_TEXT )
		{
			mLabelPropertyCounter = SHORT_TEXT;
			mLabelSizePolicyCounter++;
			if ( mLabelSizePolicyCounter > WRAP_HORIZONTALLY_WRAP_VERTICALLY)
			{
				mLabelSizePolicyCounter = FILL_HORIZONTALLY_FILL_VERTICALLY;
				mContainerSizePolicyCounter++;
				if ( mContainerSizePolicyCounter > WRAP_HORIZONTALLY_WRAP_VERTICALLY)
				{
					mContainerSizePolicyCounter = FILL_HORIZONTALLY_FILL_VERTICALLY;
					mContainerLayoutTypeCounter++;
					if ( mContainerLayoutTypeCounter > VERTICAL_LAYOUT )
					{
						// Reset
						mLayoutsCounter = 1;
						mContainerLayoutTypeCounter = HORIZONTAL_LAYOUT;
						mContainerSizePolicyCounter = FILL_HORIZONTALLY_FILL_VERTICALLY;
						mLabelSizePolicyCounter = FILL_HORIZONTALLY_FILL_VERTICALLY;
						mLabelPropertyCounter = SHORT_TEXT;
						return true;
					}
				}
			}
		}
		return false;
	}

	void printCounters(const char* aMarker)
	{
		printf("------------------------------");
		printf("%s mLayoutsCounter: %d", aMarker, mLayoutsCounter);
		printf("%s mContainerLayoutTypeCounter: %d", aMarker, mContainerLayoutTypeCounter);
		printf("%s mContainerSizePolicyCounter: %d", aMarker, mContainerSizePolicyCounter);
		printf("%s mLabelSizePolicyCounter: %d", aMarker, mLabelSizePolicyCounter);
		printf("%s mLabelPropertyCounter: %d", aMarker, mLabelPropertyCounter);
	}

	void setRoundPadding(Layout* aTargetLayout)
	{
		aTargetLayout->setPaddingBottom(int(mFontSize * 0.5));
		aTargetLayout->setPaddingTop(int(mFontSize * 0.5));
		aTargetLayout->setPaddingLeft(int(mFontSize * 0.5));
		aTargetLayout->setPaddingRight(int(mFontSize * 0.5));
	}

	void addCheckboxWidget(Layout* aTargetLayout)
	{
		VerticalLayout* betaWrapper = new VerticalLayout();
		betaWrapper->fillSpaceHorizontally();
		betaWrapper->wrapContentVertically();
		betaWrapper->setPaddingTop(int(mFontSize * 0.5));

		VerticalLayout* betaContainer = new VerticalLayout();
		betaContainer->fillSpaceHorizontally();
		betaContainer->wrapContentVertically();
		betaContainer->setBackgroundColor(AUX_COLOR);
		setRoundPadding(aTargetLayout);

		HorizontalLayout* subSettingsContainer = new HorizontalLayout();
		subSettingsContainer->fillSpaceHorizontally();
		subSettingsContainer->wrapContentVertically();
		subSettingsContainer->setChildVerticalAlignment(MAW_ALIGNMENT_CENTER);
		subSettingsContainer->setChildHorizontalAlignment(MAW_ALIGNMENT_RIGHT);

		Label* betaText = new Label();
		betaText->setText("This is a checkBox");
		betaText->setFontColor(LABEL_FONT_COLOR);
		betaText->setFontSize(mFontSize);
		betaText->fillSpaceHorizontally();
		betaText->wrapContentVertically();
		betaText->setTextHorizontalAlignment(MAW_ALIGNMENT_LEFT);

		CheckBox* betaCB = new CheckBox();
		betaCB->wrapContentHorizontally();
		betaCB->wrapContentVertically();
		betaCB->setState(true);

		subSettingsContainer->addChild(betaText);
		subSettingsContainer->addChild(betaCB);

		betaContainer->addChild(subSettingsContainer);

		betaWrapper->addChild(betaContainer);
		aTargetLayout->addChild(betaWrapper);
	}

	void addDescriptionWidget(Layout* aTargetLayout)
	{
		VerticalLayout* paddingLayout = new VerticalLayout();
		paddingLayout->fillSpaceHorizontally();
		paddingLayout->wrapContentVertically();
		paddingLayout->setPaddingBottom(int(mFontSize * 0.5));

		VerticalLayout* descLayout = new VerticalLayout();
		descLayout->fillSpaceHorizontally();
		descLayout->wrapContentVertically();
		descLayout->setBackgroundColor(AUX_COLOR);
		setRoundPadding(descLayout);

		Label* layoutCounterLbl = new Label();
		char buf[64];
		sprintf(buf, "Layout nr: %d", mLayoutsCounter);
		layoutCounterLbl->setText(buf);

		Label* containerTypeLbl = new Label();
		containerTypeLbl->setBackgroundColor(CONTAINER_BGK_COLOR);
		sprintf(buf, "Container layout type: %s", mNamesMap[mContainerLayoutTypeCounter]);
		containerTypeLbl->setText(buf);

		Label* containerPolicyLbl = new Label();
		containerPolicyLbl->setBackgroundColor(CONTAINER_BGK_COLOR);
		sprintf(buf, "Container size policy: %s", mNamesMap[mContainerSizePolicyCounter]);
		containerPolicyLbl->setText(buf);

		Label* labelPolicyLbl = new Label();
		labelPolicyLbl->setBackgroundColor(LABEL_BGK_COLOR);
		sprintf(buf, "Label size policy: %s", mNamesMap[mLabelSizePolicyCounter]);
		labelPolicyLbl->setText(buf);

		Label* labelTypeLbl = new Label();
		labelTypeLbl->setBackgroundColor(LABEL_BGK_COLOR);
		sprintf(buf, "Label type: %s", mNamesMap[mLabelPropertyCounter]);
		labelTypeLbl->setText(buf);

		Label* helpLbl = new Label();
		sprintf(buf, "Tap to play/pause layouts. Update: %d ms", UPDATE_INTERVAL);
		helpLbl->setText(buf);

		helpLbl->setFontSize(DESCRIPTION_FONT);
		layoutCounterLbl->setFontSize(DESCRIPTION_FONT);
		containerTypeLbl->setFontSize(DESCRIPTION_FONT);
		containerPolicyLbl->setFontSize(DESCRIPTION_FONT);
		labelPolicyLbl->setFontSize(DESCRIPTION_FONT);
		labelTypeLbl->setFontSize(DESCRIPTION_FONT);

		descLayout->addChild(helpLbl);
		TestUtils::addVerticalSpacerToLayout(descLayout);

		descLayout->addChild(layoutCounterLbl);
		TestUtils::addVerticalSpacerToLayout(descLayout);

		descLayout->addChild(containerTypeLbl);
		descLayout->addChild(containerPolicyLbl);
		TestUtils::addVerticalSpacerToLayout(descLayout);

		descLayout->addChild(labelPolicyLbl);
		descLayout->addChild(labelTypeLbl);
		paddingLayout->addChild(descLayout);
		aTargetLayout->addChild(paddingLayout);
	}

	/**
	 * Called when a key is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		if (MAK_BACK == keyCode || MAK_0 == keyCode)
		{
			// Call close to exit the application.
			close();
		}
	}

	/**
	* This method is called if the touch-up event was inside the
	* bounds of the button.
	* @param button The button object that generated the event.
	*/
	virtual void buttonClicked(Widget* button)
	{
		((Button*) button)->setText("Hello World");
	}

	virtual void pointerPressEvent(MAPoint2d p)
	{}

	virtual void pointerMoveEvent(MAPoint2d p)
	{}

	virtual void pointerReleaseEvent(MAPoint2d p)
	{
		if ( !mIsFrozen )
		{
			printf("pointerReleaseEvent - changing layouts is now frozen.");
			MAUtil::Environment::removeTimer(this);
		}
		else
		{
			printf("pointerReleaseEvent - changing layouts is now active.");
			runTimerEvent();
			MAUtil::Environment::addTimer(this, UPDATE_INTERVAL, 0);
		}
		mIsFrozen = !mIsFrozen;
	}

private:
    Screen* mScreen;			//A Native UI screen

    int mFontSize;
	int mContainerLayoutTypeCounter;
	int mContainerSizePolicyCounter;
	int mLabelSizePolicyCounter;
	int mLabelPropertyCounter;

	int mLayoutsCounter;

	bool mIsFrozen;

	MAUtil::Map<int, char*> mNamesMap;
};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	Moblet::run(new NativeUIMoblet());
	return 0;
}
