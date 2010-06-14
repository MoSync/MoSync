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

#include "sisfilegen.h"

#include <stdio.h>
#include "crtsis.h"
#include <stdlib.h>

#include "signutils.h"

#include "sisfield.h"
#include <openssl/sha.h>
#include <stack>
extern "C" {
#include "crc.h"
}

using std::stack;

static uint32_t uidCrc(uint32_t uid1, uint32_t uid2, uint32_t uid3) {
	uint8_t buf1[] = { uint8_t(uid1 >> 8), uint8_t(uid1 >> 24), uint8_t(uid2 >> 8), uint8_t(uid2 >> 24), uint8_t(uid3 >> 8), uint8_t(uid3 >> 24) };
	uint8_t buf2[] = { uint8_t(uid1 >> 0), uint8_t(uid1 >> 16), uint8_t(uid2 >> 0), uint8_t(uid2 >> 16), uint8_t(uid3 >> 0), uint8_t(uid3 >> 16) };
	uint16_t crc1 = crcFast(buf1, 6);
	uint16_t crc2 = crcFast(buf2, 6);
	return (uint32_t(crc1)<<16) | crc2;
}


CSISFileGenerator::CSISFileGenerator(const CSISWriter *sisw) : siswriter(sisw) {
}

CSISFileGenerator::~CSISFileGenerator() {
}

static SISArray* GetStringArray(const LANGSTRINGNODE* base) {
	SISArray* array = new SISArray(SISFieldType::SISString);
	stack<SISString*> tmpStack;
	while (base) {
		tmpStack.push(new SISString(base->pszString));
		base = base->pNext;
	}
	while (!tmpStack.empty()) {
		array->AddElement(tmpStack.top());
		tmpStack.pop();
	}
	return array;
}

static SISArray* GetDependencyArray(const DEPENDNODE* dependency) {
	SISArray* array = new SISArray(SISFieldType::SISDependency);
	stack<SISDependency*> tmpStack;
	while (dependency) {
		SISArray* depNameArray = GetStringArray(dependency->pLangStringBase);
		SISUid* depUid = new SISUid(dependency->dwUID);
		SISVersion* depFromVer = new SISVersion(dependency->fromMajor, dependency->fromMinor, dependency->fromBuild);
		SISVersion* depToVer = NULL;
		if (dependency->hasTo)
			depToVer = new SISVersion(dependency->toMajor, dependency->toMinor, dependency->toBuild);
		SISVersionRange* depRange = new SISVersionRange(depFromVer, depToVer);
		SISDependency* dep = new SISDependency(depUid, depRange, depNameArray);
		tmpStack.push(dep);

		dependency = dependency->pNext;
	}
	while (!tmpStack.empty()) {
		array->AddElement(tmpStack.top());
		tmpStack.pop();
	}
	return array;
}

static SISArray* GetPropertyArray(const CAPABILITYNODE* capability) {
	SISArray* array = new SISArray(SISFieldType::SISProperty);
	stack<SISProperty*> tmpStack;
	while (capability) {
		SISProperty* prop = new SISProperty(capability->iKey, capability->iValue);
		tmpStack.push(prop);

		capability = capability->pNext;
	}
	while (!tmpStack.empty()) {
		array->AddElement(tmpStack.top());
		tmpStack.pop();
	}
	return array;
}

static void Recompress(SISDataUnit* dataUnit, TCompressionAlgorithm algorithm) {
	if (algorithm != ECompressNone)
		return;
	SISArray* fileDataArray = (SISArray*) dataUnit->FindElement(SISFieldType::SISArray);
	for (uint32_t i = 0; i < fileDataArray->Count(); i++) {
		SISFileData* fileData = (SISFileData*) fileDataArray->ElementAt(i);
		SISCompressed* compressed = (SISCompressed*) fileData->FindElement(SISFieldType::SISCompressed);
		compressed->Compress(algorithm);
	}
}

SISInfo* CSISFileGenerator::CreateSISInfo(bool stub) {
	SISUid* uid = new SISUid(siswriter->GetUID());
	SISString* vendorName = NULL;
	const LANGSTRINGNODE* vendorNameNode = siswriter->GetVendorName();
	if (!vendorNameNode)
		throw ErrNoVendorName;
	vendorName = new SISString(vendorNameNode->pszString);

	SISArray* pkgNames = GetStringArray(siswriter->GetLangStringBase());
	if (!siswriter->GetLocalizedVendorBase())
		throw ErrNoLocalizedVendorName;
	SISArray* locVenNames = GetStringArray(siswriter->GetLocalizedVendorBase());

	WORD major, minor;
	DWORD build;
	siswriter->QueryVersionInfo(&major, &minor, &build);
	SISVersion* version = new SISVersion(major, minor, build);

	DWORD year, month, day, hours, minutes, seconds;
	SISDate* date = NULL;
	SISTime* time = NULL;
	if (stub) {
		date = new SISDate(0x7d4, 0x00, 0x01);
		time = new SISTime(0, 0, 0);
	} else if (siswriter->GetTimestamp(&year, &month, &day, &hours, &minutes, &seconds)) {
		date = new SISDate(year, month, day);
		time = new SISTime(hours, minutes, seconds);
	} else {
		date = new SISDate();
		time = new SISTime();
	}

	uint32_t flags = siswriter->GetFlags();
	flags &= ~(EInstFlagNoCompress | EInstFlagIsUnicode);

	SISInfo* info = new SISInfo(uid, vendorName, pkgNames, locVenNames, version, new SISDateTime(date, time), siswriter->GetType(), flags);
	return info;
}

SISCapabilities* CSISFileGenerator::GetCapabilities(const char* filename) {
	SISCapabilities* cap = GetE32Capabilities(filename);
	if (cap)
		return cap;
	return GetPECapabilities(filename);
}

SISCapabilities* CSISFileGenerator::GetE32Capabilities(const char* filename) {
	FILE* in = fopen(filename, "rb");
	if (!in) {
		perror(filename);
		return NULL;
	}
	fseek(in, 16, SEEK_SET);
	char buf[4];
	memset(buf, 0, sizeof(buf));
	fread(buf, 1, 4, in);
	if (memcmp(buf, "EPOC", 4)) {
		fclose(in);
		return NULL;
	}
	fseek(in, 0x88, SEEK_SET);
	uint8_t caps[4];
	if (fread(caps, 1, 4, in) != 4) {
		fprintf(stderr, "Unable to read caps from %s\n", filename);
		fclose(in);
		return NULL;
	}
	fclose(in);
	uint8_t zeroCaps[4];
	memset(zeroCaps, 0, sizeof(zeroCaps));
	if (!memcmp(caps, zeroCaps, 4))
		return NULL;
	SISCapabilities* capabilities = new SISCapabilities();
	capabilities->AddCapabilityBytes(caps, 4);
	return capabilities;
}

SISCapabilities* CSISFileGenerator::GetPECapabilities(const char* filename) {
	FILE* in = fopen(filename, "rb");
	if (!in) {
		perror(filename);
		return NULL;
	}
	uint8_t buf[4];
	memset(buf, 0, sizeof(buf));
	fread(buf, 1, 2, in);
	if (memcmp(buf, "MZ", 2)) {
		fclose(in);
		return NULL;
	}
	fseek(in, 0x3c, SEEK_SET);
	uint8_t coffOffset;
	fread(&coffOffset, 1, 1, in);
	fseek(in, coffOffset, SEEK_SET);
	fread(buf, 1, 4, in);
	if (memcmp(buf, "PE\0\0", 4)) {
		fclose(in);
		return NULL;
	}
	fseek(in, 2, SEEK_CUR);
	fread(buf, 1, 2, in);
	uint16_t numberOfSections = buf[0] | (buf[1] << 8);
	bool found = false;
	uint32_t sectionOffset = 0;
	for (uint16_t i = 0; i < numberOfSections; i++) {
		fseek(in, coffOffset + 248 + 40*i, SEEK_SET);
		uint8_t nameBuf[8];
		fread(nameBuf, 1, 8, in);
		if (!memcmp(nameBuf, ".SYMBIAN", 8)) {
			fseek(in, 12, SEEK_CUR);
			fread(buf, 1, 4, in);
			found = true;
			sectionOffset = buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
			break;
		}
	}
	if (!found) {
		fclose(in);
		return NULL;
	}
	fseek(in, sectionOffset + 24, SEEK_SET);
	uint8_t caps[4];
	if (fread(caps, 1, 4, in) != 4) {
		fprintf(stderr, "Unable to read caps from %s\n", filename);
		fclose(in);
		return NULL;
	}
	fclose(in);
	uint8_t zeroCaps[4];
	memset(zeroCaps, 0, sizeof(zeroCaps));
	if (!memcmp(caps, zeroCaps, 4))
		return NULL;
	SISCapabilities* capabilities = new SISCapabilities();
	capabilities->AddCapabilityBytes(caps, 4);
	return capabilities;
}
static SISHash* GetHash(RawFile* rawFile) {
	SHA_CTX sha;
	SHA1_Init(&sha);
	if (rawFile) {
		uint8_t* fileContent = new uint8_t[rawFile->Length()];
		uint8_t* fileContentPtr = fileContent;
		try {
			rawFile->CopyFieldData(fileContentPtr);
		} catch (SISFieldError) {
			throw ErrCantRead;
		}
		SHA1_Update(&sha, fileContent, rawFile->Length());
		delete [] fileContent;
	}
	uint8_t hashData[SHA_DIGEST_LENGTH];
	SHA1_Final(hashData, &sha);

	SISBlob* hashBlob = NULL;
	if (rawFile)
		hashBlob = new SISBlob(hashData, sizeof(hashData));
	else
		hashBlob = new SISBlob(hashData, 0);
	return new SISHash(ESISHashAlgSHA1, hashBlob);
}

void CSISFileGenerator::LoadFile(const wchar_t* filename, RawFile** rawFile, uint64_t* compressedSize, uint64_t* uncompressedSize, int* index, SISFileData** fileData) {
	char sourceFile[MAX_PATH];
	wcstombs(sourceFile, filename, sizeof(sourceFile));
	sourceFile[MAX_PATH-1] = '\0';

	*rawFile = NULL;
	*compressedSize = 0;
	*uncompressedSize = 0;
	if (strlen(sourceFile) > 0) {
		try {
			*rawFile = new RawFile(sourceFile);
			TCompressionAlgorithm algorithm = ECompressAuto;
			if (siswriter->GetFlags() & EInstFlagNoCompress)
				algorithm = ECompressNone;
			SISCompressed* compressedFile = new SISCompressed(*rawFile, algorithm);
			*fileData = new SISFileData(compressedFile);
			*compressedSize = compressedFile->CompressedSize();
			*uncompressedSize = (*rawFile)->Length();
		} catch (SISFieldError) {
			throw ErrCantRead;
		}
	} else {
		*fileData = NULL;
		*index = 0;
	}
}

void CSISFileGenerator::GetFileDescription(const PKGLINEFILE* file, int index, SISFileDescription** fileDesc, SISFileData** fileData, bool langDep) {
	RawFile* rawFile;
	uint64_t compressedSize, uncompressedSize;
	LoadFile(file->pszSource, &rawFile, &compressedSize, &uncompressedSize, &index, fileData);

	SISString* target = new SISString(file->pszDest);
	SISString* mimeType = new SISString(file->pszMimeType);
	SISHash* hash = GetHash(rawFile);

	SISCapabilities* capabilities = NULL;
	if (!langDep && rawFile) {
		char sourceFile[MAX_PATH];
		wcstombs(sourceFile, file->pszSource, sizeof(sourceFile));
		sourceFile[MAX_PATH-1] = '\0';
		capabilities = GetCapabilities(sourceFile);
	}

	uint32_t operation = EOpInstall;
	uint32_t options = 0;
	if (file->type == EInstFileTypeSimple) {
		if (langDep)
			operation = 0;
		else
			operation = EOpInstall;
	} else if (file->type == EInstFileTypeNull) {
		operation = EOpNull;
	} else if (file->type == EInstFileTypeText) {
		operation = EOpText;
		options = file->options.iTextOption;
	} else if (file->type == EInstFileTypeRun) {
		operation = EOpRun;
		options = file->options.iRunOption;
	}
	if (mimeType->Length() > 0) {
		operation = EOpRun;
		options = file->options.iMimeOption;
	}

	*fileDesc = new SISFileDescription(target, mimeType, capabilities, hash, operation, options, compressedSize, uncompressedSize, index);
}

bool CSISFileGenerator::GetLogo(const LOGO* logo, int index, SISFileDescription** fileDesc, SISFileData** fileData) {
	if (!logo)
		return false;
	RawFile* rawFile;
	uint64_t compressedSize, uncompressedSize;
	LoadFile(logo->pszSource, &rawFile, &compressedSize, &uncompressedSize, &index, fileData);

	SISString* target = new SISString(logo->pszDest);
	SISString* mimeType = new SISString(logo->pszMimeType);
	SISHash* hash = GetHash(rawFile);

	uint32_t operation = EOpRun;
	uint32_t options = EInstFileRunOptionByMimeType | EInstFileRunOptionInstall;
	if (target->Length() > 0)
		operation |= EOpInstall;

	*fileDesc = new SISFileDescription(target, mimeType, NULL, hash, operation, options, compressedSize, uncompressedSize, index);
	return true;
}

static SISExpression* GetExpression(const struct PKGLINECONDITION* condition) {
	switch (condition->exprType) {
	case EFuncExists: {
		const struct PKGLINECONDITION* prim = condition->b.pLhs;
		const wchar_t* str = prim->pPrim->pszString;
		return new SISExpression(condition->exprType, 0, new SISString(str));
	}
	case EBinOpEqual:
	case EBinOpNotEqual:
	case EBinOpGreaterThan:
	case EBinOpLessThan:
	case EBinOpGreaterOrEqual:
	case EBinOpLessOrEqual:
	case ELogOpAnd:
	case ELogOpOr:
	case EFuncAppProperties: {
		SISExpression* left = GetExpression(condition->b.pLhs);
		SISExpression* right = GetExpression(condition->b.pRhs);
		return new SISExpression(condition->exprType, 0, NULL, left, right);
	}
	case EUnaryOpNot:
	case EFuncDevProperties:
		return new SISExpression(condition->exprType, 0, NULL, GetExpression(condition->pExpr));
	case EPrimTypeString:
		return new SISExpression(condition->exprType, 0, new SISString(condition->pPrim->pszString));
	case EPrimTypeVariable:
	case EPrimTypeNumber:
	case EPrimTypeOption:
		return new SISExpression(condition->exprType, condition->pPrim->dwNumber);
	default:
		break;
	}

	return NULL;
}

static SISContents* LoadSISFile(const wchar_t* name, uint32_t uid) {
	char sourceFile[MAX_PATH];
	wcstombs(sourceFile, name, sizeof(sourceFile));
	sourceFile[MAX_PATH-1] = '\0';

        FILE* in = fopen(sourceFile, "rb");
        if (!in) {
                perror(sourceFile);
		throw ErrSISFileNotFound;
        }
        fseek(in, 0, SEEK_END);
        uint32_t len = ftell(in);
        fseek(in, 0, SEEK_SET);
        uint8_t* buffer = new uint8_t[len];
        fread(buffer, 1, len, in);
        fclose(in);
	const uint8_t* uidPtr = buffer + 8;
	uint32_t fileUid = uidPtr[0] | (uidPtr[1] << 8) | (uidPtr[2] << 16) | (uidPtr[3] << 24);
	if (fileUid != uid) {
		fprintf(stderr, "UID in embedded SIS file %s doesn't match %x\n", sourceFile, uid);
		throw ErrUIDMismatch;
	}

        const uint8_t* ptr = buffer + 16;
        len -= 16;
	SISField* field = NULL;
	try {
		field = SISField::LoadOneField(ptr, len);
	} catch (SISFieldError err) {
		fprintf(stderr, "Caught exception %d in loading embedded SIS file %s\n", err, sourceFile);
		throw ErrEmbeddedSIS;
	}
	delete [] buffer;

	if (field->Id() != SISFieldType::SISContents) {
		fprintf(stderr, "Main field in embedded SIS file has incorrect field type\n");
		throw ErrEmbeddedSIS;
	}

	return (SISContents*) field;
}

void CSISFileGenerator::LoadEmbeddedSis(const PKGLINEFILE* file, SISArray* embeddedArray, SISArray* dataUnitArray) {
	SISContents* contents = LoadSISFile(file->pszSource, file->options.iComponentUid);
	SISCompressed* cController = (SISCompressed*) contents->FindRemoveElement(SISFieldType::SISCompressed);
	cController->Uncompress();
	cController->LoadChild();
	SISField* field = cController->Field();
	if (field->Id() != SISFieldType::SISController) {
		fprintf(stderr, "Bad SIS field type in the top level compressed field\n");
		throw ErrEmbeddedSIS;
	}
	SISController* embedController = (SISController*) field;
	cController->DetachField();
	delete cController;
	SISData* data = (SISData*) contents->FindRemoveElement(SISFieldType::SISData);
	delete contents;

	SISDataIndex* index = (SISDataIndex*) embedController->FindElement(SISFieldType::SISDataIndex);
	index->iValue = dataUnitArray->Count();

	embeddedArray->AddElement(embedController);


	SISArray* embedDataUnitArray = (SISArray*) data->FindRemoveElement(SISFieldType::SISArray);
	delete data;
	SISDataUnit* embedDataUnit;
	TCompressionAlgorithm algorithm = ECompressAuto;
	if (siswriter->GetFlags() & EInstFlagNoCompress)
		algorithm = ECompressNone;
	while ((embedDataUnit = (SISDataUnit*) embedDataUnitArray->FindRemoveElement(SISFieldType::SISDataUnit))) {
		Recompress(embedDataUnit, algorithm);
		dataUnitArray->AddElement(embedDataUnit);
	}
	delete embedDataUnitArray;
}

void CSISFileGenerator::GenerateSISFile(const wchar_t* target, bool stub) {
	char targetFile[MAX_PATH];
	wcstombs(targetFile, target, sizeof(targetFile));
	targetFile[MAX_PATH-1] = '\0';

	FILE* out = fopen(targetFile, "wb");
	if (!out) {
		perror(targetFile);
		throw ErrCantWrite;
	}

	crcInit();

	if (!stub)
		WriteHeaderL(0x10201A7A, 0x0, siswriter->GetUID(), out);

	SISInfo* info = CreateSISInfo(stub);

	SISController* controller = new SISController();

	controller->AddElement(info);

	SISArray* optionsArray = new SISArray(SISFieldType::SISSupportedOption);
	SISSupportedOptions* supportedOptions = new SISSupportedOptions(optionsArray);
	controller->AddElement(supportedOptions);

	SISArray* languagesArray = new SISArray(SISFieldType::SISLanguage);
	const LANGNODE* language = siswriter->GetLanguageBase();
	stack<SISLanguage*> languageTmp;
	while (language) {
//		languagesArray->AddElement(new SISLanguage(language->wLang));
		languageTmp.push(new SISLanguage(language->wLang));
		language = language->pNext;
	}
	uint32_t *langCodes = new uint32_t[languageTmp.size()];
	for (int i = 0; !languageTmp.empty(); i++) {
		langCodes[i] = ((SISLanguage*)languageTmp.top())->iValue;
		languagesArray->AddElement(languageTmp.top());
		languageTmp.pop();
	}

	SISSupportedLanguages* supportedLanguages = new SISSupportedLanguages(languagesArray);
	controller->AddElement(supportedLanguages);


	SISArray* deviceDep = GetDependencyArray(siswriter->GetPlatformDependencyBase());
	SISArray* packageDep = GetDependencyArray(siswriter->GetDependencyBase());

	SISPrerequisites* prerequisites = new SISPrerequisites(deviceDep, packageDep);
	controller->AddElement(prerequisites);

	SISArray* propertyArray = GetPropertyArray(siswriter->GetCapabilityBase());
	SISProperties* properties = new SISProperties(propertyArray);
	controller->AddElement(properties);

	uint32_t index = 0;
	SISArray* fileDataArray = new SISArray(SISFieldType::SISFileData);

	const LOGO* logo = siswriter->GetLogoBase();
	stack<const LOGO*> logos;
	while (logo) {
		logos.push(logo);
		logo = logo->pNext;
	}
	SISLogo* logoField = NULL;
	while (!logos.empty()) {
		logo = logos.top();
		logos.pop();
		SISFileData* fileData;
		SISFileDescription* fileDesc;
		GetLogo(logo, index, &fileDesc, &fileData);
		index++;
		fileDataArray->AddElement(fileData);
		delete logoField;
		logoField = new SISLogo(fileDesc);
	}
	if (logoField)
		controller->AddElement(logoField);

	SISArray* fileDescArray = new SISArray(SISFieldType::SISFileDescription);
	SISArray* ifArray = new SISArray(SISFieldType::SISIf);
	SISArray* elseIfArray = NULL;
	SISArray* embeddedArray = new SISArray(SISFieldType::SISController);

	SISDataUnit* dataUnit = new SISDataUnit(fileDataArray);
	SISArray* dataUnitArray = new SISArray(SISFieldType::SISDataUnit);
	dataUnitArray->AddElement(dataUnit);

	const PKGLINENODE* pkgline = siswriter->GetPkgLineBase();

	stack<const PKGLINENODE*> pkglines;
	while (pkgline) {
		pkglines.push(pkgline);
		pkgline = pkgline->pNext;
	}
	stack<SISArray*> fileDescArrays;
	stack<SISArray*> embeddedArrays;
	stack<SISArray*> ifArrays;
	stack<SISArray*> elseIfArrays;
	while (!pkglines.empty()) {
		pkgline = pkglines.top();
		pkglines.pop();

		if (pkgline->iPackageLineType == EInstPkgLineFile) {
			const PKGLINEFILE* file = pkgline->file;
			if (file->type == EInstFileTypeComponent) {
				LoadEmbeddedSis(file, embeddedArray, dataUnitArray);
			} else {
				SISFileData* fileData;
				SISFileDescription* fileDesc;
				GetFileDescription(file, index, &fileDesc, &fileData, false);
				if (file->type != EInstFileTypeNull) {
					index++;
					if (stub)
						delete fileData;
					else
						fileDataArray->AddElement(fileData);
				}
				fileDescArray->AddElement(fileDesc);
			}
		} else if (pkgline->iPackageLineType == EInstPkgLineLanguageFile) {
			SISArray* elseIfArray = new SISArray(SISFieldType::SISElseIf);
			SISIf* ifField = NULL;
			for (uint32_t i = 0; i < languagesArray->Count(); i++) {
				SISExpression* langExp = new SISExpression(EPrimTypeVariable, EVarLanguage);
				SISExpression* langVal = new SISExpression(EPrimTypeNumber, langCodes[i]);
				SISExpression* langCmp = new SISExpression(EBinOpEqual, 0, NULL, langExp, langVal);
				SISArray* localFileDescArray = new SISArray(SISFieldType::SISFileDescription);
				SISArray* embeddedArray = new SISArray(SISFieldType::SISController);

				const PKGLINEFILE* file = pkgline->file;
				if (file->type == EInstFileTypeComponent) {
					LoadEmbeddedSis(file, embeddedArray, dataUnitArray);
				} else {
					SISFileData* fileData;
					SISFileDescription* fileDesc;
					GetFileDescription(file, index, &fileDesc, &fileData, true);
					index++;
					if (stub)
						delete fileData;
					else
						fileDataArray->AddElement(fileData);
					localFileDescArray->AddElement(fileDesc);
				}

				SISInstallBlock* installBlock = new SISInstallBlock(localFileDescArray, embeddedArray, new SISArray(SISFieldType::SISIf));

				if (i == 0) {
					ifField = new SISIf(langCmp, installBlock, elseIfArray);
				} else {
					SISElseIf* elseIf = new SISElseIf(langCmp, installBlock);
					elseIfArray->AddElement(elseIf);
				}

				if (i + 1 < languagesArray->Count()) {
					pkgline = pkglines.top();
					pkglines.pop();
				}
			}
			ifArray->AddElement(ifField);
		} else if (pkgline->iPackageLineType == EInstPkgLineCondIf || pkgline->iPackageLineType == EInstPkgLineCondElseIf || pkgline->iPackageLineType == EInstPkgLineCondElse) {
			if (pkgline->iPackageLineType != EInstPkgLineCondIf) {
				fileDescArray = fileDescArrays.top();
				fileDescArrays.pop();
				ifArray = ifArrays.top();
				ifArrays.pop();
				embeddedArray = embeddedArrays.top();
				embeddedArrays.pop();
			}

			fileDescArrays.push(fileDescArray);
			ifArrays.push(ifArray);
			embeddedArrays.push(embeddedArray);

			SISArray* curIfArray = ifArray;
			ifArray = new SISArray(SISFieldType::SISIf);
			embeddedArray = new SISArray(SISFieldType::SISController);
			fileDescArray = new SISArray(SISFieldType::SISFileDescription);
			SISInstallBlock* installBlock = new SISInstallBlock(fileDescArray, embeddedArray, ifArray);
			SISExpression* exp = NULL;
			if (pkgline->iPackageLineType == EInstPkgLineCondElse)
				exp = new SISExpression(EUnaryOpNot, 0, NULL, new SISExpression(EPrimTypeNumber, 0, NULL));
			else
				exp = GetExpression(pkgline->cond);

			if (pkgline->iPackageLineType == EInstPkgLineCondIf) {
				elseIfArrays.push(elseIfArray);
				elseIfArray = new SISArray(SISFieldType::SISElseIf);
				curIfArray->AddElement(new SISIf(exp, installBlock, elseIfArray));
			} else {
				elseIfArray->AddElement(new SISElseIf(exp, installBlock));
			}

		} else if (pkgline->iPackageLineType == EInstPkgLineCondEndIf) {
			fileDescArray = fileDescArrays.top();
			fileDescArrays.pop();
			ifArray = ifArrays.top();
			ifArrays.pop();
			elseIfArray = elseIfArrays.top();
			elseIfArrays.pop();
			embeddedArray = embeddedArrays.top();
			embeddedArrays.pop();
		} else if (pkgline->iPackageLineType == EInstPkgLineOption) {
			const LANGSTRINGNODE* base = pkgline->option->pLangStringBase;
			stack<SISSupportedOption*> optionStack;
			for (uint32_t i = 0; i < pkgline->option->dwNumOptions; i++) {
				SISArray* array = new SISArray(SISFieldType::SISString);
				stack<SISString*> tmpStack;
				for (uint32_t i = 0; i < languagesArray->Count(); i++) {
					tmpStack.push(new SISString(base->pszString));
					base = base->pNext;
				}
				while (!tmpStack.empty()) {
					array->AddElement(tmpStack.top());
					tmpStack.pop();
				}
				SISSupportedOption* option = new SISSupportedOption(array);
//				optionsArray->AddElement(option);
				optionStack.push(option);
			}
			while (!optionStack.empty()) {
				optionsArray->AddElement(optionStack.top());
				optionStack.pop();
			}
		}


	}



	SISInstallBlock* installBlock = new SISInstallBlock(fileDescArray, embeddedArray, ifArray);
	controller->AddElement(installBlock);

	SISDataIndex* dataIndex = new SISDataIndex(0);
	controller->AddElement(dataIndex);

	if (stub) {
		controller->CopyHeaderData(out);
		delete controller;
		delete [] langCodes;
		delete dataUnitArray;
		fclose(out);
		return;
	}

	SISData* data = new SISData(dataUnitArray);
/*
	FILE* cOut = fopen("testcontroller", "wb");
	controller->CopyHeaderData(cOut);
	fclose(cOut);
*/
	TCompressionAlgorithm algorithm = ECompressAuto;
	if (siswriter->GetFlags() & EInstFlagNoCompress)
		algorithm = ECompressNone;
	SISCompressed* compressedController = new SISCompressed(controller, algorithm);

	uint8_t* dataContent = new uint8_t[data->HeaderDataLength()];
	uint8_t* dataPtr = dataContent;
	data->CopyHeaderData(dataPtr);
	uint16_t dcsum = crcFast(dataContent, data->HeaderDataLength());
	delete [] dataContent;

	uint8_t* compressedControllerData = new uint8_t[compressedController->HeaderDataLength()];
	uint8_t* compressedControllerPtr = compressedControllerData;
	compressedController->CopyHeaderData(compressedControllerPtr);
	uint16_t ccsum = crcFast(compressedControllerData, compressedController->HeaderDataLength());
	delete [] compressedControllerData;

	SISControllerChecksum* cchecksum = new SISControllerChecksum(ccsum);
	SISDataChecksum* dchecksum = new SISDataChecksum(dcsum);
	SISContents* contents = new SISContents(cchecksum, dchecksum, compressedController, data);

	contents->CopyHeaderData(out);

	delete contents;

	delete [] langCodes;

	fclose(out);
}

static void writeUint32(uint32_t value, FILE* out) {
	uint8_t buf[4];
	buf[0] = (value >>  0) & 0xff;
	buf[1] = (value >>  8) & 0xff;
	buf[2] = (value >> 16) & 0xff;
	buf[3] = (value >> 24) & 0xff;
	fwrite(buf, 1, 4, out);
}

void CSISFileGenerator::WriteHeaderL(uint32_t uid1, uint32_t uid2, uint32_t uid3, FILE* out) {
	uint32_t checksum = uidCrc(uid1, uid2, uid3);
	writeUint32(uid1, out);
	writeUint32(uid2, out);
	writeUint32(uid3, out);
	writeUint32(checksum, out);
}

