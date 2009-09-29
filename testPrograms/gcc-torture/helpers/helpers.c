#include <ma.h>

void abort() {
	maExit(420);
}

#ifdef MAPIP
void exit(int code) {
	maExit(code);
}
#endif

int main();

int MAMain() {
	return main();
}
