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
#include "MoSyncUI.h"
#include "MoSyncMain.h"
#include "MoSyncUISyscalls.h"
#include "iphone_helpers.h"

#include <AudioToolbox/AudioToolbox.h>
#import <MessageUI/MessageUI.h>

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

SYSCALL(int, maSendTextSMS(const char* dst, const char* msg))
{
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

int maTextBox(const wchar* title, const wchar* inText, wchar* outText, int maxSize, int constraints)
{
    MoSync_ShowTextBox(title, inText, outText, maxSize, constraints);
    return 0;
}
