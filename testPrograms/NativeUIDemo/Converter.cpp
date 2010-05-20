#include "Converter.h"
#include <MAUtil/util.h>
#include <MAUtil/String.h>
#include <MAUtil/Vector.h>
#include <MAUtil/Map.h>
#include <MAUtil/Set.h>

namespace Converter {

using namespace MAUtil;

class UnitConverter {
public:
	UnitConverter(const String& srcUnit, const String& dstUnit)
		: mSrcUnit(srcUnit), mDstUnit(dstUnit) {}

	const String& getSrcUnit() const {
		return mSrcUnit;
	}

	const String& getDstUnit() const {
		return mDstUnit;
	}
	virtual const String& srcToDst(const String& value) {
		convertSrcToDst(value);
		strRes = doubleToString(result);
		return strRes;
	}
	virtual const String& dstToSrc(const String& value) {
		convertDstToSrc(value);
		strRes = doubleToString(result);
		return strRes;
	}
protected:
	virtual void convertSrcToDst(const String& value) = 0;
	virtual void convertDstToSrc(const String& value) = 0;
	String mSrcUnit;
	String mDstUnit;
	double result;
	String strRes;
};

class CelsiusFarenheitConverter : public UnitConverter {
public:
	CelsiusFarenheitConverter() : UnitConverter("C", "F") {}
protected:
	void convertSrcToDst(const String& value) {
		double celsius = stringToDouble(value);
		result = (celsius * 9.0/5.0) + 32.0;
	}
	void convertDstToSrc(const String& value) {
		double farenheit = stringToDouble(value);
		result = (farenheit - 32.0) * 5.0/9.0;
	}
};

class CelsiusKelvinConverter : public UnitConverter {
public:
	CelsiusKelvinConverter() : UnitConverter("C", "K") {}
protected:
	void convertSrcToDst(const String& value) {
		double celsius = stringToDouble(value);
		result = celsius + 273.15;
	}
	void convertDstToSrc(const String& value) {
		double kelvin = stringToDouble(value);
		result = kelvin - 273.15;
	}
};

class FarenheitKelvinConverter : public UnitConverter {
public:
	FarenheitKelvinConverter() : UnitConverter("F", "K") {}
protected:
	void convertSrcToDst(const String& value) {
		double farenheit = stringToDouble(value);
		result = ((farenheit - 32.0) * 5.0/9.0) + 273.15;
	}
	void convertDstToSrc(const String& value) {
		double kelvin = stringToDouble(value);
		result = ((kelvin - 273.15) - 9.0/5.0) + 32.0;
	}
};

class KilogramOunceConverter : public UnitConverter {
public:
	KilogramOunceConverter() : UnitConverter("kg", "ounces") {}
protected:
	void convertSrcToDst(const String& value) {
		double kg = stringToDouble(value);
		result = kg * 35.2739619;
	}
	void convertDstToSrc(const String& value) {
		double ounces = stringToDouble(value);
		result = ounces / 35.2739619;
	}
};

class KilogramPoundConverter : public UnitConverter {
public:
	KilogramPoundConverter() : UnitConverter("kg", "lbs") {}
protected:
	void convertSrcToDst(const String& value) {
		double kg = stringToDouble(value);
		result = kg * 2.2;
	}
	void convertDstToSrc(const String& value) {
		double lbs = stringToDouble(value);
		result = lbs / 2.2;
	}
};

Map<String,Vector<UnitConverter*> > converters;

void init() {
	converters["Temperature"].add(new CelsiusFarenheitConverter());
	converters["Temperature"].add(new CelsiusKelvinConverter());
	converters["Temperature"].add(new FarenheitKelvinConverter());
	converters["Mass"].add(new KilogramOunceConverter());
	converters["Mass"].add(new KilogramPoundConverter());
}

const String& convertUnit(const String& category, const String& srcUnit, const String& dstUnit, const String& value) {
	Vector<UnitConverter*> conv = converters[category];

	for(int i = 0; i < conv.size(); i++) {
		if(conv[i]->getSrcUnit() == srcUnit && conv[i]->getDstUnit() == dstUnit) {
			return conv[i]->srcToDst(value);
		}
		else if(conv[i]->getSrcUnit() == dstUnit && conv[i]->getDstUnit() == srcUnit) {
			return conv[i]->dstToSrc(value);
		}
	}
}

Vector<String> getCategories() {
	Vector<String> ret;
	for(Map<String, Vector<UnitConverter*> >::ConstIterator i = converters.begin(); i != converters.end(); ++i) {
		String key = (*i).first;
		ret.add(key);
	}
	return ret;
}

Set<String> getUnitsInCategory(const String& category) {
	Vector<UnitConverter*> c = converters[category];
	Set<String> ret;
	for(int i = 0; i < c.size(); i++) {
		ret.insert(c[i]->getSrcUnit());
		ret.insert(c[i]->getDstUnit());
	}
	return ret;
}


}
