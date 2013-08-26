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

// range 'expression to calculate the range'
#define _range_(x, group) __attribute((gccxml("range", #x), gccxml("group", group)))

// defaults to handle without range
// generate 'true/false to say if a pointer wrapper should be generated' 'true/false to say if a handle wrapper should be generated' 'the range to use to validate pointers'
// must add range attribute later so that you can validate the memory. maybe add native_pointer that returns a native pointer directly
// don't support permutations of these either yet. If you have an argument of handle type it will generate a functions where all pointer arguments are handles and vice versa.
#define _use_type_(pointer, handle) __attribute((gccxml("use_pointer", #pointer), gccxml("use_handle", #handle)))
