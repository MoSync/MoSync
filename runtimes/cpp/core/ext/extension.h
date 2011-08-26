#ifndef _MOSYNC_EXTENSION_H_
#define _MOSYNC_EXTENSION_H_

#define SYSCALL(ret, func) ret func

#define REG(r) gRegs[r]

extern int* gRegs;
extern void* gMemDs;

template<class Dst, class Src> Dst union_cast(Src s) {
	union {
		Dst d;
		Src s;
	} u;
	u.s = s;
	return u.d;
}
#define MAKE(type, a) union_cast<type>(a)

#endif	//_MOSYNC_EXTENSION_H_
