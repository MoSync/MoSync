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

#include "File.h"
#include <string>
#include <list>
#include "filelist/filelist.h"


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

	size_t i = m_path.find_last_of( F_SEPERATOR );
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

	g_fileList = &l;

// FIX (niklas): I don't know why this should work in another way?
//#ifndef WIN32
//	g_currentPath = "";
//#else
	g_currentPath = m_path + F_SEPERATOR;
//#endif
	scanDirectory( (m_path + "/*").c_str( ), callback );
	g_fileList = NULL;

	return l;
}

String getExtension(String filename) {
	size_t ix = filename.find_last_of('.');
	if (ix != String::npos) {
		return filename.substr(ix + 1, filename.length() - ix - 1);
	}
	return String();
}
