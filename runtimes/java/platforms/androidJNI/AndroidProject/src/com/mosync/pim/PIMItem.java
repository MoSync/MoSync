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

	ArrayList<PIMField> mPIMFields;

	PIMItem()
	{
		mPIMFields = new ArrayList<PIMField>();
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
				DebugPrint("************START ENTRY");
				String[] info = new String[columns.length];
				for (int i=0; i<columns.length; i++)
				{
					if ( itemType.equals(Photo.CONTENT_ITEM_TYPE) && columns[i].equals(Photo.PHOTO))
					{
						info[i] = loadContactPhoto(cr, contactId);
						if (info[i] != null)
						{
							DebugPrint(columns[i] + ": " + info[i] + "; length = " + info[i].length());
						}
						else
						{
							DebugPrint(columns[i] + " not available");
						}
					}
					else
					{
						try
						{
							info[i] = cursor.getString( cursor.getColumnIndex(columns[i]) );
							DebugPrint(columns[i] + ": " + info[i] + "; length = " + info[i].length());
						}
						catch (Exception e)
						{
							DebugPrint(columns[i] + " not available");
						}
					}
				}

				p.add(info);
				DebugPrint("************END ENTRY");
			}
		}
		DebugPrint("ADD: " + itemType);
		add(p);
	}

	public String loadContactPhoto(ContentResolver cr, String id)
	{
		Uri uri = ContentUris.withAppendedId(Contacts.CONTENT_URI, Long.parseLong(id));
		InputStream input = Contacts.openContactPhotoInputStream(cr, uri);
		if (input == null)
		{
			return null;//getBitmapFromURL("http://thinkandroid.wordpress.com");
		}

		return Integer.toString(PIM.addImage(BitmapFactory.decodeStream(input)));
	}

	void updateField(ContentResolver cr, String dataIDName, String dataIDValue, String dataName, String dataValue)
	{
		ArrayList<ContentProviderOperation> ops = new ArrayList<ContentProviderOperation>();

		ops.add(ContentProviderOperation.newUpdate(Data.CONTENT_URI)
				.withSelection(dataIDName + "=?",
				new String[] {dataIDValue})
				.withValue(dataName, dataValue)
				.build());
//		try
//		{
//			cr.applyBatch(ContactsContract.AUTHORITY, ops);
//		}
//		catch (Exception e)
//		{
//			e.printStackTrace();
//		}
	}
}
