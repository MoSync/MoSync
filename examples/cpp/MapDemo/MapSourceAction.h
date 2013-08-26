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

#ifndef MAPSOURCEACTION_H_
#define MAPSOURCEACTION_H_

#include "Action.h"
#include <MAP/MapWidget.h>
#include <MAP/MemoryMgr.h>

using namespace MapDemoUI;
using namespace MAP;

namespace MapDemo
{
	//=========================================================================
	//
	// Repositions map of a MapWidget to the specified location
	//
	class MapSourceAction: public Action
	//=========================================================================
	{
	public:
		MapSourceAction( MapWidget* widget, MapSource* source, const char* label );

		virtual	~MapSourceAction( );
		//
		// Action overrides
		//
		virtual const char* getShortName( ) const;

		virtual Action* clone( ) const
		{
			return newobject( MapSourceAction, new MapSourceAction( mWidget, mSource, mLabel ) );
		}

	protected:
		//
		// Action protected overrides
		//
		virtual void performPrim( );

	private:
		MapWidget* mWidget;
		MapSource* mSource;
		const char* mLabel;
	};
}

#endif // MAPSOURCEACTION_H_
