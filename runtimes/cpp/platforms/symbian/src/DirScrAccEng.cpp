/*
* ============================================================================
*  Name     : CDirScrAccEng from DirScrAccEng.cpp
*  Part of  : DirScrAcc
*  Created  : 06/11/2003 by Forum Nokia
*  Description:
*     Engine for drawing using Direct Screen Access.
*  Version  :
*  Copyright: Nokia Corporation
* ============================================================================
*/

#include <eikenv.h>
#include <aknutils.h>

#include "config_platform.h"

#include "DirScrAccEng.h"
#include "base_errors.h"

using namespace MoSyncError;

_LIT(KDirScrAccTxt, "DirectScreenAccess");

//******************************************************************************
//Helper classes
//******************************************************************************

class TMyDrawBuf {
public:
	TMyDrawBuf(CFbsBitmap& aDrawSurface) : mDS(aDrawSurface) {
#ifdef __S60_50__	//5th edition
		mDS.BeginDataAccess();
#else	//2nd or 3rd edition
		mDS.LockHeap();
#endif
		mPtr = (Pixel*)mDS.DataAddress();
	}
	operator Pixel*() { return mPtr; }
	~TMyDrawBuf() {
#ifdef __S60_50__	//5th edition
		mDS.EndDataAccess();
#else	//2nd or 3rd edition
		mDS.UnlockHeap();
#endif
	}
private:
	CFbsBitmap& mDS;
	Pixel* mPtr;
};

//******************************************************************************
//Drawing functions
//******************************************************************************

#define Plot(drawBuffer, x, y)\
	(drawBuffer[(x) + (y) * iDrawWidth] = gCurrentConvertedColor)
#define HorizontalLineDraw(drawBuffer, x, y, dx)\
	FillDirect(drawBuffer, (x) + (y) * iDrawWidth, dx)
#define FillLines(drawBuffer, y, dy)\
	FillDirect(drawBuffer, (y)*iDrawWidth, (dy)*iDrawWidth)

void CDirScrAccEng::FillDirect(Pixel* drawBuffer, long xy,long d) {
	Pixel* end = (drawBuffer + xy + d);
	for(Pixel* ptr = (drawBuffer + xy); ptr < end; ptr++) {
		*ptr = gCurrentConvertedColor;
	}
}

void CDirScrAccEng::HorizontalLineDrawClip(TMyDrawBuf& drawBuffer, long x,long y,long dx) {
	//LOGD("HorizontalLineDrawClip %i %i d%i\n", x, y, dx);
	TRect rect(TPoint(x, y), TSize(dx, 1));
	rect.Intersection(iCurrentClipRect);
	if(!rect.IsEmpty()) {
		HorizontalLineDraw(drawBuffer, rect.iTl.iX, rect.iTl.iY, rect.Width());
	}
}

void CDirScrAccEng::LineDrawClip(long x,long y,long dx,long dy)
{
	//LOGD("LineDrawClip %i %i d%i d%i\n", x, y, dx, dy);
	//line vars
	register long sdx,sdy,dxabs,dyabs,n,ac,linelen;
	TMyDrawBuf drawBuffer(*iDrawSurface);
	if(dy == 0) {
		DrawSpan(x, y, dx, drawBuffer);
		return;
	}
	// Get sign of direction
	sdx = sgn(dx);
	sdy = sgn(dy);
	// Get positive slope values absolute magentude
	dxabs = Abs(dx) + 1;
	dyabs = Abs(dy) + 1;
	//main loop stuff
	if (dxabs >= dyabs)
	{
		ac = dxabs/2;
		linelen = dxabs;  // x*x + y*y
		//LOGD("X mode. ac %i  len %i\n", ac, linelen);
		for (n=0;n<dxabs;n++)
		{
			if(x >= iCurrentClipRect.iTl.iX && x < iCurrentClipRect.iBr.iX &&
				y >= iCurrentClipRect.iTl.iY && y < iCurrentClipRect.iBr.iY) {
				Plot(drawBuffer, x, y);
			}
			// line math
			ac += dyabs;
			if (ac > dxabs)
			{
				ac -= dxabs;
				y  += sdy;
			}
			x += sdx;
		}
	}
	else
	{
		ac = dyabs/2;
		linelen = dyabs;
		//LOGD("Y mode. ac %i  len %i\n", ac, linelen);
		for (n=0;n<dyabs;n++)
		{      
			if(x >= iCurrentClipRect.iTl.iX && x < iCurrentClipRect.iBr.iX &&
				y >= iCurrentClipRect.iTl.iY && y < iCurrentClipRect.iBr.iY) {
				Plot(drawBuffer, x, y);
			}
			// line math
			ac += dxabs;
			if (ac > dyabs)
			{
				ac -= dyabs;
				x  += sdx;
			}
			y += sdy;
		}
	}	
}

void CDirScrAccEng::SetColor(int argb) {
#ifdef __SERIES60_3X__
	gCurrentUnconvertedColor = argb & 0xFFFFFF;	//set alpha to 0 (opaque).
#else	//2nd ed.
	gCurrentUnconvertedColor = ((argb & 0xff0000) >> 16) |
		(argb & 0x00ff00) |
		((argb & 0x0000ff) << 16);
#endif

#ifdef __S60_50__
	gCurrentConvertedColor = argb | 0xFF000000;	//test
#elif defined(__SERIES60_3X__)
	gCurrentConvertedColor = argb & 0xFFFFFF;
#elif defined(MA_PROF_SUPPORT_FRAMEBUFFER_32BIT)
	gCurrentConvertedColor = argb;
		//((argb & 0xFF0000) >> 8) | ((argb & 0xFF00) >> 8) | ((argb & 0xFF) << 24);
#else  //Series 60, 2nd Ed.
	gCurrentConvertedColor =  //565
		((argb & 0xF80000) >> 8) | ((argb & 0xFC00) >> 5) | ((argb & 0xF8) >> 3);
#endif	//__SERIES60_3X__
}

void CDirScrAccEng::PlotClip(int x, int y) {
	if(x >= iCurrentClipRect.iTl.iX && x < iCurrentClipRect.iBr.iX &&
		y >= iCurrentClipRect.iTl.iY && y < iCurrentClipRect.iBr.iY)
	{
		TMyDrawBuf drawBuffer(*iDrawSurface);
		Plot(drawBuffer, x, y);
	}
}
void CDirScrAccEng::FillRectClip(int left, int top, int width, int height) {
	if(width <= 0 || height <= 0)
		return;
	//Clip
	/*if(left >= gDrawWidth) return;
	if(top >= gDrawHeight) return;
	if(left < 0) {
		width += left;
		left = 0;
	}
	if(left+width > gDrawWidth) {
		width = gDrawWidth - left;
	}
	if(height < 0) {
		height += top;
		top = 0;
	}
	if(top+height > gDrawHeight) {
		height = gDrawHeight - top;
	}*/
	TRect rect(TPoint(left, top), TSize(width, height));
	rect.Intersection(iCurrentClipRect);
	//Draw
	TMyDrawBuf drawBuffer(*iDrawSurface);
	if(rect.Width() == iDrawWidth) {
		FillLines(drawBuffer, rect.iTl.iY, rect.Height());
	} else {
		for(int i=0; i<rect.Height(); i++) {
			HorizontalLineDraw(drawBuffer, rect.iTl.iX, rect.iTl.iY + i, rect.Width());
		}
	}
}
void CDirScrAccEng::FillTriangleStripClip(const MAPoint2d* points, int count) {
	LOGG("fp color 0x%08x %i:", gCurrentConvertedColor, count);
	for(int i=0; i<count; i++) {
		LOGG(" %ix%i", points[i].x, points[i].y);
	}
	LOGG("\n");
	MYASSERT(count >= 3, ERR_POLYGON_TOO_FEW_POINTS);
	TMyDrawBuf drawBuffer(*iDrawSurface);
	for(int i = 2; i < count; i++) {
		DrawTriangle(drawBuffer,
			points[i-2].x, 
			points[i-2].y,
			points[i-1].x, 
			points[i-1].y,
			points[i].x, 
			points[i].y);
	}
}

MAExtent CDirScrAccEng::GetTextSizeL(const TDesC& aText) {
	return EXTENT(gFont->TextWidthInPixels(aText), gFont->HeightInPixels());
}
void CDirScrAccEng::DrawTextL(const TDesC& aText, TPoint aPoint) {
	aPoint.iY += gFontBaseline;
	LOGG("DrawTextL(0x%06x)\n", gCurrentUnconvertedColor.Internal());
	iFBGc->SetPenColor(gCurrentUnconvertedColor | 0xff000000);
	iFBGc->DrawText(aText, aPoint);
}

void CDirScrAccEng::DrawImage(TAlphaBitmap* img, int left, int top) {
	if(img->Alpha()) {
		iFBGc->BitBltMasked(TPoint(left, top), img->Color(), TRect(TPoint(0,0),
			img->Color()->SizeInPixels()), img->Alpha(), false);
	} else {
		iFBGc->BitBlt(TPoint(left, top), img->Color());
	}
}
void CDirScrAccEng::DrawImage(TAlphaBitmap* img, const TRect& srcRect,
	const TPoint& dstPoint, int /*transform*/)
{
	if(img->Alpha()) {
		iFBGc->BitBltMasked(dstPoint, img->Color(), srcRect, img->Alpha(), false);
	} else {
		iFBGc->BitBlt(dstPoint, img->Color(), srcRect);
	}
}

//******************************************************************************
//DrawTriangle
//******************************************************************************

//static void DrawSpan(int x1, int x2, int y)
void CDirScrAccEng::DrawSpan(long x,long y,long dx, TMyDrawBuf& drawBuffer)
{
	/*x = MAX(iCurrentClipRect.iTl.iX, x);
	x = MIN(x, iCurrentClipRect.iBr.iX - 1);
	y = MAX(iCurrentClipRect.iTl.iY, y);
	y = MIN(y, iCurrentClipRect.iBr.iY - 1);*/
	/*if(y < iCurrentClipRect.iTl.iY || y > iCurrentClipRect.iBr.iY ||
		x < iCurrentClipRect.iTl.iX || (x + dx) > iCurrentClipRect.iBr.iX)
		return;*/
	if(dx < 0) {
		x = x + dx;
		dx = -dx;
	}
	/*if(x+dx > gDrawWidth) {
		dx = gDrawWidth - x;
	}*/
	HorizontalLineDrawClip(drawBuffer, x, y, dx);
}


void CDirScrAccEng::DrawTriangle(TMyDrawBuf& drawBuffer,
	int x1,int y1,int x2,int y2,int x3,int y3)
{
	int step1,step2,xstart1,xstart2,ystart1,ystart2;
	int a, t;
	//LOGG("DrawTriangle %ix%i, %ix%i, %ix%i\n", x1, y1, x2, y3, x3, y3);

	ystart2=0;

	if (y2 < y1) 
	{
		t = y1;
		y1 = y2;
		y2 = t;

		t = x1;
		x1 = x2;
		x2 = t;
	}

	if (y3 < y2) 
	{
		t = y2;
		y2 = y3;
		y3 = t;

		t = x2;
		x2 = x3;
		x3 = t;

		if (y2 < y1) 
		{
			t = y1;
			y1 = y2;
			y2 = t;

			t = x1;
			x1 = x2;
			x2 = t;
		}
	}

	if (y1 == y3)
		return;

	step1 = (x3 - x1) * 65536 / (y3 - y1);
	xstart1 = (int) x1 << 16;

	if ((y1 != y2))
	{
		step2 = (x2 - x1) * 65536 / (y2 - y1);
		xstart2 = xstart1;
		ystart2 = y2 - y1;

		for (a=0; a<ystart2; a++) 
		{
			DrawSpan(xstart1 >> 16, a+y1, (xstart2 >> 16) - (xstart1 >> 16), drawBuffer);

			xstart1 += step1;
			xstart2 += step2;
		}
		ystart1 = ystart2;
	}

	if (y2 == y3)
		return;

	ystart2 = y3 - y2;

	step2 = (x3 - x2) * 65536 / (y3 - y2);
	xstart2 = (int) x2 << 16;

	ystart1 = y2 - y1;

	for (a=ystart1; a<ystart2+ystart1; a++) 
	{
		DrawSpan(xstart1 >> 16, a+y1, (xstart2 >> 16) - (xstart1 >> 16), drawBuffer);
		xstart1 += step1;
		xstart2 += step2;
	}
}

//******************************************************************************
//Main class
//******************************************************************************

CDirScrAccEng::CDirScrAccEng(RWsSession& aClient, CWsScreenDevice& aScreenDevice,
	RWindow& aWindow)
	: iClient(aClient),	iScreenDevice(aScreenDevice),
	iWindow(aWindow), iDirectScreenAccess(0), iScreenGc(0), gCurrentConvertedColor(0),
	iDrawing(EFalse), iOffScreenBmp(0), iOffScreenDevice(0), iFBGc(0),
	iRawFrameBuf(NULL)
{    
	LOGG("DSAE\n");
	TSize size = aWindow.Size();
	// calculate the (used) frame buffer size in bytes
	iFrameBufLen = size.iWidth * size.iHeight * sizeof(Pixel);
}

CDirScrAccEng* CDirScrAccEng::NewL(RWsSession& aClient, CWsScreenDevice& aScreenDevice,
	RWindow& aWindow)
{
	CDirScrAccEng* self = new (ELeave) CDirScrAccEng(aClient, aScreenDevice, aWindow);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
}

CDirScrAccEng::~CDirScrAccEng()	{
	if(iDirectScreenAccess) {
		if(iDirectScreenAccess->IsActive())
			iDirectScreenAccess->Cancel();
		delete iDirectScreenAccess;
	}
	if(iFBGc)
		delete iFBGc;
	if(iOffScreenDevice)
		delete iOffScreenDevice;
	if(iOffScreenBmp)
		delete iOffScreenBmp;
}

void CDirScrAccEng::ConstructL() {
	LOGG("DSAEC\n");

	// for emulator, always use offscreen bitmap
	// create the offscreen bitmap
	iOffScreenBmp = new (ELeave) CFbsBitmap;
	TSize offScreenSize = iScreenDevice.SizeInPixels();
	iOffScreenBmp->Create(offScreenSize, iScreenDevice.DisplayMode());
	iOffScreenClipRect = offScreenSize;
	iFBGc = CFbsBitGc::NewL();
	iOffScreenDevice = CFbsBitmapDevice::NewL(iOffScreenBmp);
	SetDrawSurfaceL(HANDLE_SCREEN);

	// Create the DSA object
	iDirectScreenAccess = CDirectScreenAccess::NewL(iClient, iScreenDevice, iWindow, *this);
	LOGG("DSAEC3\n");

#ifdef MOSYNC_COMMERCIAL
	//clear the screen    //defined in IDL
	ClearScreen();
#endif
}

void CDirScrAccEng::ClearScreen() {
	LOGG("ClearScreen\n");
	TMyDrawBuf drawBuffer(*iOffScreenBmp);
	FillLines(drawBuffer, 0, iOffScreenBmp->SizeInPixels().iHeight);
	iFBGc->CancelClippingRect();
}

void CDirScrAccEng::StartDrawingL() {
	LOGG("SD\n");
	DEBUG_ASSERT(!iDrawing);

	// Initialise DSA

	// Trap the call to CDirectScreenAccess->StartL() to suppress system
	// error notes (e.g. active screen saver may cause problems)
	TRAPD(dsaErr, iDirectScreenAccess->StartL());
	if(dsaErr == KErrNone) {
		LOGG("SD1\n");
		// Get graphics context for it
		iScreenGc = iDirectScreenAccess->Gc();

		// Get region that DSA can draw in
		iRegion = iDirectScreenAccess->DrawingRegion();
		
		LOGG("Region: %i sections.\n", iRegion->Count());
#ifdef GRAPHICS_DEBUGGING_MODE
		const TRect* rl = iRegion->RectangleList();
		for(int i=0; i<iRegion->Count(); i++) {
			const TRect& r(rl[i]);
			LOGG("%ix%i, %ix%i\n", r.iTl.iX, r.iTl.iY, r.iBr.iX, r.iBr.iY);
		}
#endif

		// Set the display to clip to this region
		iScreenGc->SetClippingRegion(iRegion);

		gFont = AknLayoutUtils::FontFromId(EAknLogicalFontPrimarySmallFont);
		iFBGc->UseFont(gFont);
		gFontBaseline = gFont->AscentInPixels();
		//gDrawBuffer = (TUint16*)iDrawSurface->DataAddress();

		iDrawing = ETrue;
		
#ifdef __S60_50__	//5th edition
		UpdateScreen();
#endif
	} else {
		LHEL(dsaErr);
	}
	LOGG("SD2\n");
}
void CDirScrAccEng::SetDrawSurfaceL(TAlphaBitmap* img) {
	iDrawSurface = img ? img->Color() : iOffScreenBmp;
	CFbsBitmapDevice* device = img ? img->Device() : iOffScreenDevice;
	iDrawWidth = iDrawSurface->SizeInPixels().iWidth;
	iCurrentClipRect = img ? img->ClipRect() : iOffScreenClipRect;
	iFBGc->Reset();

	/*LOGD("SetDrawSurfaceL\n");
	LogAvailableMemory();
	LOGD("new byte[1024]; allocates %i bytes\n", User::AllocLen(new (ELeave) byte[1024]));

	LogAvailableMemory();*/
	//SAFE_DELETE(iFBDevice);
	//LogAvailableMemory();
	//iFBDevice = CFbsBitmapDevice::NewL(iDrawSurface);
	/*LOGD("CFbsBitmapDevice struct size: %i bytes\n", User::AllocLen(iFBDevice));
	LogAvailableMemory();*/

	iFBGc->Activate(device);
	//LogAvailableMemory();
	//gDrawBuffer = (TUint16*)iDrawSurface->DataAddress();
	/*TSize size = iDrawSurface->SizeInPixels();
	gDrawWidth = size.iWidth;
	gDrawHeight = size.iHeight;*/
	UpdateClip();

	if(iDrawing) {
		iFBGc->UseFont(gFont);
		//LogAvailableMemory();
	}
}

void CDirScrAccEng::SetClip(const TRect& aRect) {
	if(iDrawSurface == iOffScreenBmp)
		iOffScreenClipRect = aRect;
	iCurrentClipRect = aRect;
	iCurrentClipRect.Intersection(TRect(iDrawSurface->SizeInPixels()));
	UpdateClip();
}

const TRect& CDirScrAccEng::GetScreenClip() const {
	return iOffScreenClipRect;
}

void CDirScrAccEng::UpdateClip() {
	if(iCurrentClipRect.iTl == TPoint(0,0) &&
		iCurrentClipRect.Size() == iDrawSurface->SizeInPixels())
		iFBGc->CancelClippingRect();
	else
		iFBGc->SetClippingRect(iCurrentClipRect);
}

void CDirScrAccEng::StopDrawing() {
	LOGD("SoD\n");
	DEBUG_ASSERT(iDrawing);
	LOGG("SoD1\n");

	iDirectScreenAccess->Cancel();
	LOGG("SoD2\n");
	iDrawing = EFalse;
}

void CDirScrAccEng::Restart(RDirectScreenAccess::TTerminationReasons /*aReason*/) {
	// Restart display
	// Note that this will result in the clipping region being updated
	// so that menus, overlaying dialogs, etc. will not be drawn over      
	LOGG("R\n");
	LTRAP(StartDrawingL());
}

void CDirScrAccEng::AbortNow(RDirectScreenAccess::TTerminationReasons /*aReason*/) {
	// Cancel DSA
	LOGG("AN\n");
	iDirectScreenAccess->Cancel();    
	LOGG("AN2\n");
	iDrawing = EFalse;
}

void CDirScrAccEng::UpdateScreen() {
	//LOGD("US\n");
	if(!iDrawing)
		return;
	if(iRawFrameBuf != NULL) {
		TMyDrawBuf drawBuffer(*iOffScreenBmp);
		memcpy(drawBuffer, iRawFrameBuf, iFrameBufLen);
	}
	// blit the offscreen bitmap (if used) to screen
	iScreenGc->BitBlt(TPoint(0,0), iOffScreenBmp, TRect(iWindow.Size()));
	// Force screen update: this is required for WINS, but may
	// not be for all hardware. 
	// For Series 60 devices this is necessary, 
	// we can't access screen memory directly, 
	// rather we access a screen buffer.
	iDirectScreenAccess->ScreenDevice()->Update();
	//LOGD("US4\n");
}

int CDirScrAccEng::FrameBufferGetInfo(MAFrameBufferInfo* info) {
	info->bytesPerPixel = sizeof(Pixel);
	info->bitsPerPixel = sizeof(Pixel) * 8;

#ifdef __SERIES60_3X__
	//gCurrentConvertedColor = argb & 0xFFFFFF;
	info->redMask = 0xff0000;
	info->redShift = 16;
	info->redBits = 8;
	info->greenMask = 0x00ff00;
	info->greenShift = 8;
	info->greenBits = 8;
	info->blueMask = 0x0000ff;
	info->blueShift = 0;
	info->blueBits = 8;
#elif defined(MA_PROF_SUPPORT_FRAMEBUFFER_32BIT)
	//gCurrentConvertedColor = argb;
		//((argb & 0xFF0000) >> 8) | ((argb & 0xFF00) >> 8) | ((argb & 0xFF) << 24);
	info->redMask = 0xff0000;
	info->redShift = 16;
	info->redBits = 8;
	info->greenMask = 0x00ff00;
	info->greenShift = 8;
	info->greenBits = 8;
	info->blueMask = 0x0000ff;
	info->blueShift = 0;
	info->blueBits = 8;
#else  //Series 60, 2nd Ed.
	//gCurrentConvertedColor =  //565
		//((argb & 0xF80000) >> 8) | ((argb & 0xFC00) >> 5) | ((argb & 0xF8) >> 3);
	info->redMask = 0xf800;
	info->redShift = 11;
	info->redBits = 5;
	info->greenMask = 0x07e0;
	info->greenShift = 5;
	info->greenBits = 6;
	info->blueMask = 0x001f;
	info->blueShift = 0;
	info->blueBits = 5;
#endif	//__SERIES60_3X__

	TSize size = iDrawSurface->SizeInPixels();
	info->width = size.iWidth;
	info->height = size.iHeight;
	info->pitch = size.iWidth * sizeof(Pixel);
	info->sizeInBytes = iFrameBufLen;

	info->supportsGfxSyscalls = 0; // TODO: right?

	return 1;
}

int CDirScrAccEng::FrameBufferInit(const void* data) {
	iRawFrameBuf = data;
	return 1;
}

int CDirScrAccEng::FrameBufferClose() {
	iRawFrameBuf = NULL;
	return 1;
}
