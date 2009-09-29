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

#define BUFSIZE 1024
char LineBuf[BUFSIZE];//, RecvBuf[BUFSIZE];
int LineBufPos = 0;//, RecvBufPos = 0, RecvBufLen = 0;
int LineBufReady = 0;

void GPS_IntLine()
{
	LineBuf[0] = 0;
	LineBufPos = 0;
	LineBufReady = 0;
}

/*{
	GPSRead(&v, 1);
	Wait(WAIT_GPSREAD);
	int res = GPSReadState();
  	if(res != 1) {
	  printlnf("Error %i", res);
	  Wait(WAIT_KEY);
	  Exit(1);
	}
	}*/
/*static inline char getMeh() {
  if(RecvBufPos == 0) {
    GPSRead(RecvBuf, BUFSIZE);
    Wait(WAIT_GPSREAD);
    int res = GPSReadState();
    if(res <= 0) {
      printlnf("Error %i", res);
      Wait(WAIT_KEY);
      Exit(1);
    }
    RecvBufLen = res;
  }
  char c = RecvBuf[RecvBufPos++];
  if(RecvBufPos >= RecvBufLen)
    RecvBufPos = 0;
  return c;
}*/

//****************************************
//
//****************************************

void GPS_FetchLine()
{
  unsigned char v=0;

  if (LineBufReady)
    return;

  int i;
  for(i=0; i<128; i++)
  {
    //v = getMeh();
    GPSRead(&v, 1);
    Wait(WAIT_GPSREAD);
    int res = GPSReadState();
    if(res != 1) {
      printlnf("Error %i", res);
      Wait(WAIT_KEY);
      Exit(1);
    }

    if (v == 13 || v == 10)
    {
      if (LineBuf[0] == '$')
      {
	LineBuf[LineBufPos++] = 0;
	LineBufReady = 1;
	return;
      }
      continue;
    }

    if (v > 31 && v < 128)
      LineBuf[LineBufPos++] = v;
  }
}

//****************************************
//
//****************************************

extern const char *FilePtr;

void GPSDecode()
{
  while(!LineBufReady)	//3vil
    GPS_FetchLine();

  {
    FilePtr = LineBuf;
    if (Token("$GPGGA"))
    {
      //println(LineBuf);
      DecodeGGA();
    }
    GPS_IntLine();
  }
}

//****************************************
//
//****************************************

/*     GGA          Global Positioning System Fix Data */
/* $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47 */

#define CONV_DEGREES 1

#define MAXAVG 10

void DecodeGGA()
{

	char FixQuality, TrackedSats, LatDir, LongDir, AltiType, GeoidAltiType;

	SkipWhiteSpace();	
	NeedToken(",");

// 123519 Fix taken at 12:35:19 UTC
	
	FixTime = GetFloat();
	
	SkipWhiteSpace();	
	NeedToken(",");

// 4807.038,N   Latitude 48 deg 07.038' N

	// get latitude [ddmm.mmmmm]

	Latitude = GetFloat();
	//printdln("lat %f", Latitude);

#if CONV_DEGREES
	{
		// convert to pure degrees [dd.dddd] format
		double degrees, minutesfrac;
		
		minutesfrac = modf(ddiv(Latitude,100), &degrees);
		//printlnf(".%i\n", d2i(dmul(minutesfrac, 100000.0)));
		Latitude = dadd(degrees, ddiv(dmul(minutesfrac, 100), 60));

		// convert to radians
//		Latitude *= (M_PI/180);
		//printdln("latc %f", Latitude);
	}
#endif

	SkipWhiteSpace();	
	NeedToken(",");

	LatDir = GetChar();

	// correct latitute for N/S

	if(LatDir == 'S')
		Latitude = dneg(Latitude);

	SkipWhiteSpace();	
	NeedToken(",");

// 01131.000,E  Longitude 11 deg 31.000' E

	Longitude = GetFloat();

#if CONV_DEGREES
	{
		// convert to pure degrees [dd.dddd] format
		double degrees, minutesfrac;
		
		minutesfrac = modf(ddiv(Longitude,100), &degrees);
		Longitude = dadd(degrees, ddiv(dmul(minutesfrac, 100), 60));

		// convert to radians
//		Longitude *= (M_PI/180);
	}
#endif

	SkipWhiteSpace();	
	NeedToken(",");

	LongDir = GetChar();

	// correct latitute for E/W

	if (LongDir == 'W')
		Longitude = dneg(Longitude);


	SkipWhiteSpace();	
	NeedToken(",");
	
/*	Fix quality:
		0 = invalid
		1 = GPS fix (SPS)
		2 = DGPS fix
		3 = PPS fix
		4 = Real Time Kinematic
		5 = Float RTK
		6 = estimated (dead reckoning) (2.3 feature)
		7 = Manual input mode
		8 = Simulation mode
*/

	FixQuality = GetNum();
	SkipWhiteSpace();	
	NeedToken(",");

//  Number of satellites being tracked

	TrackedSats = GetNum();
	SkipWhiteSpace();	
	NeedToken(",");

//     0.9          Horizontal dilution of position

	HorizDilution = GetFloat();
	SkipWhiteSpace();	
	NeedToken(",");


//     545.4,M      Altitude, Meters, above mean sea level

	Altitude = GetFloat();
	SkipWhiteSpace();	
	NeedToken(",");

	AltiType = GetChar();
	SkipWhiteSpace();	
	NeedToken(",");

//     46.9,M       Height of geoid (mean sea level) above WGS84
//                      ellipsoid

	GeoidAltitude = GetFloat();
	SkipWhiteSpace();
	NeedToken(",");

	GeoidAltiType = GetChar();
	SkipWhiteSpace();	
	NeedToken(",");
	

//     (empty field) time in seconds since last DGPS update
//     (empty field) DGPS station ID number
//     *47          the checksum data, always begins with *


	// Debug code
	
	
/*	double FixTime;
	double Latitude;
	double Longitude;
	double HorizDilution;
	double Altitude;
	double GeoidAltitude;

	char FixQuality, TrackedSats, LatDir, LongDir, AltiType, GeoidAltiType;
*/

/*	printf("Time %f LL %f,%f ",FixTime, Latitude, Longitude);

	if (FixQuality == 0)
		printf("NoFix ");
	else if (FixQuality == 1)
		printf("Fix");
	else if (FixQuality == 0)
		printf("OtherFix ");

	printf("(%d)\n", TrackedSats);*/
	//printdln("lat %f", Latitude);
	//printdln("long %f", Longitude);
	Project_LL_to_XY(Longitude, Latitude);

	if (PointCount < MAX_POINTS)
	{
		xPos[PointCount] = d2i(resx);
		yPos[PointCount] = d2i(resy);
		PointCount++;
	}


	ReadCount++;
	//LatTot = dadd(LatTot, Latitude);
	//LogTot = dadd(LogTot, Longitude);

	//UpdateScreen++;
}


double ConvertLL(double deg, double min, double sec)
{
	double res = dadd(dadd(deg, ddiv(min, 60)), ddiv(sec, 3600));
	return res;
}

//****************************************
//
//****************************************

//PARAMETRAR FÖR DIREKT PROJEKTION, SWEREF 99 -> RT 90 2.5 gon V 0:-15
// http://www.lm.se/geodesi/refsys/sweref-rt/sweref99-rt90.htm
// Projektionstyp: Gauss-Krüger (Transverse Mercator)
// http://www.lm.se/geodesi/kartprojektion/galleri/gauss.htm
// Ellipsoid: GRS 80 (SWEREF 99)
// a: 6378137
// f: 1/298.257222101
// Medelmeridian (Central meridian) : 15°48'22.624306" E (15.80628453)
// Latitud för origo (Latitude of origin) : 0° 
// Skalreduktionsfaktor (Scale on central meridian) : 1.00000561024
// x0, x-tillägg (False Northing) : -667.711 m
// y0, y-tillägg (False Easting) : 1500064.274 m

#define FalseN 				-6500614.7836
#define FalseE				100178.1808
#define ScaleReduction		0.99999425			// Scale reduction factor
#define CentralMaridian		18.05779 			//15.80628453
#define Latitude0			(CentralMaridian * M_PI_D180)

//****************************************
//
//****************************************

void Project_LL_to_XY(double Latitude, double Longitude) {
  double xip;
  double etap;
  double LonX;
  double dLatitude;
  double sinLon, sinLon2, sinLon4, sinLon6;

  // Convert lat/long to radians

  Latitude = dmul(Latitude, M_PI_D180);
  Longitude = dmul(Longitude, M_PI_D180);

  sinLon = sin(Longitude);
  sinLon2 = dmul(sinLon, sinLon);
  sinLon4 = dmul(sinLon2, sinLon2);
  sinLon6 = dmul(sinLon4, sinLon2);

  LonX = dsub(Longitude, dmul(dmul(sinLon, cos(Longitude)),
    dadd(pA,
      dadd(dmul(pB, sinLon2),
	dadd(dmul(pC, sinLon4),
	  dmul(pD, sinLon6))))));

  dLatitude = dsub(Latitude, Latitude0);

  xip = atan2(tan(LonX), cos(dLatitude));
  etap = atanh(dmul(cos(LonX), sin(dLatitude)));

  double sraa = dmul(ScaleReduction, aa);
  double xip2 = dmul(2, xip);
  double xip4 = dmul(4, xip);
  double xip6 = dmul(6, xip);
  double xip8 = dmul(8, xip);
  double etap2 = dmul(2, etap);
  double etap4 = dmul(4, etap);
  double etap6 = dmul(6, etap);
  double etap8 = dmul(8, etap);

  resx = dadd(dmul(sraa, dadd(xip,
    dadd(dmul(dmul(beta1, sin(xip2)), cosh(etap2)),
    dadd(dmul(dmul(beta2, sin(xip4)), cosh(etap4)),
    dadd(dmul(dmul(beta3, sin(xip6)), cosh(etap6)),
    dmul(dmul(beta4, sin(xip8)), cosh(etap8))))))), FalseN);

  resy = dadd(dmul(sraa, dadd(etap,
    dadd(dmul(dmul(beta1, cos(xip2)), sinh(etap2)),
    dadd(dmul(dmul(beta2, cos(xip4)), sinh(etap4)),
    dadd(dmul(dmul(beta3, cos(xip6)), sinh(etap6)),
    dmul(dmul(beta4, cos(xip8)), sinh(etap8))))))), FalseE);
}


//****************************************
//
//****************************************

void LL_to_ST74()
{
	//int nnn;
	
	double lat = ConvertLL(59, 27, 0);
	double lon = ConvertLL(18, 12, 0);
	
	//println("Hello");
	
	//for(nnn=0;nnn<1000000;nnn++)
		Project_LL_to_XY(lon,lat);
	
	//printlnf("Lat = %f : Lon = %f\n", lat, lon);
	printlnf("X = %i", d2i(resx));
	printlnf("Y = %i", d2i(resy));


	//printf("Central Maridian= %f\n", ConvertLL(18, 03, 28.044));
}
