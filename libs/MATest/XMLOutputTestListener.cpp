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

#include <conprint.h>
#include <matime.h>
#include <mavsprintf.h>
#include "Test.h"
#include "XMLOutputTestListener.h"

#define XML_LOGGING
#ifdef XML_LOGGING
#define XML_LOG lprintfln
#else
#define XML_LOG(...)
#endif

namespace MATest
{
	using namespace MAUtil;

	XMLOutputTestListener::XMLOutputTestListener(
		MAHandle placeholder,
		const String& storeName)
	{
		this->storeName = storeName;
		this->placeHolder = placeholder;

		beginXML();
	}

	XMLOutputTestListener::~XMLOutputTestListener()
	{
		endXML();
	}

	void XMLOutputTestListener::beginXML()
	{
		xmlOutput="";
		xmlOutput+="<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n";
		xmlOutput+="<TestReport time=\"";
		xmlOutput+=sprint_time(maLocalTime());
		xmlOutput+="\">\n";
	}

	void XMLOutputTestListener::endXML()
	{
		XML_LOG("endXML");
		xmlOutput+="</TestReport>\n";
#ifdef XML_LOGGING
		maWriteLog(xmlOutput.c_str(), xmlOutput.length());
#endif
		MAHandle store = maOpenStore(storeName.c_str(), MAS_CREATE_IF_NECESSARY);
		maCreateData(placeHolder, xmlOutput.length());
		maWriteData(placeHolder, xmlOutput.c_str(), 0, xmlOutput.length());
		maWriteStore(store, placeHolder);
		maCloseStore(store, 0);
	}

	void XMLOutputTestListener::beginTestSuite(const String &str)
	{
		XML_LOG("---------------------------------------");
		XML_LOG("test suite: %s", str.c_str());
		XML_LOG("---------------------------------------");

		beginXML();

		xmlOutput+="\t<TestSuite name=\""+str+"\">\n";
	}

	void XMLOutputTestListener::endTestSuite()
	{
		xmlOutput+="\t</TestSuite>\n";

		endXML();
	}

	void XMLOutputTestListener::beginTestCase(const String &str)
	{
		XML_LOG("---------------------------------------");
		XML_LOG("test case: %s", str.c_str());
		XML_LOG("---------------------------------------");

		xmlOutput+="\t\t<TestCase name=\""+str+"\">\n";
	}

	void XMLOutputTestListener::endTestCase()
	{
		xmlOutput+="\t\t</TestCase>\n";
	}

	void XMLOutputTestListener::assertion(const String &str, bool cond)
	{
		String res;
		if (cond)
		{
			XML_LOG("%s, succeeded.", str.c_str());
			res = "true";
		}
		else
		{
			XML_LOG("%s, failed.", str.c_str());
			res = "false";
		}
		xmlOutput+="\t\t\t<Assertion name=\"" + str + "\" result=\"" + res + "\"/>\n";
	}
}
// namespace
