#include <ma.h>

static char *argv[] = {"exe", "-width", "240", "-height", "320", "-bpp", "32", NULL};
static int argc = sizeof(argv)/sizeof(char*);

int MAMain() {
	return main(argc, argv);
}
