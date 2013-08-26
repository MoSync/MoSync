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

/**
 * @file rescomp.cpp
 * @author Florin Leu
 * @date 22 Nov 2011
 **/

#include "rescomp.h"
#include "ResCompiler.h"

int loadResource(MAHandle handle)
{
	resManager->loadResource(handle);
	return 0;
}

int unloadResource(MAHandle handle)
{
	resManager->unloadResource(handle);
	return 0;
}

int resource_selector()
{
	resManager = new ResourceCompiler::VariantResourceLookup();

	int labelMapping = maFindLabel("variant-mapping");
	int labelTypes = maFindLabel("res-types");

	resManager->countResources();

	if ((labelMapping == -1) || (labelTypes == -1))
	{
		resManager->loadResources(false);
		return -1;
	}

	resManager->readVariantMapping(labelMapping + 1);

	resManager->readResourceTypes(labelTypes + 1);

	resManager->pickResources();

	resManager->loadResources(true);

	return 0;
}
