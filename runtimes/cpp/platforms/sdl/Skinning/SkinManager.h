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
		DeviceSkin* createSkinFor(DeviceProfile* profile);
		void addSkinFactory(SkinFactory* factory);
		void removeSkinFactory(SkinFactory* factory);
	private:
		SkinManager();
		static SkinManager* sInstance;
		std::vector<SkinFactory*> mSkinFactories;
	};
}

#endif // _SKINFACTORY_H_