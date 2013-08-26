/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
