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

#ifndef _SE_MSAB_RSSQUERYRESULT_H_
#define _SE_MSAB_RSSQUERYRESULT_H_

#include <MAUI/QueryResult.h>
#include "mafeed.h"

namespace MAUI {
	class RSSQueryResult : public QueryResult {
	public:
		typedef enum {
			RSSQ_TITLE,
			RSSQ_ITEMS
		} RSSQueryResultType;

		RSSQueryResult(const String& query, MAFeedChannel* feed);

		bool          first();
		bool          absolute(int n);
		bool          next();
		bool          previous();
		const String& getString(const String& columnName);
		int	          getInt(const String& columnName);
		Handle        getImage(const String& columnName);
		int	          getColumnCount();
		QueryType     getType(int column);
		const String& getColumnName(int column);
		int	          getColumnIndex(const String& columnName);

	protected:
		MAFeedChannel* feed;
		int queryType; // must be 
		int pos;

	};
}

#endif
