/**
 * @file TestStack.cpp
 * @author Gabriela Rata
 */

#include <stack>
#include <vector>
#include "../STLMoblet.h"
#include "../LOG.h"

void STLMoblet::STL_stack()
{
	LOG("\n");
	LOG("========================= stack =================================================================================\n");
	LOG("/**");
	LOG(" *  stack is a container that operates as in a LIFO (last in first out) mode.");
	LOG(" *  The elements are inserted and extracted only from the end of the container.");
	LOG(" *  std::stack is implemented as a container adaptor. Containers adapters are classes that use an");
	LOG(" *  encapsulated container and provide a restricted interface to that container.");
	LOG(" *  The underlying container can be a STL container or some other  container type. It has to provide the");
	LOG(" *  following public member functions: back(), push_back(), pop_back().");
	LOG(" *  stack is defined as a template taking two parameters:");
	LOG(" *  		template < 	class T,");
	LOG(" *  				class Container = deque<T>");
	LOG(" *  				>");
	LOG(" *  				class stack;");
	LOG(" * 		The first parameter is the type of elements in will contain.");
	LOG(" * 		The second parameter is the underlying container, and is by default std::deque.");
	LOG(" * std::stack is defined in the <stack> header.");
	LOG(" */");

	using std::stack;

	LOG("\n");
	LOG("/**");
	LOG(" * default constructor");
	LOG(" * Constructed with a std::deque as the underlying container. That means that it will forward all");
	LOG(" * the function calls to it's deque.");
	LOG(" */");
	TRACE(stack<int> s;);

	LOG("\n");
	LOG("/**");
	LOG("* empty function: returns true if the stack contains no elements.");
	LOG("* It calls the member function deque::empty");
	LOG(" */");
	bool e = s.empty();
	log_to_console(e, "bool e = s.empty(); 	//e = ");

	LOG("\n");
	LOG("/**");
	LOG(" * push: adds an element at the end of the underlying deque. Calls deque::push_back.");
	LOG(" */");

	TRACE(s.push(10););

	LOG("\n");
	LOG("/**");
	LOG(" * constructor");
	LOG(" * s1 is constructed with a vector as its underlying container. That means that it will forward all");
	LOG(" * the function calls to this vector.");
	LOG(" */");

	TRACE(std::vector<int> underlyingContainer;);
	stack<int, std::vector<int> > s1(underlyingContainer);
	LOG("stack<int, std::vector<int> > s1(underlyingContainer);");


	LOG("\n");
	LOG("/**");
	LOG(" * size: returns the number of elements from the underlying vector (calls vector::size()).");
	LOG(" */");

	int sz = s.size();
	log_to_console(sz, "int sz = s.size(); 	//sz = ");

	LOG("\n");
	LOG("/**");
	LOG(" * top function: returns the last element added to the stack.");
	LOG(" * In the example bellow calls vector::back().");
	LOG(" */");

	TRACE(s1.push(32););
	int lastElement = s1.top();
	log_to_console(lastElement, "int lastElement = s1.top(); 	//lastElement = ");

	LOG("\n");
	LOG("/**");
	LOG(" * pop function: Removes the element on top of the stack.");
	LOG(" * In the example bellow, it will call vector::pop_back;");
	LOG(" */");

	TRACE(s1.pop(););
	e = s1.empty();
	log_to_console(e, "e = s1.empty(); 	//e = ");

	LOG("\n");
}
