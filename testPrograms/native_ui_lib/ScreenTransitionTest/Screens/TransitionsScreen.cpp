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
 * @file FirstScreen.cpp
 * @author Bogdan Iusco & Mircea Vasiliniuc
 * @date 20 Nov 2012
 *
 * @brief Screen transitions screen.
 */

#define TITLE_TXT "Transitions"
#define INFO_TXT "Choose screen transition!"
#define BEGIN_TRANS_BTN_TEXT "Begin transition"

#define SCREEN_COLOR 0x10637b

#include <NativeUI/Button.h>
#include <NativeUI/Label.h>
#include <NativeUI/VerticalLayout.h>
#include <NativeUI/HorizontalLayout.h>
#include <NativeUI/ListView.h>
#include <NativeUI/ListViewItem.h>

#include "TransitionsScreen.h"
#include "ScreenUtils.h"

namespace ScreenTransitionTest
{
	/**
	 * Constructor.
	 * @param observer Observer for this screen.
	 */
	TransitionsScreen::TransitionsScreen(TransitionsScreenObserver& observer):
		mObserver(observer),
		mMainLayout(NULL),
		mFooterLayout(NULL),
		mShowSecondScreenButton(NULL),
		mTitleLabel(NULL),
		mScreenTransitionListView(NULL),
		mPreviousItem(NULL),
		mSelectedListItem(0)
	{
		fillScreenTransitionContainer();

		this->createUI();
		mShowSecondScreenButton->addButtonListener(this);
	}

	/**
	 * Destructor.
	 */
	TransitionsScreen::~TransitionsScreen()
	{
		mShowSecondScreenButton->removeButtonListener(this);
		mScreenTransitionListView->removeListViewListener(this);
	}

	/**
	 * Create screen's UI.
	 */
	void TransitionsScreen::createUI()
	{
		mMainLayout = new NativeUI::VerticalLayout();
		mMainLayout->setBackgroundColor(SCREEN_COLOR);
		if ( ScreenUtils::OS_WIN != ScreenUtils::getCurrentPlatform() )
		{
			mMainLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		}
		mMainLayout->fillSpaceVertically();
		mMainLayout->fillSpaceHorizontally();
		setMainWidget(mMainLayout);
		setTitle(TITLE_TXT);

		ScreenUtils::addVerticalSpacerToLayout(mMainLayout, SPACER_HEIGHT);

		addTitleLable();

		ScreenUtils::addVerticalSpacerToLayout(mMainLayout, SPACER_HEIGHT);

		addTransitionsList();

		ScreenUtils::addVerticalSpacerToLayout(mMainLayout, SPACER_HEIGHT);

		addFooter();
	}

	void TransitionsScreen::addTitleLable()
	{
		mTitleLabel = new NativeUI::Label();
		mTitleLabel->setText(INFO_TXT);
		mTitleLabel->fillSpaceHorizontally();
		mTitleLabel->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		mTitleLabel->setTextVerticalAlignment(MAW_ALIGNMENT_CENTER);
		mTitleLabel->setFontSize(TITLE_FONT_SIZE);
		mMainLayout->addChild(mTitleLabel);
	}

	void TransitionsScreen::addTransitionsList()
	{
		mListLayout = new NativeUI::HorizontalLayout();
		mListLayout->setBackgroundColor(SCREEN_COLOR);
		if ( ScreenUtils::OS_WIN != ScreenUtils::getCurrentPlatform() )
		{
			mListLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		}
		mListLayout->fillSpaceVertically();
		mListLayout->fillSpaceHorizontally();

		mScreenTransitionListView = new NativeUI::ListView();
		mScreenTransitionListView->addListViewListener(this);
		mScreenTransitionListView->fillSpaceVertically();
		mScreenTransitionListView->fillSpaceHorizontally();

		// Just for esthetics.
		if ( ScreenUtils::OS_WIN == ScreenUtils::getCurrentPlatform() )
			mScreenTransitionListView->setBackgroundColor(0x000000);

		populateTransitionList();
		mListLayout->addChild(mScreenTransitionListView);
		mMainLayout->addChild(mListLayout);
	}

	void TransitionsScreen::addFooter()
	{
		mFooterLayout = new NativeUI::HorizontalLayout();
		mFooterLayout->setBackgroundColor(BLACK_COLOR);
		mFooterLayout->setHeight(FOOTER_HEIGHT);

		mShowSecondScreenButton = new NativeUI::Button();
		mShowSecondScreenButton->setText(BEGIN_TRANS_BTN_TEXT);

		if ( ScreenUtils::OS_WIN == ScreenUtils::getCurrentPlatform() )
		{
			mFooterLayout->fillSpaceHorizontally();
			mFooterLayout->addChild(new NativeUI::HorizontalLayout());
			mFooterLayout->addChild(mShowSecondScreenButton);
			mFooterLayout->addChild(new NativeUI::HorizontalLayout());
		}
		else
		{
			mFooterLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
			mFooterLayout->setChildVerticalAlignment(MAW_ALIGNMENT_CENTER);
			mFooterLayout->addChild(mShowSecondScreenButton);
		}
		mMainLayout->addChild(mFooterLayout);
	}

	/**
	 * This method is called if the touch-up event was inside the
	 * bounds of the button.
	 * Platform: iOS, Android, Windows Phone.
	 * @param button The button object that generated the event.
	 */
	void TransitionsScreen::buttonClicked(NativeUI::Widget* button)
	{
		int transitionType = MAW_TRANSITION_TYPE_NONE;
		if ( mSelectedListItem > 0 )
		{
			transitionType = mPlatformTransitions[mSelectedListItem];
		}
		mObserver.showLastScreen(transitionType);

		giveFocusToList();
	}

	/**
	 * This method is called when a list view item is clicked.
	 * @param listView The list view object that generated the event.
	 * @param listViewItem The ListViewItem object that was clicked.
	 */
	void TransitionsScreen::listViewItemClicked(
	    NativeUI::ListView* listView,
	    NativeUI::ListViewItem* listViewItem)
	{
		if ( ScreenUtils::OS_ANDROID == ScreenUtils::getCurrentPlatform() )
		{
		    if ( NULL != mPreviousItem )
		    {
	            mPreviousItem->setBackgroundColor(BLACK_COLOR);
		    }
		    if (listView == mScreenTransitionListView)
		    {
		        printf("mScreenTransitionListView event: item clicked");
		        listViewItem->setBackgroundColor(SELECTED_COLOR);
		    }
		}
	    mPreviousItem = listViewItem;
	}

    /**
	 * This method is called when a list view item is clicked.
	 * @param listView The list view object that generated the event.
	 * @param index The index on which the  list view item is positioned.
	 */
	void TransitionsScreen::listViewItemClicked(
		NativeUI::ListView* listView,
		int index)
	{
	    if (listView == mScreenTransitionListView)
	    {
	        mSelectedListItem = index;
	    }
	}

	/**
	 * Constructs the transitions map.
	 */
	void TransitionsScreen::fillScreenTransitionContainer()
	{
		if ( ScreenUtils::OS_ANDROID == ScreenUtils::getCurrentPlatform() )
		{
			mPlatformTransitions = MAUtil::Vector<int>(ScreenUtils::kAndroidTransitionsArray,
					(sizeof(ScreenUtils::kAndroidTransitionsArray) / sizeof(ScreenUtils::kAndroidTransitionsArray[0])));
		}
		else if ( ScreenUtils::OS_IOS == ScreenUtils::getCurrentPlatform() )
		{
			mPlatformTransitions = MAUtil::Vector<int>(ScreenUtils::kiOSTransitionsArray,
					(sizeof(ScreenUtils::kiOSTransitionsArray) / sizeof(ScreenUtils::kiOSTransitionsArray[0])));
		}
		else if ( ScreenUtils::OS_WIN == ScreenUtils::getCurrentPlatform() )
		{
			mPlatformTransitions = MAUtil::Vector<int>(ScreenUtils::kWindowsTransitionsArray,
					(sizeof(ScreenUtils::kWindowsTransitionsArray) / sizeof(ScreenUtils::kWindowsTransitionsArray[0])));
		}
	}

	/**
	 * Populates the screen transition list.
	 */
	void TransitionsScreen::populateTransitionList()
	{
		if ( NULL != mScreenTransitionListView )
		{
			for ( int i = 0; i < mPlatformTransitions.size(); i++ )
			{
				NativeUI::ListViewItem* transitionItem = new NativeUI::ListViewItem();

				transitionItem->setText(ScreenUtils::getTransitionName(mPlatformTransitions[i]));
				if ( ScreenUtils::OS_ANDROID == ScreenUtils::getCurrentPlatform() )
				{
					transitionItem->setBackgroundColor(BLACK_COLOR);
					transitionItem->setFontColor(WHITE_COLOR);
					transitionItem->setSelectionStyle(NativeUI::LIST_VIEW_ITEM_SELECTION_STYLE_NONE);
				}

				transitionItem->setHeight(LIST_ITEM_HEIGHT);
				mScreenTransitionListView->addChild(transitionItem);
			}
		}
	}

	void TransitionsScreen::giveFocusToList()
	{
		if ( ScreenUtils::OS_ANDROID == ScreenUtils::getCurrentPlatform() )
		{
			//Workaround for Android 4.0 bug related to list view focus.
			mScreenTransitionListView->setProperty(MAW_LIST_VIEW_REQUEST_FOCUS, "true");
		}
	}
}
