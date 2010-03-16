#include "Widget.h"
#include "Manager.h"

Widget::Widget() {
	myid=Manager::Instance().getNextId();
}

Widget::Widget(int id) {
	myid=id;
}

Widget::~Widget() {

}

int Widget::getId() {
	return myid;
}

void *Widget::getInstance() {
	return me;
}

void Widget::processEvent(const MAEvent & e) {

}

void Widget::build(int x, int y, int h, int l, void *f) {

}

void Widget::addActionListener(ActionListener *a) {
	actionListener=a;
}

bool Widget::operator == ( const Widget & w ) const {
	if( w.myid == myid ) return true;
	else return false;
}

bool Widget::operator < ( const Widget & w ) const {
	if( w.myid < myid ) return true;
	else return false;
}
