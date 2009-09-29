/*
* ============================================================================
*  Name     : CDirScrAccEng from DirScrAccEng.h
*  Part of  : DirScrAcc
*  Created  : 06/11/2003 by Forum Nokia
*  Description:
*     Engine for drawing using Direct Screen Access.
*  Version  :
*  Copyright: Nokia Corporation
* ============================================================================
*/

#ifndef DIRSCRACCENG_H
#define DIRSCRACCENG_H

#include <w32std.h>

#include <helpers/types.h>
#include <helpers/cpp_defs.h>

#include "symbian_helpers.h"

#ifdef __WINS__
#undef MA_PROF_SUPPORT_FRAMEBUFFER_32BIT
#endif

#if defined(__SERIES60_3X__) || defined(MA_PROF_SUPPORT_FRAMEBUFFER_32BIT)
typedef TUint32 Pixel;
#else  //Series 60, 2nd Ed, except FP3
typedef TUint16 Pixel;
#endif	//__SERIES60_3X__

enum DirScrAccEngPanics {
	DirScrAccEngAlreadyStarted = 1,
	DirScrAccEngAlreadyStopped
};

class TMyDrawBuf;

class CDirScrAccEng : public MDirectScreenAccess {
public:
	static CDirScrAccEng* NewL(RWsSession& aClient, CWsScreenDevice& aScreenDevice,
		RWindow& aWindow);
	virtual ~CDirScrAccEng();	
public:
	void StartDrawingL();
	void StopDrawing();
	inline TBool IsDrawing() { return iDrawing; }
	void UpdateScreen();
	uint Width() { return iWindow.Size().iWidth; }
	uint Height() { return iWindow.Size().iHeight; }
	void SetDrawSurfaceL(TAlphaBitmap* img);
	void ClearScreen();
	
	void Reset();

	void SetColor(int argb);
	void SetClip(const TRect& aRect);
	const TRect& GetScreenClip() const;

	void PlotClip(int posX, int posY);
	void LineDrawClip(long x,long y,long dx,long dy);
	void FillRectClip(int left, int top, int width, int height);
	void FillTriangleStripClip(const MAPoint2d* points, int count);

	void DrawTextL(const TDesC& aText, TPoint aPoint);
	MAExtent GetTextSizeL(const TDesC& aText);
	void DrawImage(TAlphaBitmap* img, int left, int top);
	void DrawImage(TAlphaBitmap* img, const TRect& srcRect, const TPoint& dstPoint,
		int transform);

	int FrameBufferGetInfo(MAFrameBufferInfo* info);
	int FrameBufferInit(const void* data);
	int FrameBufferClose();
	int GetFrameBufferLen() { return iFrameBufLen; }

private:
	//MDirectScreenAccess 
	void Restart(RDirectScreenAccess::TTerminationReasons aReason);
	void AbortNow(RDirectScreenAccess::TTerminationReasons aReason);
private:
	CDirScrAccEng(RWsSession& aClient, CWsScreenDevice& aScreenDevice,
		RWindow& aWindow);
	void ConstructL();	

	void FillDirect(Pixel* drawBuffer, long xy,long d);
	void HorizontalLineDrawClip(TMyDrawBuf& drawBuffer, long x,long y,long dx);
	void DrawSpan(long x,long y,long dx, TMyDrawBuf& drawBuffer);
	void DrawTriangle(TMyDrawBuf& drawBuffer, int x1,int y1,int x2,int y2,int x3,int y3);
	
	void UpdateClip();

public:	//hack for VideoPlayer
	// Window server handling
	RWsSession& iClient;
	CWsScreenDevice& iScreenDevice;
	RWindow& iWindow;

private:
	// Direct Screen Access
	CDirectScreenAccess* iDirectScreenAccess;
	CFbsBitGc* iScreenGc;
	RRegion* iRegion;

	//just caching
	const CFont* gFont; 
	TInt gFontBaseline;
	//Pixel* gDrawBuffer;
	//int gDrawWidth, gDrawHeight;
	Pixel gCurrentConvertedColor;
	TRgb gCurrentUnconvertedColor;

	//For draw2image
	CFbsBitmap* iDrawSurface;

	// Are we drawing
	TBool iDrawing;

	TInt iFrameBufLen;         // length of the frame buffer in bytes
	CFbsBitmap* iOffScreenBmp;  // offscreen bitmap for double-buffered drawing
	CFbsBitmapDevice* iOffScreenDevice;
	TRect iOffScreenClipRect;
	TRect iCurrentClipRect;
	int iDrawWidth;
	CFbsBitGc* iFBGc;

	const void* iRawFrameBuf;

#ifndef __WINS__
	TUint8* iScreenAddr;        // frame buffer address
	TRawEvent iRedraw;	        // raw event for forcing the update of screen 
#endif	//__WINS__
};

#endif  //DIRSCRACCENG_H
