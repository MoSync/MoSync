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
