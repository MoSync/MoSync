/*
 * Layout.cpp
 *
 *  Created on: Mar 16, 2010
 *      Author: romain
 */

#include "Layout.h"
using namespace MAUtil;

Layout::Layout(Frame *f) {
	parentFrame=f;
}

Layout::~Layout() {

}

void Layout::addWidget(Widget *w) {
	set.insert(w);
}

void Layout::build() {
	int x = 10;
	int y = 10;
	int h = 60;

	int l = ((short)(( maGetScrSize() ) >> 16)) - 20;

	for( Set<Widget*>::Iterator i = set.begin(); i != set.end(); i++) {
		(*i)->build(x, y, h, l, parentFrame->getInstance());
		y = y + h + 10;
	}

}
