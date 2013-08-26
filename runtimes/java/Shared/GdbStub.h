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

#ifndef _GDB_STUB_H_
#define _GDB_STUB_H_

#include "core_consts.h"

/* used when compiling the GDB stub */
#define DBG_STAB ENDOP

#define DATA_MEMORY_START			0x00000000
#define INSTRUCTION_MEMORY_START	0x40000000
#define ADDRESS_MASK				0x3fffffff

#endif
