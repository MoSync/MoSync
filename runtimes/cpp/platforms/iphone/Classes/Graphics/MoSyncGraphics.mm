//
//  MoSyncGraphics.mm
//  MoSync
//
//  Created by Bogdan Iusco on 2/6/13.
//
//

#import "MoSyncGraphics.h"
#import "ScreenOrientation.h"
#import "MoSyncOpenGL.h"
#import "MoSyncUISyscalls.h"
#import "MoSyncMain.h"
#import "MoSyncFonts.h"

#include "CMGlyphDrawing.h"

namespace Base
{
	bool gClosing = false;
}

Surface* gBackbuffer = NULL;
Surface* gDrawTarget = NULL;
MAHandle gDrawTargetHandle = HANDLE_SCREEN;

uint realColor;
uint currentColor;
float currentRed, currentGreen, currentBlue;
uint oldColor;
int gWidth, gHeight;
unsigned char *gBackBufferData;
CRITICAL_SECTION exitMutex;



void MoSyncGraphicsCreateBackbuffer()
{
	Surface* oldDrawTarget = gDrawTarget;
	CGSize screenSize = [[ScreenOrientation getInstance] screenSize];
	float width = screenSize.width;
	float height = screenSize.height;

	gBackbuffer = new Surface(width, height);
	CGContextRestoreGState(gBackbuffer->context);
	CGContextTranslateCTM(gBackbuffer->context, 0, height);
	CGContextScaleCTM(gBackbuffer->context, 1.0, -1.0);
	CGContextSaveGState(gBackbuffer->context);

	gDrawTarget = gBackbuffer;
	delete oldDrawTarget;
}

SYSCALL(void, maSetClipRect(int left, int top, int width, int height))
{
	CGContextRestoreGState(gDrawTarget->context);
	CGContextSaveGState(gDrawTarget->context);
	CGContextClipToRect(gDrawTarget->context, CGRectMake(left, top, width, height));

	gDrawTarget->mImageDrawer->clipRect.x = left;
	gDrawTarget->mImageDrawer->clipRect.y = top;
	gDrawTarget->mImageDrawer->clipRect.width = width;
	gDrawTarget->mImageDrawer->clipRect.height = height;
}

SYSCALL(void, maGetClipRect(MARect *rect))
{
	Base::gSyscall->ValidateMemRange(rect, sizeof(MARect));
	rect->left = gDrawTarget->mImageDrawer->clipRect.x;
	rect->top = gDrawTarget->mImageDrawer->clipRect.y;
	rect->width = gDrawTarget->mImageDrawer->clipRect.width;
	rect->height = gDrawTarget->mImageDrawer->clipRect.height;

	/*
	 CGRect cr = CGContextGetClipBoundingBox(gDrawTarget->context);
	 rect->left = cr.origin.x;
	 rect->top = cr.origin.y;
	 rect->width = cr.size.width;
	 rect->height = cr.size.height;
	 */
}

SYSCALL(int, maSetColor(int argb))
{
	oldColor = currentColor;
	currentColor = argb;
	float red =   (float)((argb&0x00ff0000)>>16)/255.0f;
	float green = (float)((argb&0x0000ff00)>>8)/255.0f;
	float blue =  (float)((argb&0x000000ff))/255.0f;
	currentRed = red;
	currentGreen = green;
	currentBlue = blue;

	// hmmmm I don't know why I have to do this :)
	realColor = (argb&0xff00ff00)|((argb&0x00ff0000)>>16)|((argb&0x000000ff)<<16);

	return oldColor;
}

SYSCALL(void, maPlot(int posX, int posY))
{
	if(!gDrawTarget->data) DEBIG_PHAT_ERROR;
	gDrawTarget->mImageDrawer->drawPoint(posX, posY, realColor);
}

SYSCALL(void, maLine(int x0, int y0, int x1, int y1))
{
	gDrawTarget->mImageDrawer->drawLine(x0, y0, x1, y1, realColor);
}

SYSCALL(void, maFillRect(int left, int top, int width, int height))
{
	//CGContextSetRGBFillColor(gDrawTarget->context, currentRed, currentGreen, currentBlue, 1);
	gDrawTarget->mImageDrawer->drawFilledRect(left, top, width, height, realColor);
}

SYSCALL(void, maFillTriangleStrip(const MAPoint2d *points, int count))
{
	Base::SYSCALL_THIS->ValidateMemRange(points, sizeof(MAPoint2d) * count);
	CHECK_INT_ALIGNMENT(points);
	MYASSERT(count >= 3, ERR_POLYGON_TOO_FEW_POINTS);
	for(int i = 2; i < count; i++) {
		gDrawTarget->mImageDrawer->drawTriangle(
												points[i-2].x,
												points[i-2].y,
												points[i-1].x,
												points[i-1].y,
												points[i].x,
												points[i].y,
												realColor);
	}
}

SYSCALL(void, maFillTriangleFan(const MAPoint2d *points, int count))
{
	Base::SYSCALL_THIS->ValidateMemRange(points, sizeof(MAPoint2d) * count);
	CHECK_INT_ALIGNMENT(points);
	MYASSERT(count >= 3, ERR_POLYGON_TOO_FEW_POINTS);
	for(int i = 2; i < count; i++) {
		gDrawTarget->mImageDrawer->drawTriangle(
												points[0].x,
												points[0].y,
												points[i-1].x,
												points[i-1].y,
												points[i].x,
												points[i].y,
												realColor);
	}
}

SYSCALL(void, maUpdateScreen())
{
	if(Base::gClosing) {
		return;
	}
#ifdef SUPPORT_OPENGL_ES
	else if(sOpenGLView != -1) {
		maWidgetSetProperty(sOpenGLView, "invalidate", "");
		return;
	}
#endif
	else if(isNativeUIEnabled()) {
		return;
	}

	// we must check if the canvas is enabled, because the mosync thread will be locked until the surface is drawn to the screen.

	MoSync_UpdateView(gBackbuffer->image);
}

SYSCALL(void, maDrawImage(MAHandle image, int left, int top))
{
	Surface* img = Base::gSyscall->resources.get_RT_IMAGE(image);
	gDrawTarget->mImageDrawer->drawImage(left, top, img->mImageDrawer);
}

void flipColorsFromAxGyToAyGx(int *buf, int width, int height, int scanlength) {
	int *ptr = (int*)buf;
	for(int j = 0; j < height; j++) {
		for(int i = 0; i < width; i++) {
			int abgr = *ptr;
			*ptr = (abgr&0xff00ff00)|((abgr&0x00ff0000)>>16)|((abgr&0x000000ff)<<16);
			ptr++;
		}
		ptr+=-width+scanlength;
	}
}

SYSCALL(void, maDrawRGB(const MAPoint2d* dstPoint, const void* src, const MARect* srcRect,
						int scanlength))
{
	Base::gSyscall->ValidateMemRange(dstPoint, sizeof(MAPoint2d));
	Base::gSyscall->ValidateMemRange(srcRect, sizeof(MARect));
	Base::gSyscall->ValidateMemRange(src, scanlength*srcRect->height*4);

	Surface *srcSurface = new
	Surface(srcRect->width, srcRect->height, (char*)src, kCGImageAlphaPremultipliedLast|kCGBitmapByteOrder32Big, scanlength*4);

	ClipRect srcRectCR;
	srcRectCR.x = srcRect->left;
	srcRectCR.y = srcRect->top;
	srcRectCR.width = srcRect->width;
	srcRectCR.height = srcRect->height;
	gDrawTarget->mImageDrawer->drawImageRegion(dstPoint->x, dstPoint->y, &srcRectCR, srcSurface->mImageDrawer, 0);
	delete srcSurface;

	flipColorsFromAxGyToAyGx(&((int*)gDrawTarget->data)[srcRect->left+srcRect->top*(gDrawTarget->rowBytes>>2)], srcRect->width, srcRect->height, gDrawTarget->rowBytes>>2);
}

SYSCALL(void, maDrawImageRegion(MAHandle image, const MARect* src, const MAPoint2d* dstTopLeft,
								int transformMode))
{
	Base::gSyscall->ValidateMemRange(dstTopLeft, sizeof(MAPoint2d));
	Base::gSyscall->ValidateMemRange(src, sizeof(MARect));
	Surface* img = Base::gSyscall->resources.get_RT_IMAGE(image);

	ClipRect srcRect;
	srcRect.x = src->left;
	srcRect.y = src->top;
	srcRect.width = src->width;
	srcRect.height = src->height;
	gDrawTarget->mImageDrawer->drawImageRegion(dstTopLeft->x, dstTopLeft->y, &srcRect, img->mImageDrawer, transformMode);
}

SYSCALL(MAExtent, maGetImageSize(MAHandle image))
{
	Surface* img = Base::gSyscall->resources.get_RT_IMAGE(image);
	return EXTENT(img->width, img->height);
}

SYSCALL(void, maGetImageData(MAHandle image, void* dst, const MARect* src, int scanlength)) {
	Base::gSyscall->ValidateMemRange(src, sizeof(MARect));
	Surface* img = Base::gSyscall->resources.get_RT_IMAGE(image);
	int x = src->left;
	int y = src->top;
	int width = src->width;
	int height = src->height;
	Base::gSyscall->ValidateMemRange(dst, src->height*scanlength);

	CGRect smallRect = CGRectMake(x, y, width, height);
	CGImageRef smallImage = CGImageCreateWithImageInRect(img->image, smallRect);

	// First get the image into your data buffer
	//int imgwidth = CGImageGetWidth(img->image);
	//int imgheight = CGImageGetHeight(img->image);
	//memset(dst, 0, scanlength*height*4);

	Surface *srcSurface = new Surface(smallImage);

	//Surface *dstSurface = new Surface(imgwidth, imgheight, (char*) dst, kCGImageAlphaPremultipliedLast|kCGBitmapByteOrder32Big, scanlength*4);

	int* dstptr = (int*)dst;
	int* srcptr = (int*)srcSurface->data;
	for(int i = 0; i < height; i++) {
		memcpy(dstptr, srcptr, width*4);
		srcptr+=width;
		dstptr+=scanlength;
	}

	//CGContextDrawImage(dstSurface->context, CGRectMake(0, 0, width, height), smallImage);
	//CGImageRelease(smallImage);

	flipColorsFromAxGyToAyGx((int*)dst, width, height, scanlength);
	delete srcSurface;
	//delete dstSurface;
}

SYSCALL(MAHandle, maSetDrawTarget(MAHandle handle))
{
	MAHandle temp = gDrawTargetHandle;
	if(gDrawTargetHandle != HANDLE_SCREEN) {
		Base::SYSCALL_THIS->resources.extract_RT_FLUX(gDrawTargetHandle);
		ROOM(Base::SYSCALL_THIS->resources.add_RT_IMAGE(gDrawTargetHandle, gDrawTarget));
		gDrawTargetHandle = HANDLE_SCREEN;
	}
	if(handle == HANDLE_SCREEN) {
		gDrawTarget = gBackbuffer;
	} else {
		Surface* img = Base::SYSCALL_THIS->resources.extract_RT_IMAGE(handle);
		if(!img->data)
			BIG_PHAT_ERROR(ERR_RES_INVALID_TYPE);

		gDrawTarget = img;
		ROOM(Base::SYSCALL_THIS->resources.add_RT_FLUX(handle, NULL));
	}
	gDrawTargetHandle = handle;
	return temp;
}

SYSCALL(int, maCreateImageFromData(MAHandle placeholder, MAHandle data, int offset, int size))
{
	MYASSERT(size>0, ERR_DATA_OOB);
	Base::Stream* stream = Base::gSyscall->resources.get_RT_BINARY(data);
	Surface *bitmap = 0;

	int len;
	TEST(stream->length(len));

	if(offset<0 || offset+size>len)
		BIG_PHAT_ERROR(ERR_DATA_OOB);

	if(!stream->ptrc()) {
		// is not a memstream, read it to a buffer and load it.
		MYASSERT(stream->seek(Base::Seek::Start, offset), ERR_DATA_OOB);
		unsigned char *data = new unsigned char[size];
		if(data==NULL) return RES_OUT_OF_MEMORY;
		MYASSERT(stream->read(data, size), ERR_DATA_ACCESS_FAILED);
		Base::MemStreamC memStream(data, size);
		bitmap = Base::gSyscall->loadImage(memStream);
		delete data;
	} else {
		const char *ptr = (const char*) stream->ptrc();
		Base::MemStreamC memStream((const void*)&ptr[offset], size);
		bitmap = Base::gSyscall->loadImage(memStream);
	}

	if(!bitmap) return RES_BAD_INPUT;
	if(!bitmap->image) {
		delete bitmap;
		// most likely bad input.
		return RES_BAD_INPUT;
	}

	return Base::gSyscall->resources.add_RT_IMAGE(placeholder, bitmap);
}

SYSCALL(int, maCreateImageRaw(MAHandle placeholder, const void* src, MAExtent size, int processAlpha))
{
	int width = EXTENT_X(size); int height = EXTENT_Y(size);
	Base::gSyscall->ValidateMemRange(src, width*height*4);
	int byteSize = EXTENT_X(size)*EXTENT_Y(size)*4;
	char *data = new char[byteSize];
	if(!data) return RES_OUT_OF_MEMORY;
	memcpy(data, src, byteSize);
	Surface *bitmap = new Surface(EXTENT_X(size), EXTENT_Y(size), data, processAlpha?kCGImageAlphaPremultipliedLast:kCGImageAlphaNoneSkipLast);
	if(!bitmap) {
		delete data;
		return RES_OUT_OF_MEMORY;
	}

	if(!bitmap->context || !bitmap->image || !bitmap->data) {
		delete bitmap;
		delete data;
		return RES_OUT_OF_MEMORY;
	}

	bitmap->mOwnData = true;
	flipColorsFromAxGyToAyGx((int*)data, EXTENT_X(size), EXTENT_Y(size), EXTENT_X(size));
	return Base::gSyscall->resources.add_RT_IMAGE(placeholder, bitmap);
}

SYSCALL(int, maCreateDrawableImage(MAHandle placeholder, int width, int height))
{
	MYASSERT(width > 0 && height > 0, ERR_IMAGE_SIZE_INVALID);
	Surface *surf = new Surface(width, height);
	if(!surf) return RES_OUT_OF_MEMORY;
	if(!surf->context || !surf->image || !surf->data) {
		delete surf;
		return RES_OUT_OF_MEMORY;
	}

	return Base::gSyscall->resources.add_RT_IMAGE(placeholder, surf);
}

SYSCALL(int, maFrameBufferGetInfo(MAFrameBufferInfo *info))
{
	int bytesPerRow = CGBitmapContextGetBytesPerRow(gBackbuffer->context);
	int bitsPerPixel = CGBitmapContextGetBitsPerPixel(gBackbuffer->context);
	int bytesPerPixel = bytesPerRow/gWidth;
	int bitsPerComponent = CGBitmapContextGetBitsPerComponent(gBackbuffer->context);

	info->bitsPerPixel = bitsPerPixel;
	info->bytesPerPixel = bytesPerPixel;

	//CGBitmapInfo bInfo = CGImageGetBitmapInfo(gBackbuffer->image);

	info->redMask = 0x00ff0000;
	info->greenMask = 0x0000ff00;
	info->blueMask = 0x000000ff;
	info->redShift = 16;
	info->greenShift = 8;
	info->blueShift = 0;

	info->sizeInBytes = bytesPerRow*gHeight;
	info->width = gWidth;
	info->height = gHeight;
	info->pitch = bytesPerRow;

	info->redBits = bitsPerComponent;
	info->greenBits = bitsPerComponent;
	info->blueBits = bitsPerComponent;
	info->supportsGfxSyscalls = 1;

	return 1;
}

static Surface *sInternalBackBuffer = NULL;

SYSCALL(int, maFrameBufferInit(const void *data))
{
	if(sInternalBackBuffer!=NULL) return 0;
	sInternalBackBuffer = gBackbuffer;

	gBackbuffer = new Surface(gWidth, gHeight, (char*)data, kCGImageAlphaNoneSkipFirst | kCGBitmapByteOrder32Little);
	CGContextRestoreGState(gBackbuffer->context);
	CGContextTranslateCTM(gBackbuffer->context, 0, gHeight);
	CGContextScaleCTM(gBackbuffer->context, 1.0, -1.0);
	CGContextSaveGState(gBackbuffer->context);
	gDrawTarget = gBackbuffer;
	return 1;
}

SYSCALL(int, maFrameBufferClose())
{
	if(sInternalBackBuffer==NULL) return 0;
	delete gBackbuffer;
	gBackbuffer = sInternalBackBuffer;
	sInternalBackBuffer = NULL;
	gDrawTarget = gBackbuffer;
	return 1;
}

/*
 * Text related
 */
int stringLength(const wchar_t* str) {
    int len = 0;
    while(*str++ != 0) {
        if(*str == '\n') return len;
        len++;
    }
    return len;
}

SYSCALL(void, maDrawTextW(int left, int top, const wchar* str))
{
    int numGlyphs, resultCode;
	const UniChar* strUnichar;
	const UInt32* strUInt32;

	if (gNativeMode)
	{
		// With native library approach wchar type 32 bits long.
		strUInt32 = (const UInt32*)str;
		numGlyphs = wchartLength((const wchar_t*)str);
	}
	else
	{
		strUnichar = (const UniChar*)str;
		numGlyphs = wcharLength(str);
	}

    FontInfo *currentFont=sFontList[gCurrentFontHandle-1];
    initCGFont(currentFont);
    if(numGlyphs==0) return;
    CGGlyph* glyphs = new CGGlyph[numGlyphs];

	if (gNativeMode)
	{
		resultCode = CMFontGetGlyphsForUnichars(currentFont->cgFontObject, strUInt32, glyphs, numGlyphs);
	}
	else
	{
		resultCode = CMFontGetGlyphsForUnichars(currentFont->cgFontObject, strUnichar, glyphs, numGlyphs);
	}

	//Not all fonts in the device are supported for Unicode glyphs
    //We must check whether the operation was successful
    if(!resultCode)
    {
        delete glyphs;
        return;
    }

    CGContextSetRGBFillColor(gDrawTarget->context, currentRed, currentGreen, currentBlue, 1);
    CGContextSetTextDrawingMode(gDrawTarget->context, kCGTextFill);
    CGContextSetTextPosition (gDrawTarget->context, 0, 0);
    CGContextSetAllowsAntialiasing (gDrawTarget->context, true);
    CGContextShowGlyphsAtPoint(gDrawTarget->context, left, top+sFontList[gCurrentFontHandle-1]->size, glyphs, numGlyphs);
    CGContextSetAllowsAntialiasing (gDrawTarget->context, false);
    delete glyphs;
}

SYSCALL(void, maDrawText(int left, int top, const char* str))
{
    CGContextSetRGBFillColor(gDrawTarget->context, currentRed, currentGreen, currentBlue, 1);
    CGContextSetTextDrawingMode(gDrawTarget->context, kCGTextFill);
    CGContextSetTextPosition (gDrawTarget->context, 0, 0);
    CGContextSetAllowsAntialiasing (gDrawTarget->context, true);
    CGContextShowTextAtPoint(gDrawTarget->context, left, top+sFontList[gCurrentFontHandle-1]->size, str, strlen(str));
    CGContextSetAllowsAntialiasing (gDrawTarget->context, false);
}

SYSCALL(MAExtent, maGetTextSizeW(const wchar* str))
{
    FontInfo *currentFont=sFontList[gCurrentFontHandle-1];
    initCGFont(currentFont);
    int numGlyphs = wcharLength(str);
    if(numGlyphs==0) return EXTENT(0, 0);
    CGGlyph* glyphs = new CGGlyph[numGlyphs];
    CMFontGetGlyphsForUnichars(currentFont->cgFontObject, (const UniChar*)str, glyphs, numGlyphs);
    CGContextSetTextDrawingMode(gDrawTarget->context, kCGTextInvisible);
    CGContextSetTextPosition (gDrawTarget->context, 0, 0);
    CGContextShowGlyphsAtPoint(gDrawTarget->context, 0, 0, glyphs, numGlyphs);
    CGPoint after = CGContextGetTextPosition(gDrawTarget->context);
    int width = after.x;
    int height = (int)sFontList[gCurrentFontHandle-1]->size; //Might be wrong???
    delete glyphs;
    return EXTENT(width, height);
}

SYSCALL(MAExtent, maGetTextSize(const char* str))
{
    CGContextSetTextDrawingMode(gDrawTarget->context, kCGTextInvisible);
    CGContextSetTextPosition (gDrawTarget->context, 0, 0);
    CGContextShowTextAtPoint(gDrawTarget->context, 0, 0, str, strlen(str));
    CGPoint after = CGContextGetTextPosition(gDrawTarget->context);
    int width = after.x;
    int height = (int)sFontList[gCurrentFontHandle-1]->size; //Might be wrong???
    return EXTENT(width, height);
}
