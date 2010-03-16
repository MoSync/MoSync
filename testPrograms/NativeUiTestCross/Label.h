#ifndef LABEL_H
#define LABEL_H

#include <ma.h>
#include <maassert.h>
#include <mastring.h>
#include <IX_NATIVE_UI.h>
#include "Widget.h"


class Label : public Widget {
public:
	Label(char *str);
	~Label();
	int getId();
	void *getInstance();
	void build(int x, int y, int h, int l, void *f);
};

#endif
