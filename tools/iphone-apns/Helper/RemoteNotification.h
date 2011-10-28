#ifndef __REMOTE_NOTIFICATION_H
#define __REMOTE_NOTIFICATION_H

#include "SSLHelper.h"

#define CA_CERT_PATH        "Certs"

#if defined(IS_DEVELOPMENT_VERSION)
	/* Development Certificates */
	#define RSA_CLIENT_CERT     "Certs/apn-dev-cert.pem"
	#define RSA_CLIENT_KEY      "Certs/apn-dev-key.pem"

	/* Development Connection Infos */
	#define APPLE_HOST          "gateway.sandbox.push.apple.com"
	#define APPLE_PORT          2195

	#define APPLE_FEEDBACK_HOST "feedback.sandbox.push.apple.com"
	#define APPLE_FEEDBACK_PORT 2196
#else
	/* Release Certificates */
	#define RSA_CLIENT_CERT     "Certs/apn-dis-cert.pem"
	#define RSA_CLIENT_KEY      "Certs/apn-dis-key.pem"

	/* Release Connection Infos */
	#define APPLE_HOST          "gateway.push.apple.com"
	#define APPLE_PORT          2195

	#define APPLE_FEEDBACK_HOST "feedback.push.apple.com"
	#define APPLE_FEEDBACK_PORT 2196
#endif

#define DEVICE_BINARY_SIZE  32
#define MAXPAYLOAD_SIZE     256


typedef struct {
	/* The Message that is displayed to the user */
	const char *message;

	/* The name of the Sound which will be played back */
	const char *soundName;

	/* The Number which is plastered over the icon, 0 disables it */
	int badgeNumber;

	/* The Caption of the Action Key the user needs to press to launch the Application */
	const char *actionKeyCaption;

	/* Custom Message Dictionary, which is accessible from the Application */
	const char* dictKey[5];
	const char* dictValue[5];
} Payload;

/* Initialize the payload with zero values */
void init_payload(Payload *payload);

/* Send a Notification to a specified iPhone */
int send_remote_notification(const char *deviceTokenHex, Payload *payload);

#endif
