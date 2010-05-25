//
//  CMGlyphDrawing.h
//

//#import <Foundation/Foundation.h>
#import <QuartzCore/QuartzCore.h>

extern void CMFontGetGlyphsForUnichars(CGFontRef font, const UniChar buffer[], CGGlyph glyphs[], size_t numGlyphs);