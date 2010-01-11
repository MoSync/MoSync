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

#ifndef _INJECTOR_H_
#define _INJECTOR_H_

#include <string>
#include <map>
#include "Icon.h"

namespace MoSync {

/**
 * Base class for the icon injectors.
 */
class Injector {
public:
	/**
	 *  This virtual function is used to inject an icon into a platform-specific application
	 *   package, with varying behavior and semantics on different platforms.
	 * \param icon the Icon to be injected
	 * \param params a map of parameters passed to the tool on the command line
	 */
	virtual void inject(
		const Icon* icon, 
		const std::map<std::string, std::string>& params) = 0;
	/**
	 *  This function verifies the existance of a parameter in the parameter map passed to it,
	 *   exiting with an error if it doesn't, otherwise returning the value of that parameter.
	 * \param params a map of parameters passed to the tool on the command line
	 * \param name the name of the parameter to be verified and returned
	 * \return the value of the parameter in \a params called \a name
	 */
	static std::string verifyParameter(
		const std::map<std::string, std::string>& params, 
		const std::string& name);
};

}

#endif // _INJECTOR_H_
