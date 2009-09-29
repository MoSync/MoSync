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

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("%s file\n", argv[0]);
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
	const uint8_t* ptr = buffer + 16;
	len -= 16;
	try {
		SISField* field = SISField::LoadOneField(ptr, len);
		if (field->Id() == SISFieldType::SISContents) {
			SISContents* contents = (SISContents*) field;
			SISCompressed* compressed = (SISCompressed*) contents->FindElement(SISFieldType::SISCompressed);
			compressed->Uncompress();
			compressed->LoadChild();
			SISField* controllerField = compressed->Field();
			if (controllerField->Id() == SISFieldType::SISController) {
				SISController* controller = (SISController*) controllerField;
				SISInfo* info = (SISInfo*) controller->FindElement(SISFieldType::SISInfo);
				SISDateTime* dateTime = (SISDateTime*) info->FindElement(SISFieldType::SISDateTime);
				SISDate* date = (SISDate*) dateTime->FindElement(SISFieldType::SISDate);
				SISTime* time = (SISTime*) dateTime->FindElement(SISFieldType::SISTime);
				printf("%x,%x,%x,%x,%x,%x\n", date->iYear, date->iMonth, date->iDay, time->iHours, time->iMinutes, time->iSeconds);
			}
		}
		delete field;
	} catch (int i) {
		printf("caught error: %d\n", i);
	}
	delete [] buffer;

	return 0;
}

