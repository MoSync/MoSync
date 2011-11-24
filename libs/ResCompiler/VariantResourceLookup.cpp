/* Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/**
 * @file VariantResourceLookup.cpp
 * @author Florin Leu
 * @date 22 Nov 2011
 **/

#include "VariantResourceLookup.h"

namespace ResourceCompiler
{
	VariantResourceLookup::VariantResourceLookup():
		numberOfVariants(0),
		numberOfVariantResources(0)
	{
	}

	VariantResourceLookup::~VariantResourceLookup()
	{
		DELETE(resourceSetLookupLengths)
		DELETE_ARRAY(lookupTable, numberOfVariants)
		DELETE(resourceTypes)
	}

	void VariantResourceLookup::readVariantMapping(MAHandle handle)
	{
		PRINTINT(handle);

		PRINT("maLoadRes");
		maLoadResource(handle, handle, MA_RESOURCE_OPEN|MA_RESOURCE_CLOSE);
		PRINT("maGetDataSize");
		int bufferSize = maGetDataSize(handle);
		PRINTINT(bufferSize);

		ByteArrayStream* buffer = new ByteArrayStream(bufferSize);
		buffer->writeData(handle);

		numberOfVariants = buffer->readByte();
		PRINTINT(numberOfVariants);

		numberOfVariantResources = buffer->readShort();
		PRINTINT(numberOfVariantResources);

		resourceSetLookupLengths = new short[numberOfVariants];
		for (int i=0; i<numberOfVariants; i++)
		{
			resourceSetLookupLengths[i] = buffer->readShort();
		}
		PRINTINTARRAY(resourceSetLookupLengths, numberOfVariants);

		lookupTable = new ResourceSetLookup*[numberOfVariants];
		for (int i=0; i<numberOfVariants; i++)
		{
			lookupTable[i] = new ResourceSetLookup(numberOfVariantResources);
			lookupTable[i]->read(buffer);
			lookupTable[i]->print();
		}

		DELETE(buffer)
	}

	void VariantResourceLookup::readResourceTypes(MAHandle handle)
	{
		PRINTINT(handle);

		maLoadResource(handle, handle, MA_RESOURCE_OPEN|MA_RESOURCE_CLOSE);

		int bufferSize = maGetDataSize(handle);
		PRINTINT(bufferSize)

		ByteArrayStream* buffer = new ByteArrayStream(bufferSize);
		buffer->writeData(handle);

		numberOfResources = buffer->readShort();
		PRINTINT(numberOfResources)

		resourceTypes = new byte[numberOfResources];
		for (int i=0; i<numberOfResources; i++)
		{
			resourceTypes[i] = buffer->readByte();
		}

		DELETE(buffer)
	}

	/*
	 * xlarge screens are at least 960dp x 720dp
	 * large screens are at least 640dp x 480dp
	 * normal screens are at least 470dp x 320dp
	 * small screens are at least 426dp x 320dp
	 */
	bool VariantResourceLookup::checkVariant(char* variant)
	{
		if (strlen(variant) == 0)
		{
			PRINT("WE HAVE AN ALWAYS VALID VARIANT");
			return true;
		}
		/// Get screen dimensions.
		MAExtent size = maGetScrSize();

		/// Extract the screen width
		int screenWidth = EXTENT_X(size);

		/// Extract the screen height
		int screenHeight = EXTENT_Y(size);

		PRINTINT(screenWidth)
		PRINTINT(screenHeight)

		if (screenWidth * screenHeight >= XLARGE_SCREEN_AREA)
		{
			PRINT("WE HAVE A XLARGE SCREEN");
			if (strstr(variant, XLARGE_SCREEN_VARIANT) != NULL)
			{
				PRINT("WE HAVE A XLARGE VARIANT");
				return true;
			}
		}
		else if (screenWidth * screenHeight >= LARGE_SCREEN_AREA)
		{
			PRINT("WE HAVE A LARGE SCREEN");
			if (strstr(variant, LARGE_SCREEN_VARIANT) != NULL)
			{
				PRINT("WE HAVE A LARGE VARIANT");
				return true;
			}
		}
		else if (screenWidth * screenHeight >= MEDIUM_SCREEN_AREA)
		{
			PRINT("WE HAVE A MEDIUM SCREEN");
			if (strstr(variant, MEDIUM_SCREEN_VARIANT) != NULL)
			{
				PRINT("WE HAVE A MEDIUM VARIANT");
				return true;
			}
		}
		else //if (screenWidth * screenHeight > SMALL_SCREEN_AREA)
		{
			PRINT("WE HAVE A SMALL SCREEN");
			if (strstr(variant, SMALL_SCREEN_VARIANT) != NULL)
			{
				PRINT("WE HAVE A SMALL VARIANT");
				return true;
			}
		}

		PRINT("WE DON'T HAVE NOTHING");
		return false;
	}

	void VariantResourceLookup::pickResources()
	{
		resourceSmartHandles = new short[numberOfVariantResources];
		memset(resourceSmartHandles, 0, numberOfVariantResources * sizeof(short));

		char currentPriority[numberOfVariantResources];
		memset(currentPriority, 0xFF, numberOfVariantResources);

		for (int i=0; i<numberOfVariants; i++)
		{
			ResourceSetLookup* table = lookupTable[i];
			if (checkVariant(table->getVariantDescription()))
			{
				for (int j=0; j<numberOfVariantResources; j++)
				{
					PRINT("Lookup = %d, Priority = %d, CurrentPriority = %d", table->getLookup(j), table->getPriority(j), currentPriority[j]);
					if ( (table->getLookup(j) > 0) &&
							(table->getPriority(j) > currentPriority[j]) )
					{
						currentPriority[j] = table->getPriority(j);
						resourceSmartHandles[j] = table->getLookup(j);
						PRINT("SET RESOURCE %d", j);
					}
				}
			}
		}

		for (int i=0; i<numberOfVariantResources; i++)
		{
			PRINT("resourceSmartHandles[%d]=%d", i, resourceSmartHandles[i]);
		}
	}

	MAHandle VariantResourceLookup::getSmartHandle(MAHandle handle)
	{
		if (handle > numberOfVariantResources)
		{
			return handle;
		}
		return resourceSmartHandles[handle - 1];
	}

	void VariantResourceLookup::loadResource(MAHandle handle, byte flag)
	{
		int newHandle = getSmartHandle(handle);
		PRINT("maLoadResource(%d, %d)", newHandle, handle);
		maLoadResource(newHandle, handle, flag);
	}

	void VariantResourceLookup::unloadResource(MAHandle handle)
	{
		maDestroyObject(handle);
	}

	void VariantResourceLookup::loadResources()
	{
		int flag = MA_RESOURCE_OPEN;
		int lastLoadedResource = -1;
		for (int i=0; i<numberOfResources; i++)
		{
			if ((resourceTypes[getSmartHandle(i+1)] & 0x40) == 0)
			{
				loadResource(i + 1, flag);
				lastLoadedResource = i + 1;
				flag = 0;
			}
		}
		if (lastLoadedResource != -1)
		{
			loadResource(lastLoadedResource, MA_RESOURCE_CLOSE);
		}
	}
}
