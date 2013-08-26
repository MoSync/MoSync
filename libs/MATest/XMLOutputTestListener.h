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

/*
* @file XMLOutputTestListener.h
* @brief Test listener that saves test result as XML.
*/

#ifndef __MOSYNC_LIBS_MATEST_XML_OUTPUT_TEST_LISTENER_H__
#define __MOSYNC_LIBS_MATEST_XML_OUTPUT_TEST_LISTENER_H__

#include <ma.h>
#include <MAUtil/Vector.h>
#include <MAUtil/String.h>
#include "Test.h"

namespace MATest
{
	/**
	* @brief Implementation of a TestListener that ouputs
	* test resuls as XML to data store.
	*/
	class XMLOutputTestListener : public TestListener
	{
	private:
		MAUtil::String xmlOutput;
		MAUtil::String storeName;
		MAHandle placeHolder;

	public:
		XMLOutputTestListener(
			MAHandle placeholder,
			const MAUtil::String& storeName);
		~XMLOutputTestListener();
		void beginXML();
		void endXML();
		void beginTestSuite(const MAUtil::String &str);
		void endTestSuite();
		void beginTestCase(const MAUtil::String &str);
		void endTestCase();
		void assertion(const MAUtil::String &str, bool success);
	};
} // namespace

#endif
