/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#ifdef DEBUG_SYSCALLS
final void _debug_hex(int hex) {
	String s = Integer.toHexString(hex);
	/*char[] z = new char[8-s.length()];
	for(int i=0; i<z.length; i++)
	z[i] = '0';
	DEBUG("0x"+z+s);*/
	DEBUG_SC("0x");
	for(int i=0; i<8-s.length(); i++) {
		DEBUG_SC("0");
	}
	DEBUG_SC(s);
}
#else
#define _debug_hex(hex)
#endif

/*void debug_MAPoint2d(MAPoint2d p) { DEBUG_SC("("+p.x+","+p.y+")"); }
MAPoint2d _SYSCALL_CONVERT_MAPoint2d(int ptr) throws Exception {
_debug_hex(ptr);
MAPoint2d p = new MAPoint2d(RINT(ptr), RINT(ptr+4));
debug_MAPoint2d(p);
return p;
}

void debug_MARect(MARect p) { DEBUG_SC("("+p.left+","+p.top+","+p.width+","+p.height+")"); }
MARect _SYSCALL_CONVERT_MARect(int ptr) throws Exception {
_debug_hex(ptr);				
MARect r = new MARect(RINT(ptr),RINT(ptr+4),RINT(ptr+8),RINT(ptr+12));
debug_MARect(r);
return r;
}*/
#define _SYSCALL_CONVERT_MAPoint2d _SYSCALL_CONVERT_MAAddress
#define _SYSCALL_CONVERT_MARect _SYSCALL_CONVERT_MAAddress
#define _SYSCALL_CONVERT_MAEVENT _SYSCALL_CONVERT_MAAddress

#define debug_MAString(a)
#ifdef DEBUG_SYSCALLS
final void debug_String(String str) { DEBUG_SC("("+str+")"); }
#else
#define debug_String(str)
#endif
/*String _SYSCALL_CONVERT_String(int str) throws Exception {
_debug_hex(str);
String res = new String();
char c;
while(true) {
byte ret;
RBYTE(str++, ret);
c = (char) ret;

if(c == 0)
break;
res += c;
}
debug_String(res);
return res;
}*/

//STATIC char[] gStringConvertBuffer = new char[256];
STATIC StringBuffer gStringBuffer = new StringBuffer(256);
final String _SYSCALL_CONVERT_MAString(int str) throws Exception {
	_debug_hex(str);
	if(str == 0)
		return null;
	gStringBuffer.setLength(0);
	INIT_MEMDS;;
	for(;;) {
		char b;
		RCHAR(str++, b);	//TODO: optimize by reading ints at a time.
		if(b == 0)
			break;
		gStringBuffer.append(b);
	}
	String result = new String(gStringBuffer);
	debug_String(result);
	return result;
}

final String _SYSCALL_CONVERT_MAWString(int str) throws Exception {
	_debug_hex(str);
	if(str == 0)
		return null;
	gStringBuffer.setLength(0);
	INIT_MEMDS;;
	for(;;) {
		short b;
		RSHORT(str, b);
		if(b == 0)
			break;
		str += 2;
		gStringBuffer.append((char)b);
	}
	String result = new String(gStringBuffer);
	debug_String(result);
	return result;
}

//int _SYSCALL_CONVERTRES_String(char* str) { return int(str - (char*)mem_ds); }

#ifdef DEBUG_SYSCALLS
final void debug_int(int i) { DEBUG_SC("("+i+")"); }
final int _SYSCALL_CONVERT_int(int i) {
	_debug_hex(i);
	debug_int(i);
	return i;
}
final int _SYSCALL_CONVERTRES_int(int i) { return i; }
#else
#define debug_int(i)
#define _SYSCALL_CONVERT_int(i) i
#define _SYSCALL_CONVERTRES_int(i) i
#endif	//DEBUG_SYSCALLS

#ifdef DEBUG_SYSCALLS
final void debug_uint(int i) { DEBUG_SC("("+(((long)i) & 0xFFFFFFFF)+")"); }
final long _SYSCALL_CONVERT_uint(int i) {
	_debug_hex(i);
	debug_uint(i);
	return i;
}
final int _SYSCALL_CONVERTRES_uint(long i) { return (int)i; }
#else
#define debug_uint(i)
#define _SYSCALL_CONVERT_uint(i) i
#define _SYSCALL_CONVERTRES_uint(i) (int)i
#endif	//DEBUG_SYSCALLS

#ifdef DEBUG_SYSCALLS
final void debug_short(short i) { DEBUG_SC("("+i+")"); }
final short _SYSCALL_CONVERT_short(int i) {
	_debug_hex(i);
	short s = (short)i;
	debug_short(s);
	return s;
}
final short _SYSCALL_CONVERTRES_short(short i) { return i; }
#else
#define debug_short(i)
#define _SYSCALL_CONVERT_short(i) i
#define _SYSCALL_CONVERTRES_short(i) i
#endif	//DEBUG_SYSCALLS

#ifdef DEBUG_SYSCALLS
final void debug_MAExtent(MAExtent e) { DEBUG_SC("("+EXTENT_X(e)+", "+EXTENT_Y(e)+")"); }
final MAExtent _SYSCALL_CONVERT_MAExtent(int e) {
	_debug_hex(e);
	debug_MAExtent(e);
	return e;
}
final int _SYSCALL_CONVERTRES_MAExtent(MAExtent e) { return e; }
#else
#define debug_MAExtent(e)
#define _SYSCALL_CONVERT_MAExtent(i) i
#define _SYSCALL_CONVERTRES_MAExtent(i) i
#endif	//DEBUG_SYSCALLS

#ifdef DEBUG_SYSCALLS
final void debug_MAHandle(MAHandle h) { debug_int(h); }
final MAHandle _SYSCALL_CONVERT_MAHandle(int h) {
	_debug_hex(h);
	debug_MAHandle(h);
	return h;
}
final int _SYSCALL_CONVERTRES_MAHandle(MAHandle h) { return h; }
#else
#define debug_MAHandle(h)
#define _SYSCALL_CONVERT_MAHandle(i) i
#define _SYSCALL_CONVERTRES_MAHandle(i) i
#endif	//DEBUG_SYSCALLS

#ifdef DEBUG_SYSCALLS
final void debug_MAAddress(Address a) {}
final Address _SYSCALL_CONVERT_MAAddress(int a) {
	_debug_hex(a);
	debug_MAAddress((Address)a);
	return a;
}
final int _SYSCALL_CONVERTRES_MAAddress(Address a) { return a; }
#else
#define debug_MAAddress(a)
#define _SYSCALL_CONVERT_MAAddress(i) i
#define _SYSCALL_CONVERTRES_MAAddress(i) i
#endif	//DEBUG_SYSCALLS

#ifdef DEBUG_SYSCALLS
final void debug_long(long l) throws Exception {
	if(l >= 0) {
		String s = Long.toString(l, 16);
		DEBUG_SC("0x");
		for(int i=0; i<16-s.length(); i++) {
			DEBUG_SC("0");
		}
		DEBUG_SC(s);
	} else {
		DEBUG_SC(""+l);
	}
}
#else
#define debug_long(l)
#endif	//DEBUG_SYSCALLS

#ifndef MA_PROF_SUPPORT_CLDC_10
#ifdef DEBUG_SYSCALLS
final void debug_double(long l, double a) {
	if(l >= 0) {
		String s = Long.toString(l, 16);
		DEBUG_SC("0x");
		for(int i=0; i<16-s.length(); i++) {
			DEBUG_SC("0");
		}
		DEBUG_SC(s);
	} else {
		DEBUG_SC(""+l);
	}
	DEBUG_SC("("+a+")");
	if(Double.isNaN(a)) {
		BIG_PHAT_ERROR;
	}
}
#else
#define debug_double(l, d)
#endif	//DEBUG_SYSCALLS

#ifdef DEBUG_SYSCALLS
final void debug_float(int i) throws Exception {
	float f = _SYSCALL_CONVERT_float(i);
	if(i >= 0) {
		String s = Integer.toString(i, 16);
		DEBUG_SC("0x");
		for(int j=0; j<16-s.length(); j++) {
			DEBUG_SC("0");
		}
		DEBUG_SC(s);
	} else {
		DEBUG_SC(""+i);
	}
	DEBUG_SC("("+f+")");
	if(Float.isNaN(f)) {
		BIG_PHAT_ERROR;
	}
}
#else
#define debug_float(i)
#endif
#if 0	//unused
final float _SYSCALL_CONVERT_float(int i) throws Exception {
	_debug_hex(i);
	float f = Float.intBitsToFloat(i);
	debug_float(i);
	return f;
}
#endif
#define _SYSCALL_CONVERTRES_float(f) Float.floatToIntBits(f)

#else	// MA_PROF_SUPPORT_CLDC_10
#define _SYSCALL_CONVERTRES_float(f) (f)
#define debug_float(i)
#define debug_double(l, d)
#endif	// MA_PROF_SUPPORT_CLDC_10

//#ifdef MOSYNC_STATIC_JAVA
#define _SYSCALL_CONVERT_long(hi, lo) ints2long(lo, hi)
/*
#else
#define _SYSCALL_CONVERT_long(reg) ints2long(REG(reg + 1), REG(reg))
#endif
*/
final long ints2long(int hiw, int low) throws Exception {
	//DEBUG_SC("("+hiw+","+low+")->");
	//DEBUG_SC("("+UINT(low)+")");
	long l = (((long)hiw) << 32) | UINT(low);
	return l;
}

#define _SYSCALL_CONVERT_ulong(i) (i)

#define _SYSCALL_HANDLERES_DEFAULT(type) REG(REG_r14) = res;\
	LOGSC(") returned "); _debug_hex(res); debug_##type(res); LOGSC("\n");

#define _SYSCALL_HANDLERES_MAAddress _SYSCALL_HANDLERES_DEFAULT(MAAddress)
#define _SYSCALL_HANDLERES_Address _SYSCALL_HANDLERES_DEFAULT(MAAddress)
#define _SYSCALL_HANDLERES_int _SYSCALL_HANDLERES_DEFAULT(int)
#define _SYSCALL_HANDLERES_uint _SYSCALL_HANDLERES_DEFAULT(uint)
#define _SYSCALL_HANDLERES_MAString _SYSCALL_HANDLERES_DEFAULT(MAString)
#define _SYSCALL_HANDLERES_float _SYSCALL_HANDLERES_DEFAULT(float)
#define _SYSCALL_HANDLERES_MAExtent _SYSCALL_HANDLERES_DEFAULT(MAExtent)
#define _SYSCALL_HANDLERES_MAHandle _SYSCALL_HANDLERES_DEFAULT(MAHandle)
#define _SYSCALL_HANDLERES_void LOGSC(")\n");

#define _SYSCALL_HANDLERES_longlong \
	int hiw = (int)res; int low = ((int)(res >> 32));\
	REG(REG_r14) = hiw; REG(REG_r15) = low; debug_long(res);

#define _SYSCALL_HANDLERES_double _SYSCALL_HANDLERES_longlong
