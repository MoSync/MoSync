#ifndef OSCLCONFIG_H_INCLUDED
#define OSCLCONFIG_H_INCLUDED

typedef unsigned char uint8;
typedef long long OSCL_NATIVE_INT64_TYPE;
typedef unsigned long long OSCL_NATIVE_UINT64_TYPE;
typedef wchar_t OSCL_NATIVE_WCHAR_TYPE;
typedef unsigned int uint32;
typedef int int32;
typedef char int8;
typedef short int16;
typedef unsigned short uint16;

#ifdef _MSC_VER
#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
#define DLL_EXPORT
#define DLL_IMPORT
#else
#error Unsupported compiler!
#endif

#ifdef GSM_AMR_EXPORTS
#define OSCL_EXPORT_REF DLL_EXPORT
#define OSCL_IMPORT_REF DLL_EXPORT
#else
#define OSCL_EXPORT_REF DLL_IMPORT
#define OSCL_IMPORT_REF DLL_IMPORT
#endif

#endif	//OSCLCONFIG_H_INCLUDED
