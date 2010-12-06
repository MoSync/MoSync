/* Copyright (C) 2010 MoSync AB

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

#ifndef PACKAGE_H
#define PACKAGE_H

// Validity checks should be done on access.
struct SETTINGS {
	const char* program;
	const char* resource;
	const char* icon;
	const char* model;
	const char* dst;
	const char* name;
	const char* vendor;
	const char* uid;
	bool debug;
	bool silent;
};

void package(const SETTINGS&);

void testProgram(const SETTINGS&);
void testModel(const SETTINGS&);
void testDst(const SETTINGS&);
void testName(const SETTINGS&);
void testVendor(const SETTINGS& s);
void testUid(const SETTINGS&);

const char* mosyncdir();
void sh(const char* cmd, bool hideOutput = false);

#endif	//PACKAGE_H
