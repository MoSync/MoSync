/*
 * FunctorForDummyClass.h
 *
 *  Created on: Jun 1, 2011
 *      Author: gabi
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
