#include <MAUtil/Vector.h>
#include "PhoneGapCameraUtil.h"

#define TEXT(x) x     //Not unicode
#define DWORD long
// the chunk of data read should be divisible by 3 for avoiding data loss while encoding
#define CHUNK_SIZE 6666

namespace Wormhole
{
	//Lookup table for encoding
	//If you want to use an alternate alphabet, change the characters here
	const static TCHAR encodeLookup[] = TEXT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
	const static char padCharacter[] = "==";
	const static char endline = '\n';

	/**
	 * Reads data from a 'MAHandle' and returns a string representing the data
	 * encoded in Base64.
	 * @param dataHandle The handle poining to the data to be encoded.
	 * @return The data encoded in Base64.
	 */
	String readAndBase64EncodeData(MAHandle dataHandle)
	{
		int dataSize = maGetDataSize(dataHandle);
		String encodedData = "";
		Vector<BYTE> inputVector(CHUNK_SIZE);

		char c[CHUNK_SIZE];
		int offset = 0,
			diff = 0,
			toRead = 0;
		do {
			diff = dataSize - offset;

			if (diff >= CHUNK_SIZE)
			{
				toRead = CHUNK_SIZE;
			}
			else
			{
				toRead = diff;
				inputVector = Vector<BYTE>(toRead);
			}
			maReadData(dataHandle, c, offset, toRead);
			offset+=toRead;

			for (int i = 0; i < toRead; i++)
			{
				inputVector.add(c[i]);
			}

			encodedData += base64Encode(inputVector);

			inputVector.clear();
		} while(offset < dataSize);

		return encodedData;
	}

	/**
	 * Gets a vector of bytes and returns a BasicString containing the Base64
	 * encoded data equivalent.
	 * More information about the Base64 encoding and this method here:
	 * http://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64#C.2B.2B
	 * @param inputBuffer The vector containing the input bytes.
	 * @return A BasicString containing the data encoded in Base64.
	 */
	BasicString<TCHAR> base64Encode(Vector<BYTE> inputBuffer)
	{
	        BasicString<TCHAR> encodedString;

	        encodedString.reserve(((inputBuffer.size()/3) + (inputBuffer.size() % 3 > 0)) * 4);
	        DWORD temp;
	        Vector<BYTE>::iterator cursor = inputBuffer.begin();
	        for(int idx = 0; idx < inputBuffer.size()/3; idx++)
	        {
	                temp  = (*cursor++) << 16; //Convert to big endian
	                temp += (*cursor++) << 8;
	                temp += (*cursor++);
	                encodedString.append(encodeLookup + ((temp & 0x00FC0000) >> 18),1);
	                encodedString.append(encodeLookup + ((temp & 0x0003F000) >> 12),1);
	                encodedString.append(encodeLookup + ((temp & 0x00000FC0) >> 6 ),1);
	                encodedString.append(encodeLookup + ((temp & 0x0000003F)      ),1);
	        }
	        switch(inputBuffer.size() % 3)
	        {
		        case 1:
		                temp  = (*cursor++) << 16; //Convert to big endian
		                encodedString.append(encodeLookup + ((temp & 0x00FC0000) >> 18),1);
		                encodedString.append(encodeLookup + ((temp & 0x0003F000) >> 12),1);
		                encodedString.append(padCharacter, 2);
		                break;
		        case 2:
		                temp  = (*cursor++) << 16; //Convert to big endian
		                temp += (*cursor++) << 8;
		                encodedString.append(encodeLookup + ((temp & 0x00FC0000) >> 18),1);
		                encodedString.append(encodeLookup + ((temp & 0x0003F000) >> 12),1);
		                encodedString.append(encodeLookup + ((temp & 0x00000FC0) >> 6 ),1);
		                encodedString.append(padCharacter,1);
		                break;
	        }

	        return encodedString;
	}
} // end of Wormhole namespace
