//
//  MoSyncFonts.mm
//  MoSync
//
//  Created by Mircea Vasiliniuc on 2/6/13.
//
//

#import "MoSyncFonts.h"
#import "ScreenOrientation.h"
#include "MoSyncGraphics.h"

std::vector<FontInfo*> sFontList;

CFStringRef gDefaultFontNames[FONT_DEFAULT_AMOUNT];

MAHandle gCurrentFontHandle; //Handle to the currently selected font

/* The following consts are used for the indexing system.
 * By using bitwise OR, you can get the index for that type and style combination
 * in gDefaultFontNames.
 * It's better than using the constants in maapi.h, because those can change.
 */
const short FONT_SANS_SERIF_INDEX   = 0;
const short FONT_SERIF_INDEX        = 1;
const short FONT_MONOSPACE_INDEX    = 2;
const short FONT_NORMAL_INDEX       = 0;
const short FONT_BOLD_INDEX         = 4;
const short FONT_ITALIC_INDEX       = 8;

//The style and type for the initally selected font
const short INITIAL_FONT_STYLE =    FONT_STYLE_NORMAL;
const short INITIAL_FONT_TYPE =     FONT_TYPE_SANS_SERIF;

//This will store all the font names once maFontGetCount is call for the first time
//Should I store them, or querry the iOS classes each time? (they are not straightforward)
//My calculation is that without the objective C overhead for the NSString classes,
//it will take up a bit over 1KB of memory.
NSMutableArray *gDeviceFontNames;

void initFonts()
{
    //Construction of the default font names array
    CFStringEncoding enc=kCFStringEncodingMacRoman;

    gDefaultFontNames[FONT_SERIF_INDEX|FONT_NORMAL_INDEX]=
    CFStringCreateWithCString(NULL, "TimesNewRomanPSMT",enc);
    gDefaultFontNames[FONT_SERIF_INDEX|FONT_BOLD_INDEX]=
    CFStringCreateWithCString(NULL, "TimesNewRomanPS-BoldMT",enc);
    gDefaultFontNames[FONT_SERIF_INDEX|FONT_ITALIC_INDEX]=
    CFStringCreateWithCString(NULL, "TimesNewRomanPS-ItalicMT",enc);
    gDefaultFontNames[FONT_SERIF_INDEX|FONT_BOLD_INDEX|FONT_ITALIC_INDEX]=
    CFStringCreateWithCString(NULL, "TimesNewRomanPS-BoldItalicMT",enc);
    gDefaultFontNames[FONT_SANS_SERIF_INDEX|FONT_NORMAL_INDEX]=
    CFStringCreateWithCString(NULL, "Helvetica",enc);
    gDefaultFontNames[FONT_SANS_SERIF_INDEX|FONT_BOLD_INDEX]=
    CFStringCreateWithCString(NULL, "Helvetica-Bold",enc);
    gDefaultFontNames[FONT_SANS_SERIF_INDEX|FONT_ITALIC_INDEX]=
    CFStringCreateWithCString(NULL, "Helvetica-Oblique",enc);
    gDefaultFontNames[FONT_SANS_SERIF_INDEX|FONT_BOLD_INDEX|FONT_ITALIC_INDEX]=
    CFStringCreateWithCString(NULL, "Helvetica-BoldOblique",enc);
    gDefaultFontNames[FONT_MONOSPACE_INDEX|FONT_NORMAL_INDEX]=
    CFStringCreateWithCString(NULL, "Courier",enc);
    gDefaultFontNames[FONT_MONOSPACE_INDEX|FONT_BOLD_INDEX]=
    CFStringCreateWithCString(NULL, "Courier-Bold",enc);
    gDefaultFontNames[FONT_MONOSPACE_INDEX|FONT_ITALIC_INDEX]=
    CFStringCreateWithCString(NULL, "Courier-Oblique",enc);
    gDefaultFontNames[FONT_MONOSPACE_INDEX|FONT_BOLD_INDEX|FONT_ITALIC_INDEX]=
    CFStringCreateWithCString(NULL, "Courier-BoldOblique",enc);

    // TODO Think about another initial value of the font that is not related to screen orientation.
    //Setting the initially selected font. "gHeight/40" was used originally, is kept for backwards compatibility
    MAHandle initFontHandle=maFontLoadDefault(INITIAL_FONT_TYPE,INITIAL_FONT_STYLE,
                                              [ScreenOrientation getInstance].screenSize.height/40);
    maFontSetCurrent(initFontHandle);
}

// This is a helper function that creates and stores a FontInfo with the provided arguments
MAHandle createFontInfo(CFStringRef name, GLfloat size, UIFont *uiFontObject, CGFontRef cgFontObject)
{
    MAHandle handle;
    FontInfo *newFontInfo=new FontInfo;
    newFontInfo->name=name;
    newFontInfo->size=size;
    newFontInfo->uiFontObject=uiFontObject; //Can be NULL
    newFontInfo->cgFontObject=cgFontObject; //Can be NULL

    int i;
    for(i=0; i<sFontList.size() ; i++)
    {
        if(!sFontList[i])break; //Searching for first NULL cell in the vector
    }
    if(i<sFontList.size())
    {
        sFontList[i]=newFontInfo; //Inserting at the first empty position in the vector
    }
    else
    {
        sFontList.push_back(newFontInfo); //Inserting at the end of the vector
    }

    handle=i+1; //Handles start from 1

    return handle;
}

//Used to instantiate the CGFont object only when needed
void initCGFont(FontInfo *fontInfo)
{
    if(!fontInfo->cgFontObject)
    {
        fontInfo->cgFontObject =CGFontCreateWithFontName(fontInfo->name);
    }
}

namespace Base
{
    //Used to initialize (if needed) and return a UIFont object from a Font handle, to be used by NativeUI
    UIFont* getUIFontObject(MAHandle fontHandle)
    {
        if(fontHandle<1||fontHandle>sFontList.size()||!sFontList[fontHandle-1])
        {
            printf("wrong MAHandle");
            return NULL;
        }

        FontInfo *selectedFont=sFontList[fontHandle-1];
        if (selectedFont->uiFontObject==NULL)
        {
            selectedFont->uiFontObject=
            [[UIFont fontWithName:(NSString *) selectedFont->name size:selectedFont->size] retain];
        }
        return selectedFont->uiFontObject;
    }
}

SYSCALL(MAHandle, maFontLoadDefault(int type, int style, int size))
{
    int defaultFontIndex=0; //Index to gDefaultFontNames
    if(size<=0)
    {
        return RES_FONT_INVALID_SIZE;
    }
    switch (type) {
        case FONT_TYPE_SERIF:
            defaultFontIndex|=FONT_SERIF_INDEX;
            break;
        case FONT_TYPE_SANS_SERIF:
            defaultFontIndex|=FONT_SANS_SERIF_INDEX;
            break;
        case FONT_TYPE_MONOSPACE:
            defaultFontIndex|=FONT_MONOSPACE_INDEX;
            break;
    }

    if(style & FONT_STYLE_NORMAL) defaultFontIndex|=FONT_NORMAL_INDEX;
    if(style & FONT_STYLE_BOLD) defaultFontIndex|=FONT_BOLD_INDEX;
    if(style & FONT_STYLE_ITALIC) defaultFontIndex|=FONT_ITALIC_INDEX;
    //At this point defaultFontIndex is the combined value of type and style

    if(defaultFontIndex<0 || defaultFontIndex>=FONT_DEFAULT_AMOUNT || gDefaultFontNames[defaultFontIndex]==NULL)
    {
        return RES_FONT_NO_TYPE_STYLE_COMBINATION;
    }

    // We retain the string because we don't want maFontDelete to delete it, as it might be used by other handles
    CFRetain(gDefaultFontNames[defaultFontIndex]);
    return createFontInfo(gDefaultFontNames[defaultFontIndex],(CGFloat)size,NULL,NULL);
}

SYSCALL(MAHandle, maFontLoadWithName(const char* name, int size))
{
    CFStringRef fontName=CFStringCreateWithCString(NULL,name,kCFStringEncodingMacRoman);
    if(size<=0)
    {
        return RES_FONT_INVALID_SIZE;
    }
    //Getting a UIFont object is probably the least expensive way to test if it exists.
    //Also, it's more probably that a user will use the nativeUI system rather than
    //maDrawText(W)
    UIFont *uiFontObject=[UIFont fontWithName:(NSString*)fontName size:(GLfloat)size];

    if(!uiFontObject)
    {
        return RES_FONT_NAME_NONEXISTENT;
    }

    [uiFontObject retain];
    return createFontInfo(fontName,(CGFloat)size,uiFontObject,NULL);
}

SYSCALL(MAHandle, maFontDelete(MAHandle font))
{
    int index=font-1;

    if(index<0 || index>=sFontList.size() || sFontList[index]==NULL)
    {
        return RES_FONT_INVALID_HANDLE;
    }

    FontInfo *toDelete=sFontList[index];

    CFRelease(toDelete->name);
    if(toDelete->uiFontObject)
    {
        [toDelete->uiFontObject release];
    }
    if(toDelete->cgFontObject)
    {
        CGFontRelease(toDelete->cgFontObject);
    }

    delete toDelete;
    sFontList[index]=NULL; //Make sure to create a NULL hole in the vector
    return RES_FONT_OK;
}

SYSCALL(int, maFontGetCount())
{
    if(!gDeviceFontNames) //Lazy initialization
    {
        gDeviceFontNames=[[NSMutableArray alloc] init ];

        //List of font family names. We won't save those, but we need them for now
        NSArray *familyNames=[UIFont familyNames];
        NSEnumerator *familyEnumerator=[[familyNames objectEnumerator] retain];
        NSString *familyName;

        while((familyName=[familyEnumerator nextObject]))
        {
            //These are the names we need
            NSArray *fontNamesInFamily=[UIFont fontNamesForFamilyName:familyName];
            [gDeviceFontNames addObjectsFromArray:fontNamesInFamily];
        }
        [familyEnumerator release];
    }
    return gDeviceFontNames.count;
}

SYSCALL(int, maFontGetName(int index, char* buffer, int bufferLength))
{
    if(!gDeviceFontNames) //maFontGetCount must be called first to initialize this
    {
        return RES_FONT_LIST_NOT_INITIALIZED;
    }
    NSString *fontName=[gDeviceFontNames objectAtIndex:index];

    //buffer must be large enough to hold the string
    //lenghtOfBytes does not include terminating '\0',
    //That's why we use less or equal
    if(!fontName || bufferLength<=[fontName lengthOfBytesUsingEncoding:NSASCIIStringEncoding])
    {
        return RES_FONT_INSUFFICIENT_BUFFER;
    }

    //strncpy will also fill the rest of the buffer with '\0' characters
    strncpy(buffer, [fontName cStringUsingEncoding:NSASCIIStringEncoding], bufferLength);

    //Increase by one for the terminating '\0'
    return [fontName lengthOfBytesUsingEncoding:NSASCIIStringEncoding]+1;
}

SYSCALL(MAHandle, maFontSetCurrent(MAHandle font))
{
    if(font<1||font>sFontList.size()||!sFontList[font-1])
    {
        printf("wrong MAHandle");
        return RES_FONT_INVALID_HANDLE;
    }

    MAHandle prevHandle=gCurrentFontHandle;

    gCurrentFontHandle=font;
    FontInfo *currentFont=sFontList[gCurrentFontHandle-1];

    //Used by maDrawText. It must be set now, unlike the other systems.
    CGContextSelectFont(gDrawTarget->context,
                        CFStringGetCStringPtr(currentFont->name,kCFStringEncodingMacRoman),
                        currentFont->size, kCGEncodingMacRoman);

    return prevHandle;
}
