/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/** \file ErrorListenable.h
* \brief Handling of error events
*/

#ifndef _MAUTIL_ERROR_LISTENABLE_H_
#define _MAUTIL_ERROR_LISTENABLE_H_

#include <MAUtil/String.h>
#include <MAUtil/Vector.h>

namespace MAUtil {

	class ErrorListener {
	public:
		virtual void error(int errorCode, const String& errorString) = 0;
	};

#define FIRE_ERROR_LISTENERS(listeners, errorCode, errorString) \
	Vector_each(ErrorListener*, l, listeners) {\
	(*l)->error(errorCode, errorString);\
	}\

	class ErrorListenable {
	public:
		bool assert(bool assertion, int errorCode, const String& errorString) {
			if(!assertion) {
				FIRE_ERROR_LISTENERS(mErrorListeners, errorCode, errorString);
			}
			return assertion;
		}

		void panic(int errorCode, const String& errorString) {
		}

		void addErrorListener(ErrorListener* errorListener) {
			Vector_each(ErrorListener*, l, mErrorListeners) {
				if(errorListener == *l) {
					return;
				}
			}
			mErrorListeners.add(errorListener);
		}

		void removeErrorListener(ErrorListener* errorListener) {
			Vector_each(ErrorListener*, l, mErrorListeners) {
				if(errorListener == *l) {
					mErrorListeners.remove(l);
					return;
				}
			}
		}

	protected:
		Vector<ErrorListener*> mErrorListeners;
	};

} // namespace MAUtil

#endif /* _MAUTIL_ERROR_LISTENABLE_H_ */
