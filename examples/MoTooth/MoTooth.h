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

#ifndef MOTOOTH_H
#define MOTOOTH_H

//#define MAUTIL_VECTOR_DEBUGGING

#include <conprint.h>
#define printfln printf

#include <maassert.h>

#include "../common/helpers.h"
#include "MAHeaders.h"

struct SERVICE {
	int port;
	String name;
};

struct DEVICE {
	MABtAddr address;
	String name;
	Vector<SERVICE> services;
};

extern Vector<DEVICE> gDevices;
extern int gnServices;


#define STORENAME "MoTooth.sav"

///reads a device into the database using a DataHandler
bool readDevice(DataHandler& data, DEVICE& dev);
///reads the device database from the data object
bool readDatabase();
///adds a device to the database
void addDevice(const MABtDevice& d);
///adds a service to the database
void addService(int deviceIndex, const MABtService& s);

void writeDatabase();
void writeDevice(DataHandler& data, DEVICE& dev);
///returns the size the database would have if written to a data object
int calculateDatabaseSize();

void readStore();
void deleteStore();
void writeStore();

void menu();
void scan();
//int select();
void run();

bool doSelect(int& devIndex, int& servIndex);

const char* btaddr2string(const MABtAddr& a);

#endif	//MOTOOTH_H
