/*
    Makesis 4
    Copyright 2007 Martin Storsjö

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation; either version 2.1 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

    In addition, as a special exception, the copyright holder of this
    program gives permission to link the code of its release of Makesis 4
    with the OpenSSL project's "OpenSSL" library (or with modified
    versions of it that use the same license as the "OpenSSL" library) and
    with modified Symbian code under the Symbian Example Source license;
    and distribute the linked executables. You must obey the GNU Lesser
    General Public License in all respects for all of the code used other
    than "OpenSSL" and the Symbian copyrighted code. If you modify this
    file, you may extend this exception to your version of the file, but
    you are not obliged to do so. If you do not wish to do so, delete this
    exception statement from your version.

    Martin Storsjö
    martin@martin.st
*/

#ifndef __SISFILEGEN_H
#define __SISFILEGEN_H

#include <windows.h>
#include <tchar.h>
#include "parsepkg.h"

#include <stdint.h>
#include <stdio.h>

class CSISWriter;

class SISInfo;
class SISCapabilities;
class SISFileDescription;
class SISFileData;
class RawFile;
class SISArray;

enum TSISFileGeneratorException {
	ErrSISFileNotFound,
	ErrEmbeddedSIS,
	ErrCantWrite,
	ErrCantRead,
	ErrNoVendorName,
	ErrNoLocalizedVendorName,
	ErrCantSign
};

class CSISFileGenerator {
public:
	CSISFileGenerator(const CSISWriter *siswriter);
	~CSISFileGenerator();
	
	void GenerateSISFile(const wchar_t* target, bool stub);
		
protected:
	void WriteHeaderL(uint32_t uid1, uint32_t uid2, uint32_t uid3, FILE* out);
	SISInfo* CreateSISInfo(bool stub);
	SISCapabilities* GetCapabilities(const char* filename);
	SISCapabilities* GetE32Capabilities(const char* filename);
	SISCapabilities* GetPECapabilities(const char* filename);
	void GetFileDescription(const PKGLINEFILE* file, int index, SISFileDescription** fileDesc, SISFileData** fileData, bool langDep);
	bool GetLogo(const LOGO* logo, int index, SISFileDescription** fileDesc, SISFileData** fileData);
	void LoadFile(const wchar_t* filename, RawFile** rawFile, uint64_t* compressedSize, uint64_t* uncompressedSize, int* index, SISFileData** fileData);
	void LoadEmbeddedSis(const PKGLINEFILE* file, SISArray* embeddedArray, SISArray* dataUnitArray);
	
private:
	const CSISWriter* siswriter;

};

#endif
