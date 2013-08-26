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

#ifndef _OPENGLES_H_
#define _OPENGLES_H_

namespace Base {

struct SubView {
	int x, y, w, h;
	void* data; // hold handle for instance.
};

bool subViewOpen(int left, int top, int width, int height, SubView& out);
bool subViewClose(const SubView& sv);

bool openGLInit(const SubView& subView);
bool openGLClose(const SubView& subView);
bool openGLSwap(const SubView& subView);
bool openGLProcessEvents(const SubView &subView);

}

#endif // _OPENGLES_H_