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

#ifndef ARGS_H_
#define ARGS_H_

#include <map>
#include <vector>
#include <string>

using namespace std;

class Arguments {
private:
	int mArgc;
	const char** mArgv;
	//map<string, vector<string>> mAliases;
public:
	Arguments(int argc, const char** argv) : mArgc(argc), mArgv(argv) { }
	string at(int ix);
	//void defineSwitchAlias(string key, string alternative);
	vector<string> all();
	map<string, string> getPrefixedArguments(string prefix);
	vector<string> getPrefixedList(string prefix, bool retainPrefix);
	string getSwitchValue(string value);
	bool isFlagSet(string flag);
	//vector<string> getRemaining();
};

#endif /* ARGS_H_ */
