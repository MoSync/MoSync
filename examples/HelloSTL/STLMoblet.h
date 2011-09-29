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
 * @file STLMoblet.h
 * @author Gabriela Rata
 */


#ifndef STLMOBLET_H_
#define STLMOBLET_H_

#include <ma.h>
#include <vector>
#include <algorithm>
#include <bitset>
#include <stdio.h>
#include <MAUtil/Moblet.h>

class STLMoblet : public MAUtil::Moblet
{
public:
	STLMoblet();
	~STLMoblet();
	void keyPressEvent(int keyCode);

private:

	void STL_containers();

	void STL_bitset();
//	void STL_deque();
//	void STL_list();
//	void STL_map();
//	void STL_set();
//	void STL_multiset();
//	void STL_priority_queue();
//	void STL_queue();
//	void STL_stack();
	void STL_vector();


	void STL_algorithms();
	void STL_adaptors();
	void STL_functors();

	void STL_iterators();

	void STL_string();

	void aux_functions();
	void STL_pair();
	void STL_auto_ptr();

	void pointer_adapters_explained();
	void functors_explained();
	void predicates_explained();

	void display_menu();
};

namespace NumericUtilities
{

	bool isEven(int x);
	bool isOdd(int x);
	bool isNegative(int x);

}//end NumericUtilities


#endif /* STLMOBLET_H_ */
