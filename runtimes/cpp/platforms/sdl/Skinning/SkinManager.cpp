#include "SkinManager.h"

namespace MoRE {

	SkinManager* SkinManager::sInstance = 0;
	
	SkinManager* SkinManager::getInstance() {
		if(!sInstance) sInstance = new SkinManager();
		return sInstance;
	}
	
	DeviceSkin* SkinManager::createSkinFor(DeviceProfile* profile) {
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
		if(bestSkinFactory != NULL)
			return bestSkinFactory->createInstanceFor(profile);
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
