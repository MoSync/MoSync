/* Copyright (C) 2011 MoSync AB

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

package com.mosync.internal.android.notifications;

/**
 * Holds info about the latest registration that was attempted.
 * In case of success the registrationID is provided,
 * otherwise an error code and an error message.
 * @author emma tresanszki
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
			"This phone doesn't currently support GCM/C2DM.";

	/**
	 * Constructor.
	 */
	public PushRegistrationData(){}

	/**
	 * True if a registration request was made.
	 */
	public Boolean registrationAttempted = false;

	/**
	 * True if a registration request is in progress.
	 * ( Has not received server's response yet).
	 */
	public Boolean registrationInProgress = false;

	/**
	 * True if the application is already registered to the GCM/C2DM service.
	 */
	public Boolean registrationSuccess = false;

	/**
	 * The registration ID received from the GCM/C2DM server.
	 */
	public String registrationID = null;

	/**
	 * The error message to accompany the errCode.
	 * One of the REG_ERR_MESSAGE_ constants.
	 */
	public String errorMessage = null;
}
