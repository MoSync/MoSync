/*
 * Copyright (C) 2013 MoSync AB

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

#ifndef TOUCH_H_
#define TOUCH_H_

#include <MAUtil/Environment.h>
#include <glm/glm.hpp>
#include "Utils.h"

namespace MoGraph
{
	/**
	 * \brief TouchState event
	 */
	enum TouchState{ Idle, Pressed, Moving, Released };

	/**
	 * \brief Touch is a sturct that contains all info around a touch event.
	 */
	struct Touch
	{
		/**
		 * \brief Touch,	Constructor
		 */
		Touch() : mPos(glm::vec2(0.0f,0.0f)),mOldPos(glm::vec2(0.0f,0.0f)), mId(0), mState(Idle) {}
		/**
		 * \brief Touch,	Constructor
		 * @param p,		touch position in glm::vec2
		 * @param id,		touch id
		 * @param state		state see TouchState
		 */
		Touch(MAPoint2d &p, int id, TouchState state) : mPos(glm::vec2((float)p.x,(float)p.y)),mOldPos(glm::vec2((float)p.x,(float)p.y)), mId(id), mState(state) {}

		glm::vec2 	mPos;			// Position of a touch
		glm::vec2	mOldPos;		// previous position of a touch
		int			mId;			// touch index
		TouchState 	mState;			// state if event
	};

	typedef std::pair<int, Touch> TouchPair;

	/**
	 * \brief TouchInput class used for handling the swipes for rotation and double swipe for scaling
	 */
	class TouchInput : public MAUtil::PointerListener
	{
	protected:
		int				mTouchActive;
		std::hash_map<int, Touch> mTouch;	// table of touch
		glm::vec3 		mRotSpeed;			// calculated rotation speed
		glm::vec3 		mRotPos;			// calculated rotation position
		glm::vec2 		mScalePosArr[2];	// Scale (reqd two touch position)
		float 			mScalePos;			// distance between 2 positions
		float 			mScaleOldPos;		// previous diance between 2 posions
		float 			mDelta;				// output scale speed
		float			mOldZ;
		int 			mWidth;
		int				mHeight;			// Screen resolution in ABS form e.g. 640,480

		/**
		 * \brief getSpeed,	gets the speed of touch from 2 different frames
		 * \note handles double touch speed=scale and single touch speed
		 * @param t,		touch struct containing events and positions
		 * @param speed,	input/output, speed.
		 * @return speed	returns
		 */
		glm::vec3 getSpeed(Touch &t, glm::vec3 &speed);

	public:
		/**
		 * \brief TouchInput,	Constructor
		 */
		TouchInput();
		/**
		 * \brief ~TouchInput, Destructor
		 */
		virtual ~TouchInput(){}
		/**
		 * \brief init, initiate the touch system by setting up screen limits.
		 * @param mWidth,	screen width
		 * @param mHeight,	screen height
		 */
		virtual void init(int mWidth, int mHeight);
		/**
		 * \brief update,	update swipe,scale to output rotation or scale
		 */
		virtual void update();
		/**
		 * \brief getScale,	scale for screen
		 * @return
		 */
		virtual float getScale() {return mDelta;}
		/**
		 * \brief getAngularOrientation, in degrees
		 * @return yaw angle and pitch angle (y,x) in prio order
		 */
		virtual glm::vec3 getAngularOrientation() {return mRotPos;}
		/**
		 * \brief getAngularSpeed, in degrees
		 * @return yaw speed angle and pitch speed angle (y,x) in prio order
		 */
		virtual glm::vec3 getAngularSpeed() {return mRotSpeed;}
		// optional multi touch is in use here
		/**
		 * \brief multitouchPressEvent,	fwd events from the event handler to here
		 * \note optional, if user wants to use TouchInput class to handle this
		 * @param p,		point in 2D
		 * @param touchId,	touch index
		 */
		virtual void multitouchPressEvent(MAPoint2d p, int touchId);
		/**
		 * \brief multitouchMoveEvent, fwd events from the event handler to here
		 * \note optional, if user wants to use TouchInput class to handle this
		 * @param p,		point in 2D
		 * @param touchId,	touch index
		 */
		virtual void multitouchMoveEvent(MAPoint2d p, int touchId);
		/**
		 * \brief multitouchReleaseEvent, fwd events from the event handler to here
		 * \note optional, if user wants to use TouchInput class to handle this
		 * @param p,		point in 2D
		 * @param touchId,	touch index
		 */
		virtual void multitouchReleaseEvent(MAPoint2d p, int touchId);
		// optional input handler pointers not in use
		/**
		 * \brief pointerPressEvent
		 * \note not in use
		 * @param p
		 */
		virtual void pointerPressEvent(MAPoint2d p) {};
		/**
		 * \brief pointerMoveEvent
		 * \note not in use
		 * @param p
		 */
		virtual void pointerMoveEvent(MAPoint2d p) {};
		/**
		 * \brief pointerReleaseEvent
		 * \note not in use
		 * @param p
		 */
		virtual void pointerReleaseEvent(MAPoint2d p) {};
	};

}

#endif /* TOUCH_H_ */
