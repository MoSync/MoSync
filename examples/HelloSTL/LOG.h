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

#include <algorithm>
#include <string>
#include <bitset>
#include <vector>
#include <set>
#include <deque>
#include <list>
#include <map>
#include <MAUtil/util.h>
#include <mavsprintf.h>

#include "Employee.h"

//#define LOG printf
#define LOG lprintfln

#define TRACE(ARG) lprintfln(#ARG); ARG

template<unsigned int numBits>
void log_to_console(const std::bitset<numBits> &b, const std::string &msg)
{
	std::string bits;
	for(size_t i=0; i<b.size(); ++i)
	{
		char bit = '0';
		if(b[i] == 1)
		{
			bit = '1';
		}
		bits.append(1, bit);
		bits += " ";
	}

	std::string final_msg = msg + bits;
	LOG(final_msg.c_str());
}

void log_to_console(int *array, int arraySize, const std::string &msg);
void log_to_console(char *array, int arraySize, const std::string &msg);
void log_to_console(std::string *array, int arraySize, const std::string &msg);
void log_to_console(const std::vector<int> &v, const std::string &msg);
void log_to_console(const std::list<int> &l, const std::string &msg);
void log_to_console(const std::set<int> &s, const std::string &msg);
void log_to_console(const std::multiset<int> &ms, const std::string &msg);
void log_to_console(const std::deque<int> &d, const std::string &msg);
void log_to_console(const std::vector<char> &v, const std::string &msg);
void log_to_console(const std::vector<Employee> &v, const std::string &msg);
void log_to_console(const std::set<Employee, LessExperienceFunctor> &s, const std::string &msg);
void log_to_console(const std::multiset<Employee, LessExperienceFunctor> &ms, const std::string &msg);
void log_to_console(Employee *array, int arraySize, const std::string &msg);
void log_to_console(const std::deque<char> &d, const std::string &msg);

void log_to_console(const std::map<int, Employee> &m, const std::string &msg);
void log_to_console(const std::map<int, Employee, std::greater<int> > &m, const std::string &msg);

void log_to_console(const char* val, const std::string &msg="");
void log_to_console(int i, const std::string &msg="");
void log_to_console(float i, const std::string &msg="");
void log_to_console(unsigned long u, const std::string &msg = "");
void log_to_console(char c, const std::string &msg="");
void log_to_console(bool b, const std::string &msg="");
void log_to_console(const Employee &e, const std::string &msg = "");
void log_to_console(const std::pair<int, Employee> &p, const std::string &msg);

const std::string toString(int i);


#endif /* LOG_H_ */
