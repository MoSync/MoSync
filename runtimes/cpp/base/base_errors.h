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

#ifndef _BASE_ERRORS_H_
#define _BASE_ERRORS_H_

#include "helpers/helpers.h"

#define BASE_ERRORS(m)\
	m(40001, ERR_RES_INVALID_TYPE, "Invalid resource type")\
	m(40002, ERR_RES_DESTROY_FLUX, "Attempted to destroy a resource that is in flux")\
	m(40003, ERR_RES_INVALID_INDEX, "Resource handle out of bounds")\
	m(40004, ERR_RES_OVERWRITE, "Attempted to overwrite an existing resource")\
	m(40005, ERR_RES_LOAD_UBIN, "Attempted to load a UBIN from memory")\
	m(40006, ERR_DATA_OOB, "Data object access out of bounds")\
	m(40007, ERR_RES_FILE_INCONSISTENT, "Resource file inconsistent")\
	m(40008, ERR_DIVISION_BY_ZERO, "Division by zero")\
	m(40012, ERR_CONN_HANDLE_INVALID, "Connection handle invalid")\
	m(40013, ERR_CONN_ALREADY_READING, "Connection already reading")\
	m(40014, ERR_CONN_ALREADY_WRITING, "Connection already writing")\
	m(40015, ERR_DATA_READ_ONLY, "Data object is read-only")\
	m(40016, ERR_CONN_NOT_HTTP, "Connection is not HTTP")\
	m(40017, ERR_HTTP_NOT_SETUP, "HTTP connection is not in Setup state")\
	m(40018, ERR_HTTP_NOT_FINISHED, "HTTP connection is not Finished")\
	m(40019, ERR_HTTP_ALREADY_FINISHED, "HTTP connection is already Finished")\
	m(40020, ERR_CONN_ACTIVE, "Connection is not idle")\
	m(40021, ERR_SOUND_CHANNEL_INVALID, "Sound channel handle invalid")\
	m(40022, ERR_SOUND_CHANNEL_INACTIVE, "Sound channel inactive")\
	m(40023, ERR_SOUND_LOOP_COUNT_INVALID, "Sound loop count invalid")\
	m(40024, ERR_STORE_HANDLE_INVALID, "Store handle invalid")\
	m(40025, ERR_DATA_ACCESS_FAILED, "Data object access failed")\
	m(40026, ERR_STORE_WRITE_FAILED, "Store write failed")\
	m(40027, ERR_STORE_READ_FAILED, "Store read failed")\
	m(40028, ERR_MEMORY_ALIGNMENT, "Memory alignment error")\
	m(40029, ERR_MIME_READ_FAILED, "Failed to read MIME type")\
	m(40030, ERR_PROGRAM_FILE_BROKEN, "The program file is broken")\
	m(40031, ERR_MEMORY_OOB, "Data memory access out of bounds")\
	m(40032, ERR_BAD_SYSCALL, "Attempted to use an unallocated syscall number")\
	m(40033, ERR_IMEM_OOB, "Instruction memory access out of bounds")\
	m(40034, ERR_ILLEGAL_INSTRUCTION_FORM, "Illegal instruction form")\
	m(40035, ERR_ILLEGAL_INSTRUCTION, "Illegal instruction")\
	m(40036, ERR_FUNCTION_UNSUPPORTED, "Function is not supported on this platform")\
	m(40037, ERR_PROGRAM_LOAD_FAILED, "Failed to load program")\
	m(40038, ERR_POLYGON_TOO_FEW_POINTS, "Too few points in polygon")\
	m(40039, ERR_IMAGE_TRANSFORM_INVALID, "Invalid transformation type")\
	m(40040, ERR_IMAGE_OOB, "Image access out of bounds")\
	m(40041, ERR_SLD_LOAD_FAILED, "Failed to load SLD file")\
	m(40042, ERR_STREAM_TOO_MANY, "Tried to open too many media streams")\
	m(40043, ERR_STREAM_HANDLE, "Invalid stream handle")\
	m(40044, ERR_IDL_VERSION, "IDL version mismatch")\
	m(40045, ERR_UNSUPPORTED_BPP, "Unsupported bpp")\
	m(40046, ERR_SOURCE_RECT_OOB, "Source rect out of bounds")\
	m(40047, ERR_RES_OOM, "Out of resource memory")\
	m(40048, ERR_CONN_NOT_STREAM, "Connection is not a stream")\
	m(40049, ERR_CONN_NOT_SERVER, "Connection is not a server")\
	m(40050, ERR_CONN_ALREADY_ACCEPTING, "Connection already accepting")\
	m(40051, ERR_MEMORY_PROTECTED, "Attempted access protected memory")\
	m(40052, ERR_IMAGE_SIZE_INVALID, "Invalid image size")\
	m(40053, ERR_MEMORY_NULL, "Attempted to access null pointer.")\
	m(40054, ERR_GDB_SERVER_OPEN, "Gdb stub failed to open listening server socket.")\
	m(40055, ERR_RES_NO_RESOURCES, "Attempted to use a resource, but no resources exist.")\
	m(40056, ERR_FILE_HANDLE_INVALID, "File handle invalid")\
	m(40057, ERR_INVALID_FILE_ACCESS_MODE, "Invalid file access mode")\
	m(40058, ERR_FILE_CLOSED, "The file is closed")\
	m(40059, ERR_INVALID_FILE_SEEK_MODE, "Invalid file seek mode")\
	m(40060, ERR_INVALID_CAMERA_FORMAT_INDEX, "Invalid camera format index")\
	m(40061, ERR_INVALID_PIM_HANDLE, "Invalid PIM handle")\
	m(40062, ERR_INVALID_PIM_FIELD_INDEX, "Invalid PIM field index")\
	m(40063, ERR_INVALID_PIM_VALUE_INDEX, "Invalid PIM value index")\
	m(40064, ERR_MISSING_PIM_FIELD, "Missing PIM field")\
	m(40065, ERR_STACK_OOB, "Stack pointer out of bounds. Try to increase the stack size.")\
	m(40066, ERR_PIM_LIST_CLOSED, "The PIM item's parent list is closed")\
	m(40067, ERR_CAMERA_UNPOWERED, "Camera viewfinder is not active")\
	m(40068, ERR_TEXTBOX_ACTIVE, "Text box is already active")\
	m(40069, ERR_INVALID_PIM_BUFFER_SIZE, "Invalid PIM value buffer size")\
	m(40070, ERR_PIM_UNTERMINATED_STRING, "Unterminated PIM string value")\
	m(40071, ERR_PIM_EMPTY_STRING_ARRAY, "Empty PIM string array")\
	m(40072, ERR_IMAGE_LOAD_FAILED, "Image could not be loaded.")\
	m(40073, ERR_EXT_VERSION, "Extension version mismatch")\
	m(40074, ERR_EXT_LOAD, "Failed to load extensions")\
	m(40075, ERR_EXT_CALL, "Invalid extension call")\
	m(40076, ERR_FILE_LIST_SORT, "Invalid file list sorting option")\
	m(40077, ERR_FILE_PATH_INVALID, "Invalid file path")\
	m(40078, ERR_DB_INVALID_HANDLE, "Invalid database handle")\
	m(40079, ERR_DB_INVALID_COLUMN_INDEX, "Invalid database column index")\

DECLARE_ERROR_ENUM(BASE)

namespace MoSyncError {
	bool isBaseError(int code);
	void addRuntimeSpecificPanicInfo(char *ptr, bool newLines);
}

#endif /* _BASE_ERRORS_H_ */
