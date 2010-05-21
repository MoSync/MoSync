#ifndef _WEIGHT_FRAME_
#define _WEIGHT_FRAME_

#include "ListFrame.h"
#include "Manager.h"
#include "ActionListener.h"
#include "InputFrame.h"

class WeightFrame : public ListFrame, public ActionListener {
public:
	WeightFrame(int Id);
	~WeightFrame();
	void onCreate (int id);
	void onClick (int id);
	void onItemSelected (int id, int index);
};

#endif
