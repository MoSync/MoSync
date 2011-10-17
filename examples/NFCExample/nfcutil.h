/*
 * nfcutil.h
 *
 *  Created on: Oct 11, 2011
 *      Author: mattias
 */

#ifndef NFCUTIL_H_
#define NFCUTIL_H_

#include <MAUtil/String.h>

class UriNdefRecord {
public:
	UriNdefRecord(MAHandle handle);
	virtual ~UriNdefRecord();
	static bool isValid(MAHandle handle);
	void setUri(MAUtil::String& uri);
	void setUri(int prefixCode, MAUtil::String& remainingUri);
	MAUtil::String getUri();
};
#endif /* NFCUTIL_H_ */
