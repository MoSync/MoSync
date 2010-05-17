/**
 * @brief Defines a class that wraps an array of attributes.
 * @file AttributeHandler.h
 * @author Mattias Frånberg
 */

#ifndef ATTRIBUTEHANDLER_H_
#define ATTRIBUTEHANDLER_H_

namespace MAUI {

/**
 * This class abstracts the handling of attributes.
 *
 * The current design of this class assumes that
 * the number of attributes is very small. If many
 * attributes are used, this class is not suitable.
 */
class AttributeHandler
{
public:

	/**
	 * Parses the given attributes.
	 *
	 * @param attributes A null terminated list of attributes on the form
	 *					 name, value, name, value, ..., NULL.
	 */
	AttributeHandler(const char **attributes);

	/**
	 * Returns the value of the attribute corresponding
	 * to the given name as a string.
	 *
	 * @param name The attribute to find.
	 * @return the value of the attribute, or NULL if no
	 *         attribute was found.
	 */
	const char * getValueString(const char *name);

	/**
	 * Returns the value of the attribute corresponding
	 * to the given name as an integer. If the attribute
	 * could not be converted to an integer, this will be
	 * indicated by the AttributeHandler::conversionOk
	 * function.
	 *
	 * @param name The attribute to find.
	 * @return the integer value of the attribute.
	 */
	int getValueInt(const char *name);

	/**
	 * Returns the value of the attribute corresponding
	 * to the given name as a float. If the attribute
	 * could not be converted to a float, this will be
	 * indicated by the AttributeHandler::conversionOk
	 * function.
	 *
	 * @param name The attribute to find.
	 * @return the integer value of the attribute.
	 */
	float getValueFloat(const char *name);

	/**
	 * Returns the error status of the last conversion,
	 * true if the conversion was ok, false otherwise.
	 *
	 * @return the conversion status.
	 */
	bool conversionOk() const;

private:
	/**
	 * Status indicator for attribute retrieval.
	 */
	enum Status {ATTR_OK,			/* The attribute was availible. */
				 ATTR_NOT_FOUND,	/* The attribute was not found. */
				 ATTR_ERROR};		/* An error occurred when trying to retrieve the attribute. */

	/**
	 * Status of the last conversion or retrieval.
	 */
	enum Status m_status;

	/**
	 * A null terminated list of attributes.
	 */
	const char **m_attributes;

};

} /* Namespace end */

#endif /* ATTRIBUTEHANDLER_H_ */
