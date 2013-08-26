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

#include "RSSQueryResult.h"
#include "MAFeed.h"

namespace MAUI {

// virtual functions
bool RSSQueryResult::first() {
	pos = 0;
	return true;
}

bool RSSQueryResult::absolute(int n) {
	pos = n;
	return n >= 0 && n < feed->items.size();
}

bool RSSQueryResult::next() {
	printf("RSSQueryResult::next(), pos = %d, feed = %x\n", pos, feed);
	printf("items->size() : %d\n", feed->items.size());
	pos++;
	if(!feed) return false;
	return pos >= 0 && pos < feed->items.size();
}

bool RSSQueryResult::previous() {
	pos--;
	if(!feed) return false;
	return pos >= 0 && pos < feed->items.size();
}

const String& RSSQueryResult::getString(const String& columnName) {
	static String retStr;

	if(feed == NULL) {
		retStr = "error";
		return retStr;
	}
	
	if(queryType == RSSQ_TITLE) {
		if(pos == 0 && columnName=="title")
			retStr = feed->title;
		else if(pos == 0 && columnName=="link")
			retStr = feed->link;
		else if(pos == 0 && columnName=="description")
			retStr = feed->description;
		else 
			retStr = String();
	} 
	else if(queryType == RSSQ_ITEMS) {
		if(pos < feed->items.size()) {
			MAFeedItem* item = (MAFeedItem*)feed->items[pos];
			if(columnName=="title")
				retStr = item->title;
			else if(columnName=="description")
				retStr = item->description;
			else if(columnName=="link")
				retStr = item->link;
		} else {
			retStr = String();
		}
	}
	return retStr;
}

int RSSQueryResult::getInt(const String& /*columnName*/) {
	return 0;
}

Handle RSSQueryResult::getImage(const String& /*columnName*/) {
	return 0;
}

int RSSQueryResult::getColumnCount() {
	if(queryType == RSSQ_TITLE) {
		return 1;
	} else {
		return 3;
	}
}

QueryResult::QueryType RSSQueryResult::getType(int /*index*/) {
	return QT_STRING;
}

const String& RSSQueryResult::getColumnName(int index) {
	static String retStr;

	if(queryType == RSSQ_TITLE) {
		retStr = index == 0 ? "title" : String();
	} else {
		switch(index) {
			case 0 : retStr =  "title";
			case 1 : retStr =  "description";
			case 2 : retStr =  "link";
			default: retStr =  String();
		}
	}

	return retStr;
}

int RSSQueryResult::getColumnIndex(const String& columnName) {
	if(queryType == RSSQ_TITLE) {
		if(columnName=="title") {
			return 0;
		}
		else {
			return -1;
		}

	} else {
		if(columnName=="title") {
			return 0;
		}
		else if(columnName=="description") {
			return 1;
		}
		else if(columnName=="link") {
			return 2;
		}
		else
		{
			return -1;
		}
	}
}

// constructor
RSSQueryResult::RSSQueryResult(const String& query, MAFeedChannel* feed) {
	this->feed = feed;
	if(query=="title")
		queryType = RSSQ_TITLE;
	else if (query=="items")
		queryType = RSSQ_ITEMS;
	else
		queryType = RSSQ_TITLE;
	pos = 0;
}

}