/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/*
 * @file LOG.h
 * @author Gabriela Rata
 */

#ifndef LOG_H_
#define LOG_H_

#include <string>
#include <bitset>
#include <vector>
#include <deque>
#include <MAUtil/util.h>
#include <mavsprintf.h>

#include "Employee.h"

//#define LOG printf
#define LOG lprintfln

#define TRACE(ARG) lprintfln(#ARG); ARG

template<unsigned int numBits>
void print(const std::bitset<numBits> &b, const std::string &msg)
{
	LOG("\n");
	LOG(msg.c_str());
	for(int i=0; i<b.size(); ++i)
	{
		LOG("%d", b[i]);
	}
	LOG("\n");
}

void log_to_console(int *array, int arraySize, const std::string &msg);
void log_to_console(char *array, int arraySize, const std::string &msg);
void log_to_console(std::string *array, int arraySize, const std::string &msg);
void log_to_console(const std::vector<int> &v, const std::string &msg);
void log_to_console(const std::vector<char> &v, const std::string &msg);
void log_to_console(const std::vector<Employee> &v, const std::string &msg);
void log_to_console(Employee *array, int arraySize, const std::string &msg);
void log_to_console(const std::deque<char> &d, const std::string &msg);

void log_to_console(const char* val, const std::string &msg="");
void log_to_console(int i, const std::string &msg="");
void log_to_console(float i, const std::string &msg="");
void log_to_console(char c, const std::string &msg="");
void log_to_console(bool b, const std::string &msg="");
void log_to_console(const Employee &e, const std::string &msg = "");

const std::string toString(int i);

#endif /* LOG_H_ */
