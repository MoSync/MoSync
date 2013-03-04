#ifndef NBUILD_H
#define NBUILD_H

#include <vector>
#include <string>
#include <map>

#include "args.h"

using namespace std;

#define MACRO_DEFINES "-D"
#define NAME "--name"
#define CONFIGURATION "--config"
#define BINARY_TYPE "--lib-variant" // 'debug' or 'release'
#define PROJECT_DIR "--project"
#define OUTPUT_DIR "--dst"
#define MODULE_LIST "--modules"
#define SOURCE_FILES "--source-files"
#define EXCLUDE_FILES "--exclude-files"
#define VERBOSE "--verbose"
#define CLEAN "--clean"

string require(Arguments* args, string key);
const char* mosyncdir();
void sh(const char* cmd, bool hideOutput = false, const char* shownCmdLine = 0);

#endif
