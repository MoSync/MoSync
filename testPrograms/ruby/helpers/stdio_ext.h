#ifndef STDIO_EXT_H
#define STDIO_EXT_H

#include "src/ruby.h"

struct FILE;
typedef struct FILE FILE;

FILE* fopen(const char* filename, const char* mode);
int feof(FILE*);
int fseek(FILE*, long, int);
int fileno(FILE*);
int fflush(FILE*);
int ftell(FILE*);
size_t fwrite(const void *, size_t, size_t, FILE*);
void clearerr(FILE*);
int ferror(FILE*);
int getc(FILE*);
int ungetc(int, FILE*);
size_t fread(const void *, size_t, size_t, FILE*);
int fclose(FILE*);
int fprintf(FILE*, const char* fmt, ...);
FILE* freopen(const char *, const char *, FILE*);

int dup(int orig);
int write(int fno, const void* src, int size);
int fcntl(int fno, int, int);
int read(int fno, void* dst, int size);
int isatty(int fno);
int lseek(int fno, int pos, int whence);
FILE* fdopen(int fno, const char* mode);
int open(const char* fname, int flags, int mode);
int close(int fno);
int ioctl(int fd, int cmd, int narg);

int unlink(const char* fname);
int chmod(const char* fname, int mode);
int rename(const char* old, const char* new);


#define EOF (-1)
#define O_NONBLOCK 1
#define O_RDONLY 2
#define O_WRONLY 4
#define O_RDWR 8
#define O_APPEND 16
#define O_CREAT 32
#define O_BINARY 64
#define O_TRUNC 128
#define O_EXCL 256
#define F_SETFL 0
#define SEEK_CUR 0
#define SEEK_SET 1
#define SEEK_END 2

extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;


typedef short          dev_t;      /* holds (major|minor) device pair */
typedef char           gid_t;      /* group id */
typedef unsigned long  ino_t;      /* i-node number (V3 filesystem) */
typedef unsigned short mode_t;     /* file type and permissions bits */
typedef short        nlink_t;      /* number of links to a file */
typedef unsigned long  off_t;      /* offset within a file */
typedef int            pid_t;      /* process id (must be signed) */
typedef short          uid_t;      /* user id */
typedef unsigned long zone_t;      /* zone number */
typedef unsigned long block_t;     /* block number */
typedef unsigned long  bit_t;      /* bit number in a bit map */
typedef unsigned short zone1_t;    /* zone number for V1 file systems */
typedef unsigned short bitchunk_t; /* collection of bits in a bitmap */

struct stat {
	dev_t st_dev;
	ino_t st_ino;
	mode_t st_mode;
	nlink_t st_nlink;
	uid_t st_uid;
	gid_t st_gid;
	dev_t st_rdev;
	off_t st_size;
	time_t st_atime;
	time_t st_mtime;
	time_t st_ctime;
};

/* Traditional mask definitions for st_mode. */
#define S_IFMT  0170000 /* type of file */
#define S_IFLNK 0120000 /* symbolic link */
#define S_IFREG 0100000 /* regular */
#define S_IFBLK 0060000 /* block special */
#define S_IFDIR 0040000 /* directory */
#define S_IFCHR 0020000 /* character special */
#define S_IFIFO 0010000 /* this is a FIFO */
#define S_ISUID 0004000 /* set user id on execution */
#define S_ISGID 0002000 /* set group id on execution */
                                /* next is reserved for future use */
#define S_ISVTX   01000         /* save swapped text even after use */

/* POSIX masks for st_mode. */
#define S_IRWXU   00700         /* owner:  rwx------ */
#define S_IRUSR   00400         /* owner:  r-------- */
#define S_IWUSR   00200         /* owner:  -w------- */
#define S_IXUSR   00100         /* owner:  --x------ */

#define S_IRWXG   00070         /* group:  ---rwx--- */
#define S_IRGRP   00040         /* group:  ---r----- */
#define S_IWGRP   00020         /* group:  ----w---- */
//#define S_IXGRP   00010         /* group:  -----x--- */

#define S_IRWXO   00007         /* others: ------rwx */
#define S_IROTH   00004         /* others: ------r-- */ 
#define S_IWOTH   00002         /* others: -------w- */
#define S_IXOTH   00001         /* others: --------x */

/* Synonyms for above. */
#define S_IEXEC         S_IXUSR
#define S_IWRITE        S_IWUSR
#define S_IREAD         S_IRUSR

/* The following macros test st_mode (from POSIX Sec. 5.6.1.1). */
#define S_ISREG(m)      (((m) & S_IFMT) == S_IFREG)     /* is a reg file */
#define S_ISDIR(m)      (((m) & S_IFMT) == S_IFDIR)     /* is a directory */
#define S_ISCHR(m)      (((m) & S_IFMT) == S_IFCHR)     /* is a char spec */
#define S_ISBLK(m)      (((m) & S_IFMT) == S_IFBLK)     /* is a block spec */
#define S_ISLNK(m)      (((m) & S_IFMT) == S_IFLNK)     /* is a symlink */
#define S_ISFIFO(m)     (((m) & S_IFMT) == S_IFIFO)     /* is a pipe/FIFO */


int fstat(int _fildes, struct stat *_buf);
int stat(const char *_path, struct stat *_buf);


#define X_OK 1

int umask(int);
int access(const char* path, int mode);

#endif	//STDIO_EXT_H
