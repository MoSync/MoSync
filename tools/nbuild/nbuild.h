#ifndef NBUILD_H
#define NBUILD_H

#include <vector>
#include <string>
#include <map>

using namespace std;

string require(map<string, string>& params, string key);
const char* mosyncdir();
void sh(const char* cmd, bool hideOutput = false, const char* shownCmdLine = 0);

#endif
