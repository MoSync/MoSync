/*
 * Algorithms.cpp
 *
 *  Created on: Jun 4, 2011
 *      Author: gabi
 */

#include <vector>
#include <deque>
#include <string>
#include <algorithm>
#include <numeric> 		//for numeric algorithms
#include "../TestSTL.h"
#include "../LOG.h"
#include "../Employee.h"

namespace{

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

//*****************************************************************************************
//								Filling
//*****************************************************************************************
void filling_algorithms()
{
	/**
	*  std::fill algorithm: fills a range with a value.
	*  Prototype: void fill(ForwardIterator first, ForwardIterator last, const T& val);
	*  assigns to all elements in the range [first, last) the value "val".
	*/
	std::vector<int> v;
	v.resize(4);
	std::fill(v.begin(), v.end(), 99); //performs: *first = 99; ++first; *first = 99; ect,  while first<last;

	TESTIFY_ASSERT( v[0]==99 && v[1]==99 && v[2]==99 && v[3]==99 );

	/**
	*  std::fill_n algorithm: assigns a value to a number of elements, in a range.
	*  prototype: void fill_n(OutputIterator first, Size n, const T& val);
	*  std::fill_n assigns "val" to "n" elements starting at "first".
	*/
	std::fill_n(v.begin(), 2, 1001); //performs: *first = 1001; ++first; *first = 1001; for "n" iterations
	TESTIFY_ASSERT( v[0]==1001 && v[1]==1001 && v[2]==99 && v[3]==99 );

}

//*****************************************************************************************
//								Generating
//*****************************************************************************************

int generatea_even_numbers()
{
	static int num = 0;
	int temp = num;
	num += 2;
	return temp;
}
int generatea_odd_numbers()
{
	static int num = 1;
	int temp = num;
	num += 2;
	return temp;
}

void generating_algorithms()
{
	std::vector<int> v(4);
	/**
	* std::generate algorithm: generates values for a range with a function
	* prototype: void generate(ForwardIterator first, ForwardIterator last, Generator someGen);
	* calls someGen() for each element in the range, and assigns the value returned by someGen()
	* to the element.
	*/
	std::generate(v.begin(), v.end(), generatea_even_numbers);
	TESTIFY_ASSERT( v[0]%2==0 && v[1]%2==0 && v[2]%2==0 && v[3]%2==0 );

	/**
	* std::generate_n algorithm: assigns the values, returned by a function, to a number of elements
	* in a range.
	* prototype: void generate_n(OutputIterator first, Size n, Generator someGen);
	* std::generate_n calls someGen() "n" times, for each of the "n" elements, and
	* assigns the value returned by the function to the current element.
	*/
	std::generate_n(v.begin(), 2, generatea_odd_numbers);
	TESTIFY_ASSERT( v[0]%2!=0 && v[1]%2!=0 && v[2]%2==0 && v[3]%2==0 );

	std::vector<int> v2;
	//back_inserter() inserts the values at the end of v2 (and expands v2 automatically).
	std::generate_n(back_inserter(v2), 3, generatea_even_numbers);
	TESTIFY_ASSERT( v2.size() == 3 );
	TESTIFY_ASSERT( v2[0]%2==0 && v2[1]%2==0 && v2[2]%2==0 );
}

//*****************************************************************************************
//								Counting
//*****************************************************************************************

bool isLessThan(int val)
{
	return val < 303;
}

void counting_algorithms()
{
	/**
	* std::count: counts the number of elements, from a range, that are equivalent to a value.
	* prototype: int count(InputIterator first, InputIterator last, const T& val);
	* counts the number of elements that are equal to "val".
	* operator== is used for testing the equivalence, so the type of elements in the range must have
	* an operator== defined.
	*/
	int array[] = { 0, 2, 303, 303, 303, 4, 5 };
	int arraySize = sizeof(array)/sizeof(array[0]);
	int res = std::count(array, array + arraySize, 303);
	TESTIFY_ASSERT( 3 == res );

	/**
	* std::count_if: counts the number of elements in a range that satisfy a condition.
	* prototype: int count_if(InputIterator first, InputIterator last, Predicate somePred ); //see AboutPredicates.cpp for what is a predicate
	* counts the number of elements for which somePred returns true.
	*/

	res = std::count_if(array, array + arraySize, isLessThan);
	TESTIFY_ASSERT( res == 4 ); // < 303: 0, 2, 4, 5 ( four elements )
}


//*****************************************************************************************
//							Modifying sequences
//*****************************************************************************************
void algorithms_for_modifying_sequences()
{
	int array[] = { 10, 15, 25 };
	int arraySize = sizeof(array)/sizeof(array[0]);
	/**
	* std::copy: copies (using assignment) a range into another range.
	* OutputIterator copy(InputIterator first, InputIterator last, OutputIterator destination);
	* copies the elements in the range [first, last), into the range that starts with "destination"
	*/
	std::vector<int> v(arraySize);
	std::copy(array, array + arraySize, v.begin()); //*destination = 10; ++destination; *destination = 15 ect;

	TESTIFY_ASSERT( v[0] == 10 && v[1] == 15 && v[2] == 25);
	/**
	*std::copy_backward: copies a range of elements into another range, backwards.
	*prototype: BidirectionalIterator2 copy_backward(BidirectionalIterator1 first,
	*	 									BidirectionalIterator1 last,
 	  	  	  	  	  	  	  	  	  	  	BidirectionalIterator2 destinationEnd);
 	* copies starting with *(--destinationEnd) = *(--last) and iterates until reaches "first", coping also "first".
	*/
	std::vector<int> vb(arraySize);
	std::copy_backward(array, array + arraySize, vb.end());  // performs: *--destinationEnd = 25; --destinationEnd = 15 ect;
	TESTIFY_ASSERT( vb[0] == 10 && vb[1] == 15 && vb[2] == 25 );

	/**
	* std::reverse: reverses a range
	* prototype: void reverse(BidirectionalIterator first, BidirectionalIterator last);
	*/
	std::reverse( vb.begin(), vb.end() );
	TESTIFY_ASSERT( vb[0] == 25 && vb[1] == 15 && vb[2] == 10 );

	/**
	* std::reverse_copy: copies the elements from a range, in reversed order, into another range
	* prototype: void reverse_copy(BidirectionalIterator first, BidirectionalIterator last,  OutputIterator destination );
	*/
	std::vector<int> rev_copy(arraySize);
	std::reverse_copy(array, array + arraySize, rev_copy.begin());
	TESTIFY_ASSERT( vb[0] == 25 && vb[1] == 15 && vb[2] == 10 );

	/**
	* std::swap_ranges: exchange the contents of two ranges. The ranges have to be of equal size.
	* ForwardIterator2 swap_ranges(ForwardIterator1 first1,  ForwardIterator1 last1, ForwardIterator2 first2);
	* The elements from first range [first1, last1) will be in the second range, and the elements from the
	* second range will be in the first range.
	*/
	int first_range[] = {  11, 12, 13 };
	int second_range[] = { 21, 22, 23 };
	int rangeSize = sizeof(first_range)/sizeof(first_range[0]);

	std::swap_ranges(first_range, first_range + rangeSize, second_range);

	TESTIFY_ASSERT( first_range[0]  == 21 && first_range[1]  == 22 && first_range[2]  == 23);
	TESTIFY_ASSERT( second_range[0] == 11 && second_range[1] == 12 && second_range[2] == 13);

	/**
	* std::rotate: rotates elements in a range
	* void rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last);
	* Moves the contents of [first, middle) to the end of the range, and the contents
	* of [middle, last) to the beginning. [first, last) becomes [middle, last) + [first, middle).
	*/
	std::string strArray[] = { "first", "second", "I am middle", "I am another value"};
	int strArraySize = sizeof(strArray)/sizeof(strArray[0]);

	std::rotate(strArray, strArray + 2, strArray + strArraySize);
	TESTIFY_ASSERT( strArray[0] == "I am middle");
	TESTIFY_ASSERT( strArray[1] == "I am another value");
	TESTIFY_ASSERT( strArray[2] == "first");
	TESTIFY_ASSERT( strArray[3] == "second");

	/**
	* std::rotate_copy:
	* OutputIterator rotate_copy( ForwardIterator first, ForwardIterator middle, ForwardIterator last,
  	  	  	  	  	  	  	  	  OutputIterator destination);
  	* Copies the range composed by [middle, last) +  [first, middle) into the range that begins with "destination".
	*/
	int myArray[] = { 40, 50, 60, 10, 20, 30 };
	int sizeOfArray = sizeof(myArray)/sizeof(myArray[0]);
	std::vector<int> myVector(sizeOfArray);

	std::rotate_copy(myArray, myArray + 3, myArray + sizeOfArray, myVector.begin());  //the middle element is 10
	TESTIFY_ASSERT( myVector[0] == 10  &&  myVector[1] == 20 && myVector[2] == 30 &&
					myVector[3] == 40  &&  myVector[4] == 50 && myVector[5] == 60 );


	/**
	*  	A permutation of a set of elements means an arrangement of those elements into a particular oder.
	*   For n elements in a range, we will have n! (n factorial) permutations. That is n ways to arrange the
	*   elements.
	*   All these combinations can be sorted in a dictionary-like (or lexicographically) manner.
	*   The smallest permutation is considered the one in which all elements are sorted into ascending order. The biggest
	*   permutation is the one that has all its elements sorted into descending order.
	*   	e.g: for { 1, 2, 3 } we have 3!=6 permutations:
 	*			-> first permutation: 1, 2, 3,  (smallest permutation)
 	*			-> next permutation:  1, 3, 2
 	*			-> next permutation:  2, 1, 3
 	*			-> ect
 	*			-> last permutation:  3, 2, 1 (biggest permutation)
	*/

	/**
	* std::next_permutation: rearranges the elements from a range into the next (greater) permutation.
	* If successful, returns true. If there are no more permutations, returns false.
	* It has two versions:
	* 	bool next_permutation(BidirectionalIterator first, BidirectionalIterator last);
	* 	bool next_permutation(BidirectionalIterator first, BidirectionalIterator last, BinaryPredicate somePred);
	*  The first version uses operator< for comparing the elements in the range.
	*  The second version uses binary_pred for comparing the elements in the range.
	*  Compares the elements in the range, so that it knows when the elements are sorted into descending
	*  order, and therefore it reached the "biggest" permutation (and no next permutation exists). The same
	*  for the "smallest" permutation, and the range between the biggest and smallest permutation.
	*/
	char chars[] = { 'a', 'b' };
	int sizeOfChars = sizeof(chars)/sizeof(*chars);
	std::next_permutation(chars, chars + sizeOfChars);
	TESTIFY_ASSERT( chars[0]=='b' && chars[1]=='a');

	/**
	* std::prev_permutation: rearranges the elements from a range into the previous (smaller) permutation.
	* If successful, returns true. If there are no more permutations, returns false.
	* It has two versions:
	* 	bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last);
	* 	bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last, BinaryPredicate somePred);
	* 	The first version uses operator< for comparing the elements in the range.
	*   The second version uses somePred for comparing the elements in the range.
	*/
	std::prev_permutation(chars, chars + sizeOfChars);
	TESTIFY_ASSERT( chars[0]=='a' && chars[1]=='b');


	int orderedArray[] = {0, 1, 2, 3, 4, 5, 6, 7};
	int sizeOrderedArray = sizeof(orderedArray)/sizeof(orderedArray[0]);

	/**
	* std::random_shuffle: function rearranges the elements in a range in a random way.
	* void random_shuffle(RandomAccessIterator first, RandomAccessIterator last);
	*/
	std::vector<int> rVector(orderedArray, orderedArray+ sizeOrderedArray);
	std::random_shuffle(rVector.begin(), rVector.end());
	LOG("\n\nrandom_shuffle result:\n{");
	for(int i=0; i< rVector.size(); ++i)
	{
		LOG("%d ", rVector[i]);
	}
	LOG("};\n");

	/**
	* std::partition: moves the elements that satisfy a certain condition, to the beginning of the range.
	* prototype: BidirectionalIterator partition(BidirectionalIterator first, BidirectionalIterator last, Predicate somePred);
	* Moves the elements for which "somePred" returns true at the beginning of the range.
	* std::partition returns an iterator pointing to the one past end of the range that satisfies "somePredicate".
	* The relative order of elements after the partition is not guaranteed to be the same, as before the partition.
	*/

	std::vector<int> pVector(orderedArray, orderedArray+ sizeOrderedArray);
	std::partition(pVector.begin(), pVector.end(), NumericUtilities::isEven);

	LOG("\npartition result:\n{");			//{0, 1, 2, 3, 4, 5, 6} => {0, 6, 2, 4, 3, 5, 1, 7}
	for(int i=0; i< pVector.size(); ++i)
	{
		LOG("%d ", pVector[i]);
	}
	LOG("};\n\n");

	/**
	* std::stable_partition: moves the elements that satisfy a certain condition, to the beginning of the range.
	* prototype: BidirectionalIterator partition(BidirectionalIterator first, BidirectionalIterator last, Predicate somePred);
	* Moves the elements for which "somePred" returns true, at the beginning of the range.
	* std::partition returns an iterator pointing to the one past end of the range that satisfies "somePredicate".
	* The relative order of elements after the partition is the same, as before the partition.
	*/
	std::vector<int> sVector(orderedArray, orderedArray+ sizeOrderedArray);
	std::vector<int>::iterator result = std::stable_partition(sVector.begin(), sVector.end(), NumericUtilities::isEven);

	TESTIFY_ASSERT(*result == 1);
	LOG("\nstable_partition result:\n{");			//{0, 1, 2, 3, 4, 5, 6} => {0, 2, 4, 6, 1, 3, 5, 7}
	for(int i=0; i< sVector.size(); ++i)
	{
		LOG("%d ", sVector[i]);
	}
	LOG("};\n\n");

}

//*****************************************************************************************
//						Searching and replacing
//*****************************************************************************************
void searching_algorithms()
{
	Employee staff[] = { Employee(4, "Bob"), Employee(8, "Ion"), Employee(10, "Anna") };
	int staffSize = sizeof(staff)/sizeof(staff[0]);

	int array[] = { 1, 6, -4, -4, -4, 2, 5, 3, 0 };
	int arraySize = sizeof(array)/sizeof(array[0]);

	/**
    * std::find: searches inside a range for a value.
	* prototype: InputIterator find(InputIterator first, InputIterator last, const T& value);
	* If it finds an element equal with "value"  it returns an iterator pointing to that element.
	* If doesn't find any element, it returns last.
	* An operator== must be available for comparing the elements in the range with the value.
	*/
	std::vector<Employee> v1(staff, staff + staffSize);
	std::vector<Employee>::iterator result1 = std::find(v1.begin(), v1.end(), Employee(4, "Bob")); //calls operator== for every element and the value.
	TESTIFY_ASSERT(result1 != v1.end()); //found Bob.

	result1 = std::find(v1.begin(), v1.end(), Employee(0, "Maria"));
	TESTIFY_ASSERT(result1 == v1.end()); //didn't found Maria.

	/**
	* std::find_if: searches for a value in a range, and returns the first occurrence of that value.
	* prototype: InputIterator find_if(InputIterator first, InputIterator last, Predicate pred);
	* If it finds an element for which pred(element, value) returns true it returns an iterator pointing
	* to that element. If doesn't find any element, it returns last.
	*/
	std::vector<int> v2(array, array + arraySize); //fill the vector with: { 1, 6, 4, 4, 4, 2, 5, 3, 0};
	std::vector<int>::iterator result2 = std::find_if(v2.begin(), v2.end(), NumericUtilities::isEven);
	TESTIFY_ASSERT(*result2 == 6);

	/**
	* std::adjacent_find searches for two adjacent elements that are equal.
	* It has two versions:
	* 	ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last);
	* 	ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, BinaryPredicate somePredicate);
	* 	The first version uses operator== to test for equality, the second uses "somePredicate".
	* 	Returns an iterator pointing to the first element, if it finds two equal elements. If it doesn't find
	* anything, returns last.
	*/
	std::vector<int> v3(array, array + arraySize); //fill the vector with: { 1, 6, -4, -4, -4, 2, 5, 3, 0};
	std::vector<int>::iterator result3 = std::adjacent_find(v3.begin(), v3.end());
	TESTIFY_ASSERT(*result3 == -4);
	TESTIFY_ASSERT(*(++result3) == -4);

	/**
	* std::find_first_of: searches for an element in the second range that is equal to one in the first.
	* It has two versions:
	*	ForwardIterator1 find_first_of( ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2);
	* 	ForwardIterator1 find_first_of( ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2,
	* 								  BinaryPredicate somePredicate);
	*
	* 	The first version tests for equality with the help of operator== (calls operator==(firstRangeElement, secondRangeElement)).
	* 	The second version uses "somePredicate" to test for equality (calls somePredicate(firstRangeElem, secondRangeElem).
	* Returns an iterator pointing to the found element (from the first range), or last2, if it doesn't find anything.
	*/
	int first_array[]  = {  1, 2, 3, 4, 5, 6 };
	int second_array[] = {  2, 3, 4 };
	int firstArraySize = sizeof(first_array)/sizeof(first_array[0]);
	int secondArraySize = sizeof(second_array)/sizeof(second_array[0]);

	std::vector<int> vFirst(first_array, first_array + firstArraySize);
	std::vector<int> vSecond(second_array, second_array + secondArraySize);
	std::vector<int>::iterator resFind = std::find_first_of(vFirst.begin(), vFirst.end(), vSecond.begin(), vSecond.end());
	TESTIFY_ASSERT(*resFind == 2);
	TESTIFY_ASSERT(*(--resFind) == 1); //std::find_first_of returns an iterator from the first range


	/**
	* std::search:  checks if the first range contains the second range.
	* It has two versions:
	* 	ForwardIterator1 search(	ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2);
	* 	ForwardIterator1 search(	ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2
	* 								BinaryPredicate somePredicate);
	* 	Searches the range [first1,last1) for the first occurrence of the range [first2,last2).
	*   The first version tests for equality with operator==. The second uses "somePredicate".
	* If the search is successful returns an iterator pointing to the first point in the first range where the
	* second range begins. If not, returns last1.
	*/
	std::vector<int> range(first_array, first_array + firstArraySize);		 //fill the vector with:{ 1, 2, 3, 4, 5, 6 };
	std::vector<int> subrange(second_array, second_array + secondArraySize); //fill subrange with: { 2, 3, 4 };
	std::vector<int>::iterator resSearch = std::search(range.begin(), range.end(), subrange.begin(), subrange.end());
	TESTIFY_ASSERT(*resSearch == 2);
	TESTIFY_ASSERT(*(++resSearch) == 3);
	TESTIFY_ASSERT(*(++resSearch) == 4);
	TESTIFY_ASSERT(*(++resSearch) == 5);	//std::search returns an iterator from the first range

	/**
	* std::search_n: searches a group of n consecutive values, in a range, that are equal to a value.
	* It has two versions:
	* 	ForwardIterator search_n(ForwardIterator first, ForwardIterator last, int n, const T& value);
	* 	ForwardIterator search_n(ForwardIterator first, ForwardIterator last, int n, const T& value,
  	*  							BinaryPredicate somePredicate);
	*	Searches n consecutive values in [first, last) that are equal to "value".
	*	The first version uses operator== to compare the elements in the range with the "value". The
	*	second version uses "somePredicate" to test for equality.
	*	Returns an iterator pointing to first element equal to "value". If no element is equal to
	*	"value" returns "last".
	*/
	std::vector<int> myRange(array, array + arraySize);	//fills the vector with { 1, 6, -4, -4, -4, 2, 5, 3, 0 };
	std::vector<int>::iterator search_nResult = std::search_n(myRange.begin(), myRange.end(), 3, -4);
	TESTIFY_ASSERT(*search_nResult == -4);

	/**
	* std::find_end: searches a subrange inside a range. Searches for the last occurrence of the subrange.
	* Two versions:
	* 	ForwardIterator1 find_end( ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2);
	* 	ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2,
	* 							BinaryPredicate somePredicate);
	* 	Searches the range [first1,last1) for the last occurrence [first2,last2).
	* 	The first version uses operator==, to test for equality between two elements. The second uses "somePredicate"
	*   to check if two elements are equal.
	* If the search is successful returns an iterator pointing to the last point in the first range where the
	* second range begins. If not, returns last1.
	*/
	int first_range[] = { 1, 2, 3, -900, 1, 2, 3, -800 };
	int second_range[] = { 1, 2, 3 };
	int firstRangeSize = sizeof(first_range)/sizeof(first_range[0]);
	int secondRangeSize = sizeof(subrange)/sizeof(subrange[0]);
	int *find_end_result = std::find_end(first_range, first_range + firstRangeSize, second_range, second_range + secondRangeSize);
	TESTIFY_ASSERT(*find_end_result == 1 );
	TESTIFY_ASSERT(*(--find_end_result) == -900 );

	/**
	* std::min_element: returns the smallest element in a range.
	* It has two versions:
	* 	ForwardIterator min_element(ForwardIterator first,  ForwardIterator last);
	* 	ForwardIterator min_element(ForwardIterator first,  ForwardIterator last, BinaryPredicate somePredicate);
	*   Returns the first occurrence of the smallest element in the range [first, last).
	*   The version uses the operator< to compare elements. The second version uses "somePredicate".
	*/
	int someRange[] = { 3, 1, 1, 2, 55, 55 };
	int rangeSize = sizeof(someRange)/sizeof(someRange[0]);
	int *minElement = std::min_element(someRange, someRange + rangeSize);
	TESTIFY_ASSERT(*minElement == 1 );
	TESTIFY_ASSERT(*(--minElement) == 3 ); //first occurrence of "1" is returned => previous element = 3

	/**
	* std::max_element: returns the biggest element in a range.
	* It has two versions:
	* 	ForwardIterator max_element(ForwardIterator first, ForwardIterator last);
	* 	ForwardIterator max_element(ForwardIterator first, ForwardIterator last, BinaryPredicate binary_pred);
	*   Returns the first occurrence of the biggest element in the range [first, last).
	*   The version uses the operator< to compare elements. The second version uses "somePredicate".
	*/
	int *maxElement = std::max_element(someRange, someRange + rangeSize);	//someRange: { 3, 1, 1, 2, 55, 55 };
	TESTIFY_ASSERT(*maxElement == 55 );
	TESTIFY_ASSERT(*(--maxElement) == 2 ); //first occurrence of "55" is returned => previous element = 2
}

//*****************************************************************************************
//						Replacing elements in a range
//*****************************************************************************************
void algorithms_for_replacing()
{
	int array [] = { -77, -77, 2, 2, 3, 3};
	int arraySize = sizeof(array)/sizeof(array[0]);

	/**
	* std::replace algorithm: replaces all the values in a range with a new value.
	* prototype: void replace(ForwardIterator first, ForwardIterator last, const T& oldValue, const T& newValue);
	*/
	std::vector<int> v1(array, array + arraySize);
	std::replace(v1.begin(), v1.end(), -77, 1);
	TESTIFY_ASSERT( v1[0] == 1 && v1[1] == 1 );


	/**
	* std::replace_if: replaces all the elements in a range, that satisfy a certain condition, with a new value.
	* prototype: void replace_if(ForwardIterator first, ForwardIterator last, Predicate somePred, const T& newValue);
	*/
	std::vector<int> v2(array, array + arraySize);
	std::replace_if(v2.begin(), v2.end(), NumericUtilities::isNegative, 1);
	TESTIFY_ASSERT( v1[0] == 1 && v1[1] == 1 );

	/**
	* std::replace_copy: same behaviour as std::replace, except that it doesn't modify the original range,
	* but it copies the original range, with the replaced values into a new range.
	* prototype: OutputIterator replace_copy(	InputIterator first, InputIterator last, OutputIterator destination,
	* 							  				const T& oldValue, const T& newValue);
	* copies the elements from the range [first,last) to the range starting with "destination", and replaces
	* the occurrences of "oldValue" with "newValue".
	*/
	std::vector<int> v3;
	v3.resize(arraySize);
	std::replace_copy(array, array + arraySize, v3.begin(), -77, 1); //array[] = { -77, -77, 2, 2, 3, 3};
	TESTIFY_ASSERT( v3[0] == 1 && v3[1] == 1 &&
					v3[2] == 2 && v3[3] == 2 &&
					v3[4] == 3 && v3[5] == 3 );

	/**
	* std::replace_copy_if: same behaviour as std::replace_if, except that it doesn't modify the original range,
	* but it copies the original range, with the replaced values into a new range.
	* OutputIterator replace_copy_if( InputIterator first, InputIterator last, OutputIterator destination,
	* 									Predicate somePredicate, const T& newValue);
	*/
	std::vector<int> v4;
	v4.resize(arraySize);
	std::replace_copy_if(array, array + arraySize, v4.begin(), NumericUtilities::isNegative, 1);
	TESTIFY_ASSERT( v4[0] == 1 && v4[1] == 1 &&
					v4[2] == 2 && v4[3] == 2 &&
					v4[4] == 3 && v4[5] == 3 );
}

//*****************************************************************************************
//							Comparing ranges
//*****************************************************************************************
void algorithms_for_comparing()
{
	/**
	*   std::equal: returns true if two ranges compare equal (have the same elements in the same order).
	*   It has two versions:
	* 		bool equal(InputIterator first1, InputIterator last1, InputIterator first2);
	*		bool equal(InputIterator first1, InputIterator last1, InputIterator first2 BinaryPredicate somePredicate);
	*		The first version compares the elements from the first range with the ones from the second range, using
	*		operator==.
	*		The second version uses "somePredicate." If the predicate returns true, the elements are considered equal.
	*/
	int first_range[] =  { 2, 3, 4, 5 };
	int second_range[] = { 2, 3, 4, 5 };
	int third_range[] =  { 3, 2, 4, 5 };
	int fourth_range[] = { 2, 3, 4, 5,  6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
	int rangeSize = sizeof(first_range)/sizeof(first_range[0]);

	bool equality = std::equal(first_range, first_range + rangeSize, second_range);
	TESTIFY_ASSERT(equality == true);

	equality = std::equal(second_range, second_range + rangeSize, third_range);
	TESTIFY_ASSERT(equality == false)

	equality = std::equal(first_range, first_range + rangeSize, fourth_range); //compares until reaches the end of first_range ("last1").
	TESTIFY_ASSERT(equality == true)

	/**
	*   std::lexicographical_compare: compares lexicographically two ranges, and returns true if the first is smaller
	*   then the second.
	* 	bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2);
	*	bool lexicographical_compare(InputIterator1 first1,  InputIterator1 last1, InputIterator2 first2,  InputIterator2 last2,
	*									BinaryPredicate somePredicate);
	*   The first version uses operator<, for comparing elements.
	*   The second uses "somePredicate". If the predicate returns true, the element from the first range is
	*   considered smaller (than the one from the second range).
	*/
	char first_word[] = {  's', 'o', 'c', 'i', 'e', 't', 'y' };
	char second_word[] = { 's', 'o', 'n', 'g' };
	int firstWordLen = sizeof(first_word)/sizeof(first_word[0]);
	int secondWordLen = sizeof(second_word)/sizeof(second_word[0]);
	bool firstIsLess = std::lexicographical_compare(first_word, first_word + firstWordLen, second_word,
			second_word + secondWordLen);
	TESTIFY_ASSERT(firstIsLess);

	/**
	*  std::mismatch: compares two ranges, and returns the first point were the ranges stop to be equal. The ranges have to
	*  be be of equal size.
	*	pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1,  InputIterator2 first2);
	*	pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1,  InputIterator2 first2,
	*													BinaryPredicate somePredicate);
	*   The first version returns a pair of iterators to the first element in each range for which operator== returns false.
	*   The second version returns a pair of iterators to the first element in each range for which "somePredicate"
	*   returns false.
	*/
	char first[] =  { 'a', 'b', 'c', 'd', 'e', 'f' };
	char second[] = { 'a', 'b', 'm', 'd', 'e', 'k' };
	int size = sizeof(first)/sizeof(first[0]);

	std::pair<char*, char*> firstMismatchPoint = std::mismatch(first, first + size, second);
	TESTIFY_ASSERT( *firstMismatchPoint.first ==  'c' &&
					*firstMismatchPoint.second == 'm' );
}

//*****************************************************************************************
//							Removing elements
//*****************************************************************************************


void algorithms_for_removing()
{
	int array[] = { 1, 2, 3, -99, 4, -99 };
	int arraySize = sizeof(array)/sizeof(array[0]);
	/**
	* The STL algorithms for removing, don't actually erase the elements from a range. They rearrange
	* the elements, so that the "removed" ones are placed at end of the sequence. All the other elements
	* remain in the same relative order.
	* The removing function returns an iterator to the "new end" of the range, which is the end of the
	* range, without the removed elements. The iterators past the "new end" [new_end, last_end)  are deferenceable,
	* but the values to which they point are unspecified, so we shouldn't use/deference these iterators.
	*/

	/**
	* std::remove: removes all the elements with a certain value
	* ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T& value);
	*/
	std::vector<int> v(array, array + arraySize);
	TESTIFY_ASSERT(v.size() == arraySize);

	//array contains 1, 2, 3, -99, 4, -99. Only 1, 2, 3, 4 should remain.
	std::vector<int>::iterator newEnd = std::remove(v.begin(), v.end(), -99);
	int i = 1;
	for( std::vector<int>::iterator it = v.begin(); it!=newEnd; ++it)
	{
		TESTIFY_ASSERT(*it!=-99);
		TESTIFY_ASSERT(*it == i);
		i++;
	}
	//std::distance computes the number of elements between two iterators:
	int diff = std::distance(v.begin(), newEnd);
	TESTIFY_ASSERT( 4 == diff ); // 1, 2, 3, 4

	TESTIFY_ASSERT(v.size() == arraySize); //size dindn't changed

	//the size of the vector wasn't changed. std::remove only moved the "removed" elements to the
	//end of the vector and returned a new end. In order to remove them, we have to use erase. This
	//will erase the elements, and change the size of the vector.
	v.erase(newEnd, v.end());
	TESTIFY_ASSERT(v.size() == 4); //only // 1, 2, 3, 4 exist now in the vector
	TESTIFY_ASSERT(newEnd == v.end());


	/**
	* std::remove_if: removes all the elements that satisfy a certain condition.
	* ForwardIterator remove_if(ForwardIterator first, ForwardIterator last, Predicate somePredicate);
	*/
	std::vector<int> v2(array, array + arraySize);
	newEnd = std::remove_if(v2.begin(), v2.end(), NumericUtilities::isNegative);
	//only { 1, 2, 3, 4 } should remain
	i = 1;
	for( std::vector<int>::iterator it = v2.begin(); it!=newEnd; ++it)
	{
		TESTIFY_ASSERT(*it!=-99);
		TESTIFY_ASSERT(*it == i);
		i++;
	}

	/**
	* std::remove_copy: same behaviour as std::remove, with the difference that the result of the removing
	* is copied into a new range, and the original range remains unchanged.
	* OutputIterator remove_copy(InputIterator first, InputIterator last, OutputIterator result, const T& value);
	* Returns an iterator indicating the past-the-end value the new range.
	*/
	std::vector<int> v3;
	v3.resize(arraySize + 50);	//resize the vector to hold 6 + 50 elements
	//std::distance computes the number of elements between two iterators:
	diff = std::distance(v3.begin(),v3.end());
	TESTIFY_ASSERT(diff == 56 );

	//copies { 1, 2, 3, 4 } in v3
	std::vector<int>::iterator endIt = std::remove_copy(array, array + arraySize, v3.begin(), -99);
	i = 1;
	for( std::vector<int>::iterator it = v3.begin(); it!=endIt;  ++it)
	{
		TESTIFY_ASSERT(*it!=-99);
		TESTIFY_ASSERT( *it == i);
		i++;
	}
	//std::distance computes the number of elements between two iterators:
	diff = std::distance(v3.begin(), endIt);
	TESTIFY_ASSERT(diff == 4 ); //4 elements

	/**
	* std::remove_copy_if: same behaviour as std::remove_if, with the difference that the result of the removing
	* is copied into a new range, and the original range remains unchanged.
	* OutputIterator remove_copy_if(InputIterator first, InputIterator last, OutputIterator result, Predicate somePredicate);
	*/
	std::vector<int> v4;
	v4.resize(arraySize + 34); //resize the vector to hold 6 + 34 elements
	//std::distance computes the number of elements between two iterators:
	diff = std::distance(v4.begin(), v4.end() );
	TESTIFY_ASSERT(diff == 40);

	//copies { 1, 2, 3, 4 } in v4
	std::vector<int>::iterator endIt_v4 = std::remove_copy_if(array, array + arraySize, v4.begin(),
			NumericUtilities::isNegative);
	i = 1;
	for( std::vector<int>::iterator it = v4.begin(); it != endIt_v4;  ++it)
	{
		TESTIFY_ASSERT(*it!=-99);
		TESTIFY_ASSERT( *it == i);
		i++;
	}
	// std::distance computes the number of elements between two iterators:
	diff = std::distance(v4.begin(), endIt_v4 );
	TESTIFY_ASSERT(diff == 4 ); //4 elements


	/**
	* std::unique: removes consecutive duplicate values from a range.
	* ForwardIterator unique(ForwardIterator first, ForwardIterator last);
	* std::unique iterates through the range [first, last), and if it finds consecutive values that are equal,
	* removes the the duplicate element, keeping only one. The elements are compared using the operator==.
	* std::unique returns an iterator pointing to the new end (the end of the range without the duplicate values).
	*/
	int duplicateValues[] = { 1, 1, 1, 1, 2, 3, 2 };
	int duplicateValuesSize = sizeof(duplicateValues)/sizeof(duplicateValues[0]);
	std::vector<int> v5(duplicateValues, duplicateValues + duplicateValuesSize);
	diff = std::distance(v5.begin(), v5.end());
	TESTIFY_ASSERT( 7 == diff ); //contains 7 elements: 1, 1, 1, 1, 2, 3, 2

	std::vector<int>::iterator newLastElem = std::unique(v5.begin(), v5.end());
	diff = std::distance(v5.begin(), newLastElem);
	TESTIFY_ASSERT( 4 == diff );
	TESTIFY_ASSERT( v5[0] == 1 &&
					v5[1] == 2 &&
					v5[2] == 3 &&
					v5[3] == 2 );

	/**
	* std::unique: removes consecutive equivalent elements from a range.
	* ForwardIterator unique(ForwardIterator first, ForwardIterator last, BinaryPredicate somePredicate);
	* std::unique iterates through the range [first, last), and if it finds consecutive values for which "somePredicate",
	* returns true, removes the the second element, keeping only one.
	* Returns an iterator pointing to the new end (the end of the range without the duplicate values).
	*/
	Employee cheapBob(7, "Bob");
	cheapBob.setSalary(1000);
	Employee expensiveJ(8, "John");
	expensiveJ.setSalary(3000);		//two employees are considered equal if they have the same salary and experience (operator== returns true).
	Employee expensiveM(8, "Marc");
	expensiveM.setSalary(3000);

	std::vector<Employee> staff;
	staff.push_back(cheapBob);
	staff.push_back(expensiveJ);
	staff.push_back(expensiveM);
	diff = std::distance(staff.begin(), staff.end());
	TESTIFY_ASSERT( diff == 3 );

	std::vector<Employee>::iterator new_end = std::unique(staff.begin(), staff.end(), equalSalary);
	for(std::vector<Employee>::iterator it = staff.begin(); it!= new_end; ++it)
	{
		TESTIFY_ASSERT(it->getName()!= "Marc");
	}
	diff = std::distance(staff.begin(), new_end);
	TESTIFY_ASSERT(diff == 2); //only Bob and John

	/**
	* std::unique_copy: copies a range into another range, removing duplicate values, from the first range.
	* It has two versions:	*
	* 	OutputIterator unique_copy(InputIterator first, InputIterator last, OutputIterator destination);
	*	OutputIterator unique_copy(InputIterator first, InputIterator last, OutputIterator result,
	*							BinaryPredicate somePredicate);
	*	The first version compares the elements with operator==. The second version uses "somePrediacte" to see
	*	if two consecutive elements are equivalent.
	* std::unique copies the elements from first range [first, last) into the range that begins with "destination".
	* If two consecutive elements compare equivalent, only the first one is copied into the destination range.
	* Returns an iterator pointing to the new end of the "destination" range.
	*/
	std::vector<int> v6(duplicateValuesSize + 30); //resize the vector to hold 7 + 30 elements
	diff = std::distance(v6.begin(), v6.end());
	TESTIFY_ASSERT( 37 == diff );

	//duplicateValues array contains: 1, 1, 1, 1, 2, 3, 2. In v6 will be copied only  1, 2, 3, 2.
	std::vector<int>::iterator itNewEnd = std::unique_copy(duplicateValues,
			duplicateValues + duplicateValuesSize, v6.begin());

	diff = std::distance(v6.begin(), itNewEnd);
	TESTIFY_ASSERT( diff == 4 );

	TESTIFY_ASSERT( v6[0] == 1 &&
					v6[1] == 2 &&
					v6[2] == 3 &&
					v6[3] == 2 );

}

//*****************************************************************************************
//						Sorting and operations on sorted ranges
//*****************************************************************************************
void sorting_algorithms()
{
	int array[] = { 0, 2, 1, 3 };
	int arraySize = sizeof(array)/sizeof(array[0]);

	Employee staff[] = { Employee(10, "Sam"), Employee(3, "Ham"), Employee(8, "Jam"), Employee(10, "Alex")};
	int staffSize = sizeof(staff)/sizeof(staff[0]);

	/**
	* std::sort: sorts the elements in a range into ascending order. It requires that the range
	* has random-access iterators.
	* It's not a stable sort. That means that the original order of the equivalent elements is not
	* guaranteed to remain the same, after the sorting operation.
	* It has two versions:
	* 	void sort(RandomAccessIterator first, RandomAccessIterator last);
	* 	void sort(RandomAccessIterator first, RandomAccessIterator last, BinaryPredicate somePredicate);
	* 	The first version uses operator< to compare elements. The second version uses "somePredicate".
	*/
	std::vector<int> v(array, array + arraySize);
	std::sort(v.begin(), v.end());
	TESTIFY_ASSERT( v[0] == 0 && v[1] == 1 && v[2]==2 && v[3] == 3);

	std::vector<Employee> myStaff(staff, staff + staffSize);
	//sort by experience
	std::sort(myStaff.begin(), myStaff.end(), lessExperience);
	TESTIFY_ASSERT(	myStaff[0].getExperience() == 3 &&
					myStaff[1].getExperience() == 8 &&
					myStaff[2].getExperience() == 10 &&
					myStaff[3].getExperience() == 10 );


	/**
	* std::stable_sort: sorts the elements in a range into ascending order. It requires that the range
	* has random-access iterators.
	* It is a stable sort. The original order of the equivalent elements is guaranteed to remain the same,
	* after the sorting operation.
	* void sort(RandomAccessIterator first, RandomAccessIterator last);
	* void sort(RandomAccessIterator first, RandomAccessIterator last, BinaryPredicate somePredicate);
	*/
	std::vector<Employee> myOrderedStaff( staff, staff + staffSize );

	//Compare by experience.
	std::stable_sort(myOrderedStaff.begin(), myOrderedStaff.end(), lessExperience);
	TESTIFY_ASSERT(		myOrderedStaff[0].getExperience() == 3  &&
						myOrderedStaff[1].getExperience() == 8  &&
					 ( 	myOrderedStaff[2].getExperience() == 10 && myOrderedStaff[2].getName() == "Sam" ) &&
					 ( 	myOrderedStaff[3].getExperience() == 10 && myOrderedStaff[3].getName() == "Alex") );


	/**
	* std::partial_sort: sorts a subrange of a range.
	* It has two versions:
	* 	void partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last);
	* 	void partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last,
	*  	  	  	  	  		BinaryPredicate somePredicate);
	* 	Sorts the elements from the range [first, last), that can be placed in the range [first, middle) in
	* 	ascending order. The rest of the elements are moved after "middle" and their order is not specified.
	* 	The first version uses operator==, to compare elements, the second uses "somePredicate",
	* 	to determine which element is smaller.
	*/
					   //experience, name
	Employee dev[] = { Employee(8, "Gaston"), Employee(1, "Sissy"), Employee(3, "Castor"), Employee(0, "Missy") };
	int devSize = sizeof(dev)/sizeof(dev[0]);

	std::vector<Employee> devStaff(dev, dev + devSize);
	std::vector<Employee>::iterator middle = devStaff.begin() + 2; //middle -> Castor
	//move all the employees with experience less than 3 years, to the beginning of the devStaff vector, and sort
	//them into ascending order.
	std::partial_sort( devStaff.begin(), middle, devStaff.end(), lessExperience); //middle = Castor

	TESTIFY_ASSERT(devStaff[0].getExperience() == 0 );
	TESTIFY_ASSERT(devStaff[1].getExperience() == 1 );

	/**
	* std::partial_sort_copy: sorts the elements from a first range and copies them into a second range. Only the number of
	* elements that fit inside the second range are copied and sorted.
	* It has two versions:
	* 	RandomAccessIterator partial_sort_copy(InputIterator first, InputIterator last, RandomAccessIterator destinationFirst,
	*											RandomAccessIterator destinationEnd);
	* 	RandomAccessIterator partial_sort_copy(InputIterator first, InputIterator last, RandomAccessIterator destinationFirst,
	*  	  	  	  	  	  	  	  	  	  	  RandomAccessIterator destinationEnd, BinaryPredicate somePredicate);
	*   The first version sorts the elements using the operator<. The second version uses "somePredicate".
	*/
	int first_range[] = { 4, 3, 2, 1, 10, 9, 8, 7, 6};
	int firstRangeSize = sizeof(first_range)/sizeof(first_range[0]);
	std::vector<int> second_range(4);
	std::partial_sort_copy(first_range, first_range + firstRangeSize, second_range.begin(), second_range.end());
	TESTIFY_ASSERT( second_range[0] == 1 && second_range[1] == 2 && second_range[2] == 3 && second_range[3] == 4);

	/**
	* std::nth_element: rearranges the elements in a range, so that all the elements smaller than a
	* specified element will be moved at the beginning of the range. Neither subrange is in any
	* particular order, the elements aren't sorted.
	* It has two versions:
	* 	void nth_element(RandomAccessIterator first, RandomAccessIterator nth_element, RandomAccessIterator last);
	* 	void nth_element(RandomAccessIterator first, RandomAccessIterator nth_element, RandomAccessIterator last,
	*					BinaryPredicate somePredicate);
	*/
	char letters[] = { 'b', 'd', 'a', 'c', 'w', 'z' };
	int lettersSize = sizeof(letters)/sizeof(letters[0]);
	char *nthElem = letters + 1; //'d'
	std::nth_element(letters, nthElem, letters + lettersSize);
	LOG("\nnth_element result:\n{ ");
	for(int i=0; i<lettersSize; ++i)
	{
		LOG("%c ", letters[i]);
	}
	LOG(" }\n\n");

	/**
	* binary_search: performs a binary search in a sorted range.
	* It has two versions:
	* 	bool binary_search(ForwardIterator first, ForwardIterator last, const T& value);
	* 	bool binary_search(ForwardIterator first, ForwardIterator last, const T& value,
	* 						BinaryPredicate somePredicate);
	*	Searches in the range [first, last) for "value". It it's found returns true.
	*	The first version uses operator< for testing if the elements are equal (!(a<b) && !(b<a)),
	*	and assumes that operator< was used to sort the elements.
	*	The second version uses "somePredicate" to compare elements and assumes that "somePredicate"
	*	was used also for sorting.
	*	The same comparison function has to be used for sorting and for searching. Otherwise the
	*	results are not defined.
	*/
	int myArray[] = { 4, 1, 2, 0 };
	int sizeArray = sizeof(myArray)/sizeof(myArray[0]);
	std::vector<int> myVector(myArray, myArray + sizeArray);

	std::sort(myVector.begin(), myVector.end());
	TESTIFY_ASSERT(myVector[0] == 0 && myVector[1] == 1 && myVector[2] == 2 && myVector[3] == 4);

	bool found = std::binary_search(myVector.begin(), myVector.end(), 2);
	TESTIFY_ASSERT(found == true);

	/**
	* lower_bound: returns an iterator indicating the first occurrence of a value in a range. If
	* no value was found, an iterator were it would fit is returned.
	* It has two overloads:
	* 	ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value);
	* 	ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value,
	*								BinaryPredicate somePredicate);
	*   The first version uses operator< to compare elements. The second uses "somePredicate".
	*/
	int someValues[] = { 3, 3, 7, 6, 6, 6, 8 };
	int sizeValues = sizeof(someValues)/sizeof(someValues[0]);
	std::vector<int> values(someValues, someValues + sizeValues);

	std::sort(values.begin(), values.end());		//{ 3, 3, 7, 6, 6, 6, 8 } => { 3, 3, 6, 6, 6, 7, 8 }

	std::vector<int>::iterator lower = std::lower_bound(values.begin(), values.end(), 6);
	TESTIFY_ASSERT(*lower = 6);

	//std::distance computes the number of elements between two iterators:
	int pos = std::distance(values.begin(), lower);
	TESTIFY_ASSERT(pos == 2);

	/**
	* upper_bound: returns an iterator indicating the first element in a range that is greater than a
	* specified value. If the value is not found, an iterator were it would fit is returned.
	* Two versions:
	* 	ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value);
	* 	ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value,
	* 								BinaryPredicate somePredicate);
	* The first version uses operator< to compare elements. The second uses "somePredicate".
	*/
	std::vector<int>::iterator upper = std::upper_bound(values.begin(), values.end(), 6);
	TESTIFY_ASSERT(7 == *upper);
	//std::distance computes the number of elements between two iterators:
	pos = std::distance(values.begin(), upper);
	TESTIFY_ASSERT(pos == 5);

	/**
	* equal_range: it's a combination or lower_bound and upper_bound. Returns the bounds of the largest subrange
	* that includes all the elements equal to a certain value.
	* It has two versions:
	*	pair<ForwardIterator, ForwardIterator> equal_range(ForwardIterator first, ForwardIterator last, const T& value);
	* 	pair<ForwardIterator, ForwardIterator> equal_range(ForwardIterator first, ForwardIterator last, const T& value,
	* 														BinaryPredicate somePredicat);
	* The first version uses operator< to compare elements. The second uses "somePredicate".
	* If the value isn't found in the range, the subrange returned has the length zero. Both iterators will point
	* to the nearest value greater than value. If the value is greater than all the element in the range it returns "last".
	*/
	typedef std::vector<int>::iterator ITER;
	std::pair<ITER, ITER> result = std::equal_range(values.begin(), values.end(), 6);
	TESTIFY_ASSERT(*result.first == 6);
	TESTIFY_ASSERT(*result.second == 7);

	//std::distance computes the number of elements between two iterators:
	pos = std::distance(values.begin(), result.first);
	TESTIFY_ASSERT(pos == 2);

	pos = std::distance(values.begin(), result.second);
	TESTIFY_ASSERT(pos == 5);
}

//*****************************************************************************************
//								Heap operations
//*****************************************************************************************
void heap_operations()
{
	/**
	*  A heap is a tree were each node is connected to values smaller than its own value.
	*  The first element is always the greatest value in the heap.
	*/

	/**
	*  make_heap: arranges the elements in a range so that they form a heap. The first element is always
	*  the greatest value in the heap.
	*  It has two versions:
	*  	void make_heap ( RandomAccessIterator first, RandomAccessIterator last );
	*  	void make_heap(RandomAccessIterator first, RandomAccessIterator last, BinaryPredicate somePredicate);
	*  	The first version compares elements with operator</ The second uses "somePredicate" to compare elements.
	*/
	int array[] = { 1, 4, 3, 2 };
	int arraySize = sizeof(array)/sizeof(array[0]);
	std::vector<int> v1(array, array + arraySize);
	std::make_heap(v1.begin(), v1.end());
	TESTIFY_ASSERT( v1[0] == 4 );

	/**
	*  push_heap:
	*   It has two versions: expands a heap range by inserting the last element into the heap (in the proper place).
	*   It has two variants:
	*  		void push_heap(RandomAccessIterator first, RandomAccessIterator last);
	*  		void push_heap(RandomAccessIterator first, RandomAccessIterator last, BinaryPredicate somePredicate);
	*		push_heap places the value "last - 1" at the the proper place in the heap.
	*   The first version uses operator< to compare the elements and the second uses "somePredicate".
	*/
	int array2[] = { 0, 7, 6 };
	int sizeArray2 = sizeof(array2)/sizeof(array2[0]);

	std::vector<int> v2(array2, array2 + sizeArray2);

	std::make_heap(v2.begin(), v2.end());
	TESTIFY_ASSERT( v2[0] == 7 );					//v2 contains now: 7, 0, 6

	v2.push_back(100);
	TESTIFY_ASSERT( v2[0] == 7 && v2[3] == 100 );	//v2 contains now: 7, 0, 6, 100

	std::push_heap(v2.begin(), v2.end());
	TESTIFY_ASSERT( v2[0] == 100  && v2[1] == 7 );	//v2 contains now: 100, 7, 0, 6

	/**
	*  pop_heap: places the first element, into the last position in the heap.
	*  It has two versions:
	*   void pop_heap(RandomAccessIterator first, RandomAccessIterator last);
	*   void pop_heap(RandomAccessIterator first, RandomAccessIterator last, BinaryPredicate somePredicate);
	*   Places "first" (which has the biggest value) into the position (last - 1) and rearranges the range
	*   so that is still a heap.
	*/
	//v2 contains: 100, 7, 0, 6
	std::pop_heap(v2.begin(), v2.end());
	TESTIFY_ASSERT( v2[0] == 7 && v2[3] == 100 );		//v2 contains now: 7, 0, 6, 100. The last element is 100.

	/**
	* sort_heap: arranges the elements in the heap so that they form a sorted range.
	* void sort_heap(RandomAccessIterator first, RandomAccessIterator last);
	* void sort_heap(RandomAccessIterator first, RandomAccessIterator last, BinaryPredicate somePredicate);
	*/
	//v2 contains: 7, 0, 6, 100.
	TESTIFY_ASSERT(v2[0] == 7 && v2[1] == 0 && v2[2] == 6 && v2[3] == 100);

	std::make_heap(v2.begin(), v2.end());
	TESTIFY_ASSERT(v2[0]== 100 && v2[1] == 7 && v2[2] == 6 && v2[3] == 0);
}

//*****************************************************************************************
//					Applying an operation to each element in a range
//*****************************************************************************************
void apply_operations_on_a_range()
{
	/**
	*  A unary function is a function having one argument.
	*/

	Employee someStaff[] = { Employee(1, "Bob"), Employee(1, "Bill"), Employee(1, "Ben") };
	int staffSize = sizeof(someStaff)/sizeof(someStaff[0]);
	TESTIFY_ASSERT( someStaff[0].getSalary() == 100 &&
					someStaff[1].getSalary() == 100 &&
					someStaff[2].getSalary() == 100 );
	/**
	* for_each: iterates through a range and applies a operation to each element. The operation can be
	* implemented as a simple function or a function call operator overloaded in a functor.
	* prototype: UnaryFunction for_each(InputIterator first, InputIterator last, UnaryFunction someFun);
	* Calls "someFun" on each of the elements in the range [first, last). We can supply a functor
	* or a simple function, for "someFun".
	*/
	std::vector<Employee> happierStaff(someStaff, someStaff + staffSize);
	std::for_each(happierStaff.begin(), happierStaff.end(), increaseSalary); //increase salary by 100
	TESTIFY_ASSERT( happierStaff[0].getSalary() == 200 &&
					happierStaff[1].getSalary() == 200 &&
					happierStaff[2].getSalary() == 200 );

	/**
	* std::transform: iterates through a range and applies an operation to each element, saving the result
	* in a second range. The operation can be implemented as a simple function or a function call operator
	* overloaded in a functor.
	* It has two versions:
	* 	OutputIterator transform(InputIterator first, InputIterator last, OutputIterator result, UnaryFunction someFun);
	* 	OutputIterator transform(	InputIterator1 first1, InputIterator1 last1,
	* 								InputIterator2 first2, OutputIterator destination, BinaryFunction someFun);
	*   The first version applies "someFun" to all the elements in the range [first1,last1) and
	*   stores the returned value in the range beginning at destination.
	*   It performs: *destination++ = someFun(*first++) while first!=last.
	*   The second version uses, as the first argument for the binary function, an element from the first range,
	*   and as a second argument, an element from the second range. The result is stored the range that
	*   begins with "destination".
	*   It performs: *destination++ = someFun(*first1++, *first2++) while (first1!=last1).	*
	*   For both versions "destination" must point to  a type that has an operator= available (public)).
	*
	* The return value in is the one past the end iterator for the resulting "destination" range.
	*/
	std::vector<Employee> happyStaff;
	happyStaff.resize(happierStaff.size());
	std::transform(happierStaff.begin(), happierStaff.end(), happyStaff.begin(), increaseSalary);
	TESTIFY_ASSERT( happyStaff[0].getSalary() == 300 &&
					happyStaff[1].getSalary() == 300 &&
					happyStaff[2].getSalary() == 300 );

}

//*****************************************************************************************
//								Numeric algorithms
//*****************************************************************************************

// The numeric algorithms are defined in the <numeric> header
void algorithms_for_numeric_op()
{
	/**
	* accumulate: returns the result of accumulating all the values in a range.
	* It has two versions:
	* 	T accumulate(InputIterator first, InputIterator last, T initialValue);
	* 	T accumulate(InputIterator first, InputIterator last, T initialValue, BinaryFunction someFun);
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
	int array[] = { 1, 2, 1 };
	int arraySize = sizeof(array)/sizeof(array[0]);
	int res = std::accumulate(array, array + arraySize, 5); //1 + 2 = 3; 3 + 1 = 4
	TESTIFY_ASSERT( (5 + 4 ) == res );						//5 = initial value, 4 = 1 + 2 + 1

	/**
	* inner_product: computes cumulative product of a range.
	* It has two versions:
	* 	T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T initialValue);
	* 	T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T initialValue,
	* 					BinaryFunction1 someFun1, BinaryFunction2 someFun2);
	*   The first version performs:
	*   	result = initialValue;
	*   	while( first1 != last1){
	*   	result = result + (*first1) * (*first2); }
	*   	e.g: for two arrays: { 1, 2, 3 }, { 10, 20, 30 } and initial value of -9, the inner product is:
	*   					-9 + ( 1*10 + 2*20 + 3*30 ).
	*
	*   The second version uses "someFun1" instead of operator+ and "someFun2" instead of the multiply operator ("*").
	*/
	//array[] = { 1, 2, 1 };
	int array2[] = { -1, -2, -1 };

	res = std::inner_product(array, array + arraySize,  array2, 0);  //0 + 1*(-1) + 2*(-2) + 1*(-1)
	TESTIFY_ASSERT( -6 == res );

	res = std::inner_product(array, array +arraySize, array2, 6 );
	TESTIFY_ASSERT( 0 == res );

	/**
	*
	* partial_sum: computes the partial sum of the elements in a range.
	* It has two versions:
	* 	OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator destination);
	* 	OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator destination,
	*							  BinaryFunction someFun );
	*	Assigns to every element in the range, starting at destination, the partial sum of the corresponding
	*	elements in the range, like this:
	*			*destination++ = *first;
	*			*destination++ = *first + *(first + 1);
	*			*destination++ = *first + *(first + 1) + *(first + 2);
	*			*destination++ = *first + *(first + 1) + *(first + 2) + *(first + 3);
	*			ect. Performs this until reaches at the end of the first range (last).
	*	The first version uses operator+ to sum the elements.
	*	The second version uses "someFun" instead of operator+.
	*/
	int array3[] = { 1, 2, 3 };
	int array3_size = sizeof(array3)/sizeof(array3[0]);
	std::vector<int> v3;
	v3.resize(array3_size);
	std::partial_sum(array3, array3 + array3_size, v3.begin());

	//v3 will contain: 1, 1+2, 1+2+3
	TESTIFY_ASSERT( v3[0] == 1 &&
					v3[1] == (1 + 2) &&
					v3[2] == (1 + 2 + 3) );


	/**
	* adjacent_difference: iterates through a range and computes the difference of adjacent elements.
	* It has two versions:
	* 	OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator destination);
	* 	OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator destination,
	* 										BinaryFunction someFun);
	* 	Iterates through [first, last) and computes the difference of the adjacent elements (current_element
	* 	- previous_element).
	* 	It performs:
	* 		*destination++ = *first;
	* 		*destination++ = *(first+1) - *first;
	* 		*destination++ = *(first+2) - *(first+1);
	* 		*destination++ = *(first+3) - *(first+2);
	* 		Iterates like this until reaches the end of the first range(last).
	* 	The second form uses "someFun" instead of operator-.
	*/
	std::vector<int> v4;
	v4.resize(array3_size);
	std::adjacent_difference(array3, array3 + array3_size, v4.begin()); //array3[] = { 1, 2, 3 };
	//v4 will contain: 1, 2-1, 3-1
	TESTIFY_ASSERT( v4[0] == 1 		&&
					v4[1] == (2-1) 	&&
					v4[2] == (3-2) );
}

//*****************************************************************************************
//								General utilities
//*****************************************************************************************
void general_utilities()
{
	/**
	*  std::distance: returns the number of elements in range.
	*  int distance(InputIterator first, InputIterator last);
	*/
	int longArray[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
	int longArraySize = sizeof(longArray)/sizeof(longArray[0]);
	int numElem = std::distance(longArray, longArray + 3);
	TESTIFY_ASSERT( 3 == numElem );

	numElem = std::distance(longArray, longArray + 14);
	TESTIFY_ASSERT( 14 == numElem );

	numElem = std::distance(longArray, longArray + 0);
	TESTIFY_ASSERT( 0 == numElem );

	numElem = std::distance(longArray, longArray + 20);
	TESTIFY_ASSERT( 20 == numElem );
	TESTIFY_ASSERT( longArraySize == numElem );

	/**
	*  back_inserter: returns a "back_insert_iterator" for a  container.
	*  The "back_insert_iterator" expands the container automatically, calling the "push_back"
	*  function of the container. It is used to insert elements into a container, rather than overwrite
	*  the existing elements, using operator=. It is useful with algorithms that overwrite
	*  elements like, copy.
	*  The back_insert_iterator class has its operator= defined like this:
	*  		back_insert_iterator<Container>& operator= (Container::value_type &value) {
    { 					container->push_back(value);
    					return *this; }
    		So when *iterator = someValue is called, instead of an assignment, a push_back operation takes place.
	*
	*  prototype: back_insert_iterator<Container>   back_inserter(Container& x);
	*/

	int chars[] = { 'a', 'b', 'c' };
	int charsNumber = sizeof(chars)/sizeof(chars[0]);

	//two variants to copy the "chars" array into a vector:

	std::vector<char> myVector;
	//resize the vector because std::copy increments the iterator we supply(myVector.begin()) and assigns values to it.
	//The vector won't expand automatically so we have to set the proper size.
	myVector.resize(charsNumber);
	std::copy(chars, chars + charsNumber, myVector.begin());
	TESTIFY_ASSERT( myVector[0] = 'a' && myVector[1] == 'b' && myVector[2] == 'c');

	std::vector<char> mySecondVector;
	//we supply a back_insert_iterator to std::copy. That means that the back_insert_iterator will expand the
	//vector automatically, as we copy elements in it.
	//(std::copy will perform *iterator++ = elementValue, the back_insert_iterator::operator= will be called,
	//and this will call push_back on the container).
	std::copy(chars, chars + charsNumber, std::back_inserter(mySecondVector));
	TESTIFY_ASSERT( mySecondVector[0] = 'a' && mySecondVector[1] == 'b' && mySecondVector[2] == 'c');
	TESTIFY_ASSERT( mySecondVector.size() == charsNumber );

	/**
	*  front_inserter: returns a "front_insert_iterator" for a container.
	*  The "front_insert_iterator" expands the container automatically, calling the "push_front"
	*  function of the container. It is used to insert elements into a container, rather than overwrite
	*  the existing elements, using operator=. It is useful with algorithms that overwrite
	*  elements like, copy.
	*  The front_insert_iterator class has its operator= defined like this:
	*  		front_insert_iterator<Container>& operator= (Container::value_type &value) {
	{ 					container->push_front(value);
						return *this; }
			So when *iterator = someValue is called, instead of an assignment, a push_front operation takes place.
	*
	*  prototype: front_insert_iterator<Container>  front_inserter(Container& x);
	*/
	std::deque<char> d;
	d.push_back('d');
	d.push_back('e');
	char abc[] = { 'c', 'b', 'a' };
	int abcSize = sizeof(abc)/sizeof(abc[0]);
	std::copy( abc, abc + abcSize, front_inserter(d));
	TESTIFY_ASSERT( 'a' == d[0] && 'b' == d[1] && 'c' == d[2] &&
					'd' == d[3] && 'e' == d[4] );

	/**
	*  inserter: returns a "insert_iterator" for a container.
	*  prototype:insert_iterator<Container>  inserter(Container& x, Iterator i);
	*/

	/**
	*  inserter: returns a "insert_iterator" for a container.
	*  The "insert_iterator" expands the container automatically, calling the "insert"
	*  function of the container. It is used to insert elements into a container, rather than overwrite
	*  the existing elements, using operator=. It is useful with algorithms that overwrite
	*  elements like, copy.
	*  The insert_iterator class has its operator= defined like this:
	*  		insert_iterator<Container>& operator= (Container::value_type &value) {
	{ 					iter = container->insert(iter, value);
						++iter;
						return *this; }
			So when *iterator = someValue is called, instead of an assignment, a push_front operation takes place.
	*
	*  prototype: insert_iterator<Container> inserter (Container& x, Inserter iter);
	*/
	char someChars[] = { 'a', 'b', 'e', 'f' };
	int someCharsSize = sizeof(someChars)/sizeof(someChars[0]);
	std::vector<char> myChars(someChars, someChars + someCharsSize); //vector contains now: a, b, e, f
	std::vector<char>::iterator iter = myChars.begin() + 2;

	char missingChars[] = { 'c', 'd' };
	int missingCharsSize = sizeof(missingChars)/sizeof(missingChars[0]);
	//insert c  and d after b
	std::copy(missingChars, missingChars + missingCharsSize, std::inserter(myChars, iter));

	//the vector contains now: a, b, c, d, e, f
	TESTIFY_ASSERT( 'a' == myChars[0] && 'b' == myChars[1] &&
					'c' == myChars[2] && 'd' == myChars[3] &&
					'e' == myChars[4] && 'f' == myChars[5] );
}


}//end namespace

void TestSTL::test_algorithms()
{
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
}
