#ifndef _MOSYNC_EXTENSION_COMMON_H_
#define _MOSYNC_EXTENSION_COMMON_H_

// Each function is void(void), but has access to registers and memory.
// No function needs to know its number.
typedef void (*VoidFunction)(void);

struct ExtensionData {
	int idlHash;
	unsigned int nFunctions;
	VoidFunction* functions;
};
struct CoreData {
	int* regs;
	void* memDs;
};

// Each Extension has this function.
// Don't mix 32-bit and 64-bit code. (Should be impossible on most systems.)
typedef void (*InitializeExtension)(ExtensionData*, const CoreData*);

#endif	//_MOSYNC_EXTENSION_COMMON_H_
