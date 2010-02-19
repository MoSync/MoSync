#include <vector>
#include <stdio.h>

using namespace std;

class Reporter {
private:
	static int sNum;
	int mNum;
public:
	Reporter() : mNum(sNum) {
		sNum++;
		printf("Construct %i\n", mNum);
	}
	Reporter(const Reporter& o) : mNum(sNum) {
		sNum++;
		printf("CopyConstruct %i\n", mNum);
	}
	Reporter& operator=(const Reporter& o) {
		mNum = o.mNum;
		printf("Assign %i\n", mNum);
		return *this;
	}
	~Reporter() {
		printf("Destruct %i\n", mNum);
	}
};

int Reporter::sNum = 0;

int main() {
	printf("Hello World\n");
	vector<Reporter> v(4);
	printf("pop_back\n");
	v.pop_back();
	printf("Erase\n");
	v.erase(v.begin() + 1);
	printf("Done\n");
	printf("Capacity: %i\n", v.capacity());
	return 0;
}
