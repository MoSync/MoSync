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

/**
* \file MTXml.h
* \brief Tiny XML parser with a SAX-like interface.
*
* MTXml is a simple XML parser which can handle most XML 1.0 and 1.1 documents.
* It has a SAX-like interface, and is re-entrant in that it can take a partial XML document
* and request additional data when needed.
*
* In the interests of performance, MTXml is not a conforming XML processor,
* as defined by the W3C Recommendation. It does not validate documents,
* and it only checks a few of the well-formedness criteria.
* It even ignores some "fatal errors". Still, it should be able to properly parse
* a well-formed document.
*
* It must be fed sufficiently large portions of data to parse at least one Name or Attribute
* in one go.
*
* MTXml is a destructive parser, which means that it modifies the input data during parsing.
* For example, it overwrites some key bytes with null terminators for callback delivery.
* The processing of UTF-8 and entity data replaces the original bytes.
*
* MTXml does not keep any state beyond the bare minimum required for its parsing.
* For example, there is no tag stack. Not even the name of the latest tag is saved.
* Thus, you will probably need to keep your own state variables, different for each
* document type that you parse.
*
* MTXml can output data in either 8-bit Latin-1 or wide-char Unicode.
*
* If you use mtxFeed(), the parser treats all text as Latin-1, passing UTF-8 without decoding.
* You can then use mtxProcess() to convert the interesting parts of the document.
*
* If you use mtxFeedProcess(), the parser will determine if UTF-8 is used and,
* convert all strings reported by MTX callbacks to Latin-1.
* It will also convert standard entity references. (XML and HTML 4.01)
*
* mtxFeedWide() works like mtxFeedProcess(), except the output is converted to Unicode instead
* of Latin-1.
*
* UTF-16 input is not supported.
*
* It should not crash or freeze on any input, but it will give strange data in callbacks
* if fed a document that is not well-formed, or a document it doesn't support.
*
* \see http://www.w3.org/TR/xml11/ - The W3C Specification of XML 1.1 (online)
* \see http://www.w3.org/TR/html401/sgml/entities.html#h-24.2 -
* HTML 4.01 character entity references (online)
*/

#ifndef _MTXML_H_
#define _MTXML_H_

#include <ma.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DOXYGEN
#define CONTEXT_INTERNAL_VARIABLES(m) \
	m(int, State) m(BOOL, Utf8)

#define DECLARE_IVAR(type, name) type i##name;
#endif

typedef struct MTXContext MTXContext;

/**
* \brief The context of an MTXml parser.
*
* Contains function pointers for callbacks from the parser.
* Also contains internal variables.
*
* You must set all the callbacks before calling mtxFeed(), mtxFeedProcess() or mtxFeedWide().
* If you do not, your program will crash when an un-set callback is called.
* They may not be set to NULL.
* An exception is unicodeCharacter(), which will not be called if you call mtxFeedWide().
*
* Some functions have const void* parameters. These parameters will point to
* null-terminated strings whose type is either char or wchar_t,
* depending on which output type is selected.
* You'll need to cast the pointers to the appropriate type.
* The C++ wrapper, Context, does this automatically.
*/
struct MTXContext {
	/**
	* If the "encoding" attribute is present in the "?xml" tag,
	* this function is called.
	*
	* The default encoding is UTF-8.
	*
	* \see http://www.w3.org/TR/xml11/#charencoding
	*/
	void (*encoding)(MTXContext* context, const char* value);

	/**
	* Called when a start-tag is parsed.
	* \param name The tag's name.
	* \param len The length of the name, in characters.
	*/
	void (*tagStart)(MTXContext* context, const void* name, int len);

	//lengths of name and value can be added, if you like.
	/**
	* Called when a tag attribute is parsed.
	*/
	void (*tagAttr)(MTXContext* context, const void* attrName, const void* attrValue);

	/**
	* Called when the end bracket of the most recent start-tag is parsed.
	*/
	void (*tagStartEnd)(MTXContext* context);

	/**
	* Called when character data is parsed.
	*
	* May be called more than once per data section,
	* so don't expect your data to be delivered in one piece.
	*
	* \param data The character data.
	* \param len The length of the data, in characters.
	*/
	void (*tagData)(MTXContext* context, const void* data, int len);

	/**
	* Called when an end-tag is parsed.
	* \param name The tag's name.
	* \param len The length of the name, in characters.
	* \warning Unlike regular SAX parsers, this function is NOT called when the end of an empty-element tag is parsed.
	* This is because MTXml, in the interests of performance, does not keep any copies of document data.
	* emptyTagEnd() is called instead.
	*/
	void (*tagEnd)(MTXContext* context, const void* name, int len);

	/**
	* Called at the end of an empty-element tag.
	* This marks the end of the tag most recently reported by tagStart().
	*/
	void (*emptyTagEnd)(MTXContext* context);

	/**
	* Called when some data at the end of the supplied buffer couldn't be completely parsed.
	* Prepend it to your next feed.
	* \param data The XML data. May point to the beginning of the buffer that was supplied to feed().
	* In that case, the data needn't be moved.
	* \param len The length of the data, in bytes.
	* \see mtxFeed()
	*/
	void (*dataRemains)(MTXContext* context, const char* data, int len);

	/**
	* Called when the parser detects an error.
	*/
	void (*parseError)(MTXContext* context);

	/**
	* Called during UTF-8 processing, if Latin-1 output is selected,
	* when a character, whose encoding is longer than 8 bits, is encountered.
	* In other words, its value is greater than 255(0xFF).
	* This function should return a Latin-1 representation of that Unicode character.
	*
	* This function may return zero. If so, a parse error is generated.
	*
	* A simple implementation is available: mtxBasicUnicodeConvert().
	*/
	unsigned char (*unicodeCharacter)(MTXContext* context, int unicode);

	/**
	* A value you can set to anything you like.
	* If you're parsing more than one file at a time, this can be useful to distinguish
	* the callbacks.
	*/
	void* userData;

#ifndef DOXYGEN
	// internal variables. do not modify.
	CONTEXT_INTERNAL_VARIABLES(DECLARE_IVAR);
#endif
};

/**
* Initializes a context's internal state.
* Must be called before the first call to mtxFeed() with this context.
* \param context Its variables need not be initialized.
*/
void mtxStart(MTXContext* context);

/**
* Parses \a data in a \a context.
*
* The data must be null-terminated. It needn't be the entire XML document;
* MTXContext::dataRemains() will be called with any data that couldn't be completely parsed.
* You can then call this function again when you have more data.
*
* You must not call this function from within an MTXml callback.
* Doing so would corrupt the parser's internal state.
*
* This function causes Latin-1 output.
*
* \returns Non-zero if mtxStop() was called from a callback within the call to this function,
* zero otherwise.
* \note The data is altered in unspecified ways during parsing, as to avoid time-consuming copies.
* Don't try to reuse it during or after the call to this function.
*/
int mtxFeed(MTXContext* context, char* data);

/**
* Parses \a data in a \a context.
*
* Data sent to callbacks will have its UTF-8 characters and standard entities converted to Latin-1.
*
* This function causes Latin-1 output.
*
* \see mtxFeed()
*/
int mtxFeedProcess(MTXContext* context, char* data);

/*
* This function causes wide-char output.
*
* Data sent to callbacks will have its UTF-8 characters and standard entities converted to Unicode.
*
* The parameter \a wideBuffer is a pointer to a scratch buffer,
* used by the parser.
* It must be at least as big as \a data, counted in characters.
*
* \see mtxFeed()
*/
int mtxFeedWide(MTXContext* context, char* data, wchar_t* wideBuffer);

/**
* If called from within an MTXml callback,
* this function ensures that no more callbacks will be called during this feed
* and that it is safe to call mtxStart() on the \a context.
*
* However, it is still not allowed to call mtxFeed() from within a an MTXml callback.
*
* If called from outside an MTXml callback, this function has no effect.
*/
void mtxStop(MTXContext* context);

/**
* Processes \a data, converting UTF-8 and standard entities to Latin-1.
* Returns the length of the processed string, or \< 0 on error.
* Does not cause any callbacks, even on error, except MTXContext::unicodeCharacter().
* Does not modify the \a context.
*/
int mtxProcess(MTXContext* context, char* data);

/**
* A basic implementation of MTXContext::unicodeCharacter().
* Converts the following codepoints into their Latin-1 equivalents:\n
* 2000 - 200A: space.\n
* 2010 - 2015: dash.\n
* 2018 - 201B: apostrophe.\n
* 201C - 201F: double quotes.\n
* 2022: middle dot.\n
* 20AC: euro sign.\n
* 2122: trade mark sign.\n
*
* Returns 0 for all other input, which causes a parse error.
* \note The number of conversions performed by this function may be increased in
* future versions.
* \param context Ignored.
* \param unicode The unicode character to be converted.
*/
unsigned char mtxBasicUnicodeConvert(MTXContext* context, int unicode);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/**
* \brief MoSync wrappers for XML/SAX parser
*/

namespace Mtx {
	/**
	* \see Context
	*/
	template<class Tchar> class XmlListenerT {
	public:
		/**
		* \see MTXContext::encoding
		*/
		virtual void mtxEncoding(const char* value) = 0;
		/**
		* \see MTXContext::tagStart
		*/
		virtual void mtxTagStart(const Tchar* name, int len) = 0;
		/**
		* \see MTXContext::tagAttr
		*/
		virtual void mtxTagAttr(const Tchar* attrName, const Tchar* attrValue) = 0;
		/**
		* \see MTXContext::tagStartEnd
		*/
		virtual void mtxTagStartEnd() = 0;
		/**
		* \see MTXContext::tagData
		*/
		virtual void mtxTagData(const Tchar* data, int len) = 0;
		/**
		* \see MTXContext::tagEnd
		*/
		virtual void mtxTagEnd(const Tchar* name, int len) = 0;
		/**
		* \see MTXContext::parseError
		*/
		virtual void mtxParseError() = 0;
		/**
		* \see MTXContext::emptyTagEnd
		*/
		virtual void mtxEmptyTagEnd() = 0;
	};

	class XmlListener : public XmlListenerT<char> {
	public:
		/**
		* \see MTXContext::unicodeCharacter
		* \note This function is not pure virtual, like the others.
		* Its default implementation calls mtxBasicUnicodeConvert().
		*/
		virtual unsigned char mtxUnicodeCharacter(int unicode);
	};

	class XmlListenerW : public XmlListenerT<wchar_t> {
	};

	/**
	* \see Context
	*/
	class MtxListener {
	public:
		/**
		* \see MTXContext::dataRemains
		*/
		virtual void mtxDataRemains(const char* data, int len) = 0;
	};

	class ContextBase {
	public:
		/**
		* \see mtxStop()
		*/
		void stop();
	protected:
		MTXContext mContext;
		MtxListener* mMtx;
		void initBase();
	};

	/**
	* \brief A C++ wrapper for MTXml, with wide-char Unicode output.
	*/
	class ContextW : public ContextBase {
	public:
		/**
		* \see mtxStart()
		*/
		void init(MtxListener* mtx, XmlListenerW* xml);
		/**
		* \see feedWide()
		*/
		bool feed(char* data, wchar_t* wideBuffer);
	private:
		XmlListenerW* mXml;
	};

	/**
	* \brief A C++ wrapper for MTXml, with Latin-1 output.
	*/
	class Context : public ContextBase {
	public:
		/**
		* \see mtxStart()
		*/
		void init(MtxListener* mtx, XmlListener* xml);
		/**
		* \see mtxFeed()
		*/
		bool feed(char* data);
		/**
		* \see mtxFeedProcess()
		*/
		bool feedProcess(char* data);
		/**
		* \see mtxProcess()
		*/
		int process(char* data);
	private:
		XmlListener* mXml;
	};
}
#endif

#endif	//_MTXML_H_
