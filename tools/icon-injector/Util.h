/* Copyright (C) 2012 MoSync AB

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

#ifndef UTIL_H
#define UTIL_H

#include <map>
#include <set>
#include <vector>
#include <string>
#include <ios>
#include <ostream>

// convert all \ to /
void toSlashes(std::string&);

// convert all / to '\'
void toBackSlashes(std::string&);

// Makes sure a file name properly slashed
std::string file(const char* arg);
std::string file(std::string arg);

// Makes sure arguments with spaces are quoted
std::string arg(const char* arg);
std::string arg(std::string arg);

#endif	//UTIL_H
