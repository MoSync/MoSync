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

	//*******************************************************************************************
	//						UTILITIES: pair, auto_ptr, max, min, !=, >, <=, >=
	//*******************************************************************************************
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

	//*******************************************************************************************
	//								CONTAINERS
	//*******************************************************************************************

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

	/**
	 *  std::priority_queue is a container in which, the first element is always the element with lowest priority the
	 *  of all the elements it contains. The element at the top is the element with the highest priority.
	 *  std::priority_queue is implemented as a container adaptor. Containers adapters are classes that use an
	 *  encapsulated container and provide a restricted interface to that container.
	 *  The underlying container can be a STL container or some other  container type. It has to provide the
	 *  following public member functions: back(), push_back(), pop_back().
	 *  priority_queue is defined as a template taking three parameters:
	 *  		template < 	class T,
	 *  					class Container = vector<T>,
	 *  					class Compare = less<typename Container::value_type>
	 *  				>
	 *  				class priority_queue;
	 * 		The first parameter is the type of elements in will contain.
	 * 		The second parameter is the underlying container, and is by default std::vector.
	 * 		The Compare is the comparison class. std::less is the default. Another comparison criterion can be
	 * 		provided. It may to be a functor object (a class implementing a function call operator), or just a
	 * 		pointer  to a function.
	 * 		The function object's call operator has to take two arguments of the same type as the container
	 * 		elements and to return a bool. If we provide a pointer to a function, same applies to the function.
	 * 		(See  example bellow).
	 * std::priority_queue is defined in the <queue> header.
	 */
	void test_prority_queue();

	/**
	 *  std::queue is designed to operate in a FIFO mode (first-in first-out).
	 *  std::priority_queue is implemented as a container adaptor. Containers adapters are classes that use an
	 *  encapsulated container and provide a restricted interface to that container.
	 *  The underlying container can be a STL container or some other  container type. It has to provide the
	 *  following public member functions: front(), back(), push_back(), pop_front().
	 *  priority_queue is defined as a template taking three parameters:
	 *  		template < 	class T,
	 *  					class Container = deque<T>
	 *  				>
	 *  				class queue;
	 * 		The first parameter is the type of elements it will contain.
	 * 		The second parameter is the underlying container, and is by default std::deque.
	 * std::priority_queue is defined in the <queue> header.
	 */
	void test_queue();

	/**
	*  function for testing bitset
	*  bitset is a special container class that is designed to store bits.
	*  It is optimized for space allocation: each element occupies only one bit.
	*  bitset is defined as a class template taking one parameter (of type unsigned int):
	*  			template < size_t N > class bitset;
	*  			N = the number of bits to represent
	*  It is defined in the <bitset> header.
	*/
	void test_bitset();

	//*******************************************************************************************
	//								ALGORITHMS
	//*******************************************************************************************

	/**
	* a predicate is a function that returns a boolean value
	* predicates are used usually, with STL algorithms, or with containers that sort they elements
	* Binary predicates are predicates that take two arguments.
	* Unary predicates are predicates taking one argument.
	* Usually binary predicates are used with sorting containers, providing them a way to compare
	* two elements.
	*/
	void predicate_example();


	/**
	* Standard function adaptors (see Adaptors.cpp) make assumptions
	* about the functors they use. (See Functor.cpp for what is a functor).
	* STL provides two templates (unary_function and binary_function),
	* that can be inherited by a functor, in order to meet the requirements
	* of STL functor adapter.
	* unary_function and binary_function are defined <functional> header.
	*/
	void example_function_pointer_adapters();

	void example_functors();

	/**
	* STL provides definitions for several functors that are very useful in combination
	* with the STL algorithms.
	* For what is a functor see AboutFunctors.cpp
	*/
	void test_functors();

	/**
	*  STL algorithms are defined in the <algorithm> header.
	*  They are function templates,implementing algorithms for sorting, filling, searching containers,
	*  comparing ranges, copying ranges ect.
	*  They can be used with any type of container (STL container or not),
	*  that provides the proper iterator and holds elements that overload the operators required by the algorithm.
	*  For example the count algorithm, compares every element in a container with a value we provide.
	*  For comparison it uses operator==, so we have to have inside the container a type that has an operator== defined.
	*
	*  For what is an ForwardIterator, BidirectionalIterator ect. see Iterators.cpp
	*/
	void test_algorithms();

	void test_string();
};

namespace NumericUtilities
{

	bool isEven(int x);
	bool isOdd(int x);
	bool isNegative(int x);

}//end NumericUtilities


#endif /* TESTSTL_H_ */
