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

#include "AndroidInjector.h"
#include  "Icon.h"
#include <fstream>
#include <sstream>
#include "Shared.h"

using namespace std;

namespace MoSync {

void AndroidInjector::inject(const Icon* icon, const std::map<std::string, std::string>& params) {
	string size = verifyParameter(params, "size");
	string dst = verifyParameter(params, "dst");
	const IconInstance* iconInst = icon->findBestInstance(size);
	if(!iconInst) errorExit("Couldn't find any icon instance.");

	if(!convertInstanceToImageFormat(iconInst, dst.c_str(), size, "png")) 
		errorExit("Android icon conversion failed.");
}

}
