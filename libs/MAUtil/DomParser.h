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

/** 
* \file DomParser.h
* \brief XML DOM parser
* \author Patrick Broman and Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUTIL_DOM_PARSER_H_
#define _SE_MSAB_MAUTIL_DOM_PARSER_H_

#include "ErrorListenable.h"

#include "Vector.h"
#include "String.h"
#include "Map.h"
#include "Stream.h"
#include "XML.h"

namespace MAUtil {
	namespace Dom {
		class Node {
		public:
			enum eType {
				ATTRIBUTE = 1,
				ELEMENT = 2,
				CDATA = 3,
			};

			Node(eType type);
			eType getType() const;

		private:
			eType type;
		};

		class Attribute : public Node {
		public:
			Attribute(const String &name, const String &value);
			~Attribute();

			String&			getName();
			const String&	getName() const;
			String& 		getValue();
			const String& 	getValue() const;

			int 			getValueAsInt() const;
			double 			getValueAsDouble() const;


		private:
			String 			name;
			String 			value;
		};

		class CData : public Node {
		public:
			CData(const String &data);

			void			setCDATA(const String& data);
			String&			getCDATA();
			const String&	getCDATA() const;

		private:
			String			data;
		};


		class Element : public Node {
		public:
			Element(const String &name);
			~Element();

			void						addCDATA(const String& cdata);
			void						addCDATA(CData *cdata);	
			void 						addAttribute(Attribute *a);
			void						addElement(Element *node);
			void						setParent(Element *parent);

			Element*					getParent();
			const Element&				getParent() const;
			String&						getName();
			const String&				getName() const;

			Vector<Node*>&				getChildren();
			const Vector<Node*>&		getChildren() const;
			String						getCDATA();

			Attribute&					getAttribute(const String& name);
			void						getAttributesWithName(const String& name, Vector<Node*>& output);
			void						getElementsWithName(const String& name, Vector<Node*>& output) const;

			bool						getNodesFromPath(const String& path, Vector<Node*>& result);

			String&						operator[](const String &attributeName);

			String						toXML();

		private:
			String						_toXML(int depth);

			String 					name;
			Element					*parent;
			Vector<Node*>			children;
			Map<String, Attribute*> attributes;

		};

		class Document;
		class DomParser {
		public:
			static Document* 	parseToDocument(Handle resource);
			static Document*	parseToDocument(Stream &stream);
			static Document*	parseToDocument(const String& url);

			// returns root node
			static Element* 	parseToElement(Handle resource); 
			static Element*		parseToElement(Stream &stream);	
			static Element*		parseToElement(const String& url); 

			static void initGlobals();
			String toXml();
		private:
			static Document *currentDocument;
			static Element *root;
			static Element *currentNode;

			static BOOL domStartCallback(const char* tag, const AttrMap* argv);
			static BOOL domEndCallback(const char* tag);
			static BOOL domCharDataCallback(const char* data);
		};

		class DocumentListener {
		public:
			virtual void onUpdate(Document *doc) = 0;
		};

		class Document {
		public:
			Document(Element *root);
			virtual ~Document();

			void addListener(DocumentListener *doc);
			void removeListener(DocumentListener *doc);

			void setRoot(Element *root);
			Element& getRoot();
			const Element& getRoot() const;

			String toXML();

			virtual	void update() {
				Vector_each(DocumentListener*, l, listeners) {
					(*l)->onUpdate(this);
				}
			}

		protected:
			Element* root;
			Vector<DocumentListener*> listeners;
		};

		class WebDocument : public Document, ErrorListenable {
		public:
			WebDocument(const String& url) : Document(NULL), url(url) {
				update();
			}

			void setURL(const String& url) {
				this->url = url;
			}

			const String& getURL() const {
				return url;
			}

			void update() {
				root = DomParser::parseToElement(url);
				if(assert(root!=NULL, 0, "could not download and / or parse xml document from web.")) {
					Document::update();
				}
			}

		protected:
			String url;
		};

	}
}

#endif /* _SE_MSAB_MAUTIL_DOM_PARSER_H_ */
