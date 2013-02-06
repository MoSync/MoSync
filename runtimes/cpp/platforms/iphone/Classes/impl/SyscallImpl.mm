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
#include <CoreMedia/CoreMedia.h>
#include <sys/types.h> //

#include <helpers/CPP_IX_GUIDO.h>
//#include <helpers/CPP_IX_ACCELEROMETER.h>
#include <helpers/CPP_IX_WIDGET.h>
#include "MoSyncUISyscalls.h"

#import "ImagePickerController.h"
#include "netImpl.h"
#import "PurchaseManager.h"

#define NETWORKING_H
#include "networking.h"
#include <bluetooth/discovery.h>
#include <base_errors.h>
using namespace MoSyncError;

#include <core/core.h>

#include "MoSyncMain.h"
#import "MoSyncAppDelegate.h"

#include "CMGlyphDrawing.h"

#import <CoreVideo/CoreVideo.h>
#import <CoreVideo/CVPixelBuffer.h>

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
#include "GLFixes.h"
#endif

#include "AudioSyscall.h"

#include "MoSyncExtension.h"

#import "MoSyncCamera.h"
#import "MoSyncSound.h"
#import "MoSyncPurchase.h"
#import "MoSyncCapture.h"
#import "MoSyncNotification.h"
#import "MoSyncOrientation.h"
#import "MoSyncSensorBridge.h"
#import "MoSyncMisc.h"
#import "MoSyncAds.h"
#import "MoSyncOpenGL.h"
#import "MoSyncGraphics.h"

extern ThreadPool gThreadPool;

#define NOT_IMPLEMENTED BIG_PHAT_ERROR(ERR_FUNCTION_UNIMPLEMENTED)

namespace Base {

	//***************************************************************************
	//Defines and declarations
	//***************************************************************************
	static void MAUpdateScreen();

	Syscall* gSyscall;

	EventQueue gEventQueue;
	static bool gEventOverflow	= false;

	int gVolume = -1;

	bool exited = false;

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

		gSyscall->createBackbuffer();

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

        //Setting the initially selected font. "gHeight/40" was used originally, is kept for backwards compatibility
        MAHandle initFontHandle=maFontLoadDefault(INITIAL_FONT_TYPE,INITIAL_FONT_STYLE,gHeight/40);
        maFontSetCurrent(initFontHandle);

        //Setting the time base and the time start of the app.
        initTimeStamps();

		MANetworkInit();

		MAPimInit();
        MAAudioInit();

		// init some image.h optimizations.
		initMulTable();
		initRecipLut();

        initExtensions(NULL);

		return true;
	}

	void MALibQuit() {
		DeleteCriticalSection(&exitMutex);
		MANetworkClose();
        MAPimClose();
        MAAudioClose();
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

	void Syscall::deviceOrientationChanged()
	{
		if (isNativeUIEnabled())
		{
			return;
		}

		gSyscall->createBackbuffer();
		maUpdateScreen();
	}

	void Syscall::createBackbuffer()
	{
		MoSyncGraphicsCreateBackbuffer();
	}

	void Syscall::platformDestruct() {
		MALibQuit();
	}

	//***************************************************************************
	// Proper syscalls
	//***************************************************************************


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
            while((familyName=[familyEnumerator nextObject]))
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

	SYSCALL(MAExtent, maGetScrSize()) {
		CGSize size = [[ScreenOrientation getInstance] screenSize];
		int width = (int) size.width;
		int height = (int)size.height;
		return EXTENT(width, height);
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

	SYSCALL(void, maPanic(int result, char* message))
	{
		MoSync_ShowMessageBox(nil, message, true);
		gRunning = false;
		pthread_exit(NULL);
        //[[NSThread currentThread] exit];
	}

	SYSCALL(MAExtensionModule, maExtensionModuleLoad(const char* name, int hash))
	{
		return MA_EXTENSION_MODULE_UNAVAILABLE;
	}

	SYSCALL(MAExtensionFunction, maExtensionFunctionLoad(MAHandle module, int index))
	{
		return MA_EXTENSION_FUNCTION_UNAVAILABLE;
	}

	SYSCALL(int, maExtensionFunctionInvoke(int, int, int, int)) {
		BIG_PHAT_ERROR(ERR_FUNCTION_UNIMPLEMENTED);
	}

	int maReportResourceInformation() {
		gSyscall->resources.logEverything();
		return 0;
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
        maIOCtl_syscall_case(maPimListNextSummary);
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
        maIOCtl_case(maFileSetProperty);
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
        maIOCtl_case(maCameraPreviewSize);
        maIOCtl_case(maCameraPreviewEventEnable);
        maIOCtl_case(maCameraPreviewEventDisable);
        maIOCtl_case(maCameraPreviewEventConsumed);
		maIOCtl_case(maWakeLock);
        maIOCtl_case(maSensorStart);
        maIOCtl_case(maSensorStop);
		maIOCtl_case(maImagePickerOpen);
        maIOCtl_case(maImagePickerOpenWithEventReturnType);
		maIOCtl_case(maSendTextSMS);
		maIOCtl_case(maSyscallPanicsEnable);
		maIOCtl_case(maSyscallPanicsDisable);
        maIOCtl_case(maAdsBannerCreate);
        maIOCtl_case(maAdsAddBannerToLayout);
        maIOCtl_case(maAdsRemoveBannerFromLayout);
        maIOCtl_case(maAdsBannerDestroy);
        maIOCtl_case(maAdsBannerSetProperty);
        maIOCtl_case(maAdsBannerGetProperty);
        maIOCtl_case(maNotificationLocalCreate);
        maIOCtl_case(maNotificationLocalDestroy);
        maIOCtl_case(maNotificationLocalSetProperty);
        maIOCtl_case(maNotificationLocalGetProperty);
        maIOCtl_case(maNotificationLocalSchedule);
        maIOCtl_case(maNotificationLocalUnschedule);
        maIOCtl_case(maNotificationPushRegister);
        maIOCtl_case(maNotificationPushUnregister);
        maIOCtl_case(maNotificationPushGetData);
        maIOCtl_case(maNotificationPushGetRegistration);
        maIOCtl_case(maNotificationPushDestroy);
        maIOCtl_case(maNotificationSetIconBadge);
        maIOCtl_case(maNotificationGetIconBadge);
		maIOCtl_case(maDBOpen);
		maIOCtl_case(maDBClose);
		maIOCtl_case(maDBExecSQL);
		maIOCtl_case(maDBCursorDestroy);
		maIOCtl_case(maDBCursorNext);
		maIOCtl_case(maDBCursorGetColumnData);
		maIOCtl_case(maDBCursorGetColumnText);
		maIOCtl_case(maDBCursorGetColumnInt);
		maIOCtl_case(maDBCursorGetColumnDouble);
		maIOCtl_case(maScreenSetSupportedOrientations);
		maIOCtl_case(maScreenGetSupportedOrientations);
		maIOCtl_case(maScreenGetCurrentOrientation);
		maIOCtl_case(maCaptureSetProperty);
		maIOCtl_case(maCaptureGetProperty);
		maIOCtl_case(maCaptureAction);
		maIOCtl_case(maCaptureWriteImage);
		maIOCtl_case(maCaptureGetVideoPath);
		maIOCtl_case(maCaptureDestroyData);

		maIOCtl_IX_WIDGET_caselist
#ifdef SUPPORT_OPENGL_ES
		maIOCtl_IX_OPENGL_ES_caselist;
        maIOCtl_IX_GL1_caselist;
        maIOCtl_IX_GL2_caselist;
        maIOCtl_IX_GL_OES_FRAMEBUFFER_OBJECT_caselist;
#endif	//SUPPORT_OPENGL_ES
        //maIOCtl_IX_AUDIO_caselist;
		maIOCtl_case(maAudioDataCreateFromResource);
		maIOCtl_case(maAudioDataCreateFromURL);
		maIOCtl_case(maAudioDataDestroy);
		maIOCtl_case(maAudioInstanceCreate);
		maIOCtl_case(maAudioInstanceDestroy);
		maIOCtl_case(maAudioGetLength);
		maIOCtl_case(maAudioSetNumberOfLoops);
		maIOCtl_case(maAudioPrepare);
		maIOCtl_case(maAudioPlay);
		maIOCtl_case(maAudioSetPosition);
		maIOCtl_case(maAudioGetPosition);
		maIOCtl_case(maAudioSetVolume);
		maIOCtl_case(maAudioStop);
		maIOCtl_case(maAudioPause);
		maIOCtl_case(maAudioInstanceCreateDynamic);
		maIOCtl_case(maAudioGetPendingBufferCount);
		maIOCtl_case(maAudioSubmitBuffer);
		maIOCtl_case(maExtensionModuleLoad);
        maIOCtl_case(maExtensionFunctionLoad);
        maIOCtl_case(maPurchaseSupported);
        maIOCtl_case(maPurchaseCreate);
        maIOCtl_case(maPurchaseDestroy);
        maIOCtl_case(maPurchaseRequest);
        maIOCtl_case(maPurchaseGetName);
        maIOCtl_case(maPurchaseSetStoreURL);
        maIOCtl_case(maPurchaseVerifyReceipt);
        maIOCtl_case(maPurchaseGetField);
        maIOCtl_case(maPurchaseRestoreTransactions);
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
