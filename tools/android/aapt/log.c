/*
	Adds these two implementations to the aapt build process so that it succeds
*/

int __android_log_print(int prio, const char *tag,  const char *fmt, ...) { return 1; }
void __android_log_assert(const char *cond, const char *tag, const char *fmt, ...) {}
