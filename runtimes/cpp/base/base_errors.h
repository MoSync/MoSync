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

#define BASE_ERRORS(m)\
	m(ERR_RES_INVALID_TYPE, "Invalid resource type")\
	m(ERR_RES_DESTROY_FLUX, "Attempted to destroy a resource that is in flux")\
	m(ERR_RES_INVALID_INDEX, "Resource handle out of bounds")\
	m(ERR_RES_OVERWRITE, "Attempted to overwrite an existing resource")\
	m(ERR_RES_LOAD_UBIN, "Attempted to load a UBIN from memory")\
	m(ERR_DATA_OOB, "Data object access out of bounds")\
	m(ERR_RES_FILE_INCONSISTENT, "Resource file inconsistent")\
	m(ERR_DIVISION_BY_ZERO, "Division by zero")\
	m(ERR_MAX_TILE_LAYERS, "No more available tile layers")\
	m(ERR_TILE_LAYER_HANDLE, "Invalid tile layer handle")\
	m(ERR_TILE_LAYER_INACTIVE, "Tile layer inactive")\
	m(ERR_CONN_HANDLE_INVALID, "Connection handle invalid")\
	m(ERR_CONN_ALREADY_READING, "Connection already reading")\
	m(ERR_CONN_ALREADY_WRITING, "Connection already writing")\
	m(ERR_DATA_READ_ONLY, "Data object is read-only")\
	m(ERR_CONN_NOT_HTTP, "Connection is not HTTP")\
	m(ERR_HTTP_NOT_SETUP, "HTTP connection is not in Setup state")\
	m(ERR_HTTP_NOT_FINISHED, "HTTP connection is not Finished")\
	m(ERR_HTTP_ALREADY_FINISHED, "HTTP connection is already Finished")\
	m(ERR_CONN_ACTIVE, "Connection is not idle")\
	m(ERR_SOUND_CHANNEL_INVALID, "Sound channel handle invalid")\
	m(ERR_SOUND_CHANNEL_INACTIVE, "Sound channel inactive")\
	m(ERR_SOUND_LOOP_COUNT_INVALID, "Sound loop count invalid")\
	m(ERR_STORE_HANDLE_INVALID, "Store handle invalid")\
	m(ERR_DATA_ACCESS_FAILED, "Data object access failed")\
	m(ERR_STORE_WRITE_FAILED, "Store write failed")\
	m(ERR_STORE_READ_FAILED, "Store read failed")\
	m(ERR_MEMORY_ALIGNMENT, "Memory alignment error")\
	m(ERR_MIME_READ_FAILED, "Failed to read MIME type")\
	m(ERR_PROGRAM_FILE_BROKEN, "The program file is broken")\
	m(ERR_MEMORY_OOB, "Data memory access out of bounds")\
	m(ERR_BAD_SYSCALL, "Attempted to use an unallocated syscall number")\
	m(ERR_IMEM_OOB, "Instruction memory access out of bounds")\
	m(ERR_ILLEGAL_INSTRUCTION_FORM, "Illegal instruction form")\
	m(ERR_ILLEGAL_INSTRUCTION, "Illegal instruction")\
	m(ERR_FUNCTION_UNSUPPORTED, "Function is not supported on this platform")\
	m(ERR_PROGRAM_LOAD_FAILED, "Failed to load program")\
	m(ERR_POLYGON_TOO_FEW_POINTS, "Too few points in polygon")\
	m(ERR_IMAGE_TRANSFORM_INVALID, "Invalid transformation type")\
	m(ERR_IMAGE_OOB, "Image access out of bounds")\
	m(ERR_SLD_LOAD_FAILED, "Failed to load SLD file")\
	m(ERR_STREAM_TOO_MANY, "Tried to open too many media streams")\
	m(ERR_STREAM_HANDLE, "Invalid stream handle")\
	m(ERR_IDL_VERSION, "IDL version mismatch")\
	m(ERR_UNSUPPORTED_BPP, "Unsupported bpp")\
	m(ERR_SOURCE_RECT_OOB, "Source rect out of bounds")\
	m(ERR_RES_OOM, "Out of resource memory")\
	m(ERR_CONN_NOT_STREAM, "Connection is not a stream")\
	m(ERR_CONN_NOT_SERVER, "Connection is not a server")\
	m(ERR_CONN_ALREADY_ACCEPTING, "Connection already accepting")\
	m(ERR_MEMORY_PROTECTED, "Attempted access protected memory")\
	m(ERR_IMAGE_SIZE_INVALID, "Invalid image size")\
	m(ERR_MEMORY_NULL, "Attempted to access null pointer.")\
	m(ERR_GDB_SERVER_OPEN, "Gdb stub failed to open listening server socket.")

DECLARE_ERROR_ENUM(BASE)

namespace MoSyncError {
	bool isBaseError(int code);
	void addRuntimeSpecificPanicInfo(char *ptr, bool newLines);
}

#endif /* _BASE_ERRORS_H_ */
