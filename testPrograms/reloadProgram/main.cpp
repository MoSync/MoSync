#include <ma.h>
#include <mastring.h>

static void LOG(const char* msg) {
	maWriteLog(msg, strlen(msg));
}

int MAMain() {
	// check to see if we're the original.
	MAHandle program;
	if(maFindLabel("label") > 0) {
		LOG("write\n");
		// if so, save program file to store.
		MAHandle h = maOpenStore("program", MAS_CREATE_IF_NECESSARY);
		maWriteStore(h, 1);
		maCloseStore(h, 0);
		program = 1;
	} else {
		LOG("read\n");
		// otherwise, read program file from store.
		MAHandle h = maOpenStore("program", 0);
		program = maCreatePlaceholder();
		maReadStore(h, program);
		maCloseStore(h, 0);
	}

	LOG("load\n");
	maLoadProgram(program, 0);
	return 1;
}
