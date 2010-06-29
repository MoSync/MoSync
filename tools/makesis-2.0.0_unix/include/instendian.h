// insendian.h
// Copyright (c) 2004 P. A. Bagyenda <bagyenda@dsmagic.com>
//
// Defines some macros for Endian switching of structures and types in instform.h
// from symbian makesis package
#ifndef __INSTENDIAN_INCLUDED__
#define __INSTENDIAN_INCLUDED__

// ===========================================================================
// Macros for endian switch
// ===========================================================================

#ifdef __BIG_ENDIAN__
// byte re-order of  int4/uint4
#define flipi(x) do { \
assert(sizeof (x) == 4); \
unsigned char *__px = (unsigned char *)&(x); \
unsigned char __x = __px[0]; __px[0] = __px[3]; __px[3] = __x; \
__x = __px[1]; __px[1] = __px[2]; __px[2] = __x; \
} while(0)

// byte re-order of short/ushort
#define flips(x) do { \
assert(sizeof (x) == 2); \
unsigned char *__px = (unsigned char *)&(x); \
unsigned char __x = __px[0]; __px[0] = __px[1]; __px[1] = __x; \
} while(0)
	
// Macros for each compound type
#define flipTInstString(x) do {\
	flipi((x).iCount); \
	flipi((x).iStringPtr); \
	} while (0)


#define flipTInstVersion(x) do { \
	flips((x).iMajor); \
	flips((x).iMinor); \
	flipi((x).iBuild); \
	} while(0)

#define flipTInstUID(x) do { \
	flipi((x).iUid[0]); \
	flipi((x).iUid[1]); \
	flipi((x).iUid[2]); \
	} while(0)

#define flipTInstFileOption(x) flipi((x).iComponentUid)

#define flipTInstFile(x) do {\
	flipi((x).iFileType); \
	flipTInstFileOption((x).iOption); \
	flipTInstString((x).iSource); \
	flipTInstString((x).iDestination); \
       } while(0)

#define flipTInstDependency(x) do { \
	flipi((x).iUid); \
	flipTInstVersion((x). iVersion); \
	} while(0)

#define flipTInstCapability(x) do {\
	flipi((x).iKey);\
	flipi((x).iValue);\
	} while(0)

// This type is a union, but the two fields both begin with an int32, so
// we can simply flip the larger of them.
#define flipTInstCondPrim(x) do {\
	flipTInstString((x).iString);\
	} while(0)

// Same for this one

#define flipTInstCondExpr(x) flipi((x).iExprType)

#define flipTInstOptions(x) flipi((x).iNumOptions)


#define flipTInstTimeStamp(x) do {\
	flips((x).iYear); \
	flips((x).iMonth); \
	flips((x).iDay); \
	flips((x).iHour); \
	flips((x).iMinute); \
	flips((x).iSecond); \
       } while(0)

#define flipTInstCertBlock(x) do {\
	flipTInstTimeStamp((x).iTimeStamp); \
	flipi((x).iNumCerts); \
	} while(0)

#define flipTInstPackageLine(x) flipi((x).iPackageLineType)

#define flipTInstInstallation(x) do { \
	flipTInstUID((x).iThisUID); \
	flipi((x).iUidChecksum); \
	flips((x).iChecksum); \
        flips((x).iNumLanguages); \
	flips((x).iNumPkgLines); \
        flips((x).iNumDependencies); \
	flips((x).iLangChosen); \
	flips((x).iNumPkgLinesDone); \
	flips((x).iInstallDrive); \
	flips((x).iNumCapabilities); \
	flipi((x).iInstVersion); \
	flips((x).iFlags); \
	flips((x).iType); \
	flipTInstVersion((x).iCompVersion); \
	flipi((x).iLanguageBlock); \
	flipi((x).iPackageLinesBlock); \
	flipi((x).iDependenciesBlock); \
	flipi((x).iCertificatesBlock); \
	flipi((x).iNames); \
	flipi((x).iSignatureBlock); \
	flipi((x).iCapabilitiesBlock); \
	flipi((x).iSpaceUsed); \
	flipi((x).iMaximumSpaceNeeded); \
        for (unsigned long __j = 0; __j < KInstallSISSpareLen; __j++) flips((x).iSpare2[__j]); \
	} while(0)

// Macros for simple arrays
#define pflipi(a,n) do {\
          for (unsigned long __j = 0; __j < (n); __j++) flipi((a)[__j]); \
          } while(0)

#define pflips(a,n) do {\
          for (unsigned long __j = 0; __j < (n); __j++) flips((a)[__j]); \
          } while(0)

#else
#define flipi(x) (void)0
#define flips(x) (void)0
#define flipTInstString(x) (void)0
#define flipTInstVersion(x) (void)0
#define flipTInstUID(x) (void)0
#define flipTInstFile(x) (void)0
#define flipTInstDependency(x) (void)0
#define flipTInstCapability(x) (void)0
#define flipTInstCondPrim(x) (void)0
#define flipTInstFileOption(x) (void)0
#define flipTInstCondExpr(x) (void)0
#define flipTInstOptions(x) (void)0
#define flipTInstTimeStamp(x) (void)0
#define flipTInstCertBlock(x) (void)0
#define flipTInstPackageLine(x) (void)0
#define flipTInstInstallation(x) (void)0
#define pflipi(a,n) (void)0
#define pflips(a,n) (void)0
#endif


#endif
