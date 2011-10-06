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
 * @file Algorithms.cpp
 * @author Gabriela Rata
 */

#include <assert.h>
#include <vector>
#include <deque>
#include <string>
#include <algorithm>
#include <numeric> 		//for numeric algorithms
#include "../STLMoblet.h"
#include "../LOG.h"
#include "../Employee.h"

namespace{


//*****************************************************************************
//								Filling
//*****************************************************************************
void filling_algorithms()
{
	log_to_console("\n");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");
	log_to_console("filling algorithms: fill, fill_n");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");

	std::vector<int> v;
	v.resize(4);

	/**
	*  std::fill algorithm: fills a range with a value.
	*  Prototype: void fill(ForwardIterator first, ForwardIterator last,
	*  const T& val);
	*  assigns to all elements in the range [first, last) the value "val".
	*/
	std::fill(v.begin(), v.end(), 99);  //performs: *first = 99; ++first;
										//*first = 99; ect,  while first<last;
	log_to_console(v, "v after calling std::fill(v.begin(), v.end(), 99): ");


	/**
	*  std::fill_n algorithm: assigns a value to a number of elements,
	*  in a range.
	*  prototype: void fill_n(OutputIterator first, Size n, const T& val);
	*  std::fill_n assigns "val" to "n" elements starting at "first".
	*/
	std::fill_n(v.begin(), 2, 1001); //performs: *first = 1001; ++first; *first
									 //= 1001; for "n" iterations
	log_to_console(v, "v after calling std::fill_n(v.begin(), 2, 1001): ");

}

//*****************************************************************************
//								Generating
//*****************************************************************************

int generatea_even_numbers()
{
	static int num = 0;
	int temp = num;
	num += 2;
	LOG("Calling generatea_even_numbers(). Returns %d", temp);
	return temp;
}
int generatea_odd_numbers()
{
	static int num = 1;
	int temp = num;
	num += 2;
	LOG("Calling generatea_odd_numbers(). Returns %d", temp);
	return temp;
}

void generating_algorithms()
{
	log_to_console("\n");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");
	log_to_console("generators: generate, generate_n");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");

	std::vector<int> v(4);

	log_to_console(v, "v contains: ");
	log_to_console("Calling std::generate(v.begin(), v.end(), "
			"generatea_even_numbers)");

	/**
	* std::generate algorithm: generates values for a range with a function
	* prototype: void generate(ForwardIterator first, ForwardIterator last,
	* Generator someGen);
	* calls someGen() for each element in the range, and assigns the value
	* returned by someGen() to the element.
	*/
	std::generate(v.begin(), v.end(), generatea_even_numbers);

	log_to_console(v, "v after calling std::generate(v.begin(), v.end(),"
			"generatea_even_numbers): ");


	log_to_console("\n    Calling std::generate_n(v.begin(), 2, "
			"generate_odd_numbers): ");

	/**
	* std::generate_n algorithm: assigns the values, returned by a function,
	* to a number of elements
	* in a range.
	* prototype: void generate_n(OutputIterator first, Size n,
	* Generator someGen);
	* std::generate_n calls someGen() "n" times, for each of the "n" elements,
	* and assigns the value returned by the function to the current element.
	*/
	std::generate_n(v.begin(), 2, generatea_odd_numbers);

	log_to_console(v, "v after calling std::generate_n(v.begin(), 2, "
			"generatea_odd_numbers);");


	std::vector<int> v2;
	log_to_console("\n    v2 is empty");
	log_to_console("Calling std::generate_n(back_inserter(v2), 3, "
			"generatea_even_numbers);");

	/**
	* back_inserter() inserts the values at the end of v2 (and expands v2
	* automatically).
	*/
	std::generate_n(back_inserter(v2), 3, generatea_even_numbers);

	log_to_console(v2, "v after calling std::generate_n(back_inserter(v2), 3, "
			"generatea_even_numbers): ");
}

//*****************************************************************************
//								Counting
//*****************************************************************************

bool isLessThan303(int val)
{
	if( val< 303)
	{
		LOG("Calling isLessThan303(%d). Returns true.", val);
	}
	else
	{
		LOG("Calling isLessThan303(%d). Returns false.", val);
	}

	return val < 303;
}

void counting_algorithms()
{
	log_to_console("\n");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");
	log_to_console("algorithms for counting: count, count_if");
	log_to_console("----------------------------------------------------------"
					"---------------------------------------------------------");

	int array[] = { 0, 2, 303, 303, 303, 4, 5 };
	int arraySize = sizeof(array)/sizeof(array[0]);
	log_to_console(array, arraySize, "array contains: ");

	/**
	* std::count: counts the number of elements, from a range, that are
	* equivalent to a value.
	* prototype: int count(InputIterator first, InputIterator last,
	* const T& val);
	* counts the number of elements that are equal to "val".
	* operator== is used for testing the equivalence, so the type of elements
	* in the range must have an operator== defined.
	*/
	int res = std::count(array, array + arraySize, 303);

	log_to_console(res, "Calling std::count(array, array + arraySize, 303). "
			"Returns ");


	log_to_console("\n    Calling std::count_if(array, array + arraySize, "
			"isLessThan)");

	/**
	* std::count_if: counts the number of elements in a range that satisfy a
	* condition.
	* prototype: int count_if(InputIterator first, InputIterator last,
	* Predicate somePred ); //see AboutPredicates.cpp for what is a predicate
	* counts the number of elements for which somePred returns true.
	*/
	res = std::count_if(array, array + arraySize, isLessThan303);

	// < 303: 0, 2, 4, 5 ( four elements )
	log_to_console(res, "std::count_if returned: ");

}


//*****************************************************************************
//							Modifying sequences
//*****************************************************************************
void algorithms_for_modifying_sequences()
{
	log_to_console("\n");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");
	log_to_console( "algorithms for modifying sequences:"
		"\n\tcopy, copy_backward,"
		"\n\treverse, reverse_copy, "
		"\n\tswap_ranges,"
		"\n\trotate, rotate_copy,"
		"\n\tnext_permutation, prev_permutation,"
		"\n\trandom_shuffle, "
		"\n\tpartition, stable_partition");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");

	int array[] = { 10, 15, 25 };
	int arraySize = sizeof(array)/sizeof(array[0]);
	log_to_console(array, arraySize, "array contains: ");

	std::vector<int> v(arraySize);
	log_to_console(v, "\n    v contains: ");
	/**
	* std::copy: copies (using assignment) a range into another range.
	* OutputIterator copy(InputIterator first, InputIterator last,
	* OutputIterator destination);
	* copies the elements in the range [first, last), into the range that
	* starts with "destination".
	*/
	std::copy(array, array + arraySize, v.begin()); //*destination = 10;
													//++destination;
													//*destination = 15 ect;
	log_to_console(v, "v after calling std::copy(array, array + arraySize, "
			"v.begin()): ");


	std::vector<int> vb(arraySize);
	log_to_console(vb, "\n    vb contains: ");
	/**
	*std::copy_backward: copies a range of elements into another range,
	*backwards.
	*prototype: BidirectionalIterator2 copy_backward(
	*			 							BidirectionalIterator1 first,
	*	 									BidirectionalIterator1 last,
	*										BidirectionalIterator2 destEnd);
	*
	*copies starting with *(--destEnd) = *(--last) and iterates until
	*reaches "first", coping also "first".
	*/
	std::copy_backward(array, array + arraySize, vb.end());  // performs:
															//*--destEnd = 25;
															//--destEnd=15 ect;
	log_to_console(vb, "vb after std::copy_backward(array, array + arraySize, "
			"vb.end()): ");

	/**
	* std::reverse: reverses a range
	* prototype: void reverse(BidirectionalIterator first,
	* BidirectionalIterator last);
	*/
	std::reverse( vb.begin(), vb.end() );
	log_to_console(vb, "\n    vb after std::reverse( vb.begin(), vb.end()): ");

	std::vector<int> rev_copy(arraySize);
	log_to_console(rev_copy, "\n    rev_copy contains: ");
	log_to_console(array, arraySize, "array contains: ");
	/**
	* std::reverse_copy: copies the elements from a range, in reversed order,
	* into another range
	* prototype: void reverse_copy( BidirectionalIterator first,
	* 								BidirectionalIterator last,
	* 								OutputIterator destination );
	*/
	std::reverse_copy(array, array + arraySize, rev_copy.begin());
	log_to_console(rev_copy, "rev_copy after std::reverse_copy(array, "
			"array + arraySize, rev_copy.begin()): ");


	int first_range[] = {  11, 12, 13 };
	int second_range[] = { 21, 22, 23 };
	int rangeSize = sizeof(first_range)/sizeof(first_range[0]);
	log_to_console(first_range, rangeSize, "\n    first_range contains: ");
	log_to_console(second_range, rangeSize, "second_range contains: ");
	/**
	* std::swap_ranges: exchange the contents of two ranges. The ranges have
	* to be of equal size.
	* ForwardIterator2 swap_ranges(	ForwardIterator1 first1,
	* 								ForwardIterator1 last1,
	* 								ForwardIterator2 first2);
	* The elements from first range [first1, last1) will be in the second range,
	* and the elements from the second range will be in the first range.
	*/
	std::swap_ranges(first_range, first_range + rangeSize, second_range);

	log_to_console("After calling std::swap_ranges(first_range, "
			"first_range + rangeSize, second_range)");
	log_to_console(first_range, rangeSize, "\t\tfirst_range contains: ");
	log_to_console(second_range, rangeSize, "\t\tsecond_range contains: ");

	std::string strArray[] = { "Hello", "S", "T", "L"};
	int strArraySize = sizeof(strArray)/sizeof(strArray[0]);
	log_to_console(strArray, strArraySize, std::string("\n    strArray "
			"contains: "));

	/**
	* std::rotate: rotates elements in a range
	* void rotate(  ForwardIterator first,
	* 				ForwardIterator middle,
	* 				ForwardIterator last);
	* Moves the contents of [first, middle) to the end of the range, and
	* the contents of [middle, last) to the beginning.
	* [first, last) becomes [middle, last) + [first, middle).
	*/
	std::rotate(strArray, strArray + 2, strArray + strArraySize);
	log_to_console(strArray, strArraySize, std::string("strArray after calling"
			" std::rotate(strArray, strArray + 2, strArray + strArraySize): "));


	int myArray[] = { 40, 50, 60, 10, 20, 30 };
	int sizeOfArray = sizeof(myArray)/sizeof(myArray[0]);
	std::vector<int> myVector(sizeOfArray);
	log_to_console(myArray, sizeOfArray, "\n    myArray contains: ");

	/**
	* std::rotate_copy:
	* OutputIterator rotate_copy( ForwardIterator first,
	* 							  ForwardIterator middle,
	* 							  ForwardIterator last,
								  OutputIterator destination);
	*Copies the range composed by [middle, last) +  [first, middle) into the
	*range that begins with "destination".
	*/
	std::rotate_copy(myArray, myArray + 3, myArray + sizeOfArray,
			myVector.begin());  //the middle element is 10

	log_to_console(myVector, "myVector after calling "
			"std::rotate_copy(myArray, myArray + 3, myArray + sizeOfArray,"
			"myVector.begin(): ");


	char chars[] = { 'a', 'b', 'c' };
	int sizeOfChars = sizeof(chars)/sizeof(*chars);
	log_to_console(chars, sizeOfChars, "\n    chars contains: ");
	/**
	*	A permutation of a set of elements means an arrangement of those
	*	elements into a particular oder.
	*	For n elements in a range, we will have n! (n factorial) permutations.
	*	That is n ways to arrange the elements.
	*	All these combinations can be sorted in a dictionary-like (or
	*	lexicographically) manner.
	*	The smallest permutation is considered the one in which all
	*	elements are sorted into ascending order. The biggest
	*	permutation is the one that has all its elements sorted into descending
	*	order.
	*		e.g: for { 1, 2, 3 } we have 3!=6 permutations:
	*			-> first permutation: 1, 2, 3,  (smallest permutation)
	*			-> next permutation:  1, 3, 2
	*			-> next permutation:  2, 1, 3
	*			-> ect
	*			-> last permutation:  3, 2, 1 (biggest permutation)
	*/

	/**
	* std::next_permutation: rearranges the elements from a range into the next
	* (greater) permutation.
	* If successful, returns true. If there are no more permutations, returns
	* false.
	* It has two versions:
	*	bool next_permutation(	BidirectionalIterator first,
	*							BidirectionalIterator last);
	*	bool next_permutation(	BidirectionalIterator first,
	*							BidirectionalIterator last,
	*							BinaryPredicate somePred);
	*
	*	The first version uses operator< for comparing the elements in the range.
	*	The second version uses binary_pred for comparing the elements in
	*	the range.
	*	Compares the elements in the range, so that it knows when the elements
	*	are sorted into descending
	*	order, and therefore it reached the "biggest" permutation (and no next
	*	permutation exists). The same for the "smallest" permutation, and the
	*	range between the biggest and smallest permutation.
	*/
	std::next_permutation(chars, chars + sizeOfChars);
	log_to_console(chars, sizeOfChars, "chars after calling next_permutation"
			"(chars, chars + sizeOfChars): ");

	/**
	*	std::prev_permutation: rearranges the elements from a range into the
	*	previous (smaller) permutation.
	*	If successful, returns true. If there are no more permutations, returns
	*	false.
	*   It has two versions:
	*	bool prev_permutation(  BidirectionalIterator first,
	*							BidirectionalIterator last);
	*	bool prev_permutation(  BidirectionalIterator first,
	*							BidirectionalIterator last,
	*							BinaryPredicate somePred);
	*   The first version uses operator< for comparing the elements in the range.
	*	The second version uses somePred for comparing the elements in the range.
	*/
	std::prev_permutation(chars, chars + sizeOfChars);
	log_to_console(chars, sizeOfChars, "chars after calling prev_permutation("
			"chars, chars + sizeOfChars): ");


	int orderedArray[] = { 0, 1, 2, 3, 4, 5, 6 };
	int sizeOrderedArray = sizeof(orderedArray)/sizeof(orderedArray[0]);

	std::vector<int> rVector(orderedArray, orderedArray+ sizeOrderedArray);
	log_to_console(rVector, "\n    rVector contains: ");

	/**
	*	std::random_shuffle: function rearranges the elements in a range in a
	*	random way.
	*	void random_shuffle(RandomAccessIterator first,
	*						RandomAccessIterator last);
	*/

	std::random_shuffle(rVector.begin(), rVector.end());
	log_to_console(rVector, "rVector after std::random_shuffle("
			"rVector.begin(), rVector.end()): ");


	std::vector<int> pVector(orderedArray, orderedArray+ sizeOrderedArray);
	log_to_console(pVector, "\n    pVector contains: ");
	log_to_console("Calling std::partition(pVector.begin(), pVector.end(), "
			"isEven)");
	/**
	*	std::partition: moves the elements that satisfy a certain condition,
	*	to the beginning of the range.
	*	prototype: BidirectionalIterator partition(BidirectionalIterator first,
	*		BidirectionalIterator last, Predicate somePred);
	*	Moves the elements for which "somePred" returns true at the beginning
	*	of the range.
	*	std::partition returns an iterator pointing to the one past end of the
	*	range that satisfies "somePredicate".
	*	The relative order of elements after the partition is not guaranteed to
	*	be the same, as before the partition.
	*/
	std::partition(pVector.begin(), pVector.end(), NumericUtilities::isEven);

	//{ 0, 1, 2, 3, 4, 5, 6 } => { 0, 6, 2, 4, 3, 5, 1 }
	log_to_console(pVector, "pVector after "
			"std::partition(pVector.begin(), pVector.end(), isEven)");


	std::vector<int> sVector(orderedArray, orderedArray+ sizeOrderedArray);
	log_to_console(sVector, "\n    sVector contains: ");
	log_to_console("Calling "
			"std::stable_partition(sVector.begin(), sVector.end(),isEven);");
	/**
	* std::stable_partition: moves the elements that satisfy a certain
	* condition, to the beginning of the range.
	* prototype: BidirectionalIterator partition(BidirectionalIterator first,
	* 	BidirectionalIterator last, Predicate somePred);
	* Moves the elements for which "somePred" returns true, at the beginning
	* of the range.
	* std::partition returns an iterator pointing to the one past end of the
	* range that satisfies "somePredicate".
	* The relative order of elements after the partition is the same, as before
	* the partition.
	*/
	std::vector<int>::iterator result = std::stable_partition(
			sVector.begin(), sVector.end(), NumericUtilities::isEven);

	log_to_console(sVector, "sVector after calling std::stable_partition("
			"sVector.begin(), sVector.end(),isEven): ");
}

//*****************************************************************************
//						Searching and replacing
//*****************************************************************************
void searching_algorithms()
{
	log_to_console("\n");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");
	log_to_console("algorithms for searching:\n\tfind, find_if,"
			"\n\tadjacent_find,\n\tfind_first_of, \n\tsearch, search_n,"
			"\n\tfind_end, \n\tmin_element, max_element");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");


	Employee staff[] = { Employee(4, "Bob"),
						 Employee(8, "Ion"),
						 Employee(10, "Anna") };
	int staffSize = sizeof(staff)/sizeof(staff[0]);
	std::vector<Employee> v1(staff, staff + staffSize);
	log_to_console(v1, "\n    v1 contains: ");

	/**
    * std::find: searches inside a range for a value.
	* prototype: InputIterator find(InputIterator first,
	* 								InputIterator last,
	* 								const T& value);
	* If it finds an element equal with "value"  it returns an iterator
	* pointing to that element.
	* If doesn't find any element, it returns last.
	* An operator== must be available for comparing the elements in the range
	* with the value.
	*/
	std::vector<Employee>::iterator result1 = std::find(v1.begin(), v1.end(),
		Employee(4, "Bob")); //calls operator== for every element and the value.

	log_to_console(*result1, std::string("std::find(v1.begin(), v1.end(), "
			"Employee(4, \"Bob\")) returns: "));


	result1 = std::find(v1.begin(), v1.end(), Employee(0, "Maria"));
	assert(result1 == v1.end()); //didn't found Maria.


	int array[] = { 6, -4, -4, -4,  5 };
	int arraySize = sizeof(array)/sizeof(array[0]);
	std::vector<int> v2(array, array + arraySize);

	log_to_console(v2, "\n    v2 contains: ");
	log_to_console("Calling find_if(v2.begin(), v2.end(), isEven);");

	/**
	* std::find_if: searches for a value in a range, and returns the first
	* occurrence of that value.
	* prototype: InputIterator find_if(	InputIterator first,
	* 									InputIterator last,
	* 									Predicate pred);
	* If it finds an element for which pred(element, value) returns true it will
	* return an iterator pointing to that element. If doesn't find any element,
	* returns last.
	*/
	std::vector<int>::iterator result2 =
			std::find_if(v2.begin(), v2.end(), NumericUtilities::isEven);

	log_to_console(*result2, "find_if(v2.begin(), v2.end(), isEven) returns: ");


	std::vector<int> v3(array, array + arraySize);
	log_to_console(v3, "\n    v3 contains: ");

	/**
	* std::adjacent_find searches for two adjacent elements that are equal.
	* It has two versions:
	* 	ForwardIterator adjacent_find(	ForwardIterator first,
	* 									ForwardIterator last);
	* 	ForwardIterator adjacent_find(	ForwardIterator first,
	* 									ForwardIterator last,
	* 									BinaryPredicate somePredicate);
	* 	The first version uses operator== to test for equality, the second uses
	*   "somePredicate".
	* 	Returns an iterator pointing to the first element, if it finds two equal
	* 	elements. If it doesn't find anything, returns last.
	*/
	 std::vector<int>::iterator result3 = std::adjacent_find(v3.begin(),
			 v3.end());

	 log_to_console("std::vector<int>::iterator result3 = "
			 "adjacent_find(v3.begin(), v3.end());");
	 log_to_console(*result3, "result3 = ");
	 log_to_console(*(result3-1), "*(result-1) = ");


	int first_array[]  = {  1, 2, 3, 4, 5, 6 };
	int second_array[] = {  2, 3, 4 };
	int firstArraySize = sizeof(first_array)/sizeof(first_array[0]);
	int secondArraySize = sizeof(second_array)/sizeof(second_array[0]);

	std::vector<int> vFirst(first_array, first_array + firstArraySize);
	std::vector<int> vSecond(second_array, second_array + secondArraySize);
	log_to_console(vFirst, "\n    vFirst contains: ");
	log_to_console(vSecond, "vSecond contains: ");

	/**
	* std::find_first_of: searches for an element in the second range that is
	* equal to one in the first.
	* It has two versions:
	*	ForwardIterator1 find_first_of( ForwardIterator1 first1,
	*									ForwardIterator1 last1,
	*									ForwardIterator2 first2,
	*									ForwardIterator2 last2);
	* 	ForwardIterator1 find_first_of( ForwardIterator1 first1,
	* 									ForwardIterator1 last1,
	* 									ForwardIterator2 first2,
	* 									ForwardIterator2 last2,
	* 								  	BinaryPredicate somePredicate);
	*
	* 	The first version tests for equality with the help of
	* 	operator == (calls operator==(firstRangeElement, secondRangeElement)).
	* 	The second version uses "somePredicate" to test for equality
	* 	(calls somePredicate(firstRangeElem, secondRangeElem).
	*
	* Returns an iterator pointing to the found element (from the first
	* 	range), or last2, if it doesn't find anything.
	*/
	std::vector<int>::iterator resFind = std::find_first_of(
			vFirst.begin(), vFirst.end(), vSecond.begin(), vSecond.end());

	log_to_console("std::vector<int>::iterator resFind = "
			"find_first_of(vFirst.begin(), vFirst.end(), "
			"vSecond.begin(), vSecond.end());");
	log_to_console(*resFind, "*resFind = ");
	log_to_console("resFind == vSecond.begin()");


	std::vector<int> range(first_array, first_array + firstArraySize);
	std::vector<int> subrange(second_array, second_array + secondArraySize);
	log_to_console(range, "\n    range contains: ");
	log_to_console(subrange, "subrange contains: ");
	/**
	* std::search:  checks if the first range contains the second range.
	* It has two versions:
	* 	ForwardIterator1 search(	ForwardIterator1 first1,
	* 								ForwardIterator1 last1,
	* 								ForwardIterator2 first2,
	* 								ForwardIterator2 last2);
	*
	* 	ForwardIterator1 search(	ForwardIterator1 first1,
	* 								ForwardIterator1 last1,
	* 								ForwardIterator2 first2,
	* 								ForwardIterator2 last2
	* 								BinaryPredicate somePredicate);
	*
	* 	Searches the range [first1,last1) for the first occurrence of
	* 	the range [first2,last2).
	*   The first version tests for equality with operator ==.
	*   The second uses "somePredicate".
	*
	* If the search is successful returns an iterator pointing to the first
	* point in the first range where the second range begins. If not, returns
	* last1.
	*/

	std::vector<int>::iterator resSearch = std::search(range.begin(),
			range.end(), subrange.begin(), subrange.end());

	log_to_console("std::vector<int>::iterator resSearch = "
			"search(range.begin(), range.end(),"
			"subrange.begin(), subrange.end()): ");
	log_to_console(*resSearch, "*resSearch = ");
	log_to_console(*(resSearch-1), "*(resSearch-1) = ");

	std::vector<int> myRange(array, array + arraySize);
	log_to_console(myRange, "\n    myRange contains: ");
	/**
	* std::search_n: searches a group of n consecutive values, in a range, that
	* are equal to a value.
	* It has two versions:
	* 	ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
	* 							int n, const T& value);
	* 	ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
	* 							int n, const T& value,
	*  							BinaryPredicate somePredicate);
	*
	*	Searches n consecutive values in [first, last) that are equal
	*	to "value".
	*	The first version uses operator== to compare the elements in the range
	*	with the "value". The
	*	second version uses "somePredicate" to test for equality.
	*	Returns an iterator pointing to first element equal to "value". If no
	*	element is equal to "value" returns "last".
	*/
	std::vector<int>::iterator search_nResult = std::search_n(myRange.begin(),
			myRange.end(), 3, -4);
	log_to_console("std::vector<int>::iterator search_nResult = "
			"search_n(myRange.begin(), myRange.end(), "
			"3, -4) returns: ");
	log_to_console(*search_nResult, "*search_nResult = ");
	log_to_console(*(search_nResult-1), "*(search_nResult-1) = ");

	int first_range[] = { 1, 2, 3, -900, 1, 2, 3, -800 };
	int second_range[] = { 1, 2, 3 };
	int firstRangeSize = sizeof(first_range)/sizeof(first_range[0]);
	int secondRangeSize = sizeof(subrange)/sizeof(subrange[0]);
	log_to_console(first_range, firstRangeSize, "\n    first_range contains: ");
	log_to_console(second_range, secondRangeSize, "second_range contains: ");
	/**
	* std::find_end: searches a subrange inside a range. Searches for the last
	* occurrence of the subrange.
	* Two versions:
	* 	ForwardIterator1 find_end( 	ForwardIterator1 first1,
	* 								ForwardIterator1 last1,
	* 								ForwardIterator2 first2,
	* 								ForwardIterator2 last2);
	*
	* 	ForwardIterator1 find_end(	ForwardIterator1 first1,
	* 								ForwardIterator1 last1,
	* 								ForwardIterator2 first2,
	* 								ForwardIterator2 last2,
	* 								BinaryPredicate somePredicate);
	*
	* 	Searches the range [first1,last1) for the last occurrence [first2,last2).
	* 	The first version uses operator==, to test for equality between two
	* 	elements. The second uses "somePredicate"
	*   to check if two elements are equal.
	* If the search is successful returns an iterator pointing to the last point
	* in the first range where the second range begins. If not, returns last1.
	*/
	int *find_end_result =
			std::find_end( first_range, first_range + firstRangeSize,
			second_range, second_range + secondRangeSize);

	log_to_console("int *find_end_result = find_end( first_range, "
			"first_range + firstRangeSize, second_range, "
			"second_range + secondRangeSize) returns: ");
	log_to_console(*find_end_result, "*find_end_result = ");
	log_to_console(*(find_end_result - 1), "*(find_end_result - 1) = ");

	int someRange[] = { 3, 1, 1, 2, 55, 55 };
	int rangeSize = sizeof(someRange)/sizeof(someRange[0]);
	log_to_console(someRange, rangeSize, "\n    someRange contains: ");
	/**
	* std::min_element: returns the smallest element in a range.
	* It has two versions:
	* 	ForwardIterator min_element(ForwardIterator first,ForwardIterator last);
	* 	ForwardIterator min_element(ForwardIterator first, ForwardIterator last,
	* 			BinaryPredicate somePredicate);
	*
	*   Returns the first occurrence of the smallest element in the
	*   range [first, last).
	*   The version uses the operator< to compare elements. The second version
	*   uses "somePredicate".
	*/
	int *minElement = std::min_element(someRange, someRange + rangeSize);

	log_to_console(*minElement, "min_element(someRange, someRange + rangeSize) "
			"returns: ");

	/**
	* std::max_element: returns the biggest element in a range.
	* It has two versions:
	* 	ForwardIterator max_element(ForwardIterator first, ForwardIterator last);
	* 	ForwardIterator max_element(ForwardIterator first, ForwardIterator last,
	* 		BinaryPredicate binary_pred);
	*   Returns the first occurrence of the biggest element in the
	*   range [first, last).
	*   The version uses the operator< to compare elements. The second version
	*   uses "somePredicate".
	*/
	int *maxElement = std::max_element(someRange, someRange + rangeSize);
	log_to_console(*maxElement, "max_element(someRange, someRange + rangeSize) "
			"returns: ");
}

//*****************************************************************************
//						Replacing elements in a range
//*****************************************************************************
void algorithms_for_replacing()
{
	log_to_console("\n");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");
	log_to_console("algorithms for replacing elements in a range:"
			"\n\treplace, replace_if, replace_copy, replace_copy_if");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");

	int array [] = { -77, -77, 2, 2, 3, 3};
	int arraySize = sizeof(array)/sizeof(array[0]);
	std::vector<int> v1(array, array + arraySize);
	log_to_console(v1, "\n    v1 contains: ");

	/**
	* std::replace algorithm: replaces all the values in a range with a
	* new value.
	* prototype: void replace(ForwardIterator first, ForwardIterator last,
	* 						  const T& oldValue, const T& newValue);
	*/
	std::replace(v1.begin(), v1.end(), -77, 1);
	log_to_console(v1, "v1 after calling "
			"replace(v1.begin(), v1.end(), -77, 1): ");


	std::vector<int> v2(array, array + arraySize);
	log_to_console(v2, "\n    v2 contains: ");
	/**
	* std::replace_if: replaces all the elements in a range, that satisfy a
	* certain condition, with a new value.
	* prototype: void replace_if(	ForwardIterator first, ForwardIterator last,
	* 								Predicate somePred, const T& newValue);
	*/
	std::replace_if(v2.begin(), v2.end(), NumericUtilities::isNegative, 1);
	log_to_console(v2, "v2 after calling replace_if(v2.begin(), v2.end(),"
			"isNegative, 1): ");

	std::vector<int> v3;
	v3.resize(arraySize);
	log_to_console(v3, "\n    v3 contains: ");
	log_to_console(array, arraySize, "array contains: ");
	/**
	* std::replace_copy: same behaviour as std::replace, except that it doesn't
	* modify the original range,
	* but it copies the original range, with the replaced values into a
	* new range.
	* prototype: OutputIterator replace_copy(	InputIterator first,
	* 											InputIterator last,
	* 											OutputIterator destination,
	* 							  				const T& oldValue,
	* 											const T& newValue);
	*
	* copies the elements from the range [first,last) to the range starting
	* with "destination", and replaces the occurrences of "oldValue" with
	* "newValue".
	*/
	std::replace_copy(array, array + arraySize, v3.begin(), -77, 1);
	log_to_console(v3, "v3 after calling replace_copy(array, "
			"array + arraySize, v3.begin(), -77, 1): ");

	std::vector<int> v4;
	v4.resize(arraySize);
	log_to_console(v4, "\n    v4 contains: ");
	log_to_console(array, arraySize, "array contains: ");
	/**
	* std::replace_copy_if: same behaviour as std::replace_if, except that it
	* doesn't modify the original range,
	* but it copies the original range, with the replaced values into a
	* new range.
	* OutputIterator replace_copy_if( 	InputIterator first, InputIterator last,
	* 									OutputIterator destination,
	* 									Predicate somePredicate,
	* 									const T& newValue);
	*/
	std::replace_copy_if(array, array + arraySize, v4.begin(),
			NumericUtilities::isNegative, 1);
	log_to_console(v4, "v4 after replace_copy_if(array, array + arraySize, "
			"v4.begin(), isNegative, 1): ");
}

//*****************************************************************************
//							Comparing ranges
//*****************************************************************************
void algorithms_for_comparing()
{
	log_to_console("\n");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");
	log_to_console("algorithms for comparing ranges:\n\tequal, "
			"lexicographical_compare, mismatch");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");

	int first_range[] =  { 2, 3, 4, 5 };
	int second_range[] = { 2, 3, 4, 5 };
	int third_range[] =  { 3, 2, 4, 5 };
	int fourth_range[] = { 2, 3, 4, 5,  6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
	int rangeSize = sizeof(first_range)/sizeof(first_range[0]);

	log_to_console(first_range, rangeSize, "\n    first_range contains: ");
	log_to_console(second_range, rangeSize, "second_range contains: ");
	log_to_console(third_range, rangeSize, "third_range contains: ");
	log_to_console(fourth_range, rangeSize, "fourth_range contains: ");
	/**
	*   std::equal: returns true if two ranges compare equal (have the same
	*   elements in the same order).
	*   It has two versions:
	* 		bool equal(InputIterator first1, InputIterator last1,
	* 		InputIterator first2);
	*		bool equal(	InputIterator first1, InputIterator last1,
	*					InputIterator first2 BinaryPredicate somePredicate);
	*
	*		The first version compares the elements from the first range with
	*		the ones from the second range, using
	*		operator==.
	*		The second version uses "somePredicate." If the predicate returns
	*		true, the elements are considered equal.
	*/
	bool equality = std::equal(first_range, first_range + rangeSize, second_range);
	log_to_console(equality, "equal(first_range, "
			"first_range + rangeSize, second_range) returns: ");

	equality = std::equal(second_range, second_range + rangeSize, third_range);
	log_to_console(equality, "equal(second_range, "
			"second_range + rangeSize, third_range) returns: ");

	//compares until reaches the end of first_range ("last1").
	equality = std::equal(first_range, first_range + rangeSize, fourth_range);
	log_to_console(equality, "equal(first_range, first_range + rangeSize, "
			"fourth_range) returns: ");

	char first_word[] = {  's', 'o', 'c', 'i', 'e', 't', 'y' };
	char second_word[] = { 's', 'o', 'n', 'g' };
	int firstWordLen = sizeof(first_word)/sizeof(first_word[0]);
	int secondWordLen = sizeof(second_word)/sizeof(second_word[0]);
	log_to_console(first_word, firstWordLen, "\n    first_word contains: ");
	log_to_console(second_word, secondWordLen, "second_word: ");
	/**
	*   std::lexicographical_compare: compares lexicographically two ranges,
	*   and returns true if the first is smaller
	*   then the second.
	* 	bool lexicographical_compare(	InputIterator1 first1,
	* 									InputIterator1 last1,
	* 									InputIterator2 first2,
	* 									InputIterator2 last2);
	*	bool lexicographical_compare(InputIterator1 first1,
	*								 InputIterator1 last1, InputIterator2 first2,
	*								 InputIterator2 last2,
	*								 BinaryPredicate somePredicate);
	*   The first version uses operator<, for comparing elements.
	*   The second uses "somePredicate". If the predicate returns true, the
	*   element from the first range is considered smaller (than the one from
	*   the second range).
	*/
	bool firstIsLess = std::lexicographical_compare(first_word,
			first_word + firstWordLen, second_word,
			second_word + secondWordLen);
	log_to_console(firstIsLess, "lexicographical_compare(first_word,"
			"first_word + firstWordLen, second_word,"
			"second_word + secondWordLen) returns: ");


	char first[] =  { 'a', 'b', 'c', 'd', 'e', 'f' };
	char second[] = { 'a', 'b', 'm', 'd', 'e', 'k' };
	int size = sizeof(first)/sizeof(first[0]);
	log_to_console(first, size, "\n    first contains: ");
	log_to_console(second, size, "second contains: ");
	/**
	*  std::mismatch: compares two ranges, and returns the first point were
	*  the ranges stop to be equal. The ranges have to
	*  be be of equal size.
	*	pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1,
	*		InputIterator1 last1,  InputIterator2 first2);
	*	pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1,
	*		InputIterator1 last1,  InputIterator2 first2,
	*		BinaryPredicate somePredicate);
	*   The first version returns a pair of iterators to the first element in
	*   each range for which operator== returns false.
	*   The second version returns a pair of iterators to the first element in
	*   each range for which "somePredicate" returns false.
	*/
	std::pair<char*, char*> firstMismatchPoint = std::mismatch(first,
			first + size, second);

	log_to_console("std::pair<char*, char*> firstMismatchPoint = "
			"std::mismatch(first, first + size, second);");
	std::string str = "*firstMismatchPoint.first = ";
	str.append(1, *firstMismatchPoint.first);
	str += ", *firstMismatchPoint.second = ";
	str.append(1, *firstMismatchPoint.second);
	log_to_console(str.c_str());
}

//*****************************************************************************
//							Removing elements
//*****************************************************************************

/**
* The STL algorithms for removing, don't actually erase the elements from a
* range. They rearrange the elements, so that the "removed" ones are placed
* at end of the sequence. All the other elements remain in the same
* relative order.
* The removing function returns an iterator to the "new end" of the range,
* which is the end of the range, without the removed elements. The
* iterators past the "new end" [new_end, last_end)  are deferenceable,
* but the values to which they point are unspecified, so we shouldn't
* use/deference these iterators.
*/
void algorithms_for_removing()
{
	log_to_console("\n");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");
	log_to_console("algorithms for removing elements from a range:"
			"\n\tremove, remove_if, \n\tremove_copy, remove_copy_if,"
			"\n\tunique, unique_copy");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");

	int array[] = { 1, 2, 3, -99, 4, -99 };
	int arraySize = sizeof(array)/sizeof(array[0]);
	std::vector<int> v(array, array + arraySize);
	log_to_console(v, "\n     v contains: ");

	/**
	* std::remove: removes all the elements with a certain value
	* ForwardIterator remove(ForwardIterator first, ForwardIterator last,
	* 		const T& value);
	*/
	std::vector<int>::iterator newEnd = std::remove(v.begin(), v.end(), -99);
	log_to_console("std::vector<int>::iterator newEnd = "
			"std::remove(v.begin(), v.end(), -99);");
	log_to_console(*newEnd, "*newEnd = ");
	log_to_console(v, "v contains now: ");

	std::string msg = "\n     The size of the vector wasn't changed."
			"\n     std::remove only moved the elements to the end of the "
			"vector and returned a new end."
			"\n     In order to remove them we have to use erase."
			"\n     This will erase the elements, and change the size of the"
			" vector.";
	log_to_console(msg.c_str());

	log_to_console("\n     Calling v.erase(newEnd, v.end());");
	v.erase(newEnd, v.end());
	log_to_console(v, "v contains now: ");

	std::vector<int> v2(array, array + arraySize);
	log_to_console(v2, "\n     v2 contains: ");
	/**
	* std::remove_if: removes all the elements that satisfy a certain condition.
	* ForwardIterator remove_if(ForwardIterator first, ForwardIterator last,
	* Predicate somePredicate);
	*/
	newEnd = std::remove_if(v2.begin(), v2.end(), NumericUtilities::isNegative);
	log_to_console("newEnd = std::remove_if(v2.begin(), v2.end(), isNegative)");
	log_to_console(*newEnd, "*newEnd = ");
	log_to_console(v2, "v2 contains: ");

	v2.erase(newEnd, v2.end());
	log_to_console("v2.erase(newEnd, v2.end());");
	log_to_console(v2, "v2 contains now: ");

	std::vector<int> v3;
	v3.resize(arraySize + 3);	//resize the vector to hold 6 + 3 elements
	log_to_console(v3, "\n     v3 contains: ");
	log_to_console(array, arraySize, "array contains: ");
	/**
	* std::remove_copy: same behaviour as std::remove, with the difference that
	* the result of the removing is copied into a new range, and the original
	* range remains unchanged.
	* OutputIterator remove_copy(	InputIterator first, InputIterator last,
	* 								OutputIterator result, const T& value);
	*
	* Returns an iterator indicating the past-the-end value the new range.
	*/
	std::vector<int>::iterator endIt = std::remove_copy(array,
			array + arraySize, v3.begin(), -99);
	log_to_console(v3, "v3 after calling remove_copy(array, array + arraySize,"
			" v3.begin(), -99): ");

	std::vector<int> v4;
	v4.resize(arraySize + 5); //resize the vector to hold 6 + 5 elements
	log_to_console(v4, "\n     v4 contains: ");
	log_to_console(array, arraySize, "array contains: ");
	/**
	* std::remove_copy_if: same behaviour as std::remove_if, with the difference
	* that the result of the removing
	* is copied into a new range, and the original range remains unchanged.
	* OutputIterator remove_copy_if(InputIterator first, InputIterator last,
	* 		OutputIterator result, Predicate somePredicate);
	*/
	std::vector<int>::iterator endIt_v4 = std::remove_copy_if(array,
			array + arraySize, v4.begin(),
			NumericUtilities::isNegative);
	log_to_console(v4, "v4 after calling remove_copy_if(array, "
			"array + arraySize, v4.begin(), isNegative)");

	int duplicateValues[] = { 1, 1, 1, 1, 2, 3, 2 };
	int duplicateValuesSize = sizeof(duplicateValues)/sizeof(duplicateValues[0]);
	std::vector<int> v5(duplicateValues, duplicateValues + duplicateValuesSize);
	log_to_console(duplicateValues, duplicateValuesSize,
			"\n    duplicateValues contains: ");
	/**
	* std::unique: removes consecutive duplicate values from a range.
	* ForwardIterator unique(ForwardIterator first, ForwardIterator last);
	* std::unique iterates through the range [first, last), and if it finds
	* consecutive values that are equal, removes the the duplicate element,
	* keeping only one. The elements are compared using the operator==.
	* std::unique returns an iterator pointing to the new end (the end of the
	* range without the duplicate values).
	*/
	std::vector<int>::iterator newLastElem = std::unique(v5.begin(), v5.end());
	log_to_console("std::vector<int>::iterator newLastElem = "
			"std::unique(v5.begin(), v5.end());");
	log_to_console(*newLastElem, "*newLastItem = ");
	log_to_console(v5, "v5 after calling unique: ");

	v5.erase(newLastElem, v5.end() );
	log_to_console("v5.erase(newLastElem, v5.end());");
	log_to_console(v5, "v5 after calling erase: ");

	/**
	* std::unique: removes consecutive equivalent elements from a range.
	* ForwardIterator unique(ForwardIterator first, ForwardIterator last,
	* 		BinaryPredicate somePredicate);
	* std::unique iterates through the range [first, last), and if it finds
	* consecutive values for which "somePredicate", returns true, removes the
	* second element, keeping only one.
	* Returns an iterator pointing to the new end (the end of the range without
	* the duplicate values).
	*/
	Employee cheapBob(7, "Bob");
	cheapBob.setSalary(1000);

	//two employees are considered equal if they have the same salary and
	//experience (operator== returns true).

	Employee expensiveJ(8, "John");
	expensiveJ.setSalary(3000);

	Employee expensiveM(8, "Marc");
	expensiveM.setSalary(3000);

	std::vector<Employee> staff;
	staff.push_back(cheapBob);
	staff.push_back(expensiveJ);
	staff.push_back(expensiveM);
	log_to_console(staff, "\n     staff contains: ");

	std::vector<Employee>::iterator new_end = std::unique(staff.begin(),
			staff.end(), equalSalary);

	log_to_console("std::vector<Employee>::iterator new_end = "
			"std::unique(staff.begin(), staff.end(), equalSalary);");
	log_to_console(*new_end, "*new_end = ");
	log_to_console(staff, "staff after calling std::unique: ");

	staff.erase(new_end, staff.end());
	log_to_console("staff.erase(new_end, staff.end());");
	log_to_console(staff, "staff after calling erase: ");


	//size the vector to hold 7 + 3 elements
	std::vector<int> v6(duplicateValuesSize + 3);
	log_to_console(v6, "\n     v6 contains: ");
	log_to_console(duplicateValues, duplicateValuesSize,
			"duplicateValues contains: ");
	/**
	* std::unique_copy: copies a range into another range, removing duplicate
	* values, from the first range.
	* It has two versions:	*
	* 	OutputIterator unique_copy(	InputIterator first, InputIterator last,
	* 								OutputIterator destination);
	*	OutputIterator unique_copy(	InputIterator first, InputIterator last,
	*								OutputIterator result,
	*								BinaryPredicate somePredicate);
	*	The first version compares the elements with operator==.
	*	The second version uses "somePrediacte" to see if two consecutive
	*	elements are equivalent.
	*
	* std::unique_copy copies the elements from first range [first, last)
	* into the range that begins with "destination".
	* If two consecutive elements compare equivalent, only the first one is
	* copied into the destination range.
	* Returns an iterator pointing to the new end of the "destination" range.
	*/
	std::vector<int>::iterator itNewEnd = std::unique_copy(duplicateValues,
			duplicateValues + duplicateValuesSize, v6.begin());

	log_to_console("std::vector<int>::iterator itNewEnd = std::unique_copy"
			"(duplicateValues, duplicateValues + duplicateValuesSize, "
			"v6.begin());");

	log_to_console(*itNewEnd, "*itNewEnd = ");
	log_to_console(v6, "v6 after calling unique_copy: ");

	v6.erase(itNewEnd, v6.end());
	log_to_console("v6.erase(itNewEnd, v6.end());");
	log_to_console(v6, "v6 after calling erase: ");

}

//*****************************************************************************
//						Sorting and operations on sorted ranges
//*****************************************************************************
void sorting_algorithms()
{
	log_to_console("\n");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");
	log_to_console("algorithms for sorting and operations on sorted ranges:"
			"\n     sort, stable_sort, partial_sort, partial_sort_copy,"
			"\n     nth_element, binary_search, "
			"\n     lower_bound, upper_bound, equal_range");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");

	int array[] = { 0, 2, 1, 3 };
	int arraySize = sizeof(array)/sizeof(array[0]);

	std::vector<int> v(array, array + arraySize);
	log_to_console(v, "\n    v contains: ");

	/**
	* std::sort: sorts the elements in a range into ascending order. It
	* requires that the range has random-access iterators.
	* It's not a stable sort. That means that the original order of the
	* equivalent elements is not guaranteed to remain the same, after the
	* sorting operation.
	* It has two versions:
	* 	void sort(RandomAccessIterator first, RandomAccessIterator last);
	* 	void sort(	RandomAccessIterator first,
	* 				RandomAccessIterator last,
	* 				BinaryPredicate somePredicate);
	* 	The first version uses operator< to compare elements.
	* 	The second version uses "somePredicate".
	*/
	std::sort(v.begin(), v.end());
	log_to_console(v, "v after calling sort(v.begin(), v.end()): ");

	Employee staff[] = { Employee(12, "Sam"), Employee(3, "Ham"),
					     Employee(8, "Jam"),  Employee(14, "Alex"),
					     Employee(11, "Jonny")};
	int staffSize = sizeof(staff)/sizeof(staff[0]);
	std::vector<Employee> myStaff(staff, staff + staffSize);
	log_to_console(myStaff, "\n    myStaff contains: ");

	//sort by experience
	std::sort(myStaff.begin(), myStaff.end(), lessExperiencePredicate);
	log_to_console(myStaff, "myStaff after sort(myStaff.begin(), myStaff.end(),"
			" lessExperience): ");


	std::vector<Employee> myOrderedStaff( staff, staff + staffSize );
	log_to_console(myOrderedStaff, "\n    myOrderedStaff contains: ");
	/**
	* std::stable_sort: sorts the elements in a range into ascending order. It
	* requires that the range has random-access iterators.
	* It is a stable sort. The original order of the equivalent elements is
	* guaranteed to remain the same, after the sorting operation.
	* void sort(RandomAccessIterator first, RandomAccessIterator last);
	* void sort(RandomAccessIterator first,
	* 			RandomAccessIterator last,
	* 			BinaryPredicate somePredicate);
	*/
	std::stable_sort(myOrderedStaff.begin(), myOrderedStaff.end(),
			lessExperiencePredicate); //Compare by experience.

	log_to_console(myOrderedStaff, "myOrderedStaff after calling "
			"stable_sort(myOrderedStaff.begin(), myOrderedStaff.end(),"
			" lessExperience): ");


	//experience, name
	Employee dev[] = { 	Employee(8, "Gaston"), Employee(1, "Sissy"),
						Employee(3, "Castor"), Employee(0, "Missy") };
	int devSize = sizeof(dev)/sizeof(dev[0]);
	std::vector<Employee> devStaff(dev, dev + devSize);
	log_to_console(devStaff, "\n    devStaff contains: ");

	/**
	* std::partial_sort: sorts a subrange of a range.
	* It has two versions:
	* 	void partial_sort(	RandomAccessIterator first,
	* 						RandomAccessIterator middle,
	* 						RandomAccessIterator last);
	* 	void partial_sort(	RandomAccessIterator first,
	* 						RandomAccessIterator middle,
	* 						RandomAccessIterator last,
	*  	  	  	  	  		BinaryPredicate somePredicate);
	*
	* 	Sorts the elements from the range [first, last), that can be placed in
	* 	the range [first, middle) in ascending order. The rest of the elements
	* 	are moved after "middle" and their order is not specified.
	* 	The first version uses operator==, to compare elements, the second
	* 	uses "somePredicate", to determine which element is smaller.
	*/
	std::vector<Employee>::iterator middle =
			devStaff.begin() + 2; //middle -> Castor

	log_to_console(*middle, "Calling partial_sort(devStaff.begin(), middle, "
				"devStaff.end(), lessExperience) with *middle = ");
	log_to_console("This will move all the employees with experience less "
				"than 3 years to the beginning of the devStaff vector, and sort"
				" them into ascending order.");

	std::partial_sort( devStaff.begin(), middle, devStaff.end(),
			lessExperiencePredicate); //middle = Castor


	log_to_console(devStaff, "devStaff after partial_sort( devStaff.begin(), "
			"middle, devStaff.end(),lessExperience): ");


	int first_range[] = { 4, 3, 2, 1, 10, 9, 8, 7, 6};
	int firstRangeSize = sizeof(first_range)/sizeof(first_range[0]);
	std::vector<int> second_range(4);
	log_to_console(first_range, firstRangeSize, "\n    first_range contains: ");
	log_to_console(second_range, "secondRange contains: ");
	/**
	* std::partial_sort_copy: sorts the elements from a first range and copies
	* them into a second range. Only the number of elements that fit inside the
	* second range are copied and sorted.
	* It has two versions:
	* 	RandomAccessIterator partial_sort_copy(	InputIterator first,
	* 			InputIterator last,
	* 			RandomAccessIterator destinationFirst,
	*			RandomAccessIterator destinationEnd);
	* 	RandomAccessIterator partial_sort_copy(InputIterator first,
	* 			InputIterator last,
	* 			RandomAccessIterator destinationFirst,
	*  	  	  	RandomAccessIterator destinationEnd,
	*  			BinaryPredicate somePredicate);
	*
	*   The first version sorts the elements using the operator<.
	*   The second version uses "somePredicate".
	*/
	std::partial_sort_copy(first_range, first_range + firstRangeSize,
			second_range.begin(), second_range.end());
	log_to_console(second_range, "second_range after calling partial_sort_copy("
			"first_range, first_range + firstRangeSize, second_range.begin(), "
			"second_range.end()): ");


	char letters[] = { 'b', 'd', 'a', 'c', 'w', 'z' };
	int lettersSize = sizeof(letters)/sizeof(letters[0]);
	log_to_console(letters, "\n    letters contains: ");
	/**
	* std::nth_element: rearranges the elements in a range, so that all the
	* elements smaller than a specified element will be moved at the beginning
	* of the range. Neither subrange is in any particular order, the elements
	* aren't sorted.
	* It has two versions:
	* 	void nth_element(	RandomAccessIterator first,
	* 						RandomAccessIterator nth_element,
	* 						RandomAccessIterator last);
	*
	* 	void nth_element(	RandomAccessIterator first,
	* 						RandomAccessIterator nth_element,
	* 						RandomAccessIterator last,
	*						BinaryPredicate somePredicate);
	*/

	char *nthElem = letters + 1; //'d'
	std::nth_element(letters, nthElem, letters + lettersSize);
	log_to_console(letters, "letters after nth_element(letters, nthElem,"
			" letters + lettersSize): ");
	log_to_console(*nthElem, "*nthElem = ");


	int myArray[] = { 0, 1, 2, 3 };
	int sizeArray = sizeof(myArray)/sizeof(myArray[0]);
	std::vector<int> myVector(myArray, myArray + sizeArray);
	log_to_console(myVector, "\n    myVector contains: ");
	/**
	* binary_search: performs a binary search in a sorted range.
	* It has two versions:
	* 	bool binary_search(ForwardIterator first, ForwardIterator last,
	* 		const T& value);
	* 	bool binary_search(ForwardIterator first, ForwardIterator last,
	* 		const T& value,	BinaryPredicate somePredicate);
	*
	*	Searches in the range [first, last) for "value". It it's found returns
	*	true.
	*	The first version uses operator< for testing if the elements are equal
	*	(!(a<b) && !(b<a)), and assumes that operator< was used to sort the
	*	elements.
	*	The second version uses "somePredicate" to compare elements and assumes
	*	that "somePredicate" was used also for sorting.
	*	The same comparison function has to be used for sorting and for
	*	searching. Otherwise the results are not defined.
	*/


	bool found = std::binary_search(myVector.begin(), myVector.end(), 2);
	log_to_console(found, "binary_search(myVector.begin(), myVector.end(), 2)"
			" returns: ");

	int someValues[] = { 3, 3, 7, 6, 6, 6, 8 };
	int sizeValues = sizeof(someValues)/sizeof(someValues[0]);
	std::vector<int> values(someValues, someValues + sizeValues);
	//lower_bound must be used with sorted ranges. So sort values.
	std::sort(values.begin(), values.end());
	log_to_console(values, "\n    values contains: ");
	/**
	* lower_bound: returns an iterator indicating the first occurrence of a
	* value in a range. If no value was found, an iterator were it would fit
	* is returned.
	* It has two overloads:
	* 	ForwardIterator lower_bound(ForwardIterator first,
	* 			ForwardIterator last, const T& value);
	* 	ForwardIterator lower_bound(ForwardIterator first,
	* 			ForwardIterator last, const T& value,
	*			BinaryPredicate somePredicate);
	*   The first version uses operator< to compare elements.
	*   The second uses "somePredicate".
	*/
	std::vector<int>::iterator lower = std::lower_bound(values.begin(),
			values.end(), 6);

	log_to_console("\n    std::vector<int>::iterator lower = "
			"std::lower_bound(values.begin(), values.end(), 6)");
	log_to_console(*lower, "*lower = ");
	log_to_console(*(lower-1), "*(lower-1) = ");


	/**
	* upper_bound: returns an iterator indicating the first element in a range
	* that is greater than a specified value. If the value is not found, an
	* iterator were it would fit is returned.
	*
	* Two versions:
	* 	ForwardIterator upper_bound(ForwardIterator first,
	* 			ForwardIterator last, const T& value);
	* 	ForwardIterator upper_bound(ForwardIterator first,
	* 			ForwardIterator last, const T& value,
	* 			BinaryPredicate somePredicate);
	* The first version uses operator< to compare elements.
	* The second uses "somePredicate".
	*/
	std::vector<int>::iterator upper = std::upper_bound(values.begin(),
			values.end(), 6);

	log_to_console("\n     std::vector<int>::iterator upper = "
			"std::upper_bound(values.begin(), values.end(), 6)");
	log_to_console(*upper, "*upper = ");
	log_to_console(*(upper + 1), "*(upper + 1) = ");


	/**
	* equal_range: it's a combination or lower_bound and upper_bound. Returns
	* the bounds of the largest subrange that includes all the elements equal
	* to a certain value.
	* It has two versions:
	*	pair<ForwardIterator, ForwardIterator> equal_range(
	*		ForwardIterator first,
	*		ForwardIterator last,
	*		const T& value);
	* 	pair<ForwardIterator, ForwardIterator> equal_range(ForwardIterator first,
	* 			ForwardIterator last, const T& value,
	* 			BinaryPredicate somePredicat);
	* The first version uses operator< to compare elements.
	* The second uses "somePredicate".
	* If the value isn't found in the range, the subrange returned has the
	* length zero. Both iterators will point to the nearest value greater than
	* value. If the value is greater than all the element in the range it
	* returns "last".
	*/
	typedef std::vector<int>::iterator ITER;
	std::pair<ITER, ITER> result = std::equal_range(values.begin(),
			values.end(), 6);

	log_to_console("\n     std::pair<std::vector<int>::iterator, "
			"std::vector<int>::iterator> result = equal_range(values.begin(), "
			"values.end(), 6)");
	log_to_console(*result.first, "*result.first = ");
	log_to_console(*result.second, "*result.second = ");
	log_to_console(*(result.first - 1), "*(result.first - 1) = ");
	log_to_console(*(result.second + 1), "*(result.second + 1) = ");
}

//*****************************************************************************
//								Heap operations
//****************************************************************************
/**
*  A heap is a tree were each node is connected to values smaller than its own
*  value.
*  The first element is always the greatest value in the heap.
*/
void heap_operations()
{
	log_to_console("\n");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");
	log_to_console("heap operations: make_heap, push_heap, pop_heap");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");

	int array[] = { 1, 4, 3, 2 };
	int arraySize = sizeof(array)/sizeof(array[0]);
	std::vector<int> v1(array, array + arraySize);
	log_to_console(v1, "\n    v1 contains: ");

	/**
	*  make_heap: arranges the elements in a range so that they form a heap.
	*  The first element is always the greatest value in the heap.
	*  It has two versions:
	*  	void make_heap ( RandomAccessIterator first, RandomAccessIterator last );
	*  	void make_heap(RandomAccessIterator first, RandomAccessIterator last,
	*  		BinaryPredicate somePredicate);
	*  	The first version compares elements with operator</ The second uses
	* 	"somePredicate" to compare elements.
	*/
	std::make_heap(v1.begin(), v1.end());
	log_to_console(v1, "v1 after make_heap(v1.begin(), v1.end()): ");

	/**
	*  push_heap:
	*   It has two versions: expands a heap range by inserting the last element
	*   into the heap (in the proper place).
	*   It has two variants:
	*  		void push_heap(	RandomAccessIterator first,
	*  						RandomAccessIterator last);
	*  		void push_heap(	RandomAccessIterator first,
	*  						RandomAccessIterator last,
	*  						BinaryPredicate somePredicate);
	*		push_heap places the value "last - 1" at the the proper place in
	*		the heap.
	*   The first version uses operator< to compare the elements and the
	*   second uses "somePredicate".
	*/
	int array2[] = { 0, 7, 6 };
	int sizeArray2 = sizeof(array2)/sizeof(array2[0]);
	std::vector<int> v2(array2, array2 + sizeArray2);
	log_to_console(v2, "\n    v2 contains: ");

	std::make_heap(v2.begin(), v2.end());
	log_to_console(v2, "v2 after make_heap(v2.begin(), v2.end()): ");

	v2.push_back(100);
	log_to_console("\n    v2.push_back(100);");

	std::push_heap(v2.begin(), v2.end());
	log_to_console(v2, "v2 after push_heap(v2.begin(), v2.end()): ");

	/**
	*  pop_heap: places the first element, into the last position in the heap.
	*  It has two versions:
	*   void pop_heap(RandomAccessIterator first, RandomAccessIterator last);
	*   void pop_heap(RandomAccessIterator first, RandomAccessIterator last,
	*   		BinaryPredicate somePredicate);
	*   Places "first" (which has the biggest value) into the position
	*   (last - 1) and rearranges the range so that is still a heap.
	*/
	std::pop_heap(v2.begin(), v2.end());
	log_to_console(v2, "v2 after calling pop_heap(v2.begin(), v2.end())");

	/**
	* sort_heap: arranges the elements in the heap so that they form a sorted
	* range.
	* void sort_heap(RandomAccessIterator first, RandomAccessIterator last);
	* void sort_heap(RandomAccessIterator first, RandomAccessIterator last,
	* 		BinaryPredicate somePredicate);
	*/
	std::make_heap(v2.begin(), v2.end());
	log_to_console(v2, "v2 after calling make_heap(v2.begin(), v2.end())");
}

//*****************************************************************************
//					Applying an operation to each element in a range
//*****************************************************************************
void apply_operations_on_a_range()
{
	log_to_console("\n");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");
	log_to_console("algorithms for applying an operation to each element"
			"\n     in a range: for_each, transform");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");

	/**
	*  A unary function is a function having one argument.
	*/

	Employee someStaff[] = { 	Employee(1, "Bob"), Employee(1, "Bill"),
								Employee(1, "Ben") };
	int staffSize = sizeof(someStaff)/sizeof(someStaff[0]);
	std::vector<Employee> happierStaff(someStaff, someStaff + staffSize);
	log_to_console(someStaff, staffSize, "\n    someStaff");
	log_to_console(happierStaff, "happierStaff contains: ");
	log_to_console("Calling for_each(happierStaff.begin(), happierStaff.end(),"
			"increaseSalary)");

	/**
	* for_each: iterates through a range and applies a operation to each
	* element. The operation can be implemented as a simple function or a
	* function call operator overloaded in a functor.
	* prototype: UnaryFunction for_each(InputIterator first,
	* 									InputIterator last,
	* 									UnaryFunction someFun);
	* Calls "someFun" on each of the elements in the range [first, last).
	* We can supply a functor or a simple function, for "someFun".
	*/
	std::for_each(happierStaff.begin(), happierStaff.end(), increaseSalary);
	log_to_console(happierStaff, "happierStaff after calling "
			"for_each(happierStaff.begin(), happierStaff.end(), increaseSalary)");


	std::vector<Employee> happyStaff;
	happyStaff.resize(happierStaff.size());
	log_to_console(happierStaff, "\n    happierStaff contains: ");
	log_to_console("Calling transform(happierStaff.begin(), happierStaff.end(),"
			"happyStaff.begin(), increaseSalary)");
	/**
	* std::transform: iterates through a range and applies an operation to each
	* element, saving the result in a second range. The operation can be
	* implemented as a simple function or a function call operator overloaded
	* in a functor.
	* It has two versions:
	* 	OutputIterator transform(InputIterator first, InputIterator last,
	* 							 OutputIterator result, UnaryFunction someFun);
	* 	OutputIterator transform(InputIterator1 first1, InputIterator1 last1,
	* 			InputIterator2 first2, OutputIterator destination,
	* 			BinaryFunction someFun);
	*
	*   The first version applies "someFun" to all the elements in the
	*   range [first1,last1) and
	*   stores the returned value in the range beginning at destination.
	*   It performs: *destination++ = someFun(*first++) while first!=last.
	*   The second version uses, as the first argument for the binary function,
	*   an element from the first range,
	*   and as a second argument, an element from the second range. The result
	*   is stored the range that begins with "destination".
	*   It performs: *destination++ = someFun(*first1++, *first2++) while
	*   (first1!=last1).
	*   For both versions "destination" must point to  a type that has an
	*   operator= available (public)).
	*
	* The return value in is the one past the end iterator for the resulting
	* "destination" range.
	*/
	std::transform(happierStaff.begin(), happierStaff.end(),
			happyStaff.begin(), increaseSalary);

	log_to_console(happierStaff, "transform(happierStaff.begin(), "
			"happierStaff.end(), happyStaff.begin(), increaseSalary): ");
}

//*****************************************************************************
//								Numeric algorithms
//*****************************************************************************

// The numeric algorithms are defined in the <numeric> header
void algorithms_for_numeric_op()
{
	log_to_console("\n");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");
	log_to_console("algorithms for numeric operations: accumulate, "
			"\n     inner_product, partial_sum, adjacent_difference");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");


	int array[] = { 1, 2, 1 };
	int arraySize = sizeof(array)/sizeof(array[0]);
	log_to_console(array, arraySize, "\n    array contains: ");
	/**
	* accumulate: returns the result of accumulating all the values in a range.
	* It has two versions:
	* 	T accumulate(InputIterator first, InputIterator last, T initialValue);
	* 	T accumulate(InputIterator first, InputIterator last, T initialValue,
	* 		BinaryFunction someFun);
	*   The first version uses operator+.
	*   It performs:
	*   	result = initialValue;
	*   	while( first != last) {
	*   		result = result + *first++; }
	*   The second version performs:
	*   	result = initialValue;
	*   	while( first != last) {
	*   		result = someFun(result, *first);
	*   		++first; }
	*/
	int res = std::accumulate(array, array + arraySize, 5); //1 + 2 = 3;
															//3 + 1 = 4
//	log_to_console("1 + 2 = 3; 3 + 1 = 4");
	log_to_console(res, "accumulate(array, array + arraySize, 5) returns: ");

	int array2[] = { -1, -2, -1 };
	log_to_console(array2, arraySize, "\n    array2 contains: ");
	log_to_console(array, arraySize, "array contains: ");
	/**
	* inner_product: computes cumulative product of a range.
	* It has two versions:
	* 	T inner_product(InputIterator1 first1, InputIterator1 last1,
	* 					InputIterator2 first2, T initialValue);
	* 	T inner_product(InputIterator1 first1, InputIterator1 last1,
	* 					InputIterator2 first2, T initialValue,
	* 					BinaryFunction1 someFun1, BinaryFunction2 someFun2);
	*   The first version performs:
	*   	result = initialValue;
	*   	while( first1 != last1){
	*   	result = result + (*first1) * (*first2); }
	*   	e.g: for two arrays: { 1, 2, 3 }, { 10, 20, 30 } and initial value
	*   	of -9, the inner product is: -9 + ( 1*10 + 2*20 + 3*30 ).
	*
	*   The second version uses "someFun1" instead of operator+ and "someFun2"
	*   instead of the multiply operator ("*").
	*/
	res = std::inner_product(array, array + arraySize,  array2, 0);
//	log_to_console("0 + 1*(-1) + 2*(-2) + 1*(-1)");
	log_to_console(res, "inner_product(array, array + arraySize,  array2, 0) "
			"returns: ");

	res = std::inner_product(array, array +arraySize, array2, 6 );
	log_to_console(res, "inner_product(array, array +arraySize, array2, 6 )");


	int array3[] = { 1, 2, 3 };
	int array3_size = sizeof(array3)/sizeof(array3[0]);
	std::vector<int> v3;
	v3.resize(array3_size);
	log_to_console(array3, array3_size, "\n    array3 contains: ");
	log_to_console(v3, "v3 contains: ");
	/**
	*
	* partial_sum: computes the partial sum of the elements in a range.
	* It has two versions:
	* 	OutputIterator partial_sum(	InputIterator first, InputIterator last,
	* 								OutputIterator destination);
	* 	OutputIterator partial_sum(InputIterator first, InputIterator last,
	* 								OutputIterator destination,
	*							  	BinaryFunction someFun );
	*	Assigns to every element in the range, starting at destination,
	*			the partial sum of the corresponding
	*	elements in the range, like this:
	*			*destination++ = *first;
	*			*destination++ = *first + *(first + 1);
	*			*destination++ = *first + *(first + 1) + *(first + 2);
	*			*destination++ = *first + *(first + 1) + *(first + 2) +
	*							 *(first + 3);
	*			ect. Performs this until reaches at the end of the first
	*			range (last).
	*	The first version uses operator+ to sum the elements.
	*	The second version uses "someFun" instead of operator+.
	*/
	std::partial_sum(array3, array3 + array3_size, v3.begin()); //v3 = {1, 1+2,
																//1+2+3 };
	log_to_console(v3, "v3 after calling partial_sum(array3, array3 + "
			"array3_size, v3.begin())");


	std::vector<int> v4;
	v4.resize(array3_size);
	log_to_console(v4, "\n    v4 contains: ");
	log_to_console(array3, array3_size, "array3 contains: ");
	/**
	* adjacent_difference: iterates through a range and computes the difference
	* of adjacent elements.
	* It has two versions:
	* 	OutputIterator adjacent_difference(	InputIterator first,
	* 										InputIterator last,
	* 										OutputIterator destination);
	* 	OutputIterator adjacent_difference(	InputIterator first,
	* 										InputIterator last,
	* 										OutputIterator destination,
	* 										BinaryFunction someFun);
	*
	* 	Iterates through [first, last) and computes the difference of the
	* 	adjacent elements (current_element - previous_element).
	* 	It performs:
	* 		*destination++ = *first;
	* 		*destination++ = *(first+1) - *first;
	* 		*destination++ = *(first+2) - *(first+1);
	* 		*destination++ = *(first+3) - *(first+2);
	* 		Iterates like this until reaches the end of the first range(last).
	* 	The second form uses "someFun" instead of operator-.
	*/
	std::adjacent_difference(array3, array3 + array3_size, v4.begin());

	//array3[] = { 1, 2, 3 };
	//v4 will contain: 1, 2-1, 3-1
	log_to_console(v4, "v4 after calling adjacent_difference(array3, "
			"array3 + array3_size, v4.begin()): ");
}

//*****************************************************************************
//								General utilities
//*****************************************************************************
void general_utilities()
{
	log_to_console("\n");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");
	log_to_console("general utilities: distance, front_inserter, inserter, "
			"back_inserter");
	log_to_console("----------------------------------------------------------"
				   "---------------------------------------------------------");

	int longArray[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
			17, 18, 19, 20 };
	int longArraySize = sizeof(longArray)/sizeof(longArray[0]);
	log_to_console(longArray, longArraySize, "\n    longArray contains: ");

	/**
	*  std::distance: returns the number of elements in range.
	*  int distance(InputIterator first, InputIterator last);
	*/
	int numElem = std::distance(longArray, longArray + 3);
	log_to_console(numElem, "\n    distance(longArray, longArray + 3) returns: ");

	numElem = std::distance(longArray, longArray + 14);
	log_to_console(numElem, "distance(longArray, longArray + 14) returns: ");

	numElem = std::distance(longArray, longArray + 0);
	log_to_console(numElem, "distance(longArray, longArray + 0)");

	numElem = std::distance(longArray, longArray + 20);
	log_to_console(numElem, "distance(longArray, longArray + 20)");

	/**
	*  back_inserter: returns a "back_insert_iterator" for a  container.
	*  The "back_insert_iterator" expands the container automatically, calling
	*  the "push_back" function of the container. It is used to insert elements
	*  into a container, rather than overwrite the existing elements, using
	*  operator=.
	*  It is useful with algorithms that overwrite elements like, copy.
	*  The back_insert_iterator class has its operator= defined like this:
	*  		back_insert_iterator<Container>& operator=(Container::value_type &v)
	*  		{
	* 					container->push_back(value);
	*					return *this;
	*		}
	*		So when *iterator = someValue is called, instead of an assignment,
	*		a push_back operation takes place.
	*
	*  prototype: back_insert_iterator<Container> back_inserter(Container& x);
	*/

	int chars[] = { 'a', 'b', 'c' };
	int charsNumber = sizeof(chars)/sizeof(chars[0]);
	log_to_console(chars, charsNumber, "\n    chars contains: ");

	//two variants to copy the "chars" array into a vector:

	std::vector<char> myVector;
	//resize the vector because std::copy increments the iterator we
	//supply(myVector.begin()) and assigns values to it.
	//The vector won't expand automatically so we have to set the proper size.
	myVector.resize(charsNumber);
	log_to_console(myVector, "myVector contains: ");

	std::copy(chars, chars + charsNumber, myVector.begin());
	log_to_console(myVector, "myVector after calling copy(chars, "
			"chars + charsNumber, myVector.begin()): ");

	std::vector<char> mySecondVector;
	log_to_console((int)mySecondVector.size(), "mySecondVector.size() = ");
	//we supply a back_insert_iterator to std::copy. That means that the
	//back_insert_iterator will expand the vector automatically, as we copy
	//elements in it.
	//(std::copy will perform *iterator++ = elementValue, the
	//back_insert_iterator::operator= will be called, and this will call
	//push_back on the container).
	std::copy(chars, chars + charsNumber, std::back_inserter(mySecondVector));
	log_to_console(mySecondVector, "mySecondvector after calling "
			"copy(chars, chars + charsNumber, "
			"std::back_inserter(mySecondVector))");

	/**
	*  front_inserter: returns a "front_insert_iterator" for a container.
	*  The "front_insert_iterator" expands the container automatically, calling
	*  the "push_front" function of the container. It is used to insert
	*  elements into a container, rather than overwrite the existing elements,
	*  using operator=. It is useful with algorithms that overwrite elements
	*  like, copy.
	*  The front_insert_iterator class has its operator= defined like this:
	*  front_insert_iterator<Container>& operator=(Container::value_type &value)
	*  {
	* 		container->push_front(value);
	*		return *this;
	*  }
	*  So when *iterator = someValue is called, instead of an assignment, a
	*  push_front operation takes place.
	*
	*  prototype: front_insert_iterator<Container>  front_inserter(Container& x);
	*/
	std::deque<char> d;
	d.push_back('d');
	d.push_back('e');
	log_to_console(d, "\n    d contains: ");

	char abc[] = { 'c', 'b', 'a' };
	int abcSize = sizeof(abc)/sizeof(abc[0]);
	std::copy( abc, abc + abcSize, front_inserter(d));
	log_to_console(abc, abcSize, "abc after calling "
		"std::copy( abc, abc + abcSize, front_inserter(d)) contains: ");


	/**
	*  inserter: returns a "insert_iterator" for a container.
	*  The "insert_iterator" expands the container automatically, calling the
	*  "insert" function of the container. It is used to insert elements into
	*  a container, rather than overwrite the existing elements, using
	*  operator=. It is useful with algorithms that overwrite elements
	*  like, copy.
	*  The insert_iterator class has its operator= defined like this:
	*  insert_iterator<Container>& operator= (Container::value_type &value)
	* { 				iter = container->insert(iter, value);
	*					++iter;
	*					return *this;
	* }
	*		So when *iterator = someValue is called, instead of an assignment,
	* a push_front operation takes place.
	*
	*  prototype: insert_iterator<Container> inserter(	Container& x,
	*  													Inserter iter);
	*/
	char someChars[] = { 'a', 'b', 'e', 'f' };
	int someCharsSize = sizeof(someChars)/sizeof(someChars[0]);
	std::vector<char> myChars(someChars, someChars + someCharsSize);
	std::vector<char>::iterator iter = myChars.begin() + 2;
	log_to_console(myChars, "\n     myChars contains: ");
	log_to_console(*iter, "*iter = ");

	char missingChars[] = { 'c', 'd' };
	int missingCharsSize = sizeof(missingChars)/sizeof(missingChars[0]);
	log_to_console(missingChars, missingCharsSize, "missingChars contains: ");
	//insert c  and d after b
	std::copy(missingChars, missingChars + missingCharsSize, std::inserter
			(myChars, iter));
	log_to_console(myChars, "myChars after calling: copy(missingChars, "
			"missingChars + missingCharsSize, std::inserter(myChars, iter))");
}


}//end namespace

void STLMoblet::STL_algorithms()
{
	LOG("\n");
	LOG("========================= STL algorithms ========================================================================");
	LOG("/**");
	LOG("*  STL algorithms are defined in the <algorithm> header.");
	LOG("*  They are function templates,implementing algorithms for sorting, filling,");
	LOG("*  searching containers,");
	LOG("*  comparing ranges, copying ranges ect.");
	LOG("*  They can be used with any type of container (STL container or not),");
	LOG("*  that provides the proper iterator and holds elements that overload the");
	LOG("*  operators required by the algorithm.");
	LOG("*  For example the count algorithm, compares every element in a container");
	LOG("*  with a value we provide.");
	LOG("*  For comparison it uses operator==, so we have to have inside the container");
	LOG("*  a type that has an operator== defined.");
	LOG("*");
	LOG("*  For what is an ForwardIterator, BidirectionalIterator ect. see Iterators.cpp");
	LOG("*/");
	LOG("\n");

	LOG("				Example using algorithms 						  ");

	filling_algorithms();
	generating_algorithms();
	counting_algorithms();
	algorithms_for_modifying_sequences();
	searching_algorithms();
	algorithms_for_comparing();
	algorithms_for_replacing();
	algorithms_for_removing();
	sorting_algorithms();
	heap_operations();
	apply_operations_on_a_range();
	algorithms_for_numeric_op();
	general_utilities();

	LOG("\n");
}
