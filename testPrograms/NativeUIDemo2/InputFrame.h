#ifndef _INPUT_FRAME_
#define _INPUT_FRAME_

#include "Frame.h"
#include "Manager.h"
#include "ActionListener.h"
#include "Label.h"
#include "Edit.h"
#include "Button.h"
#include "Layout.h"

class InputFrame : public Frame, public ActionListener {
public:
	InputFrame(int Id);
	~InputFrame();
	void onCreate (int id);
	void onClick (int id);
	void onItemSelected (int id, int index);
};

#endif
