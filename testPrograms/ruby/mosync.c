#include "config.h"
#include "src/ruby.h"

#include <maassert.h>
#include <conprint.h>

#include "stdio_ext.h"

VALUE ruby_errinfo;

FILE* stdin;
FILE* stdout;
FILE* stderr;
int errno;

char **environ = NULL;
int ruby_yydebug;


int snprintf(char* dst, size_t size, const char* fmt, ...) {
  va_list args;
  int n;

  va_start(args, fmt);
  n = vsnprintf(dst, size, fmt, args);
  va_end(args);

  return n;
}

int vsnprintf(char* dst, size_t size, const char* fmt, va_list args) {
	return vsprintf(dst, fmt, args);
}

void abort() {
	BIG_PHAT_ERROR;
}

char *strerror(int foo) {
	return NULL;
}

void _exit(int code) {
	printf("_exit(%i)\n", code);
	FREEZE;
}

void exit(int code) {
	printf("exit(%i)\n", code);
	FREEZE;
}

int gettimeofday(struct timeval* tv, void* tzp) {
	return 0;
}

void sleep(int duration) {
	maWait(duration);
}


char* getenv(const char* name) {
	return NULL;
}

int getpid() {
	return rand();
}

int getcwd(char* buf, int size) {
	return 0;
}

int strcasecmp(char *us1, char *us2) {
	while (toupper(*us1) == toupper(*us2++))
		if (*us1++ == '\0')
			return (0);
	return (toupper(*us1) - toupper(*--us2)); 	
}


//doubles
//TODO
int isinf(double d) {
	return 0;
}

int isnan(double d) {
	return 0;
}


//file numbers
//TODO
int fileno(FILE* fiel) {
	BIG_PHAT_ERROR;
	return 0;
}
int dup(int orig) {
	BIG_PHAT_ERROR;
	return 0;
}
int write(int fno, const void* src, int size) {
	BIG_PHAT_ERROR;
	return 0;
}
int fcntl(int fno, int a, int b) {
	BIG_PHAT_ERROR;
	return 0;
}
int read(int fno, void* dst, int size) {
	BIG_PHAT_ERROR;
	return 0;
}
int isatty(int fno) {
	BIG_PHAT_ERROR;
	return 0;
}
int lseek(int fno, int pos, int whence) {
	BIG_PHAT_ERROR;
	return 0;
}
FILE* fdopen(int fno, const char* mode) {
	BIG_PHAT_ERROR;
	return 0;
}
int open(const char* fname, int flags, int mode) {
	BIG_PHAT_ERROR;
	return 0;
}
int close(int fno) {
	return 0;
}
int ioctl(int fd, int cmd, int narg) {
	BIG_PHAT_ERROR;
	return 0;
}
int dup2(int a, int b) {
	return 0;
}

//file descriptors
//TODO
int feof(FILE* fiel) {
	return 1;
}

int fseek(FILE* fiel, long offset, int mode) {
	return 1;
}

int fflush(FILE* fiel) {
	BIG_PHAT_ERROR;
	return 0;
}

void clearerr(FILE* fiel) {
}

FILE* fopen(const char* filename, const char* mode) {
	return NULL;
}
int ftell(FILE* f) {
	return EOF;
}
size_t fwrite(const void * s, size_t c, size_t n, FILE* f) {
	return 0;
}
int ferror(FILE* f) {
	return EOF;
}
int getc(FILE* f) {
	return EOF;

}
int ungetc(int c, FILE* f) {
	return EOF;
}
size_t fread(const void * d, size_t c, size_t n, FILE* f) {
	return 0;
}
int fclose(FILE* f) {
	return 0;
}
int fprintf(FILE* f, const char* fmt, ...) {
	return 0;
}
FILE* freopen(const char * n, const char * n2, FILE* f) {
	return NULL;
}

//stat
int fstat(int _fildes, struct stat *_buf) {
	return 0;
}
int stat(const char *_path, struct stat *_buf) {
	return 0;
}



//file system
//TODO
int unlink(const char* fname) {
	return 0;
}
int chmod(const char* fname, int mode) {
	return 0;
}
int rename(const char* old, const char* new) {
	return 0;
}
int umask(int mask) {
	return 0;
}
int access(const char* path, int mode) {
	return 0;
}



//time
static struct tm sSharedTime;

//TODO
struct tm* gmtime(const time_t* timer) {
	return &sSharedTime;
}
struct tm* localtime(const time_t* timer) {
	return &sSharedTime;
}

static char sSharedBuffer[128];

char* asctime(const struct tm* timeptr) {
	return sprint_tm(timeptr, sSharedBuffer);
}

size_t strftime(char* ptr, size_t maxsize, const char* format, const struct tm* timeptr) {
	return sprintf(ptr, "strftime %s", format);
}


#include "src/intern.h"

int setjmp(int* buf) {
	//printf("setjmp 0x%p\n", buf);
	return 0;
}

extern VALUE ruby_errinfo;
extern int ruby_sourceline;

void longjmp(int* buf) {
	if(ruby_errinfo != Qnil) {
		const char *classname = rb_obj_classname(ruby_errinfo);
	    StringValue(ruby_errinfo);
	    char temp[256];
		sprintf(temp, "Exception `%s' at line %d - %s\n",
			classname,
			ruby_sourceline,
			RSTRING(ruby_errinfo)->ptr);
		maPanic(0, temp);
		/*
		VALUE name = rb_funcall(ruby_errinfo, rb_intern("to_s"), 0);
		StringValue(name);
		maPanic(0, StringValuePtr(ruby_errinfo)); 
		*/
	}
		
	printf("longjmp 0x%p\n", buf);
	maExit(666);
}

#include "MAHeaders.h"

//static const char* argv[3];

int is_valid_char(char c) {
	if(c>=32&&c<=126) return 1;
	else return 0;
}

//main
int MAMain() {
	int initStartTime;
	InitConsole();
	ConsoleLogging = 1;
	printf("initializing...");
	initStartTime = maGetMilliSecondCount();
	ruby_init();
	printf("ruby_init took %i ms.\n", maGetMilliSecondCount() - initStartTime);
	
	/*
#if 0
	printf("compile...");
	char script[1024];
	VALUE e_script = rb_str_new(0,0);
	rb_str_cat2(e_script, "10.upto 20 { puts 'hej' }");
	rb_str_cat2(e_script, "\n");
	require_libraries();
	rb_compile_string(script, e_script, 1);
#else
	int size = maGetDataSize(R_RUBY_EXAMPLE);
	char* script = malloc(size);
	maReadData(R_RUBY_EXAMPLE, script, 0, size);
	{	//fix \n
		char* ptr = script;
		while(*ptr) {
			if(*ptr == '\n')
				*ptr = ';';
			ptr++;
		}
	}
#endif
*/
	Handle store = maOpenStore("application.rb", 0);
	Handle data = R_RUBY_EXAMPLE;
	
	if(store!=STERR_NONEXISTENT) {
		data = maCreatePlaceholder();
		maReadStore(store, data);
		maCloseStore(store, 0);
	}
	

	int size = maGetDataSize(data);
	char* script = malloc(size);
	maReadData(data, script, 0, size);	

	{	//fix \n
		char* ptr = script;
		while(*ptr) {
			if((*ptr == 0xd) && *(ptr+1) && (*(ptr+1)==0xa)) {
				*ptr = ' ';
				ptr++;	
				*ptr = ';';
			} else if((*ptr == 0xd) || (*ptr == 0xa) || (*ptr == '\n'))
				*ptr = ';';
			else if(!is_valid_char(*ptr)) {
				*ptr = ' ';
			}
			
			ptr++;
		}
	}		
	
	//maWriteLog(script, size);

	//printf("ruby_options");
	//const char* argv[] = { "ruby", "-e", "fasd,fjkbaslfailuhas"};//script };
	char* argv[3];
	argv[0] = "ruby";
	argv[1] = "-e";
	argv[2] = script;
#if 0
	int i;
	for(i=0; i<3; i++) {
		//printf("%i: \"%s\"\n", i, argv[i]);
		puts(argv[i]);
	}
#endif

	ruby_options(3, argv);
	//printf("ruby_options done\n");
	ruby_run();
	printf("ruby_run done\n");
	return 0;
}
