#ifndef _CATEGORIES_FRAME_
#define _CATEGORIES_FRAME_

#include "ListFrame.h"
#include "Manager.h"
#include "ActionListener.h"
#include "TemperatureFrame.h"
#include "WeightFrame.h"

class CategoriesFrame : public ListFrame, public ActionListener {
public:
	CategoriesFrame(int Id);
	~CategoriesFrame();
	void onCreate (int id);
	void onClick (int id);
	void onItemSelected (int id, int index);
};

#endif
