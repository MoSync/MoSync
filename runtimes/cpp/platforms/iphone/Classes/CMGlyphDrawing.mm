/* 
	Replacement for CGFontGetGlyphsForUniChars.
	http://github.com/jamesjhu/CMGlyphDrawing 
*/

#import "CMGlyphDrawing.h"
#import <CoreGraphics/CoreGraphics.h>
#include <dlfcn.h>


UInt16 getUInt16WithByteIndex(CFDataRef data, CFIndex index) {
    UInt16 value = 0;
    CFDataGetBytes(data, CFRangeMake(index, 2), (UInt8 *)&value);
    return CFSwapInt16BigToHost(value);
}

UInt32 getUInt32WithByteIndex(CFDataRef data, CFIndex index) {
    UInt32 value = 0;
    CFDataGetBytes(data, CFRangeMake(index, 4), (UInt8 *)&value);
    return CFSwapInt32BigToHost(value);
}

UInt16 getUInt16(CFDataRef data, CFIndex index) {
    UInt16 value = 0;
    CFDataGetBytes(data, CFRangeMake(index * 2, 2), (UInt8 *)&value);
    return CFSwapInt16BigToHost(value);
}


typedef struct _CMCMap4 {
	UInt16 length;
	UInt16 language;
	UInt16 segCount;
	UInt16 segCountX2;
	UInt16 searchRange;
	UInt16 entrySelector;
	UInt16 rangeShift;
	CFDataRef endCode;
	CFDataRef startCode;
	CFDataRef idDelta;
	CFDataRef idRangeOffset;
	CFDataRef glyphIndexArray;
} CMCMap4;

typedef CMCMap4 * CMCMap4Ref;

static CFMutableDictionaryRef cmapCache = NULL; // Dictionary mapping CMFontRef to _CMCMap
CMCMap4Ref CMGetCMapForFont(CGFontRef cgFont);
CGGlyph CMCMapGetGlyphIndexForUnichar(CMCMap4Ref cmap, UniChar c);

CMCMap4Ref CMCMapCreate(CFDataRef fontTable, UInt32 segmentOffset);
void CMCMapRelease(CMCMap4Ref cmap);


void CMFontGetGlyphsForUnichars(CGFontRef cgFont, const UniChar buffer[], CGGlyph glyphs[], size_t numGlyphs) {
	
	CMCMap4Ref cmap = CMGetCMapForFont(cgFont);
	for (int i = 0; i < numGlyphs; i++) {
		glyphs[i] = CMCMapGetGlyphIndexForUnichar(cmap, buffer[i]);
	}
}


CMCMap4Ref CMGetCMapForFont(CGFontRef cgFont) {
	// Create dictionary to cache cmaps
	if (cmapCache == NULL) {
		CFDictionaryValueCallBacks nonRetainingDictionaryValueCallbacks = kCFTypeDictionaryValueCallBacks;
		nonRetainingDictionaryValueCallbacks.retain = NULL;
		nonRetainingDictionaryValueCallbacks.release = NULL;
		cmapCache = CFDictionaryCreateMutable(NULL, 0, &kCFTypeDictionaryKeyCallBacks, &nonRetainingDictionaryValueCallbacks);
	}
	
	// TODO: Check for memory leaks!
	CFStringRef fontName = CGFontCopyFullName(cgFont);
	// Check dictionary to see if a cmap exists for given font name
	CMCMap4Ref cmap = (CMCMap4Ref)CFDictionaryGetValue(cmapCache, fontName);
	if (cmap != NULL) {
		return cmap;
	}
	
	CFDataRef fontTable = CGFontCopyTableForTag(cgFont, 'cmap');
	
	if (fontTable != NULL) {
		UInt16 version = 0;
		UInt16 subtableCount = 0;

		version = getUInt16WithByteIndex(fontTable, 0);
		subtableCount = getUInt16WithByteIndex(fontTable, 2);

		UInt32 segmentOffset = 0;
		UInt16 platformID = 0;
		// Iterate through all subtables
		for (UInt16 subtableIndex = 0; subtableIndex < subtableCount; ++subtableIndex) {
			
			platformID = getUInt16WithByteIndex(fontTable, subtableIndex * 8 + 4);
			//UInt16 platformSpecificID = getUInt16WithByteIndex(fontTable, subtableIndex * 8 + 6);
			UInt32 offset = getUInt32WithByteIndex(fontTable, subtableIndex * 8 + 8);
			
			if (platformID == 0) {
				// Found unicode mapping.
				segmentOffset = offset;
				break;
			} else if (platformID == 1) {
				// Found mac mapping. Keep looking...
				segmentOffset = offset;
			}
		}

		// Only support Unicode...
		if (platformID == 0) {
			UInt16 format = getUInt16WithByteIndex(fontTable, segmentOffset);
			if(format == 4) {
				cmap = CMCMapCreate(fontTable, segmentOffset);
				// Assign cmap to font name and cache it in the dictionary
				CFDictionaryAddValue(cmapCache, fontName, cmap);
				return cmap;
			}
		}
	}
	
    return NULL;
}

CGGlyph CMCMapGetGlyphIndexForUnichar(CMCMap4Ref cmap, UniChar c) {
	// TODO: Binary search?
	for (int i = 0; i < cmap->segCount; i++) {
		// Find first endcode greater or equal to the char code
		UInt16 end = getUInt16(cmap->endCode, i);
		if (end >= c) {
			UInt16 start = getUInt16(cmap->startCode, i);
			if (start <= c) {
				UInt16 delta = getUInt16(cmap->idDelta, i);
				UInt16 rangeOffset = getUInt16(cmap->idRangeOffset, i);
				if(rangeOffset == 0) {
					return delta + c;
				} else {
					//                                       rangeOffset / 2
					return getUInt16(cmap->glyphIndexArray, (rangeOffset >> 1) + (c - start) - (cmap->segCount - i));
				}
			} else {
				// our code is not within one of this font's segments
				return 0;
			}
		}
	}
	// Epic fail.
	return 0;
}

CMCMap4Ref CMCMapCreate(CFDataRef fontTable, UInt32 segmentOffset) {
	CMCMap4Ref cmap = (CMCMap4Ref)calloc(1, sizeof(CMCMap4));

	cmap->length = getUInt16WithByteIndex(fontTable, segmentOffset + 2);
	cmap->language = getUInt16WithByteIndex(fontTable, segmentOffset + 4);
	cmap->segCountX2 =  getUInt16WithByteIndex(fontTable, segmentOffset + 6);
	cmap->segCount = cmap->segCountX2 / 2;
	cmap->searchRange = getUInt16WithByteIndex(fontTable, segmentOffset + 8);
	cmap->entrySelector = getUInt16WithByteIndex(fontTable, segmentOffset + 10);
	cmap->rangeShift = getUInt16WithByteIndex(fontTable, segmentOffset + 12);

	CFIndex endCodeOffset = segmentOffset + 14;
	cmap->endCode = CFDataCreate(kCFAllocatorDefault, CFDataGetBytePtr(fontTable) + endCodeOffset, cmap->segCountX2);

	CFIndex startCodeOffset = endCodeOffset + 2 + cmap->segCountX2;
	cmap->startCode = CFDataCreate(kCFAllocatorDefault, CFDataGetBytePtr(fontTable) + startCodeOffset, cmap->segCountX2);

	CFIndex idDeltaOffset = startCodeOffset + cmap->segCountX2;        
	cmap->idDelta = CFDataCreate(kCFAllocatorDefault, CFDataGetBytePtr(fontTable) + idDeltaOffset, cmap->segCountX2);

	CFIndex idRangeOffsetOffset = idDeltaOffset + cmap->segCountX2;
	cmap->idRangeOffset = CFDataCreate(kCFAllocatorDefault, CFDataGetBytePtr(fontTable) + idRangeOffsetOffset, cmap->segCountX2);

	CFIndex glyphIndexArrayOffset = idRangeOffsetOffset + cmap->segCountX2;
	cmap->glyphIndexArray = CFDataCreate(kCFAllocatorDefault, CFDataGetBytePtr(fontTable) + glyphIndexArrayOffset, segmentOffset + cmap->length - glyphIndexArrayOffset);

	return cmap;
}

void CMCMapRelease(CMCMap4Ref cmap) {
	// Release CFData references
	CFRelease(cmap->endCode);
    CFRelease(cmap->startCode);
    CFRelease(cmap->idDelta);
    CFRelease(cmap->idRangeOffset);
    CFRelease(cmap->glyphIndexArray);
	// Release struct
	free(cmap);
}
