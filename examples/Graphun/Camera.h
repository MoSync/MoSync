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
