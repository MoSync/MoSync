#include <MAUI/WidgetSkin.h>

#include "SkinSet.h"

namespace MAUI {

SkinSet::SkinSet()
{

}

void
SkinSet::addSkin(const char *name, MAUI::WidgetSkin *skin)
{
	m_skinMap[ MAUtil::String( name ) ] = skin;
}

WidgetSkin *
SkinSet::getSkin(const char *name)
{
	if( name == NULL )
	{
		return NULL;
	}

	MAUtil::String keyName( name );
	MAUtil::HashMap< MAUtil::String, MAUI::WidgetSkin * >::Iterator it = m_skinMap.find( keyName );
	if( it != m_skinMap.end( ) )
	{
		return (*it).second;
	}
	else
	{
		return NULL;
	}
}

} /* Namespace end */
