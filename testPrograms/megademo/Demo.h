/* Copyright (C) 2010 MoSync AB

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
