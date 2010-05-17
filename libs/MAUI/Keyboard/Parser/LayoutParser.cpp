#include <mastdlib.h>
#include <mastring.h>

#include <MAUI/Keyboard/Parser/AttributeHandler.h>

#include <MAUI/Keyboard/VirtualKeyboardFactory.h>
#include <MAUI/Keyboard/VirtualKeyboard.h>
#include <MAUI/Keyboard/VirtualKey.h>

#include "LayoutParser.h"


namespace MAUI {

/* LayoutParser */
LayoutParser::LayoutParser(VirtualKeyboard *keyboard)
{
	m_keyboard = keyboard;
	m_rowCounter.reset( );
	m_internalError = false;
}

void
LayoutParser::mtxEncoding(const char *value)
{
}

void
LayoutParser::mtxStartElement(const char *name, const char **attributes)
{
	AttributeHandler attrHandler( attributes );
	if(strcmp(name, "layout") == 0)
	{
		/* Create a new layout, and set up its UI components */
		const char *layoutName = attrHandler.getValueString("name");
		if(layoutName != NULL)
		{
			m_curLayout = new VirtualLayout( layoutName );
			m_keyboard->addLayout( m_curLayout );
		}
		m_rowCounter.reset( 0 );
	}
	else if(strcmp(name, "key") == 0)
	{
		VirtualKey *key = VirtualKeyboardFactory::createFromAttributes( &attrHandler );
		if( key )
		{
			m_curLayout->addKey( m_rowCounter, key );
		}
	}
	else if(strcmp(name, "row") == 0)
	{
		float height = attrHandler.getValueFloat("height");
		if( attrHandler.conversionOk( ) )
		{
			m_curLayout->setRowHeight( m_rowCounter, height );
		}
	}
}

void
LayoutParser::mtxEndElement(const char *name)
{
	if(strcmp(name, "row") == 0)
	{
		m_rowCounter.increment( );
	}
}

void
LayoutParser::mtxCharacters(const char *data, int length)
{

}

unsigned char
LayoutParser::mtxUnicodeCharacter (int character)
{
	return (unsigned char) character;
}

void
LayoutParser::mtxParseError (void)
{
	m_internalError = true;
}

bool
LayoutParser::internalError() const
{
	return m_internalError;
}

/* BufferHandler */
BufferHandler::BufferHandler(char *buffer, unsigned int bufferSize)
{
	m_bufferSize = bufferSize;
	m_buffer = buffer;
	m_startOffset = 0;
}

unsigned int
BufferHandler::sizeLeft() const
{
	return m_bufferSize - m_startOffset;
}

char *
BufferHandler::getBuffer()
{
	return m_buffer + m_startOffset;
}

char *
BufferHandler::getStart()
{
	return m_buffer;
}

void
BufferHandler::mtxDataRemains(const char* data, int len)
{
	// Don't allow the same pointers to be used
	if( data == m_buffer )
	{
		return;
	}

	if(len > (int)m_bufferSize)
	{
		maPanic(0, "BufferHandler::mtxDataRemains: Buffer too small.");
	}

	/* Don't use memcpy here since it might copy in blocks */
	for(int i = 0; i < len; i++)
	{
		m_buffer[i] = data[i];
	}
	m_startOffset = len;
}

} /* Namespace end */
