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
#include "tools.h"
#include "permissions.h"
#include "nfc.h"
#include "mustache.h"
#include "helpers/mkdir.h"
#include "filelist/filelist.h"
#include "profiledb/profiledb.h"
#include <fstream>
#include <sstream>
#include <errno.h>
#include <vector>
#include <stdlib.h>

using namespace std;

static string writeManifest(const SETTINGS& s, const RuntimeInfo& ri);
static void fromTemplate(const char* templateFile, const char* outputFile, const SETTINGS& s, const RuntimeInfo& ri);
static void writeMain(const char* filename, const SETTINGS& s, const RuntimeInfo& ri);
static void writeStrings(const char* filename, const SETTINGS& s, const RuntimeInfo& ri);
static void injectIcons(const SETTINGS& s, const RuntimeInfo& ri);
static void sign(const SETTINGS& s, const RuntimeInfo& ri, string& unsignedApk, string& signedApk);
static void createSignCmd(ostringstream& cmd, string& keystore, string& alias, string& storepass, string& keypass, string& signedApk, string& unsignedApk, bool hidden);
static void writeNFCResource(const SETTINGS& s, const RuntimeInfo& ri);
static string packageNameToByteCodeName(const string& packageName);

class AndroidContext : public DefaultContext {
private:
	unsigned int fAndroidVersion;

public:
	AndroidContext(MustacheContext* parent, unsigned int androidVersion) : DefaultContext(parent) {
		fAndroidVersion = androidVersion;
	}

	string getParameter(string key) {
		// Special variable.
		unsigned int min;
		unsigned int max;
		int matches = sscanf(key.c_str(), "sdk-version-%u-%u", &min, &max);
		if (matches && matches != EOF) {
			bool matchesMin = fAndroidVersion >= min;
			bool matchesMax = matches < 2 || fAndroidVersion <= max;
			return matchesMin && matchesMax ? key : string();
		} else {
			return DefaultContext::getParameter(key);
		}
	}
};

void packageAndroid(const SETTINGS& s, const RuntimeInfo& ri) {
	testDst(s);
	testProgram(s);
	testName(s);
	testVendor(s);
	testAndroidPackage(s);
	testAndroidVersionCode(s);
	testAndroidInstallLocation(s);

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

	string manifestXml = writeManifest(s, ri);

	if (s.nfc) {
		writeNFCResource(s, ri);
	}

	string mainXml = layout + "/main.xml";
	writeMain(mainXml.c_str(), s, ri);
	string stringsXml = values + "/strings.xml";
	writeStrings(stringsXml.c_str(), s, ri);

	// run mosync/bin/android/aapt
	std::ostringstream cmd;
	string resourcesAp_ = dstDir + "/resources.ap_";

	cmd.str("");
	cmd <<getBinary("android/aapt")<<" package -f -M "<<file(manifestXml)<<
		" -F "<<file(resourcesAp_)<<
		" -I \""<<mosyncdir()<<"/bin/android/android-"<<ri.androidVersion<<".jar\""<<
		" -S "<<file(res)<<
		" -0 -A "<<file(add);
	sh(cmd.str().c_str());

	// unzip Runtime class files
	string classes = dstDir + "/classes";
	_mkdir(classes.c_str());

	cmd.str("");
	cmd <<getBinary("unzip")<<
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
	cmd <<"java -jar "<<getBinary("android/dx.jar")<<
		" --dex --patch-string com/mosync/java/android"
		" "<<packageNameToByteCodeName(s.androidPackage)<<""
		" \"--output="<<classesDex<<"\" \""<<classes<<"\"";
	sh(cmd.str().c_str());

	// run android/apkbuilder.jar
	string unsignedApk = dstDir + "/" + string(s.name) + "_unsigned.apk";
	cmd.str("");
	cmd <<"java -jar "<<getBinary("android/apkbuilder.jar")<<
		" "<<file(unsignedApk)<<
		" -u -z "<<file(resourcesAp_)<<
		" -f "<<file(classesDex)<<
		" -nf "<<file(addlib);
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
			printf("Android: No certificate provided, falling back to default signing options");
		}
		if(isDirectory(keystore.c_str()) < 0) {	// does not exist
			// create default keystore
			ostringstream createKeystoreCmd;
			createKeystoreCmd.str("");
			createKeystoreCmd << "keytool -genkeypair -v -alias "<<alias<<
				" -validity 3650 -keypass "<<keypass<<
				" -keystore "<< file(keystore)<<
				" -storepass "<< arg(storepass)<<
				" -dname \"cn=MoSync, ou=MoSync, o=MoSync, c=SE\"";
			sh(createKeystoreCmd.str().c_str());
		}
	}

	// run android/tools-stripped.jar, for signing
	createSignCmd(cmd, keystore, alias, storepass, keypass, signedApk, unsignedApk, false);
	createSignCmd(hiddenCmd, keystore, alias, storepass, keypass, signedApk, unsignedApk, true);

	string cmdStr = cmd.str();
	sh(cmdStr.c_str(), false, s.showPasswords ? cmdStr.c_str() : hiddenCmd.str().c_str());
}

static void createSignCmd(ostringstream& cmd, string& keystore, string& alias, string& storepass, string& keypass, string& signedApk, string& unsignedApk, bool hidden) {
	storepass = hidden ? "*** HIDDEN ***" : storepass;
	keypass = hidden ? "*** HIDDEN ***" : keypass;

	cmd <<"java -jar "<<getBinary("android/tools-stripped.jar")<<
		" -keystore "<<arg(keystore)<<" -storepass "<<arg(storepass)<<
		" -keypass "<<arg(keypass)<<
		" -signedjar "<<file(signedApk)<<
		" "<<file(unsignedApk)<<
		" "<<arg(alias);
}

static void injectIcons(const SETTINGS& s, const RuntimeInfo& ri) {
	if (s.icon) {
		vector<string> sizes = vector<string>();
		vector<string> directories = vector<string>();

		sizes.push_back("36x36");
		directories.push_back("/res/drawable");

		if (ri.androidVersion >= 4) {
			// For Android >= 1.6
			//36x36 for low-density (ldpi)
			//48x48 for medium-density (mdpi)
			//72x72 for high-density (hdpi)
			sizes.push_back("36x36");
			sizes.push_back("48x48");
			sizes.push_back("72x72");
			directories.push_back("/res/drawable-ldpi");
			directories.push_back("/res/drawable-mdpi");
			directories.push_back("/res/drawable-hdpi");
		}

		if (ri.androidVersion >= 8) {
			//96x96 for extra high-density (xhdpi)
			sizes.push_back("96x96");
			directories.push_back("/res/drawable-xhdpi");
		}

		for(size_t i = 0; i < sizes.size(); i++) {
			std::ostringstream iconInjectCmd;
			string size = sizes.at(i);
			string outputDir = file(string(s.dst) + directories.at(i));
			_mkdir(outputDir.c_str());
			string outputIcon = outputDir + "/icon.png";
			injectIcon("Android", size.c_str(), s.icon, file(outputIcon).c_str(), s.silent);
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

static string writeManifest(const SETTINGS& s, const RuntimeInfo& ri) {
	string dstDir = string(s.dst);
	string manifestXml = dstDir + "/AndroidManifest.xml";
	ProfileDB db;
	string androidProfilesDir = db.profilesdir("Android");
	string manifestTemplate = file(s.androidManifestTemplate ?
			string(s.androidManifestTemplate) :
			androidProfilesDir + "/AndroidManifest.template");
	fromTemplate(manifestTemplate.c_str(), manifestXml.c_str(), s, ri);

	return manifestXml;
}

static void writeNFCResource(const SETTINGS& s, const RuntimeInfo& ri) {
	if (!s.nfc || !existsFile(s.nfc)) {
		printf("If NFC permissions are set, then\n"
				"1.the --nfc parameter must be set, and\n"
				"2. that file must exist.");
		exit(1);
	}

	string dstDir = string(s.dst);
	ProfileDB db;
	string androidProfilesDir = db.profilesdir("Android");

	// To customize the NFC resources should be a *very* rare event, but we allow it
	// by exposing it as a file.
	string nfcTemplate = androidProfilesDir + "/nfc.template";
	string nfcResourceDir = dstDir + "/res/xml/";
	_mkdir(nfcResourceDir.c_str());
	string nfcResource = string(nfcResourceDir) + "nfc.xml";
	fromTemplate(nfcTemplate.c_str(), nfcResource.c_str(), s, ri);
}

static void fromTemplate(const char* templateFile, const char* filename, const SETTINGS& s, const RuntimeInfo& ri) {
	string packageName = string(s.androidPackage);
	string versionCode = string(s.androidVersionCode);
	string version = string(s.version);
	string installLocation = string(s.androidInstallLocation ? s.androidInstallLocation : "internalOnly");

	MustacheParser parser(true);
	AndroidContext root(NULL, ri.androidVersion);
	root.setParameter("debug", s.debug ? "debug" : "");
	root.setParameter("package-name", packageName);
	root.setParameter("version-code", versionCode);
	root.setParameter("version", version);
	root.setParameter("install-location", installLocation);
	char androidVersionStr[8];
	sprintf(androidVersionStr, "%d", ri.androidVersion);
	root.setParameter("sdk-version", androidVersionStr);

	PermissionContext permissions(&root, s.permissions);
	root.addChild("permissions", &permissions);

	vector<AndroidContext*> deleteUs;

	if (s.nfc) {
		NfcInfo* nfcInfo = NfcInfo::parse(string(s.nfc));
		vector<TechList*> techLists = nfcInfo->getTechLists();
		for (size_t i = 0; i < techLists.size(); i++) {
			TechList* techList = techLists.at(i);
			AndroidContext* techListContext = new AndroidContext(&root, ri.androidVersion);
			deleteUs.push_back(techListContext);
			root.addChild("tech-list", techListContext);
			vector<string> technologies = techList->technologies;
			for (size_t j = 0; j < technologies.size(); j++) {
				AndroidContext* techContext = new AndroidContext(techListContext, ri.androidVersion);
				deleteUs.push_back(techContext);
				techListContext->addChild("tech", techContext);
				string techName = technologies.at(j);
				techContext->setParameter("name", techName);
			}
		}
	}

	ofstream output(filename);
	if (output.good()) {
		DefaultParserCallback cb(&root, output);
		string templateFileStr = string(templateFile);
		string result = parser.parseFile(templateFileStr, &cb);
		output.flush();
		output.close();
		if (!result.empty()) {
			printf("Error creating android manifest: %s\n", result.c_str());
			exit(1);
		}
	} else {
		printf("Could not write android manifest!\n");
		exit(1);
	}

	for (size_t i = 0; i < deleteUs.size(); i++) {
		delete deleteUs[(int) i];
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
