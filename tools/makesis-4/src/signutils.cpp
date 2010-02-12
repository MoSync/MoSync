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

#include "signutils.h"
#include <openssl/evp.h>
#include <openssl/pem.h>
#ifndef WIN32
#include <pwd.h>
#endif
//#include <unistd.h>
#include <string.h>
//#include <stdint.h>
#include <openssl/err.h>
//#include <unistd.h>
#include "sisfield.h"

#ifdef WIN32
char passwordBuffer[200];
#include <conio.h>
static char* getpass(const char* prompt) {
	fprintf(stderr, "%s", prompt);
	int index = 0;
	while (true) {
		char c = _getch();
		if (c == '\r' || c == '\n') {
			break;
		} else if (c == '\b') {
			if (index > 0)
				index--;
		} else {
			passwordBuffer[index++] = c;
		}
	}
	fprintf(stderr, "\n");
	passwordBuffer[index] = '\0';
	return passwordBuffer;
}
#endif

static int password_cb(char *buf, int size, int rwflag, void *userdata) {
	char* pwd = (char*) userdata;
	if (!pwd)
		pwd = getpass("Enter private key password: ");
	int len = strlen(pwd);
	if (len > size)
		len = size;
	memcpy(buf, pwd, len);
	return len;
}

char* loadTextFile(const char* name) {
	FILE* in = fopen(name, "rb");
	if (!in) {
		perror(name);
		throw SignFileNotFound;
	}
	fseek(in, 0, SEEK_END);
	uint32_t len = ftell(in);
	fseek(in, 0, SEEK_SET);
	char* buffer = new char[len + 1];
	fread(buffer, 1, len, in);
	fclose(in);
	buffer[len] = '\0';
	return buffer;
}

static SISSignature* makeSignature(SISField* controller, const char* keyData, const char* passphrase, SigType type) {
	if (type == SigAuto) {
		if (strstr(keyData, " DSA "))
			type = SigDsa;
		else
			type = SigRsa;
	}
	BIO* io = BIO_new_mem_buf((void*) keyData, -1);
	EVP_PKEY* key = PEM_read_bio_PrivateKey(io, NULL, password_cb, (void*) passphrase);
	if (!key) {
		ERR_print_errors_fp(stderr);
		fprintf(stderr, "Unable to load key\n");
		throw SignBadKey;
	}
	BIO_free_all(io);
	uint8_t* buffer = new uint8_t[controller->Length()];
	uint8_t* ptr = buffer;
	controller->CopyFieldData(ptr);

	EVP_MD_CTX ctx;
	EVP_MD_CTX_init(&ctx);
	const EVP_MD* md = NULL;
	if (type == SigDsa)
		md = EVP_dss1();
	else
		md = EVP_sha1();
	if (EVP_SignInit_ex(&ctx, md, NULL) == 0) {
		ERR_print_errors_fp(stderr);
		throw SignOpenSSLErr;
	}
	if (EVP_SignUpdate(&ctx, buffer, controller->Length()) == 0) {
		ERR_print_errors_fp(stderr);
		throw SignOpenSSLErr;
	}
	delete [] buffer;
	unsigned int siglen = EVP_PKEY_size(key);
	uint8_t* signature = new uint8_t[siglen];
	if (EVP_SignFinal(&ctx, signature, &siglen, key) == 0) {
		ERR_print_errors_fp(stderr);
		throw SignOpenSSLErr;
	}
	EVP_MD_CTX_cleanup(&ctx);
	EVP_PKEY_free(key);

	SISString* algorithm = NULL;
	if (type == SigDsa)
		algorithm = new SISString(L"1.2.840.10040.4.3");
	else
		algorithm = new SISString(L"1.2.840.113549.1.1.5");
	SISSignatureAlgorithm* signatureAlgorithm = new SISSignatureAlgorithm(algorithm);
	SISBlob* blob = new SISBlob(signature, siglen);
	delete [] signature;

	return new SISSignature(signatureAlgorithm, blob);
}

/*
uint8_t* decodeBase64(const char* str, uint32_t* length) {
	BIO* b64 = BIO_new(BIO_f_base64());
	BIO* bio = BIO_new_mem_buf((void*) str, -1);
	bio = BIO_push(b64, bio);
	uint32_t bufferSize = 4096;
	uint8_t* buffer = new uint8_t[bufferSize];
	uint32_t bufferLength = 0;
	int n;
	while ((n = BIO_read(bio, buffer + bufferLength, bufferSize - bufferLength)) > 0) {
		bufferLength += n;
		if (bufferLength == bufferSize) {
			bufferSize *= 2;
			uint8_t* newBuf = new uint8_t[bufferSize];
			memcpy(newBuf, buffer, bufferLength);
			delete [] buffer;
			buffer = newBuf;
		}
	}
	BIO_free_all(bio);
	if (length)
		*length = bufferLength;
	return buffer;
}

static const char* certStartTag = "-----BEGIN CERTIFICATE----";
static const char* certEndTag = "-----END CERTIFICATE-----";

SISCertificateChain* makeChain(const char* certData) {
	uint8_t* data = NULL;
	uint32_t dataLength = 0;
	while (true) {
		const char* start = strstr(certData, certStartTag);
		if (!start)
			break;
		start += strlen(certStartTag);
		const char* end = strstr(start, certEndTag);
		if (!end) {
			fprintf(stderr, "Bad certificate file\n");
			throw SignBadCert;
		}
		certData = end + strlen(certStartTag);

		int len = end - start;
		char* certBuf = new char[len+1];
		memcpy(certBuf, start, len);
		certBuf[len] = '\0';
		uint32_t rawLength;
		uint8_t* rawBuf = decodeBase64(certBuf, &rawLength);
		delete [] certBuf;

		if (!data) {
			data = new uint8_t[rawLength];
		} else {
			uint8_t* newBuf = new uint8_t[dataLength + rawLength];
			memcpy(newBuf, data, dataLength);
			delete [] data;
			data = newBuf;
		}
		memcpy(data + dataLength, rawBuf, rawLength);
		dataLength += rawLength;
		delete [] rawBuf;

	}
	if (!data) {
		fprintf(stderr, "Bad certificate file\n");
		throw SignBadCert;
	}
	SISBlob* blob = new SISBlob(data, dataLength);
	delete [] data;
	return new SISCertificateChain(blob);
}
*/

static SISCertificateChain* makeChain(const char* certData) {
	BIO* in = BIO_new_mem_buf((void*) certData, -1);
	BIO* out = BIO_new(BIO_s_mem());

	while (true) {
		X509* cert = PEM_read_bio_X509(in, NULL, NULL, NULL);
		if (!cert) {
			unsigned long err = ERR_peek_last_error();
			int lib = ERR_GET_LIB(err);
			int func = ERR_GET_FUNC(err);
			int reason = ERR_GET_REASON(err);
			if (lib == ERR_LIB_PEM && func == PEM_F_PEM_READ_BIO && reason == PEM_R_NO_START_LINE)
				break;
			ERR_print_errors_fp(stderr);
			throw SignBadCert;
		}
		i2d_X509_bio(out, cert);

		X509_OBJECT obj;
		obj.type = X509_LU_X509;
		obj.data.x509 = cert;
		X509_OBJECT_free_contents(&obj);
	}
	BIO_free_all(in);

	char* ptr;
	long length = BIO_get_mem_data(out, &ptr);
	if (length <= 0) {
		fprintf(stderr, "Bad certificate file\n");
		throw SignBadCert;
	}
	SISBlob* blob = new SISBlob((uint8_t*) ptr, length);
	BIO_free_all(out);

	return new SISCertificateChain(blob);
}

SISSignatureCertificateChain* makeChain(SISField* controller, const char* certData, const char* keyData, const char* passphrase, SigType type) {
	SISSignature* signature = makeSignature(controller, keyData, passphrase, type);
	SISArray* signatures = new SISArray(SISFieldType::SISSignature);
	signatures->AddElement(signature);
	SISCertificateChain* chain = makeChain(certData);
	return new SISSignatureCertificateChain(signatures, chain);
}

void initSigning() {
	OpenSSL_add_all_algorithms();
	ERR_load_crypto_strings();
//	EVP_add_cipher(EVP_des_ede3_cbc());
}

void cleanupSigning() {
	ERR_remove_state(0);
//	ENGINE_cleanup();
//	CONF_modules_unload();
	EVP_cleanup();
	ERR_free_strings();
	CRYPTO_cleanup_all_ex_data();
}


