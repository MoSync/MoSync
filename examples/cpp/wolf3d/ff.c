#include "ff.h"

int findfirst(const char *pathname, struct ffblk *ffblk, int attrib) {

/*
  TBuf8<256> path8;
  TBuf<256> path;
  TBuf8<256> file8;
  TBuf<256> file;
  char cwd[256];

  getcwd(cwd,256);
  path8.Format(_L8("%s%s"),cwd,pathname);
  path.Copy(path8);
  CEikonEnv::Static()->FsSession().GetDir(path,KEntryAttMaskSupported,ESortByName,dirlist);
  if (dirlist->Count()>0) {
    lpos=0;
    file=(*dirlist)[lpos].iName;
    file8.Copy(file);

    strcpy(ffblk->ff_name,(char *)file8.PtrZ());
    return 0;
  }

  return 1;
  */
	return 0;

}

int findnext(struct ffblk *ffblk) {
/*
  TBuf8<256> TFile8;
  TBuf<256> TFile;

  ++lpos;
  if (lpos<dirlist->Count()) {
    TFile=(*dirlist)[lpos].iName;
    TFile8.Copy(TFile);
    strncpy(ffblk->ff_name,(char *)TFile8.Ptr(),TFile8.Length());
    return 0;
  }
  return 1;
*/
	return 0;
}
