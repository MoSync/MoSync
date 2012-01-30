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

/*
 * transformpipe.hpp
 *
 *  Created on: 2009-jun-11
 *      Author: Ali Mosavian
 */

#ifndef __TRANSFORMPIPE_HPP__
#define __TRANSFORMPIPE_HPP__

#include "common.h"
#include "vector3fi.hpp"
#include "matrix4fi.hpp"

/**
 * A simple transformation and projection pipe.
 *
 */
class TransformPipe
{
private:
	sint32			m_swdth;
	sint32			m_shght;
	sint32			m_dist;
	sint32			m_aspect;
	Matrix4fi		m_transform;

public:
	/**
	 * Constructor
	 *
	 * @param w		Screen width
	 * @param h		Screen height
	 */
	TransformPipe ( sint32 w,
					sint32 h )
	: m_transform( true )
	{
		m_swdth = w/2;
		m_shght = h/2;
		m_aspect = FLT2FIX( (float32)w/h );
		m_dist   = 150;
	}


	/**
	 * Resets pipe transformations
	 *
	 */
	void resetPipe ( void )
	{
		m_transform.identity( );
	}


	/**
	 * Adds a transformation to the front of the pipe
	 *
	 * @param m		Transformation to add
	 */
	void addTransform ( const Matrix4fi &m )
	{
		m_transform = m*m_transform;
	}


	/**
	 * Transform a vector and projects it to screen space
	 *
	 * @param sx	The projected screen x coordinate
	 * @param sy	The projected screen y coordinate
	 * @param v		Fixed point vector to transform and project
	 */
	void project( sint32 &sx,
				  sint32 &sy,
				  const Vector3fi &v ) const
	{
		Vector3fi vt = m_transform*v;
		if(vt.m_z == 0) vt.m_z = 1;
		sx = m_swdth + (m_dist*vt.m_x)/vt.m_z;
		sy = m_shght - m_dist*FIXMULT(m_aspect, vt.m_y)/vt.m_z;

	}
};


#endif /* TRANSFORMPIPE_HPP_ */
