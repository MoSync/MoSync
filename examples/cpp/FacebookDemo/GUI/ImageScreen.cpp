/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/*
 * ImageScreen.cpp
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
