/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/**
 * @file  Test_auto_ptr.h
 * @author Gabriela Rata
 */

#ifndef TEST_AUTO_PTR_H_
#define TEST_AUTO_PTR_H_


/**
 *  test_auto_ptr: small function for showing the usage of auto_ptr template class from STL
 *  The auto_ptr is a smart pointer class template.
 *  It has a default constructor and a constructor that takes a pointer to its generic type.
 *  The auto_ptr takes ownership of the provided pointer. The pointer is deleted automatically
 *  when the auto_ptr object is destroyed.

 *  The auto_ptr is defined in <memory> header
 */

void test_auto_ptr();

#endif /* TEST_AUTO_PTR_H_ */
