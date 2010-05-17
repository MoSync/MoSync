#include <MAUI/WidgetSkin.h>
#include <MAUI/Keyboard/Util/SkinSet.h>
#include <MAUI/Keyboard/Util/ResourceManager.h>

#include "ResSkin.h"

namespace MAUI {

ResSkin::ResSkin(const char *name, struct uskin_t *skins)
{
	SkinSet *skinSet = new SkinSet( );
	addUnallocatedSkins( skinSet, skins );
	ResourceManager::addSkins( name, skinSet );
}

ResSkin::ResSkin(const char *name, struct askin_t *skins)
{
	SkinSet *skinSet = new SkinSet( );
	addAllocatedSkins( skinSet, skins );
	ResourceManager::addSkins( name, skinSet );
}

void
ResSkin::addAllocatedSkins(SkinSet *skinSet, struct askin_t *skins)
{
	for(int i = 0; skins[i].name != NULL; i++)
	{
		struct askin_t *skin = &skins[ i ];

		WidgetSkin *widgetSkin = new WidgetSkin(
										   skin->selected,
										   skin->unselected,
										   skin->rect.x1,
										   skin->rect.x2,
										   skin->rect.y1,
										   skin->rect.y2,
										   skin->sTransparent,
										   skin->uTransparent
										 );

		skinSet->addSkin( skin->name, widgetSkin );
	}

	
}

void
ResSkin::addUnallocatedSkins(SkinSet *skinSet, struct uskin_t *skins)
{
	for(int i = 0; skins[i].name != NULL; i++)
	{
		struct uskin_t *skin = &skins[ i ];

		MAExtent skinSize = EXTENT( skin->width, skin->height );

		/* Allocate selected skin */
		MAHandle selectedStorage = maCreatePlaceholder( );
		int status = maCreateImageRaw( selectedStorage,
									   (void *) skin->sPixels,
									   skinSize,
									   1 );

		/* Allocate unselected skin */
		MAHandle unselectedStorage = maCreatePlaceholder( );
		status = maCreateImageRaw( unselectedStorage,
								   (void *) skin->uPixels,
								   skinSize,
								   1 );

		WidgetSkin *widgetSkin = new WidgetSkin(
										   unselectedStorage,
										   selectedStorage,
										   skin->rect.x1,
										   skin->rect.x2,
										   skin->rect.y1,
										   skin->rect.y2,
										   skin->sTransparent,
										   skin->uTransparent
										 );

		skinSet->addSkin( skin->name, widgetSkin );
	}
}

}
