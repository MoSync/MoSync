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
