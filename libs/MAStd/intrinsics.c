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

/*
These functions return the result of shifting a left by b bits. 
*/
long __ashldi3(long a, int b) {
	return (long) ((int)a << b);
}

/*
These functions return the remainder of the signed division of a and b. 
*/
long __moddi3(long a, long b) {
	return (long) ((int)a % (int)b);
}

/*
These functions convert a to a signed long, rounding toward zero. 
*/
long __fixdfdi(double a) {
	return (long)((int)a);
}

/*
These functions convert i, a signed long, to floating point. 
*/
double __floatdidf(long i) {
	return (double)((int)i);
}

/*
These functions convert i, a signed long, to floating point. 
*/
float __floatdisf(long i) {
	return (float)((int)i);
}

/*
These functions convert a to an unsigned long, rounding toward zero. Negative values all become zero. 
*/
unsigned long __fixunssfdi(float a) {
	return (unsigned long) ((unsigned int)a);
}

/*
These functions perform a signed comparison of a and b. If a is less than b, they return 0; if a is greater than b, they return 2; and if a and b are equal they return 1. 
*/
int __cmpdi2(long a, long b) {
	int aa = (int)a;
	int bb = (int)b;
	if(aa < bb) return -1;
	else if(aa == bb) return 0;
	else return 1;
}

/*
These functions convert a to a signed long, rounding toward zero. 
*/
long __fixsfdi(float a) {
	return (long)((int)a);
}
