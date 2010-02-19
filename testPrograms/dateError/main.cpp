#include <ma.h>
#include <matime.h>
#include <conprint.h>
#include <maassert.h>

extern "C" int MAMain() {
	printf("Local time:\n");
	time_t myTime = maLocalTime();
	for(int i = 0; i < 61 ; i++){
		printf("%s\n", sprint_time(myTime));
		myTime -= (60 * 60 * 24);
	}
	FREEZE;
}
