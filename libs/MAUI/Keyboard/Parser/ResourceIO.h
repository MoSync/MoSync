/**
 * @brief A class that abstracts resource IO.
 * @file ResourceIO.h
 * @author Mattias Frånberg
 */

#ifndef FILE_H_
#define FILE_H_

#include <maapi.h>

namespace MAUI {

/**
 * Convinience class for handling resource IO, whose purpose
 * is to avoid having to check for bounds, this class will report
 * when the end of resource has been reached.
 */
class ResourceIO
{
public:
	/**
	 * Opens the specified resource.
	 *
	 * @param res The resource file that will be opened.
	 */
	ResourceIO(MAHandle res);

	/**
	 * Reads from a resource into a buffer.
	 *
	 * @param buffer The buffer where the read data will be stored, it
	 * 				 is assumed that buffer can contain at least nbytes.
	 * @param nbytes The number of bytes to read from the resource.
	 *
	 */
	unsigned int read(char *buffer, unsigned int nbytes);

	/**
	 * XXX: Not implemented.
	 *
	 * Writes a buffer to a resource.
	 *
	 * @param buffer The buffer that will be written to the resource.
	 * @param nbytes The number of bytes that will be written, it
	 * 				 is assumed that buffer is at least nbytes long.
	 * @return The number of bytes that was actually written.
	 */
	unsigned int write(const char *buffer, unsigned int nbytes);

	/**
	 * Indicates whether end of resource has been reached.
	 *
	 * @return true if end of resource has been reached, otherwise false.
	 */
	bool eof();

	/**
	 * Closes the resource.
	 */
	~ResourceIO();

private:
	/* Total file size in bytes. */
	unsigned int m_size;

	/* Current file position. */
	unsigned int m_pos;

	/* The resource. */
	MAHandle m_res;
};

} /* Namespace end */

#endif /* FILE_H_ */
