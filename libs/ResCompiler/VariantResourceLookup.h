/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
