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

#include <set>
#include <string>

using namespace std;

struct ProfNode;

struct sort_ProfNode_totalTime {
	// return true if x is strictly before y.
	bool operator()(const ProfNode* x, const ProfNode* y);
};

typedef set<ProfNode*, sort_ProfNode_totalTime> ProfSet;

struct ProfNode {
	enum Flags {
		N, C, CH, T, LT, CHT, _flags_end
	};

	ProfNode* const parent;
	const string name;
	const int count, nChildren;
	const float totalTime, childrenTime, localTime;
	ProfSet children;
};

bool sort_ProfNode_totalTime::operator()(const ProfNode* x, const ProfNode* y) {
	return x->totalTime > y->totalTime;
}
