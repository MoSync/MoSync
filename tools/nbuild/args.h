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
