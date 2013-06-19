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

enum ProfileType { DEVICE_BASED, PLATFORM_BASED };

// Validity checks should be done on access.
struct SETTINGS {
	const char* program;
	const char* resource;
	const char* icon;
	const char* cppOutputDir;
	const char* csOutputDir;
	const char* model;
	const char* mosyncProjectPath;
	const char* dst;
	const char* name;
	const char* vendor;
	const char* version;
	const char* permissions;
	const char* extensions;
	const char* outputType;
	const char* s60v3uid;
	const char* s60v2uid;
	const char* s60cert;
	const char* s60key;
	const char* s60pass;
	const char* profileType;
	bool debug;
	bool silent;
	bool showPasswords;

	// API specific settings
	const char* nfc;

	// iOS specific settings
	const char* iOSCert;
	const char* iOSSdk;
	const char* iOSXcodeTarget;
	const char* iOSBundleId;
	bool iOSgenerateOnly;

	// Windows Phone specific settings
	const char* WPconfig;
	const char* WPtarget;
	bool WPgenerateOnly;
	const char* WPguid;
	const char* WPvsBuildPath;
	const char* WPIncludePaths;
	char* WPMacroDefines;

	// Android specific settings
    const char* androidPackage;
    const char* androidVersionCode;
    const char* androidKeystore;
    const char* androidStorePass;
    const char* androidAlias;
    const char* androidKeyPass;
    const char* androidInstallLocation;
    const char* androidManifestTemplate;
    const char* androidHeap;
    const char* androidSdkLocation;

    // BlackBerry specific settings
    const char* blackberryJde;
    const char* blackberrySignKey;
    bool blackberryPackAsCOD;

    // JavaME specific settings
    const char* javameKeystore;
    const char* javameStorePass;
    const char* javameAlias;
    const char* javameKeyPass;
};

void package(const SETTINGS&);

void testProgram(const SETTINGS&);
void testModel(const SETTINGS&);
void testDst(const SETTINGS&);
void testProfileType(const SETTINGS&);
void testName(const SETTINGS&);
void testVendor(const SETTINGS&);
void testVersion(const SETTINGS&);
void testExtensions(const SETTINGS&);
void testOutputType(const SETTINGS& s);
void testS60v3Uid(const SETTINGS&);
void testS60v2Uid(const SETTINGS&);

void testIOSCert(const SETTINGS&);
void testIOSSdk(const SETTINGS&);
void testCppOutputDir(const SETTINGS&);
void testCsOutputDir(const SETTINGS&);

void testIOSCert(const SETTINGS&);
void testIOSSdk(const SETTINGS&);

void testVsBuildPath(const SETTINGS& s);

void testAndroidPackage(const SETTINGS&);
void testAndroidVersionCode(const SETTINGS&);
void testAndroidInstallLocation(const SETTINGS&);
void testAndroidHeap(const SETTINGS& s);

void testBlackberryJde(const SETTINGS&);

void testJavaMESigning(const SETTINGS& s);

bool isJavaIdentifierStart(char ch);
bool isJavaIdentifierPart(char ch);

const char* mosyncdir();
void sh(const char* cmd, bool hideOutput = false, const char* hideCmdLine = 0);

#endif	//PACKAGE_H
