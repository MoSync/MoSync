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
 * @file ITest.cpp
 * @author Bogdan Iusco
 * @date 10 May 2012
 *
 * @brief Base class for test classes.
 */

#include "ITest.h"

namespace PurchaseTest
{

	/**
	 * Constructor.
	 */
	ITest::ITest():
		mFailedReason(NULL)
	{

	}

	/**
	 * Destructor.
	 */
	ITest::~ITest()
	{
		delete mFailedReason;
	}

	/**
	 * Get the reason why the test failed.
	 * @return Reason why it failed.
	 * If the test has not finished or if it succeeded the returned string
	 * will be empty.
	 */
	MAUtil::String ITest::getReason()
	{
		if (!mFailedReason)
		{
			return "";
		}
		return *mFailedReason;
	}

	/**
	 * Set the reason why the test failed.
	 * @param reason The reason why it failed.
	 */
	void ITest::setFailedReason(const MAUtil::String& reason)
	{
		delete mFailedReason;
		mFailedReason = new MAUtil::String(reason);
	}
}
