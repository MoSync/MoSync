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

void GPSDecode();
void DecodeGGA();
void Project_LL_to_XY(double Latitude, double Longitude);
void LL_to_ST74();

// ST71 vars

double pA,pB,pC,pD,resx,resy,aa;
double beta1,beta2,beta3,beta4;

double firstx,firsty;

// WGS84 vars

//double LatTot, LogTot;
int ReadCount = 0;
double FixTime, Latitude,Longitude,HorizDilution,Altitude,GeoidAltitude;

//int OriginX = 256;
//int OriginY = 256;
//double Rotation = 0.0;
//double Scale = 1.0;

#define MAX_POINTS 4096
int xPos[MAX_POINTS];
int yPos[MAX_POINTS];
int PointCount = 0;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define M_PI_D180 ((double)((double) 3.14159265358979323846 / (double) 180))

