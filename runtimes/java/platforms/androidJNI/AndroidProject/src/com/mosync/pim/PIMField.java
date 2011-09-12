package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;
import com.mosync.internal.android.BigPhatError;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

import android.content.ContentProviderOperation;
import android.content.ContentResolver;
import android.content.ContentUris;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.provider.ContactsContract;
import android.provider.ContactsContract.Contacts;
import android.provider.ContactsContract.CommonDataKinds.Email;
import android.provider.ContactsContract.CommonDataKinds.Event;
import android.provider.ContactsContract.CommonDataKinds.Im;
import android.provider.ContactsContract.CommonDataKinds.Nickname;
import android.provider.ContactsContract.CommonDataKinds.Note;
import android.provider.ContactsContract.CommonDataKinds.Organization;
import android.provider.ContactsContract.CommonDataKinds.Phone;
import android.provider.ContactsContract.CommonDataKinds.Photo;
import android.provider.ContactsContract.CommonDataKinds.Relation;
import android.provider.ContactsContract.CommonDataKinds.StructuredName;
import android.provider.ContactsContract.CommonDataKinds.StructuredPostal;
import android.provider.ContactsContract.CommonDataKinds.Website;

// field types
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_ADDR;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_BIRTHDAY;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_CLASS;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_EMAIL;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_FORMATTED_ADDR;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_FORMATTED_NAME;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_NAME;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_NICKNAME;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_NOTE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_ORG;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_PHOTO;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_PHOTO_URL;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_PUBLIC_KEY;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_PUBLIC_KEY_STRING;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_REVISION;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_TEL;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_TITLE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_UID;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_URL;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_IM;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_RELATION;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_FIELD_CONTACT_ORG_INFO;

// field data types
import static com.mosync.internal.generated.IX_PIM.MA_PIM_TYPE_BINARY;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_TYPE_BOOLEAN;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_TYPE_DATE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_TYPE_INT;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_TYPE_STRING;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_TYPE_STRING_ARRAY;

//address field attributes
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_ADDR_HOME;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_ADDR_WORK;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_ADDR_OTHER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_ADDR_CUSTOM;

//email field attributes
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_EMAIL_HOME;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_EMAIL_WORK;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_EMAIL_MOBILE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_EMAIL_OTHER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_EMAIL_CUSTOM;

//formatted address field attributes
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_FORMATTED_ADDR_HOME;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_FORMATTED_ADDR_WORK;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_FORMATTED_ADDR_OTHER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_FORMATTED_ADDR_CUSTOM;

//organization field attributes
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_ORG_WORK;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_ORG_OTHER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_ORG_CUSTOM;

//phone field attributes
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_HOME;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_MOBILE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_HOME_FAX;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_WORK_FAX;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_PAGER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_IPHONE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_WORK;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_CALLBACK;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_CAR;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_COMPANY_MAIN;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_ISDN;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_OTHER_FAX;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_RADIO;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_TELEX;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_TTY_TDD;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_WORK_MOBILE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_WORK_PAGER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_ASSISTANT;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_MMS;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_OTHER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_PHONE_CUSTOM;

//title field attributes
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_TITLE_WORK;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_TITLE_OTHER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_TITLE_CUSTOM;

//website field attributes
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_WEBSITE_HOMEPAGE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_WEBSITE_BLOG;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_WEBSITE_PROFILE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_WEBSITE_HOME;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_WEBSITE_WORK;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_WEBSITE_FTP;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_WEBSITE_OTHER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_WEBSITE_CUSTOM;

//im field attributes
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_IM_HOME;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_IM_WORK;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_IM_OTHER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_IM_CUSTOM;

//relation field attributes
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_MOTHER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_FATHER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_PARENT;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_SISTER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_BROTHER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_CHILD;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_FRIEND;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_SPOUSE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_PARTNER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_MANAGER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_ASSISTANT;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_DOMESTIC_PARTNER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_REFERRED_BY;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_RELATIVE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_RELATION_CUSTOM;

//organization info field attributes
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_ORG_INFO_WORK;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_ORG_INFO_OTHER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTR_ORG_INFO_CUSTOM;

//preferred attribute
import static com.mosync.internal.generated.IX_PIM.MA_PIM_ATTRPREFERRED;

//protocols
import static com.mosync.internal.generated.IX_PIM.MA_PIM_CONTACT_IM_PROTOCOL_AIM;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_CONTACT_IM_PROTOCOL_MSN;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_CONTACT_IM_PROTOCOL_YAHOO;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_CONTACT_IM_PROTOCOL_SKYPE;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_CONTACT_IM_PROTOCOL_QQ;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_CONTACT_IM_PROTOCOL_GOOGLE_TALK;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_CONTACT_IM_PROTOCOL_ICQ;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_CONTACT_IM_PROTOCOL_JABBER;
import static com.mosync.internal.generated.IX_PIM.MA_PIM_CONTACT_IM_PROTOCOL_NETMEETING;


public class PIMField
{
	public enum State { NONE, ADDED, UPDATED, DELETED }

	Map<Integer,Integer> mAddressAttributes = new HashMap<Integer,Integer>();
	Map<Integer,Integer> mEmailAttributes = new HashMap<Integer,Integer>();
	Map<Integer,Integer> mFormattedAddressAttributes = new HashMap<Integer,Integer>();
	Map<Integer,Integer> mOrganizationAttributes = new HashMap<Integer,Integer>();
	Map<Integer,Integer> mPhoneAttributes = new HashMap<Integer,Integer>();
	Map<Integer,Integer> mTitleAttributes = new HashMap<Integer,Integer>();
	Map<Integer,Integer> mWebsiteAttributes = new HashMap<Integer,Integer>();
	Map<Integer,Integer> mIMAttributes = new HashMap<Integer,Integer>();
	Map<Integer,Integer> mRelationAttributes = new HashMap<Integer,Integer>();
	Map<Integer,Integer> mOrgInfoAttributes = new HashMap<Integer,Integer>();

	Map<Integer,String> mProtocols = new HashMap<Integer,String>();

	String[] mStrNames;
	ArrayList<String[]> mStrInfos;
	String mStrType;
	ArrayList<State> mState;

	PIMField(String type, String[] names)
	{
		mStrNames = new String[names.length];
		System.arraycopy(names, 0, mStrNames, 0, names.length);
		mStrInfos = new ArrayList<String[]>();
		mStrType = type;
		createMaps();
		mState = new ArrayList<State>();
	}

	void createMaps()
	{
		// Address attributes
		mAddressAttributes.put(MA_PIM_ATTR_ADDR_HOME, StructuredPostal.TYPE_HOME);
		mAddressAttributes.put(MA_PIM_ATTR_ADDR_WORK, StructuredPostal.TYPE_WORK);
		mAddressAttributes.put(MA_PIM_ATTR_ADDR_OTHER, StructuredPostal.TYPE_OTHER);
		mAddressAttributes.put(MA_PIM_ATTR_ADDR_CUSTOM, StructuredPostal.TYPE_CUSTOM);

		// Email attributes
		mEmailAttributes.put(MA_PIM_ATTR_EMAIL_HOME, Email.TYPE_HOME);
		mEmailAttributes.put(MA_PIM_ATTR_EMAIL_WORK, Email.TYPE_WORK);
		mEmailAttributes.put(MA_PIM_ATTR_EMAIL_MOBILE, Email.TYPE_MOBILE);
		mEmailAttributes.put(MA_PIM_ATTR_EMAIL_OTHER, Email.TYPE_OTHER);
		mEmailAttributes.put(MA_PIM_ATTR_EMAIL_CUSTOM, Email.TYPE_CUSTOM);

		// Formatted adress attributes
		mFormattedAddressAttributes.put(MA_PIM_ATTR_FORMATTED_ADDR_HOME, StructuredPostal.TYPE_HOME);
		mFormattedAddressAttributes.put(MA_PIM_ATTR_FORMATTED_ADDR_WORK, StructuredPostal.TYPE_WORK);
		mFormattedAddressAttributes.put(MA_PIM_ATTR_FORMATTED_ADDR_OTHER, StructuredPostal.TYPE_OTHER);
		mFormattedAddressAttributes.put(MA_PIM_ATTR_FORMATTED_ADDR_CUSTOM, StructuredPostal.TYPE_CUSTOM);

		// Organization attributes
		mOrganizationAttributes.put(MA_PIM_ATTR_ORG_WORK, Organization.TYPE_WORK);
		mOrganizationAttributes.put(MA_PIM_ATTR_ORG_OTHER, Organization.TYPE_OTHER);
		mOrganizationAttributes.put(MA_PIM_ATTR_ORG_CUSTOM, Organization.TYPE_CUSTOM);

		// Phone attributes
		mPhoneAttributes.put(MA_PIM_ATTR_PHONE_HOME, Phone.TYPE_HOME);
		mPhoneAttributes.put(MA_PIM_ATTR_PHONE_MOBILE, Phone.TYPE_MOBILE);
		mPhoneAttributes.put(MA_PIM_ATTR_PHONE_HOME_FAX, Phone.TYPE_FAX_HOME);
		mPhoneAttributes.put(MA_PIM_ATTR_PHONE_WORK_FAX, Phone.TYPE_FAX_WORK);
		mPhoneAttributes.put(MA_PIM_ATTR_PHONE_PAGER, Phone.TYPE_PAGER);
		mPhoneAttributes.put(MA_PIM_ATTR_PHONE_IPHONE, Phone.TYPE_CUSTOM);
		mPhoneAttributes.put(MA_PIM_ATTR_PHONE_WORK, Phone.TYPE_WORK);
		mPhoneAttributes.put(MA_PIM_ATTR_PHONE_CALLBACK, Phone.TYPE_CALLBACK);
		mPhoneAttributes.put(MA_PIM_ATTR_PHONE_CAR, Phone.TYPE_CAR);
		mPhoneAttributes.put(MA_PIM_ATTR_PHONE_COMPANY_MAIN, Phone.TYPE_COMPANY_MAIN);
		mPhoneAttributes.put(MA_PIM_ATTR_PHONE_ISDN, Phone.TYPE_ISDN);
		mPhoneAttributes.put(MA_PIM_ATTR_PHONE_OTHER_FAX, Phone.TYPE_OTHER_FAX);
		mPhoneAttributes.put(MA_PIM_ATTR_PHONE_RADIO, Phone.TYPE_RADIO);
		mPhoneAttributes.put(MA_PIM_ATTR_PHONE_TELEX, Phone.TYPE_TELEX);
		mPhoneAttributes.put(MA_PIM_ATTR_PHONE_TTY_TDD, Phone.TYPE_TTY_TDD);
		mPhoneAttributes.put(MA_PIM_ATTR_PHONE_WORK_MOBILE, Phone.TYPE_WORK_MOBILE);
		mPhoneAttributes.put(MA_PIM_ATTR_PHONE_WORK_PAGER, Phone.TYPE_WORK_PAGER);
		mPhoneAttributes.put(MA_PIM_ATTR_PHONE_ASSISTANT, Phone.TYPE_ASSISTANT);
		mPhoneAttributes.put(MA_PIM_ATTR_PHONE_MMS, Phone.TYPE_MMS);
		mPhoneAttributes.put(MA_PIM_ATTR_PHONE_OTHER, Phone.TYPE_OTHER);
		mPhoneAttributes.put(MA_PIM_ATTR_PHONE_CUSTOM, Phone.TYPE_CUSTOM);

		// Title attributes
		mTitleAttributes.put(MA_PIM_ATTR_TITLE_WORK, Organization.TYPE_WORK);
		mTitleAttributes.put(MA_PIM_ATTR_TITLE_OTHER, Organization.TYPE_OTHER);
		mTitleAttributes.put(MA_PIM_ATTR_TITLE_CUSTOM, Organization.TYPE_CUSTOM);

		// Website attributes
		mWebsiteAttributes.put(MA_PIM_ATTR_WEBSITE_HOMEPAGE, Website.TYPE_HOMEPAGE);
		mWebsiteAttributes.put(MA_PIM_ATTR_WEBSITE_BLOG, Website.TYPE_BLOG);
		mWebsiteAttributes.put(MA_PIM_ATTR_WEBSITE_PROFILE, Website.TYPE_PROFILE);
		mWebsiteAttributes.put(MA_PIM_ATTR_WEBSITE_HOME, Website.TYPE_HOME);
		mWebsiteAttributes.put(MA_PIM_ATTR_WEBSITE_WORK, Website.TYPE_WORK);
		mWebsiteAttributes.put(MA_PIM_ATTR_WEBSITE_FTP, Website.TYPE_FTP);
		mWebsiteAttributes.put(MA_PIM_ATTR_WEBSITE_OTHER, Website.TYPE_OTHER);
		mWebsiteAttributes.put(MA_PIM_ATTR_WEBSITE_CUSTOM, Website.TYPE_CUSTOM);

		// IM attributes
		mIMAttributes.put(MA_PIM_ATTR_IM_HOME, Im.TYPE_HOME);
		mIMAttributes.put(MA_PIM_ATTR_IM_WORK, Im.TYPE_WORK);
		mIMAttributes.put(MA_PIM_ATTR_IM_OTHER, Im.TYPE_OTHER);
		mIMAttributes.put(MA_PIM_ATTR_IM_CUSTOM, Im.TYPE_CUSTOM);

		// Relation attributes
		mRelationAttributes.put(MA_PIM_ATTR_RELATION_MOTHER, Relation.TYPE_MOTHER);
		mRelationAttributes.put(MA_PIM_ATTR_RELATION_FATHER, Relation.TYPE_FATHER);
		mRelationAttributes.put(MA_PIM_ATTR_RELATION_PARENT, Relation.TYPE_PARENT);
		mRelationAttributes.put(MA_PIM_ATTR_RELATION_SISTER, Relation.TYPE_SISTER);
		mRelationAttributes.put(MA_PIM_ATTR_RELATION_BROTHER, Relation.TYPE_BROTHER);
		mRelationAttributes.put(MA_PIM_ATTR_RELATION_CHILD, Relation.TYPE_CHILD);
		mRelationAttributes.put(MA_PIM_ATTR_RELATION_FRIEND, Relation.TYPE_FRIEND);
		mRelationAttributes.put(MA_PIM_ATTR_RELATION_SPOUSE, Relation.TYPE_SPOUSE);
		mRelationAttributes.put(MA_PIM_ATTR_RELATION_PARTNER, Relation.TYPE_PARTNER);
		mRelationAttributes.put(MA_PIM_ATTR_RELATION_MANAGER, Relation.TYPE_MANAGER);
		mRelationAttributes.put(MA_PIM_ATTR_RELATION_ASSISTANT, Relation.TYPE_ASSISTANT);
		mRelationAttributes.put(MA_PIM_ATTR_RELATION_DOMESTIC_PARTNER, Relation.TYPE_DOMESTIC_PARTNER);
		mRelationAttributes.put(MA_PIM_ATTR_RELATION_REFERRED_BY, Relation.TYPE_REFERRED_BY);
		mRelationAttributes.put(MA_PIM_ATTR_RELATION_RELATIVE, Relation.TYPE_RELATIVE);
		mRelationAttributes.put(MA_PIM_ATTR_RELATION_CUSTOM, Relation.TYPE_CUSTOM);

		// Organization info
		mOrgInfoAttributes.put(MA_PIM_ATTR_ORG_INFO_WORK, Organization.TYPE_WORK);
		mOrgInfoAttributes.put(MA_PIM_ATTR_ORG_INFO_OTHER, Organization.TYPE_OTHER);
		mOrgInfoAttributes.put(MA_PIM_ATTR_ORG_INFO_CUSTOM, Organization.TYPE_CUSTOM);

		//Protocols
		mProtocols.put(Im.PROTOCOL_AIM, MA_PIM_CONTACT_IM_PROTOCOL_AIM);
		mProtocols.put(Im.PROTOCOL_MSN, MA_PIM_CONTACT_IM_PROTOCOL_MSN);
		mProtocols.put(Im.PROTOCOL_YAHOO, MA_PIM_CONTACT_IM_PROTOCOL_YAHOO);
		mProtocols.put(Im.PROTOCOL_SKYPE, MA_PIM_CONTACT_IM_PROTOCOL_SKYPE);
		mProtocols.put(Im.PROTOCOL_QQ, MA_PIM_CONTACT_IM_PROTOCOL_QQ);
		mProtocols.put(Im.PROTOCOL_GOOGLE_TALK, MA_PIM_CONTACT_IM_PROTOCOL_GOOGLE_TALK);
		mProtocols.put(Im.PROTOCOL_ICQ, MA_PIM_CONTACT_IM_PROTOCOL_ICQ);
		mProtocols.put(Im.PROTOCOL_JABBER, MA_PIM_CONTACT_IM_PROTOCOL_JABBER);
		mProtocols.put(Im.PROTOCOL_NETMEETING, MA_PIM_CONTACT_IM_PROTOCOL_NETMEETING);
		mProtocols.put(Im.PROTOCOL_CUSTOM, "OTHER");
	}

	void read(ContentResolver cr, Cursor cursor, String contactId, String[] columns, String itemType)
	{
		DebugPrint("************START ENTRY");
		String[] info = new String[columns.length];
		for (int i=0; i<columns.length; i++)
		{
			if ( itemType.equals(Photo.CONTENT_ITEM_TYPE) && columns[i].equals(Photo.PHOTO))
			{
				info[i] = loadPhoto(cr, contactId);
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

		add(info);
		DebugPrint("************END ENTRY");
	}

	public String loadPhoto(ContentResolver cr, String id)
	{
		Uri uri = ContentUris.withAppendedId(Contacts.CONTENT_URI, Long.parseLong(id));
		InputStream input = Contacts.openContactPhotoInputStream(cr, uri);
		if (input == null)
		{
			return null;//getBitmapFromURL("http://thinkandroid.wordpress.com");
		}

		return Integer.toString(PIM.addImage(BitmapFactory.decodeStream(input)));
	}

	public String loadPhoto(String url)
	{
		return Integer.toString(PIM.addImage(getBitmapFromURL(url)));
	}

	public Bitmap getBitmapFromURL(String src)
	{
        try {
            URL url = new URL(src);
            HttpURLConnection connection = (HttpURLConnection)
            url.openConnection();
            connection.setDoInput(true);
            connection.connect();
            InputStream input = connection.getInputStream();
            Bitmap myBitmap = BitmapFactory.decodeStream(input);
            return myBitmap;
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }

	public static Object getKeyFromValue(Map<?, ?> hm, Object value)
	{
		for(Object o:hm.keySet())
		{
			if(hm.get(o).equals(value))
			{
				return o;
			}
		}
		return null;
	}

	void add(String[] infos)
	{
		mStrInfos.add(infos);
		mState.add(State.NONE);
	}

	void remove(int index)
	{
		mStrInfos.remove(index);
	}

	int length()
	{
		return mStrInfos.size();
	}

	String getType()
	{
		return mStrType;
	}

	int getMoSyncType()
	{
		if ( mStrType.equals(StructuredPostal.CONTENT_ITEM_TYPE) )
		{
			if (mStrNames.length > 0)
			{
				if ( mStrNames[1].equals(StructuredPostal.FORMATTED_ADDRESS) )
					return MA_PIM_FIELD_CONTACT_FORMATTED_ADDR;
				else
					return MA_PIM_FIELD_CONTACT_ADDR;
			}
		}
		if ( mStrType.equals(Event.CONTENT_ITEM_TYPE) )
		{
			return MA_PIM_FIELD_CONTACT_BIRTHDAY;
		}
		if ( mStrType.equals(Email.CONTENT_ITEM_TYPE) )
		{
			return MA_PIM_FIELD_CONTACT_EMAIL;
		}
		if ( mStrType.equals(StructuredName.CONTENT_ITEM_TYPE) )
		{
			return MA_PIM_FIELD_CONTACT_NAME;
		}
		if ( mStrType.equals(Nickname.CONTENT_ITEM_TYPE) )
		{
			return MA_PIM_FIELD_CONTACT_NICKNAME;
		}
		if ( mStrType.equals(Note.CONTENT_ITEM_TYPE) )
		{
			return MA_PIM_FIELD_CONTACT_NOTE;
		}
		if ( mStrType.equals(Photo.CONTENT_ITEM_TYPE) )
		{
			return MA_PIM_FIELD_CONTACT_PHOTO;
		}
		if ( mStrType.equals(Phone.CONTENT_ITEM_TYPE) )
		{
			return MA_PIM_FIELD_CONTACT_TEL;
		}
		if ( mStrType.equals(ContactsContract.Contacts._ID) )
		{
			return MA_PIM_FIELD_CONTACT_UID;
		}
		if ( mStrType.equals(Organization.CONTENT_ITEM_TYPE) )
		{
			if (mStrNames.length > 0)
			{
				if ( mStrNames[1].equals(Organization.COMPANY) )
				{
					return MA_PIM_FIELD_CONTACT_ORG;
				}
				else if ( mStrNames[1].equals(Organization.TITLE) )
				{
					return MA_PIM_FIELD_CONTACT_TITLE;
				}
				else
				{
					return MA_PIM_FIELD_CONTACT_ORG_INFO;
				}
			}
		}
		if ( mStrType.equals(Website.CONTENT_ITEM_TYPE) )
		{
			return MA_PIM_FIELD_CONTACT_URL;
		}
		if ( mStrType.equals(Im.CONTENT_ITEM_TYPE) )
		{
			return MA_PIM_FIELD_CONTACT_IM;
		}
		if ( mStrType.equals(Relation.CONTENT_ITEM_TYPE) )
		{
			return MA_PIM_FIELD_CONTACT_RELATION;
		}
		return 0;
	}

	int getFieldIntValue(String[] names, String[] infos, String column)
	{
		for (int i=0; i<names.length; i++)
		{
			if ( names[i].equals(column) && infos[i] != null)
			{
				return Integer.parseInt(infos[i]);
			}
		}
		return -1;
	}

	String getFieldValue(String[] names, String[] infos, String column)
	{
		for (int i=0; i<names.length; i++)
		{
			if ( names[i].equals(column) )
			{
				return infos[i];
			}
		}
		return null;
	}

	String setFieldValue(String[] names, String[] infos, String column, String value)
	{
		for (int i=0; i<names.length; i++)
		{
			if ( names[i].equals(column) )
			{
				infos[i] = value;
			}
		}
		return null;
	}

	int getAddressAttribute(int attr)
	{
		if (attr > 0)
			return ((Integer)getKeyFromValue(mAddressAttributes, attr)).intValue();
		return 0;
	}

	int getAddressNativeAttribute(int attr)
	{
		if (!mAddressAttributes.containsKey(attr))
			return -1;
		return mAddressAttributes.get(attr);
	}

	int getEmailAttribute(int attr)
	{
		if (attr > 0)
			return ((Integer)getKeyFromValue(mEmailAttributes, attr)).intValue();
		return 0;
	}

	int getEmailNativeAttribute(int attr)
	{
		if (!mEmailAttributes.containsKey(attr))
			return -1;
		return mEmailAttributes.get(attr);
	}

	int getFormattedAddressAttribute(int attr)
	{
		if (attr > 0)
			return ((Integer)getKeyFromValue(mFormattedAddressAttributes, attr)).intValue();
		return 0;
	}

	int getFormattedAddressNativeAttribute(int attr)
	{
		if (!mFormattedAddressAttributes.containsKey(attr))
			return -1;
		return mFormattedAddressAttributes.get(attr);
	}

	int getOrganizationAttribute(int attr)
	{
		if (attr > 0)
			return ((Integer)getKeyFromValue(mOrganizationAttributes, attr)).intValue();
		return 0;
	}

	int getOrganizationNativeAttribute(int attr)
	{
		if (!mOrganizationAttributes.containsKey(attr))
			return -1;
		return mOrganizationAttributes.get(attr);
	}

	int getPhoneAttribute(int attr)
	{
		if (attr > 0)
			return ((Integer)getKeyFromValue(mPhoneAttributes, attr)).intValue();
		return 0;
	}

	int getPhoneNativeAttribute(int attr)
	{
		if (!mPhoneAttributes.containsKey(attr))
			return -1;
		return mPhoneAttributes.get(attr);
	}

	int getTitleAttribute(int attr)
	{
		if (attr > 0)
			return ((Integer)getKeyFromValue(mTitleAttributes, attr)).intValue();
		return 0;
	}

	int getTitleNativeAttribute(int attr)
	{
		if (!mTitleAttributes.containsKey(attr))
			return -1;
		return mTitleAttributes.get(attr);
	}

	int getOrganizationInfoAttribute(int attr)
	{
		if (attr > 0)
			return ((Integer)getKeyFromValue(mOrgInfoAttributes, attr)).intValue();
		return 0;
	}

	int getOrganizationInfoNativeAttribute(int attr)
	{
		if (!mOrgInfoAttributes.containsKey(attr))
			return -1;
		return mOrgInfoAttributes.get(attr);
	}

	int getWebsiteAttribute(int attr)
	{
		if (attr > 0)
			return ((Integer)getKeyFromValue(mWebsiteAttributes, attr)).intValue();
		return 0;
	}

	int getWebsiteNativeAttribute(int attr)
	{
		if (!mWebsiteAttributes.containsKey(attr))
			return -1;
		return mWebsiteAttributes.get(attr);
	}

	int getImAttribute(int attr)
	{
		if (attr > 0)
			return ((Integer)getKeyFromValue(mIMAttributes, attr)).intValue();
		return 0;
	}

	int getImNativeAttribute(int attr)
	{
		if (!mIMAttributes.containsKey(attr))
			return -1;
		return mIMAttributes.get(attr);
	}

	int getRelationAttribute(int attr)
	{
		if (attr > 0)
			return ((Integer)getKeyFromValue(mRelationAttributes, attr)).intValue();
		return 0;
	}

	int getRelationNativeAttribute(int attr)
	{
		if (!mRelationAttributes.containsKey(attr))
			return -1;
		return mRelationAttributes.get(attr);
	}

	int getAttribute(int index)
	{
		int ret = 0;
		String[] names = mStrNames;
		String[] infos = mStrInfos.get(index);

		DebugPrint("TYPE: " + getMoSyncType());

		switch ( getMoSyncType() )
		{
			case MA_PIM_FIELD_CONTACT_ADDR:
				ret = getAddressAttribute(getFieldIntValue(names, infos, StructuredPostal.TYPE));
				if (getFieldIntValue(names, infos, StructuredPostal.IS_PRIMARY) != 0)
					ret |= MA_PIM_ATTRPREFERRED;
				break;
			case MA_PIM_FIELD_CONTACT_FORMATTED_ADDR:
				ret = getFormattedAddressAttribute(getFieldIntValue(names, infos, StructuredPostal.TYPE));
				if (getFieldIntValue(names, infos, StructuredPostal.IS_PRIMARY) != 0)
					ret |= MA_PIM_ATTRPREFERRED;
				break;
			case MA_PIM_FIELD_CONTACT_EMAIL:
				ret = getEmailAttribute(getFieldIntValue(names, infos, Email.TYPE));
				if (getFieldIntValue(names, infos, Email.IS_PRIMARY) != 0)
					ret |= MA_PIM_ATTRPREFERRED;
				break;
			case MA_PIM_FIELD_CONTACT_ORG:
				ret = getOrganizationAttribute(getFieldIntValue(names, infos, Organization.TYPE));
				if (getFieldIntValue(names, infos, Organization.IS_PRIMARY) != 0)
					ret |= MA_PIM_ATTRPREFERRED;
				break;
			case MA_PIM_FIELD_CONTACT_TITLE:
				ret = getTitleAttribute(getFieldIntValue(names, infos, Organization.TYPE));
				if (getFieldIntValue(names, infos, Organization.IS_PRIMARY) != 0)
					ret |= MA_PIM_ATTRPREFERRED;
				break;
			case MA_PIM_FIELD_CONTACT_ORG_INFO:
				ret = getOrganizationInfoAttribute(getFieldIntValue(names, infos, Organization.TYPE));
				if (getFieldIntValue(names, infos, Organization.IS_PRIMARY) != 0)
					ret |= MA_PIM_ATTRPREFERRED;
				break;
			case MA_PIM_FIELD_CONTACT_TEL:
				ret = getPhoneAttribute(getFieldIntValue(names, infos, Phone.TYPE));
				if ((ret == MA_PIM_ATTR_PHONE_CUSTOM) ||
					((getCustomLabel(index)!= null) && getCustomLabel(index).equals("iPhone")))
					ret = MA_PIM_ATTR_PHONE_IPHONE;
				if (getFieldIntValue(names, infos, Phone.IS_PRIMARY) != 0)
					ret |= MA_PIM_ATTRPREFERRED;
				break;
			case MA_PIM_FIELD_CONTACT_URL:
				ret = getWebsiteAttribute(getFieldIntValue(names, infos, Website.TYPE));
				if (getFieldIntValue(names, infos, Website.IS_PRIMARY) != 0)
					ret |= MA_PIM_ATTRPREFERRED;
				break;
			case MA_PIM_FIELD_CONTACT_IM:
				ret = getImAttribute(getFieldIntValue(names, infos, Im.TYPE));
				if (getFieldIntValue(names, infos, Im.IS_PRIMARY) != 0)
					ret |= MA_PIM_ATTRPREFERRED;
				break;
			case MA_PIM_FIELD_CONTACT_RELATION:
				ret = getRelationAttribute(getFieldIntValue(names, infos, Relation.TYPE));
				if (getFieldIntValue(names, infos, Relation.IS_PRIMARY) != 0)
					ret |= MA_PIM_ATTRPREFERRED;
				break;
		}

		return ret;
	}

	int setAttribute(int index, int attribute)
	{
		int attrNativeValue = 0;
		String[] names = mStrNames;
		String[] infos = mStrInfos.get(index);

		DebugPrint("TYPE: " + getMoSyncType());

		switch ( getMoSyncType() )
		{
			case MA_PIM_FIELD_CONTACT_ADDR:
				if ( (attribute | MA_PIM_ATTRPREFERRED) != 0 )
				{
					setFieldValue( names, infos, StructuredPostal.IS_PRIMARY, Integer.toString(attribute) );
				}
				attribute &= 0xFFFF;
				attrNativeValue = getAddressNativeAttribute(attribute);
				if (attrNativeValue > 0)
				setFieldValue( names, infos, StructuredPostal.TYPE, Integer.toString(attrNativeValue) );
				break;
			case MA_PIM_FIELD_CONTACT_FORMATTED_ADDR:
				if ( (attribute | MA_PIM_ATTRPREFERRED) != 0 )
				{
					setFieldValue( names, infos, StructuredPostal.IS_PRIMARY, Integer.toString(attribute) );
				}
				attribute &= 0xFFFF;
				attrNativeValue = getFormattedAddressNativeAttribute(attribute);
				if (attrNativeValue > 0)
				setFieldValue( names, infos, StructuredPostal.TYPE, Integer.toString(attrNativeValue) );
				break;
			case MA_PIM_FIELD_CONTACT_EMAIL:
				if ( (attribute | MA_PIM_ATTRPREFERRED) != 0 )
				{
					setFieldValue( names, infos, Email.IS_PRIMARY, Integer.toString(attribute) );
				}
				attribute &= 0xFFFF;
				attrNativeValue = getEmailNativeAttribute(attribute);
				if (attrNativeValue > 0)
				setFieldValue( names, infos, Email.TYPE, Integer.toString(attrNativeValue) );
				break;
			case MA_PIM_FIELD_CONTACT_ORG:
				if ( (attribute | MA_PIM_ATTRPREFERRED) != 0 )
				{
					setFieldValue( names, infos, Organization.IS_PRIMARY, Integer.toString(attribute) );
				}
				attribute &= 0xFFFF;
				DebugPrint("Organization attribute = " + attribute);
				attrNativeValue = getOrganizationNativeAttribute(attribute);
				if (attrNativeValue > 0)
				setFieldValue( names, infos, Organization.TYPE, Integer.toString(attrNativeValue) );
				break;
			case MA_PIM_FIELD_CONTACT_TITLE:
				if ( (attribute | MA_PIM_ATTRPREFERRED) != 0 )
				{
					setFieldValue( names, infos, Organization.IS_PRIMARY, Integer.toString(attribute) );
				}
				attribute &= 0xFFFF;
				attrNativeValue = getTitleNativeAttribute(attribute);
				if (attrNativeValue > 0)
				setFieldValue( names, infos, Organization.TYPE, Integer.toString(attrNativeValue) );
				break;
			case MA_PIM_FIELD_CONTACT_ORG_INFO:
				if ( (attribute | MA_PIM_ATTRPREFERRED) != 0 )
				{
					setFieldValue( names, infos, Organization.IS_PRIMARY, Integer.toString(attribute) );
				}
				attribute &= 0xFFFF;
				attrNativeValue = getOrganizationInfoNativeAttribute(attribute);
				if (attrNativeValue > 0)
				setFieldValue( names, infos, Organization.TYPE, Integer.toString(attrNativeValue) );
				break;
			case MA_PIM_FIELD_CONTACT_TEL:
				if ( (attribute | MA_PIM_ATTRPREFERRED) != 0 )
				{
					setFieldValue( names, infos, Phone.IS_PRIMARY, Integer.toString(attribute) );
				}
				attribute &= 0xFFFF;
				attrNativeValue = getPhoneNativeAttribute(attribute);
				if (attrNativeValue > 0)
				setFieldValue( names, infos, Phone.TYPE, Integer.toString(attrNativeValue) );
				break;
			case MA_PIM_FIELD_CONTACT_URL:
				if ( (attribute | MA_PIM_ATTRPREFERRED) != 0 )
				{
					setFieldValue( names, infos, Website.IS_PRIMARY, Integer.toString(attribute) );
				}
				attribute &= 0xFFFF;
				attrNativeValue = getWebsiteNativeAttribute(attribute);
				if (attrNativeValue > 0)
				setFieldValue( names, infos, Website.TYPE, Integer.toString(attrNativeValue) );
				break;
			case MA_PIM_FIELD_CONTACT_IM:
				if ( (attribute | MA_PIM_ATTRPREFERRED) != 0 )
				{
					setFieldValue( names, infos, Im.IS_PRIMARY, Integer.toString(attribute) );
				}
				attribute &= 0xFFFF;
				attrNativeValue = getImNativeAttribute(attribute);
				if (attrNativeValue > 0)
				setFieldValue( names, infos, Im.TYPE, Integer.toString(attrNativeValue) );
				break;
			case MA_PIM_FIELD_CONTACT_RELATION:
				if ( (attribute | MA_PIM_ATTRPREFERRED) != 0 )
				{
					setFieldValue( names, infos, Relation.IS_PRIMARY, Integer.toString(attribute) );
				}
				attribute &= 0xFFFF;
				attrNativeValue = getRelationNativeAttribute(attribute);
				if (attrNativeValue > 0)
				setFieldValue( names, infos, Relation.TYPE, Integer.toString(attrNativeValue) );
				break;
		}
		return attrNativeValue;
	}

	public String getCustomLabel(int index)
	{
		String ret = null;
		String[] names = mStrNames;
		String[] infos = mStrInfos.get(index);

		switch ( getMoSyncType() )
		{
			case MA_PIM_FIELD_CONTACT_ADDR:
			case MA_PIM_FIELD_CONTACT_FORMATTED_ADDR:
				if ( getFieldIntValue(names, infos, StructuredPostal.TYPE) == StructuredPostal.TYPE_CUSTOM )
					ret = getFieldValue(names, infos, StructuredPostal.LABEL);
				break;
			case MA_PIM_FIELD_CONTACT_EMAIL:
				if ( getFieldIntValue(names, infos, Email.TYPE) == Email.TYPE_CUSTOM )
					ret = getFieldValue(names, infos, Email.LABEL);
				break;
			case MA_PIM_FIELD_CONTACT_BIRTHDAY:
				if ( getFieldIntValue(names, infos, Event.TYPE) == Event.TYPE_CUSTOM )
					ret = getFieldValue(names, infos, Event.LABEL);
				break;
			case MA_PIM_FIELD_CONTACT_ORG:
			case MA_PIM_FIELD_CONTACT_TITLE:
			case MA_PIM_FIELD_CONTACT_ORG_INFO:
				if ( getFieldIntValue(names, infos, Organization.TYPE) == Organization.TYPE_CUSTOM )
					ret = getFieldValue(names, infos, Organization.LABEL);
				break;
			case MA_PIM_FIELD_CONTACT_TEL:
				if ( getFieldIntValue(names, infos, Phone.TYPE) == Phone.TYPE_CUSTOM )
					ret = getFieldValue(names, infos, Phone.LABEL);
				break;
			case MA_PIM_FIELD_CONTACT_URL:
				if ( getFieldIntValue(names, infos, Website.TYPE) == Website.TYPE_CUSTOM )
					ret = getFieldValue(names, infos, Website.LABEL);
				break;
			case MA_PIM_FIELD_CONTACT_IM:
				if ( getFieldIntValue(names, infos, Im.TYPE) == Im.TYPE_CUSTOM )
					ret = getFieldValue(names, infos, Im.LABEL);
				break;
			case MA_PIM_FIELD_CONTACT_RELATION:
				if ( getFieldIntValue(names, infos, Relation.TYPE) == Relation.TYPE_CUSTOM )
					ret = getFieldValue(names, infos, Relation.LABEL);
				break;
		}

		return ret;
	}

	public void setCustomLabel(int index, String label)
	{
		setState(index, State.UPDATED);
		String[] names = mStrNames;
		String[] infos = mStrInfos.get(index);

		switch ( getMoSyncType() )
		{
			case MA_PIM_FIELD_CONTACT_ADDR:
			case MA_PIM_FIELD_CONTACT_FORMATTED_ADDR:
				setFieldValue(names, infos, StructuredPostal.LABEL, label);
				break;
			case MA_PIM_FIELD_CONTACT_EMAIL:
				setFieldValue(names, infos, Email.LABEL, label);
				break;
			case MA_PIM_FIELD_CONTACT_ORG:
			case MA_PIM_FIELD_CONTACT_TITLE:
			case MA_PIM_FIELD_CONTACT_ORG_INFO:
				setFieldValue(names, infos, Organization.LABEL, label);
				break;
			case MA_PIM_FIELD_CONTACT_TEL:
				setFieldValue(names, infos, Phone.LABEL, label);
				break;
			case MA_PIM_FIELD_CONTACT_URL:
				setFieldValue(names, infos, Website.LABEL, label);
				break;
			case MA_PIM_FIELD_CONTACT_IM:
				setFieldValue(names, infos, Im.LABEL, label);
				break;
			case MA_PIM_FIELD_CONTACT_RELATION:
				setFieldValue(names, infos, Relation.LABEL, label);
				break;
		}
	}

	static int getDataType(int fieldType)
	{
		switch (fieldType)
		{
			case MA_PIM_FIELD_CONTACT_ADDR:
				return MA_PIM_TYPE_STRING_ARRAY;
			case MA_PIM_FIELD_CONTACT_BIRTHDAY:
				return MA_PIM_TYPE_DATE;
			case MA_PIM_FIELD_CONTACT_CLASS:
				return MA_PIM_TYPE_INT;
			case MA_PIM_FIELD_CONTACT_EMAIL:
				return MA_PIM_TYPE_STRING;
			case MA_PIM_FIELD_CONTACT_FORMATTED_ADDR:
				return MA_PIM_TYPE_STRING;
			case MA_PIM_FIELD_CONTACT_FORMATTED_NAME:
				return MA_PIM_TYPE_STRING;
			case MA_PIM_FIELD_CONTACT_NAME:
				return MA_PIM_TYPE_STRING_ARRAY;
			case MA_PIM_FIELD_CONTACT_NICKNAME:
				return MA_PIM_TYPE_STRING;
			case MA_PIM_FIELD_CONTACT_NOTE:
				return MA_PIM_TYPE_STRING;
			case MA_PIM_FIELD_CONTACT_ORG:
				return MA_PIM_TYPE_STRING;
			case MA_PIM_FIELD_CONTACT_PHOTO:
				return MA_PIM_TYPE_INT;
			case MA_PIM_FIELD_CONTACT_PHOTO_URL:
				return MA_PIM_TYPE_STRING;
			case MA_PIM_FIELD_CONTACT_PUBLIC_KEY:
				return MA_PIM_TYPE_BINARY;
			case MA_PIM_FIELD_CONTACT_PUBLIC_KEY_STRING:
				return MA_PIM_TYPE_STRING;
			case MA_PIM_FIELD_CONTACT_REVISION:
				return MA_PIM_TYPE_DATE;
			case MA_PIM_FIELD_CONTACT_TEL:
				return MA_PIM_TYPE_STRING;
			case MA_PIM_FIELD_CONTACT_TITLE:
				return MA_PIM_TYPE_STRING;
			case MA_PIM_FIELD_CONTACT_UID:
				return MA_PIM_TYPE_STRING;
			case MA_PIM_FIELD_CONTACT_URL:
				return MA_PIM_TYPE_STRING;
			case MA_PIM_FIELD_CONTACT_IM:
				return MA_PIM_TYPE_STRING_ARRAY;
			case MA_PIM_FIELD_CONTACT_RELATION:
				return MA_PIM_TYPE_STRING;
			case MA_PIM_FIELD_CONTACT_ORG_INFO:
				return MA_PIM_TYPE_STRING_ARRAY;
			default:
				return -1;
		}
	}

	void writeInt(int val, char[] buffer, int index)
	{
		buffer[index++] = (char)(val & 0xFFFF);
		buffer[index++] = (char)((val >> 16) & 0xFFFF);
	}

	int readInt(char[] buffer, int index)
	{
		return (buffer[index] | (buffer[index + 1] << 16));
	}

	int writeStringArray(String[] names, String[] infos, char[] buffer)
	{
		int retIndex = 0;
		int length = 0;
		switch ( getMoSyncType() )
		{
			case MA_PIM_FIELD_CONTACT_ADDR:
			case MA_PIM_FIELD_CONTACT_BIRTHDAY:
			case MA_PIM_FIELD_CONTACT_EMAIL:
			case MA_PIM_FIELD_CONTACT_FORMATTED_ADDR:
			case MA_PIM_FIELD_CONTACT_ORG:
			case MA_PIM_FIELD_CONTACT_TEL:
			case MA_PIM_FIELD_CONTACT_TITLE:
			case MA_PIM_FIELD_CONTACT_URL:
			case MA_PIM_FIELD_CONTACT_IM:
			case MA_PIM_FIELD_CONTACT_RELATION:
			case MA_PIM_FIELD_CONTACT_ORG_INFO:
				length = names.length - 4;
				break;
			case MA_PIM_FIELD_CONTACT_NAME:
			case MA_PIM_FIELD_CONTACT_NICKNAME:
			case MA_PIM_FIELD_CONTACT_NOTE:
			case MA_PIM_FIELD_CONTACT_PHOTO:
			case MA_PIM_FIELD_CONTACT_UID:
				length = names.length - 2;
				break;
		}
		writeInt(length, buffer, retIndex);
		retIndex += 2;
		for (int i=1; i<length + 1; i++)
		{
			DebugPrint(names[i] + ": " + infos[i]);
			if ( (infos[i] != null) && (names[i] != PIM.DUMMY) )
			{
				if ((getMoSyncType() == MA_PIM_FIELD_CONTACT_IM) && (names[i] == Im.PROTOCOL))
				{
					String protocolName = getProtocolName(infos[i]);
					System.arraycopy(protocolName.toCharArray(), 0, buffer, retIndex, protocolName.length());
					retIndex += protocolName.length();
				}
				else
				{
					System.arraycopy(infos[i].toCharArray(), 0, buffer, retIndex, infos[i].length());
					retIndex += infos[i].length();
				}
			}
			buffer[retIndex++] = 0;
		}
		return retIndex;
	}

	String getProtocolName(String value)
	{
		int val = Integer.parseInt(value);
		if (!mProtocols.containsKey(val))
			return "OTHER";
		return mProtocols.get(val);
	}

	String getProtocolID(String name)
	{
		Integer id = ((Integer)getKeyFromValue(mProtocols, name)).intValue();
		if (id == null)
			return Integer.toString(Im.PROTOCOL_CUSTOM);
		return id.toString();
	}

	void readStringArray(String[] names, String[] infos, char[] buffer)
	{
		int buffIndex = 0;
		int length = readInt(buffer, buffIndex);
		DebugPrint("LENGTH = " + length);
		buffIndex += 2;
		for (int i=1; i<length + 1; i++)
		{
			int crtIndex = buffIndex;
			while (buffer[crtIndex++] != 0);
			crtIndex--;
			int len = crtIndex - buffIndex;
			char[] tmp = new char[len];
			System.arraycopy(buffer, buffIndex, tmp, 0, len);
			infos[i] = new String(tmp);
			DebugPrint("INFOS[" + i + "]=" + infos[i]);
			tmp = null;
			buffIndex += infos[i].length();
			buffIndex++;
		}
	}

	int getData(int index, char[] buffer)
	{
		String[] names = mStrNames;
		String[] infos = mStrInfos.get(index);

		switch (getDataType(getMoSyncType()))
		{
			case MA_PIM_TYPE_INT:
			{
				DebugPrint("INT DATA " + infos[1]);
				int data = Integer.parseInt(infos[1]);
				writeInt(data, buffer, 0);
				return 2;
			}
			case MA_PIM_TYPE_DATE:
			{
				DebugPrint("DATE DATA " + infos[1]);
				SimpleDateFormat curFormater = new SimpleDateFormat("yyyy-MM-dd");
				Date dateObj = null;
				try
				{
					dateObj = curFormater.parse(infos[1]);
				}
				catch (ParseException e)
				{
					e.printStackTrace();
					return 0;
				}
				int date = (int)dateObj.getTime();
				DebugPrint("WriteInt " + date);
				writeInt(date, buffer, 0);
				return 2;
			}
			case MA_PIM_TYPE_STRING:
			{
				DebugPrint("STRING DATA " + infos[1]);
				System.arraycopy(infos[1].toCharArray(), 0, buffer, 0, infos[1].length());
				return infos[1].length();
			}
			case MA_PIM_TYPE_STRING_ARRAY:
			{
				DebugPrint("STRING ARRAY DATA " + infos.length);
				for (int i=0; i<infos.length; i++)
					if (infos[i] != null)
					{
						DebugPrint( i + ":" + infos[i]);
					}
				return writeStringArray(names, infos, buffer);
			}
			default:
			{
				return 0;
			}
		}
	}

	int setData(int index, char[] buffer)
		{
		setState(index, State.UPDATED);
		String[] names = mStrNames;
		String[] infos = mStrInfos.get(index);

		switch (getDataType(getMoSyncType()))
			{
			case MA_PIM_TYPE_INT:
			{
				infos[1] = Integer.toString(readInt(buffer, 0));
				DebugPrint("INFOS = " + infos[1]);
				break;
			}
			case MA_PIM_TYPE_DATE:
			{
				int data = readInt(buffer, 0);
				SimpleDateFormat curFormater = new SimpleDateFormat("yyyy-MM-dd");
				Date dateObj = new Date();
				dateObj.setTime(data);
				infos[1] = curFormater.format(dateObj);
				DebugPrint("INFOS = " + infos[1]);
				break;
			}
			case MA_PIM_TYPE_STRING:
			{
				int crtIndex = 0;
				while (buffer[crtIndex++] != 0);
				crtIndex--;
				int len = crtIndex;
				char[] tmp = new char[len];
				System.arraycopy(buffer, 0, tmp, 0, len);
				infos[1] = new String(tmp);
				if (getMoSyncType() == MA_PIM_FIELD_CONTACT_PHOTO_URL)
				{
					infos[1] = loadPhoto(infos[1]);
		}
				DebugPrint("INFOS = " + infos[1]);
				break;
			}
			case MA_PIM_TYPE_STRING_ARRAY:
			{
				readStringArray(names, infos, buffer);
				break;
			}
			default:
			{
				return -1;
			}
		}
		return 0;
	}

	int addData(char[] buffer)
	{
		String[] names = mStrNames;
		if (names == null)
			throw new BigPhatError("PIMField.addData");
		String[] infos = new String[names.length];
		mStrInfos.add(infos);
		int index = mStrInfos.size() - 1;
		setState(index, State.ADDED);
		return index;
	}

	String getId(int index)
	{
		String[] infos = mStrInfos.get(index);
		return infos[0];
	}

	boolean hasCustomLabel(int index)
	{
		String[] names = mStrNames;
		String[] infos = mStrInfos.get(index);

		int type = getMoSyncType();
		switch (type)
		{
			case MA_PIM_FIELD_CONTACT_BIRTHDAY:
			case MA_PIM_FIELD_CONTACT_NAME:
			case MA_PIM_FIELD_CONTACT_NICKNAME:
			case MA_PIM_FIELD_CONTACT_NOTE:
			case MA_PIM_FIELD_CONTACT_PHOTO:
			case MA_PIM_FIELD_CONTACT_PHOTO_URL:
				return false;
			case MA_PIM_FIELD_CONTACT_ADDR:
			case MA_PIM_FIELD_CONTACT_FORMATTED_ADDR:
				if ( getFieldIntValue(names, infos, StructuredPostal.TYPE) != StructuredPostal.TYPE_CUSTOM )
					return false;
				break;
			case MA_PIM_FIELD_CONTACT_EMAIL:
				if ( getFieldIntValue(names, infos, Email.TYPE) != Email.TYPE_CUSTOM )
					return false;
				break;
			case MA_PIM_FIELD_CONTACT_ORG:
			case MA_PIM_FIELD_CONTACT_TITLE:
			case MA_PIM_FIELD_CONTACT_ORG_INFO:
				if ( getFieldIntValue(names, infos, Organization.TYPE) != Organization.TYPE_CUSTOM )
					return false;
				break;
			case MA_PIM_FIELD_CONTACT_TEL:
				if ( getFieldIntValue(names, infos, Phone.TYPE) != Phone.TYPE_CUSTOM )
					return false;
				break;
			case MA_PIM_FIELD_CONTACT_URL:
				if ( getFieldIntValue(names, infos, Website.TYPE) != Website.TYPE_CUSTOM )
					return false;
				break;
			case MA_PIM_FIELD_CONTACT_IM:
				if ( getFieldIntValue(names, infos, Im.TYPE) != Im.TYPE_CUSTOM )
					return false;
				break;
			case MA_PIM_FIELD_CONTACT_RELATION:
				if ( getFieldIntValue(names, infos, Relation.TYPE) != Relation.TYPE_CUSTOM )
					return false;
				break;
}
		return true;
	}

	boolean isReadOnly()
	{
		int type = getMoSyncType();
		if ((type == MA_PIM_FIELD_CONTACT_UID) ||
			(type == MA_PIM_FIELD_CONTACT_REVISION))
		{
			return true;
		}
		return false;
	}

	boolean isWriteOnly()
	{
		if (getMoSyncType() == MA_PIM_FIELD_CONTACT_PHOTO_URL)
		{
			return true;
		}
		return false;
	}

	void setState(int index, State state)
	{
		if (state == State.ADDED)
		{
			mState.add(State.ADDED);
		}
		else if ((mState.get(index) == State.NONE) ||
			(mState.get(index) == State.UPDATED))
		{
			mState.set(index, state);
		}
	}

	void close()
	{
//		for (int i=0; i<mState.size(); i++)
//		{
//			if (mState.get(i) == State.ADDED)
//			{
//				add(mStrNames, mStrInfos.get(i));
//			}
//			else if (mState.get(i) == State.UPDATED)
//			{
//				update(mStrNames, mStrInfos.get(i));
//			}
//			else if (mState.get(i) == State.DELETED)
//			{
//				delete();
//			}
//		}
	}

	void add()
	{
//		ArrayList<ContentProviderOperation> ops =
//			new ArrayList<ContentProviderOperation>();
//
//		ops.add(ContentProviderOperation.newInsert(Data.CONTENT_URI)
//				.withValue(Data.RAW_CONTACT_ID, rawContactId)
//				.withValue(Data.MIMETYPE, Phone.CONTENT_ITEM_TYPE)
//				.withValue(Phone.NUMBER, "            1-800-GOOG-411      ")
//				.withValue(Phone.TYPE, Phone.TYPE_CUSTOM)
//				.withValue(Phone.LABEL, "free directory assistance")
//				.build());
//		getContentResolver().applyBatch(ContactsContract.AUTHORITY, ops);
	}

	void update()
	{
//		ArrayList<ContentProviderOperation> ops =
//			new ArrayList<ContentProviderOperation>();
//
//		ops.add(ContentProviderOperation.newUpdate(Data.CONTENT_URI)
//				.withSelection(Data._ID + "=?", new String[]{String.valueOf(dataId)})
//				.withValue(Email.DATA, "somebody@android.com")
//				.build());
//		getContentResolver().applyBatch(ContactsContract.AUTHORITY, ops);
	}

	void delete()
	{
//		ArrayList<ContentProviderOperation> ops =
//			new ArrayList<ContentProviderOperation>();
//
//		ops.add(ContentProviderOperation.newDelete(Data.CONTENT_URI)
//				.withSelection(Data._ID + "=?", new String[]{String.valueOf(dataId)})
//				.build());
//		getContentResolver().applyBatch(ContactsContract.AUTHORITY, ops);
	}
}
