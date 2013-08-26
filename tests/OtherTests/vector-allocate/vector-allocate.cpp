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
