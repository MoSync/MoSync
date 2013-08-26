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
