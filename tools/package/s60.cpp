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

#include <string>
#include <sstream>
#include "packagers.h"
#include "util.h"
#include "tools.h"
#include "helpers/types.h"

using namespace std;

static void setupS60(const SETTINGS& s, string& dstPath, string& combName) {
	testDst(s);
	testProgram(s);
	testName(s);
	testVendor(s);

	dstPath = s.dst;
	toSlashes(dstPath);
	toDir(dstPath);

	// if we have resources, combine them with the program file.
	if(s.resource) {
		combName = dstPath + "program.comb";
		copyFile(combName.c_str(), s.program);
		appendFile(combName.c_str(), s.resource);
	} else {
		combName = s.program;
	}
}

void packageS60v3(const SETTINGS& s, const RuntimeInfo& ri) {
	string dstPath, combName;
	setupS60(s, dstPath, combName);
	testS60v3Uid(s);
	const char* uid = s.s60v3uid;

	// set up resource templates
	string rscBaseName = (dstPath + uid);
	string rsgName = rscBaseName + ".rsg";
	const string& runtimePath = ri.path;

	TemplateMap tm;
	tm["app-name"] = s.name;
	tm["uid"] = uid;
	tm["resource-header"] = "#include \""+string(uid)+".rsg\"";

	// use template .prs
	string templateRssName = runtimePath + "MoSync_3rd_template.prs";
	string genRssName = rscBaseName + ".rss";
	applyTemplate(genRssName.c_str(), templateRssName.c_str(), tm);

	// call rcomp
	string mainRscName = rscBaseName + ".rsc";
	string regRscName = rscBaseName + "_reg.rsc";

	std::ostringstream cmd;
	cmd << mosyncdir()<<"/bin/rcomp -u \"-o"<<mainRscName<<"\" \"-h"<<rsgName<<
		"\" \"-s"<<genRssName<<"\"";
	sh(cmd.str().c_str(), s.silent);

	// use template reg.prs
	string templateRegName = runtimePath + "MoSync_reg_template.prs";
	string prsName = rscBaseName + "_reg.prs";
	applyTemplate(prsName.c_str(), templateRegName.c_str(), tm);

	// call cpp to process the RSG include.
	string genRegName = rscBaseName + "_reg.rss";
	cmd.str("");
	cmd << mosyncdir()<<"/bin/cpp \""<<prsName<<"\" \""<<genRegName<<"\"";
	sh(cmd.str().c_str(), s.silent);

	// call rcomp again
	cmd.str("");
	cmd << mosyncdir()<<"/bin/rcomp -u \"-o"<<regRscName<<
		"\" \"-s"<<genRegName<<"\"";
	sh(cmd.str().c_str(), s.silent);

	// call e32hack
	cmd.str("");
	cmd << mosyncdir()<<"/bin/e32hack \""<<runtimePath<<"MoSync"<<(s.debug ? "D" : "")<<".exe\" \""<<
		dstPath<<uid<<".exe\" "<<uid;
	sh(cmd.str().c_str(), s.silent);

	// call icon-injector
	if(s.icon && &ri.iconSize != 0) {
		cmd.str("");
		injectIcon("symbian9", ri.iconSize.c_str(), s.icon, (dstPath + "/" + uid + "_icon.mif").c_str(), s.silent);
	}

	// use template .pkg
	string templatePkgName = runtimePath + "MoSync-template.pkg";
	string genPkgName = (dstPath + uid) + ".pkg";
	string unsignedSisName = (dstPath + uid) + ".sis";
	string signedSisName = (dstPath + s.name) + ".sisx";

	tm["project-name"] = s.name;
	tm["vendor-name"] = s.vendor;
	tm["pkg-runtime-dir"] = runtimePath;
	tm["pkg-package-output-dir"] = dstPath;
	tm["pkg-programcomb-output"] = combName;
	tm["has-icon"] = s.icon ? "" : "; ";
	applyTemplate(genPkgName.c_str(), templatePkgName.c_str(), tm);

	// call makesis
	cmd.str("");
	cmd << mosyncdir()<<"/bin/makesis-4 \""<<genPkgName<<"\"";
	sh(cmd.str().c_str(), s.silent);

	// call signsis
	// todo: support non-default keys
	cmd.str("");
	cmd << mosyncdir()<<"/bin/signsis-4 -s \""<<unsignedSisName<<"\" \""<<signedSisName<<"\" \""<<
		mosyncdir()<<"/etc/default.cert\" \""<<mosyncdir()<<"/etc/default.key\" default";
	sh(cmd.str().c_str(), s.silent);

	// and we're done!
}


void packageS60v2(const SETTINGS& s, const RuntimeInfo& ri) {
	string dstPath, combName;
	setupS60(s, dstPath, combName);
	testS60v2Uid(s);
	const char* uid = s.s60v2uid;

	// set up resource templates
	string rscBaseName = (dstPath + uid);
	const string& runtimePath = ri.path;

	TemplateMap tm;
	tm["app-name"] = s.name;
	tm["uid"] = uid;

	// use template .prs
	string templateRssName = runtimePath + "MoSync_caption_template.prs";
	string genRssName = rscBaseName + "_caption.rss";
	applyTemplate(genRssName.c_str(), templateRssName.c_str(), tm);

	// call rcomp
	string captionRscName = rscBaseName + "_caption.rsc";
	std::ostringstream cmd;
	cmd << mosyncdir()<<"/bin/rcomp -u \"-o"<<captionRscName<<"\" \"-s"<<genRssName<<"\"";
	sh(cmd.str().c_str(), s.silent);

	// copy main rsc file
	string mainRscName = rscBaseName + ".rsc";
	copyFile(mainRscName.c_str(), (runtimePath + "MoSync.rsc").c_str());

	// call e32hack
	cmd.str("");
	cmd << mosyncdir()<<"/bin/e32hack \""<<runtimePath<<"MoSync"<<(s.debug ? "D" : "")<<".app\" \""<<
		dstPath<<uid<<".app\" "<<uid<<" -v2";
	sh(cmd.str().c_str(), s.silent);

	// use template .pkg
	string templatePkgName = runtimePath + "MoSync-template.pkg";
	string genPkgName = (dstPath + s.name) + ".pkg";

	tm["project-name"] = s.name;
	tm["vendor-name"] = s.vendor;
	tm["pkg-runtime-dir"] = runtimePath;
	tm["pkg-package-output-dir"] = dstPath;
	tm["pkg-programcomb-output"] = combName;
	applyTemplate(genPkgName.c_str(), templatePkgName.c_str(), tm);

	// call makesis
	cmd.str("");
	cmd << mosyncdir()<<"/bin/makesis-200 \""<<genPkgName<<"\"";
	sh(cmd.str().c_str(), s.silent);

	// and we're done!
}
