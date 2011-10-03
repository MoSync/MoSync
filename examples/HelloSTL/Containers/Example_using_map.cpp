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

/**
 * @file Example_using_map.cpp
 * @author Gabriela Rata
 */

#include <string>
#include <map>
#include <assert.h>

#include "../STLMoblet.h"
#include "../LOG.h"
#include "../Employee.h"


void STLMoblet::STL_map()
{
	LOG("\n");
	LOG("========================= map ===================================================================================\n");
	LOG("/**");
	LOG(" * map - is an associative container. That means that the index doesn't have to be an integer.");
	LOG(" * An entry in the map is a combination of key - value.");
	LOG(" * std::map manages key/value std::pairs as elements ( pair<key_type, value_type>).");
	LOG(" * The key is used to identify an element in the map ( the index ). So you can write for example myMap[key] = value;");
	LOG(" * similar to vector: myVector[0] = 6; ( or x = myVector[2]; myValue = myMap[key]; ).");
	LOG(" * std::map sorts its elements automatically by key ( from lower to higher ). The default comparison criterion");
	LOG(" * is std::less, and it's provided as a default template parameter like this:");
	LOG(" * 		 	template < 	class Key ,");
	LOG(" * 					class T ,");
	LOG(" * 				    	class Cmp = less <Key >,");
	LOG(" * 					....");
	LOG(" * 				  	> class map;");
	LOG(" * Another ordering criterion, than the default one, can be provided as the third template parameter. It has");
	LOG(" * to be a class that defines an operator(), that takes two arguments of the key type and returns a bool. It can");
	LOG(" * be also just a function (with the same prototype as operator()). (See example bellow).");
	LOG(" * std::map allows only unique entries. That means that you can't have in a map two entries");
	LOG(" * with the same key. If you try to insert in map a entry with a certain key,");
	LOG(" * and inside the map exits already an element with that key, the element will be overridden.");
	LOG(" * std::map provides bidirectional iterators.");
	LOG(" * std::map is designed to be efficient in accessing its elements by their key.");
	LOG(" * std::map is defined in the <map> header.");
	LOG(" */");

	using std::map;

	LOG("\n");
	LOG("/**");
	LOG(" * default constructor");
	LOG(" */");

	std::map<int, Employee> employees;
	LOG("std::map<int, Employee> employees;");

	LOG("\n");
	LOG("/**");
	LOG(" * empty - returns true if the map is empty");
	LOG(" */");

	bool e = employees.empty();
	log_to_console(e, "bool e = employees.empty(); 	//e = ");


	LOG("\n");
	LOG("/**");
	LOG(" * operator[] - returns the value that corresponds to the key (supplied as a parameter to operator[])");
	LOG(" * If you are trying to find an object by passing in a key to operator[ ] and that object doesn’t exist");
	LOG(" * operator[ ] will automatically insert a new key-value pair into the map (using the default constructor).");
	LOG(" for the value object.");
	LOG(" */");
	LOG("\n");

	TRACE(Employee senior(7, "Alex"););
	TRACE(Employee middle(4, "Jim"););
	TRACE(Employee junior(1, "Ted"););

	LOG("\n");
	TRACE(employees[senior.getSalary()] = senior;);
	TRACE(employees[middle.getSalary()] = middle;);
	TRACE(employees[junior.getSalary()] = junior;);

	LOG("\n");
	log_to_console(employees, "//employees contain now: ");

	LOG("\n");
	LOG("/**");
	LOG(" * begin: returns an iterator referring to the first element of the map. When dereferenced produces a ");
	LOG(" * pair object containing both the key and the value. (see Pair.cpp)");
	LOG(" * If the map is empty, it will return the one past-the-end element in the map,");
	LOG(" * like the map::end() function. ");
	LOG(" */");

	map<int, Employee>::iterator itBegin = employees.begin();
	log_to_console(*itBegin, "map<int, Employee>::iterator itBegin = employees.begin(); 	//*itBegin = ");

	LOG("\n");
	LOG("/**");
	LOG(" * end: returns an iterator referring to the one past the end element in the map");
	LOG(" * We must not deference the iterator returned by end(). It is used to see is if we reached the");
	LOG(" * end of the map, when we iterate through it.");
	LOG(" */");

	map<int, Employee>::iterator itEnd = employees.end();
	LOG("map<std::string, int>::iterator itEnd = employees.end();");


	LOG("\n");
	LOG("//We want to move through the entire map and increase the experience value with 1 for each employee.");
	LOG("//For that we use an iterator, which when dereferenced, produces a pair object  containing");
	LOG("// both the key and the value.");
	LOG("\n");

	for(map<int, Employee>::iterator it = itBegin; it!=itEnd; ++it)
	{
		it->second.setExperience(it->second.getExperience() + 1);
	}

	LOG("map<int, Employee>::iterator it = itBegin; it!=itEnd; ++it)");
	LOG("{");
	LOG("	it->second.setExperience(it->second.getExperience() + 1);");
	LOG("}");

	LOG("\n");
	LOG("/**");
	LOG(" * rbegin: returns an reverse iterator, referring to the last element of the map. When dereferenced produces a ");
	LOG(" * pair object containing both the key and the value. (see Pair.cpp)");
	LOG(" * If the map is empty, returns a reverse iterator referring to the element right before the first element ");
	LOG(" * (like the map::rend() function.)");
	LOG(" */");

	map<int, Employee>::reverse_iterator itRevBegin = employees.rbegin();
	log_to_console(*itRevBegin, "map<int, Employee>::reverse_iterator itRevBegin = "
			"employees.rbegin(); 	//*itRevBegin = ");

	LOG("\n");
	LOG("/**");
	LOG(" * rend: returns a reverse iterator referring to the element right before the first element");
	LOG(" * We must not deference the iterator returned by rend(). It is used to see is if we reached the");
	LOG(" * end of the map, when we do a reverse iteration.");
	LOG(" */");

	LOG("\n");
	map<int, Employee>::reverse_iterator itRevEnd = employees.rend();
	LOG("map<int, Employee>::reverse_iterator itRevEnd = employees.rend();");

	LOG("\n");
	log_to_console(employees, "//employees map contains: ");
	LOG("//Iterate through map in a reverse order and list all the employees: ");
	LOG("\n");

	LOG("for(map<int, Employee>::reverse_iterator it = itRevBegin; it!=itRevEnd; ++it)");
	LOG("{");
	LOG("	log_to_console(it);");
	LOG("}");

	LOG("\n");
	LOG("//produces: ");

	for(map<int, Employee>::reverse_iterator it = itRevBegin; it!=itRevEnd; ++it)
	{
		log_to_console(it->second);
	}

	LOG("\n");
	LOG("/**");
	LOG(" * constructor");
	LOG(" * map will sort keys with \"std::greater<int>\"");
	LOG(" */");

	LOG("\n");
	map<int, Employee, std::greater<int> > staff;
	LOG("map<int, Employee, std::greater<int> > staff;");

	TRACE(staff[senior.getSalary()] = senior;);
	TRACE(staff[middle.getSalary()] = middle;);
	TRACE(staff[junior.getSalary()] = junior;);

	LOG("\n");
	log_to_console(staff, "//staff contains now: ");

	LOG("\n");
	LOG("/**");
	LOG(" * erase(elem) - removes an element (key-value pair) from the map");
	LOG(" * erase(beg, end) - removes all the elements of the [beg, end)");
	LOG(" */");

	log_to_console(employees, "//employees map contains: ");
	TRACE(itBegin = employees.begin(););
	employees.erase(itBegin);
	log_to_console(employees, "employees.erase(itBegin);	//employees will contain: ");

	LOG("\n");
	LOG("//erase all the elements from the map:");
	TRACE(itBegin = employees.begin(););
	TRACE(employees.erase(itBegin, employees.end()););
	e = employees.empty();
	log_to_console(e, "e = employees.empty();	//e = ");

	LOG("\n");
	LOG("/**");
	LOG(" * insert(elem) - adds a key-value pair to the map");
	LOG(" * insert(beg, end) - inserts a copy of all the elements in the range [beg, end)");
	LOG(" * value_type, is provided as a type definition inside map:  typedef pair<key, value> value_type;");
	LOG(" */");

	LOG("\n");
	TRACE(Employee victor(20, "Victor"););
	employees.insert(map<int, Employee>::value_type(victor.getSalary(), victor));
	log_to_console(employees, "employees.insert(victor.getSalary(), victor); 	//employees will contain: ");

	LOG("\n");
	LOG("//Add all the elements from the \"staff\" map:");
	employees.insert(staff.begin(), staff.end());
	log_to_console(employees, "employees.insert(staff.begin(), staff.end()); 	//employees will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * copy constructor");
	LOG(" */");

	map<int, Employee> employees_copy(employees);
	log_to_console(employees_copy, "map<int, Employee> employees_copy(employees); 	//employees_copy will contain: ");

	LOG("\n");
	LOG("/**");
	LOG(" * map overloads the following operators: ==, !=, <, >, <=, >=");
	LOG(" */");
	TRACE(assert(employees == employees_copy););

	LOG("\n");
	LOG("/**");
	LOG(" * operator=");
	LOG(" */");

	map<int, Employee> emptyMap;
	LOG("map<int, Employee> emptyMap;");

	TRACE(employees = emptyMap;);
	e = employees.empty();

	log_to_console(e, "e = employees.empty(); 	//e = ");

	LOG("\n");
	LOG("/**");
	LOG(" * swap - swaps the data between two maps");
	LOG(" * m1.spap(m2) -> swaps the data of m1 and m2 ");
	LOG(" */");

	std::map<int, Employee, std::greater<int> > newStaff;
	TRACE(newStaff.swap(staff););
	log_to_console(newStaff, "//newStaff contains now: ");
	log_to_console((bool)staff.empty(), "//staff.empty() returns now: ");

	LOG("\n");
	LOG("/**");
	LOG(" * find(key) - searches an element by key, and returns an iterator pointing to the element.");
	LOG(" * If it doesn't find anything returns an iterator to the element past the end of the map (same as map::end)");
	LOG(" */");

	map<int, Employee, std::greater<int> >::iterator found = newStaff.find(400);
	log_to_console(*found, "map<int, Employee, std::greater<int> >::iterator found = newStaff.find(junior); 	//*found = ");

	LOG("\n");
	found = newStaff.find(0);
	TRACE(assert(found == newStaff.end()););

}
