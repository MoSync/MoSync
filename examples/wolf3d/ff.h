#ifndef __FF_H__
#define __FF_H__

#define FA_ARCH 0

struct ffblk {
//  char ff_reserved[5];      /* used to hold the state of the search */
//  unsigned char ff_attrib;  /* actual attributes of the file found */
//  unsigned short ff_ftime;  /* hours:5, minutes:6, (seconds/2):5 */
//  unsigned short ff_fdate;  /* (year-1980):7, month:4, day:5 */
//  unsigned long ff_fsize;   /* size of file */
  char ff_name[256];        /* name of file as ASCIIZ string */
};

#ifdef __cplusplus
extern "C"
#endif
int findfirst(const char *pathname, struct ffblk *ffblk, int attrib);
#ifdef __cplusplus
extern "C"
#endif
int findnext(struct ffblk *ffblk);
#ifdef __cplusplus
extern "C"
#endif
void resetinactivity(void);

#endif
