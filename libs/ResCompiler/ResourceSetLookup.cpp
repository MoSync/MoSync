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
 * @file ResourceSetLookup.cpp
 * @author Florin Leu
 * @date 22 Nov 2011
 **/

#include "ResourceSetLookup.h"

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
