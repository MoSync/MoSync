#ifndef LABEL_H
#define LABEL_H

#include <ma.h>
#include <maassert.h>
#include <mastring.h>
#include <IX_NATIVE_UI.h>
#include "Widget.h"


class Label : public Widget {
public:
	Label(int id, void *parent, char *str);
	~Label();
	int getId();
	void *getInstance();
};

#endif
