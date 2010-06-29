/*
 * mdlfixed.cpp
 *
 *  Created on: 2009-jun-09
 *      Author: Ali Mosavian
 */
#include <conprint.h>
#include "vector2i.hpp"
#include "mdlfixed.hpp"
#include "transformpipe.hpp"
#include "endian/streamreader.hpp"
#include "anorms.h"



/**
 * Finds center of the (vertex) mass
 *
 */
Vector3f MDLFixed::calcCenter ( void )
{
	Vector3fi c;

	for ( uint32 i = 0; i < m_vtxCount; i++ )
		c += m_frameList[0].m_vertexList[i];

	c.m_x /= m_vtxCount;
	c.m_y /= m_vtxCount;
	c.m_z /= m_vtxCount;

	return Vector3f( FIX2FLT( c.m_x ),
					 FIX2FLT( c.m_y ),
					 FIX2FLT( c.m_z ) );
}



/**
 * Centers the model by subtracting the center of the
 * (vertex) mass from all the frames.
 *
 */
void MDLFixed::centerModel ( void )
{
	Vector3fi c = calcCenter( );

	for ( uint32 i = 0; i < m_frameCount; i++ )
		for ( uint32 j = 0; j < m_vtxCount; j++ )
			m_frameList[i].m_vertexList[j] -= c;
}



/**
 * Transforms, projects (using the pipe) and renders a frame.
 *
 * @param i			Index of the frame
 * @param p			The transformation pipe to use
 */
void MDLFixed::drawFrame ( uint32 i,
						   const TransformPipe &p )
{
	Vector2i u, v;
	Vector2i p1, p2, p3;
	MDLFrame &f = m_frameList[i % m_frameCount];

	for ( uint32 j = 0; j < m_triCount; j++ )
	{
		// Project
		p.project( p1.m_x, p1.m_y, f.m_vertexList[m_triList[j].m_v1] );
		p.project( p2.m_x, p2.m_y, f.m_vertexList[m_triList[j].m_v2] );
		p.project( p3.m_x, p3.m_y, f.m_vertexList[m_triList[j].m_v3] );

		// Backface cull
		u = p2-p1;
		v = p3-p1;
		if ( u.cross( v ) < 0 )
			continue;

		// Draw
		if ( m_doRender == true )
		{
			maLine( p1.m_x, p1.m_y, p2.m_x, p2.m_y );
			maLine( p2.m_x, p2.m_y, p3.m_x, p3.m_y );
			maLine( p3.m_x, p3.m_y, p1.m_x, p1.m_y );
		}
	}
}


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
void MDLFixed::drawFrameLerp ( uint32 i,
							   fix32 interp,
							   const TransformPipe &p )
{
	Vector2i u, v;
	Vector2i p1, p2, p3;
	MDLFrame &f1 = m_frameList[(i+0) % m_frameCount];
	MDLFrame &f2 = m_frameList[(i+1) % m_frameCount];

	for ( uint32 j = 0; j < m_triCount; j++ )
	{
		Vector3fi &v1_1 = f1.m_vertexList[m_triList[j].m_v1];
		Vector3fi &v1_2 = f1.m_vertexList[m_triList[j].m_v2];
		Vector3fi &v1_3 = f1.m_vertexList[m_triList[j].m_v3];
		Vector3fi &v2_1 = f2.m_vertexList[m_triList[j].m_v1];
		Vector3fi &v2_2 = f2.m_vertexList[m_triList[j].m_v2];
		Vector3fi &v2_3 = f2.m_vertexList[m_triList[j].m_v3];

		// Project
		p.project( p1.m_x, p1.m_y, v1_1 + (v2_1-v1_1)*interp );
		p.project( p2.m_x, p2.m_y, v1_2 + (v2_2-v1_2)*interp );
		p.project( p3.m_x, p3.m_y, v1_3 + (v2_3-v1_3)*interp );

		// Backface cull
		u = p2-p1;
		v = p3-p1;
		if ( u.cross( v ) < 0 )
			continue;

		// Draw tri
		if ( m_doRender == true )
		{
			maLine( p1.m_x, p1.m_y, p2.m_x, p2.m_y );
			maLine( p2.m_x, p2.m_y, p3.m_x, p3.m_y );
			maLine( p3.m_x, p3.m_y, p1.m_x, p1.m_y );
		}
	}
}



/**
 * Constructor
 * Loads the model
 */
MDLFixed::MDLFixed ( MAHandle h,
		             bool r )
{
	uint32			i, j;
	Vector3fi		scale;
	Vector3fi		trans;
	uint32			skinCount;
	uint32			skinWidth;
	uint32			skinHeight;
	StreamReader 	*stream;

	m_doRender = r;

	// Create stream
	stream = StreamReader::createStreamReader( StreamReader::LITTLE,
											   h, 0 );

	printf( "Starting to load\n" );
	//
	// Read header
	//
	if ( stream->readUInt32( ) != 0x4F504449 )
		FatalError( (char *)"Not a QUAKE 1 MDL file" );
	if ( stream->readUInt32( ) != 6 )
		FatalError( (char *)"Not version 6" );

	// Scale
	scale.m_x = FLT2FIX( stream->readFloat32( ) );
	scale.m_y = FLT2FIX( stream->readFloat32( ) );
	scale.m_z = FLT2FIX( stream->readFloat32( ) );

	// Global coordinate
	trans.m_x = FLT2FIX( stream->readFloat32( ) );
	trans.m_y = FLT2FIX( stream->readFloat32( ) );
	trans.m_z = FLT2FIX( stream->readFloat32( ) );

	//Bounding radius, not needed
	stream->readFloat32( );

	// Viewpoint
	m_eye.m_x = stream->readFloat32( );
	m_eye.m_y = stream->readFloat32( );
	m_eye.m_z = stream->readFloat32( );

	// num_skins, skin_width, skin_height
	skinCount = stream->readUInt32( );
	skinWidth = stream->readUInt32( );
	skinHeight= stream->readUInt32( );

	// Vertices, triangles, frame count
	m_vtxCount = stream->readUInt32( );
	m_triCount = stream->readUInt32( );
	m_frameCount = stream->readUInt32( );

	// sync_type, flags, size
	stream->readUInt32( );
	stream->readUInt32( );
	stream->readFloat32( );

	printf( "Vertex #: %u\n", m_vtxCount );
	printf( "Tri #: %u\n", m_triCount );
	printf( "Frame #: %u\n", m_frameCount );
	printf( "Skin #: %u\n", skinCount );
	printf( "Skin w: %u\n", skinWidth );
	printf( "Skin h: %u\n", skinHeight );

	//
	// Read textures (1 byte attribute + 8bpp)
	//
	i = 0;
	while ( i < skinCount )
	{
		uint32 group = stream->readUInt32( );
		if ( group == 0 )
		{
			stream->seek( stream->getPos( ) + skinWidth*skinHeight );
			i++;
		}
		else
		{
			uint32 nb = stream->readUInt32( );
			stream->seek( stream->getPos( ) + 4*nb + skinWidth*skinHeight*nb );
			i += nb;
		}
	}

	//
	// Read texture coordinates
	//
	printf( "Skipping texture coordinates\n" );
	stream->seek( stream->getPos( ) + 12*m_vtxCount );
	/*
	for ( i = 0; i < m_vtxCount; i++ )
	{
		uint32 onseam = stream->readUInt32( );
		uint32 s	   = stream->readUInt32( );
		uint32 t	   = stream->readUInt32( );
		printf( "Coord %u", i );
	}
	*/

	//
	// Read triangles
	//
	printf( "Loading triangles\n" );
	m_triList = new MDLTri[m_triCount];
	for ( i = 0; i < m_triCount; i++ )
	{

		m_triList[i].m_front = stream->readUInt32( ) != 0;
		m_triList[i].m_v1 = stream->readUInt32( );
		m_triList[i].m_v2 = stream->readUInt32( );
		m_triList[i].m_v3 = stream->readUInt32( );
	}


	//
	// Read frames
	//
	printf( "Loading frames\n" );
	uint8 *buffer = new uint8[m_vtxCount*4] ;
	m_frameList = new MDLFrame[m_frameCount];
	for ( i = 0; i < m_frameCount; i++ )
	{
		MDLFrame &frame = m_frameList[i];
		frame.m_normList   = new uint8[m_vtxCount];
		frame.m_vertexList = new Vector3fi[m_vtxCount];

		//uint32 type = stream->readUInt32( );
		stream->readUInt32( );

		frame.m_boundMin.m_x = INT2FIX( stream->readUInt8( ) );
		frame.m_boundMin.m_y = INT2FIX( stream->readUInt8( ) );
		frame.m_boundMin.m_z = INT2FIX( stream->readUInt8( ) );
		stream->readUInt8( );

		frame.m_boundMax.m_x = INT2FIX( stream->readUInt8( ) );
		frame.m_boundMax.m_y = INT2FIX( stream->readUInt8( ) );
		frame.m_boundMax.m_z = INT2FIX( stream->readUInt8( ) );
		stream->readUInt8( );

		stream->read( frame.m_name, 16 );
		printf( "%u: name: %s\n", i, frame.m_name );

		stream->read( buffer, m_vtxCount*4 );
		for ( j = 0; j < m_vtxCount; j++ )
		{
			frame.m_vertexList[j].m_x = INT2FIX( buffer[j*4+0] );
			frame.m_vertexList[j].m_y = INT2FIX( buffer[j*4+1] );
			frame.m_vertexList[j].m_z = INT2FIX( buffer[j*4+2] );
			frame.m_normList[j]       = buffer[j*4+3];

			frame.m_vertexList[j] = frame.m_vertexList[j]*scale+trans;
		}
	}

	// Load normals
	m_normList = new Vector3fi[m_vtxCount];
	for ( i = 0; i < sizeof( norms )/(3*sizeof(float)); i++ )
	{
		m_normList[i].m_x = FLT2FIX( norms[i][0] );
		m_normList[i].m_y = FLT2FIX( norms[i][1] );
		m_normList[i].m_z = FLT2FIX( norms[i][2] );
	}

	delete[] buffer;
	delete stream;
}

/**
 * Destructor
 * Deletes the allocated memory
 *
 */
MDLFixed::~MDLFixed (  )
{
	delete m_triList;
	delete[] m_frameList;
}
