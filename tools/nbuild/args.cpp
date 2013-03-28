#include "args.h"
#include "util.h"

using namespace std;

string Arguments::at(int ix) {
	if (ix < 0 || ix >= mArgc) {
		return "";
	}
	return mArgv[ix];
}

/*void Arguments::defineSwitchAlias(string key, string alternative) {
	vector aliasList = mAliases[key];
	aliasList.push_back(alternative);
}*/

string Arguments::getSwitchValue(string key) {
	for (int i = 0; i < mArgc; i++) {
		if (key == at(i)) {
			return at(i + 1);
		}
	}
	return "";
}

bool Arguments::isFlagSet(string key) {
	for (int i = 0; i < mArgc; i++) {
		if (key == at(i)) {
			return true;
		}
	}
	return false;
}

map<string, string> Arguments::getPrefixedArguments(string prefix) {
	map<string, string> result;
	for (int i = 0; i < mArgc; i++) {
		string arg = at(i);
		if (prefix.length() < arg.length() && arg.substr(0, prefix.length()) == prefix) {
			string trimmedArg = arg.substr(prefix.length());
			vector<string> keyAndValue;
			split(keyAndValue, trimmedArg, "=");
			result[keyAndValue[0]] = keyAndValue.size() > 1 ? keyAndValue[1] : "*";
		}
	}
	return result;
}

vector<string> Arguments::getPrefixedList(string prefix, bool retainPrefix) {
	vector<string> result;
	for (int i = 0; i < mArgc; i++) {
		string arg = at(i);
		if (prefix.length() < arg.length() && arg.substr(0, prefix.length()) == prefix) {
			string trimmedArg = retainPrefix ? arg : arg.substr(prefix.length());
			result.push_back(trimmedArg);
		}
	}
	return result;
}

vector<string> Arguments::all() {
	vector<string> result;
	for (int i = 0; i < mArgc; i++) {
		string arg = at(i);
		result.push_back(arg);
	}
	return result;
}
