#ifndef MANAGER_H
#define MANAGER_H

#include <ma.h>
#include <maassert.h>
#include <mastring.h>
#include <IX_NATIVE_UI.h>
#include <MAUtil\Map.h>
#include "Widget.h"



class Manager
{
public:
  static Manager& Instance();
  void addWidget(Widget *w);
  void dispatch(const MAEvent &);
  int getNextId();

private:
  Manager();                                 // Private constructor
  ~Manager();
  Manager(const Manager&);                 // Prevent copy-construction
  Manager& operator=(const Manager&);      // Prevent assignment
  MAUtil::Map<int, Widget*> map;
  int generatedId;

};

#endif
