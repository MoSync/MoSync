/** 
* \file macpp.h
* \brief Defines and typedefs needed for libsupc++
* \author Fredrik Eldh
*/

#ifndef MACPP_H
#define MACPP_H

#if defined(__cplusplus) && defined(MAPIP)

//******************************************************************************
//Defines and typedefs needed for libsupc++
//******************************************************************************

namespace std {
	typedef unsigned long size_t;
}

#include "new"

#undef __EXCEPTIONS

#endif	//defined(__cplusplus) && defined(MAPIP)

#endif	//MACPP_H
