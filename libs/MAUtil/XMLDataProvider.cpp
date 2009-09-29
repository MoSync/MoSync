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

#include "XMLDataProvider.h"

namespace MAUtil {

	XMLDataProvider::XMLDataProvider() : 
		node(NULL), 
		isCompiled(false) {
		//printf("Created XMLDataProvider at %x\n", this);
	}

	XMLDataProvider::XMLDataProvider(Node *node, const String& path) : 
		node(node) {
		//printf("Created XMLDataProvider at %x\n", this);
		setPath(path);
	}

	void XMLDataProvider::addListener(XMLDataProviderListener* listener) {
		for(int i = 0; i < this->listeners.size(); i++) {
			if(listener == listeners[i]) {
				return;
			}
		}
		this->listeners.add(listener);
	}

	void XMLDataProvider::removeListener(XMLDataProviderListener* listener) {
		for(int i = 0; i < this->listeners.size(); i++) {
			if(listener == listeners[i]) {
				listeners.remove(i);
				return;
			}
		}
	}

	bool XMLDataProvider::setNode(Node *node) {
		this->node = node;
		data.clear();
		if(!isCompiled) {
			return false;
		} else {
			if(!exp.execute(node, data)) {
				return false;
			} 
		}

		Vector_each(XMLDataProviderListener*, l, listeners) {
			(*l)->onUpdate(this);
		}

		return true;
	}

	bool XMLDataProvider::setPath(const String& path) {
		data.clear();
		for(int i = 0; i < 20; i++) {
			//printf(".\n");
		}

		if(!exp.compile(path)) {
			//printf("*********** ERROR COMPILING XPATH EXPRESSION !!\n");
			isCompiled = false;
			return false;
		}
		isCompiled = true;
		
		if(!node) {
			//printf("*********** ERROR, NODE IS NULL !!\n");
			return false;
		}
		if(!exp.execute(node, data)) {
			//printf("*********** ERROR EXECUTING XPATH EXPRESSION !!\n");
			return false;
		} 

		//printf("************* CALLING ALL THE XMLDATAPROVIDER LISTENERS!!!!\n");
		
		Vector_each(XMLDataProviderListener*, l, listeners) {
			(*l)->onUpdate(this);
		}

		return true;
	}

	const Vector<Node*>& XMLDataProvider::getData() const {
		return data;
	}

	XMLDocDataProvider::XMLDocDataProvider() : XMLDataProvider(), doc(NULL) {;
	}

	XMLDocDataProvider::XMLDocDataProvider(Document *doc, const String& path) : XMLDataProvider() {
		this->node = (Node*)&doc->getRoot();
		setPath(path); // will compile
		this->doc = doc;
		doc->addListener(this);
	}

	void XMLDocDataProvider::onUpdate(Document *doc) {
		setNode(&doc->getRoot());
	}

	void XMLDocDataProvider::setDocument(Document *doc) {
		//printf("Got a document...\n");
		onUpdate(doc);
		this->doc = doc;
		doc->addListener(this);
	}
	
} // namespace MAUtil
