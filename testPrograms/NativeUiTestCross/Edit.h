#ifndef EDIT_H
#define EDIT_H

#include <ma.h>
#include <maassert.h>
#include <mastring.h>
#include <IX_NATIVE_UI.h>
#include "Widget.h"


class Edit : public Widget {
public:
	Edit(int id, void *parent, char *str);
	~Edit();
	int getId();
	void *getInstance();

};

#endif
