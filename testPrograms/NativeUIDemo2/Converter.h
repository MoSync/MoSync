#ifndef _CONVERTER_H_
#define _CONVERTER_H_

#include <MAUtil/String.h>
#include <MAUtil/Set.h>

namespace Converter {

using MAUtil::String;
using MAUtil::Vector;
using MAUtil::Set;

void init();
const String& convertUnit(const String& category, const String& srcUnit, const String& dstUnit, const String& value);
Vector<String> getCategories();
Vector<String> getUnitsInCategory(const String& category);


}

#endif // _CONVERTER_H_
