/*
 * Layout.h
 *
 *  Created on: Mar 16, 2010
 *      Author: romain
 */

#ifndef LAYOUT_H_
#define LAYOUT_H_
#include "Widget.h"
#include "Frame.h"
#include <MAUtil\Set.h>

class Layout {
public :
	Layout(Frame *f);
	~Layout();
	void addWidget(Widget *w);
	void build();

private:
	MAUtil::Set<Widget*> set;
	Frame *parentFrame;
};

#endif /* LAYOUT_H_ */
