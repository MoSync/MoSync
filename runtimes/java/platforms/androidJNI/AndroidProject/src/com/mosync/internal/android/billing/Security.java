// Copyright 2010 Google Inc. All Rights Reserved.

package com.mosync.internal.android.billing;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.security.KeyFactory;
import java.security.NoSuchAlgorithmException;
import java.security.PublicKey;
import java.security.SecureRandom;
import java.security.Signature;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.X509EncodedKeySpec;
import java.util.ArrayList;
import java.util.HashSet;

import android.text.TextUtils;
import android.util.Log;

import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_COMPLETED;
import static com.mosync.internal.android.MoSyncHelpers.SYSLOG;

/**
 * Security-related methods for verifying the receipts.
 *
 * @author emma
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

    /** Generates a nonce (a random number used once). */
    public static long generateNonce()
    {
        long nonce = RANDOM.nextLong();
        sKnownNonces.add(nonce);
        return nonce;
    }

    public static void removeNonce(long nonce)
    {
        sKnownNonces.remove(nonce);
    }

    public static boolean isNonceKnown(long nonce)
    {
        return sKnownNonces.contains(nonce);
    }

    /**
     * Verifies that the data was signed with the given signature, and returns
     * the list of verified purchases. The data is in JSON format and contains
     * a nonce (number used once) that we generated and that was signed
     * (as part of the whole data string) with a private key. The data also
     * contains the purchaseState and product ID of the purchase.
     * In the general case, there can be an array of purchase transactions
     * because there may be delays in processing the purchase on the backend
     * and then several purchases can be batched together.
     * @param signedData the signed JSON string (signed, not encrypted)
     * @param signature the signature for the data, signed with the private key
     */
    public static ArrayList<PurchaseInformation> verifyPurchase(
			PublicKey key, String signedData, String signature)
    {
        if (signedData == null)
        {
            Log.e("@@MoSync", "Billing Information: data is null!!");
            return null;
        }

        boolean verified = false;
        if (!TextUtils.isEmpty(signature))
        {
            verified = Security.verify(key, signedData, signature);
            if (!verified)
            {
                Log.e("@@MoSync", "Billing Information: signature does not match data.");
                return null;
            }
        }else
        {
			Log.e("@@MoSync","Billing Information not signed, no signature received!");
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
                Log.e("@@MoSync","BillingService received " + numTransactions + " transactions !");
            }
        } catch (JSONException e)
        {
			// Cannot parse receipt,but since the product is not known,
			// there is no need to send an event.
            return null;
        }

        if (!Security.isNonceKnown(nonce))
        {
            Log.w("@@MoSync", "Billing Information: Nonce not found: " + nonce);
            return null;
        }

        ArrayList<PurchaseInformation> purchases = new ArrayList<PurchaseInformation>();
        try {
            for (int i = 0; i < numTransactions; i++)
            {
                JSONObject jElement = jTransactionsArray.getJSONObject(i);
                int response = jElement.getInt(Consts.TRANSACTION_PURCHASE_STATE);
                SYSLOG("@@MoSync Purchase field response - " + response);
                int purchaseState = Consts.purchaseStateValue(response);
                String productId = jElement.getString(Consts.TRANSACTION_PRODUCT_ID);
                SYSLOG("MoSync Purchase field productId - " + productId);
                String packageName = jElement.getString(Consts.TRANSACTION_PACKAGE_NAME);
                SYSLOG("MoSync Purchase field packageName - " + packageName);
                long purchaseTime = jElement.getLong(Consts.TRANSACTION_PURCHASE_TIME);
                SYSLOG("MoSync Purchase field purchaseTime - " + purchaseTime);
                String orderId = jElement.optString(Consts.TRANSACTION_ORDER_ID, "");
                SYSLOG("MoSync Purchase field orderId - " + orderId);
                String payload = jElement.optString(Consts.TRANSACTION_DEVELOPER_PAYLOAD,"");
                SYSLOG("MoSync Purchase field payload - " + payload);

                String notifyId = null;
                if (jElement.has(Consts.TRANSACTION_NOTIFICATION_ID))
                {
                    notifyId = jElement.getString(Consts.TRANSACTION_NOTIFICATION_ID);
                    SYSLOG("MoSync Purchase field notifyId - " + notifyId);
                }
                // If the purchase state is PURCHASED, then we require a verified nonce.
                if ( purchaseState == MA_PURCHASE_STATE_COMPLETED && !verified) {
                    continue;
                }
                // For REFUNDED or RESTORED purchases, the payload will be left null.
                purchases.add(new PurchaseInformation(purchaseState, notifyId,
						productId, orderId, purchaseTime, packageName, payload));
            }
        } catch (JSONException e) {
            SYSLOG("MoSync Purchase Information: JSON exception: " + e.toString());
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
            throw new IllegalArgumentException(e);
        } catch (Base64DecoderException e) {
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
                return false;
            }
            return true;
        } catch (Exception e) {
        }
        return false;
    }
}
