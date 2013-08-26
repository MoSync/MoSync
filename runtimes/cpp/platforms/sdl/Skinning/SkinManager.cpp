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

#include "SkinManager.h"

namespace MoRE {

	SkinManager* SkinManager::sInstance = 0;
	
	SkinManager* SkinManager::getInstance() {
		if(!sInstance) sInstance = new SkinManager();
		return sInstance;
	}
	
	DeviceSkin* SkinManager::createSkinFor(const DeviceProfile* profile) {
		SkinFactory::Fitness bestFitness = SkinFactory::SKIN_FITNESS_NONE;
		SkinFactory* bestSkinFactory = NULL;

		for(
			std::vector<SkinFactory*>::const_iterator i = mSkinFactories.begin(); 
			i != mSkinFactories.end(); 
			++i
		) {
			SkinFactory::Fitness currentFitness = (*i)->getFitnessFor(profile);
			if(currentFitness > bestFitness) {
				bestFitness = currentFitness;
				bestSkinFactory = (*i);
			}
		}
		if(bestSkinFactory != NULL) {
			DeviceSkin* skin = bestSkinFactory->createInstanceFor(profile);
			if(!skin) return NULL;
			if(!skin->init()) {
				delete skin;
				return NULL;
			}
			return skin;
		}
		else
			return NULL;
	}
	
	void SkinManager::addSkinFactory(SkinFactory* factory) {
		mSkinFactories.push_back(factory);
	}
	
	void SkinManager::removeSkinFactory(SkinFactory* factory) {
		for(
			std::vector<SkinFactory*>::iterator i = mSkinFactories.begin(); 
			i != mSkinFactories.end(); 
			++i
		) {
			if(factory == (*i)) {
				mSkinFactories.erase(i);
				return;
			}
		}
	}
	
	SkinManager::SkinManager() {
	}

}
