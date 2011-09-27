package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Iterator;

import com.mosync.pim.PIMField.State;

import android.content.ContentProviderOperation;
import android.content.ContentResolver;
import android.content.ContentUris;
import android.content.OperationApplicationException;
import android.database.Cursor;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.RemoteException;
import android.provider.ContactsContract;
import android.provider.ContactsContract.CommonDataKinds.Nickname;
import android.provider.ContactsContract.CommonDataKinds.Phone;
import android.provider.ContactsContract.Contacts;
import android.provider.ContactsContract.Data;
import android.provider.ContactsContract.CommonDataKinds.Photo;

public class PIMItem {

	public enum State { NONE, ADDED, UPDATED, DELETED }

	ArrayList<PIMField> mPIMFields;
	State mState;

	PIMItem()
	{
		mPIMFields = new ArrayList<PIMField>();
		mState = State.NONE;
	}

	void read(ContentResolver cr, String contactId, String[] types, String[][] columns)
	{
		DebugPrint("Fields Columns = " + columns.length);

		setID(contactId, types[0], columns[0]);

		DebugPrint("Contact ID = " + getID());

		for (int i = 1; i < columns.length; i++)
		{
			DebugPrint("FIELD = " + i);
			readField(cr, contactId, columns[i], types[i]);
		}
	}

	void add(PIMField p)
	{
		mPIMFields.add(p);
	}

	void add(String itemType, String[] names)
	{
		PIMField p = new PIMField(itemType, names);
		add(p);
	}

	void create(String[] types, String[][] names)
	{
		setState(State.ADDED);
		for (int i = 1; i < names.length; i++)
		{
			add(types[i], names[i]);
		}
	}

	PIMField getField(int type)
	{
		Iterator<PIMField> i = mPIMFields.iterator();
		while (i.hasNext())
		{
			PIMField p = i.next();
			if (p.getMoSyncType() == type)
				return p;
		}
		return null;
	}

	int getFieldsCount()
	{
		return mPIMFields.size();
	}

	int getFieldType(int n)
	{
		PIMField field = mPIMFields.get(n);
		return field.getMoSyncType();
	}

	void setID(String id, String type, String[] names)
	{
		PIMField p = new PIMField(type, names);
		p.add(new String[] {id});
		add(p);
	}

	String getID()
	{
		return mPIMFields.get(0).getId(0);
	}

	void readField(ContentResolver cr, String contactId, String[] columns, String itemType)
	{
		Cursor cursor = cr.query(Data.CONTENT_URI, columns,
				Data.CONTACT_ID + "=?" + " AND "
					+ Data.MIMETYPE + "='" + itemType + "'",
					new String[] {String.valueOf(contactId)}, null);

		PIMField p = new PIMField(itemType, columns);
		if (cursor.getCount() > 0)
		{
			while (cursor.moveToNext())
		    {
				p.read(cr, cursor, contactId, columns, itemType);
			}
		}
		DebugPrint("ADD: " + itemType);
		add(p);
	}

	void setState(State state)
	{
		if (state == State.ADDED)
		{
			mState = State.ADDED;
		}
		else if ((mState == State.NONE) ||
			(mState == State.UPDATED))
		{
			mState = state;
		}
	}

	void close(ContentResolver cr)
	{
		DebugPrint("PIM ITEM CLOSE " + mState.toString());
		ArrayList<ContentProviderOperation> ops = new ArrayList<ContentProviderOperation>();
		int rawContactIndex = 0;
		if (mState == State.ADDED)
		{
			rawContactIndex = ops.size();
			ops.add(ContentProviderOperation.newInsert(ContactsContract.RawContacts.CONTENT_URI)
					.withValue(ContactsContract.RawContacts.ACCOUNT_TYPE, null)
					.withValue(ContactsContract.RawContacts.ACCOUNT_NAME, null)
					.build());
            setState(State.NONE);
		}
		else if (mState == State.UPDATED)
		{
			rawContactIndex = Integer.parseInt(getID());
			setState(State.NONE);
		}

		Iterator <PIMField> it = mPIMFields.iterator();
		while (it.hasNext())
		{
			it.next().close(ops, rawContactIndex);
		}
		// Asking the Contact provider to create a new contact
		try
		{
			DebugPrint("REQUEST UPDATE");
			cr.applyBatch(ContactsContract.AUTHORITY, ops);
		}
		catch (Exception e)
		{
			e.printStackTrace();
			DebugPrint("Exception: " + e.getMessage());
		}
	}

	void delete(ContentResolver cr)
	{
		DebugPrint("PIM ITEM DELETE " + getID());
		ArrayList<ContentProviderOperation> ops = new ArrayList<ContentProviderOperation>();
		ops.add(ContentProviderOperation.newDelete(Contacts.CONTENT_URI)
				.withSelection(Contacts._ID + "=?", new String[]{getID()})
				.build());
		try
		{
			DebugPrint("REQUEST UPDATE");
			cr.applyBatch(ContactsContract.AUTHORITY, ops);
		}
		catch (Exception e)
		{
			e.printStackTrace();
			DebugPrint("Exception: " + e.getMessage());
		}
	}
}
