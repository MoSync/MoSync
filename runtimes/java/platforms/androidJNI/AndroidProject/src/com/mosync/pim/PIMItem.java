package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Iterator;

import android.content.ContentProviderOperation;
import android.content.ContentResolver;
import android.content.ContentUris;
import android.database.Cursor;
import android.graphics.BitmapFactory;
import android.net.Uri;
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

	void close()
	{
		Iterator <PIMField> it = mPIMFields.iterator();
		while (it.hasNext())
		{
			it.next().close();
		}
	}
}
