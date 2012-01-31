/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vec.h"

class Camera {
public:
	Camera(const vec3& position, const vec3& target, const vec3& up);

	void setPosition(const vec3& position);
	void setTarget(const vec3& target);
	void setUpVector(const vec3& up);
	vec3 getPosition() const;
	vec3 getTarget() const;
	vec3 getUpVector() const;

	void setCameraMatrix();

protected:
	vec3 mPosition;
	vec3 mTarget;
	vec3 mUpVector;

};

class FirstPersonCamera : public Camera {
public:
	FirstPersonCamera();
	void move(float step);
	void strafe(float step);
	void changePitch(float step);
	void changeYaw(float step);

private:
	vec3 cameraOriginalUp;
	vec3 cameraOriginalLook;
	vec3 cameraOriginalStrafe;
	vec3 mDirection;

	float pitch, yaw;
	vec3 mStrafeDirection;
	void updateAxes();
};


#endif // _CAMERA_H_
