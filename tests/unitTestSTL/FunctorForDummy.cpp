/*
 * FunctorForDummy.cpp
 *
 *  Created on: Jun 1, 2011
 *      Author: gabi
 */

#include "FunctorForDummyClass.h"

bool FunctorForDummy::operator()(const DummyClass &lv, const DummyClass &rv) const
{
	return lv.getName() < rv.getName();
}


bool BinaryPredicateForDummy(const DummyClass &lv, const DummyClass &rv)
{
	return lv.getName() < rv.getName();
}

