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

#include <stdio.h>
#include <string>
#include <windows.h>
#include <process.h>

#include <helpers/attribute.h>

using namespace std;

static void error(const char *message) GCCATTRIB(noreturn);
static void error(const char *message) {
	MessageBox(NULL, message, "MoSyncUpdater error", MB_OK | MB_ICONERROR); 
	exit(1);
}

static void notify(const char *message) {
	MessageBox(NULL, message, "MoSyncUpdater", MB_OK | MB_ICONINFORMATION); 
}

static bool invoke_bat(const char *file) {
	SHELLEXECUTEINFO si;
	memset(&si,0,sizeof(si));
	si.cbSize=sizeof(si);
	si.fMask=SEE_MASK_FLAG_DDEWAIT|SEE_MASK_NOCLOSEPROCESS;
	si.hwnd=GetDesktopWindow();
	si.lpVerb="open"; // Open
	si.lpFile=file;
	si.nShow=SW_SHOWNORMAL;//SW_SHOWNORMAL; SW_HIDE
	si.lpDirectory="";
	si.lpParameters="";
	if(ShellExecuteEx(&si)==TRUE) {
		DWORD dwSignaled = ::WaitForSingleObject( si.hProcess , INFINITE );
		// WAIT_OBJECT_0 is success for the event trigger
		if ( dwSignaled == WAIT_OBJECT_0 ) {
			// Returns Non-Zero on success
			DWORD dwRet;
			if( ::GetExitCodeProcess( si.hProcess , &dwRet ) ) {
				return true;
			}
		}   
	}
	return false;
}

static bool exists(const char* filename) {
	FILE* file = fopen(filename, "rb");
	if(file == NULL) {
		return false;
	}
	fclose(file);
	return true;
}

int main(int argc, char **argv) {
	if(argc!=3) {
		error("Wrong arguments\n"
			"should be:\n"
			"MoSyncUpdater pid <exe to be started post update, a path relative to %MOSYNCDIR%>"
			);
	}

	const char *tempDir = getenv("MOSYNCDIR");

	if(!tempDir) {
		error("No MOSYNCDIR environment variable!");
	}

	string mosyncdir = string(tempDir);

	// we have a pid we should wait for
	DWORD dwPID = atoi(argv[1]);
	HANDLE   hProc ;
	hProc = OpenProcess(SYNCHRONIZE, FALSE, dwPID);

	if(hProc) {
		while(1) {
			DWORD ret = WaitForSingleObject(hProc, 5000);
			if(ret == WAIT_TIMEOUT) {
				notify("Could not close MoBuild, please close it manually.");
			} else {
				break;
			}
		}
		CloseHandle(hProc);
	} else {
		//it's already closed, let's go ahead and unpack.
#if 0
		char temp[256];
		sprintf(temp, "OpenProcess failed\nerror code: %d\n", GetLastError());
		error(temp);
#endif
	}

	string unzipdir = mosyncdir + "\\bin\\unzip";
	string updatedir = mosyncdir + "\\update.zip";
	string cmd =  unzipdir + " -o " + updatedir + " -d " + mosyncdir;
	if(system(cmd.c_str()) != 0) {
		error("Could not unzip update.zip");
	}

	string updatebat =  mosyncdir + "\\update.bat";
	//cmd = "cmd /c " + updatebat;

	if(exists(updatebat.c_str())) {
		if(!invoke_bat(updatebat.c_str())) {
			notify("Could not invoke update.bat");
		}

		if(remove(updatebat.c_str())) {
			notify(("Could not remove " + updatebat).c_str());
		}	
	}

	if(remove(updatedir.c_str())) {
		notify(("Could not remove " + updatedir).c_str());
	}

	notify("Update successful!");

	//string mobuildguidir = mosyncdir + "/bin/MoBuildGUI.exe";
	//string mobuildguidir = mosyncdir + "/eclipse/mosync.exe";
	string mobuildguidir = mosyncdir + string(argv[2]);
	
	//if( _spawnl(_P_NOWAIT, mobuildguidir.c_str(), " ", NULL, NULL) == -1) {
	//	error("could not restart mobuild");
	//}

	//maybe CreateProcess can make MoBuild not start in the background

	if((int)ShellExecute(NULL, NULL, mobuildguidir.c_str(), NULL, NULL, SW_SHOWNORMAL) <= 32) {
		error("Could not restart MoBuild");
	}

	return 0;
}
