#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "Helper/RemoteNotification.h"

/* MAIN Function */
int main(int argc, char *argv[])
{
	/* Phone specific Payload message as well as hex formated device token */
	const char* deviceTokenHex = NULL;
	if(argc == 1)
	{
		deviceTokenHex = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
	}
	else
	{
		deviceTokenHex = argv[1];
	}

	if(strlen(deviceTokenHex) < 64 || strlen(deviceTokenHex) > 70)
	{
		printf("Device Token is to short or to long. Length without spaces should be 64 chars...\n");
		printf("%s\n", deviceTokenHex);
		exit(1);
	}

	Payload *payload = (Payload*)malloc(sizeof(Payload));
	init_payload(payload);

	// This is the message the user gets once the Notification arrives
	payload->message = "Message to print out";

	// This is the red numbered badge that appears over the Icon
	payload->badgeNumber = 1;

	// This is the Caption of the Action key on the Dialog that appears
	payload->actionKeyCaption = "Caption of the second Button";

	// These are two dictionary key-value pairs with user-content
	payload->dictKey[0] = "Key1";
	payload->dictValue[0] = "Value1";

	payload->dictKey[1] = "Key2";
	payload->dictValue[1] = "Value2";

	/* Send the payload to the phone */
	printf("Sending APN to Device with UDID: %s\n", deviceTokenHex);
	send_remote_notification(deviceTokenHex, payload);

	return 0;
}
