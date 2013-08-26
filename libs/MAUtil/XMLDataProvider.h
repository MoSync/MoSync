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

/** \file XMLDataProvider.h
* \brief XML Data Provider
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
