/*
 * Test_auto_ptr.h
 *
 *  Created on: May 26, 2011
 *      Author: gabi
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
