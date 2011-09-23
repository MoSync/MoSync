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
#include "permissions.h"
#include "helpers/mkdir.h"
#include "filelist/filelist.h"
#include <fstream>
#include <sstream>
#include <errno.h>
#include <vector>
#include <stdlib.h>

using namespace std;

static void writeManifest(const char* filename, const SETTINGS& s, const RuntimeInfo& ri);
static void writeMain(const char* filename, const SETTINGS& s, const RuntimeInfo& ri);
static void writeStrings(const char* filename, const SETTINGS& s, const RuntimeInfo& ri);
static void injectIcons(const SETTINGS& s, const RuntimeInfo& ri);
static void sign(const SETTINGS& s, const RuntimeInfo& ri, string& unsignedApk, string& signedApk);
static void createSignCmd(ostringstream& cmd, string& keystore, string& alias, string& storepass, string& keypass, string& signedApk, string& unsignedApk, bool hidden);
static void writePermissions(ostream& stream, const SETTINGS& s, const RuntimeInfo& ri);
static void writePermission(ostream& stream, bool flag, const char* nativePerm);
static string packageNameToByteCodeName(const string& packageName);

void packageAndroid(const SETTINGS& s, const RuntimeInfo& ri) {
	testDst(s);
	testProgram(s);
	testName(s);
	testVendor(s);
	testAndroidPackage(s);
	testAndroidVersionCode(s);

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

	// Inject icon
	injectIcons(s, ri);

	string manifestXml = dstDir + "/AndroidManifest.xml";
	writeManifest(manifestXml.c_str(), s, ri);
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
		" "<<packageNameToByteCodeName(s.androidPackage)<<""
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


	string signedApk = dstDir + "/" + string(s.name) + ".apk";
	sign(s, ri, unsignedApk, signedApk);

	// there, that should do it.
}

static void sign(const SETTINGS& s, const RuntimeInfo& ri, string& unsignedApk, string& signedApk) {
	string keystore = mosyncdir() + string("/etc/mosync.keystore");
	string alias = "mosync.keystore";
	string storepass = "default";
	string keypass = "default";
	ostringstream cmd;
	ostringstream hiddenCmd;

	if (s.androidKeystore) {
		keystore = s.androidKeystore;
		alias = s.androidAlias;
		storepass = s.androidStorePass;
		keypass = s.androidKeyPass;
	} else {
		if (!s.silent) {
			printf("Android: No certficate provided, falling back to default signing options");
		}
		if(isDirectory(keystore.c_str()) < 0) {	// does not exist
			// create default keystore
			ostringstream createKeystoreCmd;
			createKeystoreCmd.str("");
			createKeystoreCmd << "keytool -genkeypair -v -alias "<<alias<<
				" -validity 3650 -keypass "<<keypass<<
				" -keystore \""<<keystore<<"\""
				" -storepass "<<storepass<<
				" -dname \"cn=MoSync, ou=MoSync, o=MoSync, c=SE\"";
			sh(createKeystoreCmd.str().c_str());
		}
	}

	// run android/tools-stripped.jar, for signing
	createSignCmd(cmd, keystore, alias, storepass, keypass, signedApk, unsignedApk, false);
	createSignCmd(hiddenCmd, keystore, alias, storepass, keypass, signedApk, unsignedApk, true);

	string cmdStr = cmd.str();
	sh(cmdStr.c_str(), true, s.showPasswords ? cmdStr.c_str() : hiddenCmd.str().c_str());
}

static void createSignCmd(ostringstream& cmd, string& keystore, string& alias, string& storepass, string& keypass, string& signedApk, string& unsignedApk, bool hidden) {
	storepass = hidden ? "*** HIDDEN ***" : storepass;
	keypass = hidden ? "*** HIDDEN ***" : keypass;

	cmd <<"java -jar \""<<mosyncdir()<<"/bin/android/tools-stripped.jar\""
		" -keystore \""<<keystore<<"\" -storepass \""<<storepass<<"\""
		" -keypass \""<<keypass<<"\" "
		" -signedjar \""<<signedApk<<"\""
		" \""<<unsignedApk<<"\""
		" "<<alias;
}

static void injectIcons(const SETTINGS& s, const RuntimeInfo& ri) {
	if (s.icon) {
		vector<string> sizes = vector<string>();
		vector<string> directories = vector<string>();

		sizes.push_back("36x36");
		directories.push_back("/res/drawable");

		if (ri.androidVersion >= 4) {
			printf("Multiple screen support not yet impl!");
			// For Android >= 1.6
			//36x36 for low-density (ldpi)
			//48x48 for medium-density (mdpi)
			//72x72 for high-density (hdpi)
			/*sizes.push_back("36x36");
			sizes.push_back("48x48");
			sizes.push_back("72x72");
			directories.push_back("/res/drawable-ldpi");
			directories.push_back("/res/drawable-mdpi");
			directories.push_back("/res/drawable-hdpi");*/
		}

		if (ri.androidVersion >= 8) {
			//96x96 for extra high-density (xhdpi)
			sizes.push_back("96x96");
			directories.push_back("/res/drawable-xhdpi");
		}

		for(size_t i = 0; i < sizes.size(); i++) {
			std::ostringstream iconInjectCmd;
			string size = sizes.at(i);
			string outputDir = string(s.dst) + directories.at(i);
			_mkdir(outputDir.c_str());
			string outputIcon = outputDir + "/icon.png";
			iconInjectCmd << "\"" << mosyncdir() << "/bin/icon-injector\" -platform android -src \"" <<
				s.icon << "\" -size " << size.c_str() << " -dst \"" << outputIcon.c_str() << "\"";
			sh(iconInjectCmd.str().c_str(), s.silent);
		}
	}
}

static string packageNameToByteCodeName(const string& packageName) {
	string bcn = packageName;
	for(size_t i=0; i<bcn.size(); i++) {
		if(bcn[i] == '.')
			bcn[i] = '/';
	}
	return bcn;
}

static void writeManifest(const char* filename, const SETTINGS& s, const RuntimeInfo& ri)
{
	string packageName = string(s.androidPackage);
	string versionCode = string(s.androidVersionCode);
	string version = string(s.version);

	ofstream file(filename, ios::binary);
	file <<"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
		<<"<manifest xmlns:android=\"http://schemas.android.com/apk/res/android\"\n"
		<<"\tpackage=\"" << packageName << "\" >\n"
		<<"\tandroid:versionCode=\"" << versionCode << "\"\n"
		<<"\tandroid:versionName=\"" << version << "\">\n"
		<<"\t<application\n";
	if (s.icon) {
		file <<"\t\tandroid:icon=\"@drawable/icon\"\n";
	}
	file <<"\t\tandroid:label=\"@string/app_name\">\n"
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

	writePermissions(file, s, ri);

	file <<"</manifest>\n";
	if(!file.good()) {
		printf("Failed to write %s\n", filename);
		exit(1);
	}
}

static void writePermissions(ostream& stream, const SETTINGS& s, const RuntimeInfo& ri) {
	if (!s.permissions) {
		return;
	}

	set<string> permissionSet = set<string>();
	parsePermissions(permissionSet, s.permissions);

	writePermission(stream, isPermissionSet(permissionSet, VIBRATE), "android.permission.VIBRATE");
	writePermission(stream, isPermissionSet(permissionSet, INTERNET), "android.permission.INTERNET");
	writePermission(stream, isPermissionSet(permissionSet, INTERNET), "android.permission.ACCESS_NETWORK_STATE");
	writePermission(stream, isPermissionSet(permissionSet, LOCATION_COARSE), "android.permission.ACCESS_COARSE_LOCATION");
	writePermission(stream, isPermissionSet(permissionSet, LOCATION_FINE), "android.permission.ACCESS_FINE_LOCATION");
	writePermission(stream, isPermissionSet(permissionSet, POWER_MANAGEMENT), "android.permission.BATTERY_STATS");
	writePermission(stream, isPermissionSet(permissionSet, CALENDAR_READ), "android.permission.READ_CALENDAR");
	writePermission(stream, isPermissionSet(permissionSet, CALENDAR_WRITE), "android.permission.WRITE_CALENDAR");
	writePermission(stream, isPermissionSet(permissionSet, CONTACTS_READ), "android.permission.READ_CONTACTS");
	writePermission(stream, isPermissionSet(permissionSet, CONTACTS_WRITE), "android.permission.WRITE_CONTACTS");
	writePermission(stream, isPermissionSet(permissionSet, SMS_READ), "android.permission.READ_SMS");
	writePermission(stream, isPermissionSet(permissionSet, SMS_SEND), "android.permission.SEND_SMS");
	writePermission(stream, isPermissionSet(permissionSet, SMS_RECEIVE), "android.permission.RECEIVE_SMS");
	writePermission(stream, isPermissionSet(permissionSet, CAMERA), "android.permission.CAMERA");
	writePermission(stream, isPermissionSet(permissionSet, HOMESCREEN), "android.permission.GET_TASKS");
	writePermission(stream, isPermissionSet(permissionSet, HOMESCREEN), "android.permission.SET_WALLPAPER");
	writePermission(stream, isPermissionSet(permissionSet, HOMESCREEN), "android.permission.SET_WALLPAPER_HINTS");
	writePermission(stream, isPermissionSet(permissionSet, HOMESCREEN), "com.android.launcher.permission.INSTALL_SHORTCUT");
	writePermission(stream, isPermissionSet(permissionSet, HOMESCREEN), "com.android.launcher.permission.UNINSTALL_SHORTCUT");
	writePermission(stream, isPermissionSet(permissionSet, AUTOSTART), "android.permission.RECEIVE_BOOT_COMPLETED");

	// Only add this for android 1.6 and higher.
	if (ri.androidVersion >= 4)
	{
		// And always enable it to be able to log in debug runtime
		writePermission(stream, s.debug || isPermissionSet(permissionSet, FILE_STORAGE_WRITE), "android.permission.WRITE_EXTERNAL_STORAGE");
	}

	// Only add this for android 2.0 and higher.
	if (ri.androidVersion >= 7)
	{
		writePermission(stream, isPermissionSet(permissionSet, BLUETOOTH), "android.permission.BLUETOOTH");
		writePermission(stream, isPermissionSet(permissionSet, BLUETOOTH), "android.permission.BLUETOOTH_ADMIN");
	}

	//if (ri.androidVersion >= 10) {
		writePermission(stream, isPermissionSet(permissionSet, NFC), "android.permission.NFC");
	//}

	// Always add this.
	writePermission(stream, true, "android.permission.READ_PHONE_STATE");
}
static void writePermission(ostream& stream, bool flag, const char* nativePerm) {
	if (flag) {
		stream <<"\t<uses-permission android:name=\""<<nativePerm<<"\" />\n";
	}
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
