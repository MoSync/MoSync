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

#include <MTXml/MTXml.h>
#include <conprint.h>
#include <maassert.h>

static char sDocument[] = "<devices>"
"<!-- New devices -->"
"	<device>"
"		<identifier>MobileSorcery/Emulator</identifier>"
"		<features>hasPointerEvents</features> "
"		<capability name=\"JavaPackage\" value=\"mmapi, btapi\" />"
"		<capability name=\"ScreenSize\" value=\"240x320\" />"
"		<capability name=\"JavaPlatform\" value=\"MIDP/2.0\" />"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.1\" />"
" 	</device>"
""
"	<device>"
"		<identifier>Motorola/ROKR Z6</identifier>"
"		<capability name=\"JavaPackage\" value=\"mmapi\" />"
"		<capability name=\"ScreenSize\" value=\"240x320\" />"
"		<capability name=\"BitsPerPixel\" value=\"16\" />"
"		<capability name=\"JavaPlatform\" value=\"MIDP/2.0\" />"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.1\" />"
"	</device>"
""
""
"<!-- Changed devices -->"
"	<device>"
"		<identifier>HTC/Excalibur, HTC/S620, HTC/S621</identifier>"
"		<features>hasCommandKeyEvents</features>"
"		<capability name=\"ScreenSize\" value=\"320x240\" />"
"		<capability name=\"JavaPackage\" value=\"bluetooth\"/>"
"	</device>"
""
"	<device>"
"		<identifier>HTC/Vox, HTC/S720</identifier>"
"		<features>hasCommandKeyEvents</features> "
"		<capability name=\"JavaPackage\" value=\"bluetooth\"/>"
"	</device>"
""
"	<device>"
"		<identifier>HTC/Titan</identifier>"
"		<capability name=\"JavaPackage\" value=\"bluetooth\"/>"
"	</device>"
""
"	<device>"
"		<identifier>HTC/Tornado</identifier>"
"		<capability name=\"JavaPackage\" value=\"bluetooth\"/>"
"	</device>"
""
"	<device>"
"		<identifier>HTC/Wizard</identifier>"
"		<capability name=\"JavaPackage\" value=\"bluetooth\"/>"
"	</device>"
""
""
"	<device>"
"		<identifier>Motorola/C261</identifier>"
"		<capability name=\"ScreenSize\" value=\"128x160\" />"
"	</device>"
""
"	<device>"
"		<identifier>O2/XDAII</identifier>"
"		<capability name=\"ScreenSize\" value=\"240x320\" />"
"	</device>"
""
"	<device>"
"		<identifier>Motorola/SLVR</identifier>"
"		<features>doubleBuffering, hasCamera</features>"
"	</device>"
""
"	<device>"
"		<identifier>Motorola/V1050</identifier>"
"		<features>doubleBuffering, hasCamera</features>"
"	</device>"
""
"	<device>"
"		<identifier>Motorola/V360</identifier>"
"		<features>doubleBuffering, hasCamera</features>"
"	</device>"
""
"	<device>"
"		<identifier>Sony-Ericsson/P1</identifier> "
"		<features>hasCamera, hasVideo, doubleBuffering, hasCommandKeyEvents, hasPointerEvents</features> "
"	</device>"
""
"	<device>"
"		<identifier>Sony-Ericsson/P990i</identifier>"
"		<capability name=\"MoSyncBugs\" value=\"backlightFlashes\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Nokia/5700_XpressMusic</identifier>"
"		<groups>Series60E3FP1</groups>"
"		<capability name=\"IconSize\" value=\"53x53,55x55\"/>"
"	</device>"
""
""
"<!-- Missing attributes -->"
"	<device>"
"		<identifier>Motorola/V220</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.0\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Motorola/V180</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.0\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Motorola/V80</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.0\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Motorola/T725</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.0\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Motorola/V300</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.0\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Motorola/V400</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.0\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Motorola/V500</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.0\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Motorola/V600</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.0\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Motorola/A760</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.0\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Nokia/6660</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"nonexistent phone\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Nokia/7610</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.0\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Nokia/6620</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.0\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Nokia/6600</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.0\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Nokia/5140</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.1\"/>"
"	</device>"
""
"	<device>"
"		<identifier>Nokia/3220</identifier>"
"		<capability name=\"JavaConfiguration\" value=\"CLDC/1.1\"/>"
"	</device>"
""
""
""
"	<!-- Imate devices -->"
"	<device>"
"  		<identifier>Imate/JasJar</identifier>"
"  		<capability name=\"ScreenSize\" value=\"640x480\" />"
"	</device>"
"	 "
"	<device>"
"		<identifier>Imate/K-Jam</identifier>"
"		<capability name=\"ScreenSize\" value=\"240x320\"/>"
"	</device>"
"	"
"	<device>"
"		<identifier>Imate/Jam</identifier>"
"		<capability name=\"ScreenSize\" value=\"240x320\" />"
"	</device>"
"	"
"	<device>"
"		<identifier>Imate/Jamin</identifier>"
"		<capability name=\"ScreenSize\" value=\"240x320\" />"
"	</device>"
"	"
"	<device>"
"		<identifier>Imate/SP3, Imate/SP3K</identifier>"
"		<capability name=\"ScreenSize\" value=\"176x220\" />"
"	</device>"
""
"</devices>"
;

static void tagStart(MTX_CONTEXT* context, const char* name, int len) {
	printf("s %i: \"%s\"\n", len, name);
}
static void tagAttr(MTX_CONTEXT* context, const char* attrName, const char* attrValue) {
	printf("a \"%s\": \"%s\"\n", attrName, attrValue);
}
static void tagData(MTX_CONTEXT* context, const char* data, int len) {
	printf("d %i: \"%s\"\n", len, data);
}
static void tagEnd(MTX_CONTEXT* context, const char* name, int len) {
	printf("e %i: \"%s\"\n", len, name);
}
static void dataRemains(MTX_CONTEXT* context, const char* data, int len) {
	printf("r %i: \"%s\"\n", len, data);
}
static void parseError(MTX_CONTEXT* context) {
	printf("parseError\n");
}
static void emptyTagEnd(MTX_CONTEXT* context) {
	printf("emptyTagEnd\n");
}

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;
	printf("Hello World!\n");

	MTX_CONTEXT c;
	c.tagStart = tagStart;
	c.tagAttr = tagAttr;
	c.tagData = tagData;
	c.tagEnd = tagEnd;
	c.dataRemains = dataRemains;
	c.parseError = parseError;
	c.emptyTagEnd = emptyTagEnd;
	mtxStart(&c);
	mtxFeed(&c, sDocument);

	FREEZE;
}
