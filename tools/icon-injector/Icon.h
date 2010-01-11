/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#ifndef _ICON_H_
#define _ICON_H_

#include <string>
#include <vector>

namespace MoSync {
	
/**
*	This struct represents a single size variation of a MoSync application icon,
*	basically a size string (WxH format) mapped to a relative file path.
*/
struct IconInstance {
	IconInstance(const std::string size, const std::string& filename)
		: size(size), filename(filename) {}
	std::string size;
	std::string filename;
};

/**
* 	This struct constitutes the in-memory representation of a MoSync application icon structure,
*	containing a number of instances of IconInstance. It is capable of populating itself from an XML file,
*	and can thereafter be used to find the closest source icon image to use as the icon for a
*	specific device, automatically scaling it to the desired resolution.
*/
struct Icon {
public:
	/**
	 * Parses the XML file describing the icon. An example file could look as follows:
	 *
	 *	<icon>
	 *		<instance size="32x32" src="res/icon32x32.png"/>
	 *		<instance size="64x64" src="res/icon.svg"/>
	 *	</icon>
	 *
	 */
	static Icon* parse(const std::string& filename);
	void addInstance(const std::string& size, const std::string& filename);
	const IconInstance* findBestInstance(const std::string& size) const;
	std::vector<IconInstance*>& getInstances();
protected:
	std::vector<IconInstance*> instances;
};

}

#endif	//_ICON_H_
