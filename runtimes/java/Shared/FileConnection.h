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
