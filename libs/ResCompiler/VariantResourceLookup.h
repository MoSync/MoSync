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
 * @file VariantResourceLookup.h
 * @author Florin Leu
 * @date 22 Nov 2011
 *
 * @brief Class that stores all the variants.
 *
 **/

#ifndef __VARIANTRESOURCELOOKUP_H__
#define __VARIANTRESOURCELOOKUP_H__

#include <ma.h>
#include <mastring.h>

#include "rescompdefines.h"

namespace ResourceCompiler
{
	class ResourceSetLookup;

	class VariantResourceLookup
	{
	public:
		VariantResourceLookup();
		~VariantResourceLookup();
		void countResources();
		void readVariantMapping(MAHandle handle);
		void readResourceTypes(MAHandle handle);
		bool checkVariant(char* variant);
		void pickResources();
		MAHandle getSmartHandle(MAHandle handle);
		void loadResource(MAHandle handle, byte flag = MA_RESOURCE_OPEN|MA_RESOURCE_CLOSE);
		void unloadResource(MAHandle handle);
		void loadResources(bool checkDelayed);

	private:
		// The number of variants that has specific resource sets
		byte numberOfVariants;

		// The number of resources
		short numberOfResources;

		// The number of resources that may have variants. The
		// resources in the index range [1,(numberOfVariantResources + 1]
		// are variant resources, the rest are not.
		short numberOfVariantResources;

		// The length for each ResourceSetLookup
		short* resourceSetLookupLengths;

		// The lookup tables themselves (variable size)
		ResourceSetLookup** lookupTable;

		byte* resourceTypes;

		short* resourceSmartHandles;
	};
}

#endif //__VARIANTRESOURCELOOKUP_H__
