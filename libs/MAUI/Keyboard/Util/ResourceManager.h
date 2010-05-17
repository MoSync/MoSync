/**
 * @brief Keeps resources in one place accessible by strings.
 * @file ResourceManager.h
 * @author Mattias Frånberg
 */

#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_

#include <MAUtil/HashMap.h>

namespace MAUI {

class SkinSet;
class Font;

/**
 * The resource manager allows simple access resources. The
 * primary purpose of it is to provide the resources by mapping
 * each resource to a string.
 */
class ResourceManager
{
public:
	/**
	 * Returns the skin set corresponding to the
	 * given name.
	 *
	 * @param name Name of the skin set to retrieve.
	 */
	static SkinSet *getSkins(const char *name);

	/**
	 * Associates the given name with the given skin set.
	 *
	 * @param name An identifier corresponding to the given skin.
	 * @param skinSet The set of skins to associate with name.
	 */
	static void addSkins(const char *name, SkinSet *skinSet);

	/**
	 * Associates name with the given font.
	 *
	 * @param name An identifier corresponding to the given font.
	 * @param font The set of font to associate with name.
	 */
	static void addFont(const char *name, MAUI::Font *font);

	/**
	 * Returns the font associated with the given name.
	 * 
	 * @param name Name of the font to get.
	 * @return The font corresponding to name, or NULL if no such font 
	 *         exists.
	 */
	static MAUI::Font * getFont(const char *name);

private:
	/**
	 * Returns the singleton resource manager instance.
	 *
	 * @return the singleton resource manager instance.
	 */
	static ResourceManager *getInstance();

	/**
	 * Internal access to a skin.
	 *
	 * @param name Name of the skin to retrieve.
	 * @return the skin set corresponding to name.
	 */
	SkinSet *__getSkins(const char *name);

	/**
	 * Adds a skin to the resource manager internally.
	 *
	 * @param name An identifier corresponding to the given skin.
	 * @param skinSet The set of skins to associate with name.
	 */
	void __addSkins(const char *name, SkinSet *skinSet);

	/**
	 * Returns the font associated with the given name.
	 *
	 * @param name Name of the font to get.
	 * @return The font corresponding to name, or NULL if no such font 
	 *         exists.
	 */
	MAUI::Font *__getFont(const char *name);

	/**
	 * Associates name with the given font.
	 *
	 * @param name An identifier corresponding to the given font.
	 * @param font The set of font to associate with name.
	 */
	void __addFont(const char *name, MAUI::Font *font);

	/**
	 * Private constructor for the resource manager, only
	 * used for the singleton instance.
	 */
	ResourceManager();
	~ResourceManager();

	/**
	 * Map from an identifier to a skin set.
	 */
	MAUtil::HashMap< MAUtil::String, SkinSet * > m_skinSets;

	/**
	 * Mapping between a name and a font.
	 */
	MAUtil::HashMap< MAUtil::String, MAUI::Font * > m_fonts;
};

} /* Namespace end */

#endif /* RESOURCEMANAGER_H_ */
