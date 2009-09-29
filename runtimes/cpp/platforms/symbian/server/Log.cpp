/* Copyright (C) 2009 Mobile Sorcery AB

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

#include <f32file.h>

#include "Log.h"

_LIT(KLogFilePath, "C:\\data\\msslog.txt");
_LIT(KOldLogFilePath, "C:\\data\\msslogold.txt");

LOCAL_C void PanicIfError(TInt code) {
	if(code < 0) {
		User::Panic(KLogFilePath, code);
	}
}

class MyRFs {
public:
	MyRFs() {}
	void Connect() {
		PanicIfError(rfs.Connect());
	}
	~MyRFs() {
		rfs.Close();
	}
	RFs& operator()() { return rfs; }
private:
	RFs rfs;
};
#define FSS (myrfs())

LOCAL_C void AppendToFile(const TDesC& aFileName, const TDesC8& aText) {
	MyRFs myrfs;
	myrfs.Connect();
	RFile file;
	PanicIfError(file.Open(FSS, aFileName,
		EFileWrite | EFileShareAny | EFileStreamText));
	TInt pos = 0;
	PanicIfError(file.Seek(ESeekEnd, pos));
	PanicIfError(file.Write(aText));
	file.Close();
}

void LogV(const char* fmt, VA_LIST args) {
	TBuf8<512> buffer;
	TPtrC8 fmtP(CBP fmt);
	buffer.FormatList(fmtP, args);
	AppendToFile(KLogFilePath, buffer);
}

void InitLog() {
	{
		MyRFs myrfs;
		myrfs.Connect();
		FSS.Delete(KOldLogFilePath);
		FSS.Rename(KLogFilePath, KOldLogFilePath);
		{
			RFile file;
				PanicIfError(file.Create(FSS, KLogFilePath,
				EFileWrite | EFileShareExclusive | EFileStreamText));
			file.Close();
		}
	}
	Log("Logging started.\n");
}
