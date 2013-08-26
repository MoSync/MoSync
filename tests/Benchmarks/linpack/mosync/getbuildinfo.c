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

/*
 * A little C program that retrieves build info from the file:
 * __MOSYNC_PATH__/bin/version.dat and store it in the resource file "resources.lst"
 * in this programs current working dir.
 *
 * This means that we, thanks to the resource compiler can get this info at runtime later.
 */

#include <mastring.h>
#include "MAFS/File.h"

char * PATH = "../../bin/version.dat";

int main(int argc, char ** argv) {

	FILE *rfp, *wfp; //file pointers to read from and write to.
	rfp = fopen(PATH , "r");
	wfp = fopen("./buildinfo.h", "w");
	char buf[100];
	if (rfp == NULL || wfp == NULL)
	{
	//	perror ("Error opening files");
		maPanic(0, "Error opening files");
	}
	else{
		int row = 0;
		char *m1 = "//Info about the build, git-hash, version name and such. \n";
		char *m2 = "#define BUILDVAR%d \"%s\" \n";
		while(fgets(buf , 100 , rfp) != NULL){
			if(row == 0)
			{
			//	fprintf(wfp, "//Info about the build, git-hash, version name and such. \n"); //write this to the first row in the file
				fwrite(m1, sizeof(char), strlen(m1), wfp);
			}
			buf[strlen(buf)-1] = 0;
			printf("%s\n", buf);
			//fprintf(wfp, "#define BUILDVAR%d \"%s\" \n", row, buf);
			fwrite(m2, sizeof(char), strlen(m2), wfp);
			++row;
		}
		fclose (rfp);
		fclose (wfp);
	}

	return 0;
}
