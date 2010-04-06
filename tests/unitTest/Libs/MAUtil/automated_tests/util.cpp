/* Copyright (C) 2010 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#include <MAUtil/util.h>
#include <conprint.h>
#include <wchar.h>

#include "common.h"

#define TEST_CAPACITY 16

static const int srcData[] = {1, 2, 3};

using namespace MAUtil;

class MAUtilUtilTest : public MATest::TestCase {
public:
	MAUtilUtilTest() : MATest::TestCase("MAUtilUtil") {}

	void start() {
		printf("MAUtil/util tests\n");
		printf("-stringSplit\n");
		testStringSplit();
		printf("-stringSplitW\n");
		testStringSplitW();
		printf("-stringBreak\n");
		testStringBreak();
		printf("-stringBreakW\n");
		testStringBreakW();
		suite->runNextCase();
	}

	void testStringSplit() {
		Vector<String> o;
		stringSplit("f93hnfsoöfa0389=df9a38hjöo=d9q3hdono?fh38", "=d", o);
		assert("stringSplit size", o.size() == 3);
		assert("stringSplit 0", o[0] == "f93hnfsoöfa0389");
		assert("stringSplit 1", o[1] == "f9a38hjöo");
		assert("stringSplit 2", o[2] == "9q3hdono?fh38");
	}

	void testStringSplitW() {
		Vector<WString> o;
		stringSplit((wchar*)L"f93hnfsoöfa0389=df9a38hjöo=d9q3hdono?fh38=", (wchar*)L"=d", o);
		assert("stringSplitW size", o.size() == 3);
		assert("stringSplitW 0", o[0] == (wchar*)L"f93hnfsoöfa0389");
		assert("stringSplitW 1", o[1] == (wchar*)L"f9a38hjöo");
		assert("stringSplitW 2", o[2] == (wchar*)L"9q3hdono?fh38=");
	}

	void testStringBreak() {
		Vector<String> o;
		stringBreak("asd+380hnonf031d+92j4+´fj4m20f83no´f0239jn", "+´", o);
		assert("stringBreak size", o.size() == 5);
		assert("stringBreak 0", o[0] == "asd");
		assert("stringBreak 1", o[1] == "380hnonf031d");
		assert("stringBreak 2", o[2] == "92j4");
		assert("stringBreak 3", o[3] == "fj4m20f83no");
		assert("stringBreak 4", o[4] == "f0239jn");
	}

	void testStringBreakW() {
		Vector<WString> o;
		stringBreak((wchar*)L"´+asd+380hnonf031d+92j4+´fj4m20f83no´f0239jn", (wchar*)L"+´", o);
		assert("stringBreak size", o.size() == 5);
		assert("stringBreak 0", o[0] == (wchar*)L"asd");
		assert("stringBreak 1", o[1] == (wchar*)L"380hnonf031d");
		assert("stringBreak 2", o[2] == (wchar*)L"92j4");
		assert("stringBreak 3", o[3] == (wchar*)L"fj4m20f83no");
		assert("stringBreak 4", o[4] == (wchar*)L"f0239jn");
	}

};

void addMAUtilUtilTests(MATest::TestSuite* suite);
void addMAUtilUtilTests(MATest::TestSuite* suite) {
	suite->addTestCase(new MAUtilUtilTest);
}
