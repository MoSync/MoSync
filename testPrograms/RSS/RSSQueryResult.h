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
