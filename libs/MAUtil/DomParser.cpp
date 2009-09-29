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

#include "DomParser.h"
#include <mastdlib.h>
#include <mastring.h>
#include <madmath.h>
#include <conprint.h>
#include "XPathTokenizer.h"
#include "HttpGet.h"
#include "xml_int.h"

using namespace MAUtil::XPath;

namespace MAUtil {
	namespace Dom {
		Node::Node(eType type) : type(type) {
		}

		Node::eType Node::getType() const {
			return type;
		}

		Attribute::Attribute(const String &name, const String &value) :
		Node(Node::ATTRIBUTE),
			name(name),
			value(value) {
		}

		Attribute::~Attribute() {
		}

		String& Attribute::getName() {
			return name;
		}

		const String& Attribute::getName() const {
			return name;
		}

		int Attribute::getValueAsInt() const {
			return atoi(value.c_str());
		}

		double Attribute::getValueAsDouble() const {
			return atof(value.c_str());
		}

		String& Attribute::getValue() {
			return value;
		}

		const String& Attribute::getValue() const {
			return value;
		}

		CData::CData(const String &data) : Node(Node::CDATA), data(data) {
		}

		void		CData::setCDATA(const String& data) {
			this->data = data;
		}

		String&		CData::getCDATA() {
			return data;
		}

		const String&	CData::getCDATA() const {
			return data;
		}

		Element::Element(const String &name) :
		Node(Node::ELEMENT),
			name(name),
			parent(NULL) {
		}

		Element::~Element() {
			for(int i = 0; i < children.size(); i++) {
				delete children[i];
			}
			attributes.clear();
		}

		void Element::addCDATA(const String& cdata) {
			children.add(new CData(cdata));
		}

		void Element::addCDATA(CData *cdata) {
			children.add(cdata);
		}

		Element* Element::getParent() {
			return parent;
		}

		const Element& Element::getParent() const {
			return *parent;
		}

		void Element::addAttribute(Attribute *a) {
			attributes[a->getName()] = a;
		}

		void Element::addElement(Element *node) {
			children.add(node);
			node->setParent(this);
		}

		void Element::setParent(Element *parent) {
			this->parent = parent;
		}

		String& Element::getName() {
			return name;
		}

		const String& Element::getName() const {
			return name;
		}

		Attribute& Element::getAttribute(const String& name) {
			return *attributes[name];
		}

		void Element::getAttributesWithName(const String& name, Vector<Node*>& output) {
			if(name!="*") {
				Attribute **attrib = attributes.get(name);
				if(attrib != NULL)
					output.add(attributes[name]);
				return;
			}
			const Vector<String>& attributeNames = attributes.getKeySet();
			for(int i = 0; i < attributeNames.size(); i++) {
				output.add(attributes[attributeNames[i]]);
			}
		}

		Vector<Node*>& Element::getChildren() {
			return children;
		}

		const Vector<Node*>& Element::getChildren() const {
			return children;
		}

		void Element::getElementsWithName(const String& name, Vector<Node*>& output) const {
			bool selectAll = false;
			if(name=="*") selectAll = true;
			for(int i = 0; i < children.size(); i++) {
				if(children[i]->getType() == ELEMENT && (selectAll || ((((Element*)children[i])->getName() == name)))) {
					output.add(children[i]);
				}
			}
		}

		String Element::getCDATA() {
			String cdata = "";
			for(int i = 0; i < children.size(); i++) {
				if(children[i]->getType() == CDATA) {
					cdata+= ((CData*)children[i])->getCDATA();
				}
			}
			return cdata;
		}

		String Element::_toXML(int depth) {
			String result = "";
			result += "<" + this->getName() + " ";

			const Vector<String>& keys = attributes.getKeySet();
			for(int i = 0; i < keys.size(); i++) {
				result += keys[i] + "=\"";
				String temp = operator[](keys[i]) + "\" ";
				result += temp;
			}

			if(children.size() == 0) {
				result += "/>";
				return result;
			} else {
				result += ">";

				for(int i = 0; i < children.size(); i++) {
					if(children[i]->getType() == Node::ELEMENT)
						result += ((Element*)children[i])->_toXML(depth + 1);
					else if(children[i]->getType() == Node::CDATA) {
						result += ((CData*)children[i])->getCDATA();
					}
				}

				result += "</" + this->getName() + ">";
			}
			return result;
		}

		String Element::toXML() {
			return _toXML(0);
		}


		bool Element::getNodesFromPath(const String& path, Vector<Node*>& result) {
			XPathExpression exp = XPathExpression(path);
			if(exp.execute(this, result)) {
				return true;
			} else {
				return false;
			}
		}

		String& Element::operator[](const String &attributeName) {
			return attributes[attributeName]->getValue();
		}

		Document::Document(Element *root) : root(root) {
		}

		Document::~Document() {
			if(root != NULL)
				delete root;
		}

		void Document::setRoot(Element *element) {
			if(root != NULL) {
				delete root;
			}
			root = element;
		}

		void Document::addListener(DocumentListener *listener) {
			for(int i = 0; i < this->listeners.size(); i++) {
				if(listener == listeners[i]) {
					return;
				}
			}
			this->listeners.add(listener);		
		}

		void Document::removeListener(DocumentListener *listener) {
			for(int i = 0; i < this->listeners.size(); i++) {
				if(listener == listeners[i]) {
					listeners.remove(i);
					return;
				}
			}
		}


		Element& Document::getRoot() {
			return *root;
		}

		const Element& Document::getRoot() const{
			return *root;
		}

		String Document::toXML() {
			if(root)
				return "XML output:\n" + root->toXML();
			return "";
		}


		Document *DomParser::currentDocument;
		Element *DomParser::root = NULL;
		Element *DomParser::currentNode = NULL;

		void DomParser::initGlobals() {
			root = NULL;
			currentNode = NULL;
		}

		BOOL DomParser::domStartCallback(const char* tag, const AttrMap* argv) {
			if(root == NULL) {
				root = new Element(tag);
				for(int i = 0; i < argv->nAttrs; i++) {
					root->addAttribute(new Attribute(AM_NAME(argv, i), AM_VALUE(argv, i)));		
				}
				currentNode = root;
			} else {
				Element *element = new Element(tag);
				for(int i = 0; i < argv->nAttrs; i++) {
					element->addAttribute(new Attribute(AM_NAME(argv, i), AM_VALUE(argv, i)));		
				}
				currentNode->addElement(element);
				currentNode = element;
			}

			return TRUE;
		}

		BOOL DomParser::domEndCallback(const char* tag) {
			currentNode = currentNode->getParent();
			return TRUE;
		}

		BOOL DomParser::domCharDataCallback(const char* data) {
			currentNode->addCDATA(data);
			return TRUE;
		}

		Document* DomParser::parseToDocument(Handle resource) {
			Element* node = parseToElement(resource);
			if(node) {
				return new Document(node);
			} else {
				return NULL;
			}
		}

		Document* DomParser::parseToDocument(Stream &stream) {
			Element* node = parseToElement(stream);
			if(node) {
				return new Document(node);
			} else {
				return NULL;
			}
		}

		// returns root node
		Element* DomParser::parseToElement(Handle resource) {
			if(!resource) return NULL;
			char buf[64];
			maReadData(resource, buf, 0, 63);
			buf[63] = 0;
			printf("XML data:");
			printf(buf);
			Stream* stream = Stream_create_data(resource);
			if(stream == NULL)
				return NULL;
			Element* node = parseToElement(*stream);
			delete stream;
			if(node == NULL) {
				return NULL;
			}
			return node;
		}

		Element*	DomParser::parseToElement(Stream &stream) {
			initGlobals();
			int result = XML_parseEx(&stream, domStartCallback, domEndCallback, domCharDataCallback, 
				128, 16, 4*1024, 16, FALSE);

			//int result = XML_parse(&stream, domStartCallback, domEndCallback, domCharDataCallback);
			if(result<0) {
				delete root;
				return NULL;
			} else {
				return root;
			}
		}

		Document*	DomParser::parseToDocument(const String& url) {
			Element* node = parseToElement(url);
			if(node) {
				return new Document(node);
			} else {
				return NULL;
			}
		}

		class XMLHTTPGet {
		public:
			XMLHTTPGet::XMLHTTPGet(const char* url, Vector<char>& xmlFile, int blockSize) :
				xmlFile(xmlFile),
				blockSize(blockSize) {
				mConn = maConnect(url);
				mState = mConn >= 0 ? 0 : mConn;
				mPos = 0;
			}

			XMLHTTPGet::~XMLHTTPGet() {
				if(mConn > 0)
					maConnClose(mConn);
			}

			int XMLHTTPGet::state() const {
				return mState;
			}

			void read() {
				int initialPos = xmlFile.size();
				xmlFile.resize(xmlFile.size()+blockSize);
				maConnRead(mConn, &xmlFile[initialPos], blockSize);			
			}

			void XMLHTTPGet::notify(const CONN_EVENT_DATA& ed) {
				if(ed.handle != mConn || ed.result == 0)
					return;
				switch(ed.opType) {
				case CONNOP_CONNECT:
					if(ed.result < 0) {
						mState = ed.result;
						return;
					} else {
						read();
					}
					break;
				case CONNOP_READ:
					if(ed.result < 0) {

						if(ed.result == CONNERR_CLOSED) {
							xmlFile.resize(mPos);
							mState = xmlFile.size();
							return;
						}

						mState = ed.result;
						return;
					}
					mPos += ed.result;
					
					if(ed.result < blockSize) {
						xmlFile.resize(mPos);
					}

					read();

					break;
				default:
					BIG_PHAT_ERROR;
				}
			}
		protected:
			Handle mConn;
			int mPos;
			int mSize;
			int blockSize;
		private:
			int mState;
			Vector<char>& xmlFile;

		};

		Element*	DomParser::parseToElement(const String& url) {
			Vector<char> xmlFile;
			int res = 0;
			{
				XMLHTTPGet http(url.c_str(), xmlFile, 128);
				while(res == 0) {
					EVENT event;
					maWait(0);
					while(maGetEvent(&event)) {
						if(event.type == EVENT_TYPE_CLOSE ||
							(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_0))
						{
							maExit(0);
						} else if(event.type == EVENT_TYPE_CONN) {
							http.notify(event.conn);
							res = http.state();
						}
					}
				}
			}
			if(res < 0) {
				// error;
				return NULL;
			}

			const char* str = xmlFile.pointer();

			/*
		Handle xmlFileHandle = maOpenStore("olle.xml", MAS_CREATE_IF_NECESSARY);
		Handle xmlDataHandle = maCreatePlaceholder();
		maCreateData(xmlDataHandle, xmlFile.size()+1);
		maWriteData(xmlDataHandle, xmlFile.pointer(), 0, xmlFile.size()+1);
		maWriteStore(xmlFileHandle, xmlDataHandle);
		*/

			Stream *stream = Stream_create_memory(xmlFile.pointer(), xmlFile.size());
			return parseToElement(*stream);
		}
	}
}
