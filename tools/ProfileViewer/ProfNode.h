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
