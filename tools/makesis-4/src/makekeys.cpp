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

    Martin Storsjö
    martin@martin.st
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
//#include <pwd.h>
//#include <unistd.h>
#define PATH_MAX 512

int main(int argc, char *argv[]) {
	bool cert = false;
	bool view = false;
	bool password = false;
	int len = 1024;
	int expdays = 365;

	const char* keyfile = NULL;
	const char* certfile = NULL;

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-cert"))
			cert = true;
		else if (!strcmp(argv[i], "-view"))
			view = true;
		else if (!strcmp(argv[i], "-password"))
			password = true;
		else if (!strcmp(argv[i], "-len"))
			len = atoi(argv[++i]);
		else if (!strcmp(argv[i], "-expdays"))
			expdays = atoi(argv[++i]);
		else {
			if (cert) {
				if (!keyfile)
					keyfile = argv[i];
				else if (!certfile)
					certfile = argv[i];
			} else if (view) {
				certfile = argv[i];
			}
		}
	}

	if (view) {
		char buffer[2*PATH_MAX];
		sprintf(buffer, "openssl x509 -in \"%s\" -text -noout", certfile);
		return system(buffer);
	}

	if (cert) {
		if (!certfile || !keyfile) {
			printf("Both a certificate and key file must be speicified to generate a certificate\n");
			return 1;
		}
		FILE* test = fopen(keyfile, "rb");
		char buffer[2*PATH_MAX];
		if (!test) {
			const char* temp = "dsaparam";
			sprintf(buffer, "openssl dsaparam -genkey %d -out %s", len, temp);
			if (system(buffer))
				return 1;
			sprintf(buffer, "openssl gendsa %s -out %s %s", password ? "-des3" : "", keyfile, temp);
			if (system(buffer))
				return 1;
			_unlink(temp);
		} else {
			fclose(test);
		}
		sprintf(buffer, "openssl req -new -x509 -nodes -sha1 -days %d -key %s -out %s", expdays, keyfile, certfile);
		if (system(buffer))
			return 1;
		printf("Done\n");
		return 0;
	}

	printf("%s -cert [-password] [-len keylength] [-expdays days] privatekey certificate\n", argv[0]);
	printf("%s -view certificate\n", argv[0]);
	

	return 0;
}

