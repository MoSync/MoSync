#ifndef FRAME_H
#define FRAME_H

#include <ma.h>
#include <IX_NATIVE_UI.h>
#include "Widget.h"


class Frame : public Widget {
public:
	Frame(int id);
	~Frame();
	int getId();
	void *getInstance();
	void processEvent(const MAEvent &);


};

#endif
