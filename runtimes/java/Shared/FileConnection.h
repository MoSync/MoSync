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

//*****************************************************************************
//Variables
//*****************************************************************************
static class FileListing {
	Enumeration en;
	String last;	//non-null if this should be used instead of en.next().
	FileConnection fc;	//null if scanning roots
}

static class FileHandle {
	FileConnection fc;
	int position;
	int maxPos;	// maximum possible position. Use instead of fileSize() if greater.
	DataInputStream in;
	OutputStream out;

	// FileConnection.isDirectory() only works if the directory already exists.
	final boolean isDirectory() {
		String url = fc.getURL();
		return url.charAt(url.length()-1) == '/';
	}
}

Hashtable mFileListings = new Hashtable();
int mFileListNextHandle = 1;

Hashtable mFileHandles = new Hashtable();
int mFileNextHandle = 1;
