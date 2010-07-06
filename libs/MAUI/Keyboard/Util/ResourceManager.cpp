#include <MAUI/Keyboard/Util/SkinSet.h>

#include "ResourceManager.h"

namespace MAUI {

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{
}

ResourceManager *
ResourceManager::getInstance()
{
	static ResourceManager *s_singleton = new ResourceManager( );
	return s_singleton;
}

SkinSet *
ResourceManager::getSkins(const char *name)
{
	ResourceManager *manager = getInstance();

	return manager->__getSkins( name );
}

void
ResourceManager::addSkins(const char *name, SkinSet *skinSet)
{
	ResourceManager *manager = getInstance( );

	manager->__addSkins( name, skinSet );
}


void
ResourceManager::addFont( const char *name, MAUI::Font *font )
{
	ResourceManager *manager = getInstance( );

	manager->__addFont( name, font );
}

MAUI::Font *
ResourceManager::getFont( const char *name )
{
	ResourceManager *manager = getInstance( );

	return manager->__getFont( name );
}

SkinSet *
ResourceManager::__getSkins(const char *name)
{
	if( name == NULL )
	{
		return NULL;
	}

	return m_skinSets[ MAUtil::String( name ) ];
}

void
ResourceManager::__addSkins(const char *name, SkinSet *skinSet)
{
	m_skinSets[ MAUtil::String( name ) ] = skinSet;
}


void
ResourceManager::__addFont(const char *name, MAUI::Font *font)
{
	m_fonts[ MAUtil::String( name ) ] = font;
}


MAUI::Font *
ResourceManager::__getFont(const char *name)
{
	if( name == NULL )
	{
		return NULL;
	}

	return m_fonts[ MAUtil::String( name ) ];
}

} /* Namespace end */
