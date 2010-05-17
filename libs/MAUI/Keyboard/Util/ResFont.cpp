/*
 * Fonts.cpp
 *
 *  Created on: 7 maj 2010
 *      Author: Mattias
 */

#include <maapi.h>
#include <MAUI/Font.h>

#include "ResFont.h"
#include "ResourceManager.h"

namespace MAUI {

ResFont::ResFont(struct ufont_t *fonts)
{

	for(int i = 0; fonts[i].name != NULL; i++)
	{
		MAHandle fontStorage = maCreatePlaceholder( );
		maCreateData( fontStorage, fonts[i].dataSize );
		maWriteData( fontStorage,
					 (void *) fonts[i].data,
					 0,
					 fonts[i].dataSize );


		ResourceManager::addFont(fonts[i].name, new MAUI::Font( fontStorage ));
	}
}

} /* Namespace end */
