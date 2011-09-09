package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;


import com.mosync.internal.generated.IX_PIM;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_ADDR;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_ORG_INFO;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_CONTACTS;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_EVENTS;

import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NONE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_UNAVAILABLE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NATIVE_TYPE_MISMATCH;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_LIST_TYPE_INVALID;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_LIST_UNAVAILABLE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_LIST_ALREADY_OPENED;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_FIELD_INVALID;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_FIELD_UNSUPPORTED;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_FIELD_EMPTY;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_FIELD_COUNT_MAX;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_FIELD_READ_ONLY;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_FIELD_WRITE_ONLY;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_INDEX_INVALID;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_HANDLE_INVALID;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_ATTRIBUTE_COMBO_UNSUPPORTED;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_ATTRIBUTE_CUSTOM;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NO_ATTRIBUTES;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_BUFFER_TOO_SMALL;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_BUFFER_INVALID;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_NO_LABEL;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ERR_OPERATION_NOT_PERMITTED;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Iterator;

import com.mosync.internal.android.MoSyncThread;
import com.mosync.internal.android.MoSyncThread.ImageCache;

import android.app.Activity;
import android.content.ContentResolver;
import android.database.Cursor;
import android.graphics.Bitmap;
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
			StructuredPostal.COUNTRY, StructuredPostal.NEIGHBORHOOD,
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

	private final static int PIM_ERROR_LIST_TYPE_NOT_SUPPORTED = -3;

	/*
	 * PIM list
	 */
	private ArrayList<PIMItem> mPIMContactsList;
	private Iterator<PIMItem> mPIMIterator;

	/**
	 * Table that holds PIM resources.
	 */
	private Hashtable<Integer, ArrayList<PIMItem>> mPIMLists =
		new Hashtable<Integer, ArrayList<PIMItem>>();
	private Hashtable<Integer, Iterator<PIMItem>> mPIMIterators =
		new Hashtable<Integer, Iterator<PIMItem>>();
	private Hashtable<Integer, PIMItem> mPIMItems =
		new Hashtable<Integer, PIMItem>();

	/**
	 * Mapping between image handles and bitmaps.
	 */
	private static Hashtable<Integer, ImageCache> mImageTable =
		new Hashtable<Integer, ImageCache>();
	private static int mImagePlaceholder = 0;

	/*
	 * Handle for PIM
	 */
	private int mResourceIndex = 0;

	/**
	 * The MoSync thread object.
	 */
	private MoSyncThread mMoSyncThread;

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
//			case PIM_TYPE_EVENTS:
//				return openEventsList();
		}
		return MA_PIM_ERR_LIST_TYPE_INVALID;
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
			return MA_PIM_ERR_LIST_UNAVAILABLE;
		}

	    if (cur == null)
	    {
			return MA_PIM_ERR_LIST_UNAVAILABLE;
	    }

	    if (mPIMContactsList == null)
	    {
			mPIMContactsList = new ArrayList<PIMItem>();
	    }
	    else
	    {
			return MA_PIM_ERR_LIST_ALREADY_OPENED;
	    }

	    DebugPrint("Contacts " + cur.getCount());

	    while (cur.moveToNext())
	    {
			String contactId = cur.getString(cur.getColumnIndex(ContactsContract.Contacts._ID));

			PIMItem pimItem = new PIMItem();
			pimItem.read(getContentResolver(), contactId, PIMFieldsTypes, PIMFieldsColumns);

			mPIMContactsList.add(pimItem);
	    }

	    mPIMIterator = mPIMContactsList.iterator();

	    mPIMLists.put(mResourceIndex, mPIMContactsList);
	    mPIMIterators.put(mResourceIndex, mPIMIterator);

	    return mResourceIndex++;
	}

	/*
	 * Opens the events list.
	 */
//	int openEventsList()
//	{
//		return PIM_ERROR_LIST_TYPE_NOT_SUPPORTED;
//	}

	public int maPimListNext(int list)
	{
		DebugPrint("maPimListNext " + list);
		if ((list < 0) || ((mPIMIterator = mPIMIterators.get(list)) == null))
		{
			return MA_PIM_ERR_HANDLE_INVALID;
		}

	    if (mPIMIterator.hasNext())
	    {
			mPIMItems.put(mResourceIndex, mPIMIterator.next());
	    }
	    else
	    {
			return 0;
	    }

		return mResourceIndex++;
	}

	public int maPimListClose(int list)
	{
		if ((list < 0) || (!mPIMLists.containsKey(list)) )
		{
			return MA_PIM_ERR_HANDLE_INVALID;
		}

		Iterator<PIMItem> it = mPIMLists.get(list).iterator();

		while (it.hasNext())
		{
			it.next().close();
		}

		mPIMContactsList = mPIMLists.remove(list);
		mPIMContactsList = null;

		return MA_PIM_ERR_NONE;
	}

	public int maPimItemCount(int item)
	{
		PIMItem pimItem = null;
		if ( (item < 0) || ((pimItem = mPIMItems.get(item)) == null) )
		{
			return MA_PIM_ERR_HANDLE_INVALID;
		}

		return pimItem.getFieldsCount();
	}

	public int maPimItemGetField(int item, int n)
	{
		PIMItem pimItem = null;
		if ( (item < 0) || ((pimItem = mPIMItems.get(item)) == null) )
		{
			return MA_PIM_ERR_HANDLE_INVALID;
		}

		if ( (n < 0) || (n >= pimItem.getFieldsCount()) )
		{
			return MA_PIM_ERR_INDEX_INVALID;
		}

		return pimItem.getFieldType(n);
	}

	public int maPimItemFieldCount(int item, int field)
	{
		PIMItem pimItem = null;
		if ( (item < 0) || ((pimItem = mPIMItems.get(item)) == null) )
		{
			return MA_PIM_ERR_HANDLE_INVALID;
		}

		if ((field < MA_PIM_FIELD_CONTACT_ADDR) ||
				(field > MA_PIM_FIELD_CONTACT_ORG_INFO))
		{
			return MA_PIM_ERR_FIELD_INVALID;
		}

		PIMField pimField = null;
		if ( (pimField = pimItem.getField(field)) == null )
		{
			return MA_PIM_ERR_FIELD_UNSUPPORTED;
		}

		return pimField.length();
	}

	public int maPimItemGetAttributes(int item, int field, int index)
	{
		PIMItem pimItem = null;
		if ( (item < 0) || ((pimItem = mPIMItems.get(item)) == null) )
		{
			return MA_PIM_ERR_HANDLE_INVALID;
		}

		if ((field < MA_PIM_FIELD_CONTACT_ADDR) ||
				(field > MA_PIM_FIELD_CONTACT_ORG_INFO))
		{
			return MA_PIM_ERR_FIELD_INVALID;
		}

		PIMField pimField = null;
		if ( (pimField = pimItem.getField(field)) == null )
		{
			return MA_PIM_ERR_FIELD_UNSUPPORTED;
		}

		if ( (index < 0) || (index >= pimField.length()) )
		{
			return MA_PIM_ERR_INDEX_INVALID;
		}

		return pimField.getAttribute(index);
	}

	public int maPimItemSetLabel(int item, int field, int buffPointer, int buffSize, int index)
	{
		PIMItem pimItem = null;
		if ( (item < 0) || ((pimItem = mPIMItems.get(item)) == null) )
		{
			return MA_PIM_ERR_HANDLE_INVALID;
		}

		if ((field < MA_PIM_FIELD_CONTACT_ADDR) ||
				(field > MA_PIM_FIELD_CONTACT_ORG_INFO))
		{
			return MA_PIM_ERR_FIELD_INVALID;
		}

		PIMField pimField = null;
		if ( (pimField = pimItem.getField(field)) == null )
		{
			return MA_PIM_ERR_FIELD_UNSUPPORTED;
		}

		if (pimField.length() == 0)
		{
			return MA_PIM_ERR_FIELD_EMPTY;
		}

		if ( (index < 0) || (index >= pimField.length()) )
		{
			return MA_PIM_ERR_INDEX_INVALID;
		}

		if (!pimField.hasCustomLabel(index))
		{
			return MA_PIM_ERR_NO_LABEL;
		}

		char[] buffer = readStringFromMemory(buffPointer, buffSize >> 1);

		pimField.setCustomLabel(index, new String(buffer));

		return MA_PIM_ERR_NONE;
	}

	public int maPimItemGetLabel(int item, int field, int buffPointer, int buffSize, int index)
	{
		PIMItem pimItem = null;
		if ( (item < 0) || ((pimItem = mPIMItems.get(item)) == null) )
		{
			return MA_PIM_ERR_HANDLE_INVALID;
		}

		if ((field < MA_PIM_FIELD_CONTACT_ADDR) ||
				(field > MA_PIM_FIELD_CONTACT_ORG_INFO))
		{
			return MA_PIM_ERR_FIELD_INVALID;
		}

		PIMField pimField = null;
		if ( (pimField = pimItem.getField(field)) == null )
		{
			return MA_PIM_ERR_FIELD_UNSUPPORTED;
		}

		if (pimField.length() == 0)
		{
			return MA_PIM_ERR_FIELD_EMPTY;
		}

		if ( (index < 0) || (index >= pimField.length()) )
		{
			return MA_PIM_ERR_INDEX_INVALID;
		}

		if (!pimField.hasCustomLabel(index))
		{
			return MA_PIM_ERR_NO_LABEL;
		}

		char[] buffer = pimField.getCustomLabel(index).toCharArray();

		if ( buffer.length > buffSize )
			return MA_PIM_ERR_BUFFER_TOO_SMALL;

		copyStringToMemory(buffPointer, buffer);

		return buffer.length;
	}

	public int maPimFieldType(int list, int field)
	{
		if ((list < 0) || ((mPIMIterator = mPIMIterators.get(list)) == null))
		{
			return MA_PIM_ERR_HANDLE_INVALID;
		}

		if ((field < MA_PIM_FIELD_CONTACT_ADDR) ||
				(field > MA_PIM_FIELD_CONTACT_ORG_INFO))
		{
			return MA_PIM_ERR_FIELD_INVALID;
		}

		int ret = PIMField.getDataType(field);
		if (ret < 0)
		{
			return MA_PIM_ERR_FIELD_UNSUPPORTED;
		}
		return ret;
	}

	public int maPimItemGetValue(int item, int field, int buffPointer, int buffSize, int index)
	{
		PIMItem pimItem = null;
		if ( (item < 0) || ((pimItem = mPIMItems.get(item)) == null) )
		{
			return MA_PIM_ERR_HANDLE_INVALID;
		}

		if ((field < MA_PIM_FIELD_CONTACT_ADDR) ||
				(field > MA_PIM_FIELD_CONTACT_ORG_INFO))
		{
			return MA_PIM_ERR_FIELD_INVALID;
		}

		PIMField pimField = null;
		if ( (pimField = pimItem.getField(field)) == null )
		{
			return MA_PIM_ERR_FIELD_UNSUPPORTED;
		}

		if ( (index < 0) || (index >= pimField.length()) )
		{
			return MA_PIM_ERR_INDEX_INVALID;
		}

		if (pimField.isWriteOnly())
		{
			return MA_PIM_ERR_FIELD_WRITE_ONLY;
		}

		char[] buffer = new char[1024];
		int length = pimField.getData(index, buffer);

		if ( length > buffSize )
		{
			return MA_PIM_ERR_BUFFER_INVALID;
		}

		if (length > 0)
			copyStringToMemory(buffPointer, buffer, length);
		return length;
	}

	public int maPimItemSetValue(int item, int field, int buffPointer, int buffSize, int index, int attributes)
	{
		PIMItem pimItem = null;
		if ( (item < 0) || ((pimItem = mPIMItems.get(item)) == null) )
		{
			return MA_PIM_ERR_HANDLE_INVALID;
		}

		if ((field < MA_PIM_FIELD_CONTACT_ADDR) ||
				(field > MA_PIM_FIELD_CONTACT_ORG_INFO))
		{
			return MA_PIM_ERR_FIELD_INVALID;
		}

		PIMField pimField = null;
		if ( (pimField = pimItem.getField(field)) == null )
		{
			return MA_PIM_ERR_FIELD_UNSUPPORTED;
		}

		if (pimField.isReadOnly())
		{
			return MA_PIM_ERR_FIELD_READ_ONLY;
		}

		if ( (index < 0) || (index >= pimField.length()) )
		{
			return MA_PIM_ERR_INDEX_INVALID;
		}

		char[] buffer = readStringFromMemory(buffPointer, buffSize >> 1);
		if (pimField.setData(index, buffer) < 0)
		{
			return MA_PIM_ERR_BUFFER_INVALID;
		}

		if (pimField.setAttribute(index, attributes) < 0)
		{
			return MA_PIM_ERR_ATTRIBUTE_COMBO_UNSUPPORTED;
		}

		return MA_PIM_ERR_NONE;
	}

	public int maPimItemAddValue(int item, int field, int buffPointer, int buffSize, int attributes)
	{
		PIMItem pimItem = null;
		if ( (item < 0) || ((pimItem = mPIMItems.get(item)) == null) )
		{
			return MA_PIM_ERR_HANDLE_INVALID;
		}
		if ((field < MA_PIM_FIELD_CONTACT_ADDR) ||
				(field > MA_PIM_FIELD_CONTACT_ORG_INFO))
		{
			return MA_PIM_ERR_FIELD_INVALID;
		}
		PIMField pimField = null;
		if ( (pimField = pimItem.getField(field)) == null )
		{
			return MA_PIM_ERR_FIELD_UNSUPPORTED;
		}

		if (pimField.isReadOnly())
		{
			return MA_PIM_ERR_FIELD_READ_ONLY;
		}

		DebugPrint("BUFFSIZE = " + buffSize);
		char[] buffer = readStringFromMemory(buffPointer, buffSize >> 1);
		if (buffer.length == 0)
		{
			return MA_PIM_ERR_BUFFER_INVALID;
		}
		int index = pimField.addData(buffer);
		if ( index < 0)
		{
			return MA_PIM_ERR_BUFFER_INVALID;
		}

		if (pimField.setAttribute(index, attributes) < 0)
		{
			return MA_PIM_ERR_ATTRIBUTE_COMBO_UNSUPPORTED;
		}

		return index;
	}

	public int maPimItemRemoveValue(int item, int field, int index)
	{
		PIMItem pimItem = null;
		if ( (item < 0) || ((pimItem = mPIMItems.get(item)) == null) )
		{
			return MA_PIM_ERR_HANDLE_INVALID;
		}

		if ((field < MA_PIM_FIELD_CONTACT_ADDR) ||
				(field > MA_PIM_FIELD_CONTACT_ORG_INFO))
		{
			return MA_PIM_ERR_FIELD_INVALID;
		}

		PIMField pimField = null;
		if ( (pimField = pimItem.getField(field)) == null )
		{
			return MA_PIM_ERR_FIELD_UNSUPPORTED;
		}

		if ( (index < 0) || (index >= pimField.length()) )
		{
			return MA_PIM_ERR_INDEX_INVALID;
		}

		pimField.remove(index);

		return MA_PIM_ERR_NONE;
	}

	public int maPimItemClose(int item)
	{
		PIMItem pimItem = null;
		if ( (item < 0) || ((pimItem = mPIMItems.get(item)) == null) )
		{
			return MA_PIM_ERR_HANDLE_INVALID;
		}
		pimItem.close();

		return MA_PIM_ERR_NONE;
	}

	public int maPimItemCreate(int list)
	{
		if ((list < 0) || ((mPIMIterator = mPIMIterators.get(list)) == null))
		{
			return MA_PIM_ERR_HANDLE_INVALID;
		}

		PIMItem p = new PIMItem();
		for (int i = 1; i < PIMFieldsColumns.length; i++)
		{
			p.add(PIMFieldsTypes[i], PIMFieldsColumns[i]);
		}
		mPIMItems.put(mResourceIndex, p);

		return mResourceIndex++;
	}

	public int maPimItemRemove(int list, int item)
	{
		if ((list < 0) || ((mPIMIterator = mPIMIterators.get(list)) == null))
		{
			return MA_PIM_ERR_HANDLE_INVALID;
		}

		if ( (item < 0) || (mPIMItems.get(item) == null) )
		{
			return MA_PIM_ERR_HANDLE_INVALID;
		}

		mPIMItems.remove(item);

		return MA_PIM_ERR_NONE;
	}

	static int addImage(Bitmap img)
	{
		mImageTable.put(new Integer(mImagePlaceholder),
				new ImageCache(null, img));

		return mImagePlaceholder++;
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
	void copyStringToMemory(int address, char[] str)
	{
		copyStringToMemory(address, str, str.length);
	}

	/**
	 * Copy a String value to the given address in the system memory.
	 */
	void copyStringToMemory(int address, char[] str, int length)
	{
		getMemDataSection().position(address);
		for (int i=0; i<length; i++)
		{
			getMemDataSection().putChar(str[i]);
		}
		getMemDataSection().put((byte)0); // Terminating null char.
	}

	/**
	 * Read string from system memory.
	 */
	char[] readStringFromMemory(int address, int length)
	{
		char[] buffer = new char[length];
		getMemDataSection().position(address);
		for (int i=0; i<length; i++)
		{
			buffer[i] = getMemDataSection().getChar();
		}
		return buffer;
	}
}
