/*
Copyright (C) 2013 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/*! \addtogroup WormHoleGroup
 *  @{
 */

/** @defgroup WormHoleGroup Wormhole Library
 *  @{
 */

/**
 * @file ExtensionMessageHandler.h
 * @author Mattias Bybro
 *
 * Handles extension invocations
 */

#ifndef WORMHOLE_LIBS_EXTENSIONS_EXTENSIONMESSAGEHANDLER_H_
#define WORMHOLE_LIBS_EXTENSIONS_EXTENSIONMESSAGEHANDLER_H_

#include <NativeUI/WebView.h>
#include <MAUtil/Vector.h>
#include <MAUtil/HashMap.h>
#include <MAUtil/String.h>
#include "../../MessageStream.h"

using namespace NativeUI; // WebView widget
using namespace std;

#define MAX_ARRAY_SIZE_MASK 0xffff;

namespace Wormhole
{
	inline const char* getNext(MessageStream& stream, const char* w = NULL);

	// TODO: Create abstract base class.
	class JSMarshaller {
	private:
		JSMarshaller* createMarshaller(const char* def, bool isArgList);
		MAUtil::Vector<JSMarshaller*> mList;
		MAUtil::Vector<JSMarshaller*> mRefList;
		bool mIsArgList;
		bool mHasReturnValue;
		MAUtil::String mName;
	protected:
		bool mHasOutValues;
		JSMarshaller(bool pHasOutValues) : mIsArgList(false), mHasReturnValue(false), mHasOutValues(pHasOutValues) { };
	public:
		JSMarshaller() : mIsArgList(false), mHasReturnValue(false), mHasOutValues(false) { };
		virtual ~JSMarshaller();
		void initialize(MessageStream& stream);
		virtual int marshal(MessageStream& stream, char* buffer);
		virtual int unmarshal(char* buffer, MAUtil::String& jsExpr);
		int unmarshalArgList(char* buffer, MAUtil::String& jsExpr);
		virtual bool hasOutValues() { return mHasOutValues; }
		void setName(MAUtil::String name) { mName = name; }
		virtual int getDataOffset() { return 0; }
		int getChildCount();
		virtual const char* getType() { return mIsArgList ? "args" : "struct"; }
	};

	class JSArrayMarshaller : public JSMarshaller {
	private:
		JSMarshaller* mDelegate;
	public:
		JSArrayMarshaller(JSMarshaller* delegate) : JSMarshaller(), mDelegate(delegate) { };
		virtual int marshal(MessageStream& stream, char* buffer);
		virtual int unmarshal(char* buffer, MAUtil::String& jsExpr);
		virtual bool hasOutValues();
		virtual int getDataOffset();
		virtual ~JSArrayMarshaller();
		virtual const char* getType() { return (MAUtil::String("array: ") + mDelegate->getType()).c_str(); }
	};

	class JSNumberMarshaller : public JSMarshaller {
	private:
		char mVariant;
	public:
		JSNumberMarshaller(char variant) : JSMarshaller(toupper(variant) != variant), mVariant(toupper(variant)) { };
		virtual ~JSNumberMarshaller() { };
		virtual int marshal(MessageStream& stream, char* buffer);
		virtual int unmarshal(char* buffer, MAUtil::String& jsExpr);
		virtual const char* getType() { return (MAUtil::String("number: ") + mVariant).c_str(); }
	};

	class JSStringMarshaller : public JSMarshaller {
	public:
		JSStringMarshaller(bool out) : JSMarshaller(out) { }
		virtual ~JSStringMarshaller() { };
		virtual int getDataOffset();
		virtual int marshal(MessageStream& stream, char* buffer);
		virtual int unmarshal(char* buffer, MAUtil::String& jsExpr);
		virtual const char* getType() { return "string"; }
	};

	class JSRefMarshaller : public JSMarshaller {
	private:
		int mRefId;
		MAUtil::Vector<JSMarshaller*>* mRefList;
	public:
		JSRefMarshaller(int refId, MAUtil::Vector<JSMarshaller*>* refList, bool out) : JSMarshaller(out), mRefId(refId), mRefList(refList) { };
		virtual ~JSRefMarshaller() { };
		virtual int marshal(MessageStream& stream, char* buffer);
		virtual int unmarshal(char* buffer, MAUtil::String& jsExpr);
		virtual int getDataOffset();
		virtual const char* getType() { return (MAUtil::String("ref") + MAUtil::integerToString(mRefId)).c_str(); }
	};

	class JSExtensionModule {
	public:
		JSExtensionModule(MAUtil::String extName, int hash) : mExtName(extName), mHash(hash) { };
		virtual ~JSExtensionModule();
		JSMarshaller* getMarshaller(int fnId);
		JSMarshaller* setMarshaller(int fnId, JSMarshaller* marshaller);
		MAExtensionFunction getExtensionFunction(int fnId);
	private:
		MAUtil::HashMap<int, JSMarshaller*> mMarshallers;
		MAUtil::String mExtName;
		int mHash;
	};

	class ExtensionMessageHandler {
	private:
		WebView* mWebView;
		MAUtil::HashMap<MAUtil::String, JSExtensionModule*> mModules;
	public:
		ExtensionMessageHandler(WebView* webView) : mWebView(webView) { };
		virtual ~ExtensionMessageHandler() { };
		bool handleMessage(MessageStream& stream);
	};
}

#endif /* WORMHOLE_LIBS_EXTENSIONS_EXTENSIONMESSAGEHANDLER_H_ */
