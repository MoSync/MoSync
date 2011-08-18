package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Iterator;

import com.mosync.internal.android.MoSyncThread;
import android.app.Activity;
import android.content.ContentResolver;
import android.database.Cursor;
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
			Im. TYPE, Im.LABEL, Im.IS_PRIMARY },
		{ Relation._ID, Relation.NAME, Relation.TYPE, Relation.LABEL, Relation.IS_PRIMARY },
		{ Organization._ID, Organization.DEPARTMENT, Organization.JOB_DESCRIPTION,
			Organization.SYMBOL, Organization.PHONETIC_NAME, Organization.OFFICE_LOCATION,
			Organization.TYPE, Organization.LABEL, Organization.IS_PRIMARY },
	};

	final static String[] PIMFieldsTypes =
	{
		ContactsContract.Contacts._ID,
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
	 * PIM types
	 */
	private final static int PIM_TYPE_CONTACTS = 1;
	private final static int PIM_TYPE_EVENTS = 2;

	/*
	 * Errors
	 */
	private final static int PIM_ERROR_NONE = 0;
	private final static int PIM_ERROR_CANNOT_OPEN = -1;
	private final static int PIM_ERROR_ALREADY_OPENED = -2;
	private final static int PIM_ERROR_LIST_TYPE_NOT_SUPPORTED = -3;
	private final static int PIM_ERROR_INVALID_LIST_HANDLE = -4;
	private final static int PIM_ERROR_INVALID_ITEM_HANDLE = -4;
	private final static int PIM_ERROR_INVALID_FIELD = -5;
	private final static int PIM_ERROR_INVALID_INDEX = -6;
	private final static int PIM_ERROR_NO_LABEL = -7;
	private final static int PIM_ERROR_NO_VALUE = -8;
	private final static int PIM_ERROR_BUFFER_TOO_SMALL = -9;

	private final static int BUFFER_SIZE = 255;

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
			case PIM_TYPE_CONTACTS:
				return openContactsList();
			case PIM_TYPE_EVENTS:
				return openEventsList();
		}
		return PIM_ERROR_LIST_TYPE_NOT_SUPPORTED;
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
			return PIM_ERROR_CANNOT_OPEN;
		}

	    if (cur == null)
	    {
			return PIM_ERROR_CANNOT_OPEN;
	    }

	    if (mPIMContactsList == null)
	    {
			mPIMContactsList = new ArrayList<PIMItem>();
	    }
	    else
	    {
			return PIM_ERROR_ALREADY_OPENED;
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

	/*
	 * Opens the events list.
	 */
	int openEventsList()
	{
		return 0;
	}

	public int maPimListNext(int list)
	{
		DebugPrint("maPimListNext " + list);
		if ((list < 0) || ((mPIMIterator = mPIMIterators.get(list)) == null))
		{
			return PIM_ERROR_INVALID_LIST_HANDLE;
		}

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
		if ((list < 0) || (!mPIMLists.containsKey(list)) )
		{
			return PIM_ERROR_INVALID_LIST_HANDLE;
		}

		mPIMContactsList = mPIMLists.remove(list);
		mPIMContactsList = null;

		return 0;
	}

	public int maPimItemCount(int item)
	{
		PIMItem pimItem = null;
		if ( (item < 0) || ((pimItem = mPIMItems.get(item)) == null) )
		{
			return PIM_ERROR_INVALID_ITEM_HANDLE;
		}

		return pimItem.getFieldsCount();
	}

	public int maPimItemGetField(int item, int n)
	{
		PIMItem pimItem = null;
		if ( (item < 0) || ((pimItem = mPIMItems.get(item)) == null) )
		{
			return PIM_ERROR_INVALID_ITEM_HANDLE;
		}

		if ( (n < 0) || (n >= pimItem.getFieldsCount()) )
		{
			return PIM_ERROR_INVALID_INDEX;
		}

		return pimItem.getFieldType(n);
	}

	public int maPimItemFieldCount(int item, int field)
	{
		PIMItem pimItem = null;
		if ( (item < 0) || ((pimItem = mPIMItems.get(item)) == null) )
		{
			return PIM_ERROR_INVALID_ITEM_HANDLE;
		}

		PIMField pimField = null;
		if ( (pimField = pimItem.getField(field)) == null )
		{
			return PIM_ERROR_INVALID_FIELD;
		}

		return pimField.length();
	}

	public int maPimItemGetAttributes(int item, int field, int index)
	{
		PIMItem pimItem = null;
		if ( (item < 0) || ((pimItem = mPIMItems.get(item)) == null) )
		{
			return PIM_ERROR_INVALID_ITEM_HANDLE;
		}

		PIMField pimField = null;
		if ( (pimField = pimItem.getField(field)) == null )
		{
			return PIM_ERROR_INVALID_FIELD;
		}

		if ( (index < 0) || (index >= pimField.length()) )
		{
			return PIM_ERROR_INVALID_INDEX;
		}

		return pimField.getAttribute(index);
	}

	public int maPimItemSetLabel(int item, int field, int buffPointer, int buffSize, int index)
	{
		PIMItem pimItem = null;
		if ( (item < 0) || ((pimItem = mPIMItems.get(item)) == null) )
		{
			return PIM_ERROR_INVALID_ITEM_HANDLE;
		}

		PIMField pimField = null;
		if ( (pimField = pimItem.getField(field)) == null )
		{
			return PIM_ERROR_INVALID_FIELD;
		}

		if ( (index < 0) || (index >= pimField.length()) )
		{
			return PIM_ERROR_INVALID_INDEX;
		}

		String buffer = readStringFromMemory(buffPointer, buffSize);
		if (buffer == null)
		{
			return PIM_ERROR_NO_LABEL;
		}

		pimField.setCustomLabel(index, buffer);
		//pimItem.updateField(getContentResolver(), contactId, PIMFieldsColumns[i], PIMFieldsTypes[i]);

		return PIM_ERROR_NONE;
	}

	public int maPimItemGetLabel(int item, int field, int buffPointer, int buffSize, int index)
	{
		PIMItem pimItem = null;
		if ( (item < 0) || ((pimItem = mPIMItems.get(item)) == null) )
		{
			return PIM_ERROR_INVALID_ITEM_HANDLE;
		}

		PIMField pimField = null;
		if ( (pimField = pimItem.getField(field)) == null )
		{
			return PIM_ERROR_INVALID_FIELD;
		}

		if ( (index < 0) || (index >= pimField.length()) )
		{
			return PIM_ERROR_INVALID_INDEX;
		}

		String buffer = pimField.getCustomLabel(index);
		buffer = "buf";
		if (buffer == null)
		{
			return PIM_ERROR_NO_LABEL;
		}

		if ( buffer.length() > buffSize )
			return PIM_ERROR_BUFFER_TOO_SMALL;

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
		DebugPrint("maPimListNext " + list);
		if ((list < 0) || ((mPIMIterator = mPIMIterators.get(list)) == null))
		{
			return PIM_ERROR_INVALID_LIST_HANDLE;
		}

		return PIMField.getDataType(field);
	}

	public int maPimItemGetValue(int item, int field, int buffPointer, int buffSize, int index)
	{
		PIMItem pimItem = null;
		if ( (item < 0) || ((pimItem = mPIMItems.get(item)) == null) )
		{
			return PIM_ERROR_INVALID_ITEM_HANDLE;
		}

		PIMField pimField = null;
		if ( (pimField = pimItem.getField(field)) == null )
		{
			return PIM_ERROR_INVALID_FIELD;
		}

		if ( (index < 0) || (index >= pimField.length()) )
		{
			return PIM_ERROR_INVALID_INDEX;
		}

		String buffer = pimField.getData(index);
		DebugPrint("GET DATA BUFFER = " + buffer);
		if (buffer.length() == 0)
		{
			return PIM_ERROR_NO_VALUE;
		}

		if ( buffer.length() > buffSize )
			return PIM_ERROR_BUFFER_TOO_SMALL;

		DebugPrint("buffP = " + buffPointer + "; buff = " + buffer);
		copyStringToMemory(buffPointer, buffer);

		return buffer.length();
	}

	int maPimItemSetValue(int item, int field, int buffPointer, int buffSize, int index, int attributes)
	{
		return 0;
	}

	int maPimItemAddValue(int item, int field, int buffPointer, int buffSize, int attributes)
	{
		return 0;
	}

	int maPimItemRemoveValue(int item, int field, int index)
	{
		return 0;
	}

	int maPimItemClose(int item)
	{
		return 0;
	}

	int maPimItemCreate(int list)
	{
		return 0;
	}

	int maPimItemRemove(int list, int item)
	{
		return 0;
	}
}
