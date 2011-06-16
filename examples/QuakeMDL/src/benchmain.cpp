/*
 * main.cpp
 *
 *  Created on: 2009-jun-09
 *      Author: Ali Mosavian
 */

#include <ma.h>
#include <madmath.h>
#include <maassert.h>
#include <conprint.h>
#include "common.h"
#include "main.h"
#include "mdlfixed.hpp"
#include "matrix4fi.hpp"
#include "transformpipe.hpp"
#include "MAHeaders.h"

#define BENCH_PASSES	3
#define FRAMES_PASS		1000


int BenchMain ( void )
{
	//ConsoleLogging = 1;

	int 			tmrIni  = 0,
					tmrEnd  = 0;
	int 			renderedFrames = 0;
	int 			frameIndex = 0;
	float 			frameInterp = 0;
	float			rotRadY = 0.0f,
					rotRadX = 0.0f;
	float			avgFpsPerPass[BENCH_PASSES];

	Matrix4fi		transMatrix( false );
	MAExtent		size = maGetScrSize( );
	MDLFixed 		model( RES_MODEL_OGRE, false );
	TransformPipe	pipe( EXTENT_X( size ), EXTENT_Y( size ) );

	// Center model
	model.centerModel( );

	//
	// Rendering loop
	//
	printf( ".\n" );
	printf( "-------------\n" );
	printf( "Starting benchmark" );
	for ( int i = 0; i < BENCH_PASSES; i++ )
	{
		//
		// Start pass
		//
		printf( "    - Pass %d/%d\n", i+1, BENCH_PASSES );
		renderedFrames = 0;
		tmrIni = maGetMilliSecondCount( );
		while ( true )
		{
			//
			// Construct transformation pipe
			//
			pipe.resetPipe( );

			transMatrix.rotateX( (float)-M_PI/2 );
			pipe.addTransform( transMatrix );
			transMatrix.rotateY( (float)M_PI/4 );
			pipe.addTransform( transMatrix );

			transMatrix.rotateX( rotRadX );
			pipe.addTransform( transMatrix );
			transMatrix.rotateY( rotRadY );
			pipe.addTransform( transMatrix );
			transMatrix.translate( 0, 0, 130 );
			pipe.addTransform( transMatrix );


			//
			// Animate, transform and project model
			//
			model.drawFrameLerp( frameIndex, FLT2FIX( frameInterp ), pipe );
			frameInterp += 0.25f;
			if ( frameInterp >= 1.0f )
			{
				frameIndex  = (frameIndex+1) % model.getFrameCount( );
				frameInterp = 0.0f;
			}


			// Count frames
			renderedFrames++;
			if ( renderedFrames == FRAMES_PASS )
				break;

			//
			// Handle input
			//
			MAEvent e;
			while ( maGetEvent( &e ) )
			{
				if ( e.type == EVENT_TYPE_CLOSE ||
					 e.type == EVENT_TYPE_KEY_PRESSED ||
					 e.type == EVENT_TYPE_POINTER_PRESSED )
					maExit( 0 );
			}
		}

		// Calculate average fps for pass
		tmrEnd  = maGetMilliSecondCount( );
		int denom = (tmrEnd-tmrIni);
		if(denom == 0) denom = 1;
		avgFpsPerPass[i] = renderedFrames*1000.0f/denom;
	}

	float averageFps = 0.0f;
	for ( int i = 0; i < BENCH_PASSES; i++ )
		averageFps += avgFpsPerPass[i];
	averageFps /= (float)BENCH_PASSES;
	printf( "Benchmark done\nAverage fps %.1f\n", averageFps );
	printf( "-------------\n\n" );
	printf( ".\n" );

	FREEZE;
}
