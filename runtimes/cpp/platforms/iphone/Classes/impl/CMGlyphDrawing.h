//
//  CMGlyphDrawing.h
//

#import <QuartzCore/QuartzCore.h>

extern int CMFontGetGlyphsForUnichars(CGFontRef font, const UniChar buffer[], CGGlyph glyphs[], size_t numGlyphs);

// temporary fix for native library approach
extern int CMFontGetGlyphsForUnichars(CGFontRef font, const UInt32 buffer[], CGGlyph glyphs[], size_t numGlyphs);