/* Copyright (C) 2009 Mobile Sorcery AB

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
