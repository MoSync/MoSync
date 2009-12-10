// RCSCAN.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include "fileline.h"

class rcscan : public yy_scan
{
public:
	rcscan(const FileLineManager& aFileLineHandler,FILE* aSourceFile);
	virtual void yyerror(const char *,...);
	int ErrorWasFound() const;
	rcscan& operator=(const rcscan&);
private:
	int iErrorFound;
	const FileLineManager& iFileLineHandler;
};
