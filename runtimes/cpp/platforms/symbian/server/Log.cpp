/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
