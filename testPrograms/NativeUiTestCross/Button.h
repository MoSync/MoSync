#ifndef BUTTON_H
#define BUTTON_H

#include <ma.h>
#include <maassert.h>
#include <mastring.h>
#include <IX_NATIVE_UI.h>
#include "Widget.h"


class Button : public Widget {
public:
	Button(int id, char *str);
	~Button();
	int getId();
	void *getInstance();
	void processEvent(const MAEvent &);
	void build(int x, int y, int h, int l, void *f);

};

#endif
