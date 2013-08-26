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
 * @file ResourceSetLookup.h
 * @author Florin Leu
 * @date 22 Nov 2011
 *
 * @brief Class that implements a variant.
 *
 **/

#ifndef __RESOURCESETLOOKUP_H__
#define __RESOURCESETLOOKUP_H__

#include <ma.h>

#include "rescompdefines.h"

namespace ResourceCompiler
{
	class ByteArrayStream;

	class ResourceSetLookup
	{
	private:
		ResourceSetLookup();

	public:
		ResourceSetLookup(short n);
		~ResourceSetLookup();
		void read(ByteArrayStream* buffer);
		void print();
		char* getVariantDescription();
		byte getPriority(int index);
		short getLookup(int index);

	private:
		// The length of the variant description string, used to determine
		// what variant this resource set lookup should be used for.
		// Example: “screenSize:large”
		byte variantDescLength;
		// The number of resources that may have variants. The
		// resources in the index range [1,(numberOfVariantResources + 1]
		// are variant resources, the rest are not.
		short numberOfVariantResources;
		// The actual string
		char* variantDesc;
		// The lookup to find the ‘internal’ resource of a variant resource.
		// To perform a lookup now only requires a simple array lookup:
		// lookup[resourceHandle - 1]
		// If lookup[resourceHandle - 1] is equal to the special value 0,
		// then there is no special resource defined for this variant and
		// the next matching variant should be used
		short* lookup;
		// The priority array contains the priority of a this variant’s
		// resources -- if several variants matches the currently running
		// runtime, this number is used to find which one to use
		byte* priority;
	};
}

#endif //__RESOURCESETLOOKUP_H__
