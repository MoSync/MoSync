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

#ifndef BTDIALOG_H
#define BTDIALOG_H

enum BtdResult {
	BTD_ERROR,
	BTD_OK,
	BTD_CANCEL
};

#ifdef BTDIALOG_EXPORTS
#	define BTDIALOG_API __declspec(dllexport)
#else
#	define BTDIALOG_API __declspec(dllimport)
#endif

typedef unsigned char byte;
typedef unsigned short wchar;

typedef struct BT_DEVICE {
	byte address[6];
	wchar name[248];
} BT_DEVICE;

//returns one of BtdResult.
//fills device if BTD_OK is returned.
int BTDIALOG_API btDialog(BT_DEVICE* device);

#endif	//BTDIALOG_H
