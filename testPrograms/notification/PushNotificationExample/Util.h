#ifndef UTIL_H_
#define UTIL_H_

#include <maprofile.h>	// Profile database.
#include <mastring.h>		// C string functions

    /**
     * Detects if the current platform is Android.
     * @return true if the platform is Android, false otherwise.
     */
    static bool isAndroid()
    {
		if ( NULL != strstr(MA_PROF_STRING_PLATFORM, "android"))
		{
			return true;
		}
		else
		{
			return false;
		}
    }

#endif /* UTIL_H_ */
