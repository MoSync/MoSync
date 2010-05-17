/**
 * @brief Defines a class that represents a set of WidgetSkins.
 * @file SkinSet.h
 * @author Mattias Frånberg
 */

#ifndef SKINSET_H_
#define SKINSET_H_

#include <ma.h>

#include <MAUI/WidgetSkin.h>
#include <MAUtil/String.h>
#include <MAUtil/HashMap.h>

namespace MAUI {

/**
 * Represents a set of skins.
 */
class SkinSet
{
public:
	/**
	 * Initializes an empty skin set.
	 */
	SkinSet();

	/**
	 * Returns the skin corresponding to the given name.
	 *
	 * @param name The name of the skin to find.
	 * @return the skin corresponding to name, NULL if no skin by
	 *          that name was found.
	 */
	MAUI::WidgetSkin *getSkin(const char *name);

	/**
	 * Maps the given name to the given skin.
	 *
	 * @param name Name of the skin.
	 * @param skin The skin to map to the given name.
	 */
	void addSkin(const char *name, MAUI::WidgetSkin *skin);

private:
	/**
	 * Mapping from skin names to skins.
	 */
	MAUtil::HashMap< MAUtil::String, MAUI::WidgetSkin * > m_skinMap;

};

} /* Namespace end */
#endif /* SKINSET_H_ */
