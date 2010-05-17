#include <MAUI/Keyboard/Parser/AttributeHandler.h>
#include <MAUI/Keyboard/Parser/LayoutParser.h>
#include <MAUI/Keyboard/Parser/ResourceIO.h>

#include <MAUI/Keyboard/VirtualKeyboard.h>
#include <MAUI/Keyboard/VirtualKey.h>

#include "VirtualKeyboardFactory.h"

namespace MAUI {

VirtualKeyboard *
VirtualKeyboardFactory::createFromXML(MAHandle xmlDocument)
{
	char xmlBuffer[ XML_BUFFER_SIZE+1 ];
	Mtx::SaxContext saxContext;
	ResourceIO xmlIO( xmlDocument );
	BufferHandler bufferHandler( xmlBuffer, XML_BUFFER_SIZE );

	VirtualKeyboard *keyboard = new VirtualKeyboard( );
	LayoutParser layoutParser( keyboard );

	saxContext.start( layoutParser, bufferHandler );

	/*
	 * Fill the buffer one block at a time, and let
	 * BufferHandle handle the situation where we can't
	 * parse parts of the XML file.
	 */
	while( !xmlIO.eof( ) )
	{
		/* Fill the parts of the buffer that can be filled. */
		char *readBuffer = bufferHandler.getBuffer( );
		unsigned int readBytes = xmlIO.read( readBuffer, bufferHandler.sizeLeft( ) );
		readBuffer[ readBytes ] = '\0';

		/* Feed read data into parser */
		char *buffer = bufferHandler.getStart( );
		saxContext.feed( buffer );
	}

	if( !layoutParser.internalError( ) )
	{
		return keyboard;
	}
	else
	{
		delete keyboard;
		return 0;
	}
}

VirtualKeyboard *
VirtualKeyboardFactory::createFromXMLString(char *xmlString)
{
	char xmlBuffer[ 1 ];
	Mtx::SaxContext saxContext;
	BufferHandler bufferHandler( xmlBuffer, 1 );

	VirtualKeyboard *keyboard = new VirtualKeyboard( );
	LayoutParser layoutParser( keyboard );

	saxContext.start( layoutParser, bufferHandler );

	saxContext.feed( xmlString );

	if( !layoutParser.internalError( ) )
	{
		return keyboard;
	}
	else
	{
		delete keyboard;
		return 0;
	}
}

VirtualKey *
VirtualKeyboardFactory::createFromAttributes(AttributeHandler *attrHandler)
{
	/* Check for that key has a label */
	const char *label = attrHandler->getValueString("label");

	if( label == NULL )
	{
		return NULL;
	}

	const char *output = attrHandler->getValueString("output");
	const char *action = attrHandler->getValueString("onclick");

	/* Default values */
	VirtualKey::Type keyType = stringToType( action );
	int width = 1;
	const char *layout = NULL;

	/* Get layout change */
	const char *newLayout = attrHandler->getValueString("change");
	if( newLayout )
	{
		keyType = VirtualKey::CHANGE_LAYOUT;
		layout = newLayout;
	}

	if( keyType == VirtualKey::NONE )
	{
		return NULL;
	}

	/* Get width */
	int newWidth = attrHandler->getValueInt("width");
	if( attrHandler->conversionOk( ) )
	{
		width = newWidth;
	}

	/* Create the actual key */
	VirtualKey *key = new VirtualKey(
									  label,
									  output,
									  keyType,
									  layout,
									  width
									 );
	return key;
}

enum VirtualKey::Type
VirtualKeyboardFactory::stringToType(const char *type)
{
	if( type == NULL )
	{
		return VirtualKey::KEY;
	}

	for(unsigned int i = 0; g_types[i].name != NULL; i++)
	{
		if( strcmp(type, g_types[i].name) == 0 )
		{
			return g_types[i].type;
		}
	}

	return VirtualKey::NONE;
}

} /* Namespace end */
