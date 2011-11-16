
/*
Hello!

This program packs fiels a directory tree into a binary file.

To compile using gcc:

gcc mopack.cpp -o mopack
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define SUCCESS 1
#define ERROR -1

typedef unsigned char BYTE;

void PrintHelp();

bool StringsEqual(const char* stringOne, const char* stringTwo);

FILE* FileOpenForWriting(const char* path);
FILE* FileOpenForReading(const char* path);
int FileClose(FILE* file);

int FileWrite(FILE* file, void* dataPointer, int numberOfBytesToWrite);
int FileWriteByte(FILE* file, int byteValue);
int FileWriteInt(FILE* file, int value);
int FileWritePascalString(FILE* file, const char* cstring);

int FileRead(FILE* file, void* dataPointer, int numberOfBytesToRead);
int FileReadByte(FILE* file);
int FileReadInt(FILE* file);
char* FileReadPascalString(FILE* file);

int PackDirectory(const char* sourceDir, const char* destFile);
int PackDirectoryRecursively(
	const char* baseSourceDir,
	const char* sourceDir,
	FILE* packedFile);
int PackFile(
	const char* fullPath,
	const char* localPath,
	int fileSize,
	FILE* packedFile);
int UnpackDirectory(const char* sourceFile, const char* destDir);

void PrintHelp()
{
	printf("mopack usage:\n");
	printf("  Syntax:\n");
	printf("    mopack pack <sourcedir> <destfile>\n");
	printf("    mopack unpack <sourcefile> <destdir>\n");
	printf("  Example:\n");
	printf("    mopack pack /files c/packed/files.bin\n");
	printf("    mopack unpack /packed/files.bin /files\n");
}

bool StringsEqual(const char* stringOne, const char* stringTwo)
{
	return 0 == strcmp(stringOne, stringTwo);
}

FILE* FileOpenForWriting(const char* path)
{
	return fopen(path, "wb");
}

FILE* FileOpenForReading(const char* path)
{
	return fopen(path, "rb");
}

int FileClose(FILE* file)
{
	return fclose(file);
}

/**
 * @return Number of bytes written.
 */
int FileWrite(FILE* file, void* dataPointer, int numberOfBytesToWrite)
{
	return (int) fwrite(dataPointer, 1, numberOfBytesToWrite, file);
}

/**
 * @return SUCCESS or ERROR.
 */
int FileWriteByte(FILE* file, int byteValue)
{
	BYTE b = byteValue;

	int result = FileWrite(file, &b, 1);

	if (1 != result)
	{
		return ERROR;
	}

	return SUCCESS;
}

/**
 * @return SUCCESS or ERROR
 */
int FileWriteInt(FILE* file, int value)
{
	BYTE b[4];

	b[0] = (value) & 0xFF;
	b[1] = (value >> 8) & 0xFF;
	b[2] = (value >> 16) & 0xFF;
	b[3] = (value >> 24) & 0xFF;

	int result =  FileWrite(file, b, 4);

	if (4 != result)
	{
		return ERROR;
	}

	return SUCCESS;
}

/**
 * @return SUCCESS or ERROR
 */
int FileWritePascalString(FILE* file, const char* cstring)
{
	int length = strlen(cstring);

	int result = FileWriteInt(file, length);
	if (SUCCESS != result)
	{
		return ERROR;
	}

	result = FileWrite(file, (void*)cstring, length);
	if (length != result)
	{
		return ERROR;
	}

	return SUCCESS;
}

/**
 * @return Number of bytes read.
 */
int FileRead(FILE* file, void* dataPointer, int numberOfBytesToRead)
{
	return (int) fread(dataPointer, 1, numberOfBytesToRead, file);
}

/**
 * @return Byte value 0 - 255 on success, ERROR on error.
 */
int FileReadByte(FILE* file)
{
	BYTE b;

	int result = FileRead(file, &b, 1);
	if (1 != result)
	{
		return ERROR;
	}

	return b;
}

/**
 * Returns a positive integer value.
 * @return 0 - max value on success, ERROR on error.
 */
int FileReadInt(FILE* file)
{
	BYTE b[4];

	int result = FileRead(file, b, 4);

	if (4 != result)
	{
		return ERROR;
	}

	return
		(b[0]) |
		(b[1] << 8) |
		(b[2] << 16) |
		(b[3] << 24);
}

/**
 * @return Allocated string or NULL. Caller has responsibility
 * for deallocating the returned string with free.
 */
char* FileReadPascalString(FILE* file)
{
	int length = FileReadInt(file);
	if (ERROR == length)
	{
		return NULL;
	}

	char* buf = (char*) malloc(length + 1);
	int result = FileRead(file, buf, length);
	if (length != result)
	{
		free(buf);
		return NULL;
	}

	buf[length] = 0;

	return buf;
}

/**
 * Pack directory tree into destFile.
 * @return SUCCESS or ERROR.
 */
int PackDirectory(const char* sourceDir, const char* destFile)
{
	// Open destination file.
	FILE* file = FileOpenForWriting(destFile);
	if (NULL == file)
	{
		return ERROR;
	}

	// Write magic file header.
	FileWriteByte(file, 1);
	FileWriteByte(file, 2);
	FileWriteByte(file, 3);

	// Pack directory and subdirectories into file.
	int result = PackDirectoryRecursively(sourceDir, sourceDir, file);
	FileClose(file);
	return result;
}

int PackDirectoryRecursively(
	const char* baseSourceDir,
	const char* sourceDir,
	FILE* packedFile)
{
	// Open source directory.
	DIR* directory = opendir(sourceDir);
	if (NULL == directory)
	{
		return ERROR;
	}

	// Traverse files.
	struct dirent* entry;
	while (NULL != (entry = readdir(directory)))
	{
		// Filter out current directory and parent directory.
		if (StringsEqual(".", entry->d_name) ||
			StringsEqual("..", entry->d_name))
		{
			continue;
		}

		// Obtain file info.
		char path[1024];
		sprintf(path, "%s/%s", sourceDir, entry->d_name);
		printf("path: %s\n", path);
		struct stat status;
		int result = stat(path, &status);
		if (0 != result)
		{
			return ERROR;
		}

		if (S_ISDIR(status.st_mode))
		{
			// If subdirectory then recurse.
			printf("Decend\n");
			result = PackDirectoryRecursively(baseSourceDir, path, packedFile);
			printf("Acend\n");
			if (ERROR == result)
			{
				return ERROR;
			}
		}
		else if (S_ISREG(status.st_mode))
		{
			// If regular file then write to packedFile.
			char* localPath = path + strlen(baseSourceDir);
			printf("Packing:\n");
			printf("  path: %s\n", path);
			printf("  localPath: %s\n", localPath);
			result = PackFile(
				path,
				localPath,
				status.st_size,
				packedFile);
			if (ERROR == result)
			{
				return ERROR;
			}
		}
	}

	closedir(directory);

	return SUCCESS;
}

int PackFile(
	const char* fullPath,
	const char* localPath,
	int fileSize,
	FILE* packedFile)
{
	// Write byte code for file data.
	FileWriteByte(packedFile, 1);

	// Write path.
	FileWritePascalString(packedFile, localPath);

	// Write file size.
	FileWriteInt(packedFile, fileSize);

	// Read file content.
	FILE* file = FileOpenForReading(fullPath);
	if (NULL == file)
	{
		return ERROR;
	}

	int byte;
	while (ERROR != (byte = FileReadByte(file)))
	{
		FileWriteByte(packedFile, byte);
	}

	FileClose(file);

	return SUCCESS;
}

/**
 * Unpack destFile to a directory.
 * @return SUCCESS or ERROR.
 */
int UnpackDirectory(const char* sourceFile, const char* destDir)
{
	// Read sourceFile.
	FILE* file = FileOpenForReading(sourceFile);
	if (NULL == file)
	{
		return ERROR;
	}

	// Read magic header.
	printf("Magic Header: %i\n", FileReadByte(file));
	printf("Magic Header: %i\n", FileReadByte(file));
	printf("Magic Header: %i\n", FileReadByte(file));

	// Read magic header.
	// printf(
		// "Magic Header: %i %i %i\n",
		// FileReadByte(file),
		// FileReadByte(file),
		// FileReadByte(file));

	// Unpack files.
	int byteCode;
	while (ERROR != (byteCode = FileReadByte(file)))
	{
		// Check that we have the correct byte code.
		printf("ByteCode: %i\n", byteCode);
		if (1 != byteCode)
		{
			return ERROR;
		}

		// Read file data.
		char* path = FileReadPascalString(file);
		if (NULL == path)
		{
			return ERROR;
		}

		printf("path: %s\n", path);

		// Read file length.
		int length = FileReadInt(file);
		if (ERROR == length)
		{
			return ERROR;
		}

		printf("File length: %i\n", length);

		int counter = length;
		while (counter)
		{
			FileReadByte(file);
			--counter;
		}
	}

	FileClose(file);

	return SUCCESS;
}

int main(int argc, char* argv[])
{
	int result = ERROR;

	printf("argc: %i\n", argc);
	printf("arg 1: %s\n", argv[1]);

	// Check number of command parameters.
	if (argc != 4)
	{
		PrintHelp();
		return EXIT_FAILURE;
	}
	// Check sub-command.
	else if (StringsEqual("pack", argv[1]))
	{
		result = PackDirectory(argv[2], argv[3]);
	}
	else if (StringsEqual("unpack", argv[1]))
	{
		result = UnpackDirectory(argv[2], argv[3]);
	}
	else
	{
		PrintHelp();
	}

	// Return success value.
	if (SUCCESS == result)
	{
		return EXIT_SUCCESS;
	}
	else
	{
		return EXIT_FAILURE;
	}
}
