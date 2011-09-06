/*
 * ImageScreen.cpp
 *
 *  Created on: Aug 19, 2011
 *      Author: gabi
 */

#include "ImageScreen.h"

namespace FacebookDemoGUI
{

ImageScreen::ImageScreen(FacebookDemoScreen *prev):FacebookDemoScreen(prev)
{
	mImage = new NativeUI::Image();
}

ImageScreen::ImageScreen(): FacebookDemoScreen(NULL), mImage(NULL)
{
}

void ImageScreen::setImage(MAHandle handle)
{
	mImage->setImage(handle);
}

ImageScreen::~ImageScreen()
{
	delete mImage;
}



}//namespace FacebookDemoGUI
