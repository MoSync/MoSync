package com.mosync.pim;

import java.util.ArrayList;
import java.util.Iterator;

import android.content.ContentProviderOperation;
import android.content.ContentResolver;
import android.database.Cursor;
import android.provider.ContactsContract.Data;

public class PIMItem {

	ArrayList<PIMField> mPIMFields;

	PIMItem()
	{
		mPIMFields = new ArrayList<PIMField>();
	}

	void add(PIMField p)
	{
		mPIMFields.add(p);
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

	void setID(String id, String type)
	{
		PIMField p = new PIMField(type);
		p.add(new String[] {}, new String[] {id});
		add(p);
	}

	void readField(ContentResolver cr, String contactId, String[] columns, String itemType)
	{
		Cursor cursor = cr.query(Data.CONTENT_URI, columns,
				Data.CONTACT_ID + "=?" + " AND "
					+ Data.MIMETYPE + "='" + itemType + "'",
					new String[] {String.valueOf(contactId)}, null);

		if (cursor.getCount() > 0)
		{
			PIMField p = new PIMField(itemType);
			while (cursor.moveToNext())
		    {
				String[] info = new String[columns.length];
				for (int i=0; i<columns.length; i++)
					info[i] = cursor.getString( cursor.getColumnIndex(columns[i]));

				p.add(columns, info);
		    }
			add(p);
		}
	}

	void updateField(ContentResolver cr, String contactId, String[] columns, String itemType)
	{
		 ArrayList<ContentProviderOperation> ops = new ArrayList<ContentProviderOperation>();

		 ops.add(ContentProviderOperation.newUpdate(Data.CONTENT_URI)
		          .withSelection(Data.CONTACT_ID + "=?",
					new String[] {String.valueOf(contactId)})
		          .withValue(itemType, "sample_text")
		          .build());
		 //cr.applyBatch(ContactsContract.AUTHORITY, ops);
	}
}
