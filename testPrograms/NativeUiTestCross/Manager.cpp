#include "Manager.h"
using namespace MAUtil;

Manager::Manager() {

}

Manager::~Manager() {

}

Manager& Manager::Instance() {
  static Manager m;
  return m;
}

void Manager::addWidget(Widget* w) {
	//MAUtil::Pair Pair<Widget, int> ();
	//map.insert(w, w->);

	map.insert((w->getId()), w);


}

void Manager::dispatch(const MAEvent& e) {
	//MAUtil::Pair Pair<Widget, int> ();
	//map.insert(w, w->);
	Map<int, Widget*>::Iterator i = map.find(e.lo_wparam);

	(*i).second->processEvent(e);
}

int Manager::getNextId() {
	generatedId++;
	return generatedId;
}
