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
