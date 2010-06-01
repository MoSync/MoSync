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
