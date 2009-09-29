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

#include "sisfield.h"

void printField(SISField* field, int indent = 0) {
	for (int i = 0; i < indent; i++)
		printf("  ");
	printf("%d\n", field->Id());
	SISCompound* compound = dynamic_cast<SISCompound*>(field);
	if (compound) {
		for (uint32_t i = 0; i < compound->Count(); i++)
			printField(compound->ElementAt(i), indent+1);
	}
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("%s file [offset]\n", argv[0]);
		return 1;
	}
	FILE* in = fopen(argv[1], "rb");
	if (!in) {
		perror(argv[1]);
		return 1;
	}
	fseek(in, 0, SEEK_END);
	uint32_t len = ftell(in);
	fseek(in, 0, SEEK_SET);
	uint8_t* buffer = new uint8_t[len];
	fread(buffer, 1, len, in);
	fclose(in);
	int offset = 16;
	if (argc > 2) {
		offset = atoi(argv[2]);
	}
	const uint8_t* ptr = buffer + offset;
	len -= offset;
	try {
		SISField* field = SISField::LoadOneField(ptr, len);
		printField(field);
		delete field;
	} catch (int i) {
		printf("caught error: %d\n", i);
	}
	delete [] buffer;

	return 0;
}

