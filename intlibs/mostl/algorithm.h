/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

/* 
 * File:   algorithm.h
 * Author: Ali Mosavian
 *
 * Created on July 22, 2009
 */

#ifndef __MOSTL__ALGORITHM_H__
#define	__MOSTL__ALGORITHM_H__

#include "common.h"


NAMESPACE_BEGIN( mostd )

/**
*  @brief This does what you think it does.
*  @param  a  A thing of arbitrary type.
*  @param  b  Another thing of arbitrary type.
*  @return   The lesser of the parameters.
*
*  This is the simple classic generic implementation.  It will work on
*  temporary expressions, since they are only evaluated once, unlike a
*  preprocessor macro.
*/
template<typename _Tp>
inline const _Tp&
min(const _Tp& __a, const _Tp& __b)
{
  //return __b < __a ? __b : __a;
  if (__b < __a)
    return __b;
  return __a;
}

/**
*  @brief This does what you think it does.
*  @param  a  A thing of arbitrary type.
*  @param  b  Another thing of arbitrary type.
*  @return   The greater of the parameters.
*
*  This is the simple classic generic implementation.  It will work on
*  temporary expressions, since they are only evaluated once, unlike a
*  preprocessor macro.
*/
template<typename _Tp>
inline const _Tp&
max(const _Tp& __a, const _Tp& __b)
{
  //return  __a < __b ? __b : __a;
  if (__a < __b)
    return __b;
  return __a;
}

/**
 * @brief Clamps a variable between to values
 * @param  l  Low boundry
 * @param  h  High boundry
 * @param  v  Value/variable to clamp
 * @return The clamped version of v
 *
 *  This is the simple classic generic implementation.  It will work on
 *  temporary expressions, since they are only evaluated once, unlike a
 *  preprocessor macro.
 */
template<typename _Tp>
inline _Tp
clamp ( const _Tp& __l, const _Tp& __v, const _Tp& __h )
{
    return min<_Tp>( __h, max<_Tp>( __l, __v ) );
}

/**
 * @brief Swaps to variables
 * @param  a  Variable one
 * @param  b  Variable two
 *
 *  This is the simple classic generic implementation.  It will work on
 *  temporary expressions, since they are only evaluated once, unlike a
 *  preprocessor macro.
 */
template<typename _Tp>
inline void swap ( _Tp &a, _Tp &b )
{
    _Tp t = a;
    a = b;
    b = t;
}


NAMESPACE_END

#endif	/* __MOSTL__ALGORITHM_H__ */

