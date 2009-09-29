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