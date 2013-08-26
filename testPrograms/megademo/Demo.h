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
