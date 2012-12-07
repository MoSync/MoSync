#include <ma.h>
#include <conprint.h>
#include <maassert.h>

#define TEST(call) { int _res = (call); if(_res < 0) { printf("@%i: %i (%s)\n", __LINE__, _res, #call); return; } }

static void doTest() {
	printf("Test start.\n");
	char path[1024];
	int len = maGetSystemProperty("mosync.path.local", path, sizeof(path));
	if(len < 0) {
		printf("mpl: %i\n", len);
		return;
	}
	printf("len: %i\n", len);
	printf("strlen: %i\n", strlen(path));
	strcpy(path+len-1, "testFile.bin");
	printf("%s\n", path);

	MAHandle f;
	TEST(f = maFileOpen(path, MA_ACCESS_READ_WRITE));
	int exists;
	TEST(exists = maFileExists(f));
	if(exists) {
		TEST(maFileTruncate(f, 0));
	} else {
		TEST(maFileCreate(f));
	}

	TEST(maFileWrite(f, path, (sizeof(path)*2)/3));
	TEST(maFileClose(f));

	TEST(f = maFileOpen(path, MA_ACCESS_READ));
	TEST(exists = maFileExists(f));
	if(!exists) {
		printf("Error: Our file got deleted!\n");
		return;
	}
	int res = maFileRead(f, path, sizeof(path));
	if(res != MA_FERR_GENERIC) {
		printf("FR Got %i\n", res);
		printf("Expected %i\n", MA_FERR_GENERIC);
		return;
	}
	printf("FR success!\n");
	TEST(maFileSeek(f, MA_SEEK_SET, 0));
	MAHandle data = maCreatePlaceholder();
	TEST(maCreateData(data, sizeof(path)));
	res = maFileReadToData(f, data, 0, sizeof(path));
	if(res != MA_FERR_GENERIC) {
		printf("FRTD Got %i\n", res);
		printf("Expected %i\n", MA_FERR_GENERIC);
		return;
	}
	printf("FRTD success!\n");
}

int MAMain() GCCATTRIB(noreturn);
int MAMain() {
	doTest();
	FREEZE;
}
