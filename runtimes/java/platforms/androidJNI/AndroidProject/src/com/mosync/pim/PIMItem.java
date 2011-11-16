package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_FIELD_INVALID;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_FIELD_UNSUPPORTED;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_INDEX_INVALID;

import java.util.ArrayList;
import java.util.Iterator;

import com.mosync.internal.android.MoSyncError;

import android.content.ContentProviderOperation;
import android.content.ContentProviderResult;
import android.content.ContentResolver;
import android.database.Cursor;
import android.provider.ContactsContract;
import android.provider.ContactsContract.Contacts;
import android.provider.ContactsContract.RawContacts;

public class PIMItem {
	public enum State {
		NONE, ADDED, UPDATED
	}

	State mState;

	PIMFieldAddress mAddress;
	PIMFieldBirthday mBirthday;
	PIMFieldClass mClass;
	PIMFieldEmail mEmail;
	PIMFieldFormattedAddress mFormattedAddress;
	PIMFieldFormattedName mFormattedName;
	PIMFieldName mName;
	PIMFieldNickname mNickname;
	PIMFieldNote mNote;
	PIMFieldOrganization mOrganization;
	PIMFieldPhoto mPhoto;
	PIMFieldPhotoURL mPhotoURL;
	PIMFieldPublicKey mPublicKey;
	PIMFieldPublicKeyString mPublicKeyString;
	PIMFieldRevision mRevision;
	PIMFieldPhone mPhone;
	PIMFieldTitle mTitle;
	PIMFieldUID mUID;
	PIMFieldURL mURL;
	PIMFieldIM mIM;
	PIMFieldRelation mRelation;
	PIMFieldOrganizationInfo mOrganizationInfo;
	PIMFieldEvent mEvent;

	ArrayList<PIMField> mPIMFields;

	/**
	 * Constructor
	 */
	PIMItem(boolean isNew) {
		setState(State.NONE);
		if (isNew) {
			setState(State.ADDED);
		}
		mPIMFields = new ArrayList<PIMField>();
		mAddress = new PIMFieldAddress();
		mBirthday = new PIMFieldBirthday();
		mClass = new PIMFieldClass();
		mEmail = new PIMFieldEmail();
		mFormattedAddress = new PIMFieldFormattedAddress();
		mFormattedName = new PIMFieldFormattedName();
		mName = new PIMFieldName();
		mNickname = new PIMFieldNickname();
		mNote = new PIMFieldNote();
		mOrganization = new PIMFieldOrganization();
		mPhoto = new PIMFieldPhoto();
		mPhotoURL = new PIMFieldPhotoURL();
		mPublicKey = new PIMFieldPublicKey();
		mPublicKeyString = new PIMFieldPublicKeyString();
		mRevision = new PIMFieldRevision();
		mPhone = new PIMFieldPhone();
		mTitle = new PIMFieldTitle();
		mUID = new PIMFieldUID();
		mURL = new PIMFieldURL();
		mIM = new PIMFieldIM();
		mRelation = new PIMFieldRelation();
		mOrganizationInfo = new PIMFieldOrganizationInfo();
		mEvent = new PIMFieldEvent();
		mPIMFields.add(mAddress);
		mPIMFields.add(mBirthday);
		mPIMFields.add(mClass);
		mPIMFields.add(mEmail);
		mPIMFields.add(mFormattedAddress);
		mPIMFields.add(mFormattedName);
		mPIMFields.add(mName);
		mPIMFields.add(mNickname);
		mPIMFields.add(mNote);
		mPIMFields.add(mOrganization);
		mPIMFields.add(mPhoto);
		mPIMFields.add(mPhotoURL);
		mPIMFields.add(mPublicKey);
		mPIMFields.add(mPublicKeyString);
		mPIMFields.add(mRevision);
		mPIMFields.add(mPhone);
		mPIMFields.add(mTitle);
		mPIMFields.add(mUID);
		mPIMFields.add(mURL);
		mPIMFields.add(mIM);
		mPIMFields.add(mRelation);
		mPIMFields.add(mOrganizationInfo);
		// mPIMFields.add(mEvent);
	}

	/**
	 * @param errorCode
	 *            The error code returned by the syscall.
	 * @param panicCode
	 *            The panic code for this error.
	 * @param panicText
	 *            The panic text for this error.
	 * @return
	 */
	public int throwError(int errorCode, int panicCode, String panicText) {
		return MoSyncError.getSingletonObject().error(errorCode, panicCode,
				panicText);
	}

	/**
	 * Read the item with contactId.
	 * @param cr
	 * @param contactId
	 */
	void read(ContentResolver cr, String contactId) {
		DebugPrint("PIMItem.read(" + cr + ", " + contactId + ")");

		try {
			Iterator<PIMField> fieldsIt = mPIMFields.iterator();

			while (fieldsIt.hasNext()) {
				fieldsIt.next().read(cr, contactId);
			}
		} catch (Exception e) {
			DebugPrint("Failed to read contact " + contactId + ".");
			e.printStackTrace();
		}
	}

	void setState(State state) {
		if ((mState != State.ADDED) && (state != State.NONE))
			mState = state;
	}

	/**
	 * Gets the number of fields in the item.
	 */
	int length() {
		DebugPrint("length()");

		int counter = 0;
		Iterator<PIMField> fieldsIt = mPIMFields.iterator();

		while (fieldsIt.hasNext()) {
			if (!fieldsIt.next().isEmpty())
				counter++;
		}

		return counter;
	}

	/**
	 * @param type
	 * @return The field with the given type.
	 */
	PIMField getField(int type) {
		Iterator<PIMField> i = mPIMFields.iterator();
		while (i.hasNext()) {
			PIMField p = i.next();
			if (p.getType() == type)
				return p;
		}
		return null;
	}

	/**
	 * @param index
	 * @return The type of the requested field.
	 */
	int getFieldType(int index) {
		DebugPrint("PIMItem.getFieldType(" + index + ")");
		if ((index < 0) || (index >= mPIMFields.size())) {
			return throwError(MA_PIM_ERR_INDEX_INVALID,
					PIMError.PANIC_INDEX_INVALID, PIMError.sStrIndexInvalid);
		}

		return mPIMFields.get(index).getType();
	}

	/**
	 * Gets the length of a field.
	 * @param field
	 * @return The length of the given field.
	 */
	int getFieldLength(int field) {
		DebugPrint("PIMItem.getFieldLength()");
		PIMField pimField = getField(field);

		if (pimField == null) {
			return throwError(MA_PIM_ERR_FIELD_INVALID,
					PIMError.PANIC_FIELD_INVALID, PIMError.sStrFieldInvalid);
		}
		if (!pimField.isSupported()) {
			return MA_PIM_ERR_FIELD_UNSUPPORTED;
		}

		return pimField.length();
	}

	/**
	 * @param field
	 * @param index
	 * @return The attributes for the given field.
	 */
	int getFieldAttributes(int field, int index) {
		DebugPrint("getFieldAttributes(" + field + ", " + index + ")");
		PIMField pimField = getField(field);

		if (pimField == null) {
			return throwError(MA_PIM_ERR_FIELD_INVALID,
					PIMError.PANIC_FIELD_INVALID, PIMError.sStrFieldInvalid);
		}
		if (!pimField.isSupported()) {
			return MA_PIM_ERR_FIELD_UNSUPPORTED;
		}

		return pimField.getAttributes(index);
	}

	/**
	 * Gets the custom label of the specified field.
	 * @param field
	 * @param index
	 * @param buffPointer
	 * @param buffSize
	 * @return
	 */
	int getFieldLabel(int field, int index, int buffPointer, int buffSize) {
		DebugPrint("getFieldLabel(" + field + ", " + index + ", " + buffPointer
				+ ", " + buffSize + ")");
		PIMField pimField = getField(field);

		if (pimField == null) {
			return throwError(MA_PIM_ERR_FIELD_INVALID,
					PIMError.PANIC_FIELD_INVALID, PIMError.sStrFieldInvalid);
		}
		if (!pimField.isSupported()) {
			return MA_PIM_ERR_FIELD_UNSUPPORTED;
		}

		return pimField.getLabel(index, buffPointer, buffSize);
	}

	/**
	 * Sets the custom label of the specified field.
	 * @param field
	 * @param index
	 * @param buffPointer
	 * @param buffSize
	 * @return
	 */
	int setFieldLabel(int field, int index, int buffPointer, int buffSize) {
		DebugPrint("setFieldLabel(" + field + ", " + index + ", " + buffPointer
				+ ", " + buffSize + ")");
		PIMField pimField = getField(field);

		if (pimField == null) {
			return throwError(MA_PIM_ERR_FIELD_INVALID,
					PIMError.PANIC_FIELD_INVALID, PIMError.sStrFieldInvalid);
		}
		if (!pimField.isSupported()) {
			return MA_PIM_ERR_FIELD_UNSUPPORTED;
		}

		setState(State.UPDATED);
		return pimField.setLabel(index, buffPointer, buffSize);
	}

	/**
	 * Gets the value of the specified field.
	 * @param field
	 * @param index
	 * @param buffPointer
	 * @param buffSize
	 * @return
	 */
	int getFieldValue(int field, int index, int buffPointer, int buffSize) {
		DebugPrint("getFieldValue(" + field + ", " + index + ", " + buffPointer
				+ ", " + buffSize + ")");
		PIMField pimField = getField(field);

		if (pimField == null) {
			return throwError(MA_PIM_ERR_FIELD_INVALID,
					PIMError.PANIC_FIELD_INVALID, PIMError.sStrFieldInvalid);
		}
		if (!pimField.isSupported()) {
			return MA_PIM_ERR_FIELD_UNSUPPORTED;
		}

		return pimField.getValue(index, buffPointer, buffSize);
	}

	/**
	 * Sets the value of the specified field.
	 * @param field
	 * @param index
	 * @param buffPointer
	 * @param buffSize
	 * @return
	 */
	int setFieldValue(int field, int index, int buffPointer, int buffSize,
			int attributes) {
		DebugPrint("setFieldValue(" + field + ", " + index + ", " + buffPointer
				+ ", " + buffSize + ", " + attributes + ")");
		PIMField pimField = getField(field);

		if (pimField == null) {
			return throwError(MA_PIM_ERR_FIELD_INVALID,
					PIMError.PANIC_FIELD_INVALID, PIMError.sStrFieldInvalid);
		}
		if (!pimField.isSupported()) {
			return MA_PIM_ERR_FIELD_UNSUPPORTED;
		}

		setState(State.UPDATED);
		return pimField.setValue(index, buffPointer, buffSize, attributes);
	}

	/**
	 * Add the value to the specified field.
	 * @param field
	 * @param index
	 * @param buffPointer
	 * @param buffSize
	 * @return
	 */
	int addFieldValue(int field, int buffPointer, int buffSize, int attributes) {
		DebugPrint("addFieldValue(" + field + ", " + buffPointer + ", "
				+ buffSize + ", " + attributes + ")");
		PIMField pimField = getField(field);

		if (pimField == null) {
			return throwError(MA_PIM_ERR_FIELD_INVALID,
					PIMError.PANIC_FIELD_INVALID, PIMError.sStrFieldInvalid);
		}
		if (!pimField.isSupported()) {
			return MA_PIM_ERR_FIELD_UNSUPPORTED;
		}

		return pimField.addValue(buffPointer, buffSize, attributes);
	}

	/**
	 * Removes the value of the specified field.
	 * @param field
	 * @param index
	 * @param buffPointer
	 * @param buffSize
	 * @return
	 */
	int removeFieldValue(int field, int index) {
		DebugPrint("getFieldValue(" + field + ", " + index + ")");
		PIMField pimField = getField(field);

		if (pimField == null) {
			return throwError(MA_PIM_ERR_FIELD_INVALID,
					PIMError.PANIC_FIELD_INVALID, PIMError.sStrFieldInvalid);
		}
		if (!pimField.isSupported()) {
			return MA_PIM_ERR_FIELD_UNSUPPORTED;
		}

		return pimField.removeValue(index);
	}

	/**
	 * Gets the data type of the specified field.
	 * @param field
	 * @return
	 */
	int getFieldDataType(int field) {
		PIMField pimField = getField(field);

		if (pimField == null) {
			return throwError(MA_PIM_ERR_FIELD_INVALID,
					PIMError.PANIC_FIELD_INVALID, PIMError.sStrFieldInvalid);
		}
		if (!pimField.isSupported()) {
			return MA_PIM_ERR_FIELD_UNSUPPORTED;
		}

		return pimField.getDataType();
	}

	void delete(ContentResolver cr) {
		if (mState == State.ADDED) {
			return;
		}

		Iterator<PIMField> fieldsIt = mPIMFields.iterator();

		while (fieldsIt.hasNext()) {
			fieldsIt.next().close();
		}

		String id = mUID.getSpecificData(0);
		cr.delete(RawContacts.CONTENT_URI, RawContacts.CONTACT_ID + " = ?",
				new String[] { id });

		// ArrayList<ContentProviderOperation> ops = new
		// ArrayList<ContentProviderOperation>();
		// ops.add(ContentProviderOperation.newDelete(Contacts.CONTENT_URI)
		// // .withSelection(Contacts._ID + " = ?", new String[] { id })
		// .withSelection(Contacts._ID + " = " + id, null).build());
		// try {
		// DebugPrint("REQUEST UPDATE");
		// ContentProviderResult[] res = cr.applyBatch(
		// ContactsContract.AUTHORITY, ops);
		// for (int i = 0; i < res.length; i++) {
		// DebugPrint("RESULT " + i + ": " + res[i].toString());
		// }
		// } catch (Exception e) {
		// e.printStackTrace();
		// DebugPrint("Exception: " + e.getMessage());
		// }
	}

	/**
	 * Closes the item
	 */
	void close(ContentResolver cr) {
		DebugPrint("PIMItem.close()");
		ArrayList<ContentProviderOperation> ops = new ArrayList<ContentProviderOperation>();
		Iterator<PIMField> fieldsIt = mPIMFields.iterator();

		int rawContactIndex = 0;
		if (mState == State.ADDED) {
			rawContactIndex = ops.size();
			ops.add(ContentProviderOperation.newInsert(RawContacts.CONTENT_URI)
					.withValue(RawContacts.ACCOUNT_TYPE, null)
					.withValue(RawContacts.ACCOUNT_NAME, null).build());
			while (fieldsIt.hasNext()) {
				fieldsIt.next().add(ops, rawContactIndex);
			}
		} else if (mState == State.UPDATED) {
			rawContactIndex = Integer.parseInt(mUID.getSpecificData(0));
			while (fieldsIt.hasNext()) {
				fieldsIt.next().update(cr, ops, rawContactIndex);
			}
		}

		setState(State.NONE);
		try {
			DebugPrint("REQUEST UPDATE");
			ContentProviderResult[] res = cr.applyBatch(
					ContactsContract.AUTHORITY, ops);
			for (int i = 0; i < res.length; i++) {
				DebugPrint("RESULT " + i + ": " + res[i].toString());
			}
		} catch (Exception e) {
			e.printStackTrace();
			DebugPrint("Exception: " + e.getMessage());
		}

		fieldsIt = mPIMFields.iterator();

		while (fieldsIt.hasNext()) {
			fieldsIt.next().close();
		}
	}
}
