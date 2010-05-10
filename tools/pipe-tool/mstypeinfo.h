
#define RINT(xx) 		*((int *) mem_ds + xx)
#define RSHORT(xx) 		*((short *) mem_ds + xx)
#define RBYTE(xx) 		*((char *) mem_ds + xx)

#define WINT(xx,yy)		RINT(xx) = yy
#define WSHORT(xx,yy)	RSHORT(xx) = yy
#define WBYTE(xx,yy)	RBYTE(xx) = yy

#define SYSCALL(name)	_##name

void MoSyncDiv0();