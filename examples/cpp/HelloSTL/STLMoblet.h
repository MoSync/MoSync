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
	void STL_deque();
	void STL_list();
	void STL_map();
	void STL_set();
	void STL_multiset();
	void STL_priority_queue();
	void STL_queue();
	void STL_stack();
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
