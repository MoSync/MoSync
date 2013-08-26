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
* \file RelativeLayout.h 
* \brief Relative layout of child widgets.
* \author Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_RELATIVE_LAYOUT_H_
#define _SE_MSAB_MAUI_RELATIVE_LAYOUT_H_

#include "Widget.h"

namespace MAUI {

	/** 
	 * \brief Relative layout of child widgets.
	 *
	 * This widget is a container that layouts its children
	 * using a relative layout policy.
	 *
	 * TODO: This class does not seem to be implemented yet.
	 */
	class RelativeLayout : public Widget {
	public:
		/**
		 * Constructor.
		 * \param x Left coordinate of layout container.
		 * \param y Top coordinate of layout container.
		 * \param width Width of layout container.
		 * \param height Height of layout container.
		 */
		RelativeLayout(int x=0, int y=0, int width=0, int height=0);

		/**
		 * \retgurn true if this layout container is transparent,
		 * false if not.
		 */
		virtual bool isTransparent() const;
		
	protected:
		/**
		 * TODO: Comment purpose of this method.
		 */
		virtual void drawWidget();
	};
}

#endif /* _SE_MSAB_MAUI_RELATIVE_LAYOUT_H_ */
