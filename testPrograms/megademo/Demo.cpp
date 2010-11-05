#include "Demo.h"
#include <MAUtil/Vector.h>
using namespace MAUtil;

static Vector<Effect*> sEffects;
static int sEffectIndex = 0;

void addEffect(Effect* effect) {
	sEffects.add(effect);
}

void setEffectIndex(int i) {
	if(i<0) i = 0;
	if(i>=sEffects.size()) i = sEffects.size()-1;
	sEffectIndex = i;
	if(!sEffects.size()) return;
	sEffects[sEffectIndex]->prepare();
}

int getEffectIndex() {
	return sEffectIndex;
}

void initEffects(Surface* surf) {
	for(int i = 0; i < sEffects.size(); i++) {
		sEffects[i]->init(surf);
	}
	setEffectIndex(0);
}

void renderEffect(Surface* surface) {
	if(sEffects.size()==0) return;
	sEffects[sEffectIndex]->render(surface, (float)maGetMilliSecondCount()*0.001);
}
