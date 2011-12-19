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

/**
 * @file SoundListener.h
 * @author Bogdan Iusco.
 */

#ifndef SOUNDLISTENER_H_
#define SOUNDLISTENER_H_

#include <MAUtil/Environment.h>
#include <maapi.h>

using namespace MAUtil;

class SoundListener: public TimerListener
{

public:
	/**
	 * Constructor.
	 */
	SoundListener();

	/**
	 * Destructor.
	 */
	~SoundListener();

	/**
	 * Start playing sound.
	 */
	virtual void start();

private:
	virtual void runTimerEvent();
};


#endif /* SOUNDLISTENER_H_ */
