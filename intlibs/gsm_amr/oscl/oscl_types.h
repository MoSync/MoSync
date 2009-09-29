/* ------------------------------------------------------------------
 * Copyright (C) 2008 PacketVideo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 * -------------------------------------------------------------------
 */
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//       O S C L _ T Y P E S   ( B A S I C   T Y P E D E F S )

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclbase OSCL Base
 *
 * @{
 */


/*! \file oscl_types.h
 *  \brief This file contains basic type definitions for common use across platforms.
 *
 */

#ifndef OSCL_TYPES_H_INCLUDED
#define OSCL_TYPES_H_INCLUDED


// include the config header for the platform
#ifndef OSCLCONFIG_H_INCLUDED
#include "osclconfig.h"
#endif

//! The c_bool type is mapped to an integer to provide a bool type for C interfaces
typedef int c_bool;


//! The OsclAny is meant to be used the context of a generic pointer (i.e., no specific type).
typedef void OsclAny;

//! mbchar is multi-byte char (e.g., UTF-8) with null termination.
typedef char mbchar;

//! The uint type is a convenient abbreviation for unsigned int.
#if !defined(__USE_MISC)
// uint is defined in some Linux platform sys\types.h
typedef unsigned int uint;
#endif

//! The octet type is meant to be used for referring to a byte or collection bytes without suggesting anything about the underlying meaning of the bytes.
typedef uint8 octet;

//! The Float type defined as OsclFloat
typedef float OsclFloat;

#ifndef OSCL_INT64_TYPES_DEFINED
//use native type
typedef OSCL_NATIVE_INT64_TYPE int64;
//use native type
typedef OSCL_NATIVE_UINT64_TYPE uint64;
#define OSCL_INT64_TYPES_DEFINED
#endif

// define OSCL_WCHAR
typedef OSCL_NATIVE_WCHAR_TYPE oscl_wchar;

//! define OSCL_TCHAR
typedef oscl_wchar OSCL_TCHAR;

#ifndef OSCL_MEMFRAG_PTR_BEFORE_LEN
#define OSCL_MEMFRAG_PTR_BEFORE_LEN 0
#endif

// The definition of the MemoryFragment will probably
// be OS-dependant since the goal is to allow this data
// structure to be passed directly to I/O routines that take
// scatter/gather arrays.
#if ( OSCL_MEMFRAG_PTR_BEFORE_LEN )

struct OsclMemoryFragment
{
    void *ptr;
    uint32 len;
};

#else
struct OsclMemoryFragment
{
    uint32 len;
    void *ptr;
};
#endif


/*! @} */


#endif  // OSCL_TYPES_H_INCLUDED
