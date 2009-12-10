// MAIN.H
//
// Copyright (c) 2000 Symbian Ltd.  All rights reserved.
//

// Global data is held in a structure allocated in main(). This is in order for the memory leakage
// mechanism to be able see the allocation of this data.
struct GlobalData
{
	StructHeaderArray			SHA;
	ResourceItemArrayStack		RIAStack;
	StructResourceItemStack		SRIStack;
	IndexTable					Index;
	LinkArray					Links;
	NameIdMap					ResourceNameIds;
	FileLineManager				FileLineHandler;
	NameIdMap                   EnumValues;
};

extern GlobalData *pG;
extern int verbose;
extern unsigned short logmemorysetting;		// remove this


extern int ParseSourceFile(FILE* aFile, unsigned short aYYDebug);
