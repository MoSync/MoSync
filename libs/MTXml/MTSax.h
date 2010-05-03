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
* \file MTSax.h
* \brief Tiny SAX parser
*
* MTSax is a simple SAX parser built as a thin layer on top of MTXml.
*
* For detailed information about the parsing capabilities and restrictions of
* MTSax, see the documentation for MTXml.
*
* MTSax differs mainly from MTXml in that it caches certain pieces of data, in
* order to provide more user-friendly callbacks.  This means that, unlike
* MTXml, MTSax has to allocate memory from the heap during parsing.
*/

#ifndef _MTSAX_H_
#define _MTSAX_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
* The context of an MTSax parser.
* Contains function pointers for callbacks from the parser.
* Also contains internal variables.
*
* You must set all the callbacks before calling mtxSaxStart().
*/
typedef struct MTXSaxContext_t
{
	/**
	* If the "encoding" attribute is present in the "?xml" tag,
	* this function is called.
	*
	* The default encoding is UTF-8.
	*
	* \param context The current context.
	* \param value The encoding name specified in the document.
	*
	* \see http://www.w3.org/TR/xml11/#charencoding
	*/
	void (*encoding)(MTXSaxContext_t* context, const char* value);

	/**
	* Called when a start-tag or an empty element is parsed.
	*
	* \param context The current context.
	* \param name The name of the element.
	* \param attributes The attributes in this element, specified as a
	* NULL-terminated name/value pointer pair array.
	*/
	void (*startElement)(MTXSaxContext_t* context, const void* name, const void** attributes);

	/**
	* Called when an end-element or the end of an empty element is parsed.
	*
	* \param context The current context.
	* \param name The name of the element.
	*/
	void (*endElement)(MTXSaxContext_t* context, const void* name);

	/**
	* Called when character data is parsed.
	*
	* May be called more than once per data section,
	* so don't expect your data to be delivered in one piece.
	*
	* \param context The current context.
	* \param data The character data.
	* \param length The length, in characters, of the provided data.
	*/
	void (*characters)(MTXSaxContext_t* context, const void* data, int length);

	/**
	* Called during UTF-8 processing when a character is encountered whose
	* encoding is longer than 8 bits.
	*
	* This function returns an 8-bit representation of that Unicode character.
	* This function may return zero.  If so, the mtxBasicUnicodeConvert
	* function is called.  If that too returns zero, a parse error is
	* generated.
	*
	* \param context The current context.
	* \param character The Unicode code point to convert.
	*/
	unsigned char (*unicodeCharacter)(MTXSaxContext_t* context, int character);

	/**
	* Called when some data at the end of the supplied buffer couldn't be completely parsed.
	* Prepend it to your next feed.
	*
	* \param context The current context.
	* \param data The XML data.  May point to the beginning of the buffer that
	* was supplied to feed().  In that case, the data needn't be moved.
	* \param length The length, in characters, of the data.
	*
	* \see mtxSaxFeed()
	*/
	void (*dataRemains)(MTXSaxContext_t* context, const char* data, int length);

	/**
	* Called when the parser detects an error.
	*
	* \param context The current context.
	*/
	void (*parseError)(MTXSaxContext_t* context);

	/**
	* A value you can set to anything you like.  If you're parsing more than
	* one document at a time, this can be a useful way to distinguish them.
	*/
	void* userData;
#ifndef DOXYGEN
	// The MTXml context used for parsing
	MTXContext inner;
	// The name of the current element.
	// This buffer is allocated from the heap.
	void* name;
	// The size, in bytes, of the buffer holding the current element name.
	int nameSize;
	// The buffer holding the (null-terminated) attribute name and value
	// strings.  This buffer is allocated from the heap.
	char* attrBuf;
	// The size, in bytes, of the buffer holding attribute names and values
	int attrBufSize;
	// The amount of space, in bytes, in use of the attribute name/value buffer
	int attrBufUsed;
	// The offsets, in bytes, into the attribute name/value buffer where
	// individual strings start.  This buffer is allocated from the heap.
	// This buffer holds offsets instead of absolute addresses, in order not to
	// be affected by the attribute name/value buffer being re-allocated.
	// The base address is added to the offset values just prior to calling
	// startElement.
	const char** attrIndex;
	// The size, in pointers, of the attribute index
	int attrIndexSize;
	// The number of attributes (name/value pairs) in the current element
	int attrCount;
	// True for wide-char output, false for char output.
	bool isWide;
#endif
} MTXSaxContext;

/**
* Initializes a context's internal state.
* Must be called before the first call to mtxSaxFeed() with this context.
*
* \remarks As the MTSax context allocates memory from the heap, mtxSaxStop()
* must be called either to stop the parsing prematurely or after having parsed
* the entire document.  Failure to do so will lead to memory leaks.
*/
void mtxSaxStart(MTXSaxContext* context);

/**
* Parses \a data in the specified context.
*
* The data is null-terminated.  It needn't be the entire XML document;
* MTXSaxContext::dataRemains() will be called with any data that couldn't be
* completely parsed.  You can then call this function again when you have more
* data.
*
* You must not call this function from within an MTSax callback.
* Doing so would corrupt the parser's internal state.
*
* \returns True if mtxSaxStop() was called from a callback within the call to
* this function, false otherwise.
*/
bool mtxSaxFeed(MTXSaxContext* context, char* data);

/**
* Parses \a data in the specified context.
*
* Data sent to callbacks will have its UTF-8 characters and standard entities
* converted to Latin-1.
*
* \see mtxSaxFeed()
*/
bool mtxSaxFeedProcess(MTXSaxContext* context, char* data);

/**
* \copydoc mtxFeedWide()
* \see mtxSaxFeed()
*/
int mtxSaxFeedWide(MTXContext* context, char* data, wchar_t* wideBuffer);

/**
* Stops parsing and frees any allocated memory.  Must be called either to stop
* parsing prematurely, or after having parsed the document, in order to avoid
* memory leaks.
*
* If called from within an MTSax callback, this function ensures that no more
* callbacks will be called during this feed and that it is safe to call
* mtxSaxStart() on the \a context.
*
* However, it is still not allowed to call mtxSaxFeed() from within a an MTSax
* callback.
*/
void mtxSaxStop(MTXSaxContext* context);

#ifdef __cplusplus
} /* extern "C" */
#endif

#ifdef __cplusplus

namespace Mtx
{

/**
 * SAX parser callback interface class.
 *
 * Inherit from this to receive callbacks from a SaxContext.  This is analogous
 * to filling in the function pointers of a MTXSaxContext object.
 */
template<class Tchar> class SaxListenerT
{
public:
	/**
	 * \copydoc MTXSaxContext::encoding
	 */
	virtual void mtxEncoding(const char* value) = 0;
	/**
	 * \copydoc MTXSaxContext::startElement
	 */
	virtual void mtxStartElement(const Tchar* name, const Tchar** attributes) = 0;
	/**
	 * \copydoc MTXSaxContext::endElement
	 */
	virtual void mtxEndElement(const Tchar* name) = 0;
	/**
	 * \copydoc MTXSaxContext::characters
	 */
	virtual void mtxCharacters(const Tchar* data, int length) = 0;
	/**
	 * \copydoc MTXSaxContext::parseError
	 */
	virtual void mtxParseError(void) = 0;
};

class SaxListener : public SaxListenerT<char> {
public:
	/**
	 * \copydoc MTXSaxContext::unicodeCharacter
	 */
	virtual unsigned char mtxUnicodeCharacter(int character) = 0;
};

class SaxListenerW : public SaxListenerT<wchar_t> {
};

/**
 * SAX parser context wrapper class.
 *
 * This class wraps the mtxSax set of functions.  To use it, you need to
 * inherit and implement the MtxListener and SaxListener interfaces.
 */
class SaxContextBase
{
public:
	/**
	 * Destructor.
	 */
	~SaxContextBase(void);

	/**
	 * De-initializes this parser context if it was previously initialized.
	 *
	 * \see mtxSaxStop
	 */
	void stop(void);
	/**
	 * \return \c true if this parser context is initialized, otherwise \c false.
	 */
	bool isStarted(void) const;
protected:
	/**
	 * Constructor.
	 */
	SaxContextBase(void);

	static void encoding(MTXSaxContext* context, const char* value);
	static void startElement(MTXSaxContext* context, const void* name, const void** attributes);
	static void endElement(MTXSaxContext* context, const void* name);
	static void characters(MTXSaxContext* context, const void* data, int length);
	static void dataRemains(MTXSaxContext* context, const char* data, int length);
	static void parseError(MTXSaxContext* context);

	MTXSaxContext mContext;
	MtxListener* mMtxListener;
	SaxListenerW* mSaxListenerW;
};

class SaxContext : public SaxContextBase {
public:
	/**
	 * Constructor.
	 */
	SaxContext(void);
	/**
	 * Initializes this parser context using the specified callback interfaces.
	 * The SAX interface receives XML data, while the MTXml interface handles
	 * buffer management.
	 * \param newSaxListener The SAX interface implementation to use.
	 * \param newMtxListener The MTXml interface implementation to use.
	 *
	 * \see mtxSaxStart
	 */
	void start(SaxListener& newSaxListener, MtxListener& newMtxListener);
	/**
	 * Feeds some data into this parser context.  The context must already be
	 * started.  Any remaining data is passed to the MtxListener interface.
	 * \param data The (null-terminated) data to feed into the parser.
	 *
	 * \note You must not call this method from within a callback.
	 *
	 * \see mtxSaxFeed
	 */
	bool feed(char* data);
	/**
	 * Feeds some data into this parser context, with UTF-8 to Latin-1
	 * conversion.  The context must already be started.  Any remaining data is
	 * passed to the MtxListener interface.
	 * \param data The (null-terminated) data to feed into the parser.
	 *
	 * \note You must not call this method from within a callback.
	 *
	 * \see mtxSaxFeedProcess
	 */
	bool feedProcess(char* data);
private:
	static unsigned char unicodeCharacter(MTXSaxContext* context, int character);
};

class SaxContextW : public SaxContextBase {
public:
	//SaxContextW(void);
	void start(SaxListenerW& newSaxListener, MtxListener& newMtxListener);
	bool feed(char* data, wchar_t* wideBuffer);
};

} /* namespace Mtx */

#endif /* __cplusplus */

#endif  /* _MTSAX_H_ */
