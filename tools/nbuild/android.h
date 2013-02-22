#include <map>
#include <string>

using namespace std;

int buildAndroidNative(map<string, string>& params);
int generateMakefile(map<string, string>& params);
int executeNdkBuild(map<string, string>& params);
