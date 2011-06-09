#include "extensions.h"
#include "dll/dll.h"
#include "FileStream.h"
#include "base_errors.h"

using namespace Base;
using namespace MoSyncError;

#define EXT_ASSERT(func) MYASSERT(func, ERR_EXT_LOAD)

// each function is void(void), but has access to registers and memory.
// no function needs to know its number.
typedef void (*VoidFunction)(void);

// each DLL has two functions:
typedef int (*GetIdlHash)(void);
typedef VoidFunction* (*GetFunctions)(int*);

static VoidFunction* sFunctions;
static int snFunctions;

static int readIntLine(char*& pos, const char* format) {
	int len, result;
	int res = sscanf(pos, format, &result, &len);
	EXT_ASSERT(res == 1);
	pos += len;
	EXT_ASSERT(*pos == '\n');
	pos++;
	return result;
}

static const char* readLine(char*& pos) {
	const char* line = pos;
	char* end = strchr(pos, '\n');
	EXT_ASSERT(end);
	*end = 0;
	pos = end + 1;
	return line;
}

void loadExtensions(const char* extConfFileName) {
	// read file
	FileStream fs(extConfFileName);
	EXT_ASSERT(fs.isOpen());
	int len;
	EXT_ASSERT(fs.length(len));
	char* buf = new char[len+1];
	EXT_ASSERT(buf);
	EXT_ASSERT(fs.read(buf, len));

	// prepare for parsing
	char* end = buf+len;
	*end = 0;
	char* pos = buf;
	// load NumberOfFunctions
	snFunctions = readIntLine(pos, "%i%n");
	sFunctions = new VoidFunction[snFunctions];
	EXT_ASSERT(sFunctions);
	int count = 0;

	// load each extension
	while(pos < end) {
		// read the conf
		const char* fileName = readLine(pos);
		int confHash = readIntLine(pos, "%x%n");

		// load the dll and get the base functions
		Dll dll;
		bool success = dll.open(fileName);
		EXT_ASSERT(success);
		GetIdlHash getIdlHash = (GetIdlHash)dll.get("getIdlHash");
		EXT_ASSERT(getIdlHash);
		GetFunctions getFunctions = (GetFunctions)dll.get("getFunctions");
		EXT_ASSERT(getFunctions);

		// check hash
		int dllHash = getIdlHash();
		MYASSERT(dllHash == confHash, ERR_EXT_VERSION);

		// copy function pointers
		int nFunc;
		VoidFunction* dllFunctions = getFunctions(&nFunc);
		EXT_ASSERT(dllFunctions);
		EXT_ASSERT(count + nFunc <= snFunctions);
		memcpy(sFunctions + count, dllFunctions, nFunc + sizeof(VoidFunction));
	}
	EXT_ASSERT(count == snFunctions);
	EXT_ASSERT(pos == end);
	// and we're done.
}
