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

/**
 * @file AboutFunctionPointerAdaptors.cpp
 * @author Gabriela Rata
 */

#include <functional>
#include "../STLMoblet.h"
#include "../Employee.h"
#include "../LOG.h"


struct MyFirstFunctor
{
	bool operator()(const Employee &lv, const Employee &rv)
	{
		log_to_console("Calling MyFirstFunctor::operator()");
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

void STLMoblet::pointer_adapters_explained()
{
	LOG("\n");
	LOG("========================= about function pointers adapters ===============================================================");
	LOG("/**");
	LOG("* STL function adaptors make assumptions about the functors they use.");
	LOG("* See AboutFunctors.cpp for what is a functor and  Adaptors.cpp on what is an adaptor.");
	LOG("* Adaptors.cpp describes also the adaptors provided by STL.");
	LOG("* The function adapters need to know the argument type and the return value of the operator().");
	LOG("* So, a STL function adaptor requires that the supplied functor object provides the");
	LOG("* the following public type definitions:");
	LOG("* 		- for unary functors:");
	LOG("* 			argument_type 	-> for the argument type of the operator().");
	LOG("* 			return_type 	-> for the return type of the operator().");
	LOG("*  		- for binary functors:");
	LOG("*  			first_argument_type 	-> for the first argument type of operator()");
	LOG("*  			second_argument_type	-> for the second argument type of operator()");
	LOG("*			return_type 	-> for the return type of the operator().");
	LOG("* STL provides two templates that have these type definitions: unary_function");
	LOG("* and binary_function.");
	LOG("* The definitions are simple:");
	LOG("*");
	LOG("* 	template<class ArgumentType, class ResultType>");
	LOG("* 	struct unary_function");
	LOG("* 	{");
	LOG("*  	  	  typedef ArgumentType 		argument_type;");
	LOG("*  	  	  typedef ResultType 		result_type;");
	LOG("*	};");
	LOG("*");
	LOG("*	template<class FirstArgumentType, class SecondArgumentType, class ResultType>");
	LOG("*	struct binary_function");
	LOG("* 	{");
	LOG("* 	  	  typedef FirstArgumentType 		first_argument_type;");
	LOG("* 	  	  typedef SecondArgumentType 		second_argument_type;");
	LOG("* 		  typedef ResultType 				result_type;");
	LOG("*	}");
	LOG("*");
	LOG("* All the STL functors provide the type definitions required by functor adapters.");
	LOG("* If we want to use our own functors, with the STL functor adaptors, we can derive");
	LOG("* from unary_function/binary_function. They are provided so that the user doesn't");
	LOG("* have to manually write these typedefs.");
	LOG("*");
	LOG("* unary_function and binary_function are defined <functional> header.");
	LOG("*/");
	LOG("\n");


	LOG("		   Example using function pointer adapters	     		  ");

	LOG("struct MyFirstFunctor");
	LOG("{");
	LOG("	bool operator()(const Employee &lv, const Employee &rv)");
	LOG("	{");
	LOG("		return lv.getName() < rv.getName();");
	LOG("	}");
	LOG("};");

	LOG("struct MySecondFunctor : public std::binary_function<Employee, Employee, bool>");
	LOG("{");
	LOG("	bool operator()(const Employee &lv, const Employee &rv)");
	LOG("	{");
	LOG("		return lv.getName() < rv.getName();");
	LOG("	}");
	LOG("};");

	LOG("\n");
	LOG("/**");
	LOG("* using an STL functor (std::less) with an STL functor adaptor (bind1st).");
	LOG("* Always works because the STL functors provide the right typedefs.");
	LOG("*/n");

	TRACE(std::bind1st(std::less<int>(), 101));		//ok

	LOG("\n");
	LOG("/**");
	LOG("* using an STL functor adapter (bind1st) with a non standard");
	LOG("* functor (MyFirstFunctor).");
	LOG("* Doesn't work because \"MyFirstFunctor\" doesn't provide the typedefs that");
	LOG("* STL functor adaptors need.");
	LOG("*/");
	LOG("\n");

	LOG("//std::bind1st(MyFirstFunctor(), Employee(10, \"Bob\")); //doesn't compile");

	LOG("\n");
	LOG("/**");
	LOG("* using an STL functor adapter (bind1st) with a non standard");
	LOG("* functor (MySecondFunctor).");
	LOG("* It's work because \"MySecondFunctor\" inherits from std::binary_function.");
	LOG("*/");
	LOG("\n");
	TRACE(std::bind1st(MySecondFunctor(), Employee(10, "Bob"))); //compiles

	LOG("\n");
}
