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

#ifndef __SISFIELD_H
#define __SISFIELD_H

#include <wchar.h>
//#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include <time.h>

#include "instform.h"

#ifdef _MSC_VER
#pragma warning(disable:4244)
#endif

namespace SISFieldType {
	enum SISFieldType {
		SISString = 1,
		SISArray = 2,
		SISCompressed = 3,
		SISVersion = 4,
		SISVersionRange = 5,
		SISDate = 6,
		SISTime = 7,
		SISDateTime = 8,
		SISUid = 9,
		SISUnused1 = 10,
		SISLanguage = 11,
		SISContents = 12,
		SISController = 13,
		SISInfo = 14,
		SISSupportedLanguages = 15,
		SISSupportedOptions = 16,
		SISPrerequisites = 17,
		SISDependency = 18,
		SISProperties = 19,
		SISProperty = 20,
		SISSignatures = 21,
		SISCertificateChain = 22,
		SISLogo = 23,
		SISFileDescription = 24,
		SISHash = 25,
		SISIf = 26,
		SISElseIf = 27,
		SISInstallBlock = 28,
		SISExpression = 29,
		SISData = 30,
		SISDataUnit = 31,
		SISFileData = 32,
		SISSupportedOption = 33,
		SISControllerChecksum = 34,
		SISDataChecksum = 35,
		SISSignature = 36,
		SISBlob = 37,
		SISSignatureAlgorithm = 38,
		SISSignatureCertificateChain = 39,
		SISDataIndex = 40,
		SISCapabilities = 41
	};
}

enum SISFieldError {
	SISFieldBadSourceFile,
	SISFieldDecompressErr,
	SISFieldCompressErr,
	SISFieldReadErr
};

enum TCompressionAlgorithm {
	ECompressAuto = -1,
	ECompressNone = 0,
	ECompressDeflate
};

class SISField {
public:
	virtual ~SISField() {}
	virtual uint32_t Id() const = 0;
	virtual uint64_t Length() const = 0;
	virtual void CopyFieldData(uint8_t*& aPtr) const = 0;

	virtual uint64_t PaddedLength() const {
		uint64_t len = Length();
		len += (4 - (len & 3)) & 3;
		return len;
	}
	virtual uint64_t HeaderDataLength() const {
		return PaddedLength() + 8;
	}
	static void WriteUint32(uint8_t*& aPtr, uint32_t aValue) {
		*aPtr++ = (aValue >>  0) & 0xff;
		*aPtr++ = (aValue >>  8) & 0xff;
		*aPtr++ = (aValue >> 16) & 0xff;
		*aPtr++ = (aValue >> 24) & 0xff;
	}
	static void WriteUint64(uint8_t*& aPtr, uint64_t aValue) {
		WriteUint32(aPtr, aValue & 0xffffffff);
		WriteUint32(aPtr, (aValue >> 32) & 0xffffffff);
	}
	static void WriteUint16(uint8_t*& aPtr, uint16_t aValue) {
		*aPtr++ = (aValue >>  0) & 0xff;
		*aPtr++ = (aValue >>  8) & 0xff;
	}
	virtual void CopyHeaderData(uint8_t*& aPtr) const {
		WriteUint32(aPtr, Id());
		WriteUint32(aPtr, Length());
		CopyData(aPtr);
	}
	virtual void CopyData(uint8_t*& aPtr) const {
		uint64_t len = Length();
		uint64_t padded = PaddedLength();
		CopyFieldData(aPtr);
		while (len < padded) {
			*aPtr++ = 0;
			len++;
		}
	}
	virtual void CopyHeaderData(FILE* aOut) {
		uint8_t* buffer = new uint8_t[HeaderDataLength()];
		uint8_t* bufferptr = buffer;
		CopyHeaderData(bufferptr);
		fwrite(buffer, 1, HeaderDataLength(), aOut);
		delete [] buffer;
	}
	virtual void CopyData(FILE* aOut) const {
		uint8_t* buffer = new uint8_t[PaddedLength()];
		uint8_t* bufferptr = buffer;
		CopyData(bufferptr);
		fwrite(buffer, 1, PaddedLength(), aOut);
		delete [] buffer;
	}
	static uint32_t ReadUint32(const uint8_t*& aPtr, uint32_t& aLength) {
		if (aLength < 4)
			throw SISFieldReadErr;
		uint32_t val = 0;
		val |= *aPtr++ <<  0;
		val |= *aPtr++ <<  8;
		val |= *aPtr++ << 16;
		val |= *aPtr++ << 24;
		aLength -= 4;
		return val;
	}
	static uint64_t ReadUint64(const uint8_t*& aPtr, uint32_t& aLength) {
		uint64_t val = 0;
		val |= ReadUint32(aPtr, aLength);
		val |= ((uint64_t)ReadUint32(aPtr, aLength)) << 32;
		return val;
	}
	static uint16_t ReadUint16(const uint8_t*& aPtr, uint32_t& aLength) {
		if (aLength < 2)
			throw SISFieldReadErr;
		uint16_t val = 0;
		val |= *aPtr++ <<  0;
		val |= *aPtr++ <<  8;
		aLength -= 2;
		return val;
	}
	static uint8_t ReadUint8(const uint8_t*& aPtr, uint32_t& aLength) {
		if (aLength < 1)
			throw SISFieldReadErr;
		uint8_t val = 0;
		val |= *aPtr++;
		aLength -= 1;
		return val;
	}
	virtual void Load(const uint8_t* aBuffer, uint32_t aLength) = 0;
	static SISField* LoadOneField(const uint8_t*& aBuffer, uint32_t& aLength) {
		if (aLength > 0) {
			uint32_t type = ReadUint32(aBuffer, aLength);
			uint32_t length = ReadUint32(aBuffer, aLength);
			SISField* field = NewObject(type);
			field->Load(aBuffer, length);
			length += (4 - (length & 3)) & 3;
			aBuffer += length;
			aLength -= length;
			return field;
		}
		return NULL;
	}

	static SISField* NewObject(uint32_t aType);
private:
};

class SISCompound : public SISField {
protected:
	SISCompound(uint32_t aType) : iType(aType) {
		iArraySize = 10;
		iNumElements = 0;
		iElements = new SISField*[iArraySize];
	}
public:
	~SISCompound() {
		for (uint32_t i = 0; i < iNumElements; i++)
			delete iElements[i];
		delete [] iElements;
	}
	uint32_t Id() const {
		return iType;
	}
	uint64_t Length() const {
		uint32_t sum = 0;
		for (uint32_t i = 0; i < iNumElements; i++)
			sum += iElements[i]->HeaderDataLength();
		return sum;
	}
	virtual void AddElement(SISField* aField) {
		if (iNumElements == iArraySize) {
			iArraySize *= 2;
			SISField** newArray = new SISField*[iArraySize];
			for (uint32_t i = 0; i < iNumElements; i++)
				newArray[i] = iElements[i];
			delete [] iElements;
			iElements = newArray;
		}
		iElements[iNumElements++] = aField;
	}
	void CopyFieldData(uint8_t*& aPtr) const {
		for (uint32_t i = 0; i < iNumElements; i++) {
			iElements[i]->CopyHeaderData(aPtr);
		}
	}
	void Load(const uint8_t* aBuffer, uint32_t aLength) {
		while (aLength > 0) {
			SISField* field = LoadOneField(aBuffer, aLength);
			AddElement(field);
		}
	}
	void Reset() {
		for (uint32_t i = 0; i < iNumElements; i++)
			delete iElements[i];
		iNumElements = 0;
	}
	SISField* FindElement(uint32_t aType) {
		for (uint32_t i = 0; i < iNumElements; i++) {
			if (iElements[i]->Id() == aType) {
				return iElements[i];
			}
		}
		return NULL;
	}
	SISField* FindRemoveElement(uint32_t aType) {
		for (uint32_t i = 0; i < iNumElements; i++) {
			if (iElements[i]->Id() == aType) {
				SISField* field = iElements[i];
				for (uint32_t j = i; j < iNumElements-1; j++)
					iElements[j] = iElements[j+1];
				iNumElements--;
				return field;
			}
		}
		return NULL;
	}
	SISField* FindRemoveLastElement(uint32_t aType) {
		for (int i = iNumElements-1; i >= 0; i--) {
			if (iElements[i]->Id() == aType) {
				SISField* field = iElements[i];
				for (uint32_t j = i; j < iNumElements-1; j++)
					iElements[j] = iElements[j+1];
				iNumElements--;
				return field;
			}
		}
		return NULL;
	}
	uint32_t Count() {
		return iNumElements;
	}
	SISField* ElementAt(uint32_t aIndex) {
		if ((int)aIndex >= 0 && aIndex < iNumElements)
			return iElements[aIndex];
		return NULL;
	}
protected:
	uint32_t iType;
	SISField** iElements;
	uint32_t iNumElements;
	uint32_t iArraySize;
};

class SISArray : public SISCompound {
public:
	SISArray() : SISCompound(0) {
	}
	SISArray(uint32_t aType) : SISCompound(aType) {
	}
	uint32_t Id() const {
		return SISFieldType::SISArray;
	}
	uint32_t ChildType() const {
		return iType;
	}
	uint64_t Length() const {
		uint32_t sum = 4;
		for (uint32_t i = 0; i < iNumElements; i++)
			sum += 4 + iElements[i]->PaddedLength();
		return sum;
	}
	void AddElement(SISField* aField) {
		if (aField->Id() != iType) {
			fprintf(stderr, "Adding field of id %d to array of id %d\n", aField->Id(), iType);
			abort();
		}
		SISCompound::AddElement(aField);
	}
	void CopyFieldData(uint8_t*& aPtr) const {
		WriteUint32(aPtr, iType);
		for (uint32_t i = 0; i < iNumElements; i++) {
			WriteUint32(aPtr, iElements[i]->Length());
			iElements[i]->CopyData(aPtr);
		}
	}
	void Load(const uint8_t* aBuffer, uint32_t aLength) {
		iType = ReadUint32(aBuffer, aLength);
		while (aLength > 0) {
			uint32_t length = ReadUint32(aBuffer, aLength);
			SISField* field = NewObject(iType);
			field->Load(aBuffer, length);
			AddElement(field);
			length += (4 - (length & 3)) & 3;
			aBuffer += length;
			aLength -= length;
		}
	}
};

class SISString : public SISField {
public:
	SISString() {
		iLength = 0;
		iStr = NULL;
	}

	SISString(const wchar_t* aStr) {
		iLength = wcslen(aStr);
		iStr = new wchar_t[iLength + 1];
		wcscpy(iStr, aStr);
	}
	~SISString() {
		delete [] iStr;
	}
	uint32_t Id() const {
		return SISFieldType::SISString;
	}
	uint64_t Length() const {
		return 2*iLength;
	}
	void CopyFieldData(uint8_t*& aPtr) const {
		for (uint32_t i = 0; i < iLength; i++)
			WriteUint16(aPtr, iStr[i]);
	}
	void Load(const uint8_t* aBuffer, uint32_t aLength) {
		iLength = aLength/2;
		iStr = new wchar_t[iLength + 1];
		for (uint32_t i = 0; i < iLength; i++)
			iStr[i] = ReadUint16(aBuffer, aLength);
		iStr[iLength] = '\0';
	}

private:
	uint32_t iLength;
	wchar_t* iStr;
};

class RawFile : public SISField {
public:
	RawFile(const char* aFileName) {
		iName = _strdup(aFileName);
		FILE* fp = fopen(iName, "rb");
		if (!fp) {
			perror(iName);
			throw SISFieldBadSourceFile;
		}
		fseek(fp, 0, SEEK_END);
		iLength = ftell(fp);
		fclose(fp);
	}
	~RawFile() {
		free(iName);
	}
	uint32_t Id() const {
		return 0;
	}
	uint64_t Length() const {
		return iLength;
	}
	uint64_t PaddedLength() const {
		return Length();
	}
	uint64_t HeaderDataLength() const {
		return PaddedLength();
	}
	void CopyHeaderData(uint8_t*& aPtr) const {
		CopyData(aPtr);
	}
	void CopyData(uint8_t*& aPtr) const {
		CopyFieldData(aPtr);
	}
	void CopyFieldData(uint8_t*& aPtr) const {
		FILE* fp = fopen(iName, "rb");
		uint64_t n = fread(aPtr, 1, iLength, fp);
		aPtr += n;
		if (n != iLength) {
			fprintf(stderr, "Not enough data returned from file %s (requested %lld bytes, got %lld bytes)\n", iName, iLength, n);
			throw SISFieldBadSourceFile;
		}
		fclose(fp);
	}
	void Load(const uint8_t* aBuffer, uint32_t aLength) {
		// FIXME?
	}

private:
	uint64_t iLength;
	char* iName;
};

class SISCompressed : public SISField {
public:
	SISCompressed() {
		iField = NULL;
		iAlgorithm = ECompressAuto;
		iUncompressedSize = iCompressedSize = 0;
		iCompressedBuffer = iUncompressedBuffer = NULL;
	}
	SISCompressed(SISField* aField, TCompressionAlgorithm aAlgorithm = ECompressAuto) : iField(aField) {
		iCompressedBuffer = iUncompressedBuffer = NULL;
		LoadUncompressed();
		Compress(aAlgorithm);
	}
	~SISCompressed() {
		delete iField;
		delete [] iCompressedBuffer;
		delete [] iUncompressedBuffer;
	}
	void LoadUncompressed() {
		iUncompressedSize = iField->HeaderDataLength();
		delete [] iUncompressedBuffer;
		iUncompressedBuffer = new uint8_t[iUncompressedSize];
		uint8_t* bufferptr = iUncompressedBuffer;
		iField->CopyHeaderData(bufferptr);

		delete [] iCompressedBuffer;
		iCompressedBuffer = NULL;
	}
	void Compress(TCompressionAlgorithm aAlgorithm = ECompressAuto) {
		if (!iUncompressedBuffer && !iCompressedBuffer)
			LoadUncompressed();
		if (aAlgorithm == ECompressAuto || aAlgorithm == ECompressDeflate) {
			if (iCompressedBuffer)
				return;
			delete [] iCompressedBuffer;
			iCompressedBuffer = CompressDeflate(iUncompressedBuffer, iUncompressedSize, &iCompressedSize);
			if (iCompressedSize < iUncompressedSize || aAlgorithm == ECompressDeflate) {
				iAlgorithm = ECompressDeflate;
				delete [] iUncompressedBuffer;
				iUncompressedBuffer = NULL;
			} else {
				iAlgorithm = ECompressNone;
				delete [] iCompressedBuffer;
				iCompressedBuffer = NULL;
			}
		} else {
			if (!iUncompressedBuffer)
				Uncompress();
			delete [] iCompressedBuffer;
			iCompressedBuffer = NULL;
			iAlgorithm = ECompressNone;
		}
	}
	void Uncompress() {
		if (!iCompressedBuffer)
			return;
		delete [] iUncompressedBuffer;
		iUncompressedBuffer = new uint8_t[iUncompressedSize];
		uLongf uncompressed = iUncompressedSize;
		int ret = uncompress(iUncompressedBuffer, &uncompressed, iCompressedBuffer, iCompressedSize);
		if (ret != Z_OK) {
			fprintf(stderr, "Error %d in decompress\n", ret);
			throw SISFieldDecompressErr;
		}
		iUncompressedSize = uncompressed;
		delete [] iCompressedBuffer;
		iCompressedBuffer = NULL;
	}
	void LoadChild() {
		delete iField;
		const uint8_t* ptr = iUncompressedBuffer;
		uint32_t length = iUncompressedSize;
		iField = LoadOneField(ptr, length);
		delete [] iUncompressedBuffer;
		iUncompressedBuffer = NULL;
	}
	static uint8_t* CompressDeflate(const uint8_t* aData, uint64_t aLength, uint64_t* aCompressedLength) {
		// the zlib docs say 0.1% more + 12 bytes so this should be more than enough
		uint64_t outSize = 1024 + (uint64_t) (aLength*1.01);
		uint8_t* outBuf = new uint8_t[outSize];
/*
		z_stream stream;
		stream.zalloc = Z_NULL;
		stream.zfree = Z_NULL;
		stream.opaque = Z_NULL;
		stream.next_out = outBuf;
		stream.avail_out = outSize;
		stream.next_in = (Bytef*) aData;
		stream.avail_in = aLength;
		int ret = deflateInit(&stream, Z_DEFAULT_COMPRESSION);
		if (ret != Z_OK) {
			fprintf(stderr, "Error %d in deflateInit: %s\n", ret, stream.msg);
			throw SISFieldCompressErr;
		}
		ret = deflate(&stream, Z_FINISH);
		if (ret != Z_STREAM_END) {
			fprintf(stderr, "Error %d in deflate: %s\n", ret, stream.msg);
			throw SISFieldCompressErr;
		}
		*aCompressedLength = outSize - stream.avail_out;
		deflateEnd(&stream);
*/
		uLongf compressed = outSize;
		int ret = compress(outBuf, &compressed, aData, aLength);
		if (ret != Z_OK) {
			fprintf(stderr, "Error %d in compress\n", ret);
			throw SISFieldCompressErr;
		}
		*aCompressedLength = compressed;

		return outBuf;
	}
	uint32_t Id() const {
		return SISFieldType::SISCompressed;
	}
	uint64_t Length() const {
		if (iCompressedBuffer)
			return 12 + iCompressedSize;
		return 12 + iUncompressedSize;
	}
	uint64_t CompressedSize() const {
		if (iCompressedBuffer)
			return iCompressedSize;
		return iUncompressedSize;
	}
	void CopyFieldData(uint8_t*& aPtr) const {
		WriteUint32(aPtr, iAlgorithm);
		WriteUint64(aPtr, iUncompressedSize);
		if (iCompressedBuffer) {
			memcpy(aPtr, iCompressedBuffer, iCompressedSize);
			aPtr += iCompressedSize;
		} else {
			memcpy(aPtr, iUncompressedBuffer, iUncompressedSize);
			aPtr += iUncompressedSize;
		}
	}
	void Load(const uint8_t* aBuffer, uint32_t aLength) {
		iAlgorithm = (TCompressionAlgorithm) ReadUint32(aBuffer, aLength);
		iUncompressedSize = ReadUint64(aBuffer, aLength);
		iCompressedSize = aLength;
		if (iAlgorithm == ECompressNone) {
			if (iCompressedSize != iUncompressedSize) {
				throw SISFieldReadErr;
			}
			delete [] iUncompressedBuffer;
			iUncompressedBuffer = new uint8_t[iUncompressedSize];
			memcpy(iUncompressedBuffer, aBuffer, aLength);
			delete [] iCompressedBuffer;
			iCompressedBuffer = NULL;
		} else {
			delete [] iCompressedBuffer;
			iCompressedBuffer = new uint8_t[iCompressedSize];
			memcpy(iCompressedBuffer, aBuffer, aLength);
			delete [] iUncompressedBuffer;
			iUncompressedBuffer = NULL;
		}
	}
	SISField* Field() {
		return iField;
	}
	void DetachField() {
		iField = NULL;
	}
	TCompressionAlgorithm Algorithm() {
		return iAlgorithm;
	}
private:
	TCompressionAlgorithm iAlgorithm;
	SISField* iField;
	uint64_t iUncompressedSize;
	uint8_t* iUncompressedBuffer;
	uint8_t* iCompressedBuffer;
	uint64_t iCompressedSize;
};

class SISVersion : public SISField {
public:
	SISVersion() {
	}
	SISVersion(uint32_t aMajor, uint32_t aMinor, uint32_t aBuild) : iMajor(aMajor), iMinor(aMinor), iBuild(aBuild) {
	}
	uint32_t Id() const {
		return SISFieldType::SISVersion;
	}
	uint64_t Length() const {
		return 12;
	}
	void CopyFieldData(uint8_t*& aPtr) const {
		WriteUint32(aPtr, iMajor);
		WriteUint32(aPtr, iMinor);
		WriteUint32(aPtr, iBuild);
	}
	void Load(const uint8_t* aBuffer, uint32_t aLength) {
		iMajor = ReadUint32(aBuffer, aLength);
		iMinor = ReadUint32(aBuffer, aLength);
		iBuild = ReadUint32(aBuffer, aLength);
	}

private:
	uint32_t iMajor, iMinor, iBuild;
};

class SISVersionRange : public SISCompound {
public:
	SISVersionRange(SISVersion* aFrom = NULL, SISVersion* aTo = NULL) : SISCompound(SISFieldType::SISVersionRange) {
		if (aFrom)
			AddElement(aFrom);
		if (aTo)
			AddElement(aTo);
	}
};

class SISDate : public SISField {
public:
	SISDate(uint16_t aYear, uint8_t aMonth, uint8_t aDay) : iYear(aYear), iMonth(aMonth), iDay(aDay) {
	}
	SISDate() {
		time_t timestamp = time(NULL);
#ifdef WIN32
		struct tm* tptr = gmtime(&timestamp);
#else
		struct tm tm;
		gmtime_r(&timestamp, &tm);
		struct tm* tptr = &tm;
#endif
		iYear = tptr->tm_year + 1900;
		iMonth = tptr->tm_mon;
		iDay = tptr->tm_mday;
	}
	uint32_t Id() const {
		return SISFieldType::SISDate;
	}
	uint64_t Length() const {
		return 4;
	}
	void CopyFieldData(uint8_t*& aPtr) const {
		WriteUint16(aPtr, iYear);
		*aPtr++ = iMonth;
		*aPtr++ = iDay;
	}
	void Load(const uint8_t* aBuffer, uint32_t aLength) {
		iYear = ReadUint16(aBuffer, aLength);
		iMonth = ReadUint8(aBuffer, aLength);
		iDay = ReadUint8(aBuffer, aLength);
	}

public:
	uint16_t iYear;
	uint8_t iMonth, iDay;
};

class SISTime : public SISField {
public:
	SISTime(uint8_t aHours, uint8_t aMinutes, uint8_t aSeconds) : iHours(aHours), iMinutes(aMinutes), iSeconds(aSeconds) {
	}
	SISTime() {
		time_t timestamp = time(NULL);
#ifdef WIN32
		struct tm* tptr = gmtime(&timestamp);
#else
		struct tm tm;
		gmtime_r(&timestamp, &tm);
		struct tm* tptr = &tm;
#endif
		iHours = tptr->tm_hour;
		iMinutes = tptr->tm_min;
		iSeconds = tptr->tm_sec;
	}
	uint32_t Id() const {
		return SISFieldType::SISTime;
	}
	uint64_t Length() const {
		return 3;
	}
	void CopyFieldData(uint8_t*& aPtr) const {
		*aPtr++ = iHours;
		*aPtr++ = iMinutes;
		*aPtr++ = iSeconds;
	}
	void Load(const uint8_t* aBuffer, uint32_t aLength) {
		iHours = ReadUint8(aBuffer, aLength);
		iMinutes = ReadUint8(aBuffer, aLength);
		iSeconds = ReadUint8(aBuffer, aLength);
	}

public:
	uint8_t iHours, iMinutes, iSeconds;
};


class SISDateTime : public SISCompound {
public:
	SISDateTime() : SISCompound(SISFieldType::SISDateTime) {
		AddElement(new SISDate());
		AddElement(new SISTime());
	}
	SISDateTime(SISDate* aDate, SISTime* aTime) : SISCompound(SISFieldType::SISDateTime) {
		AddElement(aDate);
		AddElement(aTime);
	}
	void Load(const uint8_t* aBuffer, uint32_t aLength) {
		Reset();
		SISCompound::Load(aBuffer, aLength);
	}
};

template<class T, uint32_t aType> class SISSimpleField : public SISField {
public:
	SISSimpleField() {
	}
	SISSimpleField(T aValue) : iValue(aValue) {
	}
	uint32_t Id() const {
		return aType;
	}
	uint64_t Length() const {
		return sizeof(T);
	}
	void CopyFieldData(uint8_t*& aPtr) const {
		for (uint32_t i = 0; i < sizeof(T); i++)
			*aPtr++ = (iValue >> (8*i)) & 0xff;
	}
	void Load(const uint8_t* aBuffer, uint32_t aLength) {
		if (aLength != sizeof(T))
			throw SISFieldReadErr;
		iValue = 0;
		for (uint32_t i = 0; i < sizeof(T); i++)
			iValue |= *aBuffer++ << (8*i);
	}

public:
	T iValue;
};

typedef SISSimpleField<uint32_t, SISFieldType::SISUid> SISUid;
typedef SISSimpleField<uint32_t, SISFieldType::SISLanguage> SISLanguage;

class SISBlob : public SISField {
public:
	SISBlob() {
		iBlob = NULL;
		iLength = 0;
		iField = NULL;
		iLocalArray = false;
	}
	SISBlob(const uint8_t* aBlob, uint64_t aLength) {
		iLength = aLength;
		uint8_t* buffer = new uint8_t[iLength];
		memcpy(buffer, aBlob, iLength);
		iBlob = buffer;
		iLocalArray = true;
	}
	SISBlob(SISField* aField) : iBlob(NULL), iLength(0), iField(aField) {
		iLocalArray = false;
	}
	~SISBlob() {
		if (iLocalArray)
			delete [] iBlob;
	}
	uint32_t Id() const {
		return SISFieldType::SISBlob;
	}
	uint64_t Length() const {
		if (iBlob)
			return iLength;
		return iField->HeaderDataLength();
	}
	void CopyFieldData(uint8_t*& aPtr) const {
		if (iBlob) {
			memcpy(aPtr, iBlob, iLength);
			aPtr += iLength;
		} else {
			iField->CopyHeaderData(aPtr);
		}
	}
	void Load(const uint8_t* aBuffer, uint32_t aLength) {
		iLength = aLength;
		uint8_t* buffer = new uint8_t[iLength];
		memcpy(buffer, aBuffer, iLength);
		iBlob = buffer;
		iLocalArray = true;
	}

private:
	const uint8_t* iBlob;
	uint64_t iLength;
	SISField* iField;
	bool iLocalArray;
};

typedef SISSimpleField<uint32_t, SISFieldType::SISDataIndex> SISDataIndex;
typedef SISSimpleField<uint16_t, SISFieldType::SISControllerChecksum> SISControllerChecksum;
typedef SISSimpleField<uint16_t, SISFieldType::SISDataChecksum> SISDataChecksum;

template<uint32_t aType, uint32_t aChildType> class SISArrayField : public SISCompound {
public:
	SISArrayField() : SISCompound(aType) {
	}
	SISArrayField(SISArray* aArray) : SISCompound(aType) {
		if (aArray->ChildType() != aChildType) {
			fprintf(stderr, "Array given to field of type %d has type %d\n", aType, aArray->ChildType());
			abort();
		}
		AddElement(aArray);
	}
};

typedef SISArrayField<SISFieldType::SISData, SISFieldType::SISDataUnit> SISData;

class SISContents : public SISCompound {
public:
	SISContents() : SISCompound(SISFieldType::SISContents) {
	}
	SISContents(SISControllerChecksum* aControllerChecksum, SISDataChecksum* aDataChecksum, SISCompressed* aController, SISData* aData) : SISCompound(SISFieldType::SISContents) {
		if (aControllerChecksum)
			AddElement(aControllerChecksum);
		if (aDataChecksum)
			AddElement(aDataChecksum);
		AddElement(aController);
		AddElement(aData);
	}
};
/*
enum TInstallType {
	EInstApplication,
	EInstAugmentation,
	EInstPartialUpgrade,
	EInstPreInstalledApp,
	EInstPreInstalledPatch
};

enum TInstallFlags {
	EInstFlagShutdownApps = 1
};
*/
class SISInfo : public SISCompound {
public:
	SISInfo() : SISCompound(SISFieldType::SISInfo) {
		iInstallType = 0;
		iInstallFlags = 0;
	}
	SISInfo(SISUid* aUid, SISString* aVendor, SISArray* aNames, SISArray* aVendorNames, SISVersion* aVersion, SISDateTime* aDateTime, uint8_t aInstallType, uint8_t aInstallFlags) : SISCompound(SISFieldType::SISInfo) {
		AddElement(aUid);
		AddElement(aVendor);
		AddElement(aNames);
		AddElement(aVendorNames);
		AddElement(aVersion);
		AddElement(aDateTime);
		iInstallType = aInstallType;
		iInstallFlags = aInstallFlags;
	}
	uint64_t Length() const {
		return SISCompound::Length() + 2;
	}
	void CopyFieldData(uint8_t*& aPtr) const {
		SISCompound::CopyFieldData(aPtr);
		*aPtr++ = iInstallType;
		*aPtr++ = iInstallFlags;
	}
	void Load(const uint8_t* aBuffer, uint32_t aLength) {
		if (aLength < 2)
			throw SISFieldReadErr;
		SISCompound::Load(aBuffer, aLength - 2);
		aBuffer += aLength - 2;
		aLength = 2;
		iInstallType = ReadUint8(aBuffer, aLength);
		iInstallFlags = ReadUint8(aBuffer, aLength);
	}

private:
	uint8_t iInstallType, iInstallFlags;
};

typedef SISArrayField<SISFieldType::SISSupportedLanguages, SISFieldType::SISLanguage> SISSupportedLanguages;
typedef SISArrayField<SISFieldType::SISSupportedOptions, SISFieldType::SISSupportedOption> SISSupportedOptions;
typedef SISArrayField<SISFieldType::SISSupportedOption, SISFieldType::SISString> SISSupportedOption;

class SISPrerequisites : public SISCompound {
public:
	SISPrerequisites() : SISCompound(SISFieldType::SISPrerequisites) {
	}
	SISPrerequisites(SISArray* aDevices, SISArray* aDependencies) : SISCompound(SISFieldType::SISPrerequisites) {
		if (aDevices->ChildType() != SISFieldType::SISDependency) {
			fprintf(stderr, "Device array given to SISPrerequisites has type %d\n", aDevices->ChildType());
			abort();
		}
		if (aDependencies->ChildType() != SISFieldType::SISDependency) {
			fprintf(stderr, "Dependency array given to SISPrerequisites has type %d\n", aDependencies->ChildType());
			abort();
		}
		AddElement(aDevices);
		AddElement(aDependencies);
	}
};

class SISDependency : public SISCompound {
public:
	SISDependency() : SISCompound(SISFieldType::SISDependency) {
	}
	SISDependency(SISUid* aUid, SISVersionRange* aVersionRange, SISArray* aNames) : SISCompound(SISFieldType::SISDependency) {
		AddElement(aUid);
		if (aVersionRange)
			AddElement(aVersionRange);
		if (aNames->ChildType() != SISFieldType::SISString) {
			fprintf(stderr, "Name array given to SISDependency has type %d\n", aNames->ChildType());
			abort();
		}
		AddElement(aNames);
	}
};

typedef SISArrayField<SISFieldType::SISProperties, SISFieldType::SISProperty> SISProperties;

class SISProperty : public SISField {
public:
	SISProperty() {
	}
	SISProperty(uint32_t aKey, uint32_t aValue) : iKey(aKey), iValue(aValue) {
	}
	uint32_t Id() const {
		return SISFieldType::SISProperty;
	}
	uint64_t Length() const {
		return 8;
	}
	void CopyFieldData(uint8_t*& aPtr) const {
		WriteUint32(aPtr, iKey);
		WriteUint32(aPtr, iValue);
	}
	void Load(const uint8_t* aBuffer, uint32_t aLength) {
		iKey = ReadUint32(aBuffer, aLength);
		iValue = ReadUint32(aBuffer, aLength);
	}

private:
	uint32_t iKey, iValue;
};

class SISCapabilities : public SISField {
public:
	SISCapabilities() {
		iUsed = 0;
		memset(iCapabilities, 0, sizeof(iCapabilities));
	}
	uint32_t Id() const {
		return SISFieldType::SISCapabilities;
	}
	uint64_t Length() const {
		return (iUsed + 3) & (~3);
	}
	void CopyFieldData(uint8_t*& aPtr) const {
		memcpy(aPtr, iCapabilities, Length());
		aPtr += Length();
	}
	void Load(const uint8_t* aBuffer, uint32_t aLength) {
		if (aLength > sizeof(iCapabilities))
			throw SISFieldReadErr;
		iUsed = aLength;
		memcpy(iCapabilities, aBuffer, iUsed);
	}
	void AddCapabilityBytes(const uint8_t* aBytes, uint8_t aNumBytes) {
		if (iUsed + aNumBytes > sizeof(iCapabilities)) {
			fprintf(stderr, "Tried to add too many bytes to a capabilities sis field\n");
			abort();
		}
		memcpy(iCapabilities + iUsed, aBytes, aNumBytes);
		iUsed += aNumBytes;
	}

private:
	uint8_t iCapabilities[100]; // should be enough...
	uint8_t iUsed;
};

enum TSISHashAlgorithm {
	ESISHashAlgSHA1 = 1
};

class SISHash : public SISCompound {
public:
	SISHash() : SISCompound(SISFieldType::SISHash) {
	}
	SISHash(uint32_t aAlgorithm, SISBlob* aBlob) : SISCompound(SISFieldType::SISHash) {
		iAlgorithm = aAlgorithm;
		AddElement(aBlob);
	}
	uint64_t Length() const {
		return SISCompound::Length() + 4;
	}
	void CopyFieldData(uint8_t*& aPtr) const {
		WriteUint32(aPtr, iAlgorithm);
		SISCompound::CopyFieldData(aPtr);
	}
	void Load(const uint8_t* aBuffer, uint32_t aLength) {
		iAlgorithm = ReadUint32(aBuffer, aLength);
		SISCompound::Load(aBuffer, aLength);
	}

private:
	uint32_t iAlgorithm;
};

enum TSISFileOperation {
	EOpInstall = 1,
	EOpRun = 2,
	EOpText = 4,
	EOpNull = 8
};

enum TSISFileOperationOption {
	EInstVerifyOnRestore = 1<<15
};

class SISFileDescription : public SISCompound {
public:
	SISFileDescription() : SISCompound(SISFieldType::SISFileDescription) {
		iOperation = iOptions = 0;
		iLength = iUncompressedLength = 0;
		iIndex = 0;
	}
	SISFileDescription(SISString* aTarget, SISString* aMime, SISCapabilities* aCapabilities, SISHash* aHash, uint32_t aOperation, uint32_t aOptions, uint64_t aLength, uint64_t aUncompressedLength, uint32_t aIndex) : SISCompound(SISFieldType::SISFileDescription) {
		AddElement(aTarget);
		AddElement(aMime);
		if (aCapabilities)
			AddElement(aCapabilities);
		AddElement(aHash);
		iOperation = aOperation;
		iOptions = aOptions;
		iLength = aLength;
		iUncompressedLength = aUncompressedLength;
		iIndex = aIndex;
	}
	uint64_t Length() const {
		return SISCompound::Length() + 28;
	}
	void CopyFieldData(uint8_t*& aPtr) const {
		SISCompound::CopyFieldData(aPtr);
		WriteUint32(aPtr, iOperation);
		WriteUint32(aPtr, iOptions);
		WriteUint64(aPtr, iLength);
		WriteUint64(aPtr, iUncompressedLength);
		WriteUint32(aPtr, iIndex);
	}
	void Load(const uint8_t* aBuffer, uint32_t aLength) {
		if (aLength < 28)
			throw SISFieldReadErr;
		SISCompound::Load(aBuffer, aLength - 28);
		aBuffer += aLength - 28;
		aLength = 28;
		iOperation = ReadUint32(aBuffer, aLength);
		iOptions = ReadUint32(aBuffer, aLength);
		iLength = ReadUint64(aBuffer, aLength);
		iUncompressedLength = ReadUint64(aBuffer, aLength);
		iIndex = ReadUint32(aBuffer, aLength);
	}

private:
	uint32_t iOperation, iOptions;
	uint64_t iLength, iUncompressedLength;
	uint32_t iIndex;
};

class SISLogo : public SISCompound {
public:
	SISLogo() : SISCompound(SISFieldType::SISLogo) {
	}
	SISLogo(SISFileDescription* aDescription) : SISCompound(SISFieldType::SISLogo) {
		AddElement(aDescription);
	}
};


class SISCertificateChain : public SISCompound {
public:
	SISCertificateChain() : SISCompound(SISFieldType::SISCertificateChain) {
	}
	SISCertificateChain(SISBlob* aBlob) : SISCompound(SISFieldType::SISCertificateChain) {
		AddElement(aBlob);
	}
};

class SISSignatureAlgorithm : public SISCompound {
public:
	SISSignatureAlgorithm() : SISCompound(SISFieldType::SISSignatureAlgorithm) {
	}
	SISSignatureAlgorithm(SISString* aAlgorithm) : SISCompound(SISFieldType::SISSignatureAlgorithm) {
		AddElement(aAlgorithm);
	}
};

class SISSignature : public SISCompound {
public:
	SISSignature() : SISCompound(SISFieldType::SISSignature) {
	}
	SISSignature(SISSignatureAlgorithm* aAlgorithm, SISBlob* aBlob) : SISCompound(SISFieldType::SISSignature) {
		AddElement(aAlgorithm);
		AddElement(aBlob);
	}
};

class SISSignatureCertificateChain : public SISCompound {
public:
	SISSignatureCertificateChain() : SISCompound(SISFieldType::SISSignatureCertificateChain) {
	}
	SISSignatureCertificateChain(SISArray* aSignatures, SISCertificateChain* aChain) : SISCompound(SISFieldType::SISSignatureCertificateChain) {
		if (aSignatures->ChildType() != SISFieldType::SISSignature) {
			fprintf(stderr, "Signature array given to SISSignatureCertificateChain has type %d\n", aSignatures->ChildType());
			abort();
		}
		AddElement(aSignatures);
		AddElement(aChain);
	}
};

enum TOperator {
	EBinOpEqual = 1,
	EBinOpNotEqual,
	EBinOpGreaterThan,
	EBinOpLessThan,
	EBinOpGreaterOrEqual,
	EBinOpLessOrEqual,
	ELogOpAnd,
	ELogOpOr,
	EUnaryOpNot,
	EFuncExists,
	EFuncAppProperties,
	EFuncDevProperties,
	EPrimTypeString,
	EPrimTypeOption,
	EPrimTypeVariable,
	EPrimTypeNumber
};
/*
enum TVariableIndex {
	EVarLanguage = 0x1001,
	EVarRemoteInstall = 0x1002
};
*/
class SISExpression : public SISCompound {
public:
	SISExpression() : SISCompound(SISFieldType::SISExpression) {
		iOperator = 0;
		iIntValue = 0;
	}
	SISExpression(uint32_t aOperator, int32_t aIntValue, SISString* aString = NULL, SISExpression* aLeft = NULL, SISExpression* aRight = NULL) : SISCompound(SISFieldType::SISExpression) {
		iOperator = aOperator;
		iIntValue = aIntValue;
		if (aString)
			AddElement(aString);
		if (aLeft)
			AddElement(aLeft);
		if (aRight)
			AddElement(aRight);
	}
	uint64_t Length() const {
		return SISCompound::Length() + 8;
	}
	void CopyFieldData(uint8_t*& aPtr) const {
		WriteUint32(aPtr, iOperator);
		WriteUint32(aPtr, iIntValue);
		SISCompound::CopyFieldData(aPtr);
	}
	void Load(const uint8_t* aBuffer, uint32_t aLength) {
		iOperator = ReadUint32(aBuffer, aLength);
		iIntValue = ReadUint32(aBuffer, aLength);
		SISCompound::Load(aBuffer, aLength);
	}

private:
	uint32_t iOperator;
	int32_t iIntValue;
};

class SISInstallBlock : public SISCompound {
public:
	SISInstallBlock() : SISCompound(SISFieldType::SISInstallBlock) {
	}
	SISInstallBlock(SISArray* aFiles, SISArray* aEmbedded, SISArray* aIfs) : SISCompound(SISFieldType::SISInstallBlock) {
		if (aFiles->ChildType() != SISFieldType::SISFileDescription) {
			fprintf(stderr, "File array given to SISInstallBlock has type %d\n", aFiles->ChildType());
			abort();
		}
		if (aEmbedded->ChildType() != SISFieldType::SISController) {
			fprintf(stderr, "Embedded array given to SISInstallBlock has type %d\n", aEmbedded->ChildType());
			abort();
		}
		if (aIfs->ChildType() != SISFieldType::SISIf) {
			fprintf(stderr, "Ifs array given to SISInstallBlock has type %d\n", aIfs->ChildType());
			abort();
		}
		AddElement(aFiles);
		AddElement(aEmbedded);
		AddElement(aIfs);
	}
};

class SISIf : public SISCompound {
public:
	SISIf() : SISCompound(SISFieldType::SISIf) {
	}
	SISIf(SISExpression* aExpression, SISInstallBlock* aInstallBlock, SISArray* aElseIfs) : SISCompound(SISFieldType::SISIf) {
		AddElement(aExpression);
		AddElement(aInstallBlock);
		if (aElseIfs->ChildType() != SISFieldType::SISElseIf) {
			fprintf(stderr, "ElseIf array given to SISIf has type %d\n", aElseIfs->ChildType());
			abort();
		}
		AddElement(aElseIfs);
	}
};

class SISElseIf : public SISCompound {
public:
	SISElseIf() : SISCompound(SISFieldType::SISElseIf) {
	}
	SISElseIf(SISExpression* aExpression, SISInstallBlock* aInstallBlock) : SISCompound(SISFieldType::SISElseIf) {
		AddElement(aExpression);
		AddElement(aInstallBlock);
	}
};


class SISController : public SISCompound {
public:
	SISController() : SISCompound(SISFieldType::SISController) {
		// manually add the wanted fields later
	}
};

typedef SISArrayField<SISFieldType::SISDataUnit, SISFieldType::SISFileData> SISDataUnit;

class SISFileData : public SISCompound {
public:
	SISFileData() : SISCompound(SISFieldType::SISFileData) {
	}
	SISFileData(SISCompressed* aData) : SISCompound(SISFieldType::SISFileData) {
		AddElement(aData);
	}
};

#endif
