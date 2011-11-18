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

/**
* \file File.h
* \brief File System API
* \author Niklas Nummelin
*
* MAFS is a standard C file stream implementation. You may either mount a binary image as a read-only file source or use stores for
* the ability to write and read from a file. Use the bundle tool to build binary images.
*/

#ifndef _SE_MSAB_FILE_H_
#define _SE_MSAB_FILE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <ma.h>

#ifdef SEEK_SET
#undef SEEK_SET
#endif

#ifdef SEEK_CUR
#undef SEEK_CUR
#endif

#ifdef SEEK_END
#undef SEEK_END
#endif

#ifdef EOF
#undef EOF
#endif

#ifdef FILENAME_MAX
#undef FILENAME_MAX
#endif

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#define EOF (-1)
#define FILENAME_MAX 256

struct MA_FILE_t;
typedef struct MA_FILE_t MA_FILE;

#ifndef DONT_USE_TYPE_DEFINES
//typedef unsigned int size_t;
typedef unsigned int fpos_t;
#endif

/**
* Opens the file whose name is specified in the parameter filename and associates
* it with a stream that can be identified in future operations by the FILE object whose pointer
* is returned. The operations that are allowed on the stream and how these are performed are
* defined by the mode parameter. Whenever writing is requested the file is represented using
* a store. If the file is opened as read-only the system first checks if the file exist as a store.
* If not it searches through the mounted file system for the file.

* \param filename C string containing a file name. (note '..' isn't supported yet)
* \param mode C string containing a file access modes.

* \return If the file has been succesfully opened the function will return a pointer to a FILE object
* that is used to identify the stream on all further operations involving it. Otherwise, a null pointer is returned.
*/
MA_FILE * MA_fopen ( const char * filename, const char * mode );

/**
* Closes the file associated with the stream and disassociates it.
* All internal buffers associated with the stream are flushed: the content of any unwritten buffer is written and the content of any unread buffer is discarded.
* Even if the call fails, the stream passed as parameter will no longer be associated with the file.
* If the file has been written to, it is saved to a store.

* \param stream Pointer to a FILE object that specifies the stream to be closed.

* \return If the stream is successfully closed, a zero value is returned. On failure, EOF is returned.
*/
int MA_fclose ( MA_FILE * stream );

/**
* Read block of data from stream
* Reads an array of count elements, each one with a size of size bytes, from the stream and stores them in the block of memory specified by ptr.
* The postion indicator of the stream is advanced by the total amount of bytes read.
* The total amount of bytes read if successful is (size * count).

* \param ptr Pointer to a block of memory with a minimum size of (size*count) bytes.
* \param size Size in bytes of each element to be read.
* \param count Number of elements, each one with a size of size bytes.
* \param stream Pointer to a FILE object that specifies an input stream.

* \return The total number of elements successfully read is returned as a size_t object, which is an integral data type.
* If this number differs from the count parameter, either an error occured or the End Of File was reached.
* You can use either ferror or feof to check whether an error happened or the End-of-File was reached.
*/
size_t MA_fread ( void * ptr, size_t size, size_t count, MA_FILE * stream);

/**
* Writes an array of count elements, each one with a size of size bytes, from the block of memory pointed by ptr to the current position in the stream.
* The postion indicator of the stream is advanced by the total amount of bytes written.
* The total amount of bytes written is (size * count).

* \param ptr Pointer to the array of elements to be written.
* \param size Size in bytes of each element to be written.
* \param count Number of elements, each one with a size of size bytes.
* \param stream Pointer to a FILE object that specifies an output stream.

* \return The total number of elements successfully written is returned as a size_t object, which is an integral data type.
* If this number differs from the count parameter, it indicates an error.
*/
size_t MA_fwrite ( const void * ptr, size_t size, size_t count, MA_FILE * stream);

/**
* Sets the position indicator associated with the stream to a new position defined by adding offset to a reference position specified by origin.
* The End-of-File internal indicator of the stream is cleared after a call to this function, and all effects from previous calls to ungetc are dropped.
* When using fseek on text files with offset values other than zero or values retrieved with ftell, bear in mind that on some platforms some format transformations occur with text files which can lead to unexpected repositioning.
* On streams open for update (read+write), a call to fseek allows to switch between reading and writing.

* \param stream Pointer to a FILE object that identifies the stream.
* \param offset Number of bytes to offset from origin.
* \param origin Position from where offset is added. It is specified by one of the following constants:
* #SEEK_SET Beginning of file.
* #SEEK_CUR Current position of the file pointer.
* #SEEK_END End of file.
* \return If successful, the function returns a zero value. Otherwise, it returns nonzero value.
*/
int MA_fseek ( MA_FILE * stream, long int offset, int origin );

/**
* Checks whether the End-of-File indicator associated with stream is set, returning a value different from zero if it is.
* This indicator is generally set by a previous operation on the stream that reached the End-of-File.
* Further operations on the stream once the End-of-File has been reached will fail until either rewind, fseek or fsetpos is successfully called to set the position indicator to a new value.

* \param stream Pointer to a FILE object that identifies the stream.

* \return A non-zero value is returned in the case that the End-of-File indicator associated with the stream is set.
* Otherwise, a zero value is returned.
*/
int MA_feof ( MA_FILE * stream );

/**
* Checks if the error indicator associated with stream is set, returning a value different from zero if it is.
* This indicator is generaly set by a previous operation on the stream that failed.

* \param stream Pointer to a FILE object that identifies the stream.

* \return If the error indicator associated with the stream was set, the function returns a nonzero value.
* Otherwise, it returns a zero value.
*/
int MA_ferror( MA_FILE *stream );


/**
* Sets the position indicator associated with stream to the beginning of the file.
* A call to rewind is equivalent to:
* fseek ( stream , 0L , SEEK_SET );
* except that, unlike fseek, rewind clears the error indicator.
* On streams open for update (read+write), a call to rewind allows to switch between reading and writing.

* \param stream Pointer to a FILE object that identifies the stream.
*/
void MA_rewind( MA_FILE *stream );

/**
* Returns the character currently pointed by the internal file position indicator of the specified stream. The internal file position indicator is then advanced by one character to point to the next character.
* fgetc and getc are equivalent, except that the latter one may be implemented as a macro.

* \param stream Pointer to a FILE object that identifies the stream on which the operation is to be performed.
* \return The character read is returned as an int value. If the End-of-File is reached or a reading error happens, the function returns EOF and the corresponding error or eof indicator is set. You can use either ferror or feof to determine whether an error happened or the End-Of-File was reached.
*/
int MA_fgetc ( MA_FILE * stream );

/**
* Returns the character currently pointed by the internal file position indicator of the specified stream. The internal file position indicator is then advanced by one character to point to the next character.
* getc is equivalent to fgetc and also expects a stream as parameter, but getc may be implemented as a macro, so the argument passed to it should not be an expression with potential side effects.
* See getchar for a similar function without stream parameter.

* \param stream pointer to a FILE object that identifies the stream on which the operation is to be performed.

* \return The character read is returned as an int value.
* If the End-of-File is reached or a reading error happens, the function returns EOF and the corresponding error or eof indicator is set. You can use either ferror or feof to determine whether an error happened or the End-Of-File was reached.
*/
int MA_getc ( MA_FILE * stream );

/**
* Reads characters from stream and stores them as a C string into str until (num-1) characters have been read or either a newline or a the End-of-File is reached, whichever comes first.
* A newline character makes fgets stop reading, but it is considered a valid character and therefore it is included in the string copied to str.
* A null character is automatically appended in str after the characters read to signal the end of the C string.

* \param str Pointer to an array of chars where the string read is stored.
* \param num Maximum number of characters to be read (including the final null-character). Usually, the length of the array passed as str is used.
* \param stream Pointer to a FILE object that identifies the stream where characters are read from.
* To read from the standard input, stdin can be used for this parameter.

* \return On success, the function returns the same str parameter.
* If the End-of-File is encountered and no characters have been read, the contents of str remain unchanged and a null pointer is returned.
* If an error occurs, a null pointer is returned.
* Use either ferror or feof to check whether an error happened or the End-of-File was reached.
*/
char * MA_fgets ( char * str, int num, MA_FILE * stream );

/**
* Writes a character to the stream and advances the position indicator.
* The character is written at the current position of the stream as indicated by the internal position indicator, which is then advanced one character.

* \param character Character to be written. The character is passed as its int promotion.
* \param stream Pointer to a FILE object that identifies the stream where the character is to be written.

* \return If there are no errors, the same character that has been written is returned.
* If an error occurs, EOF is returned and the error indicator is set (see ferror).
*/
int MA_fputc ( int character, MA_FILE * stream );

/**
* Writes a character to the stream and advances the position indicator.
* The character is written at the current position of the stream as indicated by the internal position indicator, which is then advanced one character.
* putc is equivalent to fputc and also expects a stream as parameter, but putc may be implemented as a macro, so the argument passed should not be an expression with potential side effects.
* See putchar for a similar function without stream parameter.

* \param character Character to be written. The character is passed as its int promotion.
* \param stream Pointer to a FILE object that identifies the stream where the character is to be written.

* \return If there are no errors, the same character that has been written is returned.
* If an error occurs, EOF is returned and the error indicator is set.
*/
int MA_putc ( int character, MA_FILE * stream );

/**
* Writes the string pointed by str to the stream.
* The function begins copying from the address specified (str) until it reaches the terminating null character ('\\0'). This final null-character is not copied to the stream.

* \param str An array containing the null-terminated sequence of characters to be written.
* \param stream Pointer to a FILE object that identifies the stream where the string is to be written.

* \return On success, a non-negative value is returned.
* On error, the function returns EOF.
*/
int MA_fputs ( const char * str, MA_FILE * stream );

/**
* Returns the current value of the position indicator of the stream.
* For binary streams, the value returned corresponds to the number of bytes from the beginning of the file.
* For text streams, the value is not guaranteed to be the exact number of bytes from the beginning of the file, but the value returned can still be used to restore the position indicator to this position using fseek.

* \param stream Pointer to a FILE object that identifies the stream.

* \return On success, the current value of the position indicator is returned.
* If an error occurs, -1L is returned, and the global variable errno is set to a positive value. This value can be interpreted by perror.
*/
long int MA_ftell ( MA_FILE * stream );

/**
* Gets the information needed to uniquely identify the current value of the stream's position indicator and stores it in the location pointed by position.
* The parameter position should point to an already allocated object of the type fpos_t, which is only intended to be used as a paremeter in future calls to fsetpos.
* To retrieve the value of the internal file position indicator as an integer value, use ftell function instead.

* \param stream Pointer to a FILE object that identifies the stream.
* \param position Pointer to a fpos_t object.

* \return The function return a zero value on success, and a non-zero value in case of error.
*/
int MA_fgetpos ( MA_FILE * stream, fpos_t * position );

/**
* Changes the internal file position indicator associated with stream to a new position. The position parameter is a pointer to an fpos_t object whose value shall have been previously obtained with a call to fgetpos.
* The End-of-File internal indicator of the stream is cleared after a call to this function, and all effects from previous calls to ungetc are dropped.
* On streams open for update (read+write), a call to fsetpos allows to switch between reading and writing.

* \param stream Pointer to a FILE object that identifies the stream.
* \param pos Pointer to a fpos_t object containing a position previously obtained with fgetpos.

* \return If successful, the function returns a zero value.
* Otherwise, it returns a nonze
*/
int MA_fsetpos ( MA_FILE * stream, const fpos_t * pos );

// file system functions are redefined to the standard names, original implementations have other names to prevent collisions.
#ifdef fopen
#undef fopen
#endif
#define fopen MA_fopen

#ifdef fread
#undef fread
#endif
#define fread MA_fread

#ifdef fwrite
#undef fwrite
#endif
#define fwrite MA_fwrite

#ifdef fclose
#undef fclose
#endif
#define fclose MA_fclose

#ifdef fseek
#undef fseek
#endif
#define fseek MA_fseek

#ifdef feof
#undef feof
#endif
#define feof MA_feof

#ifdef ferror
#undef ferror
#endif
#define ferror MA_ferror

#ifdef rewind
#undef rewind
#endif
#define rewind MA_rewind

#ifdef fgetc
#undef fgetc
#endif
#define fgetc MA_fgetc

#ifdef fputc
#undef fputc
#endif
#define fputc MA_fputc

#ifdef fgets
#undef fgets
#endif
#define fgets MA_fgets

#ifdef fputs
#undef fputs
#endif
#define fputs MA_fputs

#ifdef getc
#undef getc
#endif
#define getc MA_getc

#ifdef putc
#undef putc
#endif
#define putc MA_putc

#ifdef ftell
#undef ftell
#endif
#define ftell MA_ftell

#ifdef fgetpos
#undef fgetpos
#endif
#define fgetpos MA_fgetpos

#ifdef fsetpos
#undef fsetpos
#endif
#define fsetpos MA_fsetpos

#ifdef FILE
#undef FILE
#endif
#define FILE MA_FILE

/**
* Mounts a file system image, stored as a binary resource.
* Only one file system image may be mounted at a time.
* All following calls to the file api functions will use this image.

* \param fileSystem MAHandle to the binary resource containing the
* file system image to be mounted.
* \param caseSensitive An int stating whether to take case into
* account (1) or not (0). Emulating a Windows file system should
* have this set as 0, while Unix should be 1.
*/
void setCurrentFileSystem(MAHandle fileSystem, int caseSensitive);

/**
* Frees the resources allocated for the current file system.
*/
void freeCurrentFileSystem(void);

/**
* Returns a checksum for the file system, if the header contains that.
* (Old Bundle format does not have a checksum.)
*
* \param fileSystem MAHandle to the binary resource containing the
* file system image.
*
* \return A checksum value, returns 0 if there is no checksum available.
*/
int getFileSystemChecksum(MAHandle fileSystem);

/**
* Extract the current file system bundle to the local file
* system on the device/emulator.
*
* \param destPath Path to the directory on the local file system
* where the file tree should be extracted. The path MUST end with
* a path delimeter.
*
* \return 1 on success, -1 on error.
*/
int extractCurrentFileSystem(const char* destPath);

/*
// not implemented yet
int remove ( const char * filename );
int rename ( const char * oldname, const char * newname );
FILE * tmpfile ( void );
char * tmpnam ( char * str );

FILE * reopen ( const char * filename, const char * mode, FILE * stream );
int fflush ( FILE * stream );
void setbuf ( FILE * stream, char * buffer );
int setvbuf ( FILE * stream, char * buffer, int mode, size_t size );

int fprintf ( FILE * stream, const char * format, ... );
int fputc ( int character, FILE * stream );
int fputs ( const char * str, FILE * stream );
int putc ( int character, FILE * stream );
size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream );

int fscanf ( FILE * stream, const char * format, ... );

int fgetc ( FILE * stream );
char * fgets ( char * str, int num, FILE * stream );
int getc ( FILE * stream );
char *gets ( char * str );

int ungetc ( int character, FILE * stream );

int fgetpos ( FILE * stream, fpos_t * position );

*/

#ifdef __cplusplus
}	//extern "C"
#endif

#endif	//_SE_MSAB_FILE_H_
