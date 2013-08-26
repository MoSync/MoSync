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
 * ListScreenListener.h
 *
 *  Created on: Sept 18, 2012
 *      Author: Spiridon Alexandru
 */

#ifndef LISTSCREENLISTENER_H_
#define LISTSCREENLISTENER_H_

/**
 * \brief Listener for list screen events.
 */
class ListScreenListener
{
public:
	/**
	 * This method is called when a list item is clicked.
	 * @param parentSection The parent section of the list view item clicked.
	 * @param item The list view item clicked.
	 */
	virtual void listScreenItemClicked(NativeUI::ListViewSection* &parentSection, NativeUI::ListViewItem* &listViewItem) = 0;
};


#endif /* LISTSCREENLISTENER_H_ */
