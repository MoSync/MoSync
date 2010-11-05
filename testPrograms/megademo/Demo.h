#ifndef _DEMO_H_
#define _DEMO_H_

#include <ma.h>

struct Surface {
	int w, h;
	byte* backBuffer;
};

class Effect {
public:
	virtual void init();
	virtual void prepare();
	virtual void render(Surface* surface, float time);
};

void addEffect(Effect* effect);
void setEffectIndex(int i);
int getEffectIndex();
void initEffects();
void renderEffect(Surface* surface);


#endif
