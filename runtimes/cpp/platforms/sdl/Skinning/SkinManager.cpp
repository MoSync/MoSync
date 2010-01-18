/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
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
