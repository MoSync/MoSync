package com.mosync.pim;

import static com.mosync.internal.android.MoSyncHelpers.SYSLOG;
import static com.mosync.internal.android.MoSyncHelpers.DebugPrint;

import java.util.ArrayList;

import android.content.ContentValues;
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
import android.provider.ContactsContract.CommonDataKinds.StructuredName;
import android.provider.ContactsContract.CommonDataKinds.StructuredPostal;
import android.provider.ContactsContract.CommonDataKinds.Website;

public class PIMField
{
	// field types
	final static int MA_PIM_FIELD_CONTACT_ADDR = 100;
	final static int MA_PIM_FIELD_CONTACT_BIRTHDAY = 101;
	final static int MA_PIM_FIELD_CONTACT_CLASS = 102;
	final static int MA_PIM_FIELD_CONTACT_EMAIL = 103;
	final static int MA_PIM_FIELD_CONTACT_FORMATTED_ADDR = 104;
	final static int MA_PIM_FIELD_CONTACT_FORMATTED_NAME = 105;
	final static int MA_PIM_FIELD_CONTACT_NAME = 106;
	final static int MA_PIM_FIELD_CONTACT_NICKNAME = 107;
	final static int MA_PIM_FIELD_CONTACT_NOTE = 108;
	final static int MA_PIM_FIELD_CONTACT_ORG = 109;
	final static int MA_PIM_FIELD_CONTACT_PHOTO = 110;
	final static int MA_PIM_FIELD_CONTACT_PHOTO_URL = 111;
	final static int MA_PIM_FIELD_CONTACT_PUBLIC_KEY = 112;
	final static int MA_PIM_FIELD_CONTACT_PUBLIC_KEY_STRING = 113;
	final static int MA_PIM_FIELD_CONTACT_REVISION = 114;
	final static int MA_PIM_FIELD_CONTACT_TEL = 115;
	final static int MA_PIM_FIELD_CONTACT_TITLE = 116;
	final static int MA_PIM_FIELD_CONTACT_UID = 117;
	final static int MA_PIM_FIELD_CONTACT_URL = 118;
	final static int MA_PIM_FIELD_CONTACT_IM = 119;
	final static int MA_PIM_FIELD_CONTACT_RELATION = 120;
	final static int MA_PIM_FIELD_CONTACT_ORG_INFO = 121;

	//address field attributes
	final static int MA_PIM_ATTR_ADDR_HOME = 101;
	final static int MA_PIM_ATTR_ADDR_WORK = 102;
	final static int MA_PIM_ATTR_ADDR_OTHER = 103;
	final static int MA_PIM_ATTR_ADDR_CUSTOM = 104;

	//email field attributes
	final static int MA_PIM_ATTR_EMAIL_HOME = 201;
	final static int MA_PIM_ATTR_EMAIL_WORK = 202;
	final static int MA_PIM_ATTR_EMAIL_MOBILE = 203;
	final static int MA_PIM_ATTR_EMAIL_OTHER = 204;
	final static int MA_PIM_ATTR_EMAIL_CUSTOM = 205;

	//formatted address field attributes
	final static int MA_PIM_ATTR_FORMATTED_ADDR_HOME = 301;
	final static int MA_PIM_ATTR_FORMATTED_ADDR_WORK = 302;
	final static int MA_PIM_ATTR_FORMATTED_ADDR_OTHER = 303;
	final static int MA_PIM_ATTR_FORMATTED_ADDR_CUSTOM = 304;

	//organization field attributes
	final static int MA_PIM_ATTR_ORG_WORK = 401;
	final static int MA_PIM_ATTR_ORG_OTHER = 402;
	final static int MA_PIM_ATTR_ORG_CUSTOM = 403;

	//phone field attributes
	final static int MA_PIM_ATTR_PHONE_HOME = 501;
	final static int MA_PIM_ATTR_PHONE_MOBILE = 502;
	final static int MA_PIM_ATTR_PHONE_HOME_FAX = 503;
	final static int MA_PIM_ATTR_PHONE_WORK_FAX = 504;
	final static int MA_PIM_ATTR_PHONE_PAGER = 505;
	final static int MA_PIM_ATTR_PHONE_IPHONE = 506;
	final static int MA_PIM_ATTR_PHONE_WORK = 507;
	final static int MA_PIM_ATTR_PHONE_CALLBACK = 508;
	final static int MA_PIM_ATTR_PHONE_CAR = 509;
	final static int MA_PIM_ATTR_PHONE_COMPANY_MAIN = 510;
	final static int MA_PIM_ATTR_PHONE_ISDN = 511;
	final static int MA_PIM_ATTR_PHONE_OTHER_FAX = 512;
	final static int MA_PIM_ATTR_PHONE_RADIO = 513;
	final static int MA_PIM_ATTR_PHONE_TELEX = 514;
	final static int MA_PIM_ATTR_PHONE_TTY_TDD = 515;
	final static int MA_PIM_ATTR_PHONE_WORK_MOBILE = 516;
	final static int MA_PIM_ATTR_PHONE_WORK_PAGER = 517;
	final static int MA_PIM_ATTR_PHONE_ASSISTANT = 518;
	final static int MA_PIM_ATTR_PHONE_MMS = 519;
	final static int MA_PIM_ATTR_PHONE_OTHER = 520;
	final static int MA_PIM_ATTR_PHONE_CUSTOM = 521;

	//title field attributes
	final static int MA_PIM_ATTR_TITLE_WORK = 601;
	final static int MA_PIM_ATTR_TITLE_OTHER = 602;
	final static int MA_PIM_ATTR_TITLE_CUSTOM = 603;

	//website field attributes
	final static int MA_PIM_ATTR_WEBSITE_HOMEPAGE = 701;
	final static int MA_PIM_ATTR_WEBSITE_BLOG = 702;
	final static int MA_PIM_ATTR_WEBSITE_PROFILE = 703;
	final static int MA_PIM_ATTR_WEBSITE_HOME = 704;
	final static int MA_PIM_ATTR_WEBSITE_WORK = 705;
	final static int MA_PIM_ATTR_WEBSITE_FTP = 706;
	final static int MA_PIM_ATTR_WEBSITE_OTHER = 707;
	final static int MA_PIM_ATTR_WEBSITE_CUSTOM = 708;

	//im field attributes
	final static int MA_PIM_ATTR_IM_HOME = 801;
	final static int MA_PIM_ATTR_IM_WORK = 802;
	final static int MA_PIM_ATTR_IM_OTHER = 803;
	final static int MA_PIM_ATTR_IM_CUSTOM = 804;

	//relation field attributes
	final static int MA_PIM_ATTR_RELATION_MOTHER = 901;
	final static int MA_PIM_ATTR_RELATION_FATHER = 902;
	final static int MA_PIM_ATTR_RELATION_PARENT = 903;
	final static int MA_PIM_ATTR_RELATION_SISTER = 904;
	final static int MA_PIM_ATTR_RELATION_BROTHER = 905;
	final static int MA_PIM_ATTR_RELATION_CHILD = 906;
	final static int MA_PIM_ATTR_RELATION_FRIEND = 907;
	final static int MA_PIM_ATTR_RELATION_SPOUSE = 908;
	final static int MA_PIM_ATTR_RELATION_PARTNER = 909;
	final static int MA_PIM_ATTR_RELATION_MANAGER = 910;
	final static int MA_PIM_ATTR_RELATION_ASSISTANT = 911;
	final static int MA_PIM_ATTR_RELATION_DOMESTIC_PARTNER = 912;
	final static int MA_PIM_ATTR_RELATION_REFERRED_BY = 913;
	final static int MA_PIM_ATTR_RELATION_RELATIVE = 914;
	final static int MA_PIM_ATTR_RELATION_CUSTOM = 915;

	//organization info field attributes
	final static int MA_PIM_ATTR_ORG_INFO_WORK = 1001;
	final static int MA_PIM_ATTR_ORG_INFO_OTHER = 1002;
	final static int MA_PIM_ATTR_ORG_INFO_CUSTOM = 1003;

	//preferred attribute
	final static int MA_PIM_ATTR_PREFERRED = 0x10000;

	ArrayList<String[]> mStrNames;
	ArrayList<String[]> mStrInfos;
	String mStrType;

	PIMField(String type)
	{
		mStrNames = new ArrayList<String[]>();
		mStrInfos = new ArrayList<String[]>();
		mStrType = type;
	}

	void add(String[] names, String[] infos)
	{
		for (int i=0; i<names.length; i++)
		{
			SYSLOG(names[i] + " = " + infos[i]);
		}

		mStrNames.add(names);
		mStrInfos.add(infos);
	}

	int length()
	{
		return mStrNames.size();
	}

	String getType()
	{
		return mStrType;
	}

	int getMoSyncType()
	{
		if ( mStrType.equals(StructuredPostal.CONTENT_ITEM_TYPE) )
		{
			if (!mStrNames.isEmpty())
			{
				if ( mStrNames.get(0)[1].equals(StructuredPostal.FORMATTED_ADDRESS) )
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
			if (!mStrNames.isEmpty())
			{
				if ( mStrNames.get(0)[1].equals(Organization.COMPANY) )
				{
					return MA_PIM_FIELD_CONTACT_ORG;
				}
				else if ( mStrNames.get(0)[1].equals(Organization.TITLE) )
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

	String getFieldValue(String[] names, String[] infos, String column, String value)
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
		switch (attr)
		{
			case StructuredPostal.TYPE_HOME:
				return MA_PIM_ATTR_ADDR_HOME;
			case StructuredPostal.TYPE_WORK:
				return MA_PIM_ATTR_ADDR_WORK;
			case StructuredPostal.TYPE_CUSTOM:
				return MA_PIM_ATTR_ADDR_CUSTOM;
			default:
				return MA_PIM_ATTR_ADDR_OTHER;
		}
	}

	int getEmailAttribute(int attr)
	{
		switch (attr)
		{
			case Email.TYPE_HOME:
				return MA_PIM_ATTR_EMAIL_HOME;
			case Email.TYPE_WORK:
				return MA_PIM_ATTR_EMAIL_WORK;
			case Email.TYPE_MOBILE:
				return MA_PIM_ATTR_EMAIL_MOBILE;
			case Email.TYPE_CUSTOM:
				return MA_PIM_ATTR_EMAIL_CUSTOM;
			default:
				return MA_PIM_ATTR_EMAIL_OTHER;
		}
	}

	int getFormattedAddressAttribute(int attr)
	{
		switch (attr)
		{
			case StructuredPostal.TYPE_HOME:
				return MA_PIM_ATTR_FORMATTED_ADDR_HOME;
			case StructuredPostal.TYPE_WORK:
				return MA_PIM_ATTR_FORMATTED_ADDR_WORK;
			case StructuredPostal.TYPE_CUSTOM:
				return MA_PIM_ATTR_FORMATTED_ADDR_CUSTOM;
			default:
				return MA_PIM_ATTR_FORMATTED_ADDR_OTHER;
		}
	}

	int getOrganizationAttribute(int attr)
	{
		switch (attr)
		{
			case Organization.TYPE_WORK:
				return MA_PIM_ATTR_ORG_WORK;
			case Organization.TYPE_CUSTOM:
				return MA_PIM_ATTR_ORG_CUSTOM;
			default:
				return MA_PIM_ATTR_ORG_OTHER;
		}
	}

	int getPhoneAttribute(int attr)
	{
		switch (attr)
		{
			case Phone.TYPE_CUSTOM:
				return MA_PIM_ATTR_PHONE_CUSTOM;
			case Phone.TYPE_HOME:
				return MA_PIM_ATTR_PHONE_HOME;
			case Phone.TYPE_MOBILE:
				return MA_PIM_ATTR_PHONE_MOBILE;
			case Phone.TYPE_WORK:
				return MA_PIM_ATTR_PHONE_WORK;
			case Phone.TYPE_FAX_WORK:
				return MA_PIM_ATTR_PHONE_WORK_FAX;
			case Phone.TYPE_FAX_HOME:
				return MA_PIM_ATTR_PHONE_HOME_FAX;
			case Phone.TYPE_PAGER:
				return MA_PIM_ATTR_PHONE_PAGER;
			case Phone.TYPE_CALLBACK:
				return MA_PIM_ATTR_PHONE_CALLBACK;
			case Phone.TYPE_CAR:
				return MA_PIM_ATTR_PHONE_CAR;
			case Phone.TYPE_COMPANY_MAIN:
				return MA_PIM_ATTR_PHONE_COMPANY_MAIN;
			case Phone.TYPE_ISDN:
				return MA_PIM_ATTR_PHONE_ISDN;
			case Phone.TYPE_OTHER_FAX:
				return MA_PIM_ATTR_PHONE_OTHER_FAX;
			case Phone.TYPE_RADIO:
				return MA_PIM_ATTR_PHONE_RADIO;
			case Phone.TYPE_TELEX:
				return MA_PIM_ATTR_PHONE_TELEX;
			case Phone.TYPE_TTY_TDD:
				return MA_PIM_ATTR_PHONE_TTY_TDD;
			case Phone.TYPE_WORK_MOBILE:
				return MA_PIM_ATTR_PHONE_MOBILE;
			case Phone.TYPE_WORK_PAGER:
				return MA_PIM_ATTR_PHONE_PAGER;
			case Phone.TYPE_ASSISTANT:
				return MA_PIM_ATTR_PHONE_ASSISTANT;
			case Phone.TYPE_MMS:
				return MA_PIM_ATTR_PHONE_MMS;
			case Phone.TYPE_OTHER:
			case Phone.TYPE_MAIN:
			default:
				return MA_PIM_ATTR_PHONE_OTHER;
		}
	}

	int getTitleAttribute(int attr)
	{
		switch (attr)
		{
			case Organization.TYPE_WORK:
				return MA_PIM_ATTR_TITLE_WORK;
			case Organization.TYPE_CUSTOM:
				return MA_PIM_ATTR_TITLE_CUSTOM;
			default:
				return MA_PIM_ATTR_TITLE_OTHER;
		}
	}

	int getOrganizationInfoAttribute(int attr)
	{
		switch (attr)
		{
			case Organization.TYPE_WORK:
				return MA_PIM_ATTR_ORG_INFO_WORK;
			case Organization.TYPE_CUSTOM:
				return MA_PIM_ATTR_ORG_INFO_CUSTOM;
			default:
				return MA_PIM_ATTR_ORG_INFO_OTHER;
		}
	}

	int getWebsiteAttribute(int attr)
	{
		switch (attr)
		{
			case Website.TYPE_HOMEPAGE:
				return MA_PIM_ATTR_WEBSITE_HOMEPAGE;
			case Website.TYPE_BLOG:
				return MA_PIM_ATTR_WEBSITE_BLOG;
			case Website.TYPE_PROFILE:
				return MA_PIM_ATTR_WEBSITE_PROFILE;
			case Website.TYPE_HOME:
				return MA_PIM_ATTR_WEBSITE_HOME;
			case Website.TYPE_WORK:
				return MA_PIM_ATTR_WEBSITE_WORK;
			case Website.TYPE_FTP:
				return MA_PIM_ATTR_WEBSITE_FTP;
			case Website.TYPE_CUSTOM:
				return MA_PIM_ATTR_WEBSITE_CUSTOM;
			default:
				return MA_PIM_ATTR_WEBSITE_OTHER;
		}
	}

	int getImAttribute(int attr)
	{
		switch (attr)
		{
			case Im.TYPE_HOME:
				return MA_PIM_ATTR_IM_HOME;
			case Im.TYPE_WORK:
				return MA_PIM_ATTR_IM_WORK;
			case Im.TYPE_CUSTOM:
				return MA_PIM_ATTR_IM_CUSTOM;
			default:
				return MA_PIM_ATTR_IM_OTHER;
		}
	}

	int getRelationAttribute(int attr)
	{
		switch (attr)
		{
			case Relation.TYPE_MOTHER:
				return MA_PIM_ATTR_RELATION_MOTHER;
			case Relation.TYPE_FATHER:
				return MA_PIM_ATTR_RELATION_FATHER;
			case Relation.TYPE_PARENT:
				return MA_PIM_ATTR_RELATION_PARENT;
			case Relation.TYPE_SISTER:
				return MA_PIM_ATTR_RELATION_SISTER;
			case Relation.TYPE_BROTHER:
				return MA_PIM_ATTR_RELATION_BROTHER;
			case Relation.TYPE_CHILD:
				return MA_PIM_ATTR_RELATION_CHILD;
			case Relation.TYPE_FRIEND:
				return MA_PIM_ATTR_RELATION_FRIEND;
			case Relation.TYPE_SPOUSE:
				return MA_PIM_ATTR_RELATION_SPOUSE;
			case Relation.TYPE_PARTNER:
				return MA_PIM_ATTR_RELATION_PARTNER;
			case Relation.TYPE_MANAGER:
				return MA_PIM_ATTR_RELATION_MANAGER;
			case Relation.TYPE_ASSISTANT:
				return MA_PIM_ATTR_RELATION_ASSISTANT;
			case Relation.TYPE_DOMESTIC_PARTNER:
				return MA_PIM_ATTR_RELATION_DOMESTIC_PARTNER;
			case Relation.TYPE_REFERRED_BY:
				return MA_PIM_ATTR_RELATION_REFERRED_BY;
			case Relation.TYPE_CUSTOM:
				return MA_PIM_ATTR_RELATION_CUSTOM;
			default:
				return MA_PIM_ATTR_RELATION_RELATIVE;
		}
	}

	int getAttribute(int index)
	{
		int ret = 0;
		String[] names = mStrNames.get(index);
		String[] infos = mStrInfos.get(index);

		DebugPrint("TYPE: " + getMoSyncType());

		switch ( getMoSyncType() )
		{
			case MA_PIM_FIELD_CONTACT_ADDR:
				ret = getAddressAttribute(getFieldIntValue(names, infos, StructuredPostal.TYPE));
				if (getFieldIntValue(names, infos, StructuredPostal.IS_PRIMARY) != 0)
					ret |= MA_PIM_ATTR_PREFERRED;
				break;
			case MA_PIM_FIELD_CONTACT_FORMATTED_ADDR:
				ret = getFormattedAddressAttribute(getFieldIntValue(names, infos, StructuredPostal.TYPE));
				if (getFieldIntValue(names, infos, StructuredPostal.IS_PRIMARY) != 0)
					ret |= MA_PIM_ATTR_PREFERRED;
				break;
			case MA_PIM_FIELD_CONTACT_EMAIL:
				ret = getEmailAttribute(getFieldIntValue(names, infos, Email.TYPE));
				if (getFieldIntValue(names, infos, Email.IS_PRIMARY) != 0)
					ret |= MA_PIM_ATTR_PREFERRED;
				break;
			case MA_PIM_FIELD_CONTACT_ORG:
				ret = getOrganizationAttribute(getFieldIntValue(names, infos, Organization.TYPE));
				if (getFieldIntValue(names, infos, Organization.IS_PRIMARY) != 0)
					ret |= MA_PIM_ATTR_PREFERRED;
				break;
			case MA_PIM_FIELD_CONTACT_TITLE:
				ret = getTitleAttribute(getFieldIntValue(names, infos, Organization.TYPE));
				if (getFieldIntValue(names, infos, Organization.IS_PRIMARY) != 0)
					ret |= MA_PIM_ATTR_PREFERRED;
				break;
			case MA_PIM_FIELD_CONTACT_ORG_INFO:
				ret = getOrganizationInfoAttribute(getFieldIntValue(names, infos, Organization.TYPE));
				if (getFieldIntValue(names, infos, Organization.IS_PRIMARY) != 0)
					ret |= MA_PIM_ATTR_PREFERRED;
				break;
			case MA_PIM_FIELD_CONTACT_TEL:
				ret = getPhoneAttribute(getFieldIntValue(names, infos, Phone.TYPE));
				if (getFieldIntValue(names, infos, Phone.IS_PRIMARY) != 0)
					ret |= MA_PIM_ATTR_PREFERRED;
				break;
			case MA_PIM_FIELD_CONTACT_URL:
				ret = getWebsiteAttribute(getFieldIntValue(names, infos, Website.TYPE));
				if (getFieldIntValue(names, infos, Website.IS_PRIMARY) != 0)
					ret |= MA_PIM_ATTR_PREFERRED;
				break;
			case MA_PIM_FIELD_CONTACT_IM:
				ret = getImAttribute(getFieldIntValue(names, infos, Im.TYPE));
				if (getFieldIntValue(names, infos, Im.IS_PRIMARY) != 0)
					ret |= MA_PIM_ATTR_PREFERRED;
				break;
			case MA_PIM_FIELD_CONTACT_RELATION:
				ret = getRelationAttribute(getFieldIntValue(names, infos, Relation.TYPE));
				if (getFieldIntValue(names, infos, Relation.IS_PRIMARY) != 0)
					ret |= MA_PIM_ATTR_PREFERRED;
				break;
		}

		return ret;
	}

	public String getCustomLabel(int index)
	{
		String ret = null;
		String[] names = mStrNames.get(index);
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

	public String setCustomLabel(int index, String label)
	{
		String ret = null;
		String[] names = mStrNames.get(index);
		String[] infos = mStrInfos.get(index);

		switch ( getMoSyncType() )
		{
			case MA_PIM_FIELD_CONTACT_ADDR:
			case MA_PIM_FIELD_CONTACT_FORMATTED_ADDR:
				//if ( getFieldIntValue(names, infos, StructuredPostal.TYPE) == StructuredPostal.TYPE_CUSTOM )
					//setFieldValue(names, infos, StructuredPostal.LABEL, label);
				break;
			case MA_PIM_FIELD_CONTACT_EMAIL:
				if ( getFieldIntValue(names, infos, Email.TYPE) == Email.TYPE_CUSTOM )
					ret = getFieldValue(names, infos, Email.LABEL);
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
}