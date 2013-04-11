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

#include "Touch.h"
#include <GLES2/gl2.h>
#include <mavsprintf.h>

namespace MoGraph
{

	#define FABS(x)	((x<0.0f)?-(x):(x))
	/**
	 * \brief TouchInput, constructor
	 */
	TouchInput::TouchInput()
	{
		glm::vec2 clr(0.0f,0.0f);
		glm::vec3 clr3(0.0f,0.0f,0.0f);
		mScalePosArr[0] = clr;
		mScalePosArr[1] = clr;
		mScaleOldPos 	= 1.0f;
		mScalePos 		= 1.0f;
		mRotPos 		= clr3;
		mRotSpeed 		= clr3;
		mDelta 			= 1.0f;	// must have the same size as the initial set up. 1.0f.
		mWidth 			= 640;	// constructor defaults
		mHeight 		= 480;
	}

	/**
	 * \brief, 	multitouchPressEvent, Optional, This fwd function is called with a coordinate when a pointer is pressed.
	 *
	 * @param p,		touch pointer
	 * @param touchId,	touch id (finger id)
	 */
	void TouchInput::multitouchPressEvent(MAPoint2d p, int touchId)
	{
//		lprintfln("multitouchPressEvent=(%d,%d) id=%d",p.x,p.y,touchId);
		Touch t(p,touchId,Pressed);
		mTouch[touchId] = t;		// creates or overwrites.
		mTouchActive++;
	}

	/**
	 * \brief, multitouchMoveEvent, Optional, This fwd function is called with a coordinate when a pointer is moved.
	 * @param p,		touch pointer
	 * @param touchId,	touch id (finger id)
	 */
	void TouchInput::multitouchMoveEvent(MAPoint2d p, int touchId)
	{
//		lprintfln("multitouchMoveEvent=(%d,%d) id=%d",p.x,p.y,touchId);
		Touch &t 	= mTouch[touchId];
		t.mOldPos 	= t.mPos;
		t.mPos 		= glm::vec2(p.x,p.y);
		t.mId 		= touchId;
		t.mState 	= Moving;
	}

	/**
	 * \brief, multitouchReleaseEvent, Optional, This fwd function is called with a coordinate when a pointer is released.
	 * @param p,		touch pointer
	 * @param touchId,	touch id (finger id)
	 */
	void TouchInput::multitouchReleaseEvent(MAPoint2d p, int touchId)
	{
//		lprintfln("multitouchReleaseEvent=(%d,%d) id=%d",p.x,p.y,touchId);
		Touch &t 	= mTouch[touchId];
		t.mOldPos 	= t.mPos;
		t.mPos 		= glm::vec2(p.x,p.y);
		t.mId 		= touchId;
		t.mState 	= Released;
		mTouchActive--;
	}

	/**
	 * \brief getSpeed,  get swipe speed at single touch (local helper function)
	 * @param t,		Touch param
	 * @param speed,	input/output speed
	 * @return vec2		output speed
	 */
	glm::vec3 TouchInput::getSpeed(Touch &t, glm::vec3 &speed)
	{

		glm::vec3 p,op;
		// Single touch.
		if (mTouchActive == 0)
		{
			p.x 	= 0.0f;
			p.y 	= 0.0f;
			p.z		= 0.0f;
			speed 	*= 0.98f;	// TODO : this needs delta time
		}
		else if (mTouchActive == 1)
		{
			glm::vec2 scale((float)mWidth,(float)mHeight);

			p		= glm::vec3(mTouch[0].mPos.x/scale.x, mTouch[0].mPos.y/scale.y,0.0f);
			op		= glm::vec3(mTouch[0].mOldPos.x/scale.x,mTouch[0].mOldPos.y/scale.y,0.0f);
			speed 	= p - op;
		}

		if (speed.x > 1.0f)
			speed.x = 1.0f;

		if (speed.x < -1.0f)
			speed.x = -1.0f;

		if (speed.y > 1.0f)
			speed.y = 1.0f;

		if (speed.y < -1.0f)
			speed.y = -1.0f;
/*
		if (speed.z > 1.0f)
			speed.z = 1.0f;

		if (speed.z < -1.0f)
			speed.z = -1.0f;
*/
		return speed;
	}

	/**
	 * \brief init, initiate  class
	 * @param width
	 * @param height
	 */
	void TouchInput::init(int width,int height)
	{
		mWidth 		= width;
		mHeight 	= height;
	}

	/**
	 * \brief update, for every frame
	 * takes care of borth scale and swipe functionality.
	 */
	void TouchInput::update()
	{
		const float PI = M_PI;

		if (mTouchActive>1)
		{
			if (mTouchActive > 2)		// 2 touch (scale and rotation)
			{
				lprintfln("Multi x>2 touch not supported yet! = %d",mTouchActive);
				return;
			}

			// double touch scale

			glm::vec2 scale((float)mWidth,(float)mHeight);

			mScalePosArr[0] = mTouch[0].mPos/scale;		// Normalize position (1) 0..1
			mScalePosArr[1] = mTouch[1].mPos/scale;		// Normalize position (2) 0..1
			glm::vec2 diff = mScalePosArr[0] - mScalePosArr[1];	// get delta between the thouches (from both positions)

			float delta = glm::length(diff);			// get distance from delta (hypetenuse)

			delta = (delta < 0.0f)? -delta: delta;		// abs delta
			delta = (delta < 0.1f)? 0.1f: delta;		// min/max delta (used for scale output)
			delta = (delta > 2.0f)? 2.0f: delta;

			// Handle scale and rotation with double touch during its movement
			if (mTouch[0].mState == MoGraph::Moving && mTouch[1].mState == MoGraph::Moving)
			{
				mScalePos 		= delta;					// store delta output to scalePos
				float oldZ 		= mRotPos.z;				// oldZ rot pos.
				mRotPos.z 		= -atan2(diff.y,diff.x);	// calculate the Z angle in rad,
				float speedZ 	= mRotPos.z - oldZ;			// get rotational speed

															// handle angular pos wrapp.
				if (speedZ < -PI)
					speedZ = (mRotPos.z + 2.0f*PI) - oldZ;
				else if (speedZ > PI)
					speedZ = (mRotPos.z - 2.0f*PI) - oldZ;
															// clamp angular speed
				const float maxSpeedZ = 0.2f;
				if (speedZ > maxSpeedZ)
					speedZ = maxSpeedZ;

				if (speedZ < -maxSpeedZ)
					speedZ = -maxSpeedZ;

				mRotSpeed.z = speedZ;						// store to ouput speed
			}
			else	// 2 touch but not not in moving
			{
				mScalePos = mScaleOldPos = delta;			// use delta output to scale position
			}

			mDelta -= mScaleOldPos - mScalePos;				// get scale speed from 2 pos old and current
			if (mDelta > 2.0f)								// clamp scale speed (mDelta)
				mDelta = 2.0f;
			else if (mDelta < 0.1f)
				mDelta = 0.1f;
		}
		else	// Single touch (swipe)
		{
			mRotSpeed 	= getSpeed(mTouch[0],mRotSpeed);	// get Swipe speed
			mRotPos 	+= mRotSpeed;						// use swipe speed for rot speed.
		}

		mScaleOldPos = mScalePos;							// store mSaleOldPos for next time
	}

}
