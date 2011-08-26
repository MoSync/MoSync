package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;

import com.mosync.internal.android.BigPhatError;

import com.mosync.internal.generated.IX_PIM;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_CONTACTS;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_EVENTS;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NONE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_UNAVAILABLE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NATIVE_TYPE_MISMATCH;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_FIELD_UNSUPPORTED;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_READ_ONLY;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_WRITE_ONLY;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_COMBO_UNSUPPORTED;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_FIELD_COUNT_MAX;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NO_LABEL;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_CUSTOM_ATTRIBUTE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_UNAVAILABLE_LIST;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_OPERATION_NOT_PERMITTED;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_CANNOT_OPEN;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_ALREADY_OPEN;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Iterator;

import com.mosync.internal.android.MoSyncThread;
import android.app.Activity;
import android.content.ContentResolver;
import android.database.Cursor;
import android.provider.BaseColumns;
import android.provider.ContactsContract;
import android.provider.ContactsContract.CommonDataKinds.Email;
import android.provider.ContactsContract.CommonDataKinds.Event;
import android.provider.ContactsContract.CommonDataKinds.Im;
import android.provider.ContactsContract.CommonDataKinds.Nickname;
import android.provider.ContactsContract.CommonDataKinds.Note;
import android.provider.ContactsContract.CommonDataKinds.Organization;
import android.provider.ContactsContract.CommonDataKinds.Phone;
import android.provider.ContactsContract.CommonDataKinds.Photo;
import android.provider.ContactsContract.CommonDataKinds.Relation;
import android.provider.ContactsContract.CommonDataKinds.StructuredPostal;
import android.provider.ContactsContract.CommonDataKinds.Website;
import android.provider.ContactsContract.Contacts;
import android.provider.ContactsContract.CommonDataKinds.StructuredName;
import android.provider.ContactsContract.Data;

public class PIM {

	final static String DUMMY = Data._ID;

	final static String[][] PIMFieldsColumns =
	{
		{},
		{ StructuredPostal._ID, StructuredPostal.POBOX, DUMMY, StructuredPostal.STREET,
			StructuredPostal.CITY, StructuredPostal.REGION, StructuredPostal.POSTCODE,
			StructuredPostal.COUNTRY, DUMMY, StructuredPostal.NEIGHBORHOOD,
			StructuredPostal.TYPE, StructuredPostal.LABEL, StructuredPostal.IS_PRIMARY },
		{ Event._ID, Event.START_DATE, Event.TYPE, Event.LABEL, Event.IS_PRIMARY },
		{ Email._ID, Email.DATA, Email.TYPE, Email.LABEL, Email.IS_PRIMARY },
		{ StructuredPostal._ID, StructuredPostal.FORMATTED_ADDRESS, StructuredPostal.TYPE,
			StructuredPostal.LABEL, StructuredPostal.IS_PRIMARY },
		{ StructuredName._ID, StructuredName.FAMILY_NAME, StructuredName.GIVEN_NAME, StructuredName.MIDDLE_NAME,
			StructuredName.PREFIX, StructuredName.SUFFIX, StructuredName.PHONETIC_FAMILY_NAME,
			StructuredName.PHONETIC_GIVEN_NAME, StructuredName.PHONETIC_MIDDLE_NAME, StructuredName.IS_PRIMARY},
		{ Nickname._ID, Nickname.NAME, Nickname.IS_PRIMARY },
		{ Note._ID, Note.NOTE, Note.IS_PRIMARY },
		{ Organization._ID, Organization.COMPANY, Organization.TYPE,
			Organization.LABEL, Organization.IS_PRIMARY },
		{ Photo._ID, Photo.PHOTO, Photo.IS_PRIMARY },
		{ Phone._ID, Phone.NUMBER, Phone.TYPE, Phone.LABEL, Phone.IS_PRIMARY },
		{ Organization._ID, Organization.TITLE, Organization.TYPE,
			Organization.LABEL, Organization.IS_PRIMARY },
		{ Website._ID, Website.URL, Website.TYPE, Website.LABEL, Website.IS_PRIMARY },
		{ Im._ID, Im.DATA, Im.PROTOCOL, Im.CUSTOM_PROTOCOL,
			Im.TYPE, Im.LABEL, Im.IS_PRIMARY },
		{ Relation._ID, Relation.NAME, Relation.TYPE, Relation.LABEL, Relation.IS_PRIMARY },
		{ Organization._ID, Organization.DEPARTMENT, Organization.JOB_DESCRIPTION,
			Organization.SYMBOL, Organization.PHONETIC_NAME, Organization.OFFICE_LOCATION,
			Organization.TYPE, Organization.LABEL, Organization.IS_PRIMARY },
	};

	final static String[] PIMFieldsTypes =
	{
		ContactsContract.Contacts.LOOKUP_KEY,
		StructuredPostal.CONTENT_ITEM_TYPE,
		Event.CONTENT_ITEM_TYPE,
		Email.CONTENT_ITEM_TYPE,
		StructuredPostal.CONTENT_ITEM_TYPE,
		StructuredName.CONTENT_ITEM_TYPE,
		Nickname.CONTENT_ITEM_TYPE,
		Note.CONTENT_ITEM_TYPE,
		Organization.CONTENT_ITEM_TYPE,
		Photo.CONTENT_ITEM_TYPE,
		Phone.CONTENT_ITEM_TYPE,
		Organization.CONTENT_ITEM_TYPE,
		Website.CONTENT_ITEM_TYPE,
		Im.CONTENT_ITEM_TYPE,
		Relation.CONTENT_ITEM_TYPE,
		Organization.CONTENT_ITEM_TYPE,
	};

	/*
	 * PIM list
	 */
	ArrayList<PIMItem> mPIMContactsList;
	Iterator<PIMItem> mPIMIterator;

	/**
	 * Table that holds PIM resources.
	 */
	Hashtable<Integer, ArrayList<PIMItem>> mPIMLists =
		new Hashtable<Integer, ArrayList<PIMItem>>();
	Hashtable<Integer, Iterator<PIMItem>> mPIMIterators =
		new Hashtable<Integer, Iterator<PIMItem>>();
	Hashtable<Integer, PIMItem> mPIMItems =
		new Hashtable<Integer, PIMItem>();

	/*
	 * Handle for PIM
	 */
	int mResourceIndex = 0;

	/**
	 * The MoSync thread object.
	 */
	MoSyncThread mMoSyncThread;

	/**
	 * @return The Activity object.
	 */
	private Activity getActivity()
	{
		return mMoSyncThread.getActivity();
	}

	/*
	 * @return The Content Resolver.
	 */
	private ContentResolver getContentResolver()
	{
		return getActivity().getContentResolver();
	}

	/**
	 * Constructor.
	 * @param thread The MoSync thread.
	 */
	public PIM(MoSyncThread thread)
	{
		mMoSyncThread = thread;
	}

	/*
	 * Opens the PIM list depending on the listType
	 */
	public int maPimListOpen(int listType)
	{
		switch (listType)
		{
			case MA_PIM_CONTACTS:
				return openContactsList();
		}
		return MA_PIM_ERR_UNAVAILABLE_LIST;
	}

	/*
	 * Opens the contacts list.
	 */
	int openContactsList()
	{
		Cursor cur;
		try
		{
			ContentResolver cr = getContentResolver();
			cur = cr.query(Contacts.CONTENT_URI, new String[] {Contacts._ID}, null, null, null);
		}
		catch (Exception e)
		{

			return MA_PIM_ERR_CANNOT_OPEN;
		}

		if (cur == null)
		{
			return MA_PIM_ERR_CANNOT_OPEN;
		}

		if (mPIMContactsList == null)
		{
			mPIMContactsList = new ArrayList<PIMItem>();
		}
		else
		{
			return MA_PIM_ERR_ALREADY_OPEN;
		}

		DebugPrint("Contacts " + cur.getCount());

		while (cur.moveToNext())
		{
			String contactId = cur.getString(cur.getColumnIndex(ContactsContract.Contacts._ID));
			DebugPrint("CONTACT ID = " + contactId);
			PIMItem pimItem = new PIMItem();

			DebugPrint("Fields Columns = " + PIMFieldsColumns.length);

			pimItem.setID(contactId, PIMFieldsTypes[0]);

			for (int i = 1; i < PIMFieldsColumns.length; i++)
			{
				pimItem.readField(getContentResolver(), contactId, PIMFieldsColumns[i], PIMFieldsTypes[i]);
			}

			mPIMContactsList.add(pimItem);
		}

		mPIMIterator = mPIMContactsList.iterator();

		DebugPrint("Add list at key " + mResourceIndex);

		mPIMLists.put(mResourceIndex, mPIMContactsList);
		mPIMIterators.put(mResourceIndex, mPIMIterator);

		DebugPrint("Iterator: " + mPIMIterator.hasNext());

		return mResourceIndex++;
	}

	public int maPimListNext(int list)
	{
		DebugPrint("maPimListNext " + list);
		mPIMIterator = mPIMIterators.get(list);

		if (mPIMIterator.hasNext())
		{
			mPIMItems.put(mResourceIndex, mPIMIterator.next());
		}
		else
		{
			return 0; //fleu TODO
		}

		return mResourceIndex++;
	}

	public int maPimListClose(int list)
	{
		if (!mPIMLists.containsKey(list))
		{
			throw new BigPhatError("Invalid PIM list handle");
		}

		mPIMContactsList = mPIMLists.remove(list);
		mPIMContactsList = null;

		return 0;
	}

	public int maPimItemCount(int item)
	{
		PIMItem pimItem = mPIMItems.get(item);
		return pimItem.getFieldsCount();
	}

	public int maPimItemGetField(int item, int n)
	{
		PIMItem pimItem = mPIMItems.get(item);
		return pimItem.getFieldType(n);
	}

	public int maPimItemFieldCount(int item, int field)
	{
		PIMItem pimItem = mPIMItems.get(item);
		PIMField pimField = pimItem.getField(field);
		return pimField.length();
	}

	public int maPimItemGetAttributes(int item, int field, int index)
	{
		PIMItem pimItem = mPIMItems.get(item);
		PIMField pimField = pimItem.getField(field);
		return pimField.getAttribute(index);
	}

	public int maPimItemSetLabel(int item, int field, int buffPointer, int buffSize, int index)
	{
		PIMItem pimItem = mPIMItems.get(item);
		PIMField pimField = pimItem.getField(field);
		String buffer = readStringFromMemory(buffPointer, buffSize);
		pimField.setCustomLabel(index, buffer);
		pimItem.updateField(getContentResolver(), BaseColumns._ID, pimField.getId(index), "data3", buffer);
		return MA_PIM_ERR_NONE;
	}

	public int maPimItemGetLabel(int item, int field, int buffPointer, int buffSize, int index)
	{
		PIMItem pimItem = mPIMItems.get(item);
		PIMField pimField = pimItem.getField(field);

		String buffer = pimField.getCustomLabel(index);
		if (buffer == null)
		{
			return MA_PIM_ERR_NO_LABEL;
		}

		if ( buffer.length() > buffSize )
			return buffer.length();

		DebugPrint("Data: " + getMemDataSection() + "; Position: " + Integer.toHexString(buffPointer) + "; Size: " + buffSize);
		copyStringToMemory(buffPointer, buffer);

		return buffer.length();
	}

	/**
	 * @return The MoSync data section memory buffer.
	 */
	public ByteBuffer getMemDataSection()
	{
		return mMoSyncThread.mMemDataSection;
	}

	/**
	 * Copy a String value to the given address in the system memory.
	 */
	void copyStringToMemory(int address, String str)
	{
		getMemDataSection().position(address);
		getMemDataSection().put(str.getBytes());
		getMemDataSection().put((byte)0); // Terminating null char.
	}

	/**
	 * Read string from system memory.
	 */
	String readStringFromMemory(int address,int length)
	{
		byte[] bytes = new byte[length];
		getMemDataSection().position(address);
		getMemDataSection().get(bytes);
		return bytes.toString();
	}

	public int maPimFieldType(int list, int field)
	{
		int ret = PIMField.getDataType(field);
		if (ret < 0)
			return MA_PIM_ERR_FIELD_UNSUPPORTED;
		return ret;
	}

	public int maPimItemGetValue(int item, int field, int buffPointer, int buffSize, int index)
	{
		PIMItem pimItem = mPIMItems.get(item);
		PIMField pimField = pimItem.getField(field);
		String buffer = pimField.getData(index);
		DebugPrint("GET DATA BUFFER = " + buffer);

		if ( buffer.length() > buffSize )
			return buffer.length();

		DebugPrint("buffP = " + buffPointer + "; buff = " + buffer);
		if (buffer.length() > 0)
			copyStringToMemory(buffPointer, buffer);

		return buffer.length();
	}

	int maPimItemSetValue(int item, int field, int buffPointer, int buffSize, int index, int attributes)
	{
		PIMItem pimItem = mPIMItems.get(item);
		PIMField pimField = pimItem.getField(field);
		String buffer = readStringFromMemory(buffPointer, buffSize);
		pimField.setData(index, buffer);
		pimField.setAttribute(index, attributes);
		return MA_PIM_ERR_NONE;
	}

	int maPimItemAddValue(int item, int field, int buffPointer, int buffSize, int attributes)
	{
		PIMItem pimItem = mPIMItems.get(item);
		PIMField pimField = pimItem.getField(field);
		String buffer = readStringFromMemory(buffPointer, buffSize);
		int index = pimField.addData(buffer);
		pimField.setAttribute(index, attributes);
		return index;
	}

	int maPimItemRemoveValue(int item, int field, int index)
	{
		PIMItem pimItem = mPIMItems.get(item);
		PIMField pimField = pimItem.getField(field);
		pimField.remove(index);
		return MA_PIM_ERR_NONE;
	}

	int maPimItemClose(int item)
	{
		throw new BigPhatError("maPimItemClose not implemented");
	}

	int maPimItemCreate(int list)
	{
		throw new BigPhatError("maPimItemCreate not implemented");
/*
		PIMItem p = new PIMItem();
		mPIMItems.put(mResourceIndex, p);
		return mResourceIndex++;
*/
	}

	int maPimItemRemove(int list, int item)
	{
		throw new BigPhatError("maPimItemRemove not implemented");
/*
		mPIMItems.remove(item);
		return MA_PIM_ERR_NONE;
*/
	}
}
