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
