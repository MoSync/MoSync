// H_UTL.CPP
//
// Copyright (c) 1995-2004 Symbian Software Ltd.  All rights reserved.
//


#define __INCLUDE_CAPABILITY_NAMES__

#ifdef __MSVCDOTNET__
#include <string>
#else //!__MSVCDOTNET__
#include <string.h>
#endif //__MSVCDOTNET__

#include <stdarg.h>
#include <stdlib.h>

#include <e32std.h>

#include "h_utl.h"

TBool PVerbose=ETrue;

HPrint H;

HPrint::~HPrint()
{
	iLogFile.close();
}

void HPrint::SetLogFile(TText *aFileName)
{
	iLogFile.open((const char *)aFileName);
}

TInt HPrint::PrintString(TPrintType aType,const char *aFmt,...)
//
// Print text, noting where to send it.
//
{

	TInt r=KErrNone;
	va_list list;
	va_start(list,aFmt);
	vsnprintf((char *)iText,KMaxStringLength,aFmt,list);
	va_end(list);
	switch (aType)
	{
	case EAlways:
		cout << iText;
		iLogFile << iText;
		break;
	case EScreen:
		cout << iText;
		break;
	case ELog:
		if (iVerbose)
			cout << iText;
		iLogFile << iText;
		break;
	case EWarning:
		cerr << "WARNING: " << iText;
		iLogFile << "WARNING: "<<iText;
		break;
	case EError:
		cerr << "ERROR: " << iText;
		iLogFile << "ERROR: " << iText;
		r=KErrGeneral;
		break;
	case EPeError:
		if (PVerbose)
		{
			cerr << "ERROR: " << iText;
			iLogFile << "ERROR: " << iText;
		}
		r=KErrGeneral;
		break;
	case ESevereError:
		cerr << "ERROR: " << iText;
		iLogFile << "ERROR: " << iText;
		r=KErrGeneral;
		break;
	case EDiagnostic:
		cerr << "DIAGNOSTIC MESSAGE: " << iText;
		iLogFile << "DIAGNOSTIC MESSAGE: "<<iText;
		break;
	default:
		cerr << "ERROR: Invalid print type" << endl;
		r=KErrGeneral;
	}
	cout.flush();
	iLogFile.flush();
	return r;
}
