#ifndef WORMHOLE_LIBS_PHONEGAP_PHONEGAPCAMERAUTIL_H_
#define WORMHOLE_LIBS_PHONEGAP_PHONEGAPCAMERAUTIL_H_

#include <conprint.h>
#include <MAUtil/Vector.h>
#include <MAUtil/String.h>

#define TCHAR char    //Not unicode
#define BYTE unsigned char

using namespace MAUtil;
using namespace std;

namespace Wormhole
{
	/**
	 * Reads data from a 'MAHandle' and returns a string representing the data
	 * encoded in Base64.
	 * @param dataHandle The handle poining to the data to be encoded.
	 * @return The data encoded in Base64.
	 */
	String readAndBase64EncodeData(MAHandle dataHandle);

	/**
	 * Gets a vector of bytes and returns a BasicString containing the Base64
	 * encoded data equivalent.
	 * More information about the Base64 encoding and this method here:
	 * http://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64#C.2B.2B
	 * @param inputBuffer The vector containing the input bytes.
	 * @return A BasicString containing the data encoded in Base64.
	 */
	BasicString<TCHAR> base64Encode(Vector<BYTE> inputBuffer);
} // end of Wormhole namespace

#endif // WORMHOLE_LIBS_PHONEGAP_PHONEGAPCAMERAUTIL_H_
