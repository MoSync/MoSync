#ifndef __E32BASE_H
#define __E32BASE_H

#include <stdint.h>
#include <wchar.h>

typedef void TAny;
typedef uint8_t TUint8;
typedef int8_t TInt8;
typedef uint16_t TUint16;
typedef int16_t TInt16;
typedef uint32_t TUint32;
typedef int32_t TInt32;
typedef uint64_t TUint64;
typedef int64_t TInt64;
typedef TInt32 TInt;
typedef TUint32 TUint;
typedef bool TBool;

//typedef wchar_t TText;
typedef char TText;

const TInt KErrNone = 0;
const TInt KErrGeneral = -1;

class TVersion;
class SCapabilitySet;
#define IMPORT_C

template<class T> T* PtrAdd(T* ptr, int val) { return (T*) (((uint8_t*)ptr) + val); }

#define _FOFF(type, field) ((intptr_t)&(((type*)NULL)->field))

template<class T> T Min(T a, T b) { if (a < b) return a; return b; }

const TBool EFalse = false;
const TBool ETrue = true;

class User {
public:
	static void Invariant();
};

#define __ASSERT_ALWAYS(x, y) do { if (!(x)) { y; } } while (0)
#define ASSERT(x) do { if (!(x)) { fprintf(stderr, "Assertion failed at %s:%d\n", __FILE__, __LINE__); } } while (0)

#endif
