#ifndef _MOSYNC_SYSCALL_ARGUMENTS_H_
#error Don't include this file unless you know what you're doing.
// required definitions:
// SAFUNC
// SCDEBUG_ARG
// LOGSC
// mem_ds
// #include "maapi_defs.h"
// #include "cpp_defs.h"
// ValidateMemRange
// ValidateMemStringAddress
// ValidateMemWStringAddress
#endif

SAFUNC void _debug_hex(int SCDEBUG_ARG(hex)) { LOGSC("0x%08x", hex); }

#define _SYSCALL_HANDLERES_DEFAULT(type) REG(REG_r14) = _SYSCALL_CONVERTRES_##type(res); LOGSC(");\n");

SAFUNC void debug_MACopyData(const MACopyData* SCDEBUG_ARG(p)) {
	LOGSC("(%i,%i, %i,%i, %i)",
		p->dst, p->dstOffset,	p->src, p->srcOffset,	p->size);
}
SAFUNC MACopyData* _SYSCALL_CONVERT_MACopyData(int ptr) {
	_debug_hex(ptr);
	MACopyData* p = (MACopyData*)((char*)mem_ds + ptr);
	ValidateMemRange(p, sizeof(MACopyData));
	debug_MACopyData(p);
	return p;
}

SAFUNC void debug_MAPoint2d(const MAPoint2d* SCDEBUG_ARG(p)) {
	LOGSC("(%i,%i)", p->x, p->y);
}
SAFUNC MAPoint2d* _SYSCALL_CONVERT_MAPoint2d(int ptr) {
	_debug_hex(ptr);
	MAPoint2d* p = (MAPoint2d*)((char*)mem_ds + ptr);
	ValidateMemRange(p, sizeof(MAPoint2d));
	debug_MAPoint2d(p);
	return p;
}

SAFUNC void debug_MARect(const MARect* SCDEBUG_ARG(r)) {
	LOGSC("(%i,%i,%i,%i)", r->left, r->top, r->width, r->height);
}
SAFUNC MARect* _SYSCALL_CONVERT_MARect(int ptr) {
	_debug_hex(ptr);
	MARect* r = (MARect*)((char*)mem_ds + ptr);
	ValidateMemRange(r, sizeof(MARect));
	debug_MARect(r);
	return r;
}

SAFUNC void debug_MAConnAddr(const MAConnAddr* SCDEBUG_ARG(r)) {
	LOGSC("(%i)", r->family);
}
SAFUNC MAConnAddr* _SYSCALL_CONVERT_MAConnAddr(int ptr) {
	_debug_hex(ptr);
	MAConnAddr* r = (MAConnAddr*)((char*)mem_ds + ptr);
	ValidateMemRange(r, sizeof(MAConnAddr));
	debug_MAConnAddr(r);
	return r;
}

SAFUNC void debug_MAString(char* SCDEBUG_ARG(str)) { LOGSC("(\"%s\")", str); }
SAFUNC char* _SYSCALL_CONVERT_MAString(int str) {
	_debug_hex(str);
	ValidateMemStringAddress(str);
	char* res = (char*)mem_ds + str;
	debug_MAString(res);
	return res;
}
#define _SYSCALL_CONVERTRES_MAString _SYSCALL_CONVERTRES_MAAddress
#define _SYSCALL_HANDLERES_MAString _SYSCALL_HANDLERES_DEFAULT(MAString)
#define _SYSCALL_CONVERT_NCString (char*)_SYSCALL_CONVERT_MAAddress

SAFUNC void debug_MAWString(wchar* SCDEBUG_ARG(str)) {
#ifdef SYSCALL_DEBUGGING_MODE
	LOGSC("(\"");
	while(*str) {
		LOGSC("%C", *str);
		str++;
	}
	LOGSC("\")");
#endif
}
SAFUNC wchar* _SYSCALL_CONVERT_MAWString(int str) {
	_debug_hex(str);
	ValidateMemWStringAddress(str);
	wchar* res = (wchar*)((char*)mem_ds + str);
	debug_MAWString(res);
	return res;
}
#define _SYSCALL_CONVERTRES_MAWString _SYSCALL_CONVERTRES_MAAddress
#define _SYSCALL_HANDLERES_MAWString _SYSCALL_HANDLERES_DEFAULT(MAWString)

SAFUNC void debug_int(int SCDEBUG_ARG(i)) { LOGSC("(%i)", i); }
SAFUNC int _SYSCALL_CONVERT_int(int i) {
	_debug_hex(i);
	debug_int(i);
	return i;
}
SAFUNC int _SYSCALL_CONVERTRES_int(int i) { return i; }
#define _SYSCALL_HANDLERES_int _SYSCALL_HANDLERES_DEFAULT(int)

SAFUNC int _SYSCALL_CONVERTRES_uint(uint i) { return i; }
#define _SYSCALL_HANDLERES_uint _SYSCALL_HANDLERES_DEFAULT(uint)

SAFUNC void debug_ulong(int SCDEBUG_ARG(i)) { LOGSC("(%u)", i); }
SAFUNC int _SYSCALL_CONVERT_ulong(int i) {
	_debug_hex(i);
	debug_int(i);
	return i;
}
#define _SYSCALL_HANDLERES_ulong _SYSCALL_HANDLERES_DEFAULT(ulong)

SAFUNC void debug_float(float SCDEBUG_ARG(f)) { LOGSC("(%f)", f); }
#define _SYSCALL_CONVERT_float(a) _convert_float(&a);
SAFUNC float _convert_float(int* ptr) {
	_debug_hex(*((int*)ptr));
	debug_float(*((float*)ptr));
	return *((float*)ptr);
}
SAFUNC int _SYSCALL_CONVERTRES_float(float f) { return MAKE(int, f); }
#define _SYSCALL_HANDLERES_float _SYSCALL_HANDLERES_DEFAULT(float)

SAFUNC void debug_short(short SCDEBUG_ARG(i)) { LOGSC("(%i)", i); }
SAFUNC short _SYSCALL_CONVERT_short(short i) {
	_debug_hex(i);
	debug_short(i);
	return i;
}
SAFUNC short _SYSCALL_CONVERTRES_short(short i) { return i; }
#define _SYSCALL_HANDLERES_short _SYSCALL_HANDLERES_DEFAULT(short)

SAFUNC void debug_MAExtent(MAExtent SCDEBUG_ARG(e)) { LOGSC("(%i, %i)", EXTENT_X(e), EXTENT_Y(e)); }
SAFUNC MAExtent _SYSCALL_CONVERT_MAExtent(int e) {
	_debug_hex(e);
	debug_MAExtent(e);
	return e;
}
SAFUNC int _SYSCALL_CONVERTRES_MAExtent(MAExtent e) { return e; }
#define _SYSCALL_HANDLERES_MAExtent _SYSCALL_HANDLERES_DEFAULT(MAExtent)

SAFUNC void debug_MAHandle(MAHandle h) { debug_int(h); }
SAFUNC MAHandle _SYSCALL_CONVERT_MAHandle(int h) {
	_debug_hex(h);
	debug_MAHandle(h);
	return h;
}
SAFUNC int _SYSCALL_CONVERTRES_MAHandle(MAHandle h) { return h; }
#define _SYSCALL_HANDLERES_MAHandle _SYSCALL_HANDLERES_DEFAULT(MAHandle)

SAFUNC void debug_MAAddress(MAAddress) {}
SAFUNC void* _SYSCALL_CONVERT_MAAddress(MAAddress a) {
	_debug_hex(a);
	debug_MAAddress(a);
	return (a + (byte*)mem_ds);
}
SAFUNC int _SYSCALL_CONVERTRES_MAAddress(const void* a) {
	return (int)((byte*)a - (byte*)mem_ds);
}
#define _SYSCALL_HANDLERES_MAAddress _SYSCALL_HANDLERES_DEFAULT(MAAddress)

#define _SYSCALL_CONVERT_MAEvent(i) (MAEvent*)_SYSCALL_CONVERT_MAAddress(i)

SAFUNC void DV_debug(const MA_DV& SCDEBUG_ARG(dv)) {
	LOGSC("0x%08x%08x", dv.MA_DV_HI, dv.MA_DV_LO); LOGSC("(%g)", dv.d);
}
#define _SYSCALL_CONVERT_double(a) _convert_double(&a);
SAFUNC double _convert_double(int* ptr) {
#ifdef DEBUGGING_MODE
	DEBUG_ASSERT(sizeof(MA_DV) == 8);
	DEBUG_ASSERT(sizeof(double) == 8);
#endif
	MA_DV dv;
	dv.MA_DV_HI = ptr[0];
	dv.MA_DV_LO = ptr[1];
	DV_debug(dv);
	return dv.d;
}
#define _SYSCALL_HANDLERES_double { MA_DV dv; dv.d = res;\
	REG(REG_r14) = dv.MA_DV_HI; REG(REG_r15) = dv.MA_DV_LO; LOGSC(" = "); DV_debug(dv); LOGSC(");\n"); }

#define _SYSCALL_HANDLERES_longlong { MA_DV dv; dv.ll = res;\
	REG(REG_r14) = dv.MA_LL_HI; REG(REG_r15) = dv.MA_LL_LO; LOGSC(");\n"); }

#define _SYSCALL_HANDLERES_void LOGSC(");\n");
