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

