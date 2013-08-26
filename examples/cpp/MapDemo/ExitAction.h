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

#ifndef EXITACTION_H_
#define EXITACTION_H_

#include "Action.h"
#include "MobletEx.h"
#include <MAP/MemoryMgr.h>

using namespace MAPUtil;

namespace MapDemoUI
{
	//=========================================================================
	// Forces exit of the app.
	//
	class ExitAction: public Action
	//=========================================================================
	{
	public:
		ExitAction( MobletEx* moblet );

		virtual ~ExitAction( );
		//
		// Action overrides
		//
		virtual const char* getShortName( ) const;

		virtual Action* clone( ) const
		{
			return newobject( ExitAction, new ExitAction( mMoblet ) );
		}

	protected:
		//
		// Action protected overrides
		//
		virtual void performPrim( );

	private:
		MobletEx* mMoblet;
	};
}

#endif // EXITACTION_H_
