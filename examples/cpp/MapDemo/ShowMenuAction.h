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

#ifndef SHOWMENUACTION_H_
#define SHOWMENUACTION_H_

#include "Action.h"
#include "IActionSource.h"
#include <MAP/MemoryMgr.h>
#include "Menu.h"

using namespace MAPUtil;

namespace MapDemoUI
{
	//=========================================================================
	class ShowMenuAction: public Action
	//=========================================================================
	{
	public:
		ShowMenuAction( IActionSource* source );

		virtual ~ShowMenuAction( );
		//
		// Action overrides
		//
		virtual const char* getShortName( ) const;

		virtual Action* clone( ) const
		{
			return newobject( ShowMenuAction, new ShowMenuAction( mSource ) );
		}

	protected:
		//
		// Action protected overrides
		//
		virtual void performPrim( );

	private:
		IActionSource* mSource;
		Menu* mMenu;
	};
}

#endif // SHOWMENUACTION_H_
