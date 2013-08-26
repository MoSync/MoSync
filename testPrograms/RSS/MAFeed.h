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

#ifndef _SE_MSAB_MAFEED_H_
#define _SE_MSAB_MAFEED_H_

#include <maapi.h>
#include <MAUtil/Vector.h>

using namespace MAUtil;

typedef struct MAFeedItem_t {
	char *title;
	char *link;
	char *description;
} MAFeedItem;

class MAFeedChannel {
public:
	char *title;
	char *link;
	char *description;
	char *language;
	char *copyright;
	Vector<MAFeedItem*> items;
};

MAFeedItem *MAFeedItem_create();
void MAFeedITem_destroy(MAFeedItem *item);

MAFeedChannel *MAFeedChannel_create();
void MAFeedChannel_destroy(MAFeedChannel *c);
void MAFeedCHannel_add(MAFeedChannel *c, MAFeedItem* item); 


MAFeedChannel *MAFeedChannel_parse(Handle data);
MAFeedChannel *MAFeedChannel_parseURL(const char* url);

#endif
