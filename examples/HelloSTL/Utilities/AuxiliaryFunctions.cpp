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
 * @file AuxiliaryFunctions.cpp
 * @author Gabriela Rata
 */


#include <algorithm>
#include "../STLMoblet.h"
#include "../LOG.h"
#include "../DummyClass.h"
#include "../Employee.h"


/**
 * Function for explaining auxiliary functions from the <algorithm> header: min, max, swap
 */
void STLMoblet::aux_functions()
{
	LOG("\n");
	LOG("========================= min, max and swap functions (found in <algorithm> header) =============================");
	LOG("/**");
	LOG(" * min, max template functions: If both values are equal, usually the first element is returned");
	LOG(" * 	Both functions are provided with an additional argument, that is the comparison criterion.");
	LOG(" * 	The default comparison criterion is operator<.");
	LOG(" * swap: provided to swap the values of two objects.");
	LOG(" * 	(The swap is possible only if the copy constructor and the assignment operators are defined.)");
	LOG(" */");
	LOG("\n");

	TRACE(int small = 10;);
	TRACE(int big = 20;);

	int resMin = std::min(small, big);
	log_to_console(resMin, "int resMin = std::min(small, big);	//resMin = ");

	int resMax = std::max(small, big);
	log_to_console(resMax, "int resMax = std::max(small, big);	//resMax = ");
	log_to_console("\n");

	TRACE(Employee junior(1, "Jimmy"););
	TRACE(Employee bigBoss(12, "Alex"););

	Employee smallE = std::min(junior, bigBoss, lessExperience);

	log_to_console(smallE, "Employee smaller = std::min(junior, bigBoss, "
			"lessExperience);	//smallE = ");

	Employee bigE = std::max(junior, bigBoss, lessExperience);
	log_to_console(bigE, "Employee big = std::max(junior, bigBoss, lessExperience);	//bigE = ");
	log_to_console("\n");

	TRACE(std::swap(smallE, bigE));
	log_to_console(smallE, "smallE after swap:  ");
	log_to_console(bigE, "bigE after swap: ");

	LOG("\n");
}
