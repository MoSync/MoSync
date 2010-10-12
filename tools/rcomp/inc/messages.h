/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
* A Message is an object containing information about a particular
* compiler warning or error.
*
*/


#ifndef __MESSAGES_H__
#define __MESSAGES_H__



#include "stringar.h"
#include "astring.h"

enum { KError, KWarning, };

enum {
	LT_001, LT_002, LT_003, LT_004, LT_005,
	LT_006, LT_007, LT_008, LT_009, LT_010,
	LT_011, LT_012, LT_013, LT_014, LT_015,
	LT_016, LT_017, LT_018, LT_019, LT_020,
	LT_021, LT_022, LT_023, LT_024, LT_025,
	LT_026, LT_027, LT_028, LT_029, LT_030,
	LT_031, LT_032, LT_033, LT_034, LT_035,
	LT_036, LT_037, LT_038, LT_039, LT_040,
	LT_041, LT_042, LT_043, LT_044, LT_045,
	LT_046, LT_047,
	};

class Message : public ArrayItem
	{
public:
	Message(String aMessage, int aIndex, String aTextIndex, bool aActivated=true, int aMessageType=KError);
	~Message();
	void SetActivationStatus(bool aActivationStatus);
	bool GetActivated();
	String GetMessageOutput();
	String GetTextIndex();
	int GetIndex();
private:
	String iMessage;
	int iIndex;
	String iTextIndex;
	bool iActivated;
	int iMessageType;
	};

class MessageArray : public Array
	{
public:
	MessageArray();
	void Add( Message * pNewItem);
	bool InstanceExists();
	Message * GetTextEntry(String aEntry);
	Message * GetEntry(int aEntry);
private:
	bool iInstanceExists;
	};

class MessageArrayIterator : public ArrayIterator
	{
public:
	MessageArrayIterator(const MessageArray & c);
	Message * operator()();
	};

#endif
