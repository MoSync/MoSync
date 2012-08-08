/*
 * A little C program that retrieves build info from the file:
 * __MOSYNC_PATH__/bin/version.dat and store it in the resource file "resources.lst"
 * in this programs current working dir.
 *
 * This means that we, thanks to the resource compiler can get this info at runtime later.
 */

#include <stdio.h>
#include <string.h>

char * PATH = "../../bin/version.dat";

int main(int argc, char ** argv) {

	FILE *rfp, *wfp; //file pointers to read from and write to.
	rfp = fopen(argv[1] , "r"); //get file name to version.dat from command line
	wfp = fopen("./resources.lst", "w");
	char buf[100];
	if (rfp == NULL || wfp == NULL)
		perror ("Error opening files");
	else{
		int row = 0;
		while(fgets(buf , 100 , rfp) != NULL){
			if(row == 0)
				fprintf(wfp, ".res R_BUILDINFO\n"); //write this to the first row in the file
			buf[strlen(buf)-1] = 0;
			printf("%s\n", buf);
			fprintf(wfp, ".string \"%s\" \n", buf);
			++row;
		}
		fclose (rfp);
		fclose (wfp);
	}

	return 0;
}
