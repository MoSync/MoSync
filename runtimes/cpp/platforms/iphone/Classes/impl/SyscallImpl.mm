/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#include "config_platform.h"
#import <UIKit/UIKit.h>
#import <AssetsLibrary/AssetsLibrary.h>
#import <MessageUI/MessageUI.h>
#include <string>
#include <vector>
#include <map>
#include <time.h>
#include <math.h>
#include <helpers/fifo.h>
#include <helpers/log.h>
#include <helpers/helpers.h>
#include <helpers/smartie.h>
#include <helpers/cpp_defs.h>
#include <MemStream.h>
#include <FileStream.h>
#include "Syscall.h"
#include "MoSyncDB.h"
#include "PimSyscall.h"
#include "OptionsDialogView.h"
#include <CoreMedia/CoreMedia.h>

#include <helpers/CPP_IX_GUIDO.h>
//#include <helpers/CPP_IX_ACCELEROMETER.h>
#include "MoSyncPanic.h"

#include <helpers/CPP_IX_WIDGET.h>
#include "MoSyncUISyscalls.h"
#import "CameraPreviewWidget.h"
#import "CameraConfirgurator.h"
#import "ImagePickerController.h"
#include "netImpl.h"


#define NETWORKING_H
#include "networking.h"
#include <bluetooth/discovery.h>
#include <base_errors.h>
using namespace MoSyncError;

#include <core/core.h>

#include "MoSyncMain.h"
#import "MoSyncAppDelegate.h"

#include "iphone_helpers.h"
#include "CMGlyphDrawing.h"

#import <AVFoundation/AVFoundation.h>
#include <AudioToolbox/AudioToolbox.h>

#ifdef SUPPORT_OPENGL_ES
#define DONT_WANT_IX_OPENGL_ES_TYPEDEFS
#include <helpers/CPP_IX_OPENGL_ES.h>
#include <helpers/CPP_IX_GL1.h>
#include <helpers/CPP_IX_GL2.h>
#include <helpers/CPP_IX_GL_OES_FRAMEBUFFER_OBJECT.h>
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES1/glext.h>
#include "../../../../generated/gl.h.cpp"
#endif

#include <helpers/CPP_IX_AUDIO.h>
#include "AudioSyscall.h"

extern ThreadPool gThreadPool;

#define NOT_IMPLEMENTED BIG_PHAT_ERROR(ERR_FUNCTION_UNIMPLEMENTED)

//This delegate is needed for the SMS system, because iOS does not automatically
//hide the sms window after the user sends an SMS or clicks cancel
@interface SMSResultDelegate:NSObject<MFMessageComposeViewControllerDelegate>{
}

- (void)messageComposeViewController:(MFMessageComposeViewController *)controller
				 didFinishWithResult:(MessageComposeResult)result;
@end

@implementation SMSResultDelegate

- (void)messageComposeViewController:(MFMessageComposeViewController *)controller
				 didFinishWithResult:(MessageComposeResult)result{
	MoSyncUI* msUI = getMoSyncUI();
	[msUI hideModal];

	MAEvent event;
	event.type = EVENT_TYPE_SMS;

	if (result == MessageComposeResultCancelled) {
		event.status = MA_SMS_RESULT_NOT_SENT;
	}
	else if (result == MessageComposeResultFailed) {
		event.status = MA_SMS_RESULT_NOT_DELIVERED;
	}
	else if (result == MessageComposeResultSent){
		MAEvent firstEvent;
		firstEvent.type = EVENT_TYPE_SMS;
		firstEvent.status = MA_SMS_RESULT_SENT;
		Base::gEventQueue.put(firstEvent);

		event.status = MA_SMS_RESULT_DELIVERED;
	}

	Base::gEventQueue.put(event);
}
@end

namespace Base {

	//***************************************************************************
	//Defines and declarations
	//***************************************************************************
	static void MAUpdateScreen();

	Syscall* gSyscall;

	uint realColor;
	uint currentColor;
	float currentRed, currentGreen, currentBlue;
	uint oldColor;

	int gWidth, gHeight;

	Surface* gBackbuffer;
	Surface* gDrawTarget;
	MAHandle gDrawTargetHandle = HANDLE_SCREEN;

	unsigned char *gBackBufferData;

	mach_timebase_info_data_t gTimeBase;
	uint64_t gTimeStart;

	EventQueue gEventQueue;
	static bool gEventOverflow	= false;
	static bool gClosing		= false;

	int gVolume = -1;

	bool exited = false;
	CRITICAL_SECTION exitMutex;

    //Holds information that can be used by the various font systems to generate their own font objects.
    struct FontInfo{
        CFStringRef name;       //Postscript name of the font
        CGFloat size;           //Size of the font
        UIFont *uiFontObject;   //Instantiated when first needed
        CGFontRef cgFontObject; //Instantiated when first needed
    };

    //Internal storage indexed by MAHandle-1
    static std::vector<FontInfo*> sFontList;

    const int FONT_DEFAULT_AMOUNT=15;
    //The array holding the default system font names
    //Three slots are unused due to the indexing system
    CFStringRef gDefaultFontNames[FONT_DEFAULT_AMOUNT];

    MAHandle gCurrentFontHandle; //Handle to the currently selected font


    /* The following consts are used for the indexing system.
     * By using bitwise OR, you can get the index for that type and style combination
     * in gDefaultFontNames.
     * It's better than using the constants in maapi.h, because those can change.
     */
    const short FONT_SANS_SERIF_INDEX=0;
    const short FONT_SERIF_INDEX=1;
    const short FONT_MONOSPACE_INDEX=2;
    const short FONT_NORMAL_INDEX=0;
    const short FONT_BOLD_INDEX=4;
    const short FONT_ITALIC_INDEX=8;

    //The style and type for the initally selected font
    const short INITIAL_FONT_STYLE=FONT_STYLE_NORMAL;
    const short INITIAL_FONT_TYPE=FONT_TYPE_SANS_SERIF;

    extern "C" MAHandle maFontLoadDefault(int type, int style, int size);
    extern "C" MAHandle maFontSetCurrent(MAHandle font);



#ifdef SUPPORT_OPENGL_ES
	static MAHandle sOpenGLScreen = -1;
	static MAHandle sOpenGLView = -1;
#endif

	void MALibQuit();

	//***************************************************************************
	// Resource loading
	//***************************************************************************
	enum ImageFormat {
		JPEG = 0,
		PNG = 1,
		UNKNOWN = 2
	};

	// this must be fixed in base.. too tired
	Surface* Syscall::loadImage(MemStream& s) {
		return loadImage((MemStreamC&)s);
	}

	Surface* Syscall::loadImage(MemStreamC& s)
	{
		int len;
		TEST(s.length(len));
		unsigned char *data = (unsigned char*)s.ptrc();
		ImageFormat format;
		int orientation = 1; //Default orientation
#define E(x, y) (data[x]==y)
		if(len>3 && E(0, 0xff) && E(1, 0xd8)) {
			format = JPEG;
			for(int i=0;i<len;i++){
				if(E(i,0x01) && E(i+1,0x12)){ //The orientation tag
					orientation = (int)data[i+9]; //The orientation value
					break;
				}
			}
		}
		else if(len>7 && E(0, 0x89) && E(1, 0x50) && E(2, 0x4e) && E(3, 0x47) && E(4, 0x0d) && E(5, 0x0a) && E(6, 0x1a) && E(7, 0x0a) ) {
			format = PNG;
		}
		else {
			return NULL;
		}

		CFDataRef png_data = CFDataCreate (NULL,data,len);
		CGDataProviderRef dpr  = CGDataProviderCreateWithCFData (png_data);

		CGImageRef imageRef;
		switch(format) {
			case JPEG: imageRef = CGImageCreateWithJPEGDataProvider(dpr, NULL, true, kCGRenderingIntentDefault); break;
			case PNG: imageRef = CGImageCreateWithPNGDataProvider(dpr, NULL, true, kCGRenderingIntentDefault); break;
            default: {
             CGDataProviderRelease(dpr);
             CFRelease(png_data);
             return NULL;
            }
		}

		CGDataProviderRelease(dpr);

		CFRelease(png_data);

		Surface *imageSurface = new Surface(imageRef);

		imageSurface->orientation = orientation;

		return imageSurface;
	}

	Surface* Syscall::loadSprite(void* surface, ushort left, ushort top,
		ushort width, ushort height, ushort cx, ushort cy)
	{
		return NULL;
	}

	//***************************************************************************
	// Helpers
	//***************************************************************************

	static bool MALibInit() {

		InitializeCriticalSection(&exitMutex);

		gBackbuffer = new Surface(gWidth, gHeight);
		CGContextRestoreGState(gBackbuffer->context);
		CGContextTranslateCTM(gBackbuffer->context, 0, gHeight);
		CGContextScaleCTM(gBackbuffer->context, 1.0, -1.0);
		CGContextSaveGState(gBackbuffer->context);

        //Construction of the default font names array
        CFStringEncoding enc=kCFStringEncodingMacRoman;

        gDefaultFontNames[FONT_SERIF_INDEX|FONT_NORMAL_INDEX]=
                                        CFStringCreateWithCString(NULL, "TimesNewRomanPSMT",enc);
        gDefaultFontNames[FONT_SERIF_INDEX|FONT_BOLD_INDEX]=
                                        CFStringCreateWithCString(NULL, "TimesNewRomanPS-BoldMT",enc);
        gDefaultFontNames[FONT_SERIF_INDEX|FONT_ITALIC_INDEX]=
                                        CFStringCreateWithCString(NULL, "TimesNewRomanPS-ItalicMT",enc);
        gDefaultFontNames[FONT_SERIF_INDEX|FONT_BOLD_INDEX|FONT_ITALIC_INDEX]=
                                        CFStringCreateWithCString(NULL, "TimesNewRomanPS-BoldItalicMT",enc);
        gDefaultFontNames[FONT_SANS_SERIF_INDEX|FONT_NORMAL_INDEX]=
                                        CFStringCreateWithCString(NULL, "Helvetica",enc);
        gDefaultFontNames[FONT_SANS_SERIF_INDEX|FONT_BOLD_INDEX]=
                                        CFStringCreateWithCString(NULL, "Helvetica-Bold",enc);
        gDefaultFontNames[FONT_SANS_SERIF_INDEX|FONT_ITALIC_INDEX]=
                                        CFStringCreateWithCString(NULL, "Helvetica-Oblique",enc);
        gDefaultFontNames[FONT_SANS_SERIF_INDEX|FONT_BOLD_INDEX|FONT_ITALIC_INDEX]=
                                        CFStringCreateWithCString(NULL, "Helvetica-BoldOblique",enc);
        gDefaultFontNames[FONT_MONOSPACE_INDEX|FONT_NORMAL_INDEX]=
                                        CFStringCreateWithCString(NULL, "Courier",enc);
        gDefaultFontNames[FONT_MONOSPACE_INDEX|FONT_BOLD_INDEX]=
                                        CFStringCreateWithCString(NULL, "Courier-Bold",enc);
        gDefaultFontNames[FONT_MONOSPACE_INDEX|FONT_ITALIC_INDEX]=
                                        CFStringCreateWithCString(NULL, "Courier-Oblique",enc);
        gDefaultFontNames[FONT_MONOSPACE_INDEX|FONT_BOLD_INDEX|FONT_ITALIC_INDEX]=
                                        CFStringCreateWithCString(NULL, "Courier-BoldOblique",enc);

        gDrawTarget = gBackbuffer;

        //Setting the initially selected font. "gHeight/40" was used originally, is kept for backwards compatibility
        MAHandle initFontHandle=maFontLoadDefault(INITIAL_FONT_TYPE,INITIAL_FONT_STYLE,gHeight/40);
        maFontSetCurrent(initFontHandle);

		mach_timebase_info( &gTimeBase );
		//gTimeConversion = 1e-6 * (double)machInfo.numer/(double)machInfo.denom;
		gTimeStart = mach_absolute_time();


		MANetworkInit();

		MAPimInit();
        MAAudioInit();

		// init some image.h optimizations.
		initMulTable();
		initRecipLut();

		return true;
	}

	void MALibQuit() {
		DeleteCriticalSection(&exitMutex);
		MANetworkClose();
        MAPimClose();
        MAAudioClose();
        [OptionsDialogView deleteInstance];
        [ImagePickerController deleteInstance];
	}


	bool MAProcessEvents() //returns true if maWait should return
	{
		return false;
	}

	//***************************************************************************
	// Syscall class
	//***************************************************************************

	Syscall::Syscall(int w, int h)
	{
		gWidth = w;
		gHeight = h;
		gSyscall = this;
		bool ret = MALibInit();
		if(ret == false)
		{
			// if program is already open or something failed...
			exit(0);
		}
		init();
	}

	void Syscall::platformDestruct() {
		MALibQuit();
	}

	//***************************************************************************
	// Proper syscalls
	//***************************************************************************
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
		gSyscall->ValidateMemRange(rect, sizeof(MARect));
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

	SYSCALL(int, maSetColor(int argb)) {
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

	SYSCALL(void, maPlot(int posX, int posY)) {
		if(!gDrawTarget->data) DEBIG_PHAT_ERROR;
		gDrawTarget->mImageDrawer->drawPoint(posX, posY, realColor);

	}

	SYSCALL(void, maLine(int x0, int y0, int x1, int y1)) {
		gDrawTarget->mImageDrawer->drawLine(x0, y0, x1, y1, realColor);
	}

	SYSCALL(void, maFillRect(int left, int top, int width, int height)) {
		//CGContextSetRGBFillColor(gDrawTarget->context, currentRed, currentGreen, currentBlue, 1);
		gDrawTarget->mImageDrawer->drawFilledRect(left, top, width, height, realColor);
	}

	SYSCALL(void, maFillTriangleStrip(const MAPoint2d *points, int count)) {
		SYSCALL_THIS->ValidateMemRange(points, sizeof(MAPoint2d) * count);
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

	SYSCALL(void, maFillTriangleFan(const MAPoint2d *points, int count)) {
		SYSCALL_THIS->ValidateMemRange(points, sizeof(MAPoint2d) * count);
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

	int stringLength(const wchar_t* str) {
		int len = 0;
		while(*str++ != 0) {
			if(*str == '\n') return len;
			len++;
		}
		return len;
	}
    // This is a helper function that creates and stores a FontInfo with the provided arguments
    MAHandle createFontInfo(CFStringRef name, GLfloat size, UIFont *uiFontObject, CGFontRef cgFontObject)
    {
        MAHandle handle;
        FontInfo *newFontInfo=new FontInfo;
        newFontInfo->name=name;
        newFontInfo->size=size;
        newFontInfo->uiFontObject=uiFontObject; //Can be NULL
        newFontInfo->cgFontObject=cgFontObject; //Can be NULL

        int i;
        for(i=0; i<sFontList.size() ; i++)
        {
            if(!sFontList[i])break; //Searching for first NULL cell in the vector
        }
        if(i<sFontList.size())
        {
            sFontList[i]=newFontInfo; //Inserting at the first empty position in the vector
        }
        else
        {
            sFontList.push_back(newFontInfo); //Inserting at the end of the vector
        }

        handle=i+1; //Handles start from 1

        return handle;
    }

    SYSCALL(MAHandle, maFontLoadDefault(int type, int style, int size)){

        int defaultFontIndex=0; //Index to gDefaultFontNames
        if(size<=0)
        {
            return RES_FONT_INVALID_SIZE;
        }
        switch (type) {
            case FONT_TYPE_SERIF:
                defaultFontIndex|=FONT_SERIF_INDEX;
                break;

            case FONT_TYPE_SANS_SERIF:
                defaultFontIndex|=FONT_SANS_SERIF_INDEX;
                break;

            case FONT_TYPE_MONOSPACE:
                defaultFontIndex|=FONT_MONOSPACE_INDEX;
                break;
        }

        if(style & FONT_STYLE_NORMAL) defaultFontIndex|=FONT_NORMAL_INDEX;
        if(style & FONT_STYLE_BOLD) defaultFontIndex|=FONT_BOLD_INDEX;
        if(style & FONT_STYLE_ITALIC) defaultFontIndex|=FONT_ITALIC_INDEX;
        //At this point defaultFontIndex is the combined value of type and style

        if(defaultFontIndex<0 || defaultFontIndex>=FONT_DEFAULT_AMOUNT || gDefaultFontNames[defaultFontIndex]==NULL)
        {
            return RES_FONT_NO_TYPE_STYLE_COMBINATION;
        }

        // We retain the string because we don't want maFontDelete to delete it, as it might be used by other handles
        CFRetain(gDefaultFontNames[defaultFontIndex]);
        return createFontInfo(gDefaultFontNames[defaultFontIndex],(CGFloat)size,NULL,NULL);
    }

    SYSCALL(MAHandle, maFontLoadWithName(const char* name, int size)){
        CFStringRef fontName=CFStringCreateWithCString(NULL,name,kCFStringEncodingMacRoman);
		if(size<=0)
        {
            return RES_FONT_INVALID_SIZE;
        }
        //Getting a UIFont object is probably the least expensive way to test if it exists.
        //Also, it's more probably that a user will use the nativeUI system rather than
        //maDrawText(W)
        UIFont *uiFontObject=[UIFont fontWithName:(NSString*)fontName size:(GLfloat)size];

        if(!uiFontObject)
        {
            return RES_FONT_NAME_NONEXISTENT;
        }

        [uiFontObject retain];
        return createFontInfo(fontName,(CGFloat)size,uiFontObject,NULL);

    }

    SYSCALL(MAHandle, maFontDelete(MAHandle font)){
        int index=font-1;

        if(index<0 || index>=sFontList.size() || sFontList[index]==NULL)
        {
            return RES_FONT_INVALID_HANDLE;
        }

        FontInfo *toDelete=sFontList[index];

        CFRelease(toDelete->name);
        if(toDelete->uiFontObject)
        {
            [toDelete->uiFontObject release];
        }
        if(toDelete->cgFontObject)
        {
            CGFontRelease(toDelete->cgFontObject);
        }

        delete toDelete;
        sFontList[index]=NULL; //Make sure to create a NULL hole in the vector
        return RES_FONT_OK;
    }

    //This will store all the font names once maFontGetCount is call for the first time
    //Should I store them, or querry the iOS classes each time? (they are not straightforward)
    //My calculation is that without the objective C overhead for the NSString classes,
    //it will take up a bit over 1KB of memory.
    NSMutableArray *gDeviceFontNames;

    SYSCALL(int, maFontGetCount()){
        if(!gDeviceFontNames) //Lazy initialization
        {
            gDeviceFontNames=[[NSMutableArray alloc] init ];

            //List of font family names. We won't save those, but we need them for now
            NSArray *familyNames=[UIFont familyNames];
            NSEnumerator *familyEnumerator=[[familyNames objectEnumerator] retain];

            NSString *familyName;
            while(familyName=[familyEnumerator nextObject])
            {
                //These are the names we need
                NSArray *fontNamesInFamily=[UIFont fontNamesForFamilyName:familyName];

                [gDeviceFontNames addObjectsFromArray:fontNamesInFamily];

            }
            [familyEnumerator release];
        }

        return gDeviceFontNames.count;
    }

    SYSCALL(int, maFontGetName(int index, char* buffer, int bufferLength)){
        if(!gDeviceFontNames) //maFontGetCount must be called first to initialize this
        {
            return RES_FONT_LIST_NOT_INITIALIZED;
        }
        NSString *fontName=[gDeviceFontNames objectAtIndex:index];

        //buffer must be large enough to hold the string
        //lenghtOfBytes does not include terminating '\0',
        //That's why we use less or equal
        if(!fontName || bufferLength<=[fontName lengthOfBytesUsingEncoding:NSASCIIStringEncoding])
        {
            return RES_FONT_INSUFFICIENT_BUFFER;
        }

        //strncpy will also fill the rest of the buffer with '\0' characters
        strncpy(buffer, [fontName cStringUsingEncoding:NSASCIIStringEncoding], bufferLength);

        //Increase by one for the terminating '\0'
        return [fontName lengthOfBytesUsingEncoding:NSASCIIStringEncoding]+1;
    }


    SYSCALL(MAHandle, maFontSetCurrent(MAHandle font)){
        if(font<1||font>sFontList.size()||!sFontList[font-1])
        {
            printf("wrong MAHandle");
            return RES_FONT_INVALID_HANDLE;
        }

        MAHandle prevHandle=gCurrentFontHandle;

        gCurrentFontHandle=font;
        FontInfo *currentFont=sFontList[gCurrentFontHandle-1];

        //Used by maDrawText. It must be set now, unlike the other systems.
        CGContextSelectFont(gDrawTarget->context,
                            CFStringGetCStringPtr(currentFont->name,kCFStringEncodingMacRoman),
                            currentFont->size, kCGEncodingMacRoman);

        return prevHandle;

    }

    //Used to initialize (if needed) and return a UIFont object from a Font handle, to be used by NativeUI
    UIFont* getUIFontObject(MAHandle fontHandle)
    {
        if(fontHandle<1||fontHandle>sFontList.size()||!sFontList[fontHandle-1])
        {
            printf("wrong MAHandle");
            return NULL;
        }

        FontInfo *selectedFont=sFontList[fontHandle-1];

        if (selectedFont->uiFontObject==NULL)
        {
            selectedFont->uiFontObject=
                            [[UIFont fontWithName:(NSString *) selectedFont->name size:selectedFont->size] retain];
        }
		return selectedFont->uiFontObject;
    }

    //Used to instantiate the CGFont object only when needed
    inline void initCGFont(FontInfo *fontInfo)
    {
        if(!fontInfo->cgFontObject)
        {
            fontInfo->cgFontObject =CGFontCreateWithFontName(fontInfo->name);
        }
    }

	SYSCALL(MAExtent, maGetTextSize(const char* str)) {
		CGContextSetTextDrawingMode(gDrawTarget->context, kCGTextInvisible);
		CGContextSetTextPosition (gDrawTarget->context, 0, 0);
		CGContextShowTextAtPoint(gDrawTarget->context, 0, 0, str, strlen(str));
		CGPoint after = CGContextGetTextPosition(gDrawTarget->context);
		int width = after.x;
		int height = (int)sFontList[gCurrentFontHandle-1]->size; //Might be wrong???
		return EXTENT(width, height);
	}

	SYSCALL(MAExtent, maGetTextSizeW(const wchar* str)) {
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

	SYSCALL(void, maDrawText(int left, int top, const char* str)) {
		CGContextSetRGBFillColor(gDrawTarget->context, currentRed, currentGreen, currentBlue, 1);
		CGContextSetTextDrawingMode(gDrawTarget->context, kCGTextFill);
		CGContextSetTextPosition (gDrawTarget->context, 0, 0);
		CGContextSetAllowsAntialiasing (gDrawTarget->context, true);
		CGContextShowTextAtPoint(gDrawTarget->context, left, top+sFontList[gCurrentFontHandle-1]->size, str, strlen(str));
        CGContextSetAllowsAntialiasing (gDrawTarget->context, false);
	}

	SYSCALL(void, maDrawTextW(int left, int top, const wchar* str)) {
		int numGlyphs = wcharLength(str);
        FontInfo *currentFont=sFontList[gCurrentFontHandle-1];
        initCGFont(currentFont);
		if(numGlyphs==0) return;
		CGGlyph* glyphs = new CGGlyph[numGlyphs];

        //Not all fonts in the device are supported for Unicode glyphs
        //We must check whether the operation was successful
        if(!CMFontGetGlyphsForUnichars(currentFont->cgFontObject, (const UniChar*)str, glyphs, numGlyphs))
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

	SYSCALL(void, maUpdateScreen()) {
		if(gClosing) {
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

	SYSCALL(void, maResetBacklight()) {
		// do nothing, it can't be reset as far as I can tell.. still a private api.
	}

	SYSCALL(MAExtent, maGetScrSize()) {
		return EXTENT(gWidth, gHeight);
	}

	SYSCALL(void, maDrawImage(MAHandle image, int left, int top)) {
		Surface* img = gSyscall->resources.get_RT_IMAGE(image);
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
		int scanlength)) {
		gSyscall->ValidateMemRange(dstPoint, sizeof(MAPoint2d));
		gSyscall->ValidateMemRange(srcRect, sizeof(MARect));
		gSyscall->ValidateMemRange(src, scanlength*srcRect->height*4);

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
		gSyscall->ValidateMemRange(dstTopLeft, sizeof(MAPoint2d));
		gSyscall->ValidateMemRange(src, sizeof(MARect));
		Surface* img = gSyscall->resources.get_RT_IMAGE(image);

		ClipRect srcRect;
		srcRect.x = src->left;
		srcRect.y = src->top;
		srcRect.width = src->width;
		srcRect.height = src->height;
		gDrawTarget->mImageDrawer->drawImageRegion(dstTopLeft->x, dstTopLeft->y, &srcRect, img->mImageDrawer, transformMode);
	}

	SYSCALL(MAExtent, maGetImageSize(MAHandle image)) {
		Surface* img = gSyscall->resources.get_RT_IMAGE(image);
		return EXTENT(img->width, img->height);
	}


	SYSCALL(void, maGetImageData(MAHandle image, void* dst, const MARect* src, int scanlength)) {
		gSyscall->ValidateMemRange(src, sizeof(MARect));
		Surface* img = gSyscall->resources.get_RT_IMAGE(image);
		int x = src->left;
		int y = src->top;
		int width = src->width;
		int height = src->height;
		gSyscall->ValidateMemRange(dst, src->height*scanlength);

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

	SYSCALL(MAHandle, maSetDrawTarget(MAHandle handle)) {
		MAHandle temp = gDrawTargetHandle;
		if(gDrawTargetHandle != HANDLE_SCREEN) {
			SYSCALL_THIS->resources.extract_RT_FLUX(gDrawTargetHandle);
			ROOM(SYSCALL_THIS->resources.add_RT_IMAGE(gDrawTargetHandle, gDrawTarget));
			gDrawTargetHandle = HANDLE_SCREEN;
		}
		if(handle == HANDLE_SCREEN) {
			gDrawTarget = gBackbuffer;
		} else {
			Surface* img = SYSCALL_THIS->resources.extract_RT_IMAGE(handle);
			if(!img->data)
				BIG_PHAT_ERROR(ERR_RES_INVALID_TYPE);

			gDrawTarget = img;
			ROOM(SYSCALL_THIS->resources.add_RT_FLUX(handle, NULL));
		}
		gDrawTargetHandle = handle;
		return temp;
	}

	SYSCALL(int, maCreateImageFromData(MAHandle placeholder, MAHandle data, int offset, int size)) {
		MYASSERT(size>0, ERR_DATA_OOB);
		Stream* stream = gSyscall->resources.get_RT_BINARY(data);
		Surface *bitmap = 0;

		int len;
		TEST(stream->length(len));

		if(offset<0 || offset+size>len)
			BIG_PHAT_ERROR(ERR_DATA_OOB);

		if(!stream->ptrc()) {
			// is not a memstream, read it to a buffer and load it.
			MYASSERT(stream->seek(Seek::Start, offset), ERR_DATA_OOB);
			unsigned char *data = new unsigned char[size];
			if(data==NULL) return RES_OUT_OF_MEMORY;
			MYASSERT(stream->read(data, size), ERR_DATA_ACCESS_FAILED);
			MemStreamC memStream(data, size);
			bitmap = gSyscall->loadImage(memStream);
			delete data;
		} else {
			const char *ptr = (const char*) stream->ptrc();
			MemStreamC memStream((const void*)&ptr[offset], size);
			bitmap = gSyscall->loadImage(memStream);
		}

		if(!bitmap) return RES_BAD_INPUT;
		if(!bitmap->image) {
			delete bitmap;
			// most likely bad input.
			return RES_BAD_INPUT;
		}

		return gSyscall->resources.add_RT_IMAGE(placeholder, bitmap);

	}

	SYSCALL(int, maCreateImageRaw(MAHandle placeholder, const void* src, MAExtent size, int processAlpha)) {
		int width = EXTENT_X(size); int height = EXTENT_Y(size);
		gSyscall->ValidateMemRange(src, width*height*4);
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
		return gSyscall->resources.add_RT_IMAGE(placeholder, bitmap);
	}

	SYSCALL(int, maCreateDrawableImage(MAHandle placeholder, int width, int height)) {
		MYASSERT(width > 0 && height > 0, ERR_IMAGE_SIZE_INVALID);
		Surface *surf = new Surface(width, height);
		if(!surf) return RES_OUT_OF_MEMORY;
		if(!surf->context || !surf->image || !surf->data) {
			delete surf;
			return RES_OUT_OF_MEMORY;
		}

		return gSyscall->resources.add_RT_IMAGE(placeholder, surf);
	}


	SYSCALL(int, maGetKeys())
	{
		if(gClosing)
			return 0;
		return 0; // there's no keys on iphone :)
	}


	SYSCALL(int, maGetEvent(MAEvent *dst))
	{
		gSyscall->ValidateMemRange(dst, sizeof(MAEvent));
		MYASSERT(((uint)dst & 3) == 0, ERR_MEMORY_ALIGNMENT);	//alignment

		if(!gClosing)
			gEventOverflow = false;

		MAEvent ev;
		bool ret = gEventQueue.getAndProcess(ev);
		if(!ret) return 0;
		else *dst = ev; //gEventQueue.get();

#define HANDLE_CUSTOM_EVENT(eventType, dataType) if(ev.type == eventType) {\
		memcpy(MoSync_GetCustomEventData(), (void*)ev.data, sizeof(dataType));\
		delete (dataType*)ev.data;\
		dst->data = (int)MoSync_GetCustomEventDataMoSyncPointer(); }

		CUSTOM_EVENTS(HANDLE_CUSTOM_EVENT);

		return 1;
	}

	SYSCALL(void, maWait(int timeout))
	{
		if(gClosing)
			return;
		gEventQueue.wait(timeout);
	}

	SYSCALL(int, maTime())
	{
		return (int)time(NULL);
	}

	SYSCALL(int, maLocalTime())
	{
		time_t t = time(NULL);
		tm* lt = localtime(&t);
		return t + lt->tm_gmtoff;
	}

	SYSCALL(int, maGetMilliSecondCount())
	{
		//int time = (int)(CACurrentMediaTime()*1000.0);
		//int time = (int)(CFAbsoluteTimeGetCurrent()*1000.0f);
		//int time = (int)((double)mach_absolute_time()*gTimeConversion);
		int time = (((mach_absolute_time() - gTimeStart) * gTimeBase.numer) / gTimeBase.denom) / 1000000;
		return time;
	}

	SYSCALL(int, maFreeObjectMemory()) {
		return getFreeAmountOfMemory();
	}
	SYSCALL(int, maTotalObjectMemory()) {
		return getTotalAmountOfMemory();
	}

	SYSCALL(int, maVibrate(int ms))
	{
		AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
		return 1;
	}

	SYSCALL(void, maPanic(int result, char* message))
	{
		MoSync_ShowMessageBox(nil, message, true);
		gRunning = false;
		pthread_exit(NULL);
        //[[NSThread currentThread] exit];
	}

	SYSCALL(int, maPlatformRequest(const char* url))
	{
		if(!platformRequest(url)) return -1;
		return 0;
	}

	static AVAudioPlayer* sSoundPlayer = NULL;

	SYSCALL(int, maSoundPlay(MAHandle sound_res, int offset, int size))
	{
		Stream *res = gSyscall->resources.get_RT_BINARY(sound_res);
		MYASSERT(res->seek(Seek::Start, offset), ERR_DATA_ACCESS_FAILED);
		Stream* src = res->createLimitedCopy(size);
		MYASSERT(src, ERR_DATA_ACCESS_FAILED);

		byte b;
		do {
			if(!src->readByte(b))
				BIG_PHAT_ERROR(ERR_MIME_READ_FAILED);
		} while(b);

		int pos;
		src->tell(pos);

		NSData *data = NULL;

		int encodedSize = size - pos + offset;
		if(!src->ptrc()) {
			byte* sound = new byte[encodedSize];
			src->read(sound, encodedSize);
			data = [NSData dataWithBytesNoCopy:sound length:encodedSize];

		} else {
			byte* sound = &(((byte*)src->ptrc())[pos]);
			data = [NSData dataWithBytesNoCopy:sound length:encodedSize freeWhenDone:NO];
		}

		if(sSoundPlayer) {
			if(sSoundPlayer.playing==YES)
				[sSoundPlayer stop];
			[sSoundPlayer release];
		}

		NSError *err;
		sSoundPlayer = [[AVAudioPlayer alloc] initWithData:data error:&err];
		[data release];
		if(!sSoundPlayer) {
			// check err
			return -1;
		}
        [sSoundPlayer prepareToPlay];
		[sSoundPlayer play];

		return 0;
	}

	SYSCALL(void, maSoundStop())
	{
		[sSoundPlayer stop];
		sSoundPlayer.currentTime = 0;
	}

	SYSCALL(int, maSoundIsPlaying())
	{
		return sSoundPlayer.playing==YES;
	}

	SYSCALL(void, maSoundSetVolume(int vol))
	{
		sSoundPlayer.volume = (float)vol/100.0f;
	}

	SYSCALL(int, maSoundGetVolume())
	{
		return (int)(sSoundPlayer.volume*100.0f);
	}

	SYSCALL(int, maGetBatteryCharge())
	{
		float batLeft = [[UIDevice currentDevice] batteryLevel];
		return (int)(batLeft*100.0f);
	}

	SYSCALL(int, maSendTextSMS(const char* dst, const char* msg)) {
		if ([MFMessageComposeViewController canSendText] == NO) {
			return CONNERR_UNAVAILABLE;
		}

		MFMessageComposeViewController *smsController = [[MFMessageComposeViewController alloc] init];

		smsController.recipients = [NSArray arrayWithObject:[NSString stringWithCString:dst encoding:NSASCIIStringEncoding]];
		smsController.body = [NSString stringWithCString:msg encoding:NSASCIIStringEncoding];

		smsController.messageComposeDelegate = [[SMSResultDelegate alloc] init];

		MoSyncUI* msUI = getMoSyncUI();
		[msUI showModal:smsController];

		[smsController release];
		return 0;
	}

	SYSCALL(int, maInvokeExtension(int, int, int, int)) {
		BIG_PHAT_ERROR(ERR_FUNCTION_UNIMPLEMENTED);
	}

	SYSCALL(int, maFrameBufferGetInfo(MAFrameBufferInfo *info)) {
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
	SYSCALL(int, maFrameBufferInit(const void *data)) {
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


	SYSCALL(int, maFrameBufferClose()) {
		if(sInternalBackBuffer==NULL) return 0;
		delete gBackbuffer;
		gBackbuffer = sInternalBackBuffer;
		sInternalBackBuffer = NULL;
		gDrawTarget = gBackbuffer;
		return 1;
	}

	int maLocationStart() {
		MoSync_StartUpdatingLocation();
		return MA_LPS_AVAILABLE;
	}

	int maLocationStop() {
		MoSync_StopUpdatingLocation();
		return 0;
	}

	int maTextBox(const wchar* title, const wchar* inText, wchar* outText, int maxSize, int constraints) {
		MoSync_ShowTextBox(title, inText, outText, maxSize, constraints);
		return 0;
	}

	int maGetSystemProperty(const char *key, char *buf, int size) {
		int res = -1;
		if(strcmp(key, "mosync.iso-639-1")==0) {
			// I don't know if this works perfectly (in the documentation it
			// says that it will return iso-639-x, but it looks like iso-639-1)
			CFLocaleRef userLocaleRef = CFLocaleCopyCurrent();
			CFStringRef str = (CFStringRef)CFLocaleGetValue(userLocaleRef, kCFLocaleLanguageCode);
			res = CFStringGetLength(str);
			CFStringGetCString(str, buf, size, kCFStringEncodingUTF8);
			CFRelease(str);
			CFRelease(userLocaleRef);
		} else if (strcmp(key, "mosync.path.local") == 0) {
			NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
			NSString *documentsDirectoryPath = [NSString stringWithFormat:@"%@/",[paths objectAtIndex:0]];
			[documentsDirectoryPath getCString:buf maxLength:size encoding:NSASCIIStringEncoding];
			[documentsDirectoryPath release];
			[paths release];
			res = size;
		} else if (strcmp(key, "mosync.path.local.urlPrefix") == 0) {
			[@"file://localhost/" getCString:buf maxLength:size encoding:NSASCIIStringEncoding];
		}
		return res;
	}

#ifdef SUPPORT_OPENGL_ES


// override implementations for broken bindings..
#undef maIOCtl_glGetPointerv_case
#define maIOCtl_glGetPointerv_case(func) \
case maIOCtl_glGetPointerv: \
{\
GLenum _pname = (GLuint)a; \
void* _pointer = GVMR(b, MAAddress);\
wrap_glGetPointerv(_pname, _pointer); \
return 0; \
}

#undef maIOCtl_glGetVertexAttribPointerv_case
#define maIOCtl_glGetVertexAttribPointerv_case(func) \
case maIOCtl_glGetVertexAttribPointerv: \
{\
GLuint _index = (GLuint)a; \
GLenum _pname = (GLuint)b; \
void* _pointer = GVMR(c, MAAddress);\
wrap_glGetVertexAttribPointerv(_index, _pname, _pointer); \
return 0; \
}

#undef maIOCtl_glShaderSource_case
#define maIOCtl_glShaderSource_case(func) \
case maIOCtl_glShaderSource: \
{ \
GLuint _shader = (GLuint)a; \
GLsizei _count = (GLsizei)b; \
void* _string = GVMR(c, MAAddress); \
const GLint* _length = GVMR(SYSCALL_THIS->GetValidatedStackValue(0 VSV_ARGPTR_USE), GLint); \
wrap_glShaderSource(_shader, _count, _string, _length); \
return 0; \
} \

    void wrap_glShaderSource(GLuint shader, GLsizei count, void* strings, const GLint* length) {

        int* stringsArray = (int*)strings;
        const GLchar** strCopies = new const GLchar*[count];

        for(int i = 0; i < count; i++) {
            void* src = GVMR(stringsArray[i], MAAddress);
            strCopies[i] = (GLchar*)src;
        }

        glShaderSource(shader, count, strCopies, length);
        delete strCopies;
    }


    void wrap_glGetVertexAttribPointerv(GLuint index, GLenum pname, void* pointer) {
        GLvoid* outPointer;
        glGetVertexAttribPointerv(index, pname, &outPointer);

        if(pname != GL_VERTEX_ATTRIB_ARRAY_POINTER)
            return;

        *(int*)pointer = gSyscall->TranslateNativePointerToMoSyncPointer(outPointer);
    }

    void wrap_glGetPointerv(GLenum pname, void* pointer) {
        GLvoid* outPointer;
        glGetPointerv(pname, &outPointer);

        if(pname != GL_COLOR_ARRAY_POINTER &&
           pname != GL_NORMAL_ARRAY_POINTER &&
           pname != GL_POINT_SIZE_ARRAY_POINTER_OES &&
           pname != GL_TEXTURE_COORD_ARRAY_POINTER &&
           pname != GL_VERTEX_ARRAY_POINTER)
            return;

        *(int*)pointer = gSyscall->TranslateNativePointerToMoSyncPointer(outPointer);
    }

	int maOpenGLInitFullscreen(int glApi) {
		if(sOpenGLScreen != -1) return 0;


        if(glApi == MA_GL_API_GL1)
            sOpenGLView = maWidgetCreate("GLView");
        else if(glApi == MA_GL_API_GL2)
            sOpenGLView = maWidgetCreate("GL2View");
        else
            return MA_GL_INIT_RES_UNAVAILABLE_API;

        if(sOpenGLView < 0) {
            return MA_GL_INIT_RES_UNAVAILABLE_API;
        }

        sOpenGLScreen = maWidgetCreate("Screen");
		maWidgetSetProperty(sOpenGLView, "width", "-1");
		maWidgetSetProperty(sOpenGLView, "height", "-1");
		maWidgetAddChild(sOpenGLScreen, sOpenGLView);
		maWidgetScreenShow(sOpenGLScreen);
		maWidgetSetProperty(sOpenGLView, "bind", "");
		return 1;
	}

	int maOpenGLCloseFullscreen() {
		if(sOpenGLScreen == -1) return 0;
		maWidgetRemoveChild(sOpenGLView);
		maWidgetDestroy(sOpenGLView);
		maWidgetDestroy(sOpenGLScreen);
		sOpenGLView = -1;
		sOpenGLScreen = -1;
		return 1;
	}

	int maOpenGLTexImage2D(MAHandle image) {
		Surface* img = gSyscall->resources.get_RT_IMAGE(image);

		int powWidth = nextPowerOf2(1, img->width);
		int powHeight = nextPowerOf2(1, img->height);

		if(powWidth!=img->width || powHeight!=img->height) {

			//surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, surface->format->BitsPerPixel,
			//							   surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask);

			int bytesPerPixel = 4; // for now.

			int oldBytesPerRow = img->rowBytes;
			int newBytesPerRow = powWidth*bytesPerPixel;
			int oldActualBytesPerRow = img->width*bytesPerPixel;

			byte* data = new byte[powHeight*newBytesPerRow];

			byte* src = (byte*)img->data;
			byte* dst = data;
			for(int y = 0; y < img->height; y++) {
				memcpy(dst, src, oldActualBytesPerRow);
				src+=oldBytesPerRow;
				dst+=newBytesPerRow;
			}

			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, powWidth, powHeight, 0,
						 GL_RGBA, GL_UNSIGNED_BYTE, data);

			delete data;

			return MA_GL_TEX_IMAGE_2D_OK;
		}


		// Edit the texture object's image data using the information SDL_Surface gives us
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0,
					 GL_RGBA, GL_UNSIGNED_BYTE, img->data);

		return MA_GL_TEX_IMAGE_2D_OK;
	}

	int maOpenGLTexSubImage2D(MAHandle image) {
		Surface* img = gSyscall->resources.get_RT_IMAGE(image);

		// Edit the texture object's image data using the information SDL_Surface gives us
		glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, img->width, img->height, GL_RGBA
						, GL_UNSIGNED_BYTE, img->data);

		return MA_GL_TEX_IMAGE_2D_OK;
	}

#endif	//SUPPORT_OPENGL_ES

	int maReportResourceInformation() {
		gSyscall->resources.logEverything();
		return 0;
	}

	SYSCALL(void, maMessageBox(const char* title, const char* message))
	{
		MoSync_ShowMessageBox(title, message, false);
	}

	//Shows an alert box with up to three buttons
	SYSCALL(void, maAlert(const char* title, const char* message, const char* button1, const char* button2, const char* button3))
	{
		MoSync_ShowAlert(title, message, button1, button2, button3);
	}

	SYSCALL(void, maOptionsBox(const wchar* title, const wchar* destructiveButtonTitle, const wchar* cancelButtonTitle,
                          const void* otherButtonTitles, const int otherButtonTitlesSize))
	{
        [[OptionsDialogView getInstance] show:title
                       destructiveButtonTitle:destructiveButtonTitle
                            cancelButtonTitle:cancelButtonTitle
                            otherButtonTitles:otherButtonTitles
                        otherButtonTitlesSize:otherButtonTitlesSize];
	}

    SYSCALL(void, maImagePickerOpen())
	{
		MoSync_ShowImagePicker();
	}

	//This struct holds information about what resources are connected
	//to a single camera. Each device camera has it's own instance
	//(So, one for most phones, and two for iPhone 4, for example)
	struct CameraInfo {
		AVCaptureSession *captureSession;
		AVCaptureVideoPreviewLayer *previewLayer;
		AVCaptureDevice *device; //The physical camera device
        AVCaptureStillImageOutput *stillImageOutput;
		UIView *view;
	};

	//There is only a single instance of this struct, and it holds info about the
	//devices on the system, as well as which one is the selected one for the camera
	//syscalls
	struct CameraSystemInfo {
		int numCameras;
		int currentCamera;
		BOOL initialized;
		CameraInfo *cameraInfo;
	};

	CameraSystemInfo gCameraSystem={0,0,FALSE,NULL};

	CameraConfirgurator *gCameraConfigurator;

	//This performs lazy initialization of the camera system, the first time
	//a relevant camera syscall is called.
	void initCameraSystem()
	{

		if( gCameraSystem.initialized == FALSE )
		{
			gCameraConfigurator = [[CameraConfirgurator alloc] init];
			CameraInfo *cameraInfo;
			int numCameras = 0;

			//This will also include microphones and maybe other, non camera devices
			NSArray *devices = [AVCaptureDevice devices];
			AVCaptureDevice *backCamera = NULL;
			AVCaptureDevice *frontCamera = NULL;

			for ( AVCaptureDevice *device in devices)
			{
				//This weeds out the devices we don't need
				if ( [device hasMediaType:AVMediaTypeVideo] )
				{
					numCameras++;
					//The following code assumes that all cameras not facing back,
					//will be facing forward. This works for the current phones,
					//but it could probably fail if Apple ever introduces a device
					//with three or more cameras
					if ( [device position] == AVCaptureDevicePositionBack )
					{
						backCamera = device;
					}
					else
					{
						frontCamera = device;
					}
				}
			}

			if( numCameras > 0 )
			{
				int positionCounter = 0;
				cameraInfo = new CameraInfo[numCameras];

				//Back facing camera should be first, then front facing, then the rest
				if ( backCamera != NULL )
				{
					cameraInfo[positionCounter].device = backCamera;
					positionCounter++;
				}
				if ( frontCamera != NULL )
				{
					cameraInfo[positionCounter].device = frontCamera;
					positionCounter++;
				}

				for ( AVCaptureDevice *device in devices )
				{
					if ( [device hasMediaType:AVMediaTypeVideo ] &&
						device != backCamera && device != frontCamera)
					{
						cameraInfo[positionCounter].device = device;
						positionCounter++;
					}
				}

				for (int i=0; i<numCameras; i++) {
					cameraInfo[i].captureSession = NULL;
					cameraInfo[i].previewLayer = NULL;
					cameraInfo[i].view = NULL;
				}
			}

			gCameraSystem.numCameras = numCameras;
			gCameraSystem.cameraInfo = cameraInfo;
			gCameraSystem.initialized = TRUE;
		}
	}

	//This function not only returns information about the currently selected amera, but
	//also performs lazy initialization on the session object
	CameraInfo *getCurrentCameraInfo()
	{
		initCameraSystem();

		if( gCameraSystem.numCameras == 0 )
		{
			return NULL;
		}

		CameraInfo *curCam = &gCameraSystem.cameraInfo[ gCameraSystem.currentCamera ];
		if( curCam->captureSession == NULL ) {

			curCam->captureSession = [[AVCaptureSession alloc] init];


			NSError *error = nil;
			AVCaptureDeviceInput *input =
			[AVCaptureDeviceInput deviceInputWithDevice:curCam->device error:&error];
            curCam->stillImageOutput = [[AVCaptureStillImageOutput alloc] init];
            NSDictionary *outputSettings = [[NSDictionary alloc] initWithObjectsAndKeys:
                                            AVVideoCodecJPEG, AVVideoCodecKey, nil];
            [curCam->stillImageOutput setOutputSettings:outputSettings];
			[outputSettings release];
			if ([curCam->captureSession canSetSessionPreset:AVCaptureSessionPresetMedium]) {
				curCam->captureSession.sessionPreset = AVCaptureSessionPresetMedium;
			}
			[curCam->captureSession addInput:input];
			[curCam->captureSession addOutput:curCam->stillImageOutput];
		}
		return curCam;
	}

	void StopAllCameraSessions()
	{
		if( gCameraSystem.initialized == TRUE )
		{
			for ( int i = 0; i < gCameraSystem.numCameras; i++ )
			{
				if( gCameraSystem.cameraInfo[i].captureSession )
				{
					[gCameraSystem.cameraInfo[i].captureSession stopRunning];
				}
			}
		}
	}

	SYSCALL(int, maCameraStart())
	{
		@try {
				CameraInfo *info = getCurrentCameraInfo();
				if( info )
				{
					//In this case, no preview widget was assigned to this camera.
					//Run the sublayer to the main mosync view at full screen
					if( !info->view )
					{
						if( !info->previewLayer )
						{
							info->previewLayer = [[AVCaptureVideoPreviewLayer alloc] initWithSession:info->captureSession];
						}

						MoSync_AddLayerToView(info->previewLayer);
						MoSync_UpdateView(gBackbuffer->image);
					}
					else
					{
						info->previewLayer.frame = info->view.bounds;
					}

					//Have to do it this way, because otherwise it hijacks the main thread or something wierd
					[info->captureSession	performSelectorOnMainThread:@selector(startRunning)
														   withObject:nil
														waitUntilDone:YES];

					if(info->device.torchMode == AVCaptureTorchModeOn)
					{
						[info->device lockForConfiguration:nil];
						info->device.torchMode = AVCaptureTorchModeOff;
						info->device.torchMode = AVCaptureTorchModeOn;
						[info->device unlockForConfiguration];
				}
				}
				return 1;
		}
		@catch (NSException * e) {
			return -1;
		}
	}

	SYSCALL(int, maCameraStop())
	{
		@try {
			CameraInfo *info = getCurrentCameraInfo();
			if( info )
			{
				[info->captureSession stopRunning];

				//In this case, we don't have a preview widget,
				//so we need to remove the layer from the main view.
				if ( !info->view ) {
					[info->previewLayer removeFromSuperlayer];
				}
			}
			return 1;
		}
		@catch (NSException * e) {
			return -1;
		}
	}

	SYSCALL(int, maCameraSetPreview(MAHandle widgetHandle))
	{
		@try {
			CameraPreviewWidget *widget = (CameraPreviewWidget*) [getMoSyncUI() getWidget:widgetHandle];
			if( !widget )
			{
				return 0;
			}

			UIView *newView = [widget getView];

			CameraInfo *info = getCurrentCameraInfo();

			if( !info->previewLayer )
			{
				info->previewLayer = [[AVCaptureVideoPreviewLayer alloc] initWithSession:info->captureSession];
				//info->previewLayer.videoGravity = AVLayerVideoGravityResizeAspectFill;
			}

			if ( info->view )
			{
				//Remove the preview layer from the other view
				[info->previewLayer removeFromSuperlayer];
			}

			//If this widget was assigned to another camera, we need to remove that
			//other camera's preview layer from it.
			for ( int i=0; i<gCameraSystem.numCameras; i++ ) {
				if( gCameraSystem.cameraInfo[i].view == newView )
				{
					[gCameraSystem.cameraInfo[i].previewLayer removeFromSuperlayer];
					gCameraSystem.cameraInfo[i].view = NULL;
				}
			}

			info->view = newView;
			widget.previewLayer = info->previewLayer;
			[info->view.layer addSublayer:info->previewLayer];
			info->previewLayer.frame = info->view.bounds;
			[info->view.layer needsLayout];

			//We need to "reset" the view in case we switched sublayers (because we switched cameras)
			//First we hide the view
			info->view.hidden = YES;

			//Then we tell the main thread (that's outside the user code loop) to show it again.
			//We can't do it on the same "calling circle" because the UI system won't know that we
			//tried to show and hide the view
			[widget performSelectorOnMainThread:@selector(showViewAgain) withObject:nil waitUntilDone:NO];
			return 1;
		}
		@catch (NSException * e) {
			return -1;
		}
	}

	SYSCALL(int, maCameraSelect(MAHandle cameraNumber))
	{
		@try {
			initCameraSystem();

			if (cameraNumber < 0 || cameraNumber >=gCameraSystem.numCameras) {
				return 0;
			}

			gCameraSystem.currentCamera = cameraNumber;
			return 1;
		}
		@catch (NSException * e) {
			return -1;
		}
	}

	SYSCALL(int, maCameraNumber())
	{
		@try {
			initCameraSystem();
			return gCameraSystem.numCameras;
		}
		@catch (NSException * e) {
			return -1;
		}
	}

	SYSCALL(int, maCameraSnapshot(int formatIndex, MAHandle placeholder))
	{
		@try {
			CameraInfo *info = getCurrentCameraInfo();

			AVCaptureConnection *videoConnection =	[info->stillImageOutput.connections objectAtIndex:0];
			if ([videoConnection isVideoOrientationSupported])
			{
				[videoConnection setVideoOrientation:UIDeviceOrientationPortrait];//[UIDevice currentDevice].orientation];
				if([UIDevice currentDevice].orientation == UIDeviceOrientationPortrait)
					NSLog(@"video orientation is set to Portrait");
			}
			[info->stillImageOutput captureStillImageAsynchronouslyFromConnection:videoConnection
											completionHandler:^(CMSampleBufferRef imageDataSampleBuffer, NSError *error) {
											if (imageDataSampleBuffer != NULL) {
												NSData *imageData = [AVCaptureStillImageOutput
												jpegStillImageNSDataRepresentation:imageDataSampleBuffer];
													MemStream *stream = new MemStream([imageData length]);
													stream->write([imageData bytes],[imageData length]);
													gSyscall->resources.add_RT_BINARY(placeholder, stream);
											}}];
			return 1;

		}
		@catch (NSException * e) {
			return -1;
		}
	}

	SYSCALL(int, maCameraRecord(int stopStartFlag))
	{
		return -1;
	}

	SYSCALL(int, maCameraSetProperty(const char *property, const char* value))
	{
		@try {
			int result = 0;
			CameraInfo *info = getCurrentCameraInfo();
			NSString *propertyString = [NSString stringWithUTF8String:property];
			NSString *valueString = [NSString stringWithUTF8String:value];
			result = [gCameraConfigurator	setCameraProperty: info->device
										withProperty: propertyString
										   withValue: valueString];
			[propertyString release];
			[valueString release];
			return result;
		}
		@catch (NSException * e) {
			return -1;
		}
	}

	SYSCALL(int, maCameraGetProperty(const char *property, char *value, int maxSize))
	{
		@try {
			NSString *propertyString = [NSString stringWithUTF8String:property];
			CameraConfirgurator *configurator = [[CameraConfirgurator alloc] init];
			CameraInfo *info = getCurrentCameraInfo();
			NSString* retval = [configurator	getCameraProperty:info->device
												  withProperty:propertyString];

			if(retval == nil) return -2;
			int length = maxSize;
			int realLength = [retval length];
			if(realLength > length) {
				return -2;
			}

			[retval getCString:value maxLength:length encoding:NSASCIIStringEncoding];
			[retval release];
			[propertyString release];
			[configurator release];

			return realLength;
		}
		@catch (NSException * e) {
			return -1;
		}
	}




    SYSCALL(int, maSensorStart(int sensor, int interval))
	{
		return MoSync_SensorStart(sensor, interval);
	}

    SYSCALL(int, maSensorStop(int sensor))
	{
		return MoSync_SensorStop(sensor);
	}

    SYSCALL(int, maSyscallPanicsEnable())
	{
        [[MoSyncPanic getInstance] setThowPanic:true];
        return RES_OK;
	}

    SYSCALL(int, maSyscallPanicsDisable())
	{
        [[MoSyncPanic getInstance] setThowPanic:false];
        return RES_OK;
	}

	SYSCALL(longlong, maIOCtl(int function, int a, int b, int c))
	{
		switch(function) {

		case maIOCtl_maWriteLog:
		{
			const char *str = (const char*) gSyscall->GetValidatedMemRange(a, b);
			LOGBIN(str, b);
			return 0;
		}
		maIOCtl_case(maPlatformRequest);
		maIOCtl_case(maGetBatteryCharge);
		maIOCtl_case(maLocationStart);
		maIOCtl_case(maLocationStop);
		maIOCtl_case(maFrameBufferGetInfo);
		maIOCtl_case(maFrameBufferInit);
		maIOCtl_case(maFrameBufferClose);
        maIOCtl_syscall_case(maPimListOpen);
        maIOCtl_syscall_case(maPimListNext);
        maIOCtl_syscall_case(maPimItemCount);
        maIOCtl_syscall_case(maPimItemGetValue);
        maIOCtl_syscall_case(maPimListClose);
        maIOCtl_syscall_case(maPimItemGetField);
        maIOCtl_syscall_case(maPimItemFieldCount);
        maIOCtl_syscall_case(maPimItemGetAttributes);
        maIOCtl_syscall_case(maPimItemSetLabel);
        maIOCtl_syscall_case(maPimItemGetLabel);
        maIOCtl_syscall_case(maPimFieldType);
        maIOCtl_syscall_case(maPimItemSetValue);
        maIOCtl_syscall_case(maPimItemAddValue);
        maIOCtl_syscall_case(maPimItemRemoveValue);
        maIOCtl_syscall_case(maPimItemClose);
        maIOCtl_syscall_case(maPimItemCreate);
        maIOCtl_syscall_case(maPimItemRemove);
		maIOCtl_syscall_case(maFileOpen);
		maIOCtl_syscall_case(maFileWriteFromData);
		maIOCtl_syscall_case(maFileReadToData);
		maIOCtl_syscall_case(maFileTell);
		maIOCtl_syscall_case(maFileSeek);
		maIOCtl_syscall_case(maFileRead);
		maIOCtl_syscall_case(maFileWrite);
		maIOCtl_syscall_case(maFileExists);
		maIOCtl_syscall_case(maFileClose);
		maIOCtl_syscall_case(maFileCreate);
		maIOCtl_syscall_case(maFileDelete);
		maIOCtl_syscall_case(maFileSize);
        maIOCtl_case(maFontLoadDefault);
        maIOCtl_case(maFontLoadWithName);
        maIOCtl_case(maFontDelete);
        maIOCtl_case(maFontSetCurrent);
        maIOCtl_case(maFontGetCount);
        maIOCtl_case(maFontGetName);
        maIOCtl_syscall_case(maFileAvailableSpace);
        maIOCtl_syscall_case(maFileTotalSpace);
        maIOCtl_syscall_case(maFileDate);
        maIOCtl_syscall_case(maFileRename);
        maIOCtl_syscall_case(maFileTruncate);
        maIOCtl_syscall_case(maFileListStart);
        maIOCtl_syscall_case(maFileListNext);
        maIOCtl_syscall_case(maFileListClose);
		maIOCtl_case(maTextBox);
		maIOCtl_case(maGetSystemProperty);
		maIOCtl_case(maReportResourceInformation);
		maIOCtl_case(maMessageBox);
		maIOCtl_case(maAlert);
        maIOCtl_case(maOptionsBox);
		maIOCtl_case(maCameraStart);
		maIOCtl_case(maCameraStop);
		maIOCtl_case(maCameraSetPreview);
		maIOCtl_case(maCameraSelect);
		maIOCtl_case(maCameraNumber);
		maIOCtl_case(maCameraSnapshot);
		maIOCtl_case(maCameraRecord);
		maIOCtl_case(maCameraSetProperty);
		maIOCtl_case(maCameraGetProperty);
        maIOCtl_case(maSensorStart);
        maIOCtl_case(maSensorStop);
		maIOCtl_case(maImagePickerOpen);
		maIOCtl_case(maSendTextSMS);
		maIOCtl_case(maSyscallPanicsEnable);
		maIOCtl_case(maSyscallPanicsDisable);
		maIOCtl_case(maDBOpen);
		maIOCtl_case(maDBClose);
		maIOCtl_case(maDBExecSQL);
		maIOCtl_case(maDBCursorDestroy);
		maIOCtl_case(maDBCursorNext);
		maIOCtl_case(maDBCursorGetColumnData);
		maIOCtl_case(maDBCursorGetColumnText);
		maIOCtl_case(maDBCursorGetColumnInt);
		maIOCtl_case(maDBCursorGetColumnDouble);
		maIOCtl_IX_WIDGET_caselist
#ifdef SUPPORT_OPENGL_ES
		maIOCtl_IX_OPENGL_ES_caselist;
        maIOCtl_IX_GL1_caselist;
        maIOCtl_IX_GL2_caselist;
        maIOCtl_IX_GL_OES_FRAMEBUFFER_OBJECT_caselist;
#endif	//SUPPORT_OPENGL_ES
        maIOCtl_IX_AUDIO_caselist;
		}

		return IOCTL_UNAVAILABLE;
	}

	SYSCALL(void, maLoadProgram(MAHandle data, int reload)) {
		MoSync_ReloadProgram(data, reload);
	}

} // namespace Base

void EventQueue::handleInternalEvent(int type, void *e) {
	if(type == IEVENT_TYPE_DEFLUX_BINARY) {
		InternalEventDefluxBin *d = (InternalEventDefluxBin*)e;
		SYSCALL_THIS->resources.extract_RT_FLUX(d->handle);
		ROOM(SYSCALL_THIS->resources.dadd_RT_BINARY(d->handle, d->stream));
		delete d;
	}
}

void MoSyncExit(int r)
{
	LOG("MoSyncExit(%d)\n", r);

	EnterCriticalSection(&exitMutex);
	if(!exited) {
		exited = true;
		LeaveCriticalSection(&exitMutex);
		MoSync_Exit();
		EnterCriticalSection(&exitMutex);
	}
	LeaveCriticalSection(&exitMutex);
	pthread_exit(NULL);
}

void MoSyncErrorExit(int errorCode)
{
	LOG("ErrorExit %i\n", errorCode);
	char buffer[256];
	//char* ptr = buffer +
	sprintf(buffer, "MoSync Panic\np%i.", errorCode);
#if 0
	if(gCore) {
#ifdef PUBLIC_DEBUG
		if(SYSCALL_NUMBER_IS_VALID(gCore->currentSyscallId)) {
			ptr += sprintf(ptr, "s%i.", gCore->currentSyscallId);
		}
		if(gRunning) {
			ptr += sprintf(ptr, "i%x.", Core::GetIp(gCore));
		}
#endif
		if(gRunning) {
			char appCode[5];
			for(int i=0; i<4; i++) {
				appCode[i] = (byte)(gCore->Head.AppCode >> ((3-i)*8));
			}
			appCode[4] = 0;
			sprintf(ptr, "%s", appCode);
		}
	}
#endif

	gRunning = false;
	logWithNSLog(buffer, strlen(buffer));
	MoSync_ShowMessageBox(nil, buffer, true);
	pthread_exit(NULL);
}
