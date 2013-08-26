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

#define LOG printf
//#define LOG lprintfln

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
