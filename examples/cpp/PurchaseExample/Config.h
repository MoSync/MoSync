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

#define DEVELOPER_PUBLIC_KEY "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAz9uqRfzlI/YSlR+yIScYAK2r4GJtm8abqkBvR9JOxueI0FQdAwfbI3AtbTnzVQkqdtHbeQ4GbitUKUm/lny2rNOJxCu/AbzicjTfP3fc4tuRE+Lzr0ZtApg61oPLSJa7xdkBNhmnjuvXRD+dMvskunfsXK4HzYh/TBGCywDkTD5mnZR1fiEJ3DQlHu+aZMSA3xcq07fBuvCG4PxQOo52wYu0qJ+n2p4Y3gIXPaBPMH9SjrDXrlpiH21Bmb4nOdBBKn9370atU5J1uBSk6U5Q+VNHs3SnWL8TgW2gyNv1wLzI6vq7MiRXhGg1I7lHaedKBFbxtHba0BXelF4rGxYn6wIDAQAB"
//"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEApBbxQThM/yme38OTd/Dd9+33RzhSyn/gPF6cVb/88e1Vvr0CUq5vzZCTbnH/LO8eQ2RHgh/8boQmc678ypiP8YUr1hPa4GUFWrYvOccv+ByfRrWi4HISQceDrmuPuJ0/4P0XseIfn2lOgP2JIVfC/JNr3iRNpnjf+II5nYhLUY/dqDU6TF6pif4V6R8FGn8fuS7gfIOGQvJhy5hi4gJ5togpvchMUTB6pNlbTiB/Cv9tI8909vdz4G+vs/xVx5up0w7HMivUd5azTRYbsjAEP6359+IDyq0jghrqxTcJRD/qcFyYTGiJjp1AwdTFdCy4UQrRek6ULCjgjY3Ig4jZnwIDAQAB"

#define IOS_PRODUCT_TYPE_1 "yourConsumableProductID"
#define IOS_PRODUCT_TYPE_2 "yourNonconsumableProductID"

#endif /* CONFIG_H_ */
