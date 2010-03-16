#ifndef WIDGET_H
#define WIDGET_H

#include <ma.h>
#include <IX_NATIVE_UI.h>
#include "ActionListener.h"



class Widget {
public:
	Widget();
	Widget(int id);
	~Widget();
	int getId();
	void *getInstance();
	virtual void processEvent(const MAEvent &);
	virtual void build(int x, int y, int h, int l, void *f);
	void addActionListener(ActionListener *a);

	bool operator < ( const Widget & ) const;
	bool operator == ( const Widget & ) const;

protected:
	int myid;
	void *me;

	char *text;

	ActionListener *actionListener;
};

#endif
