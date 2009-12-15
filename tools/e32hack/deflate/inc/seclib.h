// e32tools/inc/seclib.h
//
// Copyright (c) 2004 Symbian Software Ltd.  All rights reserved.
//
// Get the capabilities of an emulator / e32image.
//

/**
* Image security information structure.
*
* @internalTechnology
* @prototype
*/
struct SBinarySecurityInfo
{
	TUint32			iSecureId;
	TUint32			iVendorId;
	TUint8			iCapabilities[KCapabilitySetMaxSize];
	TBool			iE32Image;
};

/**
* Extracts security information from an image.
*
* @internalTechnology
* @prototype
*/
TInt GetSecurityInfo(const char* aFileName, SBinarySecurityInfo& aInfo);
