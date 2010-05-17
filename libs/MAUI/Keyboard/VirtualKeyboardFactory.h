/**
 * @file VirtualKeyboardFactory.h
 * @brief Defines a class whose purpose is to build keyboard-related classes from XML.
 * @author Mattias Frånberg
 */

#ifndef VIRTUALKEYBOARDFACTORY_H_
#define VIRTUALKEYBOARDFACTORY_H_

#include <MAUI/Keyboard/VirtualKey.h>

namespace MAUI {

/* Forward declarations */
class VirtualKeyboard;
class AttributeHandler;

/**
 * A mapping between strings and key types.
 */
struct typepair_t
{
	/**
	 * Name of the type as a string.
	 */
	const char *name;

	/**
	 * The internal type which the name maps to.
	 */
	VirtualKey::Type type;
};

/**
 * A list of mappings between strings and types, the
 * list MUST end with an element containing whose
 * name is NULL.
 */
static const typepair_t g_types[] = {
									 {"hide", VirtualKey::HIDE},
									 {"remove", VirtualKey::REMOVE},
									 {"shift", VirtualKey::SHIFT},
									 {"line", VirtualKey::LINE_FEED},
									 {"left", VirtualKey::LEFT},
									 {"right", VirtualKey::RIGHT},
									 {"up", VirtualKey::UP},
									 {"down", VirtualKey::DOWN},
									 {"key", VirtualKey::KEY},
									 {NULL, VirtualKey::NONE}
									};


/**
 * XXX: For the moment the XML parser has some problems when
 * parsing files in multiple chunks, therefore this size
 * must be large enough to contain the whole XML file.
 */
#define XML_BUFFER_SIZE 4096

/**
 * Builds keyboards and keys from XML data. The purpose is
 * to group all instantiations of Keyboard-related classes
 * here.
 */
class VirtualKeyboardFactory
{
public:
	/**
	 * Creates a virtual keyboard from the specified XML.
	 *
	 * @param xmlDocument A XML document resource.
	 */
	static VirtualKeyboard *createFromXML(MAHandle xmlDocument);

	/**
	 * Creates a virtual keyboard from the given XML string.
	 * 
	 * @param xmlString An xml string that contains the layout
	 *                  of the keyboard. The content of this string
	 *                  is altered/consumed after this call.
	 */
	static VirtualKeyboard *createFromXMLString(char *xmlString);

	/**
	 * Creates a key from a set of XML attributes. The supported attributes
	 * are currently:
	 * label   - The string that will be shown on the key button.
	 * output  - The string that will be outputted when the key is
	 *           pressed.
	 * onclick - The action that is taken when the key is pressed.
	 * width   - The width of the key in pixels.
	 *
	 * @param attrHandler Contains the attributes.
	 */
	static VirtualKey *createFromAttributes(AttributeHandler *attrHandler);

	/**
	 * Converts a type string to an internal representation.
	 *
	 * @param type The type that will be converted.
	 * @return The enum corresponding to the type string.
	 */
	static enum VirtualKey::Type stringToType(const char *type);
};

} /* Namespace end */

#endif /* VIRTUALKEYBOARDFACTORY_H_ */
