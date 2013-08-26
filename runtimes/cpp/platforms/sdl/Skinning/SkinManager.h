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

#ifndef _SKINMANAGER_H_
#define _SKINMANAGER_H_

#include "SkinFactory.h"

namespace MoRE {

	/**
	 * This class manages creation of skins by keeping a
	 * vector of SkinFactories, and using their compatibility
	 * information to select the best one to use for a
	 * particular DeviceProfile.
	 */
	class SkinManager {
	public:
		static SkinManager* getInstance();
		DeviceSkin* createSkinFor(const DeviceProfile* profile);
		void addSkinFactory(SkinFactory* factory);
		void removeSkinFactory(SkinFactory* factory);
	private:
		SkinManager();
		static SkinManager* sInstance;
		std::vector<SkinFactory*> mSkinFactories;
	};
}

#endif // _SKINFACTORY_H_
