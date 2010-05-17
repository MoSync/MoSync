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
	return m_skinMap[ MAUtil::String( name ) ];
}

} /* Namespace end */
