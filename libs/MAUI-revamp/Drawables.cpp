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

/** 
* \file Drawables.cpp
* \brief Common types of Drawable.
* \author Fredrik Eldh
*/

#include "Drawables.h"
#include <MAUtil/Graphics.h>

void MAUI::ImageDrawable::draw(int left, int top, int width, int height) {
	Gfx_drawImage(mImage, left, top);
}

void MAUI::ColorDrawable::draw(int left, int top, int width, int height) {
	maSetColor(mColor);
	Gfx_fillRect(left, top, width, height);
}
