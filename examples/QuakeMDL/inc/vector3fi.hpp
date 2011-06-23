/*
 * vector3fi.hpp
 *
 *  Created on: 2009-jun-10
 *      Author: Ali Mosavian
 */

#ifndef				__VECTOR3FI_H__
#define				__VECTOR3FI_H__

#include "common.h"
#include "vector3f.hpp"
#include <mavsprintf.h>

class Vector3fi
{
public:
	fix32	m_x;
	fix32	m_y;
	fix32	m_z;

	/**
	 * Constructor
	 *
	 */
	Vector3fi ( fix32 x = 0,
			    fix32 y = 0,
			    fix32 z = 0 )
	: m_x( x ),
	  m_y( y ),
	  m_z( z )
	{
	};

	/**
	 * Constructor
	 *
	 */
	Vector3fi ( const Vector3fi &v )
	: m_x( v.m_x ),
	  m_y( v.m_y ),
	  m_z( v.m_z )
	{
	};


	/**
	 * Constructor
	 *
	 */
	Vector3fi ( const Vector3f &v )
	{
		m_x = FLT2FIX( v.m_x );
		m_y = FLT2FIX( v.m_y );
		m_z = FLT2FIX( v.m_z );
	};


	/**
	 * Add two vectors
	 *
	 */
	Vector3fi operator + ( const Vector3fi &v ) const
	{
		return Vector3fi( m_x+v.m_x, m_y+v.m_y, m_z+v.m_z );
	}


	/**
	 * Subtract two vectors
	 *
	 */
	Vector3fi operator - ( const Vector3fi &v ) const
	{
		return Vector3fi( m_x-v.m_x, m_y-v.m_y, m_z-v.m_z );
	}


	/**
	 * Per element multiplication
	 *
	 */
	Vector3fi operator * ( const Vector3fi &v ) const
	{
		return Vector3fi( FIXMULT(m_x, v.m_x),
						  FIXMULT(m_y, v.m_y),
						  FIXMULT(m_z, v.m_z) );
	}


	/**
	 * Multiply with scalar
	 *
	 */
	Vector3fi operator * ( fix32 s ) const
	{
		return Vector3fi( FIXMULT( m_x, s ),
						  FIXMULT( m_y, s ),
						  FIXMULT( m_z, s ) );
	}


	/**
	 * Assignment
	 *
	 */
	Vector3fi operator = ( const Vector3fi &v )
	{
		m_x = v.m_x;
		m_y = v.m_y;
		m_z = v.m_z;
		return *this;
	}

	/**
	 * Assignment
	 *
	 */
	Vector3fi operator += ( const Vector3fi &v )
	{
		m_x += v.m_x;
		m_y += v.m_y;
		m_z += v.m_z;
		return *this;
	}

	/**
	 * Assignment
	 *
	 */
	Vector3fi operator -= ( const Vector3fi &v )
	{
		m_x -= v.m_x;
		m_y -= v.m_y;
		m_z -= v.m_z;
		return *this;
	}

	/**
	 * Per element multiplication
	 *
	 */
	fix32 dot ( const Vector3fi &v ) const
	{
		return FIXMULT(m_x, v.m_x)+
			   FIXMULT(m_y, v.m_y)+
			   FIXMULT(m_z, v.m_z);
	}

	/**
	 * Convert to a Vector3f
	 */
	Vector3f toVector3f ( void )
	{
		return Vector3f( FIX2FLT( m_x ),
						 FIX2FLT( m_y ),
						 FIX2FLT( m_z ) );
	}

	/**
	 * Debug print
	 */
	void print ( void )
	{
		lprintfln( "{%f, %f, %f}",
				   FIX2FLT( m_x ),
				   FIX2FLT( m_y ),
				   FIX2FLT( m_z ) );
	}
};

#endif
