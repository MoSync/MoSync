/* Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

#include <MAUtil/Moblet.h>
#include <conprint.h>

using namespace MAUtil;

class MyMoblet : public Moblet {
public:
	MyMoblet() {
		// todo: initialize
		int mem = 1000000000;
		MAHandle resource = maCreatePlaceholder();

		bool testSucceded = false;

		printf("RESOURCE ALLOC TEST\n\n");

		while(mem > 100)
		{
			int result = maCreateData(resource, mem);
			if(result == RES_OUT_OF_MEMORY)
			{
				printf("FAILED - allocating %d bytes\n", mem);
				mem = mem/2;
			}
			else
			{
				testSucceded = true;
				printf("SUCEESS - allocated %d bytes\n", mem);
				break;
			}
		}


		if(testSucceded)
		{
			printf("\nREALLOCATING RESOURCE\n\n");
			for ( int i = 0; i < 100; i++)
			{
				maDestroyObject(resource);
				int result = maCreateData(resource, mem);
				if(result == RES_OUT_OF_MEMORY)
				{
					printf("FAILED REALLOCATING - step %d\n", i);
					testSucceded = false;
					break;
				}
			}

		}

		if(testSucceded)
			printf("SUCCESS - ALL MEMORY OPERATION SUCCEDED!\n");
		else
			printf("UNABLE TO ALLOCATE ANY MEMORY!\n");

	}

	void keyPressEvent(int keyCode, int nativeCode) {
		// todo: handle key presses
		if(keyCode == MAK_0 || keyCode == MAK_BACK)
			maExit(0);
	}

	void keyReleaseEvent(int keyCode, int nativeCode) {
		// todo: handle key releases
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
