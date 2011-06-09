/*
 * FunctionPointerAdaptors.cpp
 *
 *  Created on: Jun 2, 2011
 *      Author: gabi
 */

#include <functional>
#include "../TestSTL.h"
#include "../Employee.h"

/**
* STL function adaptors make assumptions about the functors they use.
* See AboutFunctors.cpp for what is a functor and  Adaptors.cpp on what is an adaptor.
* Adaptors.cpp describes also the adaptors provided by STL.
* The function adapters need to know the argument type and the return value of the operator().
* So, a STL function adaptor requires that the supplied functor object provides the
* the following public type definitions:
* 		- for unary functors:
* 			argument_type 	-> for the argument type of the operator().
* 			return_type 	-> for the return type of the operator().
*  		- for binary functors:
*  			first_argument_type 	-> for the first argument type of operator()
*  			second_argument_type	-> for the second argument type of operator()
*			return_type 	-> for the return type of the operator().
* STL provides two templates that have these type definitions: unary_function
* and binary_function.
* The definitions are simple:
*
* 	template<class ArgumentType, class ResultType>
* 	struct unary_function
* 	{
*  	  	  typedef ArgumentType 		argument_type;
*  	  	  typedef ResultType 		result_type;
*	};
*
*	template<class FirstArgumentType, class SecondArgumentType, class ResultType>
*	struct binary_function
* 	{
*  	  	  typedef FirstArgumentType 		first_argument_type;
* 	  	  typedef SecondArgumentType 		second_argument_type;
* 		  typedef ResultType 				result_type;
*	}
*
* All the STL functors provide the type definitions required by functor adapters.
* If we want to use our own functors, with the STL functor adaptors, we can derive
* from unary_function/binary_function. They are provided so that the user doesn't
* have to manually write these typedefs.
*
* unary_function and binary_function are defined <functional> header.
*/

struct MyFirstFunctor
{
	bool operator()(const Employee &lv, const Employee &rv)
	{
		return lv.getName() < rv.getName();
	}
};

struct MySecondFunctor : public std::binary_function<Employee, Employee, bool>
{
	bool operator()(const Employee &lv, const Employee &rv)
	{
		return lv.getName() < rv.getName();
	}
};

void TestSTL::example_function_pointer_adapters()
{
	/**
	* using an STL functor (std::less) with an STL functor adaptor (bind1st).
	* Always works because the STL functors provide the right typedefs.
	*/
	std::bind1st(std::less<int>(), 101);		//ok

	/**
	* using an STL functor adapter (bind1st) with a non standard functor (MyFirstFunctor).
	* Doesn't work because "MyFirstFunctor" doesn't provide the typedefs that
	* STL functor adaptors need.
	*/
	//std::bind1st(MyFirstFunctor(), Employee(10, "Bob")); //doesn't compile

	/**
	* using an STL functor adapter (bind1st) with a non standard functor (MySecondFunctor).
	* It's work because "MySecondFunctor" inherits from std::binary_function.
	*/
	std::bind1st(MySecondFunctor(), Employee(10, "Bob")); //compiles
}
