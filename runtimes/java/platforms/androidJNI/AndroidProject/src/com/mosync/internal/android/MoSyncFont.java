/* Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

package com.mosync.internal.android;

//import static com.mosync.internal.android.MoSyncHelpers.SYSLOG;
import static com.mosync.internal.generated.MAAPI_consts.FONT_TYPE_MONOSPACE;
import static com.mosync.internal.generated.MAAPI_consts.FONT_TYPE_SANS_SERIF;
import static com.mosync.internal.generated.MAAPI_consts.FONT_TYPE_SERIF;
import static com.mosync.internal.generated.MAAPI_consts.RES_FONT_OK;
import static com.mosync.internal.generated.MAAPI_consts.RES_FONT_INVALID_HANDLE;
import static com.mosync.internal.generated.MAAPI_consts.RES_FONT_INDEX_OUT_OF_BOUNDS;
import static com.mosync.internal.generated.MAAPI_consts.RES_FONT_NO_TYPE_STYLE_COMBINATION;
import static com.mosync.internal.generated.MAAPI_consts.RES_FONT_NAME_NONEXISTENT;
import static com.mosync.internal.generated.MAAPI_consts.RES_FONT_LIST_NOT_INITIALIZED;
import static com.mosync.internal.generated.MAAPI_consts.RES_FONT_INSUFFICIENT_BUFFER;
import static com.mosync.internal.generated.MAAPI_consts.RES_FONT_INVALID_SIZE;
import static com.mosync.internal.generated.MAAPI_consts.RES_FONT_DELETE_DENIED;

import java.io.File;
import java.io.FilenameFilter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.List;

import android.content.res.AssetManager;
import android.graphics.Typeface;
import android.util.Log;

/**
 * Class that implements font syscalls.
 * Allows the user to load default fonts by specifying font family, style and size.
 * Allows the use to load custom true type fonts.
 * The list of font handles created here can be used for setting fonts in the NativeUI.
 *
 * @author emma tresanszki
 */
public class MoSyncFont
{
	/**
	 * Store the font location: the file system, or the assets folder,
	 * to know where to search it from.
	 */
	enum FontLocation{
		ASSETS, SYSTEM
	}

	/**
	 * The MoSync thread object.
	 */
	MoSyncThread mMoSyncThread;

	/**
	 * Internal handle for handling fonts.
	 * A font handle is created by the user via maFontLoadWithName or maFontLoadDefault.
	 * Each font is created just before usage, by providing the postscriptname/type and style, and size.
	 */
	public class MoSyncFontHandle
	{
		/*
		 * Constructor.
		 * A font is based on a typeface and size.
		 */
		public MoSyncFontHandle(Typeface aTypeface, float aSize)
		{
			mTypeface = aTypeface;
			mFontSize = aSize;
		}

		public Typeface getTypeface()
		{
			return mTypeface;
		}

		public float getFontSize()
		{
			return mFontSize;
		}

		/*
		 * The typeface of the font.
		 * It specifies the typeface and intrinsic style of a font.
		 */
		private Typeface mTypeface;

		/**
		 * The paint's text size.
		 * This value must be positive and non-negative.
		 */
		private float mFontSize;
	}

	/**
	 * Constructor Font API
	 * @param thread The underlying MoSync thread.
	 */
	public MoSyncFont(MoSyncThread thread)
	{
		mMoSyncThread = thread;

		// Add a new font, the default one.
		MoSyncFontHandle newMosyncFont = new MoSyncFontHandle( mMoSyncThread.mPaint.getTypeface(),
											mMoSyncThread.mPaint.getTextSize() );

		mDefaultFontHandle = 1;

		/*
		 * The new font is saved in the list of available fonts.
		 * Skip 0 value for font handles.
		 */
		mFonts.put(new Integer(1), newMosyncFont);
	}

	/************************ Syscalls ************************/

	/**
	* Returns a handle to one of the default fonts of the device, in the style and size you specify.
	* \param 'type' The type of the font, can be FONT_TYPE_[SANS_SERIF,SERIF,MONOSPACE].
	* \param 'style' The style of the font, can be FONT_STYLE_[NORMAL,BOLD,ITALIC].
	* \param 'size' The size of the font.
	* \return The handle to the font, RES_FONT_NO_TYPE_STYLE_COMBINATION, or RES_FONT_INVALID_SIZE.
	*/
	int maFontLoadDefault(int type, int style, int size)
	{
		if ( size > 0 )
		{

			// Create a new font handle using these settings.
			MoSyncFontHandle newMosyncFont;
			Typeface fontFamily = null;

			switch ( type)
			{
				case (FONT_TYPE_SERIF):
				{
					fontFamily = Typeface.SERIF;
					break;
				}
				case (FONT_TYPE_SANS_SERIF):
				{
					fontFamily = Typeface.SANS_SERIF;
					break;
				}
				case (FONT_TYPE_MONOSPACE):
				{
					fontFamily = Typeface.MONOSPACE;
					break;
				}
			}

		    /**
		     * Create a typeface of the given style. If the default font does not support the style,
		     * RES_FONT_NO_TYPE_STYLE_COMBINATION is returned.
		     * The style codes correspond to the ones in Typeface styles.
		     * NOTE: if new font styles are added make sure you are consistent here.
		     */
			newMosyncFont = new MoSyncFontHandle(Typeface.create(fontFamily, style), size);

			// Check if the style could be applied to the current family.
			if ( newMosyncFont.getTypeface().getStyle() != style )
			{
				return RES_FONT_NO_TYPE_STYLE_COMBINATION;
			}

			// Increase the current index in the font list.
			mFontHandle++;

			/*
			 * The new font is saved in the list of available fonts.
			 */
			mFonts.put(new Integer(mFontHandle), newMosyncFont);

			// Return the handle to the new font family, style and size combination.
			return mFontHandle;
		}
		else
		{
			return RES_FONT_INVALID_SIZE;
		}
	}

	/**
	* Sets the font to be used with maDrawText and maDrawTextW, and returns the handle
	* to the previous font.
	* \param 'font' an MAHandle for a font object.
	* \return The handle to the previous font, or RES_FONT_INVALID_HANDLE.
	*/
	int maFontSetCurrent(int fontHandle)
	{

		MoSyncFontHandle currentFont = (MoSyncFontHandle)
			mFonts.get(fontHandle);

		// If this fontHandle is not in the list, return with err code.
		if ( currentFont == null )
		{
			return RES_FONT_INVALID_HANDLE;
		}

		int previousHandle = mDefaultFontHandle;

		mDefaultFontHandle = fontHandle;

		// Set the font for the next canvas draw.
		mMoSyncThread.mPaint.setTypeface(currentFont.getTypeface());
		mMoSyncThread.mPaint.setTextSize(currentFont.getFontSize());

		return previousHandle;
	}

	/**
	* Returns the number of fonts that are available in the system.
	* \return Number of fonts.
	* \see maFontGetName.
	*/
	int maFontGetCount()
	{
		// Clear this list each time the method gets called.
		mFontNames.clear();
		mFontNamesLocation.clear();

		// Add the system fonts.
		File systemFontsFile = new File(SYSTEM_FONTS_FOLDER);

		FilenameFilter myFileNameFilter = new FilenameFilter(){
			public boolean accept(File f, String name){
				return name.endsWith(FONT_EXT);
			}
		};

		for(File font : systemFontsFile.listFiles(myFileNameFilter)){
			String fontName = font.getName();
			mFontNames.add(fontName.subSequence(0,fontName.lastIndexOf(FONT_EXT)).toString());
			// Keep track of the fonts that are located in the system.
			mFontNamesLocation.add(FontLocation.SYSTEM);
		}

		//Add custom fonts from Assets Manager.
		try {
	        String[] assetFiles = mMoSyncThread.getActivity().getAssets().list(ASSETS_FONT_FOLDER);
	        for ( int i=0; i<assetFiles.length; i++)
	        {
				if ( assetFiles[i].endsWith(FONT_EXT))
				{
					mFontNames.add(assetFiles[i].subSequence(0, assetFiles[i].lastIndexOf(FONT_EXT)).toString());
					// Keep track of the fonts that are located in the assets folder ( custom ones).
					mFontNamesLocation.add(FontLocation.ASSETS);
				}
			}
		} catch (IOException e)
		{
			Log.e("@@MoSync", "maFontGetCount: No fonts in assets.");
		}

		return mFontNames.size();
	}

	/**
	* Copies the font postscript name of the given index to the buffer.
	* You must have first called maFontGetCount() at least once before calling this function.
	* \param 'index' A 0-based index to the font.
	* \param 'buffer' An empty char buffer that will receive the font name.
	* \param 'bufferLen' The size of the buffer.
	* \return The number of bytes copied (with terminating NULL) or RES_FONT_INDEX_OUT_OF_BOUNDS
	* 		or RES_FONT_INSUFFICIENT_BUFFER or RES_FONT_LIST_NOT_INITIALIZED.
	* \see maFontGetCount, maFontLoadWithName.
	*/
	int maFontGetName(int index,
			final int memBuffer,
			final int memBufferSize)
	{
		String result;

		try
		{
			result = getFontNameOnIndex(index);
		} catch (IndexOutOfBoundsException iofbe)
		{
			Log.e("@@MoSync","maFontGetName: Index out of bounds.");
			return RES_FONT_INDEX_OUT_OF_BOUNDS;
		}

		if( result.length( ) + 1 > memBufferSize )
		{
			Log.e( "MoSync", "maFontGetName: Buffer size " + memBufferSize +
					" too short to hold buffer of size: " + result.length( ) + 1 );
			return RES_FONT_INSUFFICIENT_BUFFER;
		}

		if( result.length( ) <= 0 )
		{
			Log.e( "@@MoSync", "maFontGetName: List is not initialized.");
			return RES_FONT_LIST_NOT_INITIALIZED;
		}

		byte[] ba = result.getBytes();

		// Write string to MoSync memory.
		mMoSyncThread.mMemDataSection.position( memBuffer );
		mMoSyncThread.mMemDataSection.put( ba );
		mMoSyncThread.mMemDataSection.put( (byte)0 );

		return result.length( );
	}

	/**
	* Returns a handle to a font with the specific postscript name and size
	* \param 'postScriptName' The postscript name of the font
	* \param 'size' The size of the font
	* \return Handle to the font, or RES_FONT_NAME_NONEXISTENT
	*/
	int maFontLoadWithName(final String postScriptName, int size)
	{
		// Create a new font handle using postscript name and size.
		MoSyncFontHandle newMosyncFont;

		// First check if the postscript name exists in the list of fonts.
		// This can occur if user omits calling maFontGetCount() which retrieves the list of fonts.
		int fontIndex = mFontNames.indexOf(postScriptName);
		if ( fontIndex >= 0 )
		{
			Typeface newTypeface = null;
			if ( mFontNamesLocation.get(fontIndex) == FontLocation.SYSTEM )
			{
				try{
					// Try to get the typeface from system fonts.
					newTypeface = Typeface.createFromFile(SYSTEM_FONTS_FOLDER + postScriptName + FONT_EXT);
					if ( newTypeface == null )
					{
						// Check if the font exists.
						if ( newTypeface == null )
						{
							Log.e("@@MoSync", "maFontLoadWithName: Font does not exist anymore in the system.");
							return RES_FONT_NAME_NONEXISTENT;
						}
					}
				}
				catch (Exception ex)
				{
					// This can happen if between this call and the syscall that gets all the fonts
					// the font was physically deleted from the device.
					Log.e("@@MoSync", "maFontLoadWithName: Font does not exist in the system.");
					return RES_FONT_NAME_NONEXISTENT;
				}
			}
			else
			{
				// Try to create a typeface from assets. ( ex "fonts/myFont.ttf" )
				newTypeface = Typeface.createFromAsset(
						mMoSyncThread.getActivity().getAssets(), ASSETS_FONT_FOLDER + "/" + postScriptName + FONT_EXT);

				// Check if the font still exists.
				if ( newTypeface == null )
				{
					Log.e("@@Mosync","maFontLoadWithName: Font does not exist anymore in assets.");
					return RES_FONT_NAME_NONEXISTENT;
				}
			}

			// Create the font handle.
			newMosyncFont = new MoSyncFontHandle(newTypeface, size );

			// Increase the current index in the font list.
			mFontHandle++;

			/*
			 * The new font is saved in the list of available fonts.
			 */
			mFonts.put(new Integer(mFontHandle), newMosyncFont);

			return mFontHandle;
		}
		else
		{
			Log.e("@@MoSync","maFontLoadWithName: Postscript name does not exist.");
			return RES_FONT_NAME_NONEXISTENT;
		}
	}

	/**
	* Deletes a loaded font.
	* \param 'font' A font handle
	* \return RES_FONT_OK, RES_FONT_INVALID_HANDLE, or RES_FONT_DELETE_DENIED.
	*/
	int maFontDelete(int fontHandle)
	{
		// Cannot delete font in use.
		if ( mDefaultFontHandle == fontHandle )
		{
			return RES_FONT_DELETE_DENIED;
		}

		MoSyncFontHandle currentFont = (MoSyncFontHandle) mFonts.get(fontHandle);

		if ( currentFont == null )
		{
			return RES_FONT_INVALID_HANDLE;
		}

		// Delete this font from the list.
		mFonts.remove(fontHandle);

		return RES_FONT_OK;
	}

	/************************ Implementation Support ************************/

	String getFontNameOnIndex(int index)
		throws IndexOutOfBoundsException
	{
		String fontName;

		// Check if the font list has been retrieved.
		if ( mFontNames.size() > 0 )
		{
			// If out of bounds, IndexOutOfBounds is thrown.
			fontName = mFontNames.get(index);
			return fontName;

		}

		return "";// handle RES_FONT_LIST_NOT_INITIALIZED;
	}

	/**
	 * Search in the list of fonts for a handle.
	 * This is used when setting typeface to native ui widgets.
	 * @param 'fontHandle' A font handle
	 * @return The MoSyncFontHandle object that refers to that handle.
	 */
	public MoSyncFontHandle getMoSyncFont(int fontHandle)
	{
		MoSyncFontHandle currentFont = (MoSyncFontHandle)
			mFonts.get(fontHandle);

		return currentFont;
	}

	/************************ Class members ************************/

	/**
	 * The extension of the true type fonts.
	 */
	private final static String FONT_EXT = ".ttf";

	/**
	 * The directory of system fonts.
	 */
	private final static String SYSTEM_FONTS_FOLDER = "/system/fonts/";

	/**
	 * The subdir of the assets where custom fonts are loaded.
	 */
	private final static String ASSETS_FONT_FOLDER = "fonts";

	/**
	 * Table that holds list of fonts.
	 * These fonts are created by the user via:
	 *  maFontLoadDefault and maFontLoadWithName calls.
	 * At any time, a font can be set from this list.
	 * The first font is the default font. So,
	 * there will be no backwards compatibility issues.
	 */
	Hashtable<Integer, MoSyncFontHandle> mFonts =
		new Hashtable<Integer,MoSyncFontHandle>();

	/*
	 *  List of all device available fonts.
	 */
	List<String> mFontNames = new ArrayList<String>();

	/**
	 * List that holds location information for all the available fonts
	 * that are stored in mFontNames.
	 * Location can be SYSTEM or ASSETS.
	 */
	List<FontLocation> mFontNamesLocation = new ArrayList<FontLocation>();

	/**
	 * The current font handle index in the list of fonts.
	 * Used for loading fonts.
	 * 0 is skipped for handles.
	 */
	int mFontHandle = -1;

	/**
	 * The current font handle from the list of fonts.
	 * It is set by maFontSetCurrent.
	 * It is set to 1, for the default font.
	 */
	int mDefaultFontHandle = -1;

	/**
	 * If this is true, that means that maFontSetCurrent was called,
	 * and at the next canvas draw we need to set the font.
	 */
	boolean mNewFontUsed = false;
}
