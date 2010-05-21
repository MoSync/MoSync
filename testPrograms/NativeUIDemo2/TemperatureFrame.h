#ifndef _TEMPERATURE_FRAME_
#define _TEMPERATURE_FRAME_

#include "ListFrame.h"
#include "Manager.h"
#include "ActionListener.h"
#include "InputFrame.h"

class TemperatureFrame : public ListFrame, public ActionListener {
public:
	TemperatureFrame(int Id);
	~TemperatureFrame();
	void onCreate (int id);
	void onClick (int id);
	void onItemSelected (int id, int index);
};

#endif
