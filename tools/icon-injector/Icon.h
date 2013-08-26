/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
	IconInstance(const std::string _size, const std::string& _filename)
		: size(_size), filename(_filename) {}
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
	static Icon* parse(const std::string& filename, std::string& platform);
	void addInstance(const std::string& size, const std::string& filename);
	const IconInstance* findBestInstance(const std::string& size, const std::string& ext="") const;
	std::vector<IconInstance*>& getInstances();
protected:
	std::vector<IconInstance*> mInstances;
};

}

#endif	//_ICON_H_
