// Copyright 2010 Google Inc. All Rights Reserved.

package com.mosync.internal.android.billing;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.text.TextUtils;
import android.util.Log;

import java.security.InvalidKeyException;
import java.security.KeyFactory;
import java.security.NoSuchAlgorithmException;
import java.security.PublicKey;
import java.security.SecureRandom;
import java.security.Signature;
import java.security.SignatureException;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.X509EncodedKeySpec;
import java.util.ArrayList;
import java.util.HashSet;

/**
 * Security-related methods. For a secure implementation, all of this code
 * should be implemented on a server that communicates with the
 * application on the device. For the sake of simplicity and clarity of this
 * example, this code is included here and is executed on the device. If you
 * must verify the purchases on the phone, you should obfuscate this code to
 * make it harder for an attacker to replace the code with stubs that treat all
 * purchases as verified.
 */
public class Security
{

    public static final String KEY_FACTORY_ALGORITHM = "RSA";
    private static final String SIGNATURE_ALGORITHM = "SHA1withRSA";
    private static final SecureRandom RANDOM = new SecureRandom();

    /**
     * This keeps track of the nonces that we generated and sent to the
     * server.  We need to keep track of these until we get back the purchase
     * state and send a confirmation message back to Android Market. If we are
     * killed and lose this list of nonces, it is not fatal. Android Market will
     * send us a new "notify" message and we will re-generate a new nonce.
     * This has to be "static" so that the {@link BillingReceiver} can
     * check if a nonce exists.
     */
    private static HashSet<Long> sKnownNonces = new HashSet<Long>();

    /**
     * A class to hold the verified purchase information.
     */
    public static class VerifiedPurchase {
        public Consts.PurchaseState purchaseState;
        public String notificationId;
        public String productId;
        public String orderId;
        public long purchaseTime;

        public VerifiedPurchase(Consts.PurchaseState purchaseState, String notificationId,
                String productId, String orderId, long purchaseTime)
        {
            this.purchaseState = purchaseState;
            this.notificationId = notificationId;
            this.productId = productId;
            this.orderId = orderId;
            this.purchaseTime = purchaseTime;
        }
    }

    /** Generates a nonce (a random number used once). */
    public static long generateNonce() {
        long nonce = RANDOM.nextLong();
        sKnownNonces.add(nonce);
        return nonce;
    }

    public static void removeNonce(long nonce) {
        sKnownNonces.remove(nonce);
    }

    public static boolean isNonceKnown(long nonce) {
        return sKnownNonces.contains(nonce);
    }

    /**
     * Verifies that the data was signed with the given signature, and returns
     * the list of verified purchases. The data is in JSON format and contains
     * a nonce (number used once) that we generated and that was signed
     * (as part of the whole data string) with a private key. The data also
     * contains the {@link PurchaseState} and product ID of the purchase.
     * In the general case, there can be an array of purchase transactions
     * because there may be delays in processing the purchase on the backend
     * and then several purchases can be batched together.
     * @param signedData the signed JSON string (signed, not encrypted)
     * @param signature the signature for the data, signed with the private key
     */
    public static ArrayList<VerifiedPurchase> verifyPurchase(String signedData, String signature)
    {
        if (signedData == null)
        {
            Log.e("@@MoSync", "data is null!!");
            return null;
        }
        boolean verified = false;
        if (!TextUtils.isEmpty(signature))
        {
            /**
             * Compute your public key (that you got from the Android Market publisher site).
             *
             * Instead of just storing the entire literal string here embedded in the
             * program,  construct the key at runtime from pieces or
             * use bit manipulation (for example, XOR with some other string) to hide
             * the actual key.  The key itself is not secret information, but we don't
             * want to make it easy for an adversary to replace the public key with one
             * of their own and then fake messages from the server.
             *
             * Generally, encryption keys / passwords should only be kept in memory
             * long enough to perform the operation they need to perform.
             */
            String base64EncodedPublicKey = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAkq5y2a2tbd9xjNfBwtxt1G/wKILzrHSWp3uAwQ/Iw2SkAF72cuoLUiliTkU6IXVhD95k1VsZRLQytKOIAmPOI/faGxbZovmfIq2u/F/Gd39AyUg2geLdhV+wMrgdFWwWiPwOXl+12zigOvJdAfsxNMLh0dhJW1RAK5OaT0TXkR5d26GiUYm6kt0SW4+FX1OSiavwE4NTPTtwqsjscWxBKXvICm0A+0OYtqv6wTC3uhVMDkTnrsVG9N4pTKSGmhz//dfqJIZKI56UD21I2hjsB+8XVxy0KV741x3NLBOruB5oGGlXbCQGHTu3ekY4g5jlusCxlD19327e7LxrooM9GQIDAQAB";
            PublicKey key = Security.generatePublicKey(base64EncodedPublicKey);
            verified = Security.verify(key, signedData, signature);
            if (!verified)
            {
                Log.e("@@MoSync", "signature does not match data.");
                return null;
            }
        }

        JSONObject jObject;
        JSONArray jTransactionsArray = null;
        int numTransactions = 0;
        long nonce = 0L;
        try
        {
            jObject = new JSONObject(signedData);

            // The nonce might be null if the user backed out of the buy page.
            nonce = jObject.optLong("nonce");
            jTransactionsArray = jObject.optJSONArray("orders");
            if (jTransactionsArray != null)
            {
                numTransactions = jTransactionsArray.length();
            }
        } catch (JSONException e)
        {
            return null;
        }

        if (!Security.isNonceKnown(nonce))
        {
            Log.w("@@MoSync", "Nonce not found: " + nonce);
            return null;
        }

        ArrayList<VerifiedPurchase> purchases = new ArrayList<VerifiedPurchase>();
        try {
            for (int i = 0; i < numTransactions; i++)
            {
                JSONObject jElement = jTransactionsArray.getJSONObject(i);
                int response = jElement.getInt(Consts.TRANSACTION_PURCHASE_STATE);
                Consts.PurchaseState purchaseState = Consts.PurchaseState.valueOf(response);
                String productId = jElement.getString("productId"); // or BILLING_REQUEST_ITEM_ID
                String packageName = jElement.getString(Consts.TRANSACTION_PACKAGE_NAME);
                long purchaseTime = jElement.getLong(Consts.TRANSACTION_PURCHASE_TIME);
                String orderId = jElement.optString(Consts.TRANSACTION_ORDER_ID, "");
                String notifyId = null;
                if (jElement.has(Consts.TRANSACTION_NOTIFICATION_ID))
                {
                    notifyId = jElement.getString(Consts.TRANSACTION_NOTIFICATION_ID);
                }
//                String developerPayload = jElement.optString("developerPayload", null);

                // If the purchase state is PURCHASED, then we require a
                // verified nonce.
                if (purchaseState == Consts.PurchaseState.PURCHASED && !verified) {
                    continue;
                }
                purchases.add(new VerifiedPurchase(purchaseState, notifyId,
						productId, orderId, purchaseTime));
            }
        } catch (JSONException e) {
//            Log.e(TAG, "JSON exception: ", e);
            return null;
        }
        removeNonce(nonce);
        return purchases;
    }

    /**
     * Generates a PublicKey instance from a string containing the
     * Base64-encoded public key.
     *
     * @param encodedPublicKey Base64-encoded public key
     * @throws IllegalArgumentException if encodedPublicKey is invalid
     */
    public static PublicKey generatePublicKey(String encodedPublicKey)
    {
        try {
            byte[] decodedKey = Base64.decode(encodedPublicKey);
            KeyFactory keyFactory = KeyFactory.getInstance(KEY_FACTORY_ALGORITHM);
            return keyFactory.generatePublic(new X509EncodedKeySpec(decodedKey));
        } catch (NoSuchAlgorithmException e) {
            throw new RuntimeException(e);
        } catch (InvalidKeySpecException e) {
//            Log.e(TAG, "Invalid key specification.");
            throw new IllegalArgumentException(e);
        } catch (Base64DecoderException e) {
//            Log.e(TAG, "Base64 decoding failed.");
            throw new IllegalArgumentException(e);
        }
    }

    /**
     * Verifies that the signature from the server matches the computed
     * signature on the data.  Returns true if the data is correctly signed.
     *
     * @param publicKey public key associated with the developer account
     * @param signedData signed data from server
     * @param signature server signature
     * @return true if the data and signature match
     */
    public static boolean verify(PublicKey publicKey, String signedData, String signature)
    {

        Signature sig;
        try {
            sig = Signature.getInstance(SIGNATURE_ALGORITHM);
            sig.initVerify(publicKey);
            sig.update(signedData.getBytes());
            if (!sig.verify(Base64.decode(signature))) {
//                Log.e(TAG, "Signature verification failed.");
                return false;
            }
            return true;
        } catch (NoSuchAlgorithmException e) {
//            Log.e(TAG, "NoSuchAlgorithmException.");
        } catch (InvalidKeyException e) {
//            Log.e(TAG, "Invalid key specification.");
        } catch (SignatureException e) {
//            Log.e(TAG, "Signature exception.");
        } catch (Base64DecoderException e) {
//            Log.e(TAG, "Base64 decoding failed.");
        }
        return false;
    }
}
