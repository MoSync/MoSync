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

#include "Item.h"

using namespace MAUtil;

namespace Hybris {

	Item::Item(int id, Sprite *sprite, int x, int y) : 
		id(id), 
		sprite(sprite), 
		p(x, y),
		frame(0) {
	}
			
	int Item::getId() {
		return id;
	}
	
	const Point& Item::getPosition() const {
		return p;
	}
	
	Sprite* Item::getSprite() {
		return sprite;
	}		
			
	bool Item::collideWith(Item *i) {
		int sw = sprite->getWidth();
		int sh = sprite->getHeight();
		Rect o1 = Rect(p.x-(sw>>1), p.y-(sh>>1), sw, sh);
		const Point& o2p = i->getPosition();
		sw = i->getSprite()->getWidth();
		sh = i->getSprite()->getHeight();
		Rect o2 = Rect(o2p.x-(sw>>1), o2p.y-(sh>>1), sw, sh);
		return o1.overlaps(o2);
	}
			
	void Item::setFrame(int frame) {
		this->frame = frame;
	}
			
	void Item::draw() {
		sprite->draw(p.x, p.y, frame);
	}	

	void Item::setPosition(int x, int y) {
		p.x = x;
		p.y = y;
	}	
	
	void Item::moveUp(int amt) {
		p.y-=amt;
	}
	
	void Item::moveLeft(int amt) {
		p.x-=amt;
	}
	
	void Item::moveRight(int amt) {
		p.x+=amt;
	}
	
	void Item::moveDown(int amt) {
		p.y+=amt;
	}	
	
	void Item::setAnimation(int startFrame, int startOfAnim, int numFrames) {
		this->startOfAnim = startOfAnim;
		this->numFrames = numFrames; 
		frame = startFrame;
	}
	
	void Item::stepAnimationForward() {
		frame += 1;
		frame=((frame-startOfAnim)%numFrames)+startOfAnim;
	}
	
	void Item::stepAnimationBackwards() {
		frame -= 1;
		if(frame<startOfAnim) frame = startOfAnim+numFrames-1;
		else frame=((frame-startOfAnim)%numFrames)+startOfAnim;
	}	
	
				
}
