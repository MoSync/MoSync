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
 * @file ResourceSetLookup.cpp
 * @author Florin Leu
 * @date 22 Nov 2011
 **/

#include "ResourceSetLookup.h"
#include "ByteArrayStream.h"

namespace ResourceCompiler
{
	ResourceSetLookup::ResourceSetLookup()
	{
	}

	ResourceSetLookup::ResourceSetLookup(short n):
		variantDescLength(0),
		numberOfVariantResources(n)
	{
	}

	ResourceSetLookup::~ResourceSetLookup()
	{
		DELETE(variantDesc);
		DELETE(lookup);
		DELETE(priority);
	}

	void ResourceSetLookup::read(ByteArrayStream* buffer)
	{
		variantDescLength = buffer->readByte();
		variantDesc = new char[variantDescLength + 1];
		for (int i=0; i<variantDescLength; i++)
		{
			variantDesc[i] = buffer->readByte();
		}
		variantDesc[variantDescLength] = 0;
		//variantDesc = "screenSize:small";

		lookup = new short[numberOfVariantResources];
		for (int i=0; i<numberOfVariantResources; i++)
		{
			lookup[i] = buffer->readShort();
		}

		priority = new byte[numberOfVariantResources];
		for (int i=0; i<numberOfVariantResources; i++)
		{
			priority[i] = buffer->readByte();
		}
	}

	void ResourceSetLookup::print()
	{
		PRINTINT(variantDescLength);
		PRINTSTRING(variantDesc)
		PRINTINTARRAY(lookup, numberOfVariantResources)
		PRINTINTARRAY(priority, numberOfVariantResources)
	}

	char* ResourceSetLookup::getVariantDescription()
	{
		return variantDesc;
	}

	byte ResourceSetLookup::getPriority(int index)
	{
		return priority[index];
	}

	short ResourceSetLookup::getLookup(int index)
	{
		return lookup[index];
	}
}
