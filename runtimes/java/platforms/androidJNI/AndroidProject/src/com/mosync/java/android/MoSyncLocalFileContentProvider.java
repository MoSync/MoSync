// This class is commented out, because the content provider mechanism is
// not used anymore. It was intended to be used for fetching local files
// from a web view, but this is done using a file:// schema instead.
// Keeping this code as a reference, in case it should be needed. Micke :)

///* Copyright (C) 2011 MoSync AB
//
//This program is free software; you can redistribute it and/or modify it under
//the terms of the GNU General Public License, version 2, as published by
//the Free Software Foundation.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
//or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
//for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; see the file COPYING.  If not, write to the Free
//Software Foundation, 59 Temple Place - Suite 330, Boston, MA
//02111-1307, USA.
//*/
//
//package com.mosync.java.android;
//
//import java.io.File;
//import java.io.FileNotFoundException;
//
//import android.content.ContentProvider;
//import android.content.ContentValues;
//import android.database.Cursor;
//import android.net.Uri;
//import android.os.ParcelFileDescriptor;
//import android.util.Log;
//
//import com.mosync.internal.android.MoSyncThread;
//
///**
// * Content provider that is used by web views to get access to files 
// * stored in the application's local file system.
// * 
// * The content provider is referenced in three places:
// * - In this file (the content provider class)
// * - In the Android manifest file, created in file:
// *    Eclipse/com.mobilesorcery.sdk.builder.android/src/com/mobilesorcery/sdk/builder/android/AndroidPackager.java
// * - In the WebWidget class (base url used is "content://" 
// *   followed by the application's package name):
// *    MoSync/runtimes/java/platforms/androidJNI/AndroidProject/src/com/mosync/nativeui/ui/widgets/WebWidget.java
// *    
// * This class is based on the following article, which contains info for 
// * creating a content provider for use from a WebView:
// * http://www.techjini.com/blog/2009/01/10/android-tip-1-contentprovider-accessing-local-file-system-from-webview-showing-image-in-webview-using-content/
// * 
// * Note that each application must provide a unique authority for the
// * content provider. The application's package name is used for this.
// * See e.g. this page for more info: 
// * http://stackoverflow.com/questions/6089195/how-to-use-same-contentprovider-authority-in-free-and-paid-versions-of-app
// * 
// * @author Mikael Kindborg
// */
//public class MoSyncLocalFileContentProvider extends ContentProvider
//{
//	/**
//	 * Map a file request to a file in the application's local
//	 * (private) file system.
//	 */
//	@Override
//	public ParcelFileDescriptor openFile(Uri uri, String mode)
//		throws FileNotFoundException
//	{
//		Log.i("@@@", "uri.getPath(): " + uri.getPath());
//		
//		File privateDir = MoSyncThread.getInstance().getActivity().getFilesDir();
//		Log.i("@@@@@@ Path privateDir ", privateDir.getAbsolutePath());
//		
//		//URI fileURI = URI.create("file://data/data/mosync.dynamicwebview/files" + uri.getPath());
//		//File file = new File(fileURI);
//		File file = MoSyncThread.getInstance().getActivity().getFileStreamPath(
//			uri.getPath().substring(1));
//		return ParcelFileDescriptor.open(
//			file,
//			ParcelFileDescriptor.MODE_READ_ONLY);
//	}
//	
//	/**
//	 * Implementation of abstract method in class ContentProvider.
//	 */
//	@Override
//	public boolean onCreate()
//	{
//		return true;
//	}
//
//	/**
//	 * Implementation of abstract method in class ContentProvider.
//	 */
//	@Override
//	public int delete(Uri uri, String s, String[] as)
//	{
//		throw new UnsupportedOperationException(
//			"delete is not supported by MoSyncLocalFileContentProvider");
//	}
//
//	/**
//	 * Implementation of abstract method in class ContentProvider.
//	 */
//	@Override
//	public String getType(Uri uri)
//	{
//		throw new UnsupportedOperationException(
//			"getType is not supported by MoSyncLocalFileContentProvider");
//	}
//
//	/**
//	 * Implementation of abstract method in class ContentProvider.
//	 */
//	@Override
//	public Uri insert(Uri uri, ContentValues contentvalues)
//	{
//		throw new UnsupportedOperationException(
//			"insert is not supported by MoSyncLocalFileContentProvider");
//	}
//
//	/**
//	 * Implementation of abstract method in class ContentProvider.
//	 */
//	@Override
//	public Cursor query(
//		Uri uri, 
//		String[] as, 
//		String s, 
//		String[] as1, 
//		String s1)
//	{
//		throw new UnsupportedOperationException(
//			"query is not supported by MoSyncLocalFileContentProvider");
//	}
//
//	/**
//	 * Implementation of abstract method in class ContentProvider.
//	 */
//	@Override
//	public int update(
//		Uri uri, 
//		ContentValues contentvalues, 
//		String s,
//		String[] as)
//	{
//		throw new UnsupportedOperationException(
//			"update is not supported by MoSyncLocalFileContentProvider");
//	}
//}
