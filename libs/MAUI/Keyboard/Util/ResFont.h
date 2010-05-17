/**
 * @brief Defines a class that adds fonts to the ResourceManager.
 * @file ResFont.h
 * @author Mattias Frånberg
 */

#ifndef RESFONT_H_
#define RESFONT_H_

#include <maapi.h>

namespace MAUI {

/**
 * Unallocated font that has to be read from a byte vector.
 */
struct ufont_t
{
	/**
	 * Name of the font.
	 */
	const char *name;

	/**
	 * Array of bytes that describes the font.
	 */
	unsigned char *data;

	/**
	 * Length of the data array.
	 */
	unsigned int dataSize;
};

/**
 * Allocated font that can be read from a resource.
 */
struct afont_t
{
	/**
	 * Name of the font.
	 */
	const char *name;

	/**
	 * Resource that contains the font.
	 */
	MAHandle res;
};

/**
 * A wrapper class for adding fonts to the resource manager
 * during static initialization.
 */
class ResFont
{
public:
	/**
	 * Adds a list of fonts to the resource manager.
	 *
	 *
	 * @param fonts A list of fonts ending with an element
	 * 				which has name = NULL.
	 */
	ResFont(struct ufont_t *fonts);
};

} /* Namespace end */

#endif /* RESFONT_H_ */
