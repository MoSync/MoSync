#include <string>
#include <sstream>
#include "packagers.h"
#include "util.h"
#include "helpers/types.h"

using namespace std;

static void setupS60(const SETTINGS& s, string& dstPath, string& combName) {
	testDst(s);
	testProgram(s);
	testName(s);
	testVendor(s);
	testUid(s);

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

void packageS60v3(const SETTINGS& s, const std::string& runtimePath) {
	string dstPath, combName;
	setupS60(s, dstPath, combName);

	// set up resource templates
	string rscBaseName = (dstPath + s.uid);
	string rsgName = rscBaseName + ".rsg";

	TemplateMap tm;
	tm["app-name"] = s.name;
	tm["uid"] = s.uid;
	tm["resource-header"] = "#include \""+string(s.uid)+".rsg\"";

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
	sh(cmd.str().c_str());

	// use template reg.prs
	string templateRegName = runtimePath + "MoSync_reg_template.prs";
	string prsName = rscBaseName + "_reg.prs";
	applyTemplate(prsName.c_str(), templateRegName.c_str(), tm);

	// call cpp to process the RSG include.
	string genRegName = rscBaseName + "_reg.rss";
	cmd.str("");
	cmd << mosyncdir()<<"/bin/cpp \""<<prsName<<"\" \""<<genRegName<<"\"";
	sh(cmd.str().c_str());

	// call rcomp again
	cmd.str("");
	cmd << mosyncdir()<<"/bin/rcomp -u \"-o"<<regRscName<<
		"\" \"-s"<<genRegName<<"\"";
	sh(cmd.str().c_str());

	// call e32hack
	cmd.str("");
	cmd << mosyncdir()<<"/bin/e32hack \""<<runtimePath<<"MoSync"<<(s.debug ? "D" : "")<<".exe\" \""<<
		dstPath<<s.uid<<".exe\" "<<s.uid;
	sh(cmd.str().c_str());

	// use template .pkg
	string templatePkgName = runtimePath + "MoSync-template.pkg";
	string genPkgName = (dstPath + s.uid) + ".pkg";
	string unsignedSisName = (dstPath + s.uid) + ".sis";
	string signedSisName = (dstPath + s.name) + ".sisx";

	tm["project-name"] = s.name;
	tm["vendor-name"] = s.vendor;
	tm["pkg-runtime-dir"] = runtimePath;
	tm["pkg-package-output-dir"] = dstPath;
	tm["pkg-programcomb-output"] = combName;
	tm["has-icon"] = "; ";	//commented out. fixme.
	applyTemplate(genPkgName.c_str(), templatePkgName.c_str(), tm);

	// call makesis
	cmd.str("");
	cmd << mosyncdir()<<"/bin/makesis-4 \""<<genPkgName<<"\"";
	sh(cmd.str().c_str());

	// call signsis
	cmd.str("");
	cmd << mosyncdir()<<"/bin/signsis-4 -s \""<<unsignedSisName<<"\" \""<<signedSisName<<"\" \""<<
		mosyncdir()<<"/etc/default.cert\" \""<<mosyncdir()<<"/etc/default.key\" default";
	sh(cmd.str().c_str());

	// and we're done!
}


void packageS60v2(const SETTINGS& s, const std::string& runtimePath) {
	string dstPath, combName;
	setupS60(s, dstPath, combName);

	// set up resource templates
	string rscBaseName = (dstPath + s.uid);

	TemplateMap tm;
	tm["app-name"] = s.name;
	tm["uid"] = s.uid;

	// use template .prs
	string templateRssName = runtimePath + "MoSync_caption_template.prs";
	string genRssName = rscBaseName + "_caption.rss";
	applyTemplate(genRssName.c_str(), templateRssName.c_str(), tm);

	// call rcomp
	string captionRscName = rscBaseName + "_caption.rsc";
	std::ostringstream cmd;
	cmd << mosyncdir()<<"/bin/rcomp -u \"-o"<<captionRscName<<"\" \"-s"<<genRssName<<"\"";
	sh(cmd.str().c_str());

	// copy main rsc file
	string mainRscName = rscBaseName + ".rsc";
	copyFile(mainRscName.c_str(), (runtimePath + "MoSync.rsc").c_str());

	// call e32hack
	cmd.str("");
	cmd << mosyncdir()<<"/bin/e32hack \""<<runtimePath<<"MoSync"<<(s.debug ? "D" : "")<<".app\" \""<<
		dstPath<<s.uid<<".app\" "<<s.uid<<" -v2";
	sh(cmd.str().c_str());

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
	sh(cmd.str().c_str());

	// and we're done!
}
