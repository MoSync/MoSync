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

#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_

namespace Debugger {
	bool init(
		int port, 
		int *regs,
		byte *mem_ds, 
		unsigned int size_mem_ds, 
		byte *mem_cs, 
		unsigned int size_mem_cs,
		unsigned int *pc
		);
	
	void close();


	enum BreakResult {
		BRK_QUIT,
		BRK_READ_ERR,
		BRK_SEND_ERR,
		BRK_GENERIC_ERR,
		BRK_CONTINUE
	};

	BreakResult brk();
};

#endif /* _DEBUGGER_H_ */
