#include <set>
#include <string>

using namespace std;

struct ProfNode;

class sort_ProfNode_totalTime {
	// return true if x is strictly before y.
	bool operator()(const ProfNode* x, const ProfNode* y);
};

struct ProfNode {
	enum Flags {
		N, C, CH, T, LT, CHT, _flags_end
	};

	const ProfNode* parent;
	const string name;
	const int count, nChildren;
	const float totalTime, childrenTime, localTime;
	set<ProfNode*, sort_ProfNode_totalTime> children;
};

bool sort_ProfNode_totalTime::operator()(const ProfNode* x, const ProfNode* y) {
	return x->totalTime > y->totalTime;
}
