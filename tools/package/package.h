#ifndef PACKAGE_H
#define PACKAGE_H

// Validity checks should be done on access.
struct SETTINGS {
	const char* program;
	const char* resource;
	const char* model;
	const char* dst;
	const char* name;
	const char* uid;
	bool debug;
};

void package(const SETTINGS&);

void testProgram(const SETTINGS&);
void testModel(const SETTINGS&);
void testDst(const SETTINGS&);
void testName(const SETTINGS&);

const char* mosyncdir();
void sh(const char* cmd);

#endif	//PACKAGE_H
