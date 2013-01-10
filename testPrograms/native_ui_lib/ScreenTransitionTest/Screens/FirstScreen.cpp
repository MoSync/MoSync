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
 * @file FirstScreen.cpp
 * @author Bogdan Iusco & Mircea Vasiliniuc
 * @date 20 Nov 2012
 *
 * @brief First screen shown.
 */

#define TITLE_TEXT "1st View. Choose transition!"
#define BEGIN_TRANS_BTN_TEXT "Begin transition"

#define SCREEN_COLOR 0x10637b

#include <NativeUI/Button.h>
#include <NativeUI/Label.h>
#include <NativeUI/VerticalLayout.h>
#include <NativeUI/HorizontalLayout.h>
#include <NativeUI/ListView.h>
#include <NativeUI/ListViewItem.h>

#include "FirstScreen.h"
#include "ScreenUtils.h"

static const char* kiOSTransitionsArray[] = {"None", "Flip From Left", "Flip From Right", "Curl Up", "Curl Down"};
static const char* kAndroidTransitionsArray[] = {"None", "Slide Left", "Slide Right", "Fade in", "Fade out"};
static const char* kWindowsTransitionsArray[] = {"None", "Slide Left", "Slide Right", "Swivel in", "Swivel out", "Turnstile Foreward", "Turnstile Backward"};

namespace Transitions
{
	/**
	 * Constructor.
	 * @param observer Observer for this screen.
	 */
	FirstScreen::FirstScreen(FirstScreenObserver& observer):
		mObserver(observer),
		mMainLayout(NULL),
		mFooterLayout(NULL),
		mShowSecondScreenButton(NULL),
		mTitleLabel(NULL),
		mScreenTransitionListView(NULL),
		mPreviousItem(NULL),
		mSelectedTransition(0)
	{
		ScreenUtils::initScreenSizeConstants(
			this->getWidth(),
			this->getHeight());

		this->createUI();
		mShowSecondScreenButton->addButtonListener(this);
	}

	/**
	 * Destructor.
	 */
	FirstScreen::~FirstScreen()
	{
		mShowSecondScreenButton->removeButtonListener(this);
		mScreenTransitionListView->removeListViewListener(this);
	}

	/**
	 * This method is called if the touch-up event was inside the
	 * bounds of the button.
	 * Platform: iOS, Android, Windows Phone.
	 * @param button The button object that generated the event.
	 */
	void FirstScreen::buttonClicked(NativeUI::Widget* button)
	{
		if ( mSelectedTransition == 0 )
		{
			mObserver.showSecondScreen(MAW_TRANSITION_TYPE_NONE, kAndroidTransitionsArray[MAW_TRANSITION_TYPE_NONE]);
			return;
		}

		if ( ScreenUtils::OS_IOS == ScreenUtils::getCurrentPlatform() )
		{
			mObserver.showSecondScreen(mSelectedTransition, kiOSTransitionsArray[mSelectedTransition]);
		}
		else if ( ScreenUtils::OS_ANDROID == ScreenUtils::getCurrentPlatform() )
		{
			// We need to obtain the correct transition type/index.
			mObserver.showSecondScreen(MAW_TRANSITION_TYPE_SLIDE_LEFT + mSelectedTransition - 1, kAndroidTransitionsArray[mSelectedTransition]);

			//Workaround for Android 4.0 bug related to list view focus.
			mScreenTransitionListView->setProperty(MAW_LIST_VIEW_REQUEST_FOCUS, "true");
		}
		else if ( ScreenUtils::OS_WIN == ScreenUtils::getCurrentPlatform() )
		{
			// We need to obtain the correct transition type/index.
			int selectedTransition = MAW_TRANSITION_TYPE_NONE;
			switch ( mSelectedTransition )
			{
			case 1:
				selectedTransition = MAW_TRANSITION_TYPE_SLIDE_RIGHT;
				break;
			case 2:
				selectedTransition = MAW_TRANSITION_TYPE_SLIDE_LEFT;
				break;
			case 3:
				selectedTransition = MAW_TRANSITION_TYPE_SWIVEL_IN;
				break;
			case 4:
				selectedTransition = MAW_TRANSITION_TYPE_SWIVEL_OUT;
				break;
			case 5:
				selectedTransition = MAW_TRANSITION_TYPE_TURNSTILE_FOREWARD;
				break;
			case 6:
				selectedTransition = MAW_TRANSITION_TYPE_TURNSTILE_BACKWARD;
				break;
			default:
				selectedTransition = MAW_TRANSITION_TYPE_NONE;
				break;
			}
			mObserver.showSecondScreen(selectedTransition, kWindowsTransitionsArray[mSelectedTransition]);
		}
	}

	/**
	 * Create screen's UI.
	 */
	void FirstScreen::createUI()
	{
		mMainLayout = new NativeUI::VerticalLayout();
		mMainLayout->setBackgroundColor(SCREEN_COLOR);
		mMainLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		mMainLayout->fillSpaceVertically();
		mMainLayout->fillSpaceHorizontally();
		this->setMainWidget(mMainLayout);

		ScreenUtils::addVerticalSpacerToLayout(mMainLayout, SPACER_HEIGHT);

		mTitleLabel = new NativeUI::Label();
		mTitleLabel->setText(TITLE_TEXT);
		mTitleLabel->setFontSize(TITLE_FONT_SIZE);
		mMainLayout->addChild(mTitleLabel);

		mListLayout = new NativeUI::HorizontalLayout();
		mListLayout->setBackgroundColor(SCREEN_COLOR);
		mListLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		mListLayout->fillSpaceVertically();
		mListLayout->fillSpaceHorizontally();

		mScreenTransitionListView = new NativeUI::ListView();
		mScreenTransitionListView->addListViewListener(this);
		mScreenTransitionListView->fillSpaceVertically();
		mScreenTransitionListView->fillSpaceHorizontally();
		populateTransitionList();

		ScreenUtils::addVerticalSpacerToLayout(mMainLayout, SPACER_HEIGHT);

		mListLayout->addChild(mScreenTransitionListView);

		mMainLayout->addChild(mListLayout);

		mFooterLayout = new NativeUI::HorizontalLayout();
		mFooterLayout->setBackgroundColor(BLACK_COLOR);
		mFooterLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		mFooterLayout->setChildVerticalAlignment(MAW_ALIGNMENT_CENTER);
		mFooterLayout->setHeight(FOOTER_HEIGHT);

		mShowSecondScreenButton = new NativeUI::Button();
		mShowSecondScreenButton->setText(BEGIN_TRANS_BTN_TEXT);

		mFooterLayout->addChild(mShowSecondScreenButton);

		mMainLayout->addChild(mFooterLayout);
	}

	void FirstScreen::populateTransitionList()
	{
		if ( NULL != mScreenTransitionListView )
		{
			int size_of_TransitionsArray = 0;
			if ( ScreenUtils::OS_ANDROID == ScreenUtils::getCurrentPlatform() )
			{
				size_of_TransitionsArray = sizeof( kAndroidTransitionsArray) / sizeof( kAndroidTransitionsArray[ 0 ] );
			}
			else if ( ScreenUtils::OS_IOS == ScreenUtils::getCurrentPlatform() )
			{
				size_of_TransitionsArray = sizeof(kiOSTransitionsArray) / sizeof( kiOSTransitionsArray[ 0 ] );
			}
			else if ( ScreenUtils::OS_WIN == ScreenUtils::getCurrentPlatform() )
			{
				size_of_TransitionsArray = sizeof(kWindowsTransitionsArray) / sizeof( kWindowsTransitionsArray[ 0 ] );
			}

			for ( int i = 0; i < size_of_TransitionsArray; i++ )
			{
				NativeUI::ListViewItem* transitionItem = new NativeUI::ListViewItem();

				if ( ScreenUtils::OS_IOS == ScreenUtils::getCurrentPlatform() )
				{
					transitionItem->setText(kiOSTransitionsArray[i]);
				}
				else if ( ScreenUtils::OS_ANDROID == ScreenUtils::getCurrentPlatform() )
				{
					transitionItem->setText(kAndroidTransitionsArray[i]);
					transitionItem->setBackgroundColor(BLACK_COLOR);
					transitionItem->setFontColor(WHITE_COLOR);
					transitionItem->setSelectionStyle(NativeUI::LIST_VIEW_ITEM_SELECTION_STYLE_NONE);
				}
				else if ( ScreenUtils::OS_WIN == ScreenUtils::getCurrentPlatform() )
				{
					transitionItem->setText(kWindowsTransitionsArray[i]);
				}

				transitionItem->setHeight(LIST_ITEM_HEIGHT);
				mScreenTransitionListView->addChild(transitionItem);
			}
		}
	}

	/**
	 * This method is called when a list view item is clicked.
	 * @param listView The list view object that generated the event.
	 * @param listViewItem The ListViewItem object that was clicked.
	 */
	void FirstScreen::listViewItemClicked(
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
	void FirstScreen::listViewItemClicked(
		NativeUI::ListView* listView,
		int index)
	{
	    if (listView == mScreenTransitionListView)
	    {
	        mSelectedTransition = index;
	    }
	}
}
