#ifndef _CATEGORIES_FRAME_
#define _CATEGORIES_FRAME_

#include "ListFrame.h"
#include "Manager.h"
#include "ActionListener.h"
#include "DisplayFrame.h"

class ImagesFrame : public ListFrame, public ActionListener {
public:
	ImagesFrame(int Id);
	~ImagesFrame();
	void onCreate (int id);
	void onClick (int id);
	void onItemSelected (int id, int index);
};

#endif
