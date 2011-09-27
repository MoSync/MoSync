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

/** \file main.cpp
*
* This example shows how to retrieve accelerometer data and
* use it in a meaningful manner (rotate 3d objects).
* The 3d rendering is in a seperate module so if you dare
* you can try and plug in your own 3d renderer and see
* how your 3d world looks from different angles using
* the accelerometer. To do this you just implement the
* Renderer interface and the two functions 'init' and
* 'render'. Look at the default renderer implementation
* called SimpleRenderer to see how it is done.
*
* \author Niklas Nummelin
*
*/

#include <MAUtil/GLMoblet.h>
#include <conprint.h>
#include <madmath.h>
#include "SimpleRenderer.h"

using namespace MAUtil;

// A simple struct that represents
// a vector in 3-dimensional space.
struct Vector3 {
	Vector3() :
		x(0.0), y(0.0), z(0.0) {

	}
	Vector3(float x, float y, float z) :
		x(x), y(y), z(z) {

	}

	float x, y, z;
};

// A simple low pass filter used to
// smoothen the noisy accelerometer
// data.
struct LowPassFilter {
	LowPassFilter() :
		// this constant sets the cutoff for the filter.
		// It must be a value between 0 and 1, where
		// 0 means no filtering (everything is passed through)
		// and 1 that no signal is passed through.
		a(0.80f)
	{
		b = 1.0f - a;
	}

	Vector3 filter(const Vector3& in) {
		previousState.x = (in.x * b) + (previousState.x * a);
		previousState.y = (in.y * b) + (previousState.y * a);
		previousState.z = (in.z * b) + (previousState.z * a);
		return previousState;
	}

	float a, b;
	Vector3 previousState;
};

/**
 * A Moblet is a high-level class that defines the
 * behaviour of a MoSync program.
 */
class MyMoblet: public GLMoblet, public SensorListener {
private:
	Renderer* mRenderer;
	LowPassFilter mFilter;
	Vector3 mRotation;

public:
	/**
	 * Initialize the application in the constructor.
	 */
	MyMoblet() :
		GLMoblet(GLMoblet::GL1) {
	}

	/**
	 * Called when a key is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode) {
		if (MAK_BACK == keyCode || MAK_0 == keyCode) {
			// Call close to exit the application.
			close();
		}
	}

	void init() {

		// Add ourselves as a sensor listener.
		Environment::getEnvironment().addSensorListener(this);

		// Start the gyro or accelerometer sensor with a predefined rate
		// for sending events, suitable for games.

		if(maSensorStart(SENSOR_TYPE_ACCELEROMETER, SENSOR_RATE_GAME) < 0) {
			maPanic(1, "Failed to initialize accelerometer!");
		}

		// Make an instance of the SimpleRenderer class that just
		// renders a simple finite plane (quad).
		mRenderer = new SimpleRenderer();
		mRenderer->init(EXTENT_X(maGetScrSize()), EXTENT_Y(maGetScrSize()));
	}

	// Function that uses trigonometric math to convert accelerometer and compass
	// data into euler angles expressed as radians. This simple method won't always
	// give correct results (complex complex math may fix that for you).
	Vector3 convertAccelerometerAndCompassDataToRadians(const Vector3& a, float compass) {
		float g = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
		return Vector3(atan2(a.y, -a.z), asin(a.x / -g), compass);
	}

	// Convert euler angles from radians to degrees.
	float convertRadiansToDegrees(float radians) {
		return (radians * 360.0f / (2.0f * M_PI));
	}

	void sensorEvent(MASensor a) {

		// If the type of sensor data received is from the accelerometer
		if (a.type == SENSOR_TYPE_ACCELEROMETER) {

			// Filter the accelerometer gravity vector.
			Vector3 f = mFilter.filter(Vector3(a.values[0], a.values[1], a.values[2]));

			// And calculate the rotations. We don't pass the compass angle, just the
			// accelerometer gravity vector.
			mRotation = convertAccelerometerAndCompassDataToRadians(f, 0.0f);

			// Set the rotation.
			mRenderer->setRotation(
					convertRadiansToDegrees(mRotation.x),
					convertRadiansToDegrees(mRotation.y),
					convertRadiansToDegrees(mRotation.z)
					);
		}
	}

	void draw() {
		mRenderer->draw();
	}
};

/**
 * Entry point of the program. The MAMain function
 * needs to be declared as extern "C".
 */
extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
}
