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
 * @file LOG.cpp
 * @author Gabriela Rata
 */

#include <maxtoa.h>
#include "LOG.h"

const std::string toString(int i)
{
	char buf[10];
	itoa( i, buf, 10);
	return std::string(buf);
}

void log_to_console(int *array, int arraySize, const std::string &msg)
{
	std::string final_msg = msg;
	for(int i=0; i<arraySize; ++i)
	{
		char buf[10];
		itoa( array[i], buf, 10);
		std::string str(buf);
		final_msg += str + " ";
	}
	LOG(final_msg.c_str());
}

void log_to_console(char *array, int arraySize, const std::string &msg)
{
	std::string final_msg = msg;
	for( int i=0; i< arraySize; i++)
	{
		final_msg.append(1, array[i]);
		final_msg += " ";
	}
	LOG(final_msg.c_str());
}

void log_to_console(std::string *array, int arraySize, const std::string &msg)
{
	std::string final_msg = msg;
	for(int i=0; i< arraySize; ++i)
	{
		final_msg += array[i];
		final_msg += " ";
	}
	LOG(final_msg.c_str());
}

void log_to_console(const std::vector<int> &v, const std::string &msg)
{
	std::string final_msg = msg;
	for(int i=0; i < v.size(); ++i)
	{
		char buf[10];
		itoa( v[i], buf, 10);
		std::string str(buf);
		final_msg += str + " ";
	}
	LOG(final_msg.c_str());
}

void log_to_console(const std::list<int> &l, const std::string &msg)
{
	std::string final_msg = msg;

	for(std::list<int>::const_iterator it = l.begin(); it!=l.end(); ++it)
	{
		char buf[10];
		itoa( *it, buf, 10);
		std::string str(buf);
		final_msg += str + " ";
	}
	LOG(final_msg.c_str());
}
void log_to_console(const std::set<int> &s, const std::string &msg)
{
	std::string final_msg = msg;
	for(std::set<int>::const_iterator it = s.begin(); it!=s.end(); ++it)
	{
		char buf[10];
		itoa( *it, buf, 10);
		std::string str(buf);
		final_msg += str + " ";
	}
	LOG(final_msg.c_str());
}

void log_to_console(const std::vector<char> &v, const std::string &msg)
{
	std::string final_msg = msg;
	for(int i=0; i < v.size(); ++i)
	{
		final_msg.append(1, v[i]);
		final_msg += " ";
	}
	LOG(final_msg.c_str());
}

void log_to_console(const std::vector<Employee> &v, const std::string &msg)
{
	std::string final_msg = msg;
	for(int i=0; i<v.size(); ++i)
	{
		final_msg += v[i].getName();
		final_msg += "( exp: ";
		final_msg += toString(v[i].getExperience());
		//final_msg += ", salary: ";
		//final_msg += toString(v[i].getSalary());
		final_msg += ") ";
		final_msg += " ";
	}
	LOG(final_msg.c_str());
}

void log_to_console(const std::set<Employee, LessExperienceFunctor> &s, const std::string &msg)
{
	std::string final_msg = msg;
	for(std::set<Employee>::const_iterator it = s.begin(); it!=s.end(); ++it)
	{
		final_msg += it->getName();
		final_msg += "( exp: ";
		final_msg += toString(it->getExperience());
		//final_msg += ", salary: ";
		//final_msg += toString(v[i].getSalary());
		final_msg += ") ";
		final_msg += " ";
	}
	LOG(final_msg.c_str());
}

void log_to_console(Employee *array, int arraySize, const std::string &msg)
{
	std::string final_msg = msg;
	for(int i=0; i < arraySize; ++i)
	{
		final_msg += array[i].getName();
		final_msg += " ";
	}
	LOG(final_msg.c_str());
}

void log_to_console(const std::deque<char> &d, const std::string &msg)
{
	std::string final_msg = msg;
	for(int i=0; i < d.size(); ++i)
	{
		final_msg.append(1, d[i]);
		final_msg += " ";
	}
	LOG(final_msg.c_str());
}

void log_to_console(const char* val, const std::string &msg)
{
	if(msg.size()>0)
	{
		LOG("%s %s ", msg.c_str(), val);
	}
	else
	{
		LOG("%s ", val);
	}
}

void log_to_console(int i, const std::string &msg)
{
	if(msg.size()>0)
	{
		LOG("%s %d ", msg.c_str(), i);
	}
	else
	{
		LOG("%d ", i);
	}
}

void log_to_console(float i, const std::string &msg)
{
	if(msg.size()>0)
	{
		LOG("%s %f ", msg.c_str(), i);
	}
	else
	{
		LOG("%f ", i);
	}
}

void log_to_console(unsigned long u, const std::string &msg )
{
	if(msg.size()>0)
	{
		LOG("%s %l ", msg.c_str(), u);
	}
	else
	{
		LOG("%l ", u);
	}
}

void log_to_console(char c, const std::string &msg)
{
	if(msg.size()>0)
	{
		LOG("%s %c ", msg.c_str(), c);
	}
	else
	{
		LOG("%c ", c);
	}
}

void log_to_console(bool b, const std::string &msg)
{
	std::string message;
	if(msg.size()>0)
	{
		message += msg;

	}
	if(b)
	{
		message += "true ";
	}
	else
	{
		message += "false ";
	}
	LOG(message.c_str());
}

void log_to_console(const Employee &e, const std::string &msg)
{
	std::string final_msg = msg;
	final_msg += e.getName();
	LOG(final_msg.c_str());
}
