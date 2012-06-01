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
 * @file MainScreen.cpp
 * Class that creates a screen.
 * The screen consists of two main parts:
 *  - "Buy" section with a buy button and a list of available items for sale.
 *  - "History" section with a list of purchased items.
 *  For each purchased item there is a Receipt button.
 *  When the Receipt button is pressed a dialog/new screen shows the receipt details.
 */

#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <mastdlib.h>
#include <matime.h>

#include "Util.h"
#include "MainScreen.h"

#define BREAKLINE_HEIGHT 10
#define ANDROID_PRODUCT_TYPE_PURCHASED "android.test.purchased"
#define IOS_PRODUCT_TYPE_1 "com.mosync.purchase2.consumable"
#define IOS_PRODUCT_TYPE_2 "com.mosync.purchase2.nonconsumable"

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	Screen(),
	mMainLayout(NULL),
	mBuyButton(NULL),
	mAvailableItemsList(NULL),
	mProductToBuy(-1),
	mPurchasedItemsList(NULL),
	mReceiptDialog(NULL),
	mReceiptProductId(NULL),
	mReceiptAppId(NULL),
	mReceiptTransactionDate(NULL),
	mReceiptTransactionId(NULL),
	mReceiptVersionExternalId(NULL),
	mReceiptBid(NULL),
	mReceiptOkButton(NULL)
{
	createProductIdList();
	createMainLayout();
	createReceiptDialog();
	mBuyButton->addButtonListener(this);
	mReceiptOkButton->addButtonListener(this);
	for (int i=0; i < mItemsCheckBoxes.size(); i++)
	{
		mItemsCheckBoxes[i]->addCheckBoxListener(this);
	}
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
	mBuyButton->removeButtonListener(this);
	mReceiptOkButton->removeButtonListener(this);
	for (int i=0; i < mItemsCheckBoxes.size(); i++)
	{
		mItemsCheckBoxes[i]->removeCheckBoxListener(this);
	}
}

/**
 * This method is called if the touch-up event was inside the
 * bounds of the button.
 * Platform: iOS, Android, Windows Phone.
 * @param button The button object that generated the event.
 */
void MainScreen::buttonClicked(Widget* button)
{
	if ( mReceiptOkButton == button )
	{
		dismissReceiptDialog();
	}
}

/**
 * Fill the items in the receipt dialog with values.
 */
void MainScreen::fillReceiptDialog(MAUtil::String appID, MAUtil::String productID,
		int transactionDate, MAUtil::String transactionId,
		MAUtil::String versionExternalId, MAUtil::String BID)
{
	// If the field is unavailable, print a line character instead.
	if ( appID.length() == 0 )
		appID = "-";
	if ( productID.length() == 0 )
		productID = "-";
	if ( transactionDate == 0 )
		mReceiptTransactionDate->setText("-");
	else
	{
		tm transaction;
		split_time(transactionDate, &transaction);
		mReceiptTransactionDate->setText(
				MAUtil::integerToString(transaction.tm_mday) + "." +
				MAUtil::integerToString(transaction.tm_mon +1) + "." +
				MAUtil::integerToString(transaction.tm_year +1900) + " / " +
				MAUtil::integerToString(transaction.tm_hour +1) + ":" +
				MAUtil::integerToString(transaction.tm_min +1) + ":" +
				MAUtil::integerToString(transaction.tm_sec +1) );
	}
	mReceiptProductId->setText(productID);
	mReceiptAppId->setText(appID);
	if ( getPlatform() == IOS )
	{
		mReceiptTransactionId->setText(transactionId);
		mReceiptVersionExternalId->setText(versionExternalId);
		mReceiptBid->setText(BID);
	}

	mReceiptDialog->show();
}

/**
 * Get the index for the selected product to buy.
 * @return The product ID.
 */
MAUtil::String MainScreen::getSelectedAvailableProduct()
{
	/**
	 * Check which check box was selected, and purchase the corresponding item.
	 * If no check box is selected, display a warning message
	 * Until the purchase is completed, Disable buy button.
	 */
	bool noItemSelected = true;
	int itemSelectedIndex = -1;
	for (int i=0 ; i < mItemsCheckBoxes.size(); i++)
	{
		if ( mItemsCheckBoxes[i]->isChecked() )
		{
			noItemSelected = false;
			itemSelectedIndex = i;
			mBuyButton->setEnabled(false);
			break;
		}
	}
	if ( noItemSelected )
	{
		maAlert("Purchase","You need to select an item first!","OK","","");
		return "";
	}

	return mProductIdList[itemSelectedIndex];
}

/**
 * Dismiss receipt dialog when Ok button is pressed.
 */
void MainScreen::dismissReceiptDialog()
{
	mReceiptDialog->hide();
}

/**
 * Main screen is notified that a purchase was completed.
 * Use this method to append to productId of the purchased
 * item to the history section.
 * @param productId the product Id.
 */
void MainScreen::productPurchased(MAUtil::String productId)
{
	// Another purchase can now be requested.
	mBuyButton->setEnabled(true);

	ListViewItem* item = new ListViewItem();
	item->setText(productId);
	item->setFontColor(ITEMS_COLOR);
	mPurchasedItemsList->addChild(item);
}

/**
 * Main screen is notified of a purchase error.
 * @param errorMessage.
 */
void MainScreen::productError(MAUtil::String errorMessage)
{
	// Another purchase can now be requested.
	mBuyButton->setEnabled(true);
	maAlert("Purchase error", errorMessage.c_str(), "OK","","");
}

/**
 * This method is called when the state of the check box was changed
 * by the user.
 * @param checkBox The check box object that generated the event.
 * @param state True if the check box is checked, false otherwise.
 */
void MainScreen::checkBoxStateChanged(
    CheckBox* checkBox,
    bool state)
{
	/**
	 * Check box is used as an alternative to radio buttons,
	 * So when one check box is selected, un-check all the other ones.
	 */
	for (int i=0; i < mItemsCheckBoxes.size(); i++)
	{
		if ( mItemsCheckBoxes[i] == checkBox && state)
		{
			for (int index1=0; index1 < i; index1 ++)
				mItemsCheckBoxes[index1]->setState(false);
			for(int index2=i+1; index2 < mItemsCheckBoxes.size(); index2++)
				mItemsCheckBoxes[index2]->setState(false);
			break;
		}
	}
}

/**
 * Create the dialog that will display receipt fields, on rows.
 */
void MainScreen::createReceiptDialog()
{
	mReceiptDialog = new Dialog();
	VerticalLayout* dialogLayout = new VerticalLayout();
	mReceiptDialog->setMainWidget(dialogLayout);

	HorizontalLayout* titleLayout = new HorizontalLayout();
	titleLayout->wrapContentVertically();
	titleLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);
	Label* title = new Label("Product Receipt");
	title->setFontColor(RECEIPT_FIELD_COLOR);
	titleLayout->addChild(title);
	dialogLayout->addChild(titleLayout);

	Label* receiptProductIdInfo = new Label("Product ID");
	receiptProductIdInfo->setFontColor(RECEIPT_FIELD_COLOR);
	mReceiptProductId = new Label();
	Label* receiptAppId = new Label("App ID");
	receiptAppId->setFontColor(RECEIPT_FIELD_COLOR);
	mReceiptAppId = new Label();
	Label* receiptTransactionDate = new Label("Transaction date");
	receiptTransactionDate->setFontColor(RECEIPT_FIELD_COLOR);
	mReceiptTransactionDate = new Label();

	dialogLayout->addChild(receiptProductIdInfo);
	dialogLayout->addChild(mReceiptProductId);
	dialogLayout->addChild(receiptAppId);
	dialogLayout->addChild(mReceiptAppId);
	dialogLayout->addChild(receiptTransactionDate);
	dialogLayout->addChild(mReceiptTransactionDate);

	if ( getPlatform() == IOS )
	{
		Label* receiptTransactionId = new Label("Transaction ID");
		receiptTransactionId->setFontColor(RECEIPT_FIELD_COLOR);
		mReceiptTransactionId = new Label();
		Label* receiptVersionExternalId = new Label("Version external ID");
		receiptVersionExternalId->setFontColor(RECEIPT_FIELD_COLOR);
		mReceiptVersionExternalId = new Label();
		Label* receiptBid = new Label("BID");
		receiptBid->setFontColor(RECEIPT_FIELD_COLOR);
		mReceiptBid = new Label();
		dialogLayout->addChild(receiptTransactionId);
		dialogLayout->addChild(mReceiptTransactionId);
		dialogLayout->addChild(receiptVersionExternalId);
		dialogLayout->addChild(mReceiptVersionExternalId);
		dialogLayout->addChild(receiptBid);
		dialogLayout->addChild(mReceiptBid);
	}

	mReceiptOkButton = new Button();
	mReceiptOkButton->setText("Ok");

	dialogLayout->addChild(mReceiptOkButton);
}

/**
 * Create the list of available products.
 */
void MainScreen::createProductIdList()
{
	if ( getPlatform() == ANDROID )
	{
		/**
		 * This test product id can be bought multiple times.
		 * If you want to run the example for your own product ids,
		 * add them to the mProductIdList list.
		 */
		mProductIdList.add(ANDROID_PRODUCT_TYPE_PURCHASED);
	}
	else
	{
		mProductIdList.add(IOS_PRODUCT_TYPE_1);
		mProductIdList.add(IOS_PRODUCT_TYPE_2);
	}
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout()
{
	// Create and add the main layout to the screen.
	mMainLayout = new VerticalLayout();
	mMainLayout->setBackgroundColor(MAIN_LAYOUT_COLOR);
	Screen::setMainWidget(mMainLayout);

	// Create the "BUY" section.
	VerticalLayout* buyLayout = new VerticalLayout();
	buyLayout->wrapContentVertically();
	mMainLayout->addChild(buyLayout);
	Label* info = new Label();
	info->setText("Items for sale");
	info->setFontColor(INFO_LABELS_COLOR);
	buyLayout->addChild(info);

	// Add the list of available items for sale along with check boxes.
	for (int i=0; i < mProductIdList.size(); i++)
	{
		HorizontalLayout* itemLayout = new HorizontalLayout();
		itemLayout->wrapContentVertically();
		CheckBox* itemCheckBox = new CheckBox();
		itemLayout->addChild(itemCheckBox);
		mItemsCheckBoxes.add(itemCheckBox);
		Label* itemId = new Label();
		itemId->setText(mProductIdList[i]);
		itemId->setFontColor(ITEMS_COLOR);
		itemLayout->addChild(itemId);
		buyLayout->addChild(itemLayout);
	}

	// Use buy button to purchase the checked product it from the "BUY" section.
	mBuyButton = new Button();
	mBuyButton->setText("BUY");
	mMainLayout->addChild(mBuyButton);

	// Add a small break line between the two sections.
	HorizontalLayout* lineLayout = new HorizontalLayout();
	lineLayout->setHeight(BREAKLINE_HEIGHT);
	lineLayout->setBackgroundColor(BREAKLINE_COLOR);
	mMainLayout->addChild(lineLayout);

	// Create the "HISTORY" section.
	VerticalLayout* purchasedLayout = new VerticalLayout();
	Label* purchasedLabel = new Label();
	purchasedLabel->setText("Items you own");
	purchasedLabel->setFontColor(INFO_LABELS_COLOR);
	purchasedLayout->addChild(purchasedLabel);
	mPurchasedItemsList = new ListView();
	purchasedLayout->addChild(mPurchasedItemsList);
	mMainLayout->addChild(purchasedLayout);

}
