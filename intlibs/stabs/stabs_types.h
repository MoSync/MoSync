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

#ifndef STABS_TYPES_H
#define STABS_TYPES_H

#include "stabs_type.h"

//must call before adding any types for that file
void addTypeFile(int file, const std::string& fileName);

bool addType(const Type&);

//returns NULL if the type cannot be found.
const TypeBase* findTypeByTuple(Tuple);
const TypeBase* findTypeByTupleAndFile(Tuple t, int file);

const TypeBase* findTypeByNameAndFileGlobal(const std::string& name, int scope);
const TypeBase* findTypeByNameAndTupleAndFileGlobal(const std::string& name, Tuple tuple, int scope);

void clearTypes();

#endif	//STABS_TYPES_H
