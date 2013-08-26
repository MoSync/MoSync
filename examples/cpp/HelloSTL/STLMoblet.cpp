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
