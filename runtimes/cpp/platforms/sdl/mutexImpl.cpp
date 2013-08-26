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

#undef LOGGING_ENABLED	//debugger
#include "config_platform.h"

#include <helpers/helpers.h>

#define NETWORKING_H
#include "networking.h"

//***************************************************************************
//MoSyncMutex
//***************************************************************************

MoSyncMutex::MoSyncMutex() : mMutex(NULL) {}

void MoSyncMutex::init() {
	mMutex = SDL_CreateMutex();
	MYASSERT(mMutex, ERR_OOM);
}

MoSyncMutex::~MoSyncMutex() {
	DEBUG_ASSERT(mMutex == NULL);	//make sure it's closed
}

void MoSyncMutex::close() {
	if(mMutex) {
		SDL_DestroyMutex(mMutex);
		mMutex = NULL;
	}
}

void MoSyncMutex::lock() {
	DEBUG_ASRTZERO(SDL_mutexP(mMutex));
}

void MoSyncMutex::unlock() {
	DEBUG_ASRTZERO(SDL_mutexV(mMutex));
}
