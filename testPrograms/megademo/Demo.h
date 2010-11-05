#ifndef _DEMO_H_
#define _DEMO_H_

#include <ma.h>


#define BLIT_ADD 0
#define BLIT_REPLACE 1

struct Surface {
	int w, h;
	byte* pixels;

	void blit(Surface *src, int x, int y, int flags);
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
