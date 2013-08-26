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
 * @file MainScreen.cpp
 * Class that creates a screen.
 * The screen consists of two main parts:
 *  - "Buy" section with a buy button and a list of available items for sale.
 *  - "History" section with a list of purchased items.
 *  For each purchased item there is a Receipt button.
 *  When the Receipt button is pressed a dialog shows the receipt details.
 *  @author Emma Tresanszki
 */

#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <mastdlib.h>
#include <matime.h>

#include <Purchase/Purchase.h>

#include "../Logic/Util.h"
#include "../Logic/Config.h"
#include "../Database/DatabaseManager.h"
#include "../Database/DatabaseProduct.h"
#include "../Logic/ApplicationController.h"
#include "MainScreen.h"

#define BREAKLINE_HEIGHT 10
#define RECEIPT_DIALOG_TITLE "Receipt"

/**
 * Constructor.
 */
MainScreen::MainScreen(ApplicationController* appController) :
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
	mReceiptBid(NULL),
	mReceiptOkButton(NULL),
	mApplicationController(appController)
{
	createProductIdList();
	createMainLayout();
	createReceiptDialog();
	mBuyButton->addButtonListener(this);
	mReceiptOkButton->addButtonListener(this);
	mRestoreProducts->addButtonListener(this);
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
	mRestoreProducts->removeButtonListener(this);
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
	else if( mRestoreProducts == button )
	{
		mApplicationController->restoreTransactions();
	}
}

/**
 * Fill the items in the receipt dialog with values.
 */
void MainScreen::fillReceiptDialog(MAUtil::String appID, MAUtil::String productID,
		int transactionDate, MAUtil::String transactionId,
		MAUtil::String BID, double price, MAUtil::String title)
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
	mReceiptPrice->setText(MAUtil::doubleToString(price));
	mReceiptTitle->setText(title);

	int platform = getPlatform();
	if ( platform == ANDROID )
	{
		mReceiptAppId->setText(appID);
	}
	else if ( platform == IOS )
	{
		mReceiptTransactionId->setText(transactionId);
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
	item->setFontColor(ITEMS_COLOR);
	mPurchasedItemsList->addChild(item);

	// Search the productId among the product list,
	// and only display the product name.
	for (int i=0; i < mProductIdList.size(); i++)
	{
		if ( strcmp(mProductIdList[i].c_str(), productId.c_str()) == 0 )
		{
			item->setText(mProductNamesList[i]);
			break;
		}
	}
}

/**
 * Main screen is notified that a purchase was restored.
 * @param productId The product Id.
 */
void MainScreen::productRestored(MAUtil::String productId)
{
	ListViewItem* item = new ListViewItem();
	item->setFontColor(ITEMS_COLOR);
	item->setText("Product " + productId + " was restored.");
	mPurchasedItemsList->addChild(item);
}

/**
 * Main screen is notified that a purchase was refunded.
 * @param productId The product Id.
 */
void MainScreen::productRefunded(MAUtil::String productId)
{
	ListViewItem* item = new ListViewItem();
	item->setFontColor(ITEMS_COLOR);
	item->setText("Product " + productId + " was refunded.");
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
	mReceiptDialog->setTitle(RECEIPT_DIALOG_TITLE);
	VerticalLayout* dialogLayout = new VerticalLayout();
	mReceiptDialog->setMainWidget(dialogLayout);

	Label* receiptProductIdInfo = new Label("Product ID");
	receiptProductIdInfo->setFontColor(RECEIPT_FIELD_COLOR);
	mReceiptProductId = new Label();
	Label* receiptAppId = new Label("App ID");
	receiptAppId->setFontColor(RECEIPT_FIELD_COLOR);
	mReceiptAppId = new Label();
	Label* receiptTransactionDate = new Label("Transaction date");
	receiptTransactionDate->setFontColor(RECEIPT_FIELD_COLOR);
	mReceiptTransactionDate = new Label();

	Label* receiptProductTitle = new Label("Title");
	receiptProductTitle->setFontColor(RECEIPT_FIELD_COLOR);
	mReceiptTitle = new Label();
	Label* receiptProductPrice = new Label("Price");
	receiptProductPrice->setFontColor(RECEIPT_FIELD_COLOR);
	mReceiptPrice = new Label();

	dialogLayout->addChild(receiptProductIdInfo);
	dialogLayout->addChild(mReceiptProductId);
	dialogLayout->addChild(receiptTransactionDate);
	dialogLayout->addChild(mReceiptTransactionDate);
	dialogLayout->addChild(receiptProductTitle);
	dialogLayout->addChild(mReceiptTitle);
	dialogLayout->addChild(receiptProductPrice);
	dialogLayout->addChild(mReceiptPrice);

	int platform = getPlatform();
	if ( platform == ANDROID)
	{
		dialogLayout->addChild(receiptAppId);
		dialogLayout->addChild(mReceiptAppId);
	}
	else if ( platform == IOS )
	{
		Label* receiptTransactionId = new Label("Transaction ID");
		receiptTransactionId->setFontColor(RECEIPT_FIELD_COLOR);
		mReceiptTransactionId = new Label();
		Label* receiptBid = new Label("BID");
		receiptBid->setFontColor(RECEIPT_FIELD_COLOR);
		mReceiptBid = new Label();
		dialogLayout->addChild(receiptTransactionId);
		dialogLayout->addChild(mReceiptTransactionId);
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
		mProductIdList.add(sGooglePlayPurchasedProductId);
		mProductNamesList.add("Test product");

		mProductIdList.add(ANDROID_MANAGED_ITEM_1);
		mProductNamesList.add("Managed product 1");

		mProductIdList.add(ANDROID_MANAGED_ITEM_2);
		mProductNamesList.add("Managed product 2");

		mProductIdList.add(ANDROID_MANAGED_ITEM_3);
		mProductNamesList.add("Managed product 3");
	}
	else
	{
		mProductIdList.add(IOS_PRODUCT_TYPE_1);
		mProductNamesList.add("Consumable product");
		mProductIdList.add(IOS_PRODUCT_TYPE_2);
		mProductNamesList.add("Non-consumable product");
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

	HorizontalLayout* restoreLayout = new HorizontalLayout();
	restoreLayout->setBackgroundColor(INFO_BACKGROUND_COLOR);
	restoreLayout->wrapContentVertically();
	mRestoreProducts = new Button();
	mRestoreProducts->setText("Restore previous purchases. ONLY when running the first time after uninstall");
	restoreLayout->addChild(mRestoreProducts);
	mMainLayout->addChild(restoreLayout);

	// Items you previously purchased.
	mDatabaseItems = new VerticalLayout();
	mDatabaseItems->wrapContentVertically();
	mMainLayout->addChild(mDatabaseItems);
	Label* infoRepository = new Label("Items you have previously purchased");
	infoRepository->setFontColor(INFO_LABELS_COLOR);
	mDatabaseItems->addChild(infoRepository);

	// Add a small break line between the two sections.
	HorizontalLayout* lineBreakLayout = new HorizontalLayout();
	lineBreakLayout->setHeight(BREAKLINE_HEIGHT);
	lineBreakLayout->setBackgroundColor(BREAKLINE_COLOR);
	mMainLayout->addChild(lineBreakLayout);

	// Create the "BUY" section.
	VerticalLayout* buyLayout = new VerticalLayout();
	buyLayout->wrapContentVertically();
	mMainLayout->addChild(buyLayout);
	Label* info = new Label();
	info->setText("Items for sale");
	info->setFontColor(INFO_LABELS_COLOR);
	buyLayout->addChild(info);

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

	// Add the list of available items for sale along with check boxes ( in the BUY section)
	for (int i=0; i < mProductNamesList.size(); i++)
	{
		// Check if the product is already owned, and replace the checkbox with a label instead.
		DatabaseManager& database = mApplicationController->getDatabase();
		DatabaseProduct* dbProduct = database.getRow(mProductIdList[i]);

		if (dbProduct)
		{

			// The item is already owned, just display it.
			Label* ownedItem = new Label(mProductIdList[i]);
			ownedItem->setFontColor(ITEMS_COLOR);
			mDatabaseItems->addChild(ownedItem);
		}
		else
		{
			HorizontalLayout* itemLayout = new HorizontalLayout();
			itemLayout->wrapContentVertically();
			CheckBox* itemCheckBox = new CheckBox();
			itemLayout->addChild(itemCheckBox);
			mItemsCheckBoxes.add(itemCheckBox);
			Label* itemId = new Label();
			itemId->setText(mProductNamesList[i]);
			itemId->setFontColor(ITEMS_COLOR);
			itemLayout->addChild(itemId);
			buyLayout->addChild(itemLayout);
		}
	}
}
