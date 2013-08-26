/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
