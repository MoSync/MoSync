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
    versions of it that use the same license as the "OpenSSL" library);
    and distribute the linked executables. You must obey the GNU Lesser
    General Public License in all respects for all of the code used other
    than "OpenSSL". If you modify this file, you may extend this exception
    to your version of the file, but you are not obliged to do so. If you
    do not wish to do so, delete this exception statement from your version.

    Martin Storsjö
    martin@martin.st
*/

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include "sisfield.h"
#include "signutils.h"
extern "C" {
#include "crc.h"
}
#include <getopt.h>
#include <sys/stat.h>

#include "selfsigned.cer.h"
#include "selfsigned.key.h"


static SISContents* loadSISFile(const char* name, uint8_t* header) {
	FILE* in = fopen(name, "rb");
	if (!in) {
		perror(name);
		exit(1);
	}
	fseek(in, 0, SEEK_END);
	uint32_t len = ftell(in);
	fseek(in, 0, SEEK_SET);
	uint8_t* buffer = new uint8_t[len];
	if(fread(buffer, 1, len, in) != 1) {
		fclose(in);
		return NULL;
	}
	fclose(in);
	memcpy(header, buffer, 16);
	const uint8_t* ptr = buffer + 16;
	len -= 16;
	SISField* field = NULL;
	try {
		field = SISField::LoadOneField(ptr, len);
	} catch (int err) {
		fprintf(stderr, "Caught exception %d in loading embedded SIS file %s\n", err, name);
		exit(1);
	} catch (SISFieldError err) {
		fprintf(stderr, "Caught exception %d in loading embedded SIS file %s\n", err, name);
		exit(1);
	}
	delete [] buffer;

	if (field->Id() != SISFieldType::SISContents) {
		fprintf(stderr, "Main field in embedded SIS file has incorrect field type\n");
		exit(1);
	}

	return (SISContents*) field;
}

static void updateChecksum(SISControllerChecksum* csum, SISField* compressed) {
	uint8_t* compressedData = new uint8_t[compressed->HeaderDataLength()];
	uint8_t* compressedPtr = compressedData;
	compressed->CopyHeaderData(compressedPtr);
	uint16_t ccsum = crcSlow(compressedData, compressed->HeaderDataLength());
	delete [] compressedData;
	csum->iValue = ccsum;
}

static void showHelp(const char* argv0) {
	printf("%s [-?] [-cd | -cr] [-o[-p]] [-s] [-u] [-v] input [output [certificate key [passphrase]]]\n", argv0);
}

static void printTime(ASN1_TIME* time) {
	BIO* bio = BIO_new(BIO_s_file());
	BIO_set_fp(bio, stdout, BIO_NOCLOSE);
	ASN1_TIME_print(bio, time);
	BIO_free_all(bio);
}

static void displayCert(X509* cert) {
	printf("Issued by: ");
	X509_NAME_print_ex_fp(stdout, X509_get_issuer_name(cert), 0, XN_FLAG_ONELINE);
	printf("\n");
	printf("Issued to: ");
	X509_NAME_print_ex_fp(stdout, X509_get_subject_name(cert), 0, XN_FLAG_ONELINE);
	printf("\n");
	printf("Valid from ");
	printTime(X509_get_notBefore(cert));
	printf(" to ");
	printTime(X509_get_notAfter(cert));
	printf("\n");
	printf("\n");
}

int main(int argc, char *argv[]) {
	initSigning();
	const char* argv0 = argv[0];
	int ch;
	SigType type = SigAuto;
	bool verbose = false;
	bool remove = false;
	bool extract = false;
	bool display = false;
	while ((ch = getopt(argc, argv, "?c:opsuv")) != -1) {
		switch (ch) {
		case 'c':
			if (!strcmp(optarg, "d"))
				type = SigDsa;
			else if (!strcmp(optarg, "r"))
				type = SigRsa;
			else
				fprintf(stderr, "Unknown algorithm\n");
			break;
		case 'o':
			display = true;
			break;
		case 'p':
			extract = true;
			break;
		case 's':
			break;
		case 'u':
			remove = true;
			break;
		case 'v':
			verbose = true;
			break;
		case '?':
		default:
			showHelp(argv0);
			return 0;
		}
	}

	argc -= optind;
	argv += optind;
	char* input = NULL;
	char* output = NULL;
	char* cert = NULL;
	char* key = NULL;
	char* passphrase = NULL;

	if (argc > 5)
		argc = 5;
	switch (argc) {
	case 5:
		passphrase = argv[4];
	case 4:
		key = argv[3];
	case 3:
		cert = argv[2];
	case 2:
		output = argv[1];
	case 1:
		input = argv[0];
	}

	if (input == NULL) {
		showHelp(argv0);
		return 0;
	}

	if (!output)
		output = input;


	uint8_t header[16];
	SISContents* contents = loadSISFile(input, header);
	if (!contents)
		return 1;

	SISCompressed* cController = (SISCompressed*) contents->FindElement(SISFieldType::SISCompressed);
	TCompressionAlgorithm algorithm = cController->Algorithm();
	cController->Uncompress();
	cController->LoadChild();
	SISField* field = cController->Field();
	if (field->Id() != SISFieldType::SISController) {
		fprintf(stderr, "Bad SIS field type in the top level compressed field\n");
		exit(1);
	}
	SISController* controller = (SISController*) field;

	if (remove) {
		SISField* field = controller->FindRemoveLastElement(SISFieldType::SISSignatureCertificateChain);
		if (!field)
			fprintf(stderr, "Input SIS file not signed\n");
		delete field;
	} else if (display) {
		bool found = false;
		if (extract)
			mkdir("Chain", 0755);
		SISSignatureCertificateChain* signature;
		int index = 1;
		while ((signature = (SISSignatureCertificateChain*) controller->FindRemoveElement(SISFieldType::SISSignatureCertificateChain)) != NULL) {
			SISCertificateChain* chain = (SISCertificateChain*) signature->FindElement(SISFieldType::SISCertificateChain);
			SISBlob* blob = (SISBlob*) chain->FindElement(SISFieldType::SISBlob);
			int length = blob->Length();
			uint8_t* data = new uint8_t[length];
			uint8_t* ptr = data;
			blob->CopyFieldData(ptr);
			delete signature;

			char name[100];
			sprintf(name, "Chain/cert%d.pem", index);
			FILE* out = NULL;
			if (extract)
				out = fopen(name, "w");

			const uint8_t* end = data + length;
			ptr = data;
			while (true) {
#if OPENSSL_VERSION_NUMBER >= 0x000908000
				X509* cert = d2i_X509(NULL, (const unsigned char**) &ptr, end - ptr);
#else
				X509* cert = d2i_X509(NULL, &ptr, end - ptr);
#endif
				if (!cert)
					break;

				found = true;

				if (out)
					PEM_write_X509(out, cert);
				else
					displayCert(cert);

				X509_OBJECT obj;
				obj.type = X509_LU_X509;
				obj.data.x509 = cert;
				X509_OBJECT_free_contents(&obj);
			}

			if (out)
				fclose(out);
			delete [] data;
			index++;
		}

		delete contents;
		cleanupSigning();

		if (!found)
			printf("No signatures found\n");
		return 0;
	} else {
		SISDataIndex* dataIndex = (SISDataIndex*) controller->FindRemoveElement(SISFieldType::SISDataIndex);
		const char* certData = selfsignedCer;
		const char* keyData = selfsignedKey;
		bool freeCerts = false;
		if (cert && key) {
			try {
				certData = loadTextFile(cert);
				keyData = loadTextFile(key);
			} catch (SignUtilError err) {
				return err;
			}
			freeCerts = true;
		} else {
			printf("Warning, using built in self-signed certificate\n");
		}

		try {
			SISSignatureCertificateChain* chain = makeChain(controller, certData, keyData, passphrase, type);
			controller->AddElement(chain);
		} catch (SignUtilError err) {
			return err;
		}
		controller->AddElement(dataIndex);

		if (freeCerts) {
			delete [] certData;
			delete [] keyData;
		}
	}


/*
	FILE* tout = fopen("testcontroller", "wb");
	controller->CopyHeaderData(tout);
	fclose(tout);
*/
	cController->LoadUncompressed();
	cController->Compress(algorithm);

	SISControllerChecksum* csum = (SISControllerChecksum*) contents->FindElement(SISFieldType::SISControllerChecksum);
	updateChecksum(csum, cController);


	FILE* out = fopen(output, "wb");
	fwrite(header, 1, 16, out);
	contents->CopyHeaderData(out);
	fclose(out);
	delete contents;

	cleanupSigning();

	return 0;
}


