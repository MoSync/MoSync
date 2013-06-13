/*
 Copyright (C) 2013 MoSync AB

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

#import "MoSyncMisc.h"

#include "Platform.h"
#include "Syscall.h"
#include "MoSyncUI.h"
#include "OptionsDialogView.h"
#include "MoSyncPanic.h"
#include "MoSyncUIUtils.h"

#include "MoSyncMain.h"
#include "MoSyncUISyscalls.h"
#include "iphone_helpers.h"

#include <sys/sysctl.h>//to retrieve device model
#include <sys/xattr.h>
#include <AudioToolbox/AudioToolbox.h>

#import <MessageUI/MessageUI.h>

#import "Reachability.h"
#import "ScreenOrientation.h"

static mach_timebase_info_data_t gTimeBase;
static uint64_t gTimeStart;

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

/*
 * Time related
 */
void initTimeStamps()
{
    mach_timebase_info( &gTimeBase );
    //gTimeConversion = 1e-6 * (double)machInfo.numer/(double)machInfo.denom;
    gTimeStart = mach_absolute_time();
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

SYSCALL(void, maWait(int timeout))
{
    if(Base::gClosing)
        return;
    Base::gEventQueue.wait(timeout);
}

/*
 * Platform related
 */
SYSCALL(int, maWakeLock(int flag))
{
    if (MA_WAKE_LOCK_ON == flag)
    {
        [UIApplication sharedApplication].idleTimerDisabled = YES;
    }
    else
    {
        [UIApplication sharedApplication].idleTimerDisabled = NO;
    }
    return RES_OK;
}

SYSCALL(void, maResetBacklight())
{
    // do nothing, it can't be reset as far as I can tell.. still a private api.
}

SYSCALL(int, maVibrate(int ms))
{
    AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
    return 1;
}

SYSCALL(int, maGetBatteryCharge())
{
    float batLeft = [[UIDevice currentDevice] batteryLevel];
    return (int)(batLeft*100.0f);
}

SYSCALL(int, maPlatformRequest(const char* url))
{
    if(!platformRequest(url)) return -1;
    return 0;
}

SYSCALL(int, maGetKeys())
{
    if(Base::gClosing)
        return 0;
    return 0; // there's no keys on iphone :)
}

int maGetSystemProperty(const char *key, char *buf, int size)
{
    int res = -2; //Property not found
    if(strcmp(key, "mosync.iso-639-1")==0) {
        // I don't know if this works perfectly (in the documentation it
        // says that it will return iso-639-x, but it looks like iso-639-1)
        CFLocaleRef userLocaleRef = CFLocaleCopyCurrent();
        CFStringRef str = (CFStringRef)CFLocaleGetValue(userLocaleRef, kCFLocaleLanguageCode);
        bool success = CFStringGetCString(str, buf, size, kCFStringEncodingUTF8);
        res = (success)?strlen(buf) + 1: -1;
        CFRelease(str);
        CFRelease(userLocaleRef);
    } else if (strcmp(key, "mosync.path.local") == 0) {
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentsDirectoryPath = [NSString stringWithFormat:@"%@/",[paths objectAtIndex:0]];
        BOOL success = [documentsDirectoryPath getCString:buf maxLength:size encoding:NSUTF8StringEncoding];
        res = (success)?strlen(buf) + 1: -1;
    } else if (strcmp(key, "mosync.path.local.urlPrefix") == 0) {
        BOOL success = [@"file://localhost/" getCString:buf maxLength:size encoding:NSUTF8StringEncoding];
        res = (success)?strlen(buf) + 1: -1;
    } else if (strcmp(key, "mosync.device.name") == 0) {
        BOOL success = [[[UIDevice currentDevice] name] getCString:buf maxLength:size encoding:NSUTF8StringEncoding];
        res = (success)?strlen(buf) + 1: -1;
    } else if (strcmp(key, "mosync.device.OS")== 0) {
        BOOL success = [[[UIDevice currentDevice] systemName] getCString:buf maxLength:size encoding:NSUTF8StringEncoding];
        res = (success)?strlen(buf) + 1: -1;
    } else if (strcmp(key, "mosync.device.OS.version") == 0) {
        BOOL success = [[[UIDevice currentDevice] systemVersion] getCString:buf maxLength:size encoding:NSUTF8StringEncoding];
        res = (success)?strlen(buf) + 1: -1;
    } else if (strcmp(key, "mosync.device") == 0) {
        size_t responseSz;
        sysctlbyname("hw.machine", NULL, &responseSz, NULL, 0);
        char *machine = (char*)malloc(responseSz);
        sysctlbyname("hw.machine", machine, &responseSz, NULL, 0);
        NSString *platform = [NSString stringWithCString:machine encoding:NSUTF8StringEncoding];
        BOOL success = [platform getCString:buf maxLength:size encoding:NSUTF8StringEncoding];
        free(machine);
        res = (success)?strlen(buf) + 1: -1;
    } else if (strcmp(key, "mosync.network.type") == 0) {
        NSString* networkType;
        //Use Apples Reachability sample class for detecting the network type
        Reachability * reachability = [Reachability reachabilityForInternetConnection];
        NetworkStatus networkStatus = [reachability currentReachabilityStatus];
        NSLog(@"networkStatus is %d", networkStatus);
        switch(networkStatus)
        {
            case NotReachable:
                networkType = @"none";
                break;
            case ReachableViaWWAN:
                networkType = @"mobile"; //Generic name for mobile networks
                break;
            case ReachableViaWiFi:
                networkType = @"wifi";
                break;
            default:
                networkType = @"unknown";
                break;
        }
        BOOL success = [networkType getCString:buf maxLength:size encoding:NSUTF8StringEncoding];
        res = (success)?strlen(buf) + 1: -1;
    }
    return res;
}

/*
 * UI related
 */
SYSCALL(MAExtent, maGetScrSize())
{
    CGSize size = [[ScreenOrientation getInstance] screenSize];
    int width = (int) size.width;
    int height = (int)size.height;
    return EXTENT(width, height);
}

SYSCALL(void, maMessageBox(const char* title, const char* message))
{
    MoSyncUIUtils_ShowAlert(title, message, "OK", NULL, NULL);
}

SYSCALL(void, maImagePickerOpen())
{
    MoSyncUIUtils_ShowImagePicker();
}

SYSCALL(void, maImagePickerOpenWithEventReturnType(int returnType))
{
    MoSyncUIUtils_ShowImagePicker(returnType);
}

SYSCALL(int, maSaveImageToDeviceGallery(MAHandle imageDataHandle, const char* imageName))
{
    if ( imageDataHandle <= 0 )
    {
        return MA_MEDIA_RES_IMAGE_EXPORT_FAILED;
    }
    else
    {
        return MoSyncUIUtils_SaveImageToGallery(imageDataHandle);
    }
}

//Shows an alert box with up to three buttons
SYSCALL(void, maAlert(const char* title, const char* message, const char* button1, const char* button2, const char* button3))
{
    MoSyncUIUtils_ShowAlert(title, message, button1, button2, button3);
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

int maTextBox(const wchar* title, const wchar* inText, wchar* outText, int maxSize, int constraints)
{
    MoSyncUIUtils_ShowTextBox(title, inText, outText, maxSize, constraints);
    return 0;
}

/*
 * File related
 */
SYSCALL(int, maFileSetProperty(const char* path, int property, int value))
{
    NSURL *url = [NSURL fileURLWithPath:[NSString stringWithCString:path encoding:NSUTF8StringEncoding] isDirectory:NO];
    if(!url || ![[NSFileManager defaultManager] fileExistsAtPath:[url path]])
    {
        return MA_FERR_NOTFOUND;
    }

    int returnValue = 0;
    switch (property) {
        case MA_FPROP_IS_BACKED_UP:
        {
            if (SYSTEM_VERSION_LESS_THAN_OR_EQUAL_TO(@"5.0"))
            {
                returnValue = MA_FERR_GENERIC;
            }
            else if (&NSURLIsExcludedFromBackupKey == nil)
            {
                // For iOS <= 5.0.1.
                const char* filePath = [[url path] fileSystemRepresentation];
                const char* attrName = "com.apple.MobileBackup";
                u_int8_t attrValue = 1;
                int result = setxattr(filePath, attrName, &attrValue, sizeof(attrValue), 0, 0);
                returnValue = (result == 0) ? 0 : MA_FERR_GENERIC;
            }
            else
            {
                // For iOS >= 5.1
                NSNumber* value = [NSNumber numberWithBool:(value == 0)];
                BOOL set = [url setResourceValue:value forKey:NSURLIsExcludedFromBackupKey error:NULL];
                returnValue = set ? 0 : MA_FERR_GENERIC;
            }
        }
            break;
        default:
        {
            returnValue = MA_FERR_NO_SUCH_PROPERTY;
        }
            break;
    }
    return returnValue;
}

/*
 * Mobility
 */
SYSCALL(int, maSendTextSMS(const char* dst, const char* msg))
{
    if ([MFMessageComposeViewController canSendText] == NO) {
        return CONNERR_UNAVAILABLE;
    }

    MFMessageComposeViewController *smsController = [[MFMessageComposeViewController alloc] init];

    smsController.recipients = [NSArray arrayWithObject:[NSString stringWithCString:dst encoding:NSUTF8StringEncoding]];
    smsController.body = [NSString stringWithCString:msg encoding:NSUTF8StringEncoding];

    smsController.messageComposeDelegate = [[SMSResultDelegate alloc] init];

    MoSyncUI* msUI = getMoSyncUI();
    [msUI showModal:smsController];

    [smsController release];
    return 0;
}

/*
 * MoSync basic
 */
SYSCALL(int, maGetEvent(MAEvent *dst))
{
    Base::gSyscall->ValidateMemRange(dst, sizeof(MAEvent));
    MYASSERT(((uint)dst & 3) == 0, ERR_MEMORY_ALIGNMENT);	//alignment

    if(!Base::gClosing)
    {
        Base::gEventOverflow = false;
    }

    MAEvent ev;
    bool ret = Base::gEventQueue.getAndProcess(ev);
    if(!ret) return 0;
    else *dst = ev; //gEventQueue.get();

#define HANDLE_CUSTOM_EVENT(eventType, dataType) if(ev.type == eventType) {\
memcpy(MoSync_GetCustomEventData(), (void*)ev.data, sizeof(dataType));\
delete (dataType*)ev.data;\
dst->data = (int)MoSync_GetCustomEventDataMoSyncPointer(); }

    CUSTOM_EVENTS(HANDLE_CUSTOM_EVENT);

    return 1;
}

int maReportResourceInformation()
{
    Base::gSyscall->resources.logEverything();
    return 0;
}

SYSCALL(int, maFreeObjectMemory())
{
    return getFreeAmountOfMemory();
}

SYSCALL(int, maTotalObjectMemory())
{
    return getTotalAmountOfMemory();
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

SYSCALL(void, maPanic(int result, const char* message))
{
    MoSyncUIUtils_ShowAlert(NULL, message, "OK", NULL, NULL);
    gRunning = false;
    pthread_exit(NULL);
    //[[NSThread currentThread] exit];
}

/*
 * MoSync Extensions
 */
SYSCALL(MAExtensionModule, maExtensionModuleLoad(const char* name, int hash))
{
    return MA_EXTENSION_MODULE_UNAVAILABLE;
}

SYSCALL(MAExtensionFunction, maExtensionFunctionLoad(MAHandle module, int index))
{
    return MA_EXTENSION_FUNCTION_UNAVAILABLE;
}

SYSCALL(longlong, maExtensionFunctionInvoke(int, int, int, int MA_IOCTL_ELLIPSIS))
{
    BIG_PHAT_ERROR(ERR_FUNCTION_UNIMPLEMENTED);
}
