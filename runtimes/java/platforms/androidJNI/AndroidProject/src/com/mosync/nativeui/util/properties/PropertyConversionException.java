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

package com.mosync.nativeui.util.properties;

/**
 * Exception that is thrown when a property conversion was unsuccessful.
 * 
 * @author fmattias
 */
public class PropertyConversionException extends RuntimeException
{
	private static final long serialVersionUID = 7497514582595613978L;
	
	public PropertyConversionException(String value)
	{
		super( "Could not convert: '" + value + "'." );
	}
}