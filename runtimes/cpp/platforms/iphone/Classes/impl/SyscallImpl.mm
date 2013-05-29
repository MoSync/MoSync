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
#include "MoSyncDB.h"
#include "MoSyncUISyscalls.h"
#include "MoSyncMain.h"
#include "MoSyncExtension.h"
#include "AudioSyscall.h"
#include "ThreadPool.h"
#include "MoSyncUIUtils.h"

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
#import "MoSyncFonts.h"
#import "MoSyncLocation.h"
#import "PimSyscall.h"
#import "MoSyncUIAlertView.h"

#define NETWORKING_H
#include "networking.h"

#ifdef SUPPORT_OPENGL_ES
#define DONT_WANT_IX_OPENGL_ES_TYPEDEFS
#include <helpers/CPP_IX_OPENGL_ES_MA.h>
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

extern ThreadPool gThreadPool;

#define NOT_IMPLEMENTED BIG_PHAT_ERROR(ERR_FUNCTION_UNIMPLEMENTED)

namespace Base {

	//***************************************************************************
	//Defines and declarations
	//***************************************************************************
	static void MAUpdateScreen();

	Syscall* gSyscall;

	EventQueue gEventQueue;
	bool gEventOverflow = false;

	int gVolume = -1;

	bool exited = false;

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

        initFonts();

        //Setting the time base and the time start of the app.
        initTimeStamps();

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
		[MoSyncUIAlertView deleteInstance];
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
	SYSCALL(longlong, maIOCtl(int function, int a, int b, int c MA_IOCTL_ELLIPSIS))
	{
        va_list argptr;
        va_start(argptr, c);

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
        maIOCtl_case(maPimListOpen);
        maIOCtl_case(maPimListNextSummary);
        maIOCtl_case(maPimListNext);
        maIOCtl_case(maPimItemCount);
        maIOCtl_case(maPimItemGetValue);
        maIOCtl_case(maPimListClose);
        maIOCtl_case(maPimItemGetField);
        maIOCtl_case(maPimItemFieldCount);
        maIOCtl_case(maPimItemGetAttributes);
        maIOCtl_case(maPimItemSetLabel);
        maIOCtl_case(maPimItemGetLabel);
        maIOCtl_case(maPimFieldType);
        maIOCtl_case(maPimItemSetValue);
        maIOCtl_case(maPimItemAddValue);
        maIOCtl_case(maPimItemRemoveValue);
        maIOCtl_case(maPimItemClose);
        maIOCtl_case(maPimItemCreate);
        maIOCtl_case(maPimItemRemove);
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
        maIOCtl_case(maCameraSnapshotAsync);
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
        maIOCtl_case(maSaveImageToDeviceGallery);
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
		maIOCtl_case(maDBExecSQLParams);
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
        maIOCtl_IX_OPENGL_ES_MA_caselist
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
        va_end(argptr);
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
	MoSyncUIUtils_ShowAlert(NULL, buffer, "OK", NULL, NULL);
	pthread_exit(NULL);
}
