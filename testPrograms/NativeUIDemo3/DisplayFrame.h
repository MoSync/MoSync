#ifndef _RESULTS_FRAME_
#define _RESULTS_FRAME_

#include "Frame.h"
#include "Manager.h"
#include "ActionListener.h"
#include "Image.h"
#include "Layout.h"


class DisplayFrame : public Frame, public ActionListener {
public:
	DisplayFrame(int Id, int img);
	~DisplayFrame();
	void onCreate (int id);
	void onClick (int id);
	void onItemSelected (int id, int index);
private:
	Layout *mLayout;
	Image *mImage;

};

#endif
