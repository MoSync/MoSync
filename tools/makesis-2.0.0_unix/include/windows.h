/*
 * Copyright (C) the Wine project
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __WINE_WINBASE_H
#define __WINE_WINBASE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Calling conventions definitions */

#if defined(__i386__) && !defined(_X86_)
# define _X86_
#endif

#ifndef __stdcall
# ifdef __i386__
#  ifdef __GNUC__
#   define __stdcall __attribute__((__stdcall__))
#  elif defined(_MSC_VER)
    /* Nothing needs to be done. __stdcall already exists */
#  else
#   error You need to define __stdcall for your compiler
#  endif
# else  /* __i386__ */
#  define __stdcall
# endif  /* __i386__ */
#endif /* __stdcall */

#ifndef __cdecl
# if defined(__i386__) && defined(__GNUC__)
#  define __cdecl __attribute__((__cdecl__))
# elif !defined(_MSC_VER)
#  define __cdecl
# endif
#endif /* __stdcall */

#ifndef __WINESRC__

#ifndef pascal
#define pascal      __stdcall
#endif
#ifndef _pascal
#define _pascal     __stdcall
#endif
#ifndef _stdcall
#define _stdcall    __stdcall
#endif
#ifndef _fastcall
#define _fastcall   __stdcall
#endif
#ifndef __fastcall
#define __fastcall  __stdcall
#endif
#ifndef __export
#define __export    __stdcall
#endif
#ifndef cdecl
#define cdecl       __cdecl
#endif
#ifndef _cdecl
#define _cdecl      __cdecl
#endif

#ifndef near
#define near
#endif
#ifndef far
#define far
#endif
#ifndef _near
#define _near
#endif
#ifndef _far
#define _far
#endif
#ifndef NEAR
#define NEAR
#endif
#ifndef FAR
#define FAR
#endif

#ifndef _declspec
#define _declspec(x)
#endif
#ifndef __declspec
#define __declspec(x)
#endif

#endif /* __WINESRC__ */

#define CALLBACK    __stdcall
#define WINAPI      __stdcall
#define APIPRIVATE  __stdcall
#define PASCAL      __stdcall
#define CDECL       __cdecl
#define _CDECL      __cdecl
#define WINAPIV     __cdecl
#define APIENTRY    WINAPI
#define CONST       const

/* Misc. constants. */

#undef NULL
#ifdef __cplusplus
#define NULL  0
#else
#define NULL  ((void*)0)
#endif

#ifdef FALSE
#undef FALSE
#endif
#define FALSE 0

#ifdef TRUE
#undef TRUE
#endif
#define TRUE  1

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef OPTIONAL
#define OPTIONAL
#endif

/* Standard data types */

#include <stdint.h>

typedef void                                   *LPVOID;
typedef const void                             *LPCVOID;
typedef int             BOOL,       *PBOOL,    *LPBOOL;
typedef unsigned char   BYTE,       *PBYTE,    *LPBYTE;
typedef unsigned char   UCHAR,      *PUCHAR;
typedef uint16_t        WORD,       *PWORD,    *LPWORD;
typedef unsigned short  USHORT,     *PUSHORT;
typedef int             INT,        *PINT,     *LPINT;
typedef unsigned int    UINT,       *PUINT;
typedef long                                   *LPLONG;
typedef uint32_t        DWORD,      *PDWORD,   *LPDWORD;
typedef unsigned long   ULONG,      *PULONG;
typedef float           FLOAT,      *PFLOAT;
typedef double          DOUBLE;
typedef double          DATE;

/* Define the basic types */
#ifndef VOID
#define VOID void
#endif
typedef VOID           *PVOID;
typedef BYTE            BOOLEAN,    *PBOOLEAN;
typedef char            CHAR,       *PCHAR;
typedef short           SHORT,      *PSHORT;
typedef long            LONG,       *PLONG;

/* Some systems might have wchar_t, but we really need 16 bit characters */
typedef wchar_t         WCHAR,      *PWCHAR;

/* ANSI string types */
typedef CHAR           *PCH,        *LPCH;
typedef const CHAR     *PCCH,       *LPCCH;
typedef CHAR           *PSTR,       *LPSTR,     *NPSTR;
typedef const CHAR     *PCSTR,      *LPCSTR;

/* Unicode string types */
typedef WCHAR          *PWCH,       *LPWCH;
typedef const WCHAR    *PCWCH,      *LPCWCH;
typedef WCHAR          *PWSTR,      *LPWSTR,    *NWPSTR;
typedef const WCHAR    *PCWSTR,     *LPCWSTR;

/* Neutral character and string types */
/* These are only defined for Winelib, i.e. _not_ defined for
 * the emulator. The reason is they depend on the UNICODE
 * macro which only exists in the user's code.
 */
#ifndef __WINESRC__
# ifdef WINE_UNICODE_REWRITE

/* Use this if your compiler does not provide a 16bit wchar_t type.
 * Note that you will need to specify -fwritable-strings or an option
 * to this effect.
 * In C++ both WINE_UNICODE_TEXT('c') and WINE_UNICODE_TEXT("str") are
 * supported, but only the string form can be supported in C.
 */
EXTERN_C unsigned short* wine_rewrite_s4tos2(const wchar_t* str4);
#  ifdef __cplusplus
inline WCHAR* wine_unicode_text(const wchar_t* str4)
{
  return (WCHAR*)wine_rewrite_s4tos2(str4);
}
inline WCHAR wine_unicode_text(wchar_t chr4)
{
  return (WCHAR)chr4;
}
#   define WINE_UNICODE_TEXT(x)       wine_unicode_text(L##x)
#  else  /* __cplusplus */
#   define WINE_UNICODE_TEXT(x)       ((WCHAR*)wine_rewrite_s4tos2(L##x))
#  endif  /* __cplusplus */

# else  /* WINE_UNICODE_REWRITE */
/* Define WINE_UNICODE_NATIVE if:
 * - your compiler provides a 16bit wchar_t type, e.g. gcc >= 2.96 with
 *   -fshort-wchar option
 * - or if you decide to use the native 32bit Unix wchar_t type. Be aware
 *   though that the Wine APIs only support 16bit WCHAR characters for
 *   binary compatibility reasons.
 * - or define nothing at all if you don't use Unicode, and blissfully
 *   ignore the issue :-)
 */
#  define WINE_UNICODE_TEXT(string)   L##string

# endif  /* WINE_UNICODE_REWRITE */

# ifdef UNICODE
typedef WCHAR           TCHAR,      *PTCHAR;
typedef LPWSTR          PTSTR,       LPTSTR;
typedef LPCWSTR         PCTSTR,      LPCTSTR;
#  define __TEXT(string) WINE_UNICODE_TEXT(string)
# else  /* UNICODE */
typedef CHAR            TCHAR,      *PTCHAR;
typedef LPSTR           PTSTR,       LPTSTR;
typedef LPCSTR          PCTSTR,      LPCTSTR;
#  define __TEXT(string) string
# endif /* UNICODE */
# define TEXT(quote) __TEXT(quote)
#endif   /* __WINESRC__ */

/* Misc common WIN32 types */
typedef char            CCHAR;
typedef LONG            HRESULT;
typedef DWORD           LCID,       *PLCID;
typedef WORD            LANGID;
typedef DWORD		EXECUTION_STATE;

/* Handle type */
typedef void *HANDLE;
typedef HANDLE *PHANDLE, *LPHANDLE;

/* Macros to map Winelib names to the correct implementation name */
/* depending on __WINESRC__ and UNICODE macros.                   */
/* Note that Winelib is purely Win32.                             */

#ifdef __WINESRC__
# define WINELIB_NAME_AW(func) \
    func##_must_be_suffixed_with_W_or_A_in_this_context \
    func##_must_be_suffixed_with_W_or_A_in_this_context
#else  /* __WINESRC__ */
# ifdef UNICODE
#  define WINELIB_NAME_AW(func) func##W
# else
#  define WINELIB_NAME_AW(func) func##A
# endif  /* UNICODE */
#endif  /* __WINESRC__ */

#ifdef __WINESRC__
# define DECL_WINELIB_TYPE_AW(type)  /* nothing */
#else   /* __WINESRC__ */
# define DECL_WINELIB_TYPE_AW(type)  typedef WINELIB_NAME_AW(type) type;
#endif  /* __WINESRC__ */


/* Handle types */

typedef int HFILE;

#define GENERIC_READ               0x80000000
#define GENERIC_WRITE              0x40000000
#define GENERIC_EXECUTE            0x20000000
#define GENERIC_ALL                0x10000000

/* Callback function pointers types */

typedef INT     (CALLBACK *FARPROC)();
typedef INT     (CALLBACK *PROC)();


/* Macros to split words and longs. */

#define LOBYTE(w)              ((BYTE)(WORD)(w))
#define HIBYTE(w)              ((BYTE)((WORD)(w) >> 8))

#define LOWORD(l)              ((WORD)(DWORD)(l))
#define HIWORD(l)              ((WORD)((DWORD)(l) >> 16))

#define SLOWORD(l)             ((SHORT)(LONG)(l))
#define SHIWORD(l)             ((SHORT)((LONG)(l) >> 16))

#define MAKEWORD(low,high)     ((WORD)(((BYTE)(low)) | ((WORD)((BYTE)(high))) << 8))
#define MAKELONG(low,high)     ((LONG)(((WORD)(low)) | (((DWORD)((WORD)(high))) << 16)))
#define MAKELPARAM(low,high)   ((LPARAM)MAKELONG(low,high))
#define MAKEWPARAM(low,high)   ((WPARAM)MAKELONG(low,high))
#define MAKELRESULT(low,high)  ((LRESULT)MAKELONG(low,high))

#define SELECTOROF(ptr)     (HIWORD(ptr))
#define OFFSETOF(ptr)       (LOWORD(ptr))

/* min and max macros */
#ifndef NOMINMAX
#ifndef max
#define max(a,b)   (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)   (((a) < (b)) ? (a) : (b))
#endif
#endif  /* NOMINMAX */

#ifdef MAX_PATH /* Work-around for Mingw */ 
#undef MAX_PATH
#endif /* MAX_PATH */

#define MAX_PATH        260
#define HFILE_ERROR     ((HFILE)-1)

/* The SIZE structure */
typedef struct tagSIZE
{
    LONG cx;
    LONG cy;
} SIZE, *PSIZE, *LPSIZE;

typedef SIZE SIZEL, *PSIZEL, *LPSIZEL;

/* The POINT structure */
typedef struct tagPOINT
{
    LONG  x;
    LONG  y;
} POINT, *PPOINT, *LPPOINT;

typedef struct _POINTL
{
    LONG x;
    LONG y;
} POINTL;

/* The POINTS structure */

typedef struct tagPOINTS
{
    SHORT x;
    SHORT y;
} POINTS, *PPOINTS, *LPPOINTS;

/* The RECT structure */
typedef struct tagRECT
{
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
} RECT, *PRECT, *LPRECT;
typedef const RECT *LPCRECT;

typedef struct tagRECTL
{
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
} RECTL, *PRECTL, *LPRECTL;

typedef const RECTL *LPCRECTL;

#ifdef __cplusplus
}
#endif

/* File attribute flags */
#define FILE_SHARE_READ			0x00000001L
#define FILE_SHARE_WRITE		0x00000002L
#define FILE_SHARE_DELETE		0x00000004L
#define FILE_ATTRIBUTE_READONLY         0x00000001L
#define FILE_ATTRIBUTE_HIDDEN           0x00000002L
#define FILE_ATTRIBUTE_SYSTEM           0x00000004L
#define FILE_ATTRIBUTE_LABEL            0x00000008L  /* Not in Windows API */
#define FILE_ATTRIBUTE_DIRECTORY        0x00000010L
#define FILE_ATTRIBUTE_ARCHIVE          0x00000020L
#define FILE_ATTRIBUTE_NORMAL           0x00000080L
#define FILE_ATTRIBUTE_TEMPORARY        0x00000100L
#define FILE_ATTRIBUTE_ATOMIC_WRITE     0x00000200L
#define FILE_ATTRIBUTE_XACTION_WRITE    0x00000400L
#define FILE_ATTRIBUTE_COMPRESSED       0x00000800L
#define FILE_ATTRIBUTE_OFFLINE		0x00001000L


/* File creation flags
 */
#define FILE_FLAG_WRITE_THROUGH    0x80000000UL
#define FILE_FLAG_OVERLAPPED 	   0x40000000L
#define FILE_FLAG_NO_BUFFERING     0x20000000L
#define FILE_FLAG_RANDOM_ACCESS    0x10000000L
#define FILE_FLAG_SEQUENTIAL_SCAN  0x08000000L
#define FILE_FLAG_DELETE_ON_CLOSE  0x04000000L
#define FILE_FLAG_BACKUP_SEMANTICS 0x02000000L
#define FILE_FLAG_POSIX_SEMANTICS  0x01000000L
#define CREATE_NEW              1
#define CREATE_ALWAYS           2
#define OPEN_EXISTING           3
#define OPEN_ALWAYS             4
#define TRUNCATE_EXISTING       5

/* Standard handle identifiers
 */
#define STD_INPUT_HANDLE        ((DWORD) -10)
#define STD_OUTPUT_HANDLE       ((DWORD) -11)
#define STD_ERROR_HANDLE        ((DWORD) -12)

#define INVALID_HANDLE_VALUE     ((HANDLE) -1)
#define INVALID_FILE_SIZE        ((DWORD)0xFFFFFFFF)
#define INVALID_SET_FILE_POINTER ((DWORD)-1)
#define INVALID_FILE_ATTRIBUTES  ((DWORD)-1)

#define FILE_BEGIN              0
#define FILE_CURRENT            1
#define FILE_END                2

#define _MAX_FNAME          256
#define _MAX_DIR            _MAX_FNAME
#define _MAX_EXT            _MAX_FNAME


//
// stubs
//
BOOL   WINAPI CreateDirectory(LPCSTR pathname, void*);
UINT   WINAPI GetTempPath(UINT maxlen, LPSTR ptr);
HANDLE WINAPI CreateFileA(LPCSTR,DWORD,DWORD,void*,DWORD,DWORD,HANDLE);
BOOL   WINAPI ReadFile(HANDLE,LPVOID,DWORD,LPDWORD,void*);
BOOL   WINAPI CloseHandle(HANDLE);
DWORD  WINAPI GetFileSize(HANDLE,LPDWORD);
BOOL   WINAPI WriteFile(HANDLE,LPCVOID,DWORD,LPDWORD,void*);
DWORD  WINAPI SetFilePointer(HANDLE,LONG,LPLONG,DWORD);
BOOL   WINAPI RemoveDirectory(LPCSTR);
int _wunlink(const wchar_t*);


//
// TODO:
//
char*       _fullpath(char*,const char*, size_t);


#define CP_ACP					0
#define CP_OEMCP				1
#define CP_MACCP				2
#define CP_THREAD_ACP				3
#define CP_SYMBOL				42
#define CP_UTF7					65000
#define CP_UTF8					65001


#ifdef __cplusplus
extern "C" {
#endif


INT WINAPI MultiByteToWideChar(UINT,DWORD,LPCSTR,INT,LPWSTR,INT);
INT WINAPI WideCharToMultiByte(UINT,DWORD,LPCWSTR,INT,LPSTR,INT,LPCSTR,LPBOOL);


#ifdef __cplusplus
}
#endif

#endif
