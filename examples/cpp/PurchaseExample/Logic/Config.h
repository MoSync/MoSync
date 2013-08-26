/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/*
 * @file Config.h
 *
 * Before you can run the sample application, you need to configure it
 * and build it by doing the following:
 * 	- Add your Google Play public key to the sample application code.
 * This enables the application to verify the signature of the transaction
 * information that is returned from Google Play. To add your public key to
 * the sample application code, do the following:
 * - Log in to your Google Play publisher account.
 * - On the upper left part of the page, under your name, click Edit Profile.
 * - On the Edit Profile page, scroll down to the Licensing & In-app Billing panel.
 * - Copy your public key.
 * - Set is as DEVELOPER_PUBLIC_KEY in this file.
 * - Save the file.
 *
 * @author emma
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define DEVELOPER_PUBLIC_KEY ""

#define IOS_PRODUCT_TYPE_1 "yourConsumableProductID"
#define IOS_PRODUCT_TYPE_2 "yourNonconsumableProductID"

#define ANDROID_MANAGED_ITEM_1 "managed_item_1"
#define ANDROID_MANAGED_ITEM_2 "managed_item_2"
#define ANDROID_MANAGED_ITEM_3 "managed_item_3"
#endif /* CONFIG_H_ */
