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
* \file Drawable.h
* \brief Interface for drawables.
* \author Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_DRAWABLE_H_
#define _SE_MSAB_MAUI_DRAWABLE_H_

namespace MAUI {

	/* TODO: maybe this should handle caching etc.
	* Basic implementations should be 
	* WidgetSkin, ImageDrawable, FilledDrawable.
	* This function should use the Gfx_ calls.
	* The class should maybe have a callback/listener
	* if it wants to request a redraw (for animated drawables..).
	*/
	
	
	class Drawable {
	public:
		Drawable() : mUseCaching(true) {}

		// if applicable (images and filled rectangles don't use this).
		void setUseCaching(bool useCaching = true);
		bool isUsingCache() const;

		virtual void draw(int x, int y, int width, int height) = 0;
		virtual bool isTransparent() const = 0;
	
	protected:
		bool mUseCaching;
	};
}

#endif // _SE_MSAB_MAUI_DRAWABLE_H_
