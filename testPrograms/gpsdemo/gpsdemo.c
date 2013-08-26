/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "../maapi.h"
#include "../shared/misc.h"
#include "../shared/print.h"
#include "token.h"
#include "gps.h"

void MyPlot(int x, int y);

int main() {
  const int scrHeight = EXTENT_Y(GetScrSize());
  const int scrWidth = EXTENT_X(GetScrSize());
  int res;

  SetColor(0);

  println("Opening GPS...");
//open:
  GPSOpen();
  Wait(WAIT_GPSOPEN);
  res = GPSOpenState();
  if(res != 1) {
    printlnf("Error %i", res);
    //goto open;
  }
  //println("Done.");

  //temp init
  aa = 6367449.145771048;
  beta1 = 0.00083773182473443437;
  beta2 = 7.6085277888261638e-007;
  beta3 = 1.1976380191731537e-009;
  beta4 = 2.4433762425103682e-012;
  pA = 0.0066943800229007869;
  pB = 3.7295602110452982e-005;
  pC = 2.5925275189740247e-007;
  pD = 1.9716989472842215e-009;

  LL_to_ST74();
  //test();


  //plotter code starts here
  int offsetX, offsetY;
  GPSDecode();
  offsetX = xPos[0];
  offsetY = yPos[0];
  printlnf("oX %i", offsetX);
  printlnf("oY %i", offsetY);
  println("PAK to start, down to quit.");
  Wait(WAIT_KEY);

  offsetX -= scrWidth / 2;
  offsetY -= scrHeight / 2;

  //First Edition
  /*ClearScreen();
  SetColor(-1);	//white
  while(1) {
    Plot(xPos[PointCount-1] - offsetX, yPos[PointCount-1] - offsetY);
    UpdateScreen();
    GPSDecode();
    if(GetKeys() & MAK_DOWN)
      break;
  }*/

  //Second Edition
  while((GetKeys() & MAK_DOWN) == 0) {
    //ClearScreen();
    int i;
    for(i=2; i<PointCount; i++) {
      static const int nColors = 256, nCycles = 4;
      int time = ((i-1) * (nColors * nCycles)) / (PointCount - 2);
      int cycle = (time / nColors) % nCycles, subtime = time % nColors;
      switch(cycle) {
      case 0:	SetColor(RGB(0, 255, subtime));		break;
      case 1:	SetColor(RGB(0, 255-subtime, 255));	break;
      case 2:	SetColor(RGB(subtime, 0, 255));		break;
      case 3:	SetColor(RGB(255, 0, 255-subtime));	break;
      }
      MyPlot(xPos[i] - offsetX, yPos[i] - offsetY);
    }
    if(PointCount >= 2) {
      SetColor(RGB(0,255,0));
      MyPlot(xPos[1] - offsetX, yPos[1] - offsetY);
    }
    if(PointCount >= 1) {
      SetColor(-1); //white
      MyPlot(xPos[0] - offsetX, yPos[0] - offsetY);
    }
    UpdateScreen();
    GPSDecode();
  }
  //plotter code ends here

  SetColor(0);

  //int oldKeys = GetKeys();
  int oldPC = 0;
  while(1) {
    GPSDecode();
    //if(PointCount != oldPC - 1 && PointCount != oldPC)
      //break;
    if(PointCount != oldPC) {
      printlnf("PC %i", PointCount);
      oldPC = PointCount;
      printlnf("X %i", xPos[PointCount-1]);
      printlnf("Y %i", yPos[PointCount-1]);
    }
    /*int newKeys = GetKeys();
    int downedKeys = newKeys & (~oldKeys);
    oldKeys = newKeys;*/
  }

//end:
  println("PAK to exit");
  Wait(WAIT_KEY);
  Exit(0);
  return 0;
}

void MyPlot(int x, int y) {
  //printlnf("x %i", x);
  //printlnf("y %i", y);
  Plot(x, y);
}

#include "../shared/misc.c"
#include "../shared/print.c"
#include "token.c"
#include "gps.c"
