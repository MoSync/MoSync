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
 * @file Test1.cpp
 * @author Bogdan Iusco
 * @date 9 May 2012
 *
 * @brief Test a valid purchase of an product.
 */

#include "Test1.h"

namespace PurchaseTest
{
	/**
	 * Constructor.
	 * @param applicationController Will be notified when test's status
	 * changes.
	 */
	Test1::Test1(IApplicationController& applicationController):
		mApplicationController(applicationController)
	{

	}

	/**
	 * Destructor.
	 */
	Test1::~Test1()
	{

	}

	/**
	 * Start the test.
	 */
	void Test1::startTest()
	{

	}

	/**
	 * Get the name of the test.
	 * @return Test's name.
	 */
	MAUtil::String Test1::getTestName() const
	{
		return "";
	}

	/**
	 * Get the reason why the test failed.
	 * @return Reason why it failed.
	 */
	MAUtil::String Test1::getReason()
	{
		return "";
	}

}
