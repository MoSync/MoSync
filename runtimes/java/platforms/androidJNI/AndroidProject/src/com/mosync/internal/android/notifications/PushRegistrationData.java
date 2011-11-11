package com.mosync.internal.android.notifications;

/**
 * Holds info about the latest registration that was attempted.
 * @author emma tresanszki
 *
 */
public class PushRegistrationData {

	public static final String REG_ERR_MESSAGE_SERVICE_NOT_AVAILABLE = "The device can't read the response," +
		"or there was a 500/503 from the server that can be retried later.";

	public static final String REG_ERR_MESSAGE_ACCOUNT_MISSING = "There is no Google account on the phone." +
			"The application should ask the user to open the account manager and add a Google account.";

	public static final String REG_ERR_MESSAGE_AUTHENTICATION_FAILED = "Bad password. The application should ask " +
			"the user to enter his/her password, and let user retry manually later";

	public static final String REG_ERR_MESSAGE_TOO_MANY_REGISTRATIONS = "The user has too many applications registered." +
			"The application should tell the user to uninstall some other applications, let user retry manually";

	public static final String REG_ERR_MESSAGE_INVALID_SENDER = "The sender account is not recognized.";

	public static final String REG_ERR_MESSAGE_PHONE_ERROR = "Incorrect phone registration with Google." +
			"This phone doesn't currently support C2DM.";

	/**
	 * Constructor.
	 */
	PushRegistrationData(){}

	public Boolean registrationAttempted = false;

	/**
	 * True if the application is already registered to the C2DM service.
	 */
	public Boolean registrationSuccess = false;

	/**
	 * The registration ID received from the C2DM server.
	 */
	public String registrationID = null;

	/**
	 * The error code received from the C2DM server.
	 */
	public int errorCode;

	/**
	 * The error message to accompany the errCode.
	 * One of the REG_ERR_MESSAGE_ constants.
	 */
	public String errorMessage = null;
}
