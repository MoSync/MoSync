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
