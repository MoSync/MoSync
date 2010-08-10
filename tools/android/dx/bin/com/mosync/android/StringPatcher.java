package com.mosync.android;

import java.util.List;
import java.util.Map.Entry;
import java.util.LinkedList;
import java.util.AbstractMap.SimpleEntry;


/**
 * This is a simple singleton that is used for patching 
 * Strings, ultimately it is used for patching packages 
 * names when converting class files to dex. 
 * 
 * @author Ali Mosavian
 */
public class StringPatcher 
{
	private List<Entry<String, String>>	m_patchList;
	
	/**
	 * Private constructor
	 * 
	 */
	private StringPatcher ( )
	{
		m_patchList = new LinkedList<Entry<String,String>>( );
	}
	
	
	/**
	 * Adds a new patch to the list
	 * 
	 * @param s String to replace
	 * @param r String to replace with
	 */
	public void addPatch ( String s,
						   String r )
	{
		Entry<String, String> e = new SimpleEntry<String, String>( s, r );
		m_patchList.add( e );		
	}

	
	/**
	 * Returns the patched string
	 * Note: Only one patch per input
	 * 
	 * @param s Input string
	 * 
	 * @return Patched string, if there was something to patch,
	 * 		   if not, the same string. 
	 */
	public String patch ( String s )
	{
		// Go through patches and return after first patch
		for ( Entry<String, String> e : m_patchList )
		{
			if ( s.contains( e.getKey( ) ) )
				return s.replace( e.getKey( ), e.getValue( ) );
		}
		
		return s;
	}
	
	
	/**
	 * Returns whether or not there are any patches
	 * 
	 * @return true or false
	 */
	public boolean hasPatches ( )
	{
		return m_patchList.size( ) > 0;
	}
	
	
	/**
	 * Returns singleton instance.
	 * Note: This method is not thread safe.
	 * 
	 * @returns StringPatcher instance
	 */
	private static StringPatcher m_instance = null;
	public static StringPatcher getInstance ( )
	{
		if ( m_instance == null )
			m_instance = new StringPatcher( );
		
		return m_instance;
	}
	
}
