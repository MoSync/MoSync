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

#ifndef _SE_MSAB_MAUTIL_XML_DATA_PROVIDER_H_
#define _SE_MSAB_MAUTIL_XML_DATA_PROVIDER_H_

#include "DomParser.h"
#include "XPathTokenizer.h"

using namespace MAUtil::Dom;
using namespace MAUtil::XPath;

namespace MAUtil {

	class XMLDataProvider;
	class XMLDataProviderListener {
	public:
		virtual void onUpdate(XMLDataProvider *dataProvider) = 0;
	};

	class XMLDataProvider {
	public:
		XMLDataProvider();
		XMLDataProvider(Node *node, const String& path);

		void addListener(XMLDataProviderListener* listener);
		void removeListener(XMLDataProviderListener* listener);

		bool setNode(Node *node);
		bool setPath(const String& path);
		const Vector<Node*>& getData() const;

	protected:
		XPathExpression exp;
		Node *node;
		bool isCompiled;

		Vector<Node*> data;
		Vector<XMLDataProviderListener*> listeners;
	};

	class XMLDocDataProvider : public XMLDataProvider, DocumentListener {
	public:	
		XMLDocDataProvider();
		XMLDocDataProvider(Document *doc, const String& path);

		void setDocument(Document *doc);
		void onUpdate(Document *doc);

	private:
		Document *doc;
	};
} // namespace MAUtil

#endif /* _SE_MSAB_MAUTIL_XML_DATA_PROVIDER_H_ */
