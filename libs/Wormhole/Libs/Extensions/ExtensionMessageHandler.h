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
#include <mavsprintf.h>
#if defined(MOSYNC_NATIVE) || defined(NEWLIB)
#include <ctype.h>
#else
#include <mactype.h>
#endif
#include "../../MessageStream.h"

using namespace NativeUI; // WebView widget
using namespace std;

#define MAX_ARRAY_SIZE_MASK 0xffff;

namespace Wormhole
{
	inline const char* getNext(MessageStream& stream, const char* w = NULL);
	inline void marshalInt(char* buffer, int value);
	inline void marshalPtr(char* buffer, char* value);
	inline void marshalLong(char* buffer, long long value);
	inline void marshalFloat(char* buffer, float value);
	inline void marshalDouble(char* buffer, double value);
	inline int unmarshalInt(char* buffer);
	inline char* unmarshalPtr(char* buffer);
	inline long long unmarshalLong(char* buffer);
	inline float unmarshalFloat(char* buffer);
	inline double unmarshalDouble(char* buffer);
	inline char* pad(char* ptr);

	class Allocator {
	private:
		char* mStack;
		int mOffset;
		MAUtil::Vector<char*>* mAllocations;
	public:
		/**
		 * The stack allocated, initial buffer. Expected to be
		 * exactly 256 bytes.
		 */
		Allocator(char* stack) : mStack(stack), mOffset(0), mAllocations(NULL) { }
		virtual ~Allocator();
		/**
		 * Request memory.
		 */
		char* request(int size);
	};

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
		virtual void marshal(MessageStream& stream, char* buffer, Allocator* allocator);
		virtual void unmarshal(char* buffer, MAUtil::String& jsExpr);
		void unmarshalArgList(char* buffer, MAUtil::String& jsExpr);
		virtual bool hasOutValues() { return mHasOutValues; }
		void setName(MAUtil::String name) { mName = name; }
		virtual int getBufferSize(bool aligned);
		int getChildCount();
		virtual const char* getType() { return mIsArgList ? "args" : "struct"; }
	};

	class JSArrayMarshaller : public JSMarshaller {
	private:
		JSMarshaller* mDelegate;
	public:
		JSArrayMarshaller(JSMarshaller* delegate) : JSMarshaller(), mDelegate(delegate) { };
		virtual void marshal(MessageStream& stream, char* buffer, Allocator* allocator);
		virtual void unmarshal(char* buffer, MAUtil::String& jsExpr);
		virtual bool hasOutValues();
		virtual int getBufferSize(bool aligned);
		virtual ~JSArrayMarshaller();
		virtual const char* getType() { return (MAUtil::String("array: ") + mDelegate->getType()).c_str(); }
	};

	class JSNumberMarshaller : public JSMarshaller {
	private:
		char mVariant;
	public:
		JSNumberMarshaller(char variant) : JSMarshaller(toupper(variant) != variant), mVariant(toupper(variant)) { };
		virtual ~JSNumberMarshaller() { };
		virtual void marshal(MessageStream& stream, char* buffer, Allocator* allocator);
		virtual void unmarshal(char* buffer, MAUtil::String& jsExpr);
		virtual int getBufferSize(bool aligned);
		virtual const char* getType() { return (MAUtil::String("number: ") + mVariant).c_str(); }
	};

	class JSStringMarshaller : public JSMarshaller {
	public:
		JSStringMarshaller(bool out) : JSMarshaller(out) { }
		virtual ~JSStringMarshaller() { };
		virtual void marshal(MessageStream& stream, char* buffer, Allocator* allocator);
		virtual void unmarshal(char* buffer, MAUtil::String& jsExpr);
		virtual int getBufferSize(bool aligned);
		virtual const char* getType() { return "string"; }
	};

	class JSIndirectMarshaller : public JSMarshaller {
	private:
		JSMarshaller* mDelegate;
	public:
		JSIndirectMarshaller(JSMarshaller* delegate) : JSMarshaller(delegate->hasOutValues()), mDelegate(delegate) { }
		~JSIndirectMarshaller();
		virtual void marshal(MessageStream& stream, char* buffer, Allocator* allocator);
		virtual void unmarshal(char* buffer, MAUtil::String& jsExpr);
		virtual int getBufferSize(bool aligned);
		virtual const char* getType() { return (MAUtil::String("*") + mDelegate->getType()).c_str(); }
	};

	class JSRefMarshaller : public JSMarshaller {
	private:
		int mRefId;
		MAUtil::Vector<JSMarshaller*>* mRefList;
	public:
		JSRefMarshaller(int refId, MAUtil::Vector<JSMarshaller*>* refList, bool out) : JSMarshaller(out), mRefId(refId), mRefList(refList) { };
		virtual ~JSRefMarshaller() { };
		virtual void marshal(MessageStream& stream, char* buffer, Allocator* allocator);
		virtual void unmarshal(char* buffer, MAUtil::String& jsExpr);
		virtual int getBufferSize(bool aligned);
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
