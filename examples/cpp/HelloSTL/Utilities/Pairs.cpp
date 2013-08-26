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
 * @file Pairs.cpp
 * @author Gabriela Rata
 */

#include <string>
#include <cassert>
#include <utility>
#include "../LOG.h"
#include "../STLMoblet.h"


void STLMoblet::STL_pair()
{
	using std::pair;
	LOG("\n");
	LOG("========================= example using pair ====================================================================");
	LOG("\n");

	LOG("/**");
	LOG(" * The pair structure is provided");
	LOG(" * to treat two values as a single unit");
	LOG(" * The pair structure is defined in the <utility> header");
	LOG(" */");
	LOG("\n");

	LOG("/**");
	LOG(" * default constructor");
	LOG(" */");

	pair<float, char> p1;
	log_to_console("pair<float, char> p1;");

	LOG("\n");
	LOG("/**");
	LOG(" * initialize the  pair object");
	LOG(" */");

	TRACE(p1.first = 0.09;);
	TRACE(p1.second = 'A';);

	LOG("\n");
	LOG("/**");
	LOG(" * The template function make_pair creates a pair object");
	LOG(" * without the user having to write the types explicitly");
	LOG(" */");
	LOG("\n");

	pair<float, char> p2(std::make_pair(10.12, 'B'));
	LOG("pair<float, char> p2(std::make_pair(10.12, 'B'));	//copy constructor called");

	LOG("\n");
	LOG("/**");
	LOG(" * assignment operator");
	LOG(" */");

	TRACE(p1 = p2;);

	LOG("\n");
	LOG("/**");
	LOG(" * operator ==");
	LOG(" * returns true if p1.first == p2.first && p1.second == p2.second");
	LOG(" */");
	LOG("\n");
	bool res = (p1==p1);
	log_to_console(res, "p1 == p2 returns ");

	LOG("\n");
	LOG("/**");
	LOG(" * operator !=");
	LOG(" * implemented as !(operator==(arg1, arg2))");
	LOG(" */");
	LOG("\n");

	TRACE(p1.first -= 20;);
	res = (p1!=p2);
	log_to_console(res, "p1 != p2 returns ");

	LOG("\n");
	LOG("/**");
	LOG(" * operator <");
	LOG(" * returns true if p1.first < p2.first && p1.second < p2.second");
	LOG(" */");
	LOG("\n");
	log_to_console((bool)(p1>p2), "p1 < p2 returns ");
	log_to_console("\n");

	LOG("\n");
	LOG("/**");
	LOG(" * operator >");
	LOG(" * returns true if p1.first > p2.first && p1.second > p2.second");
	LOG(" */");
	LOG("\n");
	res = (p2>p1);
	log_to_console((bool)res, "p2 > p1 returns ");
	log_to_console("\n");

	TRACE(p1 = p2;);

	LOG("\n");
	LOG("/**");
	LOG(" * operator <=");
	LOG(" * implemented as (!(p2 < p1))");
	LOG(" */");
	LOG("\n");
	res = (p1<=p2);
	log_to_console((bool)res, "p1 <= p2 returns ");

	LOG("\n");
	LOG("/**");
	LOG(" * operator >=");
	LOG(" * implemented as if (!(p1 < p2))");
	LOG(" */");
	LOG("\n");
	res = (p1>=p2);
	log_to_console((bool)res, "p1 >= p2 returns ");

	LOG("\n");
}
