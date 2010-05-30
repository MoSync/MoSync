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

#include "File.h"
#include <list>
#include <filelist/filelist.h>


/**
 * Callback used for filelist library.
 *
 */
static String g_currentPath;
static std::list<File> *g_fileList;
static void callback ( const char *f )
{
	g_fileList->push_back( File( g_currentPath+f ) );
}

/**
 * Constructor
 *
 * @param p Path to file/directory
 */
File::File ( const String &p )
: m_path( p )
{ }


/**
 * Copy constructor
 *
 * @param f const reference to File to copy
 */
File::File ( const File & f )
: m_path( f.m_path )
{ }


/**
 * Returns if this is a directory or not
 *
 * @return true/false
 */
bool File::isDirectory ( void ) 
{
	return ::isDirectory( m_path.c_str( ) ) == 1;
}

/**
 * Returns the file name
 
 * @return File name without full path or empty
 *         string if not a file.
 */
String File::getName ( void ) 
{
	String temp = "";
	
	unsigned int i = m_path.find_last_of( F_SEPERATOR );
	if ( i != String::npos ) 
		temp = String( &m_path[i+1] );

	return temp;
}

/**
 * Returns the full path of the file.
 *
 * @return Full path as string
 */
String File::getAbsolutePath ( void ) 
{
	return m_path;
}

/**
 * Returns a linked list of the files in 
 * this directory. If this file is not a
 * directory, the list will be empty
 *
 * @return Linked list with files.
 */
std::list<File> File::listFiles ( void )
{
	std::list<File> l;

	// Can only list files for directories	
	if ( isDirectory( ) == false )
		return l;
	
	// Hack - uses global
	g_fileList = &l;
	//g_currentPath = "";
	//if ( m_path.length( ) > 0 && m_path != "."  )
		g_currentPath = m_path + F_SEPERATOR;
	scanDirectory( (m_path + "/*").c_str( ), callback );
	g_fileList = NULL;

	return l;
}