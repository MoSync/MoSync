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
	const char* version;
	const char* permissions;
	const char* s60v3uid;
	const char* s60v2uid;
	bool debug;
	bool silent;

	// iOS specific settings
	const char* iOSCert;
	const char* iOSSdk;
	bool iOSgenerateOnly;
};

void package(const SETTINGS&);

void testProgram(const SETTINGS&);
void testModel(const SETTINGS&);
void testDst(const SETTINGS&);
void testName(const SETTINGS&);
void testVendor(const SETTINGS&);
void testVersion(const SETTINGS&);
void testS60v3Uid(const SETTINGS&);
void testS60v2Uid(const SETTINGS&);

void testIOSCert(const SETTINGS&);
void testIOSSdk(const SETTINGS&);

const char* mosyncdir();
void sh(const char* cmd, bool hideOutput = false);

#endif	//PACKAGE_H
