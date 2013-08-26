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
