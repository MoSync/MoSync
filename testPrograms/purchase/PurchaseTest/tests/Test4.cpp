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
 * @file Test4.cpp
 * @author emma
 *
 * @brief Test case 4: Create new purchase object. Call getProductId and verify it.
 *
 */

#include <Purchase/PurchaseManager.h>
#include <MAUtil/util.h>

#include "Test4.h"

namespace PurchaseTest
{
	/**
	 * Constructor.
	 * @param applicationController Will be notified when test's status
	 * changes.
	 */
	Test4::Test4(IApplicationController& applicationController):
		mApplicationController(applicationController)
	{
	}

	/**
	 * Destructor.
	 */
	Test4::~Test4()
	{
		mPurchase->removePurchaseListener(this);
		delete mPurchase;
	}

	/**
	 * Start the test.
	 */
	void Test4::startTest()
	{
		MAUtil::String productType = ProductTypes::getInstance().getProductType1();
		mPurchase = new Purchase(productType, this);
	}

	/**
	 * Get the name of the test.
	 * @return Test's name.
	 */
	MAUtil::String Test4::getTestName() const
	{
		return "Test 4";
	}

	/**
	 * Notifies that the product has been validated by the App Store.
	 * NOTE: On Android there is no validation done at this step, if the
	 * product results to be unavailable, then the application will be later
	 * notified via a requestFailed() callback. The errorCode will equal
	 * #MA_PURCHASE_ERROR_INVALID_PRODUCT.
	 * Platform: iOS and Android.
	 * @param purchase The object that sent the event.
	 */
	void Test4::productValid(const Purchase& purchase)
	{
		if ( purchase.getHandle() == mPurchase->getHandle() )
		{
			if ( mPurchase->getProductId() == ProductTypes::getInstance().getProductType1() )
			{
				mApplicationController.testSucceeded(*this);
			}
			else
			{
				MAUtil::String info = "Incorrect productId for product " +
						MAUtil::integerToString(purchase.getHandle());
				mApplicationController.log(info);
				this->setFailedReason(info);
				mApplicationController.testFailed(*this);
			}
		}
	}

	/**
	 * Notifies that the product is not valid on the App Store.
	 * Platform: iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test4::productInvalid(const Purchase& purchase)
	{
		if ( purchase.getHandle() == mPurchase->getHandle() )
		{
			MAUtil::String info = "Product " +
					MAUtil::integerToString(purchase.getHandle()) + " invalid";
			mApplicationController.log(info);
			this->setFailedReason(info);
			mApplicationController.testFailed(*this);
		}
	}

	/**
	 * Notifies that the transaction has been successfully processed.
	 * The user should receive the purchased product.
	 * Platform: Android and iOS.
	 * @param purchase The object that sent the event.
	 */
	void Test4::requestCompleted(const Purchase& purchase)
	{
	}

}
