/*
 * mdlfixed.h
 *
 *  Created on: 2009-jun-09
 *      Author: Ali Mosavian
 */
#ifndef 	__MDLFLOAT_H__
#define 	__MDLFLOAT_H__

#include <maapi.h>
#include "common.h"
#include "vector3fi.hpp"
#include "transformpipe.hpp"


#ifndef NULL
#define NULL 0
#endif


/**
 * Represents a triangle indice
 */
class MDLTri
{
public:
	uint16	m_v1;
	uint16	m_v2;
	uint16	m_v3;
	bool	m_front;

	/**
	 * Constructor
	 *
	 * @param	v1	Index of first vertex
	 * @param	v2 	Index of second vertex
	 * @param	v3	Index of third vertex
	 */
	MDLTri ( uint16 v1 = 0,
			 uint16 v2 = 0,
			 uint16 v3 = 0,
			 bool front = true )
	: m_v1( v1 ),
	  m_v2( v2 ),
	  m_v3( v3 ),
	  m_front( front )
	{
	};
};


/**
 * Represents a frame
 *
 */
class MDLFrame
{
public:
	uint32		m_index;
	char		m_name[16];
	Vector3fi	m_boundMin;
	Vector3fi	m_boundMax;
	Vector3fi *	m_vertexList;
	uint8 *		m_normList;

	/**
	 * Constructor
	 *
	 */
	MDLFrame ( void )
	{
		memset( m_name, 0, 16 );
		m_vertexList = NULL;
		m_name[15] = 0;
	}

	/**
	 * Destructor
	 *
	 */
	~MDLFrame ( void )
	{
		if ( m_vertexList != NULL )
			delete[] m_vertexList;

		if ( m_normList != NULL )
			delete[] m_normList;
	}

};

class MDLFixed
{
private:
	Vector3f	m_eye;
	uint16		m_vtxCount;
	uint16		m_triCount;
	uint16		m_frameCount;
	MDLTri *	m_triList;
	MDLFrame *	m_frameList;
	Vector3fi *	m_normList;
	bool		m_doRender;


public:
	/**
	 * Constructor
	 * Loads the model
	 */
	MDLFixed ( MAHandle h,
               bool r = true );

	/**
	 * Destructor
	 * Deletes the allocated memory
	 *
	 */
	virtual ~MDLFixed ( void );


	/**
	 * Transforms, projects (using the pipe) and renders a frame.
	 *
	 * @param i			Index of the frame
	 * @param p			The transformation pipe to use
	 */
	void drawFrame ( uint32 i,
					 const TransformPipe &p );

	/**
	 * Linearly interpolates between two frames, transforms
	 * and projects the frame (using the pipe) and renders it
	 * on the screen.
	 *
	 * @param i			Index of the first (of two) frame
	 * @param interp	A fixed point number in the range 0-1
	 * 					used for interpolating
	 * @param p			The transformation pipe to use
	 */
	void drawFrameLerp ( uint32 i,
					     fix32 interp,
					     const TransformPipe &p );


	/**
	 * Finds center of the (vertex) mass
	 *
	 */
	Vector3f calcCenter ( void );

	/**
	 * Centers the model by subtracting the center of the
	 * (vertex) mass from all the frames.
	 *
	 */
	void centerModel ( void );


	/**
	 * Returns the eye position as specified in the model.
	 */
	Vector3f getEyePos ( void )
	{
		return m_eye;
	}


	/**
	 * Returns the frame count
	 *
	 */
	uint32 getFrameCount ( void )
	{
		return m_frameCount;
	}
};

#endif /* MDLFLOAT_H_ */
;
