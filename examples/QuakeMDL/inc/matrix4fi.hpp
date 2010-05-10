/*
 * matrixfixp.hpp
 *
 *  Created on: 2009-jun-10
 *      Author: Ali Mosavian
 */

#ifndef __MATRIXFIXP_HPP__
#define __MATRIXFIXP_HPP__

#include <maapi.h>
#include "common.h"
#include "vector3f.hpp"
#include "vector3fi.hpp"

class Matrix4fi
{
private:
	fix32	m_matrix[4][4];

public:
	/**
	 * Constructor
	 */
	Matrix4fi ( bool init = true )
	{
		if ( init == true )
			identity( );
	}

	/**
	 * Constructor
	 */
	Matrix4fi ( const Matrix4fi &m )
	{
		*this = m;
	}


	/**
	 * Constructs a rotation (about x-axis) matrix
	 *
	 */
	void rotateX ( float32 rad )
	{
		fix32 sinf = FLT2FIX( sin( rad ) );
		fix32 cosf = FLT2FIX( cos( rad ) );

		identity( );

		m_matrix[1][1] =  cosf;
		m_matrix[1][2] = -sinf;

		m_matrix[2][1] = -sinf;
		m_matrix[2][2] =  cosf;
	}


	/**
	 * Constructs a rotation (about x-axis) matrix
	 *
	 */
	void rotateY ( float32 rad )
	{
		fix32 sinf = FLT2FIX( sin( rad ) );
		fix32 cosf = FLT2FIX( cos( rad ) );

		identity( );

		m_matrix[0][0] =  cosf;
		m_matrix[0][2] =  sinf;

		m_matrix[2][0] = -sinf;
		m_matrix[2][2] =  cosf;
	}


	/**
	 * Constructs a rotation (about x-axis) matrix
	 *
	 */
	void rotateZ ( float32 rad )
	{
		fix32 sinf = FLT2FIX( sin( rad ) );
		fix32 cosf = FLT2FIX( cos( rad ) );

		identity( );

		m_matrix[0][0] =  cosf;
		m_matrix[0][1] = -sinf;

		m_matrix[1][0] =  sinf;
		m_matrix[1][1] =  cosf;
	}


	/**
	 * Translation matrix
	 *
	 */
	void translate ( float x, float y, float z )
	{
		identity( );

		m_matrix[0][3] = FLT2FIX( x );
		m_matrix[1][3] = FLT2FIX( y );
		m_matrix[2][3] = FLT2FIX( z );
	}


	/**
	 * Constructs a look-at matrix
	 *
	 */
	void lookAt ( const Vector3f &eye,
				  const Vector3f &up,
				  const Vector3f &at )
	{
		Vector3f zaxis = (at - eye).normalize( );
		Vector3f xaxis = up.cross( zaxis ).normalize( );
		Vector3f yaxis = zaxis.cross( xaxis );

		// X, Y, X will now be used as a orthonormal basis
		m_matrix[0][0] = FLT2FIX( xaxis.m_x );
		m_matrix[0][1] = FLT2FIX( xaxis.m_y );
		m_matrix[0][2] = FLT2FIX( xaxis.m_z );
		m_matrix[0][3] = FLT2FIX( -xaxis.dot( eye.m_x )  );

		m_matrix[1][0] = FLT2FIX( yaxis.m_x );
		m_matrix[1][1] = FLT2FIX( yaxis.m_y );
		m_matrix[1][2] = FLT2FIX( yaxis.m_z );
		m_matrix[1][3] = FLT2FIX( -yaxis.dot( eye.m_y ) );

		m_matrix[2][0] = FLT2FIX( zaxis.m_x );
		m_matrix[2][1] = FLT2FIX( zaxis.m_y );
		m_matrix[2][2] = FLT2FIX( zaxis.m_z );
		m_matrix[2][3] = FLT2FIX( -zaxis.dot( eye.m_z )  );

		m_matrix[3][0] = 0;
		m_matrix[3][1] = 0;
		m_matrix[3][2] = 0;
		m_matrix[3][3] = FIX_ONE;
	}


	/**
	 * Loads identity matrix
	 */
	void identity ( void )
	{
		for ( int i = 0; i < 4; i++ )
		{
			m_matrix[i][0] = 0;
			m_matrix[i][1] = 0;
			m_matrix[i][2] = 0;
			m_matrix[i][3] = 0;
		}

		for ( int i = 0; i < 4; i++ )
			m_matrix[i][i] = FIX_ONE;
	}


	/**
	 * Matrix addition
	 */
	Matrix4fi operator + ( const Matrix4fi &m ) const
	{
		Matrix4fi r( false );

		for ( int i = 0; i < 4; i++ )
		{
			r.m_matrix[i][0] = m_matrix[i][0]+m.m_matrix[i][0];
			r.m_matrix[i][1] = m_matrix[i][1]+m.m_matrix[i][1];
			r.m_matrix[i][2] = m_matrix[i][2]+m.m_matrix[i][2];
			r.m_matrix[i][3] = m_matrix[i][3]+m.m_matrix[i][3];
		}

		return r;
	}


	/**
	 * Matrix subtraction
	 */
	Matrix4fi operator - ( const Matrix4fi &m ) const
	{
		Matrix4fi r( false );

		for ( int i = 0; i < 4; i++ )
		{
			r.m_matrix[i][0] = m_matrix[i][0]-m.m_matrix[i][0];
			r.m_matrix[i][1] = m_matrix[i][1]-m.m_matrix[i][1];
			r.m_matrix[i][2] = m_matrix[i][2]-m.m_matrix[i][2];
			r.m_matrix[i][3] = m_matrix[i][3]-m.m_matrix[i][3];
		}

		return r;
	}


	/**
	 * Matrix scalar multiplication
	 */
	Matrix4fi operator * ( fix32 s ) const
	{
		Matrix4fi r( false );

		for ( int i = 0; i < 4; i++ )
		{
			r.m_matrix[i][0] = FIXMULT( m_matrix[i][0], s );
			r.m_matrix[i][1] = FIXMULT( m_matrix[i][1], s );
			r.m_matrix[i][2] = FIXMULT( m_matrix[i][2], s );
			r.m_matrix[i][3] = FIXMULT( m_matrix[i][0], s );
		}

		return r;
	}


	/**
	 * Matrix multiply
	 */
	Matrix4fi operator * ( const Matrix4fi &m ) const
	{
		Matrix4fi r( false );

		for ( int i = 0; i < 4; i++ )
		{
			r.m_matrix[i][0] = FIXMULT( m_matrix[i][0], m.m_matrix[0][0] ) +
							   FIXMULT( m_matrix[i][1], m.m_matrix[1][0] ) +
							   FIXMULT( m_matrix[i][2], m.m_matrix[2][0] ) +
							   FIXMULT( m_matrix[i][3], m.m_matrix[3][0] );
			r.m_matrix[i][1] = FIXMULT( m_matrix[i][0], m.m_matrix[0][1] ) +
							   FIXMULT( m_matrix[i][1], m.m_matrix[1][1] ) +
							   FIXMULT( m_matrix[i][2], m.m_matrix[2][1] ) +
							   FIXMULT( m_matrix[i][3], m.m_matrix[3][1] );
			r.m_matrix[i][2] = FIXMULT( m_matrix[i][0], m.m_matrix[0][2] ) +
							   FIXMULT( m_matrix[i][1], m.m_matrix[1][2] ) +
							   FIXMULT( m_matrix[i][2], m.m_matrix[2][2] ) +
							   FIXMULT( m_matrix[i][3], m.m_matrix[3][2] );
			r.m_matrix[i][3] = FIXMULT( m_matrix[i][0], m.m_matrix[0][3] ) +
							   FIXMULT( m_matrix[i][1], m.m_matrix[1][3] ) +
							   FIXMULT( m_matrix[i][2], m.m_matrix[2][3] ) +
							   FIXMULT( m_matrix[i][3], m.m_matrix[3][3] );
		}

		return r;
	}


	/**
	 * Matrix scalar division
	 */
	Matrix4fi operator / ( fix32 s ) const
	{
		Matrix4fi r( false );

		for ( int i = 0; i < 4; i++ )
		{
			r.m_matrix[i][0] = FIXDIV( m_matrix[i][0], s );
			r.m_matrix[i][1] = FIXDIV( m_matrix[i][1], s );
			r.m_matrix[i][2] = FIXDIV( m_matrix[i][2], s );
			r.m_matrix[i][3] = FIXDIV( m_matrix[i][0], s );
		}

		return r;
	}

	/**
	 * MAtrix assignment
	 */
	void operator = ( const Matrix4fi &m )
	{
		for ( int i = 0; i < 4; i++ )
		{
			m_matrix[i][0] = m.m_matrix[i][0];
			m_matrix[i][1] = m.m_matrix[i][1];
			m_matrix[i][2] = m.m_matrix[i][2];
			m_matrix[i][3] = m.m_matrix[i][3];
		}
	}


	/**
	 * Matrix addition
	 */
	void operator += ( const Matrix4fi &m )
	{
		for ( int i = 0; i < 4; i++ )
		{
			m_matrix[i][0] += m.m_matrix[i][0];
			m_matrix[i][1] += m.m_matrix[i][1];
			m_matrix[i][2] += m.m_matrix[i][2];
			m_matrix[i][3] += m.m_matrix[i][3];
		}
	}


	/**
	 * Matrix subtraction
	 */
	void operator -= ( const Matrix4fi &m )
	{
		for ( int i = 0; i < 4; i++ )
		{
			m_matrix[i][0] -= m.m_matrix[i][0];
			m_matrix[i][1] -= m.m_matrix[i][1];
			m_matrix[i][2] -= m.m_matrix[i][2];
			m_matrix[i][3] -= m.m_matrix[i][3];
		}
	}


	/**
	 * Matrix mult
	 */
	void operator *= ( const Matrix4fi &m )
	{
		*this = *this*m;
	}


	/**
	 * Matrix scalar mult
	 */
	void operator *= ( fix32 s )
	{
		for ( int i = 0; i < 4; i++ )
		{
			m_matrix[i][0] = FIXMULT( m_matrix[i][0], s );
			m_matrix[i][1] = FIXMULT( m_matrix[i][1], s );
			m_matrix[i][2] = FIXMULT( m_matrix[i][2], s );
			m_matrix[i][3] = FIXMULT( m_matrix[i][0], s );
		}
	}


	/**
	 * Matrix scalar div
	 */
	void operator /= ( fix32 s )
	{
		for ( int i = 0; i < 4; i++ )
		{
			m_matrix[i][0] = FIXDIV( m_matrix[i][0], s );
			m_matrix[i][1] = FIXDIV( m_matrix[i][1], s );
			m_matrix[i][2] = FIXDIV( m_matrix[i][2], s );
			m_matrix[i][3] = FIXDIV( m_matrix[i][0], s );
		}
	}


	/**
	 * Matrix-vector multiply
	 *
	 */
	Vector3fi operator * ( const Vector3fi &v ) const
	{
		fix32 x, y, z;

		x = FIXMULT( m_matrix[0][0], v.m_x ) +
			FIXMULT( m_matrix[0][1], v.m_y ) +
			FIXMULT( m_matrix[0][2], v.m_z ) +
			m_matrix[0][3];

		y = FIXMULT( m_matrix[1][0], v.m_x ) +
			FIXMULT( m_matrix[1][1], v.m_y ) +
			FIXMULT( m_matrix[1][2], v.m_z ) +
			m_matrix[1][3];

		z = FIXMULT( m_matrix[2][0], v.m_x ) +
			FIXMULT( m_matrix[2][1], v.m_y ) +
			FIXMULT( m_matrix[2][2], v.m_z ) +
			m_matrix[2][3];

		return Vector3fi( x, y, z );
	}

	void print ( void )
	{
		for ( int i = 0; i < 4; i++ )
			printf( "{%f, %f, %f, %f}",
					FIX2FLT( m_matrix[i][0] ),
					FIX2FLT( m_matrix[i][1] ),
					FIX2FLT( m_matrix[i][2] ),
					FIX2FLT( m_matrix[i][3] ) );
	}

};

#endif /* MATRIXFIXP_HPP_ */
