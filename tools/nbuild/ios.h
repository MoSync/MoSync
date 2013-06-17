#include <map>
#include <string>

#include "args.h"

using namespace std;

int buildIOSNative(Arguments* args);
int compileIOSUserCode(Arguments* params);
int prepareIOSFramework(Arguments* params);
string getTempBuildDir(Arguments* params);
