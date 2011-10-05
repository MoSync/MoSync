/*
Copyright (C) 2011 MoSync AB

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
 * @file STLMoblet.cpp
 * @author Gabriela Rata
 */

#include "STLMoblet.h"
#include "LOG.h"

STLMoblet::STLMoblet()
{
	display_menu();
}

void STLMoblet::keyPressEvent(int keyCode)
{
	switch(keyCode)
	{
	case MAK_BACK:
		maExit(0);
		break;
	case MAK_0:
		STL_containers();
		display_menu();
		break;
	case MAK_1:
		functors_explained();
		STL_functors();
		display_menu();
		break;
	case MAK_2:
		pointer_adapters_explained();
		STL_adaptors();
		display_menu();
		break;
	case MAK_3:
		predicates_explained();
		display_menu();
		break;
	case MAK_4:
		STL_algorithms();
		display_menu();
		break;
	case MAK_5:
		STL_string();
		display_menu();
		break;
	case MAK_6:
		aux_functions();
		display_menu();
		break;
	case MAK_7:
		STL_auto_ptr();
		display_menu();
		break;
	case MAK_8:
		STL_pair();
		display_menu();
		break;
	case MAK_9:
		STL_iterators();
		display_menu();
		break;
	}

}

void STLMoblet::STL_containers()
{
	STL_bitset();
	STL_list();
	STL_map();
	STL_set();
	STL_multiset();
	STL_priority_queue();
	STL_queue();
	STL_stack();
	STL_vector();
	STL_deque();
}


STLMoblet::~STLMoblet()
{

}

void STLMoblet::display_menu()
{
	LOG("Press: ");

	LOG("BACK - to exit");

	LOG("'0' - for containers");

	LOG("'1' - for functors");

	LOG("'2' - for adapters");

	LOG("'3' - for predicates");

	LOG("'4' - for algorithms");

	LOG("'5' - for string");

	LOG("'6' - for min, max, swap functions");

	LOG("'7' - auto_ptr");

	LOG("'8' - pair");

	LOG("'9' - iterators");
}

namespace NumericUtilities
{

bool isEven(int x)
{
	if(x%2 == 0)
	{
		LOG("Calling unary predicate isEven(%d). Returns true.", x);
	}
	else
	{
		LOG("Calling unary predicate isEven(%d). Returns false.", x);
	}
	return x%2 == 0;
}

bool isOdd(int x)
{
	return (x%2!=0);
}

bool isNegative(int x)
{
	return x < 0;
}

} //namespace NumericUtilities
