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
