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
 * @file FunctorForDummyClass.h
 * @author Gabriela Rata
 */

#ifndef FUNCTORFORDUMMYCLASS_H_
#define FUNCTORFORDUMMYCLASS_H_

#include "DummyClass.h"

/**
 * We have to provide a way to compare two DummyClass objects. For that we define a functor.
 * Functors are classes with a public member function operator() defined. They are treated like functions,
 * but have the advantage that can also store data.
 * We didn't provide an operator < in the DummyClass, so we have to provide some comparison criterion.
 * If we had provided an operator < for the DummyClass, then we could use the default comparison class (std::less).
 * because less::operator() calls operator<.
 * 		bool operator() (const T& x, const T& y) const		//T: the type of objects being compared
 * 		{
			return x<y;
		}

 */

struct FunctorForDummy
{
	bool operator()(const DummyClass &lv, const DummyClass &rv) const;
};

bool BinaryPredicateForDummy(const DummyClass &lv, const DummyClass &rv);

#endif /* FUNCTORFORDUMMYCLASS_H_ */
