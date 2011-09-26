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
 * @file  DummyClass.cpp
 * @author Gabriela Rata
 */


#include "DummyClass.h"
#include "LOG.h"

DummyClass::DummyClass(const std::string &name):mName(name)
{
}
DummyClass::DummyClass(const DummyClass &rv)
{
	mName = rv.mName;
}
DummyClass &DummyClass::operator=(const DummyClass &rv)
{
	if(&rv != this)
	{
		mName = rv.mName;
	}
	return *this;
}
DummyClass::~DummyClass()
{
}
void DummyClass::SomeFunction()
{
}

std::string DummyClass::getName()const
{
	return mName;
}

bool operator==(const DummyClass &lv, const DummyClass &rv)
{
	return lv.mName==rv.mName;
}
