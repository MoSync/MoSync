//
//  MoSyncFonts.h
//  MoSync
//
//  Created by Mircea Vasiliniuc on 2/6/13.
//
//

#include <vector>
#include <helpers/cpp_defs.h>
#include "Syscall.h"

const int FONT_DEFAULT_AMOUNT = 15;

//Holds information that can be used by the various font systems to generate their own font objects.
struct FontInfo{
    CFStringRef name;       //Postscript name of the font
    CGFloat size;           //Size of the font
    UIFont *uiFontObject;   //Instantiated when first needed
    CGFontRef cgFontObject; //Instantiated when first needed
};

//Internal storage indexed by MAHandle-1
extern std::vector<FontInfo*> sFontList;

//The array holding the default system font names
//Three slots are unused due to the indexing system
extern CFStringRef gDefaultFontNames[FONT_DEFAULT_AMOUNT];

extern MAHandle gCurrentFontHandle; //Handle to the currently selected font


void initFonts();
MAHandle createFontInfo(CFStringRef name, GLfloat size, UIFont *uiFontObject, CGFontRef cgFontObject);
void initCGFont(FontInfo *fontInfo);

// Namespace used for compatibility with former code. Remove this when refactoring.
namespace Base
{
    //Used to initialize (if needed) and return a UIFont object from a Font handle, to be used by NativeUI
    UIFont* getUIFontObject(MAHandle fontHandle);
}

SYSCALL(MAHandle, maFontLoadDefault(int type, int style, int size));
SYSCALL(MAHandle, maFontLoadWithName(const char* name, int size));
SYSCALL(MAHandle, maFontDelete(MAHandle font));
SYSCALL(int, maFontGetCount());
SYSCALL(int, maFontGetName(int index, char* buffer, int bufferLength));
SYSCALL(MAHandle, maFontSetCurrent(MAHandle font));
