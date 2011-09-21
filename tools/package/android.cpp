/* Copyright (C) 2010 MoSync AB

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

#include "packagers.h"
#include "util.h"
#include "helpers/mkdir.h"
#include "filelist/filelist.h"
#include <fstream>
#include <sstream>
#include <errno.h>
#include <stdlib.h>

using namespace std;

static void writeManifest(const char* filename, const string& packageName,
	const SETTINGS& s, const RuntimeInfo& ri);
static void writeMain(const char* filename, const SETTINGS& s, const RuntimeInfo& ri);
static void writeStrings(const char* filename, const SETTINGS& s, const RuntimeInfo& ri);

static void writePermission(ostream& stream, const char* perm);
static string appNameToPackageName(const string& appName);
static string packageNameToByteCodeName(const string& packageName);

void packageAndroid(const SETTINGS& s, const RuntimeInfo& ri) {
	testDst(s);
	testProgram(s);
	testName(s);
	testVendor(s);

	// copy program and resource files to add/assets/*.mp3
	// build AndroidManifest.xml, res/layout/main.xml, res/values/strings.xml
	// opt: generate res/drawable/icon.png
	// run mosync/bin/android/aapt
	// unzip Runtime class files
	// move libmosync.so
	// run android/dx.jar
	// run android/apkbuilder.jar
	// run android/tools-stripped.jar, for signing

	string dstDir = string(s.dst);

	// copy program and resource files to add/assets/*.mp3
	string add = dstDir + "/add";
	string assets = add + "/assets";
	string programMp3 = assets + "/program.mp3";

	_mkdir(add.c_str());
	_mkdir(assets.c_str());
	copyFile(programMp3.c_str(), s.program);
	if(s.resource) {
		string resMp3 = assets + "/resources.mp3";
		copyFile(resMp3.c_str(), s.resource);
	}

	// build AndroidManifest.xml, res/layout/main.xml, res/values/strings.xml
	string res = dstDir + "/res";
	string layout = res + "/layout";
	string values = res + "/values";
	_mkdir(res.c_str());
	_mkdir(layout.c_str());
	_mkdir(values.c_str());

	const string packageName = "com.mosync.app_" + appNameToPackageName(s.name);

	string manifestXml = dstDir + "/AndroidManifest.xml";
	writeManifest(manifestXml.c_str(), packageName, s, ri);
	string mainXml = layout + "/main.xml";
	writeMain(mainXml.c_str(), s, ri);
	string stringsXml = values + "/strings.xml";
	writeStrings(stringsXml.c_str(), s, ri);

	// run mosync/bin/android/aapt
	std::ostringstream cmd;
	string resourcesAp_ = dstDir + "/resources.ap_";

	cmd.str("");
	cmd <<""<<mosyncdir()<<"/bin/android/aapt package -f -M \""<<manifestXml<<"\""
		" -F \""<<resourcesAp_<<"\""
		" -I \""<<mosyncdir()<<"/bin/android/android-"<<ri.androidVersion<<".jar\""<<
		" -S \""<<res<<"\""
		" -0 -A \""<<add<<"\"";
	sh(cmd.str().c_str());

	// unzip Runtime class files
	string classes = dstDir + "/classes";
	_mkdir(classes.c_str());

	cmd.str("");
	cmd <<""<<mosyncdir()<<"/bin/unzip"
		" -o -q \""<<ri.path<<"MoSyncRuntime"<<(s.debug ? "D" : "")<<".zip\""
		" -d \""<<classes<<"\"";
	sh(cmd.str().c_str());

	// move libmosync.so
	string addlib = dstDir + "/addlib";
	string armeabi = addlib + "/armeabi";
	_mkdir(addlib.c_str());
	_mkdir(armeabi.c_str());

	string classesSo = classes + "/libmosync.so";
	string armeabiSo = armeabi + "/libmosync.so";
	remove(armeabiSo.c_str());
	renameFile(armeabiSo, classesSo);

	// run android/dx.jar
	string classesDex = classes + "/classes.dex";
	cmd.str("");
	cmd <<"java -jar \""<<mosyncdir()<<"/bin/android/dx.jar\""<<
		" --dex --patch-string com/mosync/java/android"
		" "<<packageNameToByteCodeName(packageName)<<""
		" \"--output="<<classesDex<<"\" \""<<classes<<"\"";
	sh(cmd.str().c_str());

	// run android/apkbuilder.jar
	string unsignedApk = dstDir + "/" + string(s.name) + "_unsigned.apk";
	cmd.str("");
	cmd <<"java -jar \""<<mosyncdir()<<"/bin/android/apkbuilder.jar\""
		" \""<<unsignedApk<<"\""
		" -u -z \""<<resourcesAp_<<"\""
		" -f \""<<classesDex<<"\""
		" -nf \""<<addlib<<"\"";
	sh(cmd.str().c_str());

	// todo: support non-default keys
	string keystore = mosyncdir() + string("/etc/mosync.keystore");
	string alias = "mosync.keystore";
	string storepass = "default";
	string keypass = "default";
	if(isDirectory(keystore.c_str()) < 0) {	// does not exist
		// create default keystore
		cmd.str("");
		cmd << "keytool -genkeypair -v -alias "<<alias<<
			" -validity 3650 -keypass "<<keypass<<
			" -keystore \""<<keystore<<"\""
			" -storepass "<<storepass<<
			" -dname \"cn=MoSync, ou=MoSync, o=MoSync, c=SE\"";
		sh(cmd.str().c_str());
	}

	// run android/tools-stripped.jar, for signing
	string signedApk = dstDir + "/" + string(s.name) + ".apk";
	cmd.str("");
	cmd <<"java -jar \""<<mosyncdir()<<"/bin/android/tools-stripped.jar\""
		" -keystore \""<<keystore<<"\" -storepass \""<<storepass<<"\""
		" -keypass \""<<keypass<<"\" "
		" -signedjar \""<<signedApk<<"\""
		" \""<<unsignedApk<<"\""
		" "<<alias;
	sh(cmd.str().c_str());

	// there, that should do it.
}

static string appNameToPackageName(const string& appName) {
	string pn = appName;
	for(size_t i=0; i<pn.size(); i++) {
		if(pn[i] == ' ')
			pn[i] = '_';
	}
	return pn;
}

static string packageNameToByteCodeName(const string& packageName) {
	string bcn = packageName;
	for(size_t i=0; i<bcn.size(); i++) {
		if(bcn[i] == '.')
			bcn[i] = '/';
	}
	return bcn;
}

static void writeManifest(const char* filename, const string& packageName,
	const SETTINGS& s, const RuntimeInfo& ri)
{
	ofstream file(filename, ios::binary);
	file <<"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
		<<"<manifest xmlns:android=\"http://schemas.android.com/apk/res/android\"\n"
		<<"\tpackage=\"" << packageName << "\" >\n"
		<<"\tandroid:versionCode=\"1\"\n"
		<<"\tandroid:versionName=\"1.0\">\n"
		<<"\t<application\n"
		<<"\t\tandroid:icon=\"@drawable/icon\"\n"
		<<"\t\tandroid:label=\"@string/app_name\">\n"
		<<"\t\t<activity android:name=\".MoSync\"\n"
		// Use portrait orientation as default. 
		<<"\t\t\tandroid:screenOrientation=\"portrait\"\n"
		<<"\t\t\tandroid:configChanges=\"keyboardHidden|orientation\"\n"
		<<"\t\t\tandroid:label=\"@string/app_name\">\n"
		<<"\t\t\t<intent-filter>\n"
		<<"\t\t\t\t<action android:name=\"android.intent.action.MAIN\" />\n"
		<<"\t\t\t\t<category android:name=\"android.intent.category.LAUNCHER\" />\n"
		<<"\t\t\t</intent-filter>\n"
		<<"\t\t</activity>\n"
		<<"\t\t<activity android:name=\".MoSyncPanicDialog\"\n"
		<<"\t\t\tandroid:label=\"@string/app_name\">\n"
		<<"\t\t</activity>\n"
		<<"\t\t<activity android:name=\".TextBox\"\n"
		<<"\t\t\tandroid:label=\"@string/app_name\">\n"
		<<"\t\t</activity>\n"
		<<"\t</application>\n"
		<<"\t<uses-sdk android:minSdkVersion=\""<<ri.androidVersion<<"\" />\n"
		;

	// Adding the support-screens for cupcake would lead to problems.
	if(ri.androidVersion >= 4) {
		file <<"\t<supports-screens\n"
			<<"\t\tandroid:largeScreens=\"true\"\n"
			<<"\t\tandroid:normalScreens=\"true\"\n"
			<<"\t\tandroid:smallScreens=\"true\"\n"
			<<"\t\tandroid:anyDensity=\"true\" />\n"
			;
	}

	//todo: variable permissions
	//writePermission(file, "VIBRATE");
	writePermission(file, "INTERNET");
	//writePermission(file, "ACCESS_COARSE_LOCATION");
	//writePermission(file, "ACCESS_FINE_LOCATION");
	//writePermission(file, "BATTERY_STATS");
	//writePermission(file, "READ_CALENDAR");
	//writePermission(file, "WRITE_CALENDAR");
	//writePermission(file, "READ_CONTACTS");
	//writePermission(file, "WRITE_CONTACTS");
	//writePermission(file, "READ_SMS");
	//writePermission(file, "SEND_SMS");
	//writePermission(file, "RECEIVE_SMS");
	//writePermission(file, "CAMERA");
	writePermission(file, "BLUETOOTH");
	writePermission(file, "BLUETOOTH_ADMIN");
	writePermission(file, "WRITE_EXTERNAL_STORAGE");
	writePermission(file, "READ_PHONE_STATE");

	file <<"</manifest>\n";
	if(!file.good()) {
		printf("Failed to write %s\n", filename);
		exit(1);
	}
}

static void writePermission(ostream& stream, const char* perm) {
	stream <<"\t<uses-permission android:name=\"android.permission."<<perm<<"\" />\n";
}

static void writeMain(const char* filename, const SETTINGS& s, const RuntimeInfo& ri) {
	ofstream file(filename, ios::binary);
	file <<"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
		<<"<LinearLayout xmlns:android=\"http://schemas.android.com/apk/res/android\"\n"
		<<"\tandroid:orientation=\"vertical\"\n"
		<<"\tandroid:layout_width=\"fill_parent\"\n"
		<<"\tandroid:layout_height=\"fill_parent\"\n"
		<<"/>\n";
	if(!file.good()) {
		printf("Failed to write %s\n", filename);
		exit(1);
	}
}

static void writeStrings(const char* filename, const SETTINGS& s, const RuntimeInfo& ri) {
	ofstream file(filename, ios::binary);
	file <<"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
		<<"<resources>\n"
		<<"\t<string name=\"app_name\">"<<s.name<<"</string>\n"
		<<"</resources>\n";
	if(!file.good()) {
		printf("Failed to write %s\n", filename);
		exit(1);
	}
}
