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

#ifndef _TEXTOUTPUT_H_
#define _TEXTOUTPUT_H_

#include "Image.h"
#include <windows.h>

namespace TextOutput {
	void drawText(Image *dst, int x, int y, const void *str, unsigned int color, bool wide);
	SIZE getTextSize(const void *str, bool wide);
}

#endif /* _TEXTOUTPUT_H_ */