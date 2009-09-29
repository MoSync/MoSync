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

#include "common.h"
#include <ma.h>
#include <mastdlib.h>
#include <mastring.h>
#include <conprint.h>
#include <madmath.h>

class MathTestCase : public TestCase {

public:
	MathTestCase() : TestCase("Math Test") {
	}

	void trigTest() {
		double sinTable[32] = {0.0, 0.19509032179607727, 0.38268343195052273, 0.5555702324599515, 0.7071067805519557, 0.8314696116793008, 0.9238795319961289, 0.9807852800968344, 1.0, 0.9807852807971682, 0.9238795333698833, 0.8314696136736829, 0.7071067830903227, 0.5555702354447553, 0.382683435267059, 0.19509032531689338, 3.5897930298416118E-9, -0.19509031827526108, -0.38268342863398636, -0.5555702294751477, -0.7071067780135887, -0.8314696096849186, -0.9238795306223747, -0.9807852793965004, -1.0, -0.9807852814975022, -0.9238795347436375, -0.8314696156680653, -0.7071067856286897, -0.5555702384295594, -0.38268343858359527, -0.19509032883771};
		double tanTable[32] = {0.0, 0.098491403243895, 0.1989123671464188, 0.3033466832398309, 0.414213561847382, 0.5345111352296347, 0.6681786369457049, 0.8206787895145049, 0.9999999982051034, 1.218503523079299, 1.4966057590310156, 1.8708684062362453, 2.4142135531808555, 3.2965581916316093, 5.027339450861404, 10.15317021246028, 5.571351839435277E8, -10.153170586110596, -5.027339545180137, -3.2965582342327457, -2.4142135776934954, -1.8708684223908474, -1.49660577066133, -1.2185035319990407, -1.0000000053846894, -0.8206787955220729, -0.6681786421382064, -0.5345111398450395, -0.4142135660530861, -0.303346687159954, -0.19891237087824604, -0.09849140686851124};

		double pi = 3.14159265;
		//double EPS = 0.0001;
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define EPS MAX(fabs(correct / 100000), 0.00001)
		double delta = 2*pi/32;
		int numFailed = 0;
		double ang = 0;

		for(int i = 0; i < 32; i++) {
			double correct = sinTable[i];
			double test = sin(ang);
			if((test>=correct-EPS)&&(test<=correct+EPS)) {
			}
			else {
				numFailed++;
				printf("sin(%f)=%.f %i=%f\n", ang, test, i, correct);
			}
			ang+=delta;
		}
		assert("testing sin()", numFailed==0);

		numFailed = 0;
		ang = 0;
		for(int i = 0; i < 32; i++) {
			double correct = sinTable[(i+8)&31];
			double test = cos(ang);
			if((test>=correct-EPS)&&(test<=correct+EPS)) {
			}
			else {
				numFailed++;
				printf("cos(%f)=%.f %i=%f\n", ang, test, i, correct);
			}
			ang+=delta;
		}
		assert("testing cos()", numFailed==0);

		numFailed = 0;
		ang = 0;
		delta = pi/32;
		for(int i = 0; i < 32; i++) {
			double correct = tanTable[i];
			double test = tan(ang);
			if((test>=correct-EPS)&&(test<=correct+EPS)) {
			}
			else {
				numFailed++;
				printf("tan(%f)=%.f %i=%f\n", ang, test, i, correct);
			}
			ang+=delta;
		}
		assert("testing tan()", numFailed==0);
	}

	void sqrtTest() {
		double sqrtTable[32] = {0.0, 1.0, 1.4142135623730951, 1.7320508075688772, 2.0, 2.23606797749979, 2.449489742783178, 2.6457513110645907, 2.8284271247461903, 3.0, 3.1622776601683795, 3.3166247903554, 3.4641016151377544, 3.605551275463989, 3.7416573867739413, 3.872983346207417, 4.0, 4.123105625617661, 4.242640687119285, 4.358898943540674, 4.47213595499958, 4.58257569495584, 4.69041575982343, 4.795831523312719, 4.898979485566356, 5.0, 5.0990195135927845, 5.196152422706632, 5.291502622129181, 5.385164807134504, 5.477225575051661, 5.5677643628300215};

		int numFailed = 0;
		//double EPS = 0.0001;

		for(int i = 0; i < 32; i++) {
			double correct = sqrtTable[i];
			double test = sqrt((double)i);
			if((test>=correct-EPS)&&(test<=correct+EPS)) {
			}
			else {
				numFailed++;
				printf("sqrt( %i)=%.f %f\n", i, test, correct);
			}
		}
		assert("testing sqrt()", numFailed==0);
	}

	void doubleTest() {
		double a, b;

		a = 1.0;
		b = 2.0;
		assert("testing dcmp()!=0", ((memcmp(&a, &b, sizeof(double)))!=0)&&(dcmp(a, b)!=0));

		a = 2.0;
		b = 2.0;
		assert("testing dcmp()==0", ((memcmp(&a, &b, sizeof(double)))==0)&&(dcmp(a, b)==0));

		a = __adddf3(1.0, 2.0);
		b = 3.0;
		assert("testing 1.0+2.0==3.0", dcmp(a, b)==0);

		a = __adddf3(1.0, 2.0);
		b = 2.0;
		assert("testing 1.0+2.0!=2.0", dcmp(a, b)!=0);

		a = __subdf3(2.0, 1.0);
		b = 1.0;
		assert("testing 2.0-1.0==1.0", dcmp(a, b)==0);

		a = __subdf3(2.0, 1.0);
		b = 4.0;
		assert("testing 2.0-1.0!=4.0", dcmp(a, b)!=0);

		a = __muldf3(2.0, 4.0);
		b = 8.0;
		assert("testing 2.0*4.0==8.0", dcmp(a, b)==0);

		a = __muldf3(2.0, 4.0);
		b = 7.0;
		assert("testing 2.0*4.0!=7.0", dcmp(a, b)!=0);

		a = __negdf2(2.0);
		b = -2.0;
		assert("testing dneg(2.0)==-2.0", dcmp(a, b)==0);

		a = __negdf2(2.0);
		b = -3.0;
		assert("testing dneg(2.0)!=-3.0", dcmp(a, b)!=0);

		a = __divdf3(4.0, 2.0);
		b = 2.0;
		assert("testing 4.0/2.0==2.0", dcmp(a, b)==0);

		a = __divdf3(4.0, 3.0);
		b = 2.0;
		assert("testing 4.0/3.0!=2.0", dcmp(a, b)!=0);
	}

	void start() {
		printf("running math tests\n");
		doubleTest();
		trigTest();
		sqrtTest();
		suite->runNextCase();
	}

};

void addMathTests(TestSuite* suite) {
	suite->addTestCase(new MathTestCase());
}
