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

/** Copyright (c) Mobile Sorcery AB 2005-2006 **/

#ifndef _SE_MSAB_FILE_H_
#define _SE_MSAB_FILE_H_

#include <list>
#include <string>
#include <cstdio>

#ifdef WIN32
#define F_SEPERATOR	'\\'
#else
#define F_SEPERATOR	'/'
#endif

typedef std::string String;

/**
 * This class is used to traverse through
 * a directory tree in a platform independent
 * way.
 *
 */
class File
{
public:
	/**
	 * Constructor
	 *
	 * @param p Path to file/directory
	 */
	File( const String &p );

	/**
	 * Copy constructor
	 *
	 * @param f const reference to File to copy
	 */
	File( const File & f );

	/**
	 * Returns if this is a directory or not
	 *
	 * @return true/false
	 */
	bool isDirectory ( void );

	/**
	 * Returns the file name
	 * @return File name without full path or empty
	 *         string if not a file.
	 */
	String getName ( void );

	/**
	 * Returns the full path of the file.
	 *
	 * @return Full path as string
	 */
	String getAbsolutePath ( void );

	/**
	 * Checks if it is a 'self' or 'back' reference,
	 * i.e '/.'
	 *
	 * @return true/false
	 */
	bool isSelfOrBackRef ( void )
	{
		static const char backRef[4] = { '.', '.', F_SEPERATOR, '\0' };
		return m_path[m_path.length( )-1] == '.' ||
			m_path.find( backRef ) != String::npos;
	}

	/**
	 * Returns a linked list of the files in
	 * this directory. If this file is not a
	 * directory, the list will be empty
	 *
	 * @return Linked list with files.
	 */
	std::list<File> listFiles ( void );

private:
	String	m_path;
};

#endif //_SE_MSAB_FILE_H_
