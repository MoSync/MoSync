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
