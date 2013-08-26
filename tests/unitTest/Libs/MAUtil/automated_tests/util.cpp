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
		stringSplit(L"f93hnfsoöfa0389=df9a38hjöo=d9q3hdono?fh38=", L"=d", o);
		assert("stringSplitW size", o.size() == 3);
		assert("stringSplitW 0", o[0] == L"f93hnfsoöfa0389");
		assert("stringSplitW 1", o[1] == L"f9a38hjöo");
		assert("stringSplitW 2", o[2] == L"9q3hdono?fh38=");
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
		stringBreak(L"´+asd+380hnonf031d+92j4+´fj4m20f83no´f0239jn", L"+´", o);
		assert("stringBreak size", o.size() == 5);
		assert("stringBreak 0", o[0] == L"asd");
		assert("stringBreak 1", o[1] == L"380hnonf031d");
		assert("stringBreak 2", o[2] == L"92j4");
		assert("stringBreak 3", o[3] == L"fj4m20f83no");
		assert("stringBreak 4", o[4] == L"f0239jn");
	}

};

void addMAUtilUtilTests(MATest::TestSuite* suite);
void addMAUtilUtilTests(MATest::TestSuite* suite) {
	suite->addTestCase(new MAUtilUtilTest);
}
