/*
 * ImageScreen.h
 *
 *  Created on: Aug 19, 2011
 *      Author: gabi
 */

#ifndef IMAGESCREEN_H_
#define IMAGESCREEN_H_


#include <NativeUI/Screen.h>
#include <NativeUI/Image.h>

#include "FacebookDemoScreen.h"

namespace FacebookDemoGUI
{

class ImageScreen: public FacebookDemoScreen
{
public:
	ImageScreen(FacebookDemoScreen *prev);
	ImageScreen();

	void setImage(MAHandle handle);

	~ImageScreen();

private:
	NativeUI::Image 		*mImage;
};

}//namespace FacebookDemoGUI

#endif /* IMAGESCREEN_H_ */
