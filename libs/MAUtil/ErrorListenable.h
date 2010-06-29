/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
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
