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