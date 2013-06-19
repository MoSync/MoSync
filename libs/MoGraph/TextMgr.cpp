/*
 * TextMgr.cpp
 *
 *  Created on: Mar 14, 2013
 *      Author: CodeArt
 */

#include "Graph.h"
#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <mavsprintf.h>

namespace MoGraph
{

	const char *getFormatDecimalStr(int d)
	{
		switch (d)
		{
			case 0:	return "%.0f";
			case 1:	return "%.1f";
			case 2:	return "%.2f";
			case 3:	return "%.3f";
			case 4:	return "%.4f";
			case 5:	return "%.5f";
			default: return "%.0f";
		}
		return "%.0f";
	}

	/**
	 * \brief TextMgr::draw() OBSOLETE..
	 * \note not in use due to RenderText class used by Graph does this job
	 */
	void TextMgr::draw()
	{

	}

	/**
	 * \brief TextMgr::init,	initiate all text in 3D graph
	 * \note there is defaulted text array of 3 items
	 * description of "X-Axis" "Y-AXIS" ("Y-AXIS" optional)
	 * if needed more text just push_back new text entries for mText
	 * and fill out the struct as desired
	 */
	void TextMgr::init()
	{



		// to do, align text with axis..
		const GraphDesc &desc = mScene->getGraphDesc();

		Text t;
		t.mRotate 			= glm::vec3(0.0f,0.0f,0.0f);
		t.mTextFlagX		= Text::CENTER_LEFT;
		t.mTextFlagY		= Text::CENTER_Y;

		float gridZ 		= mScene->getGridZ();
		float centerX		= mScene->getCx();
		float centerZ		= mScene->getCz();
		float size			= desc.font->getInfo()->m_size;	// get character x & y size...
		float orgScale 		= desc.gridStepYLines/size; // constant size of scalem depends on size of character.			//0.5f/(size*desc.gridStepYLines);	//desc.gridStepYLines/(10.0f*lineHeight); //500.0f;
		float scale			= orgScale;
		float bound			= mScene->getBoundScale();
		float dcenterX		= centerX*bound;
		float dcenterZ		= centerZ*bound;

		glm::vec2 scaleXZ(scale,scale);
		glm::vec3 pos(dcenterX, 0.0f,dcenterZ);
		glm::vec4 color(1.0f,1.0f,1.0f,1.0f);

		t.mColor 	= color;
		t.mText		= "MoSync 3D Graph Library 0.8 Beta";		// Subtitle
		t.mScale	= scaleXZ;
		t.mPos		= pos;

		switch (desc.flagGridLines)
		{
//			case MoGraph::DEFAULT_GRIDS:
//			{
//				mTextArray.push_back(t);
//			}
//			break;
			case MoGraph::MIRRORED_GRIDS:
			{
				t.mPos.y	= -(desc.gridYLines * desc.gridStepYLines);
				mTextArray.push_back(t);
				t.mPos.y	= pos.y;	// restore
			}
			break;
			case MoGraph::OFFSET_GRIDS:
			{
				t.mPos.y	= desc.gridOffsetStartLine;
				mTextArray.push_back(t);
				t.mPos.y	= pos.y;	// restore
			}
			break;
			default:
			{
				mTextArray.push_back(t);
			}
			break;
		}


		// Axis Text are scaled dependent on the entire grid size.

		float totGridHeight = desc.gridStepYLines * desc.gridYLines + desc.gridStepYLines;
		if (desc.flagGridLines == MoGraph::OFFSET_GRIDS)
		{
			totGridHeight += desc.gridOffsetStartLine;	// adjusting top position occording to offset
		}

		t.mPos.y	+= totGridHeight;
		t.mText		= "Y-Axis";		// Subtitle
		t.mScale	= scaleXZ;
		mTextArray.push_back(t);

		t.mText		= "X-Axis";		// Subtitle
		t.mPos 		= glm::vec3(-dcenterX, 0.0f,dcenterZ);
		t.mTextFlagX = Text::CENTER_RIGHT;
		mTextArray.push_back(t);


		if (desc.bUseGridValue)
		{
			char buf[64];
			const char *formatDecimals = getFormatDecimalStr(desc.gridDecimals);
			float step 			= desc.gridStepValue;	// grid line Y step value
			float value;
			float yPosOffset;
			float sc 			= orgScale;
			int	i;

			switch(desc.flagGridLines)
			{
/*				case MoGraph::DEFAULT_GRIDS:
					{
						value 		= 0.0f;
						yPosOffset 	= desc.gridStepYLines * 0.5f;
						i 			= 0;
					}
					break;
*/				case MoGraph::MIRRORED_GRIDS:
					{
						lprintfln("TextMgr::init: MIRRORED_GRIDS");
						value 		= -step * desc.gridYLines + step;
						yPosOffset 	= -desc.gridStepYLines * desc.gridYLines + desc.gridStepYLines;//*0.75f;
						i			= -desc.gridYLines+1;
					}
					break;
				case MoGraph::OFFSET_GRIDS:
					{
						lprintfln("TextMgr::init: OFFSET_GRIDS");
						value		= desc.gridOffsetStartValue;
						yPosOffset	= desc.gridOffsetStartLine /*+ desc.gridStepYLines * 0.75f*/;
						i			= 0;	//desc.gridOffsetStartLine + 1;
					}
					break;
				default:
					{
						lprintfln("TextMgr::init: DEFAULT_GRIDS");

						value 		= 0.0f;
						yPosOffset 	= 0.0f; //desc.gridStepYLines;// * 0.5f;
						i 			= 0;
					}
					break;
			}

			glm::vec3 pos(dcenterX * 1.01f, yPosOffset, dcenterZ * 1.01f);
			t.mPos 		= pos;
			t.mScale 	= glm::vec2(sc,sc);

			for(; i<desc.gridYLines; i++)
			{
				// let user provide formatting string.
				sprintf(buf,formatDecimals,value);

				t.mText 	= buf;
//				lprintfln("Bar Value %f=>%d and %s => %s" ,value,(int)value,formatDecimals,buf);   			// error not declared.

				t.mPos.y 	+= desc.gridStepYLines;
				t.mTextFlagX = Text::CENTER_RIGHT;
				mTextArray.push_back(t);
				value 		+= step;
			}
		}


		if (gridZ > 1)		// Check if there is a z-Axis at all
		{
			// set up text for Z-Axis
			t.mScale	= scaleXZ;
			t.mTextFlagX = Text::CENTER_LEFT;
			t.mText 	= "Z-axis";
			t.mPos 		= glm::vec3(-dcenterZ, 0.0f,-dcenterX);
			t.mRotate	= glm::vec3(0.0f,-90.0f,0.0f);
			mTextArray.push_back(t);
		}

	}

	/**
	 * TextMgr Constructor
	 * @param scene adding the scene class for back ref this obj.
	 */
	TextMgr::TextMgr(Scene *scene) : Render(scene)
	{
	}

}
