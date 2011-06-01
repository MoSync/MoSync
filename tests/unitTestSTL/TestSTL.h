/*
 * TestSTL.h
 *
 *  Created on: May 27, 2011
 *      Author: gabi
 */

#ifndef TESTSTL_H_
#define TESTSTL_H_

#include <testify/testify.hpp>

class TestSTL: public Testify::TestCase
{
public:
	TestSTL();

	/**
	 * Function for testing std::pair
	 * The pair structure is provided
	 * to treat two values as a single unit
	 * The pair structure is defined in <utility> header of STL
	 */
	void testPair();

	/**
	 * Function for testing std::auto_ptr
	 * auto_ptr is a smart pointer that takes ownership of the pointer it stores.
	 * When the auto_ptr object is destroyed, the pointer will be deleted.
	 */
	void test_auto_ptr();

	/**
	 * Function for testing function templates min and max
	 * If both values are equal, usually the first element is returned.
	 * Both functions are provided with an additional argument, that is the comparison criterion
	 * Those functions are defined in <algorithm>
	 */
	void test_aux_functions();

	/**
	 * Function for testing comparison operators defined in <utility>: !=, >, <=, >=
	 * The operators are defined as template functions in std::rel_ops namespace
	 */
	void test_comparasion_operators();

	/**
	 * Function for testing the STL vector container. Vector is a sequence container, that is
	 * the position of an element depends on the time and place of the insertion, and not of
	 * the value it has.
	 * std::vector keeps its elements in a dynamic array.  It has random access. Has the fastest
	 * access speed from all STL containers. Also has the fastest iterators from all STL containers.
	 * Appending and removing elements at the end of the container is very fast. Inserting an element
	 * at the middle or at the beginning, means that all the following elements have to be moved to make room
	 * for the new element, so it can take time.
	 * std::vector is guaranteed to have the lowest space overhead pe contained object
	 * from all the STL containers(zero bytes).
	 * Is the only STL container that guarantees that two object near each other in container,
	 * will be near each other in memory.
	 */
	void test_vector();

	/**
	 *  test function for STL container std::list
	 *  std::list is implemented as a doubly linked list of elements
	 *  std::list doesn't have random access. For accessing a element, you have to
	 *  iterate through list, until you reach the element. Compared with std::vector or std::deque
	 *  is not efficient, when accessing an random element.
	 *  std::list is efficient for insertion and removal of elements anywhere in the container.
	 *  Also for moving elements and block of elements within the container.
	 *	The std::list is efficient in removing or inserting an element in any position.
	 *	(From this point of view is more efficient than vector or deque.)
	 *	std::list is defined in <list> header
	 */
	void test_list();

	/**
	 * Function for testing the STL deque container.
	 * std::deque doesn't keep it's elements in a single block of memory (like vector). It uses multiple
	 * blocks of memory and keeps track of them. Because of that is fast on insertions at the end and also at the
	 * beginning. Also it doesn't need to copy and destroy objects when it needs to allocate more memory.
	 * Provides random access to its elements.
	 * std::deque is defined in the <deque> header.
	 */
	void test_deque();

	/**
	 * test_map: function for testing the map from STL
	 * map - is an associative container. That means that an entry in the map
	 * is a combination of key - value. std::map manages key/value std::pairs as elements.
	 * The key is used to identify an element in the map.
	 * std::map sorts its elements automatically by key. The default comparison criterion
	 * is operator<. Another ordering criterion can be provided as a template parameter (third parameter).
	 * std::map allows only unique entries. That means that you can't have in a map two entries
	 * with the same key. If you try to insert in map a entry with a certain key,
	 * and inside the map exits already an element with that key, the element will be overridden.
	 * std::map is defined in the <map> header.
	 */
	void test_map();

	/**
	 * test_set: function for testing the set from STL
	 * std::set is an associative container, that stores unique elements. Associative container means that,
	 * the index doesn't have to be an integer. It can be any type. std::set has, as his first template parameter,
	 * the index type, like this:
	 * 			template <	class Key,
	 * 						class Compare = less<Key>,
	 * 						....
	           	    	  > class set;
	 * std::set sorts automatically it's elements from lower to higher. The second template parameter is
	 * the ordering criterion, and  is by default std::less<Key>. Another one can be provided anf it has to be
	 * a class that takes two arguments of the same type as the container elements and returns a bool. (See  example bellow).
	 * std::set accepts only one copy of an object. So it's guaranteed that a set will hold unique values. If you try to insert
	 * an object that is equivalent to one that is already in the set, the set won't make the insertion.
	 * std::set has bidirectional iterators. We can advance with ++ and -- operators, but we can't move more than one step.
	 * That means that we can write for example:
	 * 				++mySetIterator; 	//ok. Move one step forward
	 * 				--mySetIterator; 	//ok. Move one step backward
	 * 				mySetIterator + 5; 	//not ok. It won't compile. Only random access iterators can move more then a step...
	 * 				mySetIterator - 2	//not ok. It won't compile.
	 * std::set is designed to be efficient accessing its elements.
	 * std::set is defined in the <set> header
	 */
	void test_set();

	/**
	 * test_multiset: function for testing the multiset
	 * std::multiset is an associative container. Associative container means that,
	 * the index doesn't have to be an integer. It can be any type. std::multiset has, as its first template parameter,
	 * the index type, like this:
	 * 			template <	class Key,
	 * 						class Compare = less<Key>,
	 * 						....
	           	    	  > class multiset;
	 * std::multiset sorts automatically it's elements from lower to higher. The second template parameter is
	 * the ordering criterion, and  is by default std::less<Key>. Another orsering criterion can be provided, and it has to be
	 * a class that takes two arguments of the same type as the container elements, and returns a bool. (See  example bellow).
	 * std::multiset accepts multiple copies of an object.
	 * std::multiset has bidirectional iterators. We can advance with ++ and -- operators, but we can't move more than one step.
	 * That means that we can write for example:
	 * 				++myMultisetIterator; 	//ok. Move one step forward
	 * 				--myMultisetIterator; 	//ok. Move one step backward
	 * 				myMultisetIterator + 5; //not ok. It won't compile. Only random access iterators can move more then a step...
	 * 				myMultisetIterator - 2	//not ok. It won't compile.
	 * std::multiset is designed to be efficient accessing its elements.
	 * std::multiset is defined in the <set> header
	 */
	void test_multiset();

	/**
	 *  stack is a container that operates as in a LIFO (last in first out) mode.
	 *  The elements are inserted and extracted only from the end of the container.
	 *  std::stack is implemented as a container adaptor. Containers adapters are classes that use an
	 *  encapsulated container and provide a restricted interface to that container.
	 *  The underlying container can be a STL container or some other  container type. It has to provide the
	 *  following public member functions: back(), push_back(), pop_back().
	 *  stack is defined as a template taking with two parameters:
	 *  		template < 	class T,
	 *  					class Container = deque<T>
	 *  				>
	 *  				class stack;
	 * 		The first parameter is the type of elements in will contain.
	 * 		The second parameter is the underlying container, and is by default std::deque.
	 * std::stack is defined in the <stack> header.
	 */
	void test_stack();

};

#endif /* TESTSTL_H_ */
