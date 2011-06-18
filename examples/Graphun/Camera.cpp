#include "Camera.h"
//#include "System.h"
#include "OpenGLHelpers.h"

Camera::Camera(const vec3& position, const vec3& target, const vec3& up) :
mPosition(position), mTarget(target), mUpVector(up) {
}

void Camera::setPosition(const vec3& position) {
	mPosition = position;
}

void Camera::setTarget(const vec3& target) {
	mTarget = target;
}

void Camera::setUpVector(const vec3& up) {
	mUpVector = up;
}

vec3 Camera::getPosition() const {
	return mPosition;
}

vec3 Camera::getTarget() const {
	return mTarget;
}

vec3 Camera::getUpVector() const {
	return mUpVector;
}

void Camera::setCameraMatrix() {
	gluLookAt(
		mPosition.x,mPosition.y, mPosition.z,
		mTarget.x, mTarget.y, mTarget.z,
		mUpVector.x, mUpVector.y, mUpVector.z
		);
}


FirstPersonCamera::FirstPersonCamera() :
	Camera(vec3(), vec3(0, 0, 1.0f), vec3(0, 1.0, 0)),
	pitch(0.0f),
	yaw(0.0f),
	cameraOriginalLook(0, 0, -1), 
	cameraOriginalUp(0, 1, 0),
	cameraOriginalStrafe(1, 0, 0) {

		mUpVector = cameraOriginalUp;
		mPosition = 0.0;
		mDirection = cameraOriginalLook;
		mTarget = mPosition + mDirection;
		mStrafeDirection = cameraOriginalStrafe;
	//updateAxes();
}

void FirstPersonCamera::move(float step) {
	mPosition += mDirection*step;
	//updateAxes();
	mTarget = mPosition + mDirection;
}

void FirstPersonCamera::strafe(float step) {
	mPosition += mStrafeDirection*step;
	//updateAxes();
	mTarget = mPosition + mDirection;
}

void FirstPersonCamera::changePitch(float step) {
	pitch+=step;
	if(pitch>(1.553343f)) pitch = (1.553343f);
	if(pitch<(-1.553343f)) pitch = (-1.553343f);
	updateAxes();
}

void FirstPersonCamera::changeYaw(float step) {
	yaw+=step;
	updateAxes();
}

void FirstPersonCamera::updateAxes() {

	mDirection = cameraOriginalLook;
	mDirection.rotate(pitch, yaw, 0);
	mDirection.normalize();
		
	mStrafeDirection = cameraOriginalStrafe;
	mStrafeDirection.rotate(pitch, yaw, 0);
	mStrafeDirection.normalize();

	mUpVector = cameraOriginalUp;
	mUpVector.rotate(pitch, yaw, 0);
	mUpVector.normalize();

	mTarget = mPosition + mDirection;
}
