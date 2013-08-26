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
