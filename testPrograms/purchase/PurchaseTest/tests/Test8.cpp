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
 * @file Test8.cpp
 * @author Bogdan Iusco
 * @date 15 May 2012
 *
 * @brief Create two products using the same product handle.
 * Expected result: the second product should receive an purchase
 * event with MA_PURCHASE_STATE_DUPLICATE_HANDLE as state.
 */

#define TEST_NAME "Test 8"

#include <conprint.h>
#include <maapi.h>

#include "Test8.h"
#include "../Util.h"

namespace PurchaseTest
{

	/**
	 * Constructor.
	 * @param applicationController Will be notified when test's status
	 * changes.
	 */
	Test8::Test8(IApplicationController& applicationController):
		mApplicationController(applicationController),
		mFirstProduct(1),
		mSecondProduct(mFirstProduct),
		mIsFirstProductCreated(false)
	{

	}

	/**
	 * Destructor.
	 */
	Test8::~Test8()
	{
		Environment::getEnvironment().removeCustomEventListener(this);
	}

	/**
	 * Start the test.
	 */
	void Test8::startTest()
	{
		String productType = ProductTypes::getInstance().getProductType1();
		Environment::getEnvironment().addCustomEventListener(this);
		maPurchaseCreate(mFirstProduct, productType.c_str());
	}

	/**
	 * Get the name of the test.
	 * @return Test's name.
	 */
	MAUtil::String Test8::getTestName() const
	{
		return TEST_NAME;
	}

	/**
	 * Handle a new event.
	 * @param event The new event.
	 */
	void Test8::customEvent(const MAEvent& event)
	{
		if (event.type == EVENT_TYPE_PURCHASE)
		{
			MAPurchaseEventData purchaseData = event.purchaseData;

			// Check if first product was created.
			if (!mIsFirstProductCreated)
			{
				// Create second product.
				mIsFirstProductCreated = true;
				String productType = ProductTypes::getInstance().getProductType1();
				maPurchaseCreate(mSecondProduct, productType.c_str());
				return;
			}

			if (purchaseData.state == MA_PURCHASE_STATE_DUPLICATE_HANDLE)
			{
				mApplicationController.testSucceeded(*this);
			}
			else
			{
				this->setFailedReason(
					"Did not receive MA_PURCHASE_STATE_DUPLICATE_HANDLE");
				mApplicationController.testFailed(*this);
			}
		}
	}

}
