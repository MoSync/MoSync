/*
Copyright (C) 2013 MoSync AB

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
