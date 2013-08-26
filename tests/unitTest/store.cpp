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

#include <mastring.h>
#include <conprint.h>
#include "common.h"
#include "MAHeaders.h"

class StoreCase : public TestCase {
public:
	StoreCase() : TestCase("store") {}

#define TI(func) { int res = (func); if(res <= 0) { printf("Error %i\n", res);\
	assert(name, false); suite->runNextCase(); return; } }

	//TestCase
	virtual void start() {
		printf("Testing stores.\n");
		//open, cin
		MAHandle store = maOpenStore("test.store", MAS_CREATE_IF_NECESSARY);
		TI(store);

		//write
		TI(maWriteStore(store, CLIENT_DATA));

		//close
		maCloseStore(store, 0);

		//open
		store = maOpenStore("test.store", 0);
		TI(store);

		//read
		MAHandle data = maCreatePlaceholder();
		maReadStore(store, data);

		//compare
		char storeBuf[DATA_SIZE], clientBuf[DATA_SIZE];
		maReadData(data, storeBuf, 0, DATA_SIZE);
		maReadData(CLIENT_DATA, clientBuf, 0, DATA_SIZE);
		if(memcmp(storeBuf, clientBuf, DATA_SIZE) != 0) {
			assert(name, false);
			suite->runNextCase();
			return;
		}

		//close, delete
		maCloseStore(store, true);
		maDestroyObject(data);

		//open, should fail
		store = maOpenStore("test.store", 0);
		if(store >= 0) {
			maCloseStore(store, true);
			assert(name, false);
			suite->runNextCase();
			return;
		}

		printf("Store test succeded!\n");

		//succeed
		assert(name, true);
		suite->runNextCase();
	}
};

void addStoreTests(TestSuite* suite);
void addStoreTests(TestSuite* suite) {
	suite->addTestCase(new StoreCase);
}
