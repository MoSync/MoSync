#ifndef _DEMO_H_
#define _DEMO_H_

#include <ma.h>

struct Surface {
	int w, h;
	byte* pixels;

	void blit(Surface *src, int x, int y);
};

class Effect {
public:
	virtual void init(Surface* surf);
	virtual void prepare();
	virtual void render(Surface* surface, float time);
};

void addEffect(Effect* effect);
void setEffectIndex(int i);
int getEffectIndex();
void initEffects(Surface* surf);
void renderEffect(Surface* surface);


#endif
