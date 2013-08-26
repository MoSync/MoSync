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
