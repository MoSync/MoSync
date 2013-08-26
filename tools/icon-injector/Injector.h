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
protected:
	bool fForce;

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

	/**
	 * If set, this will enforce icon generation regardless of whether
	 * the timestamp of the icon is older than the icon file.
	 */
	void setForce(bool force);

	/**
	 * Function used to convert an icon instance to an image.
	 * \param iconInstance The icon instance.
	 * \param dstFilename The filename of the resulting image.
	 * \param size A string describing the size of the resulting image on the form: WxH
	 * \param format A string describing the format of the resulting image (the file extension of the format)
	 */
	bool convertInstanceToImageFormat(const IconInstance *iconInstance,
		const char *dstFilename, const std::string& size, const std::string& format);
};

}

#endif // _INJECTOR_H_
