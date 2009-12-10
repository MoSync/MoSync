// CCODES.CPP +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
// Defines character code mapping tables for conversion between source
// character set and Unicode.
// ----------------------------------------------------------------------------

#include <ctable.h>


S_mapping_range cp1252_exceptions =
{ 128, 159,
{
	0x20AC    // Euro sign
		, 0xFFFE    // not used	(see also UnicodeEscape handling)
		, 0x201A    // single low-9 quotation mark
		, 0x0192    // latin small letter F with hook
		, 0x201E    // double low-9 quotation mark
		, 0x2026    // horizontal ellipsis
		, 0x2020    // dagger
		, 0x2021    // double dagger
		, 0x02C6    // modifier letter circumflex accent
		, 0x2030    // per mille sign
		, 0x0160    // latin capital letter S with caron
		, 0x2039    // single left-pointing angle quotation mark
		, 0x0152    // latin capital ligature oe
		, 0xFFFE    // not used
		, 0x017D    // latin capital letter z with caron
		, 0xFFFE    // not used
		, 0xFFFE    // not used
		, 0x2018    // left single quotation mark
		, 0x2019    // right single quotation mark
		, 0x201C    // left double quotation mark
		, 0x201D    // right double quotation mark
		, 0x2022    // bullet
		, 0x2013    // en dash
		, 0x2014    // em dash
		, 0x02DC    // small tilde
		, 0x2122    // trade mark sign
		, 0x0161    // latin small letter s with caron
		, 0x203A    // single right-pointing angle quotation mark
		, 0x0153    // latin small ligature oe
		, 0xFFFE    // not used
		, 0x017E    // latin small letter z with caron
		, 0x0178    // latin capital letter y with diaeresis
}
};

Mapping_range CP1252_exceptions = &cp1252_exceptions;







// end of CCODES.CPP
