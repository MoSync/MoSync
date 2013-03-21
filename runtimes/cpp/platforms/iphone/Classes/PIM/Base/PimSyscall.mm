/* Copyright (C) 2011 Mobile Sorcery AB

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

#include "config_platform.h"

#import "syscall.h"
#include "PimDatabase.h"
#import "PimUtil.h"
#import "PimError.h"
#import "MoSyncPanic.h"

using namespace MoSyncError;

//***************************************************************************
//Variables
//***************************************************************************

PimDatabase* sPimDatabase = NULL;

//***************************************************************************
//Initialization
//***************************************************************************

void MAPimInit() {
	sPimDatabase = [[PimDatabase alloc] init];
}

void MAPimClose() {
	[sPimDatabase release];
    [PimUtils deleteInstance];
}


MAHandle maPimListOpen(int listType)
{
    return [sPimDatabase pimListOpen:listType];
}

MAHandle maPimListNextSummary(MAHandle list)
{
    return [sPimDatabase pimListNext:list];
}

MAHandle maPimListNext(MAHandle list)
{
    return [sPimDatabase pimListNext:list];
}

int maPimListClose(MAHandle list)
{
     return [sPimDatabase pimListClose:list];
}

int maPimItemCount(MAHandle item)
{
    PimItem* pimItem = [sPimDatabase getItem:item];

    if (!pimItem)
    {
        return [[MoSyncPanic getInstance] error:MA_PIM_ERR_HANDLE_INVALID
                                  withPanicCode:PANIC_HANDLE_INVALID
                                  withPanicText:@PANIC_HANDLE_INVALID_TEXT];
    }

    return [pimItem count];
}

int maPimItemGetField(MAHandle item, int n)
{
    PimItem* pimItem = [sPimDatabase getItem:item];

    if (!pimItem)
    {
        return [[MoSyncPanic getInstance] error:MA_PIM_ERR_HANDLE_INVALID
                                  withPanicCode:PANIC_HANDLE_INVALID
                                  withPanicText:@PANIC_HANDLE_INVALID_TEXT];
    }

    return [pimItem getFieldID:n];
}

int maPimItemFieldCount(MAHandle item, int field)
{
    PimItem* pimItem = [sPimDatabase getItem:item];

    if (!pimItem)
    {
        return [[MoSyncPanic getInstance] error:MA_PIM_ERR_HANDLE_INVALID
                                  withPanicCode:PANIC_HANDLE_INVALID
                                  withPanicText:@PANIC_HANDLE_INVALID_TEXT];
    }

    return [pimItem fieldCount:field];
}

int maPimItemGetAttributes(MAHandle item, int field, int index)
{
    PimItem* pimItem = [sPimDatabase getItem:item];

    if (!pimItem)
    {
        return [[MoSyncPanic getInstance] error:MA_PIM_ERR_HANDLE_INVALID
                                  withPanicCode:PANIC_HANDLE_INVALID
                                  withPanicText:@PANIC_HANDLE_INVALID_TEXT];
    }

    return [pimItem getAttribute:field indexValue:index];
}


int maPimItemSetLabel(const MA_PIM_ARGS* args, int index)
{

    PimItem* pimItem = [sPimDatabase getItem:args->item];

    if (!pimItem)
    {
        return [[MoSyncPanic getInstance] error:MA_PIM_ERR_HANDLE_INVALID
                                  withPanicCode:PANIC_HANDLE_INVALID
                                  withPanicText:@PANIC_HANDLE_INVALID_TEXT];
    }

    return [pimItem setLabel:args indexValue:index];
}

int maPimItemGetLabel(const MA_PIM_ARGS* args, int index)
{
    PimItem* pimItem = [sPimDatabase getItem:args->item];

    if (!pimItem)
    {
        return [[MoSyncPanic getInstance] error:MA_PIM_ERR_HANDLE_INVALID
                                  withPanicCode:PANIC_HANDLE_INVALID
                                  withPanicText:@PANIC_HANDLE_INVALID_TEXT];
    }

    return [pimItem getLabel:args indexValue:index];
}

int maPimFieldType(MAHandle list, int field)
{
    bool singleFieldValue;
    int fieldType;
    int resultCode;

    // Check if the list handle is valid.
    if (MA_PIM_CONTACTS != list)
    {
        return [[MoSyncPanic getInstance] error:MA_PIM_ERR_HANDLE_INVALID
                                  withPanicCode:PANIC_HANDLE_INVALID
                                  withPanicText:@PANIC_HANDLE_INVALID_TEXT];
    }

    resultCode = [[PimUtils sharedInstance] fieldStructure:field
                                                       setType:&fieldType
                                              setIsSingleValue:&singleFieldValue];
    // Check for errors.
    if (MA_PIM_ERR_NONE == resultCode)
    {
        return fieldType;
    }

    return resultCode;
}

int maPimItemGetValue(const MA_PIM_ARGS *args, int index)
{
    PimItem* pimItem = [sPimDatabase getItem:args->item];

    if (!pimItem)
    {
        return [[MoSyncPanic getInstance] error:MA_PIM_ERR_HANDLE_INVALID
                                  withPanicCode:PANIC_HANDLE_INVALID
                                  withPanicText:@PANIC_HANDLE_INVALID_TEXT];
    }

    return [pimItem getValue:args indexValue:index];
}


int maPimItemSetValue(const MA_PIM_ARGS* args, int index, int attributes)
{
    PimItem* pimItem = [sPimDatabase getItem:args->item];

    if (!pimItem)
    {
        return [[MoSyncPanic getInstance] error:MA_PIM_ERR_HANDLE_INVALID
                                  withPanicCode:PANIC_HANDLE_INVALID
                                  withPanicText:@PANIC_HANDLE_INVALID_TEXT];
    }

    return [pimItem setValue:args
                  indexValue:index
              valueAttribute:attributes];
}

int maPimItemAddValue(const MA_PIM_ARGS* args, int attributes)
{
    PimItem* pimItem = [sPimDatabase getItem:args->item];

    if (!pimItem)
    {
        return [[MoSyncPanic getInstance] error:MA_PIM_ERR_HANDLE_INVALID
                                  withPanicCode:PANIC_HANDLE_INVALID
                                  withPanicText:@PANIC_HANDLE_INVALID_TEXT];
    }

    return [pimItem addValue:args
               withAttribute:attributes];
}

int maPimItemRemoveValue(MAHandle item, int field, int index)
{
    PimItem* pimItem = [sPimDatabase getItem:item];

    if (!pimItem)
    {
        return [[MoSyncPanic getInstance] error:MA_PIM_ERR_HANDLE_INVALID
                                  withPanicCode:PANIC_HANDLE_INVALID
                                  withPanicText:@PANIC_HANDLE_INVALID_TEXT];
    }

    return [pimItem removeValue:field atIndex:index];
}

int maPimItemClose(MAHandle item)
{
    return [sPimDatabase closeItem:item];
}

MAHandle maPimItemCreate(MAHandle list)
{
    return [sPimDatabase createItem:list];
}

int maPimItemRemove(MAHandle list, MAHandle item)
{
    return [sPimDatabase removeItem:list item:item];
}
