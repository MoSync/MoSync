#ifndef MASTDLIB_H
#define MASTDLIB_H

#include "stdlib.h"

// returns a new file descriptor of a write-only stream connnected to the maWriteLog() syscall.
int open_maWriteLog(void);

#endif	//MASTDLIB_H
