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

#import "MoSyncOrientation.h"

SYSCALL(int, maScreenSetSupportedOrientations(const int orientations))
{
	return [[ScreenOrientation getInstance] setSupportedOrientations:orientations];
}

SYSCALL(int, maScreenGetSupportedOrientations())
{
	return [[ScreenOrientation getInstance] getSupportedOrientations];
}

SYSCALL(int, maScreenGetCurrentOrientation())
{
	return [[ScreenOrientation getInstance] getCurrentScreenOrientation];
}
