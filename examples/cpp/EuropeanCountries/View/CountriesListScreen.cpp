/*
 Copyright (C) 2012 MoSync AB

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
 * @file CountriesListScreen.cpp
 * @author Bogdan Iusco
 *
 * @brief NativeUI Screen used to show a list of countries.
 */

#include <NativeUI/Image.h>
#include <NativeUI/ListView.h>
#include <NativeUI/ListViewSection.h>
#include <NativeUI/ListViewItem.h>
#include <NativeUI/RelativeLayout.h>
#include <MAUtil/String.h>

#include "CountriesListScreen.h"
#include "CountriesListScreenObserver.h"
#include "MAHeaders.h"
#include "ViewUtils.h"
#include "../Model/ICountryDatabase.h"
#include "../Model/Country.h"

namespace EuropeanCountries
{

	/**
	 * Constructor.
	 * @param database Used to read data.
	 * @param observer Will be notified when user selects a country from list.
	 */
	CountriesListScreen::CountriesListScreen(
		ICountryDatabase& database,
		CountriesListScreenObserver& observer):
		mDatabase(database),
		mObserver(observer),
		mMainLayout(NULL)
	{
		if (isIOS())
		{
			initScreenSizeConstants(this->getWidth(), this->getHeight());
		}
		this->createUI();
		mListView->addListViewListener(this);
	}

	/**
	 * Destructor.
	 */
	CountriesListScreen::~CountriesListScreen()
	{
		mListView->removeListViewListener(this);
	}

	/**
	 * This method is called when an alphabetical list view item is clicked.
	 * From ListViewListener.
	 * @param listView The list view object that generated the event.
	 * @param listViewSection The ListViewSection object that contains the selected item.
	 * @param listViewItem The ListViewItem objet clicked.
	 */
	void CountriesListScreen::segmentedListViewItemClicked(
		NativeUI::ListView* listView,
		NativeUI::ListViewSection* listViewSection,
		NativeUI::ListViewItem* listViewItem)
	{
		int countryID = mCountryMap[listViewItem->getWidgetHandle()];
		mObserver.showCountryInfoScreen(countryID);
	}

	/**
	 * Show a screen. Only one screen at a time is visible.
	 * The previous screen will be hidden when showing a screen.
	 */
	void CountriesListScreen::show()
	{
		mListView->requestFocus();
		NativeUI::Screen::show();
	}

	/**
	 * Create screen UI.
	 */
	void CountriesListScreen::createUI()
	{
		mMainLayout = new NativeUI::RelativeLayout();
		this->setMainWidget(mMainLayout);

		this->createImageWidget();
		this->createListViewWidget();
		this->addDataToListView();
	}

	/**
	 * Create and add an Image widget to the screen's main layout.
	 * It will take the screen size.
	 */
	void CountriesListScreen::createImageWidget()
	{
		NativeUI::Image* imageWidget = new NativeUI::Image();
		imageWidget->setHeight(this->getHeight());
		imageWidget->setWidth(this->getWidth());
		imageWidget->setTopPosition(0);
		imageWidget->setLeftPosition(0);
		imageWidget->setImage(R_SCREEN_BACKGROUND);
		imageWidget->setScaleMode(NativeUI::IMAGE_SCALE_XY);
		mMainLayout->addChild(imageWidget);
	}

	/**
	 * Create and add an ListView widget to the screen's main layout.
	 * It will take the screen size and will be placed above the image widget.
	 */
	void CountriesListScreen::createListViewWidget()
	{
		mListView = new NativeUI::ListView(
			NativeUI::LIST_VIEW_TYPE_ALPHABETICAL);
		mListView->setHeight(gScreenHeight);
		mListView->setWidth(gScreenWidth);
		mListView->setTopPosition(0);
		mListView->setLeftPosition(0);
		if (!isWindowsPhone())
		{
			mListView->setProperty(MAW_WIDGET_BACKGROUND_COLOR, gLayoutBackgroundColor);
		}
		mMainLayout->addChild(mListView);
	}

	/**
	 * Add data from database into list view.
	 */
	void CountriesListScreen::addDataToListView()
	{
		// Clear data from map.
		mCountryMap.clear();

		// Create first section.
		NativeUI::ListViewSection* section = NULL;
		MAUtil::String sectionTitle("A");

		// For each country read create and add an ListViewItem widget.
		int countCountries = mDatabase.countCountries();
		for (int index = 0; index < countCountries; index++)
		{
			// If index is invalid skip this country.
			Country* country = mDatabase.getCountryByIndex(index);
			if (!country)
			{
				continue;
			}

			// If country's name is an empty string skip this country.
			MAUtil::String countryName = country->getName();
			if (countryName.length() == 0)
			{
				continue;
			}

			// Check if current country can go into current section.
			if (!section || countryName[0] != sectionTitle[0])
			{
				// Create new section.
				sectionTitle[0] = countryName[0];
				section = new NativeUI::ListViewSection(
					NativeUI::LIST_VIEW_SECTION_TYPE_ALPHABETICAL);
				section->setTitle(sectionTitle);
				section->setHeaderText(sectionTitle);
				mListView->addChild(section);
			}

			// Create and add list item for this country.
			NativeUI::ListViewItem* item = new NativeUI::ListViewItem();
			item->setText(countryName);
			item->setFontColor(COLOR_WHITE);
			item->setSelectionStyle(NativeUI::LIST_VIEW_ITEM_SELECTION_STYLE_GRAY);
			item->setIcon(country->getFlagID());
			section->addItem(item);

			mCountryMap.insert(item->getWidgetHandle(), country->getID());
		}
	}

} // end of EuropeanCountries
