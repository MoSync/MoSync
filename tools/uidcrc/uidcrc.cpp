// UIDCRC.CPP
//
// Copyright (c) 1999 Symbian Ltd.  All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <e32std.h>
#include <e32uid.h>

// Get round the privateness of the checksum etc.
// See also PE_TRAN.CPP TE32ImageUids

class TCheckedUidX : public TCheckedUid
	{
public:
	inline TCheckedUidX(TUint uids[3])
		: TCheckedUid(TUidType(TUid::Uid(uids[1]),TUid::Uid(uids[2]),TUid::Uid(uids[3]))) 
		{}
	};

int usage()
	{
	fprintf(stderr, "uidcrc <uid1> <uid2> <uid3> [ <outputfile> ]\n");
	return -1;
	}

int main(int argc, char* argv[])
	{
	if (argc<4 || argc>5)
		return usage();

	TUint uids[5];
	int i=0;

	for (i=1; i<4; i++)
		{
		char* endptr = (char *) "failed";
		uids[i] = strtoul(argv[i],&endptr,0);
		if (*endptr!='\0')
			{
			fprintf(stderr, "invalid uid%d >%s<\n",i,argv[i]);
			return -1;
			}
		}

	TCheckedUidX checked(uids);
	uids[4] = checked.CRC();

	if (argc==5)
		{
		FILE* fp=fopen(argv[4], "wb");
		if (fp==0)
			{
			perror("cannot open %s for writing\n");
			return -1;
			}
		for (i=1; i<5; i++)
			{
			TUint word=uids[i];
			unsigned char bytes[4];
			bytes[0] = (unsigned char)( word     &0xFF);
			bytes[1] = (unsigned char)((word>> 8)&0xFF);
			bytes[2] = (unsigned char)((word>>16)&0xFF);
			bytes[3] = (unsigned char)((word>>24)&0xFF);
			fwrite(bytes, 4, 1, fp);
			}
		fclose(fp);
		return 0;
		}

	printf("0x%08x 0x%08x 0x%08x 0x%08x\n", uids[1], uids[2], uids[3], uids[4]);
	return 0;
	}
