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

#include <ma.h>
#include <maassert.h>
#include <MAFS/File.h>
#include "MAHeaders.h"

int testfwrite()
{
  FILE * pFile;
  char c;

  pFile=fopen("alphabet.txt","wt");
	MAASSERT(pFile);
  for (c = 'A' ; c <= 'Z' ; c++) {
    putc (c , pFile);
  }
  fclose (pFile);
  return 0;
}

int testfopen() {
	FILE *f;
	int i;
	setCurrentFileSystem(RES_BUNDLE, 0);
	f = fopen("mafs.data", "rb");
	MAASSERT(f);

	for(i = 0; i < 10; i++) {
		int j;
		/*int ret =*/ fread(&j, sizeof(int), 1, f);
		if(i == j) {
			;//int a = 2;
		}
	}

	return 0;
}

int MAMain() {
	testfopen();
	testfwrite();
	return 0;
}
