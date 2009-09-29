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

#include <MAUtil/HashMap.h>
#include <conprint.h>
#include <mastdlib.h>
#include <MAUtil/String.h>

using namespace MAUtil;

#define MAX_INSERTIONS 8
#define MAX_REMOVAL_INTERVAL 2

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	printf("Hello World!\n");

	{
		HashMap<int, int> map;

		int keyList[MAX_INSERTIONS];
		int insertedKeys[MAX_INSERTIONS];
		int numInsertedKeys = 0;
		for(int i = 0; i < MAX_INSERTIONS; i++) {
			keyList[i] = rand()&0xffff;
		}

		int numInsertions = MAX_INSERTIONS;
		int numInsertionsUntilRemoval = rand()%MAX_REMOVAL_INTERVAL;
		for(int i = 0; i < numInsertions; i++) {
			int key = keyList[rand()%MAX_INSERTIONS];
			int value = rand()&0xffff;
			printf("inserted: {%d, %d}\n", key, value);	
			map.insert(key, value);
			insertedKeys[numInsertedKeys++] = key;
			numInsertionsUntilRemoval--;
			if(numInsertionsUntilRemoval<0) {
				HashMap<int, int>::Iterator iter = map.find(insertedKeys[rand()%numInsertedKeys]);
				if(iter != map.end()) {
					printf("removed: {%d, %d}\n", iter->key, iter->value);	
					map.erase(iter);
				}
				numInsertionsUntilRemoval = rand()%MAX_REMOVAL_INTERVAL;
			}
		}
	}
	{

	SelfHashMap<String, int> map;

	String keyList[MAX_INSERTIONS];
	int insertedKeys[MAX_INSERTIONS];
	int numInsertedKeys = 0;
	for(int i = 0; i < MAX_INSERTIONS; i++) {
		char temp[128];
		sprintf(temp, "%c%c%c%c%c", rand()&0xff, rand()&0xff, rand()&0xff, rand()&0xff, rand()&0xff );
		keyList[i] = String(temp);
	}

	int numInsertions = MAX_INSERTIONS;
	int numInsertionsUntilRemoval = rand()%MAX_REMOVAL_INTERVAL;
	for(int i = 0; i < numInsertions; i++) {
		int index = rand()%MAX_INSERTIONS;
		String *key = &keyList[index];
		int value = rand()&0xffff;
		printf("inserted: {%s, %d}\n", key->c_str(), value);	
		map.insert(*key, value);
		insertedKeys[numInsertedKeys++] = index;
		numInsertionsUntilRemoval--;
		if(numInsertionsUntilRemoval<0) {
			SelfHashMap<String, int>::Iterator iter = map.find(keyList[insertedKeys[rand()%numInsertedKeys]]);
			if(iter != map.end()) {
				printf("removed: {%s, %d}\n", iter->key.c_str(), iter->value);	
				map.erase(iter);
			}
			numInsertionsUntilRemoval = rand()%MAX_REMOVAL_INTERVAL;
		}
	}

	}
	/*
	HashMap<int, int>::Iterator itr = map.find(3);
	MYASSERT(itr == map.end());
	itr = map.find(2);
	MYASSERT(itr != map.end());
	MYASSERT(itr->value == 4);
	*/

	FREEZE;
}
