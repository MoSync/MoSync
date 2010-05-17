/**
 * @brief Contains the implementation of an XML parser for layout definitions.
 * @file LayoutParser.h
 * @author Mattias Frånberg
 */

#ifndef LAYOUTPARSER_H_
#define LAYOUTPARSER_H_

#include <MAUtil/HashMap.h>
#include <MAUtil/Vector.h>

#include <MTXml/MTXml.h>
#include <MTXml/MTSax.h>

namespace MAUI {

/* Forward declarations */
class VirtualLayout;
class VirtualKeyboard;
class VirtualKey;

/**
 * A simple counter class.
 */
template <class T>
class Counter
{
public:
	/**
	 * constructor
	 *
	 * Initializes the counter to the given default value.
	 *
	 * @param counter Default value.
	 */
	Counter(T counter = T(0))
	{
		m_counter = counter;
	}

	/**
	 * Returns the current value of the counter.
	 *
	 * @return the current value of the counter.
	 */
	const T &
	get() const
	{
		return m_counter;
	}

	/**
	 * Increments the counter using the prefix operator,
	 * and returns the new value.
	 *
	 * @return the incremented value of the counter.
	 */
	const T &
	increment()
	{
		return ++m_counter;
	}

	/**
	 * Resets the counter to the specified value,
	 * or zero if none is specified.
	 *
	 * @param resetValue Value to reset the counter to.
	 */
	void
	reset(T resetValue = T(0))
	{
		m_counter = resetValue;
	}

	/**
	 * Cast Counter<T> to T operator.
	 */
	operator T() const
	{
		return m_counter;
	}

private:
	/**
	 * Current value of the counter.
	 */
	T m_counter;
};

/**
 * An implementation of the XML SAX interface, that parses
 * XML layout files.
 */
class LayoutParser
: public Mtx::SaxListener
{
public:
	LayoutParser(VirtualKeyboard *keyboard);

	/**
	 * @see Mtx::SaxListener::mtxEncoding.
	 */
	void mtxEncoding(const char *value);

	/**
	 * @see Mtx::SaxListener::mtxStartElement.
	 */
	void mtxStartElement(const char *name, const char **attributes);

	/**
	 * @see Mtx::SaxListener::mtxEndElement.
	 */
	void mtxEndElement(const char *name);

	/**
	 * @see Mtx::SaxListener::mtxCharacters.
	 */
	void mtxCharacters(const char *data, int length);

	/**
	 * @see Mtx::SaxListener::mtxUnicodeCharacter.
	 */
	unsigned char mtxUnicodeCharacter(int character);

	/**
	 * @see Mtx::SaxListener::mtxParseError.
	 */
	void mtxParseError(void);

	/**
	 * Indicates whether an internal error has occurred
	 * during parsing.
	 *
	 * @return Returns true if an internal error has occurred
	 *         during parsing, false otherwise.
	 */
	bool internalError() const;

private:

	/**
	 * Contains of layouts and its children.
	 */
	Counter<unsigned int> m_rowCounter;

	/**
	 * The keyboard that is being parsed.
	 */
	VirtualKeyboard *m_keyboard;

	/**
	 * The current keyboard layout.
	 */
	VirtualLayout *m_curLayout;

	/**
	 * Indicates whether an internal error occurred during parsing.
	 */
	bool m_internalError;
};

/**
 * Handles the read buffer between calls to the XML parser.
 * It is responsible for handling where to write to the
 * buffer, since an XML file is parsed in chunks and
 * if a part of it cannot be parsed, this class copies the
 * part to the beginning of the buffer and moves forward
 * the start of the buffer.
 */
class BufferHandler
: public Mtx::MtxListener
{
public:
	/**
	 * Creates a handler to the given buffer.
	 *
	 * @param buffer The buffer that will be handled.
	 * @param bufferSize The size of the buffer.
	 */
	BufferHandler(char *buffer, unsigned int bufferSize);

	/**
	 * Returns the current size of the buffer. NOTICE, that
	 * the size of the buffer might change between calls.
	 */
	unsigned int sizeLeft() const;

	/**
	 * Returns a pointer to the buffer. NOTICE, that this
	 * pointer might be different between calls, and you
	 * cannot assume that it will be the same.
	 *
	 * @return A pointer to the buffer.
	 */
	char *getBuffer();

	/**
	 * Returns a pointer to the start of the buffer.
	 *
	 * @return A pointer to the buffer.
	 */
	char *getStart();

	/**
	 * \see @MtxListener::mtxDataRemains
	 */
	void mtxDataRemains(const char* data, int len);

private:
	/**
	 * Start offset of the buffer.
	 */
	unsigned int m_startOffset;

	/**
	 * The internal buffer.
	 */
	char *m_buffer;

	/**
	 *Size of the buffer.
	 */
	unsigned int m_bufferSize;
};

} /* Namespace end */

#endif /* LAYOUTPARSER_H_ */
