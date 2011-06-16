/*
 * main.cpp
 *
 *  Created on: 2009-jun-09
 *      Author: Ali Mosavian
 */

#include <ma.h>
#include <madmath.h>
#include <conprint.h>
#include "common.h"
#include "mdlfixed.hpp"
#include "matrix4fi.hpp"
#include "transformpipe.hpp"
#include "MAHeaders.h"
#include "main.h"


int RenderMain ( void )
{
	int 			tmrLast = 0;
	int 			currFps = 0,
					lastFps = 0;
	int 			frameIndex = 0;
	float 			frameInterp = 0;
	float			rotRadY = 0.0f,
					rotRadX = 0.0f;
	char			textBuffer[128];
	bool 			keyTable[4] = { false };
	bool focus = true;

	Matrix4fi		transMatrix( false );
	MAExtent		size = maGetScrSize( );
	MDLFixed 		model( RES_MODEL_OGRE );
	TransformPipe	pipe( EXTENT_X( size ), EXTENT_Y( size ) );


	// Center model
	model.centerModel( );

	tmrLast = maGetMilliSecondCount( );

	//
	// Rendering loop
	//
	while ( true )
	{
		//
		// Handle input
		//
		MAEvent e;
		while ( maGetEvent( &e ) )
		{
			if ( e.type == EVENT_TYPE_CLOSE ||
				 e.type == EVENT_TYPE_KEY_PRESSED ||
				 e.type == EVENT_TYPE_POINTER_PRESSED )
				maExit(0);
			else if(e.type == EVENT_TYPE_FOCUS_LOST)
				focus = false;
			else if(e.type == EVENT_TYPE_FOCUS_GAINED)
				focus = true;

			else if ( e.type == EVENT_TYPE_KEY_PRESSED )
			{
				if ( e.key == MAK_LEFT )
					keyTable[MAK_LEFT-MAK_UP] = true;
				else if ( e.key == MAK_RIGHT )
					keyTable[MAK_RIGHT-MAK_UP] = true;
				else if ( e.key == MAK_UP )
					keyTable[MAK_UP-MAK_UP] = true;
				else if ( e.key == MAK_DOWN )
					keyTable[MAK_DOWN-MAK_UP] = true;
			}
			else if ( e.type == EVENT_TYPE_KEY_RELEASED )
			{
				if ( e.key == MAK_LEFT )
					keyTable[MAK_LEFT-MAK_UP] = false;
				else if ( e.key == MAK_RIGHT )
					keyTable[MAK_RIGHT-MAK_UP] = false;
				else if ( e.key == MAK_UP )
					keyTable[MAK_UP-MAK_UP] = false;
				else if ( e.key == MAK_DOWN )
					keyTable[MAK_DOWN-MAK_UP] = false;
			}
		}

		if(focus) {
			if ( keyTable[MAK_LEFT-MAK_UP] )
				rotRadY -= (float)M_PI / 60;
			else if ( keyTable[MAK_RIGHT-MAK_UP] )
				rotRadY += (float)M_PI / 60;
			else if ( keyTable[MAK_UP-MAK_UP] )
				rotRadX -= (float)M_PI / 60;
			else if ( keyTable[MAK_DOWN-MAK_UP] )
				rotRadX += (float)M_PI / 60;

			rotRadX = (rotRadX >= 2*(float)M_PI) ? rotRadX-2*(float)M_PI : rotRadX;
			rotRadY = (rotRadY >= 2*(float)M_PI) ? rotRadY-2*(float)M_PI : rotRadY;

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


			// Clear screen
			maSetColor( 0 );
			maFillRect( 0, 0, EXTENT_X( size ), EXTENT_Y( size ) );

			//
			// Animate and render model
			//
			maSetColor( 0xffffff );
			model.drawFrameLerp( frameIndex, FLT2FIX( frameInterp ), pipe );
			frameInterp += 0.25f;
			if ( frameInterp >= 1.0f )
			{
				frameIndex  = (frameIndex+1) % model.getFrameCount( );
				frameInterp = 0.0f;
			}

			// Draw fps to screen
			sprintf( textBuffer, "fps: %d", lastFps );
			maDrawText( 0, 0, textBuffer );

			// Update screen and keep backlight on
			maUpdateScreen( );
			maResetBacklight( );

			// FPS counter
			currFps++;
			if ( tmrLast+1000 <= maGetMilliSecondCount( ) )
			{
				lastFps = currFps;
				currFps = 0;
				tmrLast = maGetMilliSecondCount( );
				lprintfln( "fps: %d", lastFps );
			}
		} else {	// no focus
			maWait(0);
		}
	}
}
