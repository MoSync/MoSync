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

/** \file DomParser.h
*
* XML DOM parser
*
* \author Patrick Broman and Niklas Nummelin
*
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
