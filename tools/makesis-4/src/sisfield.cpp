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

#include "sisfield.h"

SISField* SISField::NewObject(uint32_t aType) {
	switch (aType) {
	case SISFieldType::SISString:
		return new SISString();
	case SISFieldType::SISArray:
		return new SISArray();
	case SISFieldType::SISCompressed:
		return new SISCompressed();
	case SISFieldType::SISVersion:
		return new SISVersion();
	case SISFieldType::SISVersionRange:
		return new SISVersionRange();
	case SISFieldType::SISDate:
		return new SISDate();
	case SISFieldType::SISTime:
		return new SISTime();
	case SISFieldType::SISDateTime:
		return new SISDateTime();
	case SISFieldType::SISUid:
		return new SISUid();
	case SISFieldType::SISUnused1:
		return NULL;
	case SISFieldType::SISLanguage:
		return new SISLanguage();
	case SISFieldType::SISContents:
		return new SISContents();
	case SISFieldType::SISController:
		return new SISController();
	case SISFieldType::SISInfo:
		return new SISInfo();
	case SISFieldType::SISSupportedLanguages:
		return new SISSupportedLanguages();
	case SISFieldType::SISSupportedOptions:
		return new SISSupportedOptions();
	case SISFieldType::SISPrerequisites:
		return new SISPrerequisites();
	case SISFieldType::SISDependency:
		return new SISDependency();
	case SISFieldType::SISProperties:
		return new SISProperties();
	case SISFieldType::SISProperty:
		return new SISProperty();
	case SISFieldType::SISSignatures:
		return NULL;
	case SISFieldType::SISCertificateChain:
		return new SISCertificateChain();
	case SISFieldType::SISLogo:
		return new SISLogo();
	case SISFieldType::SISFileDescription:
		return new SISFileDescription();
	case SISFieldType::SISHash:
		return new SISHash();
	case SISFieldType::SISIf:
		return new SISIf();
	case SISFieldType::SISElseIf:
		return new SISElseIf();
	case SISFieldType::SISInstallBlock:
		return new SISInstallBlock();
	case SISFieldType::SISExpression:
		return new SISExpression();
	case SISFieldType::SISData:
		return new SISData();
	case SISFieldType::SISDataUnit:
		return new SISDataUnit();
	case SISFieldType::SISFileData:
		return new SISFileData();
	case SISFieldType::SISSupportedOption:
		return new SISSupportedOption();
	case SISFieldType::SISControllerChecksum:
		return new SISControllerChecksum();
	case SISFieldType::SISDataChecksum:
		return new SISDataChecksum();
	case SISFieldType::SISSignature:
		return new SISSignature();
	case SISFieldType::SISBlob:
		return new SISBlob();
	case SISFieldType::SISSignatureAlgorithm:
		return new SISSignatureAlgorithm();
	case SISFieldType::SISSignatureCertificateChain:
		return new SISSignatureCertificateChain();
	case SISFieldType::SISDataIndex:
		return new SISDataIndex();
	case SISFieldType::SISCapabilities:
		return new SISCapabilities();
	default:
		return NULL;
	}
}

