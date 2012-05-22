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
 * @file Test7.cpp
 * @author emma
 *
 * @brief Test case 7:  Test checkPurchaseSupported.
 * Expected result: call returns MA_PURCHASE_RES_OK.
 */

#include <Purchase/PurchaseManager.h>

#include "Test7.h"

namespace PurchaseTest
{
	/**
	 * Constructor.
	 * @param applicationController Will be notified when test's status
	 * changes.
	 */
	Test7::Test7(IApplicationController& applicationController):
		mApplicationController(applicationController)
	{
	}

	/**
	 * Destructor.
	 */
	Test7::~Test7()
	{
	}

	/**
	 * Start the test.
	 */
	void Test7::startTest()
	{
		int result = PurchaseManager::getInstance()->checkPurchaseSupported();
		if ( result == MA_PURCHASE_RES_OK )
		{
			mApplicationController.testSucceeded(*this);
		}
		else
		{
			MAUtil::String info = "Billing is not supported.";
			mApplicationController.log(info);
			this->setFailedReason(info);
			mApplicationController.testFailed(*this);
		}
	}

	/**
	 * Get the name of the test.
	 * @return Test's name.
	 */
	MAUtil::String Test7::getTestName() const
	{
		return "Test 7";
	}

}
