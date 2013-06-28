/* Copyright (C) 2010 MoSync AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
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
