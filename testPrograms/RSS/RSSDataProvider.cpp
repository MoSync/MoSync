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

#include "rssdataprovider.h"
#include "rssqueryresult.h"
#include <conprint.h>

namespace MAUI {

QueryResult& RSSDataProvider::query(const String& query)
{
	return *(new RSSQueryResult(query, feed));
}

RSSDataProvider::RSSDataProvider(const String& url)
{
	feed = MAFeedChannel_parseURL(url.c_str());
	if(feed == NULL) {
		printf("COULD NOT GET FEED!!!\n");
	} else {
		printf("GOT FEED!!!\n");

	}
}


bool RSSDataProvider::hasFeed() {
	return feed!=NULL;
}

}