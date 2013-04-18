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

#include <yajl/YAJLDom.h>
#include <mautil/connection.h>

#include <mavsprintf.h>
#include <MAUtil/GLMoblet.h>
#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <stdlib.h>
#include "MAHeaders.h"
#include <MoGraph.h>

using namespace MAUtil;
#define CONNECTION_BUFFER_SIZE 1024	//1024

class MyGLMoblet: public GLMoblet , MAUtil::HttpConnectionListener
{
private:

	// ================== Members ==================

	int 			mWidth;
	int				mHeight;			// Screen resolution in ABS form e.g. 640,480
	MoGraph::IGraph	*mGraph;			// interface/Base class to MoGraph
	IFont			*mFont;				// interface/Base class to Font
	glm::vec4 		*mColors;			// array pointing towards a color table
	float 			*mTables;			// array pointing towards a float table
	std::vector<std::string> 	mNameTable;		// array pointing towards name of bar.
	DTime 			mDTime;				// delta time between each tick! (update) used for fps timer
	Time  			mTime;				// elapsed time since creation of this class,
	RenderText 		mText;				// generic render text class. (openGL)
	float			mOmega;				// used for sin and cos
	float			mScaleBarHeight;	// scale height of the input data to be more normalized.
	bool			mTextUpdated;		// state flag to check if text is updated
	std::string		mDateTime;			// Text date & time of retrieving the stocks.

	MAUtil::HttpConnection mHttp;		// using the HttpConnection class
	char 			*mBuffer;			// buffer to receive data to
	char 			*mSharesData;		// accumulated output data
	int 			mTotalSize;			// total size of the output data
	bool			mConnected;

public:

	// ================== Constructor ==================

	/**
	 * Constructor for the main app class
	 */
	MyGLMoblet() :
			GLMoblet(GLMoblet::GL2)
			, mGraph(0)
			, mFont(0)
			, mColors(0)
			, mTables(0)
			, mNameTable(0)
			, mOmega(0.0f)
			, mScaleBarHeight(1.0f)
			, mTextUpdated(false)
			, mHttp(this)
	{
	}

	// ================== Destructor ==================

	/**
	 * Destructor
	 */
	virtual ~MyGLMoblet()
	{
		mNameTable.clear();				// Clear the name table of shares.
		delete [] mTables;				// Delete Graph Height table
		delete [] mColors;				// Delete Graph Color table
		delete mGraph;					// Delete Graph
		delete mFont;					// Delete Font
		delete [] mBuffer;				// Delete buffer
		free(mSharesData);				// free total buffer (using realloc)  hence the free
	}

	// ================== Callback ==================

	// 1. call order after a http create... http finnished
	void httpFinished(HttpConnection *conn, int result)
	{
		if( result < 0)
		{
			lprintfln("@@@ 3DGRAPH: Cannot connect");
		}
		else
		{
			lprintfln("@@@ 3DGRAPH: HTTP %i\n", result);
			MAUtil::String contentLengthStr;
			int responseBytes = mHttp.getResponseHeader("content-length", &contentLengthStr);
			int contentLength = 0;

			if(responseBytes == CONNERR_NOHEADER)
			{
				lprintfln("@@@ 3DGRAPH: no content-length response header\n");
			}
			else
			{
				lprintfln("@@@ 3DGRAPH: content-length: %s\n", contentLengthStr.c_str());
				contentLength = atoi(contentLengthStr.c_str());
			}

//			contentLength = 2887+3;

			if(contentLength >= CONNECTION_BUFFER_SIZE || contentLength == 0)
			{
				lprintfln("@@@ 3DGRAPH: Receive in chunks..\n");
				mBuffer = new char[CONNECTION_BUFFER_SIZE];
				mHttp.recv(mBuffer, CONNECTION_BUFFER_SIZE);	// connRecvFinished is callback from this call
			}
			else
			{	// Not in use really
				mBuffer = new char[contentLength+1];
				mBuffer[contentLength] 	= 0;
				mSharesData = (char*)malloc((contentLength)*sizeof(char));	// skip the double slashes in from the buffer 4 bytes. we need an extra byte for null term though hence the -4+1 = -3
				mBuffer[contentLength] 	= 0;
				mTotalSize = contentLength;

				mHttp.read(mBuffer, contentLength);				//connReadFinnished is callback from this call
			}
		}
	}


	void connRecvFinished(Connection *conn, int result)
	{
		if(result >= 0)
		{
			lprintfln("@@@ 3DGRAPH: connRecvFinished %i\n", result);
			if(mSharesData == NULL)		// create shared data buffer (first time)
			{
				mSharesData = (char*)malloc((result-3)*sizeof(char));	// skip the double slashes in from the buffer 4 bytes. we need an extra byte for null term though hence the -4+1 = -3
				lprintfln("@@@@@@@@@@@@@@@ size: %d", (int)strlen(mSharesData));
				mTotalSize = result-4;

				for(int i = 4; i < result; i++)
					mSharesData[i-4] = mBuffer[i];

				mSharesData[mTotalSize] = '\0';
				lprintfln("@@@@@@@@@@@@@@@ FIRST:%sLAST size: %d", mSharesData, (int)strlen(mSharesData));

			}
			else	// continous buffer read
			{
				mBuffer[result] = '\0';
				lprintfln("@@@@@@@@@@@@@@@ FIRST:%sLAST size: %d", mBuffer, (int)strlen(mBuffer));

				// notice using realloc here to save code space hence the usage of malloc and free for mSharedData
				mSharesData = (char *)realloc(mSharesData ,(mTotalSize+result)*sizeof(char));
				mTotalSize += result;
				strcat(mSharesData, mBuffer);
				parseJSON();
			}

			delete [] mBuffer;
			mBuffer = new char[CONNECTION_BUFFER_SIZE];
			mHttp.recv(mBuffer, CONNECTION_BUFFER_SIZE);
			return;
		}
		else if(result == CONNERR_CLOSED)
		{
			lprintfln("@@@ 3DGRAPH: Receive finished! totalBytes=%d\n",mTotalSize);
			updateTextColumns(mNameTable,true);					// this will only update once!
		}
		else
		{
			lprintfln("@@@ 3DGRAPH: connection error %i\n", result);
			updateTextColumns(mNameTable,false);					// this will only update once!
		}


		mHttp.close();
	}

	void parseJSON()
	{
		YAJLDom::Value *jsonRoot = YAJLDom::parse((const unsigned char*)mSharesData, strlen(mSharesData));

		if(jsonRoot->getType() == YAJLDom::Value::ARRAY)
		{
			lprintfln("SUCCESS IN PARSING JSON DATA");
			//float l,l_cur,el,el_cur;
			MAUtil::String tmp,t;
			float scale = mScaleBarHeight;
			const int gridX = mGraph->getScene().getGridX();
			const int gridZ	= mGraph->getScene().getGridZ();
			bool bGotDate = false;
			for(int i=0; i<gridX; i++)
			{
				YAJLDom::Value *row = jsonRoot->getValueByIndex(i);

				int j = 0;
				t = row->getValueForKey("t")->toString();
				mNameTable[i + gridX*j] = t.c_str();

				tmp = row->getValueForKey("l")->toString();
				mTables[i + gridX*j] = scale*(atof(tmp.c_str()));
				lprintfln("@@@r l=%s t=%s", tmp.c_str(),t.c_str());
				j++;

				t = row->getValueForKey("t")->toString();
				mNameTable[i + gridX*j] = t.c_str();

				tmp = row->getValueForKey("l_cur")->toString();
				mTables[i + gridX*j] = scale*(atof(tmp.c_str()));
				lprintfln("@@@ l_cur=%s t=%s", tmp.c_str(),t.c_str());
				j++;

				t = row->getValueForKey("t")->toString();
				mNameTable[i + gridX*j] = t.c_str();

				tmp = row->getValueForKey("c")->toString();
				mTables[i + gridX*j] = scale*(atof(tmp.c_str()));
				lprintfln("@@@ c=%s t=%s", tmp.c_str(),t.c_str());
				j++;

				t = row->getValueForKey("t")->toString();
				mNameTable[i + gridX*j] = t.c_str();

				tmp = row->getValueForKey("cp")->toString();
				mTables[i + gridX*j] = scale*(atof(tmp.c_str()));
				lprintfln("@@@ cp=%s t=%s", tmp.c_str(),t.c_str());

				if (bGotDate == false)
				{
					tmp = row->getValueForKey("lt")->toString();
					mDateTime = tmp.c_str();
					bGotDate = true;
				}
			}

			glm::vec4 colScheme[gridZ];
			glm::vec4 redScheme[gridZ];

			colScheme[0] = glm::vec4(0.0f, 236.0f/255.0f,	255.0f/255.0f,	1.0f);	// L blue
			colScheme[1] = glm::vec4(0.0f, 177.0f/255.0f,	191.0f/255.0f,	1.0f);	// D blue
			colScheme[2] = glm::vec4(181.0f/255.0f, 255.0f/255.0f,	0.0f,	1.0f);	// L Green
			colScheme[3] = glm::vec4(124.0f/255.0f, 175.0f/255.0f,	0.0f,	1.0f);	// D Green

			redScheme[0] = glm::vec4(255.0f/255.0f, 0.0f, 236.0f/255.0f,	1.0f);	// L blue
			redScheme[1] = glm::vec4(177.0f/255.0f, 0.0f, 191.0f/255.0f,	1.0f);	// D blue
			redScheme[2] = glm::vec4(255.0f/255.0f, 90.0f/255.0f, 0.0f,		1.0f);	// L Red
			redScheme[3] = glm::vec4(175.0f/255.0f, 64.0f/255.0f, 0.0f,		1.0f);	// D Red

			for(int j=0; j<gridZ; j++)
			{
				for(int i=0; i<gridX; i++)
				{
					const int id 	= i+gridX*j;
					mColors[id] 	= mTables[id]>=0.0f? colScheme[j]: redScheme[j];
				}
			}
		}
	}

	// 2. call order from mHttp.read
	void connReadFinished(Connection *conn, int result)
	{
		if(result >= 0)
		{
			for(int i = 4; i < mTotalSize; i++)
				mSharesData[i-4] = mBuffer[i];

			parseJSON();
			lprintfln("@@@ 3DGRAPH: Read finished!\n");
			updateTextColumns(mNameTable,true);					// this will only update once!
		}
		else
		{
			lprintfln("@@@ 3DGRAPH: connection error %i\n", result);
			updateTextColumns(mNameTable,false);					// this will only update once!
		}
		mHttp.close();
	}


	/**
	 * This method is called when a key is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		switch (keyCode)
		{
			// Close the application if the back key, key 0,
			// or the left "softkey" is pressed (the left
			// softkey is usually located at the top left of
			// the keypad of phones that have a keypad, and
			// is often used as a back navigation key).
			case MAK_BACK:
			case MAK_0:
			case MAK_SOFTLEFT:
				// Exit the application.
				maExit(0);
				break;
		}
	}

	/**
	*  This function is called with a coordinate when a pointer is pressed.
	*/
	void multitouchPressEvent(MAPoint2d p, int touchId)
	{
		mGraph->getTouch().multitouchPressEvent(p,touchId);	// just forward function to the Graphs own touch handler
	}

	/**
	*  This function is called with a coordinate when a pointer is moved.
	*/
	void multitouchMoveEvent(MAPoint2d p, int touchId)
	{
		mGraph->getTouch().multitouchMoveEvent(p,touchId); // just forward function to the Graphs own touch handler
	}

	/**
	*  This function is called with a coordinate when a pointer is released.
	*/
	void multitouchReleaseEvent(MAPoint2d p, int touchId)
	{
		mGraph->getTouch().multitouchReleaseEvent(p,touchId); // just forward function to the Graphs own touch handler
	}

	void updateTextColumns(std::vector<std::string> &tableName, bool bIsSuccess)
	{
		MoGraph::Text text;
		MoGraph::Scene &scene 	= mGraph->getScene();	// get scene information
		std::vector<MoGraph::Text> &textArray = scene.getTextMgr().getTextArray();


		if (bIsSuccess == true)
		{
			textArray[0].mColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			textArray[0].mText = "NASDAQ:" + mDateTime;
			if (mTextUpdated == false)
			{
				const float scale 		= 0.7f*scene.getScaleFactor();
				const float cx 			= scene.getCx();		// need to be able to read the grid size
				const float cz 			= scene.getCz();
				const int	gridX		= scene.getGridX();

				for(int i = 0; i < gridX; i++)
				{
					text.mColor 	= glm::vec4(1.0f,1.0f,1.0f,1.0f);
					text.mPos 		= glm::vec3(cx+i*1.0f,0.0f,cz);
					text.mText 		= tableName[i];
					text.mRotate 	= glm::vec3(0.0f,0.0f,0.0f);
					text.mScale 	= glm::vec2(scale,scale);
					text.mTextFlagX = MoGraph::Text::CENTER_LEFT;
					text.mTextFlagY = MoGraph::Text::CENTER_TOP;

					textArray.push_back(text);
				}
				mTextUpdated = true;
			}
		}
		else
		{
			textArray[0].mColor = glm::vec4(175.0f/255.0f, 64.0f/255.0f, 0.0f, 1.0f);
			textArray[0].mText = "Connection Failed!";
		}
	}


	/**
	 * init call backed from GLMoblet
	 */
	void init()
	{
		//mHttp.create("http://www.example.com", HTTP_GET);

		/*
		 * Graph object needs to be allocated and then initiated,
		 * Font is a separate system but is required in the graph for rendering text in 3D
		 * RenterText is an independent text renderer flat on screen. but very same class is being used in Graph as well
		 * can handle both orthogonal projection see drawText and drawText3D
		 */

		int gridX	= 10;
		int gridY	= 4;

		mWidth 		= (int)(EXTENT_X(maGetScrSize()));
		mHeight 	= (int)(EXTENT_Y(maGetScrSize()));
		mGraph 		= new MoGraph::Graph();			// Create MoGraph::Graph class
		mFont 		= new BMFont();					// Create Font class
		mNameTable.resize(gridX * gridY);
		// Initiate the RenderText system that will be used in Graph
		std::vector<MAHandle> fontTexArray;
		fontTexArray.push_back(R_BOX_TEXTURE /*R_MOSYNC_LOGO*/);

		mFont->Init(R_BOX_FNT, fontTexArray);		// Initiate font where to get its resources (.fnt) file generated from BMFont and the bitmap texture that contains the aphabet
		mText.init(mWidth,mHeight,mFont);			// initiate the text system by setting a Font & Screen dimensions

		mDTime.setDesiredFps(50.0f);				// set up the DTime used for calculating FPS
		setPreferredFramesPerSecond(50);			// set preferred fps for the Moblet

		// initiate Graph by setting up a grid sz in X & Z ,
		// also grid in Y with grid step,
		// additional info like fit to screen, screen resolutions.
		mScaleBarHeight = 0.25f;

		MoGraph::GraphDesc desc;
		desc.scrWidth 		= mWidth;
		desc.scrHeight 		= mHeight;
		desc.gridX 			= gridX;
		desc.gridZ 			= gridY;
		desc.gridYLines 	= 16;
		desc.gridStepYLines = 0.5f;
		desc.gridStepValue 	= 0.5f/mScaleBarHeight;
		desc.gridDecimals 	= 1;
		desc.gridOffsetStartLine = -1;			// requires OFFSET_GRIDS to be set.
		desc.gridOffsetStartValue = -2.0f;
		desc.bFitScreen 	= true;
		desc.flagGridLines 	= MoGraph::DEFAULT_GRIDS; // /* MoGraph::OFFSET_GRIDS; //*/ MoGraph::MIRRORED_GRIDS;
		desc.bUseGridValue 	= true;
		desc.font 			= mFont;

		if (!mGraph->init(&desc))
			maPanic(1,"Failed to initiate Graph");

		glm::vec4 bkcolor(0.2f, 0.2f, 0.2f, 1.0f);
		mGraph->setBGColor(bkcolor);				// additional set background color

		// TEXT MANIPULATION IN GRAPH
		// Text strings in the graph has a default setup.
		// everything is built upon the Text structs that are pushed to an
		// std::vector<Text>   Text array
		// We can change the existing setup by changing the strings...
		std::vector<MoGraph::Text> &textArray = mGraph->getScene().getTextMgr().getTextArray();
	//	std::vector<MoGraph::Line> &lineArray = mGraph->getScene().getAxisMgr().getLineArray();
		// and we can also push new ones to the table for more text in the graph...
		// you can add as many as you please... fps issue in the end.
		const float scale = mGraph->getScene().getScaleFactor();
		const float ss = 0.75f;
		glm::vec2 scaleAxisText = glm::vec2(scale*ss,scale*ss);

		textArray[0].mColor		= glm::vec4(124.0f/255.0f, 175.0f/255.0f,	0.0f,	1.0f);
		textArray[0].mPos.y 	-= 0.3f;
		textArray[0].mText 		= "Connecting...";		// Graph title text.
		textArray[0].mTextFlagX = MoGraph::Text::CENTER_LEFT;
		textArray[0].mTextFlagY = MoGraph::Text::CENTER_TOP;
		textArray[0].mScale		= glm::vec2(scale*0.8,scale*0.8);

		textArray[1].mScale 	= scaleAxisText;	// Y-AXIS
		textArray[1].mTextFlagX = MoGraph::Text::CENTER_X;
		textArray[1].mTextFlagY = MoGraph::Text::CENTER_TOP;

		textArray[2].mPos.x		+= 0.1f;
		textArray[2].mText		= "Shares";
		textArray[2].mScale 	= scaleAxisText;	// X-AXIS
		textArray[2].mTextFlagX = MoGraph::Text::CENTER_LEFT;
		textArray[2].mTextFlagY = MoGraph::Text::CENTER_TOP;

		textArray[textArray.size()-1].mPos.x		+= 0.1f;
		textArray[textArray.size()-1].mScale 		= scaleAxisText; // Z-AXIS (last entry before user storage! due to z being optional)
		textArray[textArray.size()-1].mTextFlagX 	= MoGraph::Text::CENTER_RIGHT;
		textArray[textArray.size()-1].mTextFlagY 	= MoGraph::Text::CENTER_TOP;

		// create additional example text

		MoGraph::Text text;
		text.mColor 	= glm::vec4(1.0f,1.0f,1.0f,1.0f);
		text.mPos 		= glm::vec3(0.0f,10.0f,0.0f);
		text.mRotate 	= glm::vec3(0.0f,0.0f,0.0f);		// Rotation in degrees
		text.mScale 	= glm::vec2(scale,scale);
		text.mTextFlagX = MoGraph::Text::CENTER_X;
		text.mText 		= "MoGraph";
		textArray.push_back(text);

//		mLogo.init(R_MOSYNC_LOGO);
//		mLogoH.init(R_MOSYNC_HEIGHT);

		// Prepare the colors for the Graph.
		// colors are static so we only need to build them once.
		const int iGridZ 		= desc.gridZ;		// need to be able to read the grid size
		const int iGridX 		= desc.gridX;
		const int sz			= iGridX * iGridZ;

		if (mTables == 0)					// check if array already is allocated
			mTables = new float[sz];		// allocate an array for set data to the Bars.
		if (mColors == 0)					// check if array already is allocated
			mColors = new glm::vec4[sz];	// allocate an array for color table

		for(int j=0; j<iGridZ; j++)			// Build the data arrays for colors and for tables.
		{
			for(int i=0; i<iGridX; i++)
			{
				const int id 	= j*iGridX+i;
				mColors[id]		= glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);			// set color gradients
				mTables[id]		= 1.0f;
			}
		}

		// Start the HTTP request.
		mSharesData = NULL;
		lprintfln("@@@ MOSYNC BEFORE REQUEST");
		MAUtil::String url = "http://finance.google.com/finance/info?client=ig&q=NASDAQ:MSFT,NASDAQ:FB,NASDAQ:ERIC,NASDAQ:BBRY,NYSE:NOK,NASDAQ:YHOO,NASDAQ:INTC,NASDAQ:CSCO,NASDAQ:ORCL,NASDAQ:NVDA";

		int res = mHttp.create(url.c_str(),HTTP_GET);
		if(res < 0)
		{
			lprintfln("@@@@@@@@@@@@@@@@ unable to connect - %i\n", res);
		}
		else
		{
			mHttp.finish();
		}

	}

	/**
	 *  \brief Draw, Call back from the GLMoblet.
	 */
	void draw()
	{
		mDTime.tick();									// update delta time ticker. our fps timer (resets for every tick call)
		MoGraph::Scene &scene 	= mGraph->getScene();	// get scene information
		const int iGridZ 		= scene.getGridZ();		// need to be able to read the grid size
		const int iGridX 		= scene.getGridX();
		const int sz			= iGridX * iGridZ;

		mGraph->setValues(mTables,sz);	// set the value array to the Graph to read from
		mGraph->setColors(mColors,sz);	// set the color array to the Graph to read from

		mGraph->draw();					// Draw the whole graph system


		// DRAW ADDITIONAL TEXT ON SCREEN (Orthogonal projections)
		//---------------------------------------------------------------------
		glm::vec4 col(1.0f,1.0f,1.0f,1.0f);	// White color
		glm::vec3 pos(0.0f,0.0f,10.0f);		// set screen position upper left corner 0,0.. note need z depth value for order.
		float sy = (0.6f*(float)scene.getWidth())/320.0f; // scale size regardless to resolution our reference resolution is 320..
		mText.setScale(sy,sy);

		char buf[64];						// create text string
		sprintf ( buf, "FPS=%.2f ms=%d",mDTime.currentFps(),mDTime.getElapsed());
		mText.drawText(buf,pos,col);		// call drawText
	}
};

// ================== Start up ==================

/**
 * Entry point of the program. The MAMain function
 * needs to be declared as extern "C".
 */
extern "C" int MAMain()
{
	Moblet::run(new MyGLMoblet());
	return 0;
}
