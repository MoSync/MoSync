
#ifndef _MAIN_H_
#define _MAIN_H_

#define MAX_INPUTS 8


extern byte  debug_on;
extern byte log_error;
extern int frame_count;

extern byte soundbuffer[16][3840];
extern int mixbuffer;

int update_input(void);

#define CART_ROM_SIZE 0x200000

#endif /* _MAIN_H_ */
