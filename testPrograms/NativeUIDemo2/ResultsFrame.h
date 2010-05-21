#ifndef _RESULTS_FRAME_
#define _RESULTS_FRAME_

#include "Frame.h"
#include "Manager.h"
#include "ActionListener.h"
#include "Label.h"
#include "Edit.h"
#include "Button.h"
#include "Layout.h"
#include <MAUtil/util.h>
#include <MAUtil/String.h>
#include <MAUtil/Set.h>
#include "Converter.h"

class ResultsFrame : public Frame, public ActionListener {
public:
	ResultsFrame(int Id, const MAUtil::String& category);
	~ResultsFrame();
	void onCreate (int id);
	void onClick (int id);
	void onItemSelected (int id, int index);
private:
	Layout *mLayout;
	MAUtil::Vector<MAUtil::String> mVec;
};

#endif
