#include <map>
#include <string>

#include "args.h"

using namespace std;

int buildAndroidNative(Arguments* args);
int generateMakefile(Arguments* args);
string toMakefileFile(string file);
int executeNdkBuild(Arguments* args);
string getTempBuildDir(Arguments* params);
